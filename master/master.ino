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
void onForward();
void onBackward();
void onPower() ;

// variables
unsigned long prev; // sleep timer
uint16_t newReport;
uint16_t oldReport;

MyBLE myBLE;
RotaryEncoderD<CLK_PIN, DT_PIN> encoder;

uint8_t pending_cmd;

void setup() {
  Serial.begin(9600);

  // configure the button pin as input
  encoder.begin();
  encoder.attachForwardInterrupt(onForward);
  encoder.attachBackwardInterrupt(onBackward);
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
  Bluefruit.Scanner.filterUuid(BLEUuid(DEFAULT_SERVICE_UUID));
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

    if (pending_cmd != NONE) {
      // send the option to slave device
      myBLE.write(pending_cmd);
      oldReport = (oldReport & 0xFF00) | ((uint16_t) pending_cmd);
      pending_cmd = NONE;

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

void onForward() {
  if (active(SW_PIN)) pending_cmd = WARM;
  else pending_cmd = BRIGHT;
}

void onBackward() {
  if (active(SW_PIN)) pending_cmd = COLD;
  else pending_cmd = DIM;
}

void onPower() {
  pending_cmd = POWER;
}
