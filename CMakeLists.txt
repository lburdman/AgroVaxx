cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_TOOLCHAIN "GCC_ARM" CACHE STRING "Toolchain used to compile Mbed OS")
set(MBED_TARGET "NUCLEO_F429ZI" CACHE STRING "Mbed target implementation")

add_subdirectory(mbed-os)

project(AgroVaxx)

add_executable(AgroVaxx
    main.cpp
    core/StateMachine.cpp
    drivers/MFRC522.cpp
)

target_link_libraries(AgroVaxx mbed-os)

mbed_set_post_build(AgroVaxx)

# Peripheral Tests Executables
add_executable(test_hw_rfid tests/hw_test_rfid.cpp drivers/MFRC522.cpp)
target_link_libraries(test_hw_rfid mbed-os)
mbed_set_post_build(test_hw_rfid)

add_executable(test_hw_ble tests/hw_test_ble.cpp)
target_link_libraries(test_hw_ble mbed-os)
mbed_set_post_build(test_hw_ble)

add_executable(test_hw_led_button tests/hw_test_led_button.cpp)
target_link_libraries(test_hw_led_button mbed-os)
mbed_set_post_build(test_hw_led_button)
