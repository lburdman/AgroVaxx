#ifndef MBED_SIMPLE_DRIVERS_H
#define MBED_SIMPLE_DRIVERS_H

#include "../core/Interfaces.h"
#include "mbed.h"

class MbedButton : public IButton {
private:
  DigitalIn btn;

public:
  MbedButton(PinName pin) : btn(pin) {}

  void init() override { btn.mode(PullDown); }

  bool isPressed() override { return btn.read() == 1; }
};

class MbedLeds : public IStatusLeds {
private:
  DigitalOut blueLed;
  DigitalOut greenLed;

public:
  MbedLeds(PinName bluePin, PinName greenPin)
      : blueLed(bluePin), greenLed(greenPin) {}

  void init() override {
    blueLed = 0;
    greenLed = 0;
  }

  void setBlue(bool on) override { blueLed = on ? 1 : 0; }

  void setGreen(bool on) override { greenLed = on ? 1 : 0; }

  void toggleGreen() override { greenLed = !greenLed; }
};

class MbedTime : public ITimeProvider {
public:
  uint64_t getTicksMs() override { return Kernel::get_ms_count(); }
};

#endif // MBED_SIMPLE_DRIVERS_H
