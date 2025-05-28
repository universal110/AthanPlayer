#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
#include <TimeLib.h>

// Objects
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// SD card chip select
const int chipSelect = 10;

// Alarm time
int alarmHour = 8;
int alarmMinute = 21;
int alarmSecond = 00;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // LCD
  lcd.init();
  lcd.backlight();

  // RTC
  if (!rtc.begin()) {
    lcd.print("RTC not found!");
    while (1);
  }

  // SD Card
  if (!SD.begin(chipSelect)) {
    lcd.print("SD fail");
    while (1);
  }

  // Read alarm time from SD
  File alarmFile = SD.open("alarm.txt");
  if (alarmFile) {
    String timeStr = alarmFile.readStringUntil('\n');
    alarmFile.close();
    parseAlarmTime(timeStr);
  } else {
    lcd.print("alarm.txt error");
  }

  lcd.setCursor(0, 0);
  lcd.print("Alarm set:");
  lcd.setCursor(0, 1);
  lcd.print(String(alarmHour) + ":" + (alarmMinute < 10 ? "0" : "") + String(alarmMinute) + (alarmSecond < 60 ? "0" : "") + String(alarmSecond));
  delay(3000);
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now();
 
  lcd.setCursor(0, 0);
     lcd.print("Time:");
    lcd.print(now.hour());
    lcd.print(":");
    if (now.minute() < 10) lcd.print("0");
    lcd.print(now.minute());
    lcd.print(":");
    if (now.second() < 60) lcd.print("0");
    lcd.print(now.second());


  // Check if current time matches alarm
  if (now.hour() == alarmHour && now.minute() == alarmMinute) {
    lcd.setCursor(0, 1);
    lcd.print("Asr prayer time");
    delay(5000); // This delays the clock as well
  } else {
    lcd.setCursor(0, 1);
   // lcd.print("                ");  // Clear line
    lcd.print("Date: ");
    lcd.print(now.year());  lcd.print("-");
    lcd.print(now.month()); lcd.print("-");
    lcd.print(now.day());


  }

  delay(1000);
}

// Parse "HH:MM" from SD card
void parseAlarmTime(String timeStr) {
  int colonIndex = timeStr.indexOf(':');
  if (colonIndex > 0) {
    alarmHour = timeStr.substring(0, colonIndex).toInt();
    alarmMinute = timeStr.substring(colonIndex + 1).toInt();
  }
}