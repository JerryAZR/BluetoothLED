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

#include "MyBLE.h"
#include "master.h"
#include "helper.h"
#include <bluefruit.h>

// helper functions
void wakeUp() {}
void powerOff();
bool inline active(int pin);

// variables
unsigned long prev; // sleep timer
uint16_t newReport;
uint16_t oldReport;

MyBLE myBLE;

void setup() {
  Serial.begin(9600);

  // configure the button pin as input
  pinMode(CLK_PIN, INPUT);
  pinMode(DT_PIN, INPUT);
  pinMode(SW_PIN, INPUT);

  // initialize the Bluetooth® Low Energy hardware
  Bluefruit.begin(0, 1);
  myBLE.begin();

  // set callbacks
  Bluefruit.Central.setConnectCallback(connect_callback);
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);

  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.start(0);

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
    uint8_t option = getInput();
    if (option != NONE) {
      // send the option to slave device
      myBLE.write(option);
      oldReport = (oldReport & 0xFF00) | ((uint16_t) option);
      // simple debouncer
      // For reference, according to some random guy on stackoverflow:
      // digitalRead takes 4.9us to execute on a 16MIPS Arduino Uno
      // The Seeeduino Xiao runs at 48MHz
      // Replace the following loop with
      //    while(active(CLK_PIN));
      // if using hardware debounce circuit
      for (int i = 0; i < DEBOUNCE_COUNT; i++) {
        if (active(CLK_PIN)) i = 0;
      }
      prev = millis(); // reset timer
    } else {
      if (millis() - prev > SLEEP_TIMEOUT) {
        // setup interrupt handler
        // attachInterrupt(digitalPinToInterrupt(CLK_PIN), wakeUp, TRIGGER);
        // powerOff();
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
  // __DSB();
  // NRF_POWER->SYSTEMOFF = POWER_SYSTEMOFF_SYSTEMOFF_Enter;
  // __DSB();
  // Alternatively, include "nrf_power.h"
  // and call nrf_power_system_off()
}

bool inline active(int pin) {
  // active low
#if (TRIGGER == FALLING)
  return (digitalRead(pin) == 0);
#else
  return (digitalRead(pin) != 0);
#endif
}
