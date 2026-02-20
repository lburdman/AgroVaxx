#ifndef INTERFACES_H
#define INTERFACES_H

#include <cstdint>
#include <string>

// Forward declarations
class IRfidReader {
public:
  virtual ~IRfidReader() = default;
  virtual void init() = 0;
  virtual bool isNewTagPresent() = 0;
  virtual std::string readTagUid() = 0;
};

class IBluetoothModule {
public:
  virtual ~IBluetoothModule() = default;
  virtual void init() = 0;
  virtual bool isConnected() = 0;
  virtual std::string readMessage() = 0;
  virtual void sendMessage(const std::string &msg) = 0;
};

class IButton {
public:
  virtual ~IButton() = default;
  virtual void init() = 0;
  virtual bool isPressed() = 0;
};

class IStatusLeds {
public:
  virtual ~IStatusLeds() = default;
  virtual void init() = 0;
  virtual void setBlue(bool on) = 0;
  virtual void setGreen(bool on) = 0;
  virtual void toggleGreen() = 0;
};

class ITimeProvider {
public:
  virtual ~ITimeProvider() = default;
  virtual uint64_t getTicksMs() = 0;
};

#endif // INTERFACES_H
