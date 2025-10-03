cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

set(PICO_BOARD pico)
#set(PICO_BOARD pico_w)
#set(PICO_BOARD pico2)

set(PICO_PLATFORM rp2040)
# set(PICO_PLATFORM rp2350-arm-s)

if (PICO_BOARD STREQUAL "pico")
add_compile_definitions(USING_PICO)
set(PICO_PLATFORM rp2040)
set(PICO_LABEL RPI-RP2)
elseif(PICO_BOARD STREQUAL "pico_w")
add_compile_definitions(USING_PICO_W)
set(PICO_PLATFORM rp2040)
set(PICO_LABEL RPI-RP2)
elseif(PICO_BOARD STREQUAL "pico2")
add_compile_definitions(USING_PICO_2)
set(PICO_PLATFORM rp2350)
set(PICO_LABEL RP2350)
elseif(PICO_BOARD STREQUAL "pico2_w")
add_compile_definitions(USING_PICO_2_W)
set(PICO_PLATFORM rp2350)
set(PICO_LABEL RP2350)
endif()

include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

set(C_STANDARD 17)
set(CXX_STANDARD 17)

# Enable and disable features here.
option(USING_PRINT "Enable printing to stdout. Use this while testing and debugging." ON) # Switch to OFF when project is complete.
option(USING_MULTICORE "Enable use of the Pico's second core. Enable this for parallel processing or separate tasks." ON)

# Add theses definitions to your IDE.
if (USING_PRINT)    
add_compile_definitions(USING_PRINT=1)
set(USE_PRINT_CMAKE 1)
else()
set(USE_PRINT_CMAKE 0)
endif()
if (USING_MULTICORE)
add_compile_definitions(USING_MULTICORE=1)
set(USE_MULTICORE_CMAKE 1)
else()
set(USE_MULTICORE_CMAKE 0)
endif()

set(PICO_CXX_ENABLE_RTTI 1)

pico_sdk_init()

project(Example C CXX ASM)

add_subdirectory(../ build)

add_executable(Example src/main.cpp)

target_link_libraries(Example
    ZipsSegmentDisplayModule
    pico_stdlib
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(Example)