# Set this to your module name!
set(MODULE_NAME ZipsSegmentDisplayModule)

add_library(${MODULE_NAME} STATIC
    src/SegmentDisplayModule.cpp
)

set(MAINLIB_DIR lib/zipslib)
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${MAINLIB_DIR})
    add_subdirectory(${MAINLIB_DIR})
endif()

include(${CMAKE_CURRENT_LIST_DIR}/lib/TM1637-pico/PicoTM1637.cmake)

target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(${MODULE_NAME}
    ZipsPicoLib
    PicoTM1637
)
