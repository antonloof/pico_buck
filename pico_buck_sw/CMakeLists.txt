# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "C:/Users/Anton/Documents/Pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_buck C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pico_buck pico_buck.c )

pico_set_program_name(pico_buck "pico_buck")
pico_set_program_version(pico_buck "0.1")

pico_enable_stdio_uart(pico_buck 0)
pico_enable_stdio_usb(pico_buck 1)

# Add the standard library to the build
target_link_libraries(pico_buck pico_stdlib)
pico_generate_pio_header(pico_buck ${CMAKE_CURRENT_LIST_DIR}/sync_buck_pwm.pio)

# Add any user requested libraries
target_link_libraries(pico_buck
        hardware_pio
        hardware_clocks
        hardware_pll
        hardware_dma
        hardware_irq
)

pico_add_extra_outputs(pico_buck)

