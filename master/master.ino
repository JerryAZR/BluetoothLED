/**
 * @file master.ino
 * @author Zerui An (anzerui@126.com / jerryazr@gmail.com)
 * @brief Use a Seeeduino Xiao BLE (wired to a rotary encoder)
 *        as an LED controller.
 * @version 0.1
 * @date 2022-03-27
 * 
 * @copyright Copyright (c) 2022
 * To be added later
 */

#include "MyBLE.h" // Override the default BLEDevice definition
#include "master.h"
#include <ArduinoBLE.h>

// helper functions
void wakeUp() {}
bool active(int pin);
uint8_t getInput();
inline uint32_t getMask(uint8_t pin);
inline bool myDigitalRead(uint32_t portRegVal, uint32_t pinMask);

// variables
const char* SERV_UUID = "ffe0";
const char* CHAR_UUID = "ffe1";
// Address "98:7b:f3:65:49:ab" --> {0xab, 0x49, 0x65, 0xf3, 0x7b, 0x98}
uint8_t ADDRESS[6] = {0xab, 0x49, 0x65, 0xf3, 0x7b, 0x98};
unsigned long prev; // sleep timer
uint16_t newReport;
uint16_t oldReport;

// pin masks
const uint32_t CLK_MASK = getMask(CLK);
const uint32_t DT_MASK = getMask(DT);
const uint32_t SW_MASK = getMask(SW);

BLEDevice peripheral = BLEDevice(0, ADDRESS);

void setup() {
  Serial.begin(9600);

  // configure the button pin as input
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT);

  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();

  // no need to scan for peripherals

  // To wait for the serial ports to be initialized,
  // add some delay here
  // delay(1000);
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
  ledCharacteristic.writeValue((uint8_t)NONE); // request value update
  oldReport = NONE;
  while (peripheral.connected()) {
    // while the peripheral is connected

    // read input and forward to LED
    uint8_t option = getInput();
    if (option != NONE) {
      // send the option to slave device
      ledCharacteristic.writeValue(option);
      oldReport = option;

      // simple debouncer
      // For reference, according to some random guy on stackoverflow:
      // digitalRead takes 4.9us to execute on a 16MIPS Arduino Uno
      // The Seeeduino Xiao runs at 48MHz
      // Replace the following loop with
      //    while(active(CLK));
      // if using hardware debounce circuit
      for (int i = 0; i < 8192; i++) {
        if (active(CLK)) i = 0;
      }
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
      }
    }
    // check if report from slave is available
    // Characteristic.valueUpdated() doesn't seem to work here
    // so I have to perform the check manually
    ledCharacteristic.readValue(newReport); // pass by reference
    if (newReport != oldReport) {
      Serial.print("Yellow: ");
      Serial.println(newReport & 0xFF);
      Serial.print("White: ");
      Serial.println(newReport >> 8);
      oldReport = newReport;
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
#ifdef PORT_REG
  uint32_t portVals = PORT_REG->IN;
  bool clk = myDigitalRead(portVals, CLK_MASK);
  bool dt = myDigitalRead(portVals, DT_MASK);
  bool sw = myDigitalRead(portVals, SW_MASK);
#else
  bool clk = active(CLK);
  bool dt = active(DT);
  bool sw = active(SW);
#endif
  if(!clk) return NONE;
  if (!dt && sw) {
    return WARM;
  } else if (!dt && !sw) {
    return BRIGHT;
  } else if (dt && sw) {
    return COLD;
  } else {
    return DIM;
  }
}

inline uint32_t getMask(uint8_t pin) {
  uint32_t pinNum = digitalPinToPinName(pin);
  return 1 << (pinNum & 0x1F);
}

inline bool myDigitalRead(uint32_t portRegVal, uint32_t pinMask) {
#if(TRIGGER == FALLING)
  return (portRegVal & pinMask) == 0;
#else
  return (portRegVal & pinMask) != 0;
#endif
}
