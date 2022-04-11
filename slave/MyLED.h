#ifndef MY_LED_H
#define MY_LED_H

// option code, must be the same as the ones defined in master.h
enum {
  // option codes
  NONE = 0,
  BRIGHT,
  DIM,
  WARM,
  COLD
};

const double STEP_B  = 8;
const double STEP_T  = 0.05;

const int BYTE_MAX = 255;
const int HALF_MAX = 127;

typedef unsigned char uint8_t;

struct LEDCtrl {
  uint8_t white;
  uint8_t yellow;
  double brightness;
  double warmth;

  LEDCtrl(double brightness = 0.0, double warmth = 0.5);
  void update(int option);
  void serialPrint();
};

double bound(double original, double bound);

#endif
