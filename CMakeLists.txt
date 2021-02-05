PROJECT(xdb)

CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake/ ${CMAKE_ROOT}/Modules/   )

include(setup)
include(dependencies)

add_subdirectory(src)
#add_subdirectory(test)

include(install)

