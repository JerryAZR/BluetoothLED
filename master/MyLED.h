#ifndef MY_LED_H
#define MY_LED_H

// pin numbers
#define CLK 0
#define DT  1
#define SW  2

// option code
#define NONE    0
#define BRIGHT  1
#define DIM     2
#define WARM    3
#define COLD    4

#define STEP_B  8
#define STEP_T  0.05

#define BYTE_MAX  255
#define HALF_MAX  127

#define TRIGGER FALLING

typedef unsigned char uint8_t;

struct LEDCtrl {
  uint8_t white;
  uint8_t yellow;

  LEDCtrl(double brightness, double temperature);
  void serialPrint();
};

double bound(double original, double bound);
LEDCtrl LEDUpdate(int option);
bool active(int pin);
int getInput();

#endif
