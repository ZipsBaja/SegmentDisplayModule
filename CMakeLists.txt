# Set this to your module name!
set(MODULE_NAME ZipsTemplateModule)

add_library(${MODULE_NAME} STATIC
    src/TemplateModule.cpp
)

##### For testing only! Everything commented below must be re-commented out when pushing on Git.
#add_subdirectory(lib/zipslib)

target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(${MODULE_NAME}
#   ZipsPicoLib
)
