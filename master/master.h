/**
 * @file master.h
 * @author Zerui An (anzerui@126.com / jerryazr@gmail.com)
 * @brief Thie file contains all the constants used in master.ino
 * @version 0.1
 * @date 2022-03-27
 * 
 * @copyright Copyright (c) 2022
 * To be added later
 * 
 */

#ifndef LED_MASTER_H_
#define LED_MASTER_H_

// Peripheral address
// Address "98:7b:f3:65:49:ab" --> {0xab, 0x49, 0x65, 0xf3, 0x7b, 0x98}
//#define ADDRESS {0xab, 0x49, 0x65, 0xf3, 0x7b, 0x98}

// Constants
// SLEEP_TIMEOUT: the time in ms before the system shutoff if no action
// MAX_FAILS: number of failed connection attempts allowed before shutoff
// DEBOUNCE_COUNT: high value: more stable; low value: more sensitive
// TRIGGER: FALLING/RISING for active-low/high encoders
#define SLEEP_TIMEOUT 20000
#define MAX_FAILS 20
#define DEBOUNCE_COUNT 1024
#define DEBOUNCE_SHORT 4
#define TRIGGER FALLING

// pin numbers
// port 0: pin 0-5
// port 1: pin 6-10
// Use pins from the same port here.
// This program assumes all pins belong to port 0.
//
// For generic use, please refer to the following code example
//
// uint8_t pin = D2;
// uint32_t pinNum = digitalPinToPinName(pin);
// NRF_GPIO_Type * port = nrf_gpio_pin_port_decode(&pinNum);
// uint32_t all_gpio_val = port->IN;
// bool pinState = (all_gpio_val >> (pinNum % 32)) & 1 == 1;
#define CLK_PIN 0
#define DT_PIN  1
#define SW_PIN  2

#if (CLK_PIN < 6 && DT_PIN < 6 && SW_PIN < 6)
#define PORT_REG NRF_P0
#elif (CLK_PIN > 5 && DT_PIN > 5 && SW_PIN > 5)
#define PORT_REG NRF_P1
#endif

// option code
#define NONE    0
#define BRIGHT  1
#define DIM     2
#define WARM    3
#define COLD    4

#endif
