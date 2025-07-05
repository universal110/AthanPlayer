#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <TMRpcm.h>

#define PRAYER_COUNT 5
#define SD_CS_PIN    10
#define AZAN_FILE    "Azan.wav"
#define PRAYER_FILE  "prayer.txt"

// ——— hardware objects —————————————————————————————————————————————
RTC_DS3231        rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
TMRpcm            audio;

// ——— prayer data —————————————————————————————————————————————————————
const char* prayers[PRAYER_COUNT] = { "Fajr", "Dhuhr", "Asr", "Maghrib", "Isha" };
int         prayerHour[PRAYER_COUNT];
int         prayerMin [PRAYER_COUNT];

// ——— run-time state ——————————————————————————————————————————————————
bool          inPrayer    = false;
unsigned long prayerEnd   = 0;
const char*   currentName = nullptr;

// ——— helper to load today’s 5 times from “prayer.txt” ————————————————————
bool loadPrayers() {
  File f = SD.open(PRAYER_FILE);
  if (!f) return false;

  // build “YYYY-MM-DD”
  DateTime now;
  now = rtc.now();
  char today[11];
  sprintf(today, "%04d-%02d-%02d", now.year(), now.month(), now.day());

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
  // — init Serial, RTC, I2C bus
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();

  // — show splash on LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Loading Times");
  delay(1000);

  // — init SD card
  if (!SD.begin(SD_CS_PIN)) {
    lcd.clear();
    lcd.print("SD init failed");
    while (true);  // stop here
  }

  // — load today’s prayer times (5 entries)
  if (!loadPrayers()) {
    lcd.clear();
    lcd.print("No Times Found");
    while (true);
  }

  // — configure audio output
  audio.speakerPin = 9;
  audio.setVolume(5);

  lcd.clear();
}

void loop() {
  // — read clock
  DateTime now = rtc.now();

  // — top line: hh:mm:ss
  char timeBuf[9];
  sprintf(timeBuf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  lcd.setCursor(0, 0);
  lcd.print(timeBuf);

  // — check once if this minute matches a prayer
  if (!inPrayer) {
    for (int i = 0; i < PRAYER_COUNT; i++) {
      if (now.hour() == prayerHour[i]
       && now.minute() == prayerMin [i]) {
        inPrayer    = true;
        prayerEnd   = millis() + 5UL*60UL*1000UL;  // 5 minutes
        currentName = prayers[i];
        Serial.print("Time for "); Serial.println(currentName);
        if (SD.exists(AZAN_FILE)) audio.play(AZAN_FILE);
        break;
      }
    }
  }

  // — bottom line: either “Time for X” or date
  lcd.setCursor(0, 1);
  if (inPrayer && millis() < prayerEnd) {
    lcd.print("Time for ");
    lcd.print(currentName);
  } else {
    inPrayer = false;
    char dateBuf[11];
    sprintf(dateBuf, "%04d-%02d-%02d", now.year(), now.month(), now.day());
    lcd.print(dateBuf);
  }

  delay(250);
}
