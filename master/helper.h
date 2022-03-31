#ifndef HELPER_H_
#define HELPER_H_

#include <Arduino.h>
#include <bluefruit.h>

uint8_t getInput();
void scan_callback(ble_gap_evt_adv_report_t* report);
void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);

#endif
