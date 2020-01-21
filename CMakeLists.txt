cmake_minimum_required(VERSION 3.12.3 FATAL_ERROR)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

include(GNUInstallDirs)

project(pngwrap
  VERSION 0.8
  LANGUAGES CXX
  DESCRIPTION "PNG Wrapper to simplify usage"
)

# Set a default build type if none was specified
set(default_build_type "Release") 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
# Select flags for non multi target systems:
if(NOT CMAKE_CONFIGURATION_TYPES)
  SET(CMAKE_CXX_FLAGS "-Wall -fno-rtti")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
  SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
  SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")
endif()

find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

# pngwrap library
add_library(pngwrap STATIC src/pngwrap.cpp)
set_property(TARGET pngwrap PROPERTY CXX_STANDARD 11)
target_include_directories(pngwrap
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    src
)
target_link_libraries(pngwrap
  PNG::PNG
  ZLIB::ZLIB
)
# List the headers we want to declare as public for installation.
set_target_properties(pngwrap PROPERTIES PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/include/pngwrap.hpp)

# Test-Framework:
enable_testing()
add_subdirectory(test)

# Installation and exporting
install(TARGETS pngwrap EXPORT pngwrapConfig
  ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT pngwrapConfig 
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pngwrap
  NAMESPACE pngwrap::
  FILE pngwrapConfig.cmake
)
export(TARGETS pngwrap
  NAMESPACE pngwrap::
  FILE pngwrapTargets.cmake
)