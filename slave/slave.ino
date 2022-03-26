#include "mylib.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,4); // RX, TX
// the setup function runs once when you press reset or power the board

#define YELLOW_PIN  10
#define WHITE_PIN   9
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(WHITE_PIN, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  setPwmFrequency(WHITE_PIN, 8);
  setPwmFrequency(YELLOW_PIN, 8);
}

// the loop function runs over and over again forever
uint8_t level = 0;
void loop() {
  if (mySerial.available()) {
    level = mySerial.read();
    if (level & 0x80) {
      // white
      level &= 0x7F;
      analogWrite(WHITE_PIN, level << 1);
    } else {
      // yellow
      analogWrite(YELLOW_PIN, level << 1);
    }
    Serial.print("Received: ");
    Serial.println(level);
  }
}
