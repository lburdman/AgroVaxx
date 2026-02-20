cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_TOOLCHAIN "GCC_ARM" CACHE STRING "Toolchain used to compile Mbed OS")
set(MBED_TARGET "NUCLEO_F429ZI" CACHE STRING "Mbed target implementation")

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")

include(${MBED_PATH}/tools/cmake/app.cmake)

project(AgroVaxx)

add_subdirectory(${MBED_PATH})

add_executable(AgroVaxx
    main.cpp
    core/StateMachine.cpp
    core/TagDedupFilter.cpp
    third_party/mfrc522/MFRC522.cpp
)

target_include_directories(AgroVaxx PRIVATE third_party/mfrc522)
target_link_libraries(AgroVaxx mbed-os)

mbed_set_post_build(AgroVaxx)

# Peripheral Tests Executables
add_executable(test_hw_rfid tests/hw_test_rfid.cpp third_party/mfrc522/MFRC522.cpp)
target_include_directories(test_hw_rfid PRIVATE third_party/mfrc522)
target_link_libraries(test_hw_rfid mbed-os)
mbed_set_post_build(test_hw_rfid)

add_executable(test_hw_ble tests/hw_test_ble.cpp)
target_link_libraries(test_hw_ble mbed-os)
mbed_set_post_build(test_hw_ble)

add_executable(test_hw_led_button tests/hw_test_led_button.cpp)
target_link_libraries(test_hw_led_button mbed-os)
mbed_set_post_build(test_hw_led_button)
