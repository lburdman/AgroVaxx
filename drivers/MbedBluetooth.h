#ifndef MBED_BLUETOOTH_H
#define MBED_BLUETOOTH_H

#include "../core/Interfaces.h"
#include "mbed.h"
#include <string>

class MbedBluetooth : public IBluetoothModule {
private:
  UnbufferedSerial uart;
  DigitalIn statePin;
  std::string rxBuffer;

public:
  MbedBluetooth(PinName tx, PinName rx, PinName state)
      : uart(tx, rx, 9600), statePin(state) {}

  void init() override { statePin.mode(PullDown); }

  bool isConnected() override { return statePin.read() == 1; }

  std::string readMessage() override {
    char c;
    while (uart.readable()) {
      uart.read(&c, 1);
      if (c == '\n' || c == '\r') {
        if (!rxBuffer.empty()) {
          std::string msg = rxBuffer;
          rxBuffer.clear();
          return msg;
        }
      } else {
        rxBuffer += c;
      }
    }
    return "";
  }

  void sendMessage(const std::string &msg) override {
    uart.write(msg.c_str(), msg.length());
  }
};

#endif // MBED_BLUETOOTH_H
