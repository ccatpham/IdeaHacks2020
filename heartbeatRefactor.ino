#include <Time.h>
#include <Arduino.h>


// Heart Rate Sensor Defintions
const int HR_RX = 7;
byte oldSample, sample;
int bpm = 0;

time_t finalTime;
int beatCounter = 0;

void calculateBPM() {
  Serial.print("Your BPM is ");
  Serial.println(bpm);
  //Serial.println('/');
}

void heartCalibration() {
  sample = digitalRead(HR_RX);  //Store signal output
  if (sample && (oldSample != sample)) {
    //Serial.println("Beat");
    beatCounter++;
  }
  oldSample = sample;           //Store last signal received
  //  timer.run();
}

void setup() {
  // put your setup code here, to run once:
  // initiate heart calibration
  Serial.begin(115200);
  pinMode (HR_RX, INPUT);  //Signal pin to input
  Serial.println("Waiting for heart beat...");
  while (!digitalRead(HR_RX)) {};
  Serial.println ("Heart beat detected!");
  unsigned long initialTime = millis();
  unsigned long finalTime = initialTime;
  unsigned long diffTime;
  while (finalTime - initialTime < 30000) {
    sample = digitalRead(HR_RX);
    if (sample && (oldSample != sample)) {
      beatCounter++;
    }
    oldSample = sample;
    finalTime = millis();
  }
  bpm = beatCounter * 2;
  calculateBPM();
}

void loop() {
  // put your main code here, to run repeatedly:

}
