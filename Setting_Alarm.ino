#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;  // CS pin for SD card module

void setup() {
  Serial.begin(9600);
  while (!Serial);  // Wait for Serial Monitor (needed for Leonardo/Micro)

  Serial.println("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card failed or not present.");
    while (1);
  }
  Serial.println("SD card initialized.");

  Serial.println("Enter alarm time in HH:MM (24-hour format):");
}

void loop() {
  static String input = "";
  if (Serial.available()) {
    char c = Serial.read();
    
    if (c == '\n' || c == '\r') {
      if (input.length() >= 4 && input.indexOf(':') == 2) {
        // Save to SD
        File alarmFile = SD.open("alarm.txt", FILE_WRITE);
        if (alarmFile) {
          alarmFile.println(input);
          // alarmFile.print(":00"); // Set seconds to 0 to prevent errors
          alarmFile.close();
          Serial.println("Alarm saved to alarm.txt: " + input);
        } else {
          Serial.println("Error opening alarm.txt");
        }
      } else {
        Serial.println("Invalid format. Use HH:MM (e.g., 18:40)");
      }
      input = ""; // Clear input for next entry
      Serial.println("Enter new alarm time:");
    } else {
      input += c;
    }
  }
}