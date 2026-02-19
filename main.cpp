#include "core/StateMachine.h"
#include "drivers/MbedBluetooth.h"
#include "drivers/MbedRfidMFRC522.h"
#include "drivers/MbedSimpleDrivers.h"
#include "mbed.h"

// Hardware Pin Definitions for NUCLEO-F429ZI

// RFID SPI (MFRC522)
#define PIN_RFID_MOSI PA_7
#define PIN_RFID_MISO PA_6
#define PIN_RFID_SCLK PA_5
#define PIN_RFID_CS PE_3
#define PIN_RFID_RESET PG_1

// Bluetooth Serial (HC-05 / HM-10)
#define PIN_BLE_TX PD_6
#define PIN_BLE_RX PD_5
#define PIN_BLE_STATE PD_4

// Leds
#define PIN_LED_BLUE LED2  // NUCLEO Blue LED
#define PIN_LED_GREEN LED1 // NUCLEO Green LED

// Button
#define PIN_BUTTON USER_BUTTON // NUCLEO User Button (PC_13)

int main() {
  // Instantiate Drivers
  MbedRfidMFRC522 rfid(PIN_RFID_MOSI, PIN_RFID_MISO, PIN_RFID_SCLK, PIN_RFID_CS,
                       PIN_RFID_RESET);
  MbedBluetooth ble(PIN_BLE_TX, PIN_BLE_RX, PIN_BLE_STATE);
  MbedButton button(PIN_BUTTON);
  MbedLeds leds(PIN_LED_BLUE, PIN_LED_GREEN);
  MbedTime timeProvider;

  // Instantiate State Machine
  StateMachine sm(&rfid, &ble, &button, &leds, &timeProvider);

  sm.init();

  while (true) {
    sm.update();
    ThisThread::sleep_for(10ms); // Small delay to prevent tight polling loop
  }
}
