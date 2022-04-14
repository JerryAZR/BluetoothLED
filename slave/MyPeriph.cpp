/**
 * @file MyPeriph.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <bluefruit.h>
#include "MyPeriph.h"

MyPeriph::MyPeriph(
  uint16_t serviceUuid, uint16_t characteristicUuid)
  : _svc(serviceUuid), _rtx(characteristicUuid) {}

bool MyPeriph::begin(void) {
  // Invoke base class begin()
  _svc.begin();
  _rtx.setProperties(CHR_PROPS_NOTIFY | CHR_PROPS_READ | CHR_PROPS_WRITE);
  _rtx.setFixedLen(4); // we may want to change this in the future
  _rtx.begin();
  uint8_t tmp[4] = {0};
  _rtx.write(tmp, sizeof(tmp));

  // Start advertising
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(_svc);
  Bluefruit.setName("Seeeduino LED");
  Bluefruit.Advertising.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);

  return true;
}

uint8_t MyPeriph::read (void)
{
  return _rtx.read8();
}

uint16_t MyPeriph::read(uint8_t * buf, uint16_t size)
{
  return _rtx.read(buf, size);
}

uint16_t MyPeriph::write(uint8_t b)
{
  return _rtx.write8(b);
}

uint16_t MyPeriph::write(uint8_t * buf, uint16_t size)
{
  return _rtx.write(buf, size);
}
