set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build shared (dynamic) libraries." ON)
option(BUILD_TESTS "Build tests." OFF)

if(BUILD_SHARED_LIBS)
  set(LIBRARY_TYPE SHARED)
else()
  set(LIBRARY_TYPE STATIC)
endif()

# INSTALL_LIB_DIR
set(INSTALL_LIB_DIR lib
    CACHE PATH "Relative instalation path for libraries")

# INSTALL_BIN_DIR
set(INSTALL_BIN_DIR bin
    CACHE PATH "Relative instalation path for executables")

# INSTALL_INCLUDE_DIR
set(INSTALL_INCLUDE_DIR include
    CACHE PATH "Relative instalation path for header files")


# INSTALL_CMAKE_DIR
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/${PROJECT_NAME})
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR}
    CACHE PATH "Relative instalation path for CMake files")

# Convert relative path to absolute path (needed later on)
foreach(substring LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${substring}_DIR)
  if(NOT IS_ABSOLUTE ${${var}})
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
#  message(STATUS "${var}: "  "${${var}}")
endforeach()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release'.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(LIBRARY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/xdb/)

# Version variables
set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)
set(PATCH_VERSION 0)

set(PROJECT_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

set(LIBRARY_NAME xdb)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)

# Create 'version.hpp'
configure_file(${CMAKE_SOURCE_DIR}/CMake/version.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/version.hpp" @ONLY)
