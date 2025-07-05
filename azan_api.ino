#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!SD.begin(chipSelect)) {
    Serial.println("SD card failed to initialize.");
    return;
  }

  Serial.println("SD card initialized!");

  File testFile = SD.open("prayer.txt", FILE_WRITE);
  if (testFile) {
    testFile.println("2025-06-12, 03:36, 13:35, 18:01, 21:39, 21:43");
    testFile.println("2025-06-13, 03:37, 13:35, 18:01, 21:31, 23:31");
    testFile.println("2025-06-14, 03:38, 13:35, 18:01, 21:32, 23:32");
    testFile.println("2025-06-15, 03:40, 13:35, 18:02, 21:33, 23:33");
    testFile.println("2025-06-16, 03:41, 13:36, 18:02, 21:34, 23:34");
    testFile.println("2025-06-17, 03:42, 13:36, 18:03, 21:36, 23:36");
    testFile.println("2025-06-18, 03:43, 13:36, 18:04, 21:37, 23:37");
    testFile.println("2025-07-01, 03:36, 13:35, 18:01, 21:39, 21:43");
    testFile.println("2025-07-02, 03:37, 17:00, 17:50, 17:55, 17:35");
    testFile.println("2025-07-03, 03:36, 13:35, 18:01, 21:39, 21:43");
    testFile.println("2025-07-04, 03:37, 13:35, 18:01, 21:31, 23:31");
    // testFile.println("2025-06-20, 03:36 AM, 01:39 PM, 06:01 PM, 09:56 PM, 11:39 PM");
    // testFile.println("2025-05-21, 03:43 AM, 01:34 PM, 05:49 PM, 09:30 PM, 11:23 PM");
    // testFile.println("2025-06-21, 03:36 AM, 01:39 PM, 06:01 PM, 09:56 PM, 11:39 PM");
    // testFile.println("2025-05-22, 03:42 AM, 01:34 PM, 05:49 PM, 09:31 PM, 11:24 PM");
    // testFile.println("2025-06-22, 03:37 AM, 01:39 PM, 06:01 PM, 09:56 PM, 11:40 PM");
    // testFile.println("2025-05-23, 03:42 AM, 01:34 PM, 05:50 PM, 09:33 PM, 11:25 PM");
    // testFile.println("2025-06-23, 03:37 AM, 01:39 PM, 06:01 PM, 09:56 PM, 11:40 PM");
    // testFile.println("2025-05-24, "03:41 AM", "01:34 PM", "05:50 PM", "09:34 PM", "11:26 PM");
    // testFile.println("2025-06-24, "03:37 AM", "01:40 PM", "06:01 PM", "09:56 PM", "11:40 PM");
    // testFile.println("2025-05-25, "03:41 AM", "01:34 PM", "05:51 PM", "09:35 PM", "11:26 PM");
    // testFile.println("2025-06-25, "03:38 AM", "01:40 PM", "06:02 PM", "09:56 PM", "11:40 PM");
    // testFile.println("2025-05-26", "03:40 AM", "01:34 PM", "05:51 PM", "09:36 PM", "11:27 PM");
    // testFile.println("2025-06-26", "03:38 AM", "01:40 PM", "06:02 PM", "09:56 PM", "11:40 PM");
    // testFile.println("2025-05-27", "03:40 AM", "01:34 PM", "05:52 PM", "09:38 PM", "11:28 PM");
    // testFile.println("2025-06-27", "03:38 AM", "01:40 PM", "06:02 PM", "09:56 PM", "11:40 PM");
    // testFile.println("2025-05-28", "03:40 AM", "01:34 PM", "05:52 PM", "09:38 PM", "11:28 PM");
    // testFile.println("2025-06-28", "03:39 AM", "01:40 PM", "06:02 PM", "09:56 PM", "11:40 PM");
    // testFile.println("2025-05-29", "03:39 AM", "01:34 PM", "05:52 PM", "09:39 PM", "11:28 PM");
    // testFile.println("2025-06-29", "03:39 AM", "01:41 PM", "06:02 PM", "09:56 PM", "11:40 PM");
    // testFile.println("2025-05-30", "03:38 AM", "01:35 PM", "05:53 PM", "09:41 PM", "11:30 PM");
    // testFile.println("2025-06-30", "03:39 AM", "01:41 PM", "06:02 PM", "09:55 PM", "11:40 PM");
    // testFile.println("2025-05-31", "03:38 AM", "01:35 PM", "05:54 PM", "09:42 PM", "11:30 PM");


    testFile.close();
    Serial.println("File written successfully!");
  } else {
    Serial.println("Failed to open file for writing.");
  }
}

void loop() {}
