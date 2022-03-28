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

#define SLEEP_TIMEOUT 20000
#define TRIGGER FALLING

// pin numbers
// port 0: pin 0-5
// port 1: pin 6-10
// Use pins from the same port here.
// This program assumes all pins belong to port 0.
// For generic use, please refer to the following code example
//
// uint8_t pin = D2;
// uint32_t pinNum = digitalPinToPinName(pin);
// NRF_GPIO_Type * port = nrf_gpio_pin_port_decode(&pinNum);
// uint32_t all_gpio_val = port->IN;
#define CLK 0
#define DT  1
#define SW  2

#if (CLK < 6 && DT < 6 && SW < 6)
#define PORT_REG NRF_P0
#elif (CLK > 5 && DT > 5 && SW > 5)
#define PORT_REG NRF_P1
#endif

// option code
#define NONE    0
#define BRIGHT  1
#define DIM     2
#define WARM    3
#define COLD    4

#endif
