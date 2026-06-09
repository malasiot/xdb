cmake_minimum_required(VERSION 3.10)
project(xdb LANGUAGES CXX)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake/ ${CMAKE_ROOT}/Modules/   )

include(setup)
include(dependencies)

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    add_subdirectory(test)
endif()

#include(install)

