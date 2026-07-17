cmake_minimum_required(VERSION 3.13)
project(PortaPack_H2_Xanthorox_Firmware C CXX ASM)

# Cross-compile for ARM Cortex-M4 (typical PortaPack H2)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain (adjust paths)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)

# Flags for embedded
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -Wall -Wextra -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_SOURCE_DIR}/linker.ld -Wl,--gc-sections --specs=nano.specs")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/ui
    ${CMAKE_SOURCE_DIR}/rf_core
    ${CMAKE_SOURCE_DIR}/src/apps
)

# Source files - add all .c as you implement
file(GLOB_RECURSE SOURCES 
    "src/*.c"
    "ui/*.c"
    "rf_core/*.c"
    "src/apps/*/*.c"
)

add_executable(portapack_xanthorox ${SOURCES})

# Link libraries (add LVGL, HackRF lib, FreeRTOS if used)
# target_link_libraries(portapack_xanthorox lvgl hackrf etc.)

# Post-build: generate .bin
add_custom_command(TARGET portapack_xanthorox POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary portapack_xanthorox portapack_xanthorox.bin
    COMMENT "Generating flashable .bin"
)