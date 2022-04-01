/**
 * @file helper.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef HELPER_H_
#define HELPER_H_

#include <Arduino.h>
#include <bluefruit.h>

void scan_callback(ble_gap_evt_adv_report_t* report);
void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);

#endif
