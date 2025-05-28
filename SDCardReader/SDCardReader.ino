#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

void setup() {
  Serial.begin(57600);
  while (!Serial); // For native USB boards
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed or not present."); // Prints this if SD card is not detected
    return;
  }

  Serial.println("Card initialized.");

  // Write to file
  File writeFile = SD.open("data.txt", FILE_WRITE);
  if (writeFile) {
    writeFile.println("Hello from Arduino!");     // Writes this message into the SD card
    writeFile.println("Data log entry: 1");       // Writes this message into the SD card
    writeFile.close();
    Serial.println("Data written to data.txt");
  } else {
    Serial.println("Failed to open data.txt for writing."); // Prints this if the Nano can't read the SD card
  }

  // Read from file
  File readFile = SD.open("data.txt");
  if (readFile) {
    Serial.println("Reading data.txt:");
    while (readFile.available()) {
      Serial.write(readFile.read());              // Reads from the SD card and prints it to the terminal
    }
    readFile.close();
  } else {
    Serial.println("Failed to open data.txt for reading."); // Prints this if the Nano can't read the SD card
  }
}

void loop() {
  // Nothing in loop
}