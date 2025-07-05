#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

// Hardware setup
RTC_DS3231      rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int       chipSelect = 10;
TMRpcm          audio;

// Prayer info
const char*     prayerNames[5] = { "Fajr", "Dhuhr", "Asr", "Maghrib", "Isha" };
int             prayerHour[5], prayerMinute[5];

// state for the five-minute display
bool            prayerActive    = false;
unsigned long   prayerEndMillis = 0;
String          currentPrayer;

// forward declaration
bool loadTodayPrayerTimes();

void setup() {
  Wire.begin();
  rtc.begin();

  Serial.begin(9600);
  while (!Serial) { delay(10); } // **wait** for the Serial monitor to open

  // LCD splash
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Loading Times");
  delay(1500);

  // SD card init
  if (!SD.begin(chipSelect)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SD init failed");
    Serial.println("SD init failed");
    while (true);
  }

  // load today's file
  if (!loadTodayPrayerTimes()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Times Today");
    Serial.println("No prayer times found for today");
    while (true);
  }

  // dump what we found to Serial
  Serial.println("Loaded today's prayer times:");
  for (int i = 0; i < 5; i++) {
    Serial.print("   ");
    Serial.print(prayerNames[i]);
    Serial.print(": ");
    Serial.print(prayerHour[i]);
    Serial.print(":");
    if (prayerMinute[i] < 10) Serial.print('0');
    Serial.println(prayerMinute[i]);
  }

  lcd.clear();
}

void loop() {
  DateTime now = rtc.now();

  // --- top row: current clock ---
  char timeBuf[9];
  sprintf(timeBuf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  lcd.setCursor(0, 0);
  lcd.print(timeBuf);
  lcd.print("   ");  // wipe any leftovers

  // --- check prayer match once ---
  if (!prayerActive) {
    for (int i = 0; i < 5; i++) {
      if (now.hour() == prayerHour[i] && now.minute() == prayerMinute[i]) {
        prayerActive    = true;
        currentPrayer   = prayerNames[i];
        prayerEndMillis = millis() + 5UL * 60UL * 1000UL;
        Serial.print("Time for ");
        Serial.println(currentPrayer);
        if (SD.exists("AzanOne.wav")) {
            audio.speakerPin = 9;
            audio.setVolume(6);
            audio.play("AzanOne.wav");
        } else {
          Serial.println("Azan1.wav not found");
        }
        break;
      }
    }
  }

  // --- bottom row: either “Time for X” or date ---
  lcd.setCursor(0, 1);
  if (prayerActive && millis() <= prayerEndMillis) {
    lcd.print("Time for ");
    lcd.print(currentPrayer);
    lcd.print("   ");
  } else {
    prayerActive = false; // reset once expired
    char dateBuf[11];
    sprintf(dateBuf, "%04d-%02d-%02d", now.year(), now.month(), now.day());
    lcd.print(dateBuf);
    lcd.print("   ");
  }

  delay(250);
}

bool loadTodayPrayerTimes() {
  File file = SD.open("prayer.txt", FILE_READ);
  if (!file) {
    Serial.println("Failed to open prayer.txt");
    return false;
  }

  // build “YYYY-MM-DD”
  DateTime now = rtc.now();
  char today[11];
  sprintf(today, "%04d-%02d-%02d", now.year(), now.month(), now.day());
  Serial.print("Searching for: ");
  Serial.println(today);

  // scan each line
  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    Serial.print("> "); 
    Serial.println(line);
    if (line.startsWith(today)) {
      // skip the comma
      int idx = strlen(today) + 1;
      for (int i = 0; i < 5; i++) {
        int comma = line.indexOf(',', idx);
        if (comma < 0) comma = line.length();
        String t = line.substring(idx, comma);
        t.trim();
        int colon = t.indexOf(':');
        prayerHour[i]   = t.substring(0, colon).toInt();
        prayerMinute[i] = t.substring(colon + 1).toInt();
        idx = comma + 1;
      }
      file.close();
      return true;
    }
  }

  file.close();
  return false;
}
