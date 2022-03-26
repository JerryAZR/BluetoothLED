/*
  LED Control

  This example scans for Bluetooth® Low Energy peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the Bluetooth® Low Energy peripheral's LED, when the button is pressed or released.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button with pull-up resistor connected to pin 2.

  You can use it with another board that is compatible with this library and the
  Peripherals -> LED example.

  This example code is in the public domain.
*/

#include "MyBLE.h" // Override the default BLEDevice definition
#include "MyLED.h"
#include <ArduinoBLE.h>

// variables for button

const char* SERV_UUID = "ffe0";
const char* CHAR_UUID = "ffe1";
//String ADDRESS = "98:7b:f3:65:49:ab";
uint8_t ADDRESS[6] = {0xab, 0x49, 0x65, 0xf3, 0x7b, 0x98};

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
  
  while (peripheral.connected()) {
    // while the peripheral is connected

    // read input and forward to LED
    int option = getInput();
    LEDCtrl ctrl = LEDUpdate(option);
    uint8_t yellow = (ctrl.yellow >> 1) & 0x7F;
    uint8_t white = (ctrl.white >> 1) | 0x80;
    ledCharacteristic.writeValue(yellow);
    ledCharacteristic.writeValue(white);
    ctrl.serialPrint();
    delay(50);
    while(active(CLK));
  }
  
  Serial.println("Peripheral disconnected");
  Serial.println("Reconnecting");
}
