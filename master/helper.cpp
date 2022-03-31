#include "master.h"
#include "helper.h"
#include "MyBLE.h"

extern MyBLE myBLE;

inline bool myDigitalRead(uint32_t portRegVal, uint32_t pinMask);
inline bool active(int pin);
inline uint32_t getMask(uint8_t pin);

// pin masks
const uint32_t CLK_MASK = getMask(CLK_PIN);
const uint32_t DT_MASK = getMask(DT_PIN);
const uint32_t SW_MASK = getMask(SW_PIN);

uint8_t getInput() {
  bool clk, dt, sw;
#ifdef PORT_REG
  uint32_t portVals = PORT_REG->IN;
  clk = myDigitalRead(portVals, CLK_MASK);
#else
  clk = active(CLK_PIN);
#endif
  if(!clk) return NONE;
  // wait until the end of clock period
  for (int i = 0; i < DEBOUNCE_SHORT; i++) {
#ifdef PORT_REG
    portVals = PORT_REG->IN;
    clk = myDigitalRead(portVals, CLK_MASK);
#else
    clk = active(CLK_PIN);
#endif
    if (clk) i = 0;
  }
  // read the other pin values
#ifdef PORT_REG
  dt = myDigitalRead(portVals, DT_MASK);
  sw = myDigitalRead(portVals, SW_MASK);
#else
  dt = active(DT_PIN);
  sw = active(SW_PIN);
#endif
  if (dt && sw) {
    return WARM;
  } else if (dt && !sw) {
    return BRIGHT;
  } else if (!dt && sw) {
    return COLD;
  } else {
    return DIM;
  }
}

inline bool myDigitalRead(uint32_t portRegVal, uint32_t pinMask) {
#if(TRIGGER == FALLING)
  return (portRegVal & pinMask) == 0;
#else
  return (portRegVal & pinMask) != 0;
#endif
}

bool inline active(int pin) {
  // active low
#if (TRIGGER == FALLING)
  return (digitalRead(pin) == 0);
#else
  return (digitalRead(pin) != 0);
#endif
}


void scan_callback(ble_gap_evt_adv_report_t* report) {
  // Check if advertising contain target service
  if ( Bluefruit.Scanner.checkReportForUuid(report, 0xFFE0) ) {
    Serial.println("target device detected. Connecting ... ");

    // print report info
    Serial.println("peer_addr:");
    Serial.print("  addr_id_peer: "); Serial.println(report->peer_addr.addr_id_peer);
    Serial.print("  addr_type: "); Serial.println(report->peer_addr.addr_type);
    Serial.print("  addr: "); 
    for (int i = 0; i < 6; i++) {
      Serial.print(report->peer_addr.addr[i]);
      Serial.print(":");
    }
    Serial.println();

    // Connect to device with bleuart service in advertising
    Bluefruit.Central.connect(report);
  } else {      
    // For Softdevice v6: after received a report, scanner will be paused
    // We need to call Scanner resume() to continue scanning
    Bluefruit.Scanner.resume();
  }
}

void connect_callback(uint16_t conn_handle) {
  Serial.println("Connected");
  if (myBLE.discover(conn_handle)) {
    Serial.println("Found Service");
  } else {
    Serial.println("Service not found");
    Bluefruit.disconnect(conn_handle);
    return;
  }
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  (void) conn_handle;
  (void) reason;
  
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}

inline uint32_t getMask(uint8_t pin) {
  uint32_t pinNum = digitalPinToPinName(pin);
  return 1 << (pinNum & 0x1F);
}
