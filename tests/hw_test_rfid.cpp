#include "../drivers/MbedRfidMFRC522.h"
#include "mbed.h"

int main() {
  printf("Starting RFID Hardware Test...\n");
  MbedRfidMFRC522 rfid(PA_7, PA_6, PA_5, PE_3, PG_1);
  rfid.init();

  while (true) {
    if (rfid.isNewTagPresent()) {
      printf("Tag scanned: %s\n", rfid.readTagUid().c_str());
    }
    ThisThread::sleep_for(100ms);
  }
}
