#include <Wire.h>
#include <RTClib.h>               // <-- pull in adjust() and DateTime
#include <LiquidCrystal_I2C.h>
#include <TMRpcm.h>               // Speaker Library
#include <SD.h>                   // SD Card Library

RTC_DS3231 rtc;                  // from RTClib
TMRpcm        audio;
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // bring up the RTC chip
  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

  // if the battery died, set it to the compile-time clock
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting to compile time...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // LCD init
  if (!i2CAddrTest(0x27)) {
    lcd = LiquidCrystal_I2C(0x3F,16,2);
  }
  lcd.init();
  lcd.backlight();

  // show the date immediately
  DateTime now = rtc.now();
  lcd.setCursor(0,0);
  lcd.print(now.year(), DEC); lcd.print('-');
  lcd.print(now.month(), DEC); lcd.print('-');
  lcd.print(now.day(), DEC);

  // SD & audio
  if (!SD.begin(10)) {
    Serial.println("SD fail");
  } else {
    audio.speakerPin = 9;
    audio.setVolume(6);
    audio.play("AzanOne.wav");
  }
}

void loop() {
  // read time each second
  DateTime now = rtc.now();

  // send to serial
  Serial.print("Date: ");
  Serial.print(now.year()); Serial.print('-');
  Serial.print(now.month()); Serial.print('-');
  Serial.println(now.day());

  Serial.print("Time: ");
  Serial.print(now.hour()); Serial.print(':');
  Serial.print(now.minute()); Serial.print(':');
  Serial.println(now.second());

  // display date
  lcd.setCursor(0,0);
  lcd.print(now.year()); lcd.print('-');
  lcd.print(now.month()); lcd.print('-');
  lcd.print(now.day());

  // display time
  lcd.setCursor(0,1);
  if (now.hour()  < 10) lcd.print('0');
  lcd.print(now.hour());   lcd.print(':');
  if (now.minute() < 10) lcd.print('0');
  lcd.print(now.minute()); lcd.print(':');
  if (now.second() < 10) lcd.print('0');
  lcd.print(now.second());

  delay(1000);
}

// check for a live LCD
bool i2CAddrTest(uint8_t addr) {
  Wire.begin();
  Wire.beginTransmission(addr);
  return (Wire.endTransmission() == 0);
}
