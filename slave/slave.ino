#include "mylib.h"
#include "MyLED.h"
#include <SoftwareSerial.h>

#define YELLOW_PIN  10
#define WHITE_PIN   9

SoftwareSerial mySerial(2,4); // RX, TX
LEDCtrl ctrl(0, 0.5);

// the setup function runs once when you press reset or power the board
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
void loop() {
  if (mySerial.available()) {
    uint8_t option = mySerial.read();
    ctrl.update(option);
    analogWrite(WHITE_PIN, ctrl.white);
    analogWrite(YELLOW_PIN, ctrl.yellow);
    ctrl.serialPrint();
    // report the values back to controller
    byte msg[2];
    msg[0] = ctrl.yellow;
    msg[1] = ctrl.white;
    mySerial.write(msg, 2);
  }
}
