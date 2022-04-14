#include "MyPeriph.h"
#include "MyLED.h"

const int YELLOW_PIN = 10;
const int WHITE_PIN  =  9;

LEDCtrl ctrl(0, 0.5);
MyPeriph periph;
uint8_t packet[4] = {0};

// the setup function runs once when you press reset or power the board
void setup() {
  // Initialize PWM module
  HwPWM0.addPin(YELLOW_PIN);
  HwPWM0.addPin(WHITE_PIN);
  // Enable PWM modules with 8-bit resolutions(max)
  HwPWM0.begin();
  HwPWM0.setResolution(8);
  HwPWM0.setClockDiv(PWM_PRESCALER_PRESCALER_DIV_16); // freq = 4KHz

  // Initialize BLE
  Bluefruit.begin(1, 0);      // one peripheral
  Bluefruit.setTxPower(4);
  periph.begin();

  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  if (Bluefruit.Periph.connected()) {
    periph.read(packet, sizeof(packet));
    if (packet[0] != NONE) {
      uint8_t option = packet[0];
      ctrl.update(option);
      HwPWM0.writePin(WHITE_PIN, ctrl.white);
      HwPWM0.writePin(YELLOW_PIN, ctrl.yellow);
      ctrl.serialPrint();
      // report the values back to controller
      packet[0] = NONE;
      packet[1] = ctrl.yellow;
      packet[2] = ctrl.white;
      periph.write(packet, sizeof(packet));
    }
  }
}
