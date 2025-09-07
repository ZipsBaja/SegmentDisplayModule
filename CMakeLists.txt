# Set this to your module name!
set(MODULE_NAME ZipsTemplateModule)

add_library(${MODULE_NAME} STATIC
    src/TemplateModule.cpp
)

set(MAINLIB_DIR lib/zipslib)
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${MAINLIB_DIR})
    add_subdirectory(${MAINLIB_DIR})
endif()

target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(${MODULE_NAME}
    ZipsPicoLib
)
