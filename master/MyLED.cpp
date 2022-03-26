#include "Arduino.h"
#include "MyLED.h"

double bound(double original, double bound) {
  if (original < 0) return 0;
  else if (original > bound) return bound;
  else return original;
}

LEDCtrl::LEDCtrl(double brightness, double warmth) {
  white = (uint8_t) bound(brightness * (1-warmth), BYTE_MAX);
  yellow = (uint8_t) bound(brightness * warmth, BYTE_MAX);
}

void LEDCtrl::serialPrint() {
  Serial.print("WHITE: ");
  Serial.print(white);
  Serial.print("; YELLOW: ");
  Serial.println(yellow);
}

LEDCtrl LEDUpdate(int option) {
  static double brightness = 0.0;
  static double warmth = 0.5;
  
  switch (option) {
    case BRIGHT:
      brightness = bound(brightness + STEP_B, BYTE_MAX);
      break;
    case DIM:
      brightness = bound(brightness - STEP_B, BYTE_MAX);
      break;
    case WARM:
      warmth = bound(warmth + STEP_T, 1);
      break;
    case COLD:
      warmth = bound(warmth - STEP_T, 1);
  }
  return LEDCtrl(brightness, warmth);
}

bool active(int pin) {
  // active low
  return (digitalRead(pin) == 0);
}


int getInput() {
  while(!active(CLK));
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
