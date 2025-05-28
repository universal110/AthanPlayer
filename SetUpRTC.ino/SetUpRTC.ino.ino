#include <Wire.h>
#include <DS3231.h>

DS3231 rtc;
bool century = false;
bool h12Flag;
bool pmFlag;

void setup() {
  Wire.begin();
  Serial.begin(57600);

  // Set time once (year, month, day, hour, minute, second)
  // Remove or comment this after the time is set to avoid resetting every boot
  rtc.setClockMode(false); // false = 24h, true = 12h
  rtc.setYear(25);         // 2025 (only last two digits)
  rtc.setMonth(5);         // May
  rtc.setDate(19);         // 19th
  rtc.setHour(12);
  rtc.setMinute(0);
  rtc.setSecond(0);

  Serial.println("RTC time set!");
}

void loop() {
  Serial.print("Date: 20");
  Serial.print(rtc.getYear(), DEC); Serial.print("-");
  Serial.print(rtc.getMonth(century), DEC); Serial.print("-");
  Serial.print(rtc.getDate(), DEC); Serial.print(" ");

  int hour = rtc.getHour(h12Flag, pmFlag);
  Serial.print("Time: ");
  Serial.print(hour); Serial.print(":");
  Serial.print(rtc.getMinute()); Serial.print(":");
  Serial.print(rtc.getSecond());

  if (h12Flag) {
    Serial.print(pmFlag ? " PM " : " AM ");
  } else {
    Serial.print(" 24h ");
  }

  Serial.print(" | Temp: ");
  Serial.print(rtc.getTemperature(), 2);
  Serial.println(" °C");

  delay(1000);
}