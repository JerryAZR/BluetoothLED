/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   https://forum.arduino.cc/index.php?topic=16612#msg121031
 */
#include "Arduino.h"

#if defined(TCCR0B) && defined(TCCR1B) && defined(TCCR2B)
void setPwmFrequency(int pin, int divisor) {
  unsigned char mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
#elif defined(TCB0_CTRLA)
void setPwmFrequency(int pin, int divisor) {
/*
  Arduino Nano Every PinOut (ATmega4809)
                  _
               __|_|__    
              |       |  
  (D13)  PE2  +       +  PE1  (D12)  
        3.3V  +       +  PE0  (D11)  
  (AREF) PD7  +       +  PB1  (D10)(~)(TCA0 Channel 1)
  (D14)  PD3  +       +  PB0  (D 9)(~)(TCA0 Channel 0)
  (D15)  PD2  +       +  PE3  (D 8)
  (D16)  PD1  +       +  PA1  (D 7) 
  (D17)  PD0  +       +  PF4  (D 6)(~)(TCB0)
  (D18)  PF2  +       +  PB2  (D 5)(~)(TCA0 Channel 2)
  (D19)  PF3  +       +  PC6  (D 4)  
  (D20)  PD4  +       +  PF5  (D 3)(~)(TCB1)  
  (D21)  PD5  +       +  PA0  (D 2) 
        5.0V  +       +  GND
       RESET  +       +  RESET
         GND  +       +  PC5  (D 0) RX1
         VIN  +       +  PC4  (D 1) TX1      
              |_______|  
              
TCB3 would have as WO pin PB5 or PC1. Both are not led out on the Nano Every board.
TCB2 would have pin PC0 or PB4 as WO. Both are not led out on the Nano Every Board.
TCB1 has pin PA3 or PF5 as WO. PF5 is led out on the Nano Every Board.
TCB0 has pin PA2 or PF4 as WO. PF4 is led out on the Nano Every Board.
*/
  cli();  // Disable Interrupts
  if (pin == 5 || pin == 10 || pin == 5) {
    switch (divisor) {
      case 1:
        TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
        break;
      case 2:
        TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV2_gc | TCA_SINGLE_ENABLE_bm;
        break;
      case 4:
        TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc | TCA_SINGLE_ENABLE_bm;
        break;
      case 8:
        TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV8_gc | TCA_SINGLE_ENABLE_bm;
        break;
      case 16:
        TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc | TCA_SINGLE_ENABLE_bm;
        break;
      case 64:
        TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm;
        break;
      case 256:
        TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV256_gc | TCA_SINGLE_ENABLE_bm;
        break;
      case 1024:
        TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;
        break;
    }
  } else if (pin == 6) {
    switch (divisor) {
      case 1:
        TCB0_CTRLA = (TCB_CLKSEL_CLKDIV1_gc) | (TCB_ENABLE_bm);
        break;
      case 2:
        TCB0_CTRLA = (TCB_CLKSEL_CLKDIV2_gc) | (TCB_ENABLE_bm);
        break;
      default:
        setPwmFrequency(5, divisor); // Use TCA as clock source instead
        TCB0_CTRLA = (TCB_CLKSEL_CLKTCA_gc) | (TCB_ENABLE_bm);
    }
  } else if (pin == 3) {
    switch (divisor) {
      case 1:
        TCB1_CTRLA = (TCB_CLKSEL_CLKDIV1_gc) | (TCB_ENABLE_bm);
        break;
      case 2:
        TCB1_CTRLA = (TCB_CLKSEL_CLKDIV2_gc) | (TCB_ENABLE_bm);
        break;
      default:
        setPwmFrequency(5, divisor); // Use TCA as clock source instead
        TCB1_CTRLA = (TCB_CLKSEL_CLKTCA_gc) | (TCB_ENABLE_bm);
    }
  }
  sei();  // Enable Interrupts
}
#endif

