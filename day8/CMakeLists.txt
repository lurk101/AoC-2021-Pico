cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
pico_sdk_init()
project(day8 C CXX ASM)
add_executable(day8 day8.cpp)
pico_set_program_name(day8 "day8")
pico_enable_stdio_uart(day8 1)
pico_enable_stdio_usb(day8 0)
target_link_libraries(day8 pico_stdlib)
pico_add_extra_outputs(day8)
