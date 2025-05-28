#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Delete all files in the root directory
  deleteAllFiles(SD.open("/"));

  Serial.println("All files deleted.");
}

void loop() {
  // Nothing here
}

void deleteAllFiles(File dir) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // No more files
      break;
    }

    String filename = entry.name();
    if (!entry.isDirectory()) {
      Serial.print("Deleting file: ");
      Serial.println(filename);
      SD.remove(filename.c_str());
    }
    entry.close();
  }
}