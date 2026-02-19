#include "../drivers/MbedBluetooth.h"
#include "mbed.h"

int main() {
  printf("Starting BLE Hardware Test...\n");
  MbedBluetooth ble(PD_6, PD_5, PD_4);
  ble.init();

  while (true) {
    if (ble.isConnected()) {
      printf("BLE Connected!\n");
    }

    std::string msg = ble.readMessage();
    if (!msg.empty()) {
      printf("Received: %s\n", msg.c_str());
      ble.sendMessage("ECHO: " + msg + "\n");
    }
    ThisThread::sleep_for(100ms);
  }
}
