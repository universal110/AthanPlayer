#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <TMRpcm.h>

#define PRAYER_COUNT 5
#define SD_CS_PIN    10
#define AZAN_FILE    "Azan.wav"
#define PRAYER_FILE  "prayer.txt"

// ——— hardware objects —————————————————————————————————————————————
DS3231 myRTC;
LiquidCrystal_I2C lcd(0x27, 16, 2);
TMRpcm audio;

// ——— flags for 12/24h mode ————————————————————
bool century = false;
bool h12Flag;
bool pmFlag;

// ——— prayer data —————————————————————————————————————————————————————
const char* prayers[PRAYER_COUNT] = { "Fajr", "Dhuhr", "Asr", "Maghrib", "Isha" };
int prayerHour[PRAYER_COUNT];
int prayerMin [PRAYER_COUNT];

// ——— run-time state ——————————————————————————————————————————————————
bool          inPrayer    = false;
unsigned long prayerEnd   = 0;
const char*   currentName = nullptr;

// ——— helper to load today’s 5 times from “prayer.txt” ————————————————————
bool loadPrayers() {
  // read time from RTC
  int year   = myRTC.getYear();
  int month  = myRTC.getMonth(century);
  int date   = myRTC.getDate();

  File f = SD.open(PRAYER_FILE);
  if (!f) return false;

  // build “YYYY-MM-DD”
  char today[11];
  sprintf(today, "20%02d-%02d-%02d", year, month, date);

  // scan file line-by-line
  while (f.available()) {
    String line = f.readStringUntil('\n');
    line.trim();
    if (!line.startsWith(today)) continue;

    // parse the 5 comma-separated times
    int idx = strlen(today) + 1;
    for (int i = 0; i < PRAYER_COUNT; i++) {
      int comma = line.indexOf(',', idx);
      if (comma < 0) comma = line.length();
      String t = line.substring(idx, comma);
      t.trim();
      int colon = t.indexOf(':');
      prayerHour[i] = t.substring(0, colon).toInt();
      prayerMin [i] = t.substring(colon + 1).toInt();
      idx = comma + 1;
    }
    f.close();
    return true;
  }

  f.close();
  return false;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // ——— manually set time/date ————————————————
  // Uncomment these lines ONCE to set time, then reupload with them commented
  // myRTC.setClockMode(false); // false = 24h format
  // myRTC.setYear(25);         // last two digits → 2025
  // myRTC.setMonth(10);        // October
  // myRTC.setDate(15);         // 15th
  // myRTC.setHour(22);         // 22 = 10:00 PM
  // myRTC.setMinute(0);        // 00 minutes
  // myRTC.setSecond(0);        // 00 seconds
  // ——————————————————————————————————————————————

  // — LCD setup
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Loading Times");
  delay(500);

  // — SD card
  if (!SD.begin(SD_CS_PIN)) {
    lcd.clear();
    lcd.print("SD init failed");
    while (true);
  }

  // — load today’s prayer times
  if (!loadPrayers()) {
    lcd.clear();
    lcd.print("No Times Found");
    while (true);
  }

  // — audio
  audio.speakerPin = 9;
  audio.setVolume(5);

  lcd.clear();
}

void loop() {
  // read current time from DS3231
  int year   = myRTC.getYear();
  int month  = myRTC.getMonth(century);
  int date   = myRTC.getDate();
  int hour   = myRTC.getHour(h12Flag, pmFlag);
  int minute = myRTC.getMinute();
  int second = myRTC.getSecond();

  // — top line: current time
  lcd.setCursor(0, 0);
  if (hour < 10) lcd.print('0');
  lcd.print(hour);
  lcd.print(':');
  if (minute < 10) lcd.print('0');
  lcd.print(minute);
  lcd.print(':');
  if (second < 10) lcd.print('0');
  lcd.print(second);
  lcd.print("   "); // clear remainder of line

  // — check if it's prayer time
  if (!inPrayer) {
    for (int i = 0; i < PRAYER_COUNT; i++) {
      if (hour == prayerHour[i] && minute == prayerMin[i]) {
        inPrayer    = true;
        prayerEnd   = millis() + 5UL * 60UL * 1000UL;  // 5 minutes
        currentName = prayers[i];
        Serial.print("Time for "); Serial.println(currentName);
        if (SD.exists(AZAN_FILE)) audio.play(AZAN_FILE);
        break;
      }
    }
  }

  // — bottom line: date or prayer name
  lcd.setCursor(0, 1);
  lcd.print("                "); // clear line
  lcd.setCursor(0, 1);

  if (inPrayer && millis() < prayerEnd) {
    lcd.print("Time for ");
    lcd.print(currentName);
  } else {
    inPrayer = false;
    char dateBuf[11];
    sprintf(dateBuf, "%02d/%02d/20%02d", month, date, year);
    lcd.print(dateBuf);
  }

  delay(100);
}
