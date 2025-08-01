#include <TMRpcm.h>
#include <SD.h>

TMRpcm audio;

void setup() {
  if (!SD.begin(10)) {
    Serial.println("SD card failed!");
    return;
  }

  audio.speakerPin = 9; // Same output pin
  audio.setVolume(6);
  audio.play("Azan.wav"); // Must be 8-bit unsigned, mono WAV. Use audio editor apps to change a wav file to the correct format.
}

void loop() {
  // Nothing
}
