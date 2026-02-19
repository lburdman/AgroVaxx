#ifndef MOCKDRIVERS_H
#define MOCKDRIVERS_H

#include "../core/Interfaces.h"
#include <queue>
#include <string>

class MockRfid : public IRfidReader {
public:
  std::string nextTag;
  bool hasNewTag;

  void init() override {
    hasNewTag = false;
    nextTag = "";
  }

  bool isNewTagPresent() override { return hasNewTag; }

  std::string readTagUid() override {
    hasNewTag = false;
    return nextTag;
  }
};

class MockBluetooth : public IBluetoothModule {
public:
  bool connected = false;
  std::queue<std::string> incomingMsgs;
  std::vector<std::string> outgoingMsgs;

  void init() override {}
  bool isConnected() override { return connected; }

  std::string readMessage() override {
    if (incomingMsgs.empty())
      return "";
    std::string msg = incomingMsgs.front();
    incomingMsgs.pop();
    return msg;
  }

  void sendMessage(const std::string &msg) override {
    outgoingMsgs.push_back(msg);
  }
};

class MockButton : public IButton {
public:
  bool pressed = false;
  void init() override {}
  bool isPressed() override { return pressed; }
};

class MockLeds : public IStatusLeds {
public:
  bool blueOn = false;
  bool greenOn = false;

  void init() override {}
  void setBlue(bool on) override { blueOn = on; }
  void setGreen(bool on) override { greenOn = on; }
  void toggleGreen() override { greenOn = !greenOn; }
};

class MockTime : public ITimeProvider {
public:
  uint64_t currentMs = 0;
  uint64_t getTicksMs() override { return currentMs; }
  void advance(uint64_t ms) { currentMs += ms; }
};

#endif // MOCKDRIVERS_H
