#include "../drivers/MbedSimpleDrivers.h"
#include "mbed.h"

int main() {
  printf("Starting LED & Button Test...\n");
  MbedButton button(USER_BUTTON);
  MbedLeds leds(LED2, LED1);

  button.init();
  leds.init();

  while (true) {
    if (button.isPressed()) {
      leds.setGreen(true);
      leds.setBlue(true);
    } else {
      leds.setGreen(false);
      leds.setBlue(false);
    }
    ThisThread::sleep_for(50ms);
  }
}
