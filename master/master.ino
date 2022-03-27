/*
  LED Control

  More to be added later
*/

#include "MyBLE.h" // Override the default BLEDevice definition
#include "MyLED.h"
#include <ArduinoBLE.h>

#define SLEEP_TIMEOUT 20000

// variables for button

const char* SERV_UUID = "ffe0";
const char* CHAR_UUID = "ffe1";
//String ADDRESS = "98:7b:f3:65:49:ab";
uint8_t ADDRESS[6] = {0xab, 0x49, 0x65, 0xf3, 0x7b, 0x98};
unsigned long prev; // sleep timer

BLEDevice peripheral = BLEDevice(0, ADDRESS);

void wakeUp() {}

void setup() {
  Serial.begin(9600);

  // configure the button pin as input
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT);

  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();

  // no need to scan for peripherals
}

void loop() {
  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }
  
  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }
  
  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic(CHAR_UUID);
  
  if (!ledCharacteristic) {
    Serial.println("Peripheral does not have LED characteristic!");
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.canWrite()) {
    Serial.println("Peripheral does not have a writable LED characteristic!");
    peripheral.disconnect();
    return;
  }

  prev = millis();
  while (peripheral.connected()) {
    // while the peripheral is connected

    // read input and forward to LED
    int option = getInput();
    if (option != NONE) {
      LEDCtrl ctrl = LEDUpdate(option);
      uint8_t yellow = (ctrl.yellow >> 1) & 0x7F;
      uint8_t white = (ctrl.white >> 1) | 0x80;
      ledCharacteristic.writeValue(yellow);
      ledCharacteristic.writeValue(white);
      ctrl.serialPrint();
      delay(50);
      while(active(CLK));
      prev = millis(); // reset timer
    } else {
      if (millis() - prev > SLEEP_TIMEOUT) {
        // setup interrupt handler
        attachInterrupt(digitalPinToInterrupt(CLK), wakeUp, TRIGGER);
        __DSB();
        NRF_POWER->SYSTEMOFF = POWER_SYSTEMOFF_SYSTEMOFF_Enter;
        __DSB();
        // Alternatively, include "nrf_power.h"
        // and call nrf_power_system_off()

        // Wakeup
        detachInterrupt(digitalPinToInterrupt(CLK));
        prev = millis();
      }
    }
  }
  
  Serial.println("Peripheral disconnected");
  Serial.println("Reconnecting");
}
