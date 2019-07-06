cmake_minimum_required(VERSION 3.0)

include(config.cmake)
set(CMAKE_TOOLCHAIN_FILE cmake/arduino.cmake)
project(oledTemp C CXX ASM)

add_executable_avr(ot
    src/main.cpp
)

target_link_arduino_library(ot gfx external/Adafruit-GFX-Library)
target_link_arduino_library(ot ssd1306 external/Adafruit_SSD1306)
target_include_directories(ssd1306 PRIVATE external/Adafruit-GFX-Library)

target_link_arduino_library(ot ds18b20 external/Arduino-Temperature-Control-Library)
target_include_directories(ds18b20 PRIVATE external/OneWire)
target_link_arduino_library(ot onewire external/OneWire)

# Function add_executable_avr will build arduino-core, hence we can easily link it
target_link_libraries(ot
    arduino-core
    SPI
    Wire
)

