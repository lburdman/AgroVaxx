# AgroVaxx

AgroVaxx is an embedded C++ system for securing and digitizing livestock vaccination sessions. It validates RFID tags, verifies user confirmation via a physical button, ensures the same animal isn't vaccinated twice, and transmits the real-time session logs to a smartphone app via Bluetooth UART.

The core business logic is highly decoupled from the hardware drivers, enabling test-driven development on the host machine.

## Architecture

- **`core/`**: Hardware-agnostic State Machine logic. Tested natively using Google Test / Single-header assert frameworks.
- **`drivers/`**: Mbed OS 6 wrappers for the physical peripherals on the STM32 NUCLEO-F429ZI.
- **`tests/`**: Unit tests and standalone peripheral test executables.

## Hardware Wiring (NUCLEO-F429ZI)

| Peripheral | NUCLEO-F429ZI Pin | Description |
|---|---|---|
| MFRC522: SCK | PA_5 | SPI Clock |
| MFRC522: MISO | PA_6 | SPI MISO |
| MFRC522: MOSI | PA_7 | SPI MOSI |
| MFRC522: CS / SDA | PE_3 | SPI Chip Select |
| MFRC522: RST | PG_1 | Reset |
| Bluetooth: RX | PD_6 | UART TX from STM32 |
| Bluetooth: TX | PD_5 | UART RX to STM32 |
| Bluetooth: STATE | PD_4 | BLE Connection Status Pin |
| User Button | PC_13 | Standard NUCLEO Button (Confirmation) |
| Blue LED | LED2 (PB_7) | Standard NUCLEO Blue LED (BT Status) |
| Green LED | LED1 (PB_0) | Standard NUCLEO Green LED (Action Status) |

*Note: HC-05/HM-10 modules often operate at 3.3V logic, but check your specific module to ensure it matches the STM32's 3.3V pins to prevent damage.*

## How to Build & Flash with Mbed CLI 2 (Mbed OS 6)

1. Ensure you have the `gcc-arm-none-eabi` toolchain, `cmake`, and `ninja` installed.
2. Install Mbed tools: `pip install mbed-tools`.
3. Configure the build:
   ```bash
   mbed-tools configure -m NUCLEO_F429ZI -t GCC_ARM
   ```
4. Compile the project:
   ```bash
   mbed-tools compile -m NUCLEO_F429ZI -t GCC_ARM
   ```
5. A `.bin` file will be generated in `cmake_build/NUCLEO_F429ZI/develop/GCC_ARM/`. Copy this `.bin` file to the `NODE_F429ZI` flash drive that appears when the NUCLEO is plugged in.

## Running Tests

### Unit Tests
The Core State Machine unit tests do not require real hardware and run natively.
```bash
cd tests
mkdir build && cd build
cmake ..
make
./test_state_machine
```

*(On macOS, without CMake, compile using standard tools: `clang++ -std=c++14 test_state_machine.cpp ../core/StateMachine.cpp -o run_tests && ./run_tests`)*

### Hardware Peripheral Tests
The system comes with standalone firmware modes to test each peripheral independently. During compilation, Mbed Tools will build them alongside `AgroVaxx.bin`.
Flash the individual `.bin` files generated from `test_hw_rfid`, `test_hw_ble`, and `test_hw_led_button` to inspect outputs on your Serial Monitor (115200 Baud).

## MIT App Inventor
See `docs/app_inventor_instructions.md` for a complete breakdown of how to build a smartphone application capable of interfacing with this scanner.
