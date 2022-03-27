#ifndef MY_LED_H
#define MY_LED_H

// option code, must be the same as the ones defined in master.h
#define NONE    0
#define BRIGHT  1
#define DIM     2
#define WARM    3
#define COLD    4

#define STEP_B  8
#define STEP_T  0.05

#define BYTE_MAX  255
#define HALF_MAX  127

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
