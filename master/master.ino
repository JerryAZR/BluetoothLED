/*
  LED Control

  More to be added later
*/

#include "MyBLE.h" // Override the default BLEDevice definition
#include "master.h"
#include <ArduinoBLE.h>

// variables for button

const char* SERV_UUID = "ffe0";
const char* CHAR_UUID = "ffe1";
// Address "98:7b:f3:65:49:ab" --> {0xab, 0x49, 0x65, 0xf3, 0x7b, 0x98}
uint8_t ADDRESS[6] = {0xab, 0x49, 0x65, 0xf3, 0x7b, 0x98};
unsigned long prev; // sleep timer

BLEDevice peripheral = BLEDevice(0, ADDRESS);

// helper functions
void wakeUp() {}
bool active(int pin);
uint8_t getInput();

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
    uint8_t option = getInput();
    if (option != NONE) {
      // send the option to slave device
      ledCharacteristic.writeValue(option);
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

bool active(int pin) {
  // active low
#if (TRIGGER == FALLING)
  return (digitalRead(pin) == 0);
#else
  return (digitalRead(pin) != 0);
#endif
}

uint8_t getInput() {
  if(!active(CLK)) return NONE;
  if (!active(DT) && active(SW)) {
    return WARM;
  } else if (!active(DT) && !active(SW)) {
    return BRIGHT;
  } else if (active(DT) && active(SW)) {
    return COLD;
  } else {
    return DIM;
  }
}
