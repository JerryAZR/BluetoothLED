#include "Arduino.h"
#include "MyLED.h"

double bound(double original, double bound) {
  if (original < 0) return 0;
  else if (original > bound) return bound;
  else return original;
}

LEDCtrl::LEDCtrl(double brightness, double warmth) {
  this->white = (uint8_t) bound(brightness * (1-warmth), BYTE_MAX);
  this->yellow = (uint8_t) bound(brightness * warmth, BYTE_MAX);
  this->brightness = brightness;
  this->warmth = warmth;
}

void LEDCtrl::update(int option) {
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
  white = (uint8_t) bound(brightness * (1-warmth), BYTE_MAX);
  yellow = (uint8_t) bound(brightness * warmth, BYTE_MAX);
}

void LEDCtrl::serialPrint() {
  Serial.print("WHITE: ");
  Serial.print(white);
  Serial.print("; YELLOW: ");
  Serial.println(yellow);
}
