# Athan Player

An Arduino project that reads daily prayer times from an SD card, displays the date & time on an LCD, and plays the Athan when it’s prayer time.
This entire project is encapsulated in a case that was 3D modeled.

## 📁 Repository Contents

- **AlarmInitializer.ino**  
  The main Arduino sketch.  
  - Initializes RTC, LCD, SD card & audio  
  - Loads today’s 5 prayer times from `prayer.txt`  
  - Displays current time on the top LCD row  
  - Checks once per minute for a prayer match  
  - On match: plays `Azan.wav` and shows “Time for (prayer)” for 5 min  
  - Otherwise shows the date on the bottom row

- **PrayerTimeDB.ino**  
  A file containing all prayer times for the current year, only needs to be ran once.  
  You’d run this once to generate `prayer.txt`.

- **FinalCode.ino**  
  A previous iteration—kept here for reference.  
  *You can ignore or delete this once you’re comfortable with `AlarmInitializer.ino`.*

## How to run

After setting up the hardware, download the arduino IDE and clone the project. 
After cloning, open the files and after inserting the SD card, run the PrayerTimeDB.ino file to populate all the prayer times in the SD card.
Next Run the AlarmInitializer and you're all set. Once a prayer time has been reached, the athan will play and a message for prayer will be displayed
on the screen for 5 minutes.

*Note: This only works in Calgary as the prayer times have been hardcoded. Future updates may involve a direct API call that can fetch prayer times and can work for various regions*