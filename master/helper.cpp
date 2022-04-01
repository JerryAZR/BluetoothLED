#include "master.h"
#include "helper.h"
#include "MyBLE.h"

extern MyBLE myBLE;

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
