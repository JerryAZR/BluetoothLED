/**
 * @file MyBLE.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <bluefruit.h>
#include "MyBLE.h"

MyBLE::MyBLE(
  uint16_t serviceUuid, uint16_t characteristicUuid)
  : BLEClientService(serviceUuid), _rtx(characteristicUuid) {}

bool MyBLE::begin(void) {
  // Invoke base class begin()
  BLEClientService::begin();
  _rtx.begin(this);

  return true;
}

bool MyBLE::discover(uint16_t conn_handle)
{
  // Call Base class discover
  VERIFY( BLEClientService::discover(conn_handle) );
  _conn_hdl = BLE_CONN_HANDLE_INVALID; // make as invalid until we found all chars

  // Discover custom RTX characteristic
  VERIFY( 1 == Bluefruit.Discovery.discoverCharacteristic(conn_handle, _rtx) );

  _conn_hdl = conn_handle;
  return true;
}

void MyBLE::disconnect(void)
{
  BLEClientService::disconnect();
}

uint8_t MyBLE::read (void)
{
  return _rtx.read8();
}

uint16_t MyBLE::read(uint8_t * buf, uint16_t size)
{
  return _rtx.read(buf, size);
}

uint16_t MyBLE::write(uint8_t b)
{
  return _rtx.write8(b);
}

uint16_t MyBLE::write(uint8_t * buf, uint16_t size)
{
  return _rtx.write(buf, size);
}

uint16_t MyBLE::write_resp(uint8_t b)
{
  return _rtx.write8_resp(b);
}

uint16_t MyBLE::write_resp(uint8_t * buf, uint16_t size)
{
  return _rtx.write_resp(buf, size);
}
