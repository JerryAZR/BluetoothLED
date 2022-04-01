/**
 * @file master.ino
 * @author Zerui An (anzerui@126.com / jerryazr@gmail.com)
 * @brief Use a Seeeduino Xiao BLE (wired to a rotary encoder)
 *        as an LED controller.
 * @version 0.1
 * @date 2022-03-30
 * 
 * @copyright Copyright (c) 2022
 * To be added later
 */

#include <bluefruit.h>
#include "RotaryEncoderD.h"
#include "MyBLE.h"
#include "master.h"
#include "helper.h"

// Not the best practise, I know.
// But I'm too lazy to type all the variables explicitly
using namespace constants;

// helper functions
void powerOff();
bool inline active(int pin);

// variables
unsigned long prev; // sleep timer
uint16_t newReport;
uint16_t oldReport;

MyBLE myBLE;
RotaryEncoderD<CLK_PIN, DT_PIN> encoder;

void setup() {
  Serial.begin(9600);

  // configure the button pin as input
  encoder.begin();
  pinMode(SW_PIN, INPUT);

  // initialize the Bluetooth® Low Energy hardware
  Bluefruit.begin(0, 1);
  myBLE.begin();

  // set callbacks
  Bluefruit.Central.setConnectCallback(connect_callback);
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);

  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.setStopCallback(powerOff);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.start(SCAN_TIMEOUT);

  // To wait for the serial ports to be initialized,
  // add some delay here
  // delay(1000);
  // Attach interrupt
  // attachInterrupt(digitalPinToInterrupt(CLK_PIN), wakeUp, TRIGGER);
}

void loop() {
  if (Bluefruit.Central.connected()) {
    prev = millis();
    myBLE.write((uint8_t)NONE); // request value update
    oldReport = NONE;
  }

  while (Bluefruit.Central.connected()) {
    // while the peripheral is connected

    // read input and forward to LED
    uint8_t option = NONE;
    int direction = encoder.read();
    if (direction == encoder.FORWARD && active(SW_PIN)) {
      option = WARM;
    } else if (direction == encoder.FORWARD && !active(SW_PIN)) {
      option = BRIGHT;
    } else if (direction == encoder.BACKWARD && active(SW_PIN)) {
      option = COLD;
    } else if (direction == encoder.BACKWARD && !active(SW_PIN)) {
      option = DIM;
    }

    if (option != NONE) {
      // send the option to slave device
      myBLE.write(option);
      oldReport = (oldReport & 0xFF00) | ((uint16_t) option);

      prev = millis(); // reset timer
    } else {
      if (millis() - prev > SLEEP_TIMEOUT) {
        // setup interrupt handler
        // attachInterrupt(digitalPinToInterrupt(CLK_PIN), wakeUp, TRIGGER);
        powerOff();
      }
    }
    // check if report from slave is available
    myBLE.read((uint8_t*)&newReport, 2);
    if (newReport != oldReport) {
      Serial.print("Yellow: ");
      Serial.println(newReport & 0xFF);
      Serial.print("White: ");
      Serial.println(newReport >> 8);
      oldReport = newReport;
    }
  }
}

void powerOff() {
  // Setup wakeup pins, then go to sleep
  Serial.println("System shutting down...");
  pinMode(CLK_PIN, INPUT_PULLUP_SENSE);
  pinMode(SW_PIN, INPUT_PULLUP_SENSE);
  sd_power_system_off();
}

bool inline active(int pin) {
  // active low
  return (digitalRead(pin) == ACTIVE);
}
