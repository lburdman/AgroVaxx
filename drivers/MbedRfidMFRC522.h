#ifndef MBED_RFID_MFRC522_H
#define MBED_RFID_MFRC522_H

#include "../core/Interfaces.h"
#include "MFRC522.h"
#include "mbed.h"

class MbedRfidMFRC522 : public IRfidReader {
private:
  MFRC522 rfid;
  std::string currentUid;

public:
  MbedRfidMFRC522(PinName mosi, PinName miso, PinName sclk, PinName cs,
                  PinName reset)
      : rfid(mosi, miso, sclk, cs, reset) {}

  void init() override { rfid.PCD_Init(); }

  bool isNewTagPresent() override {
    if (!rfid.PICC_IsNewCardPresent())
      return false;
    if (!rfid.PICC_ReadCardSerial())
      return false;

    char hexStr[20];
    currentUid = "";
    for (uint8_t i = 0; i < rfid.uid.size; i++) {
      sprintf(hexStr, "%02X", rfid.uid.uidByte[i]);
      currentUid += hexStr;
    }
    return true;
  }

  std::string readTagUid() override { return currentUid; }
};

#endif // MBED_RFID_MFRC522_H
