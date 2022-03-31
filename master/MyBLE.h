#ifndef MY_BLE_H_
#define MY_BLE_H_

#define DEFAULT_SERVICE_UUID        0xFFE0
#define DEFAULT_CHARACTERISTIC_UUID 0xFFE1

#include "bluefruit_common.h"
#include "BLEClientCharacteristic.h"
#include "BLEClientService.h"

class MyBLE : public BLEClientService {
  public:
    MyBLE(
      uint16_t serviceUuid = DEFAULT_SERVICE_UUID,
      uint16_t characteristicUuid = DEFAULT_CHARACTERISTIC_UUID);

    virtual bool  begin(void);
    virtual bool  discover(uint16_t conn_handle);

    virtual uint8_t   read        ( void );
    virtual uint16_t  read        ( uint8_t * buf, uint16_t size );
    virtual uint16_t  write       ( uint8_t b );
    virtual uint16_t  write       ( uint8_t * buf, uint16_t size );
    virtual uint16_t  write_resp  ( uint8_t b );
    virtual uint16_t  write_resp  ( uint8_t * buf, uint16_t size );

  protected:
    virtual void  disconnect(void);

  private:
    BLEClientCharacteristic _rtx; // rx and tx char 2 in 1
};

#endif
