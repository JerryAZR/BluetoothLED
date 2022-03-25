#include "mylib.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,4); // RX, TX
// the setup function runs once when you press reset or power the board

#define OUT_PIN 10
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(OUT_PIN, OUTPUT);
  mySerial.begin(9600);
  setPwmFrequency(OUT_PIN, 8);
}

// the loop function runs over and over again forever
int level = 0;
void loop() {
  if (mySerial.available()) {
    level = mySerial.read();
    analogWrite(OUT_PIN, level << 4);
  }
}
