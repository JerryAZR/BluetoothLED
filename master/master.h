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

namespace constants {
    // SLEEP_TIMEOUT: the time in ms before the system shutoff if no action
    // SCAN_TIMEOUT: max scan time (before going to sleep) in 10 ms units
    // ACTIVE: 1 (active high) or 0 (active low)
    const int SLEEP_TIMEOUT     = 20000;
    const int SCAN_TIMEOUT      = 2000;
    const int ACTIVE            = 0;

    // pin numbers
    const int CLK_PIN = 0;
    const int DT_PIN  = 1;
    const int SW_PIN  = 2;

    // option codes
    const int NONE    = 0;
    const int BRIGHT  = 1;
    const int DIM     = 2;
    const int WARM    = 3;
    const int COLD    = 4;
} // namespace constants

#endif
