#ifndef MY_BLE_H_
#define MY_BLE_H_

#define DEFAULT_SERVICE_UUID        0xFFE0
#define DEFAULT_CHARACTERISTIC_UUID 0xFFE1

#include <bluefruit.h>

class MyPeriph {
  public:
    MyPeriph(
      uint16_t serviceUuid = DEFAULT_SERVICE_UUID,
      uint16_t characteristicUuid = DEFAULT_CHARACTERISTIC_UUID);

    virtual bool  begin(void);

    virtual uint8_t   read        ( void );
    virtual uint16_t  read        ( uint8_t * buf, uint16_t size );
    virtual uint16_t  write       ( uint8_t b );
    virtual uint16_t  write       ( uint8_t * buf, uint16_t size );

  private:
    BLEService _svc;
    BLECharacteristic _rtx; // rx and tx char 2 in 1
};

#endif
