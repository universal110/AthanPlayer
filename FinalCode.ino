#include <TimeLib.h>

#include <LiquidCrystal_I2C.h>

#include <Wire.h>
#include <DS3231.h>


#include <TMRpcm.h> // Speaker Library
#include <SD.h> // SD Card Library

TMRpcm audio;
DS3231 rtc;
bool century = false;
//bool h12Flag;
//bool pmFlag;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Wire.begin();

  bool h12Flag, pmFlag, centuryFlag;

  byte hour = rtc.getHour(h12Flag, pmFlag);
  byte minute = rtc.getMinute();
  byte second = rtc.getSecond();
  byte date = rtc.getDate();
  byte month = rtc.getMonth(centuryFlag);
  byte year = rtc.getYear() + 2000;

  setTime(hour, minute, second, date, month, year);

  Serial.begin(9600);

  // Set time once (year, month, day, hour, minute, second)
  // Remove or comment this after the time is set to avoid resetting every boot
  // rtc.setClockMode(false); // false = 24h, true = 12h
  // rtc.setYear(25); // 2025 (only last two digits)
  // rtc.setMonth(6); // May
  // rtc.setDate(30); // 23rd
  // rtc.setHour(18);
  // rtc.setMinute(28);
  // rtc.setSecond(0);

  // Serial.println("RTC time set!");

  if (!i2CAddrTest(0x27)) {
  lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  }
  lcd.init(); // LCD driver initialization
  lcd.backlight(); // Open the backlight
  lcd.setCursor(0,0); // Move the cursor to row 0, column 0

  lcd.print("20");
  lcd.print(rtc.getYear(), DEC); lcd.print("-");
  lcd.print(rtc.getMonth(century), DEC); lcd.print("-");
  lcd.print(rtc.getDate(), DEC); lcd.print(" ");

  if (!SD.begin(10)) {                  // Opening SD Card
    Serial.println("SD card failed!");
    return;
  }

  audio.speakerPin = 9; // Same output pin
  audio.setVolume(6);
  audio.play("AzanOne.wav"); // Must be 8-bit unsigned, mono WAV
}

void loop() {
static unsigned long lastSync = 0;
const unsigned long syncInterval = 60000; // Sync RTC time every 60 seconds

// Declare bool variables to pass by reference to DS3231 functions
bool h12Flag, pmFlag, centuryFlag;

// Sync time from RTC to TimeLib every syncInterval milliseconds
if (millis() - lastSync > syncInterval) {
lastSync = millis();

byte hour = rtc.getHour(h12Flag, pmFlag);
byte minute = rtc.getMinute();
byte second = rtc.getSecond();
byte date = rtc.getDate();
byte month = rtc.getMonth(centuryFlag);
byte year = rtc.getYear() + 2000;

setTime(hour, minute, second, date, month, year);
}

// Read current time from TimeLib
int currentYear = year();
int currentMonth = month();
int currentDay = day();
int currentHour = hour();
int currentMinute = minute();
int currentSecond = second();

// Print date and time to Serial
Serial.print("Date: ");
Serial.print(currentYear); Serial.print("-");
Serial.print(currentMonth); Serial.print("-");
Serial.println(currentDay);

Serial.print("Time: ");
Serial.print(currentHour); Serial.print(":");
Serial.print(currentMinute); Serial.print(":");
Serial.println(currentSecond);

// Display date on LCD (first row)
lcd.setCursor(0, 0);
lcd.print(currentYear); lcd.print("-");
lcd.print(currentMonth); lcd.print("-");
lcd.print(currentDay);

// Display time on LCD (second row)
lcd.setCursor(0, 1);
if(currentHour < 10) lcd.print("0");
lcd.print(currentHour); lcd.print(":");
if(currentMinute < 10) lcd.print("0");
lcd.print(currentMinute); lcd.print(":");
if(currentSecond < 10) lcd.print("0");
lcd.print(currentSecond);

delay(1000); // Update every second
}



bool i2CAddrTest(uint8_t addr) {
Wire.begin();
Wire.beginTransmission(addr);
if (Wire.endTransmission() == 0) {
return true;
}
return false;
}