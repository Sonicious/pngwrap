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
# use C++11
set(CMAKE_CXX_STANDARD 11)

find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

# pngwrap library
add_library(pngwrap STATIC src/pngwrap.cpp)
target_include_directories(pngwrap
  PUBLIC 
    include
  PRIVATE
    src
)
target_link_libraries(pngwrap
  PNG::PNG
  ZLIB::ZLIB
)
# List the headers we want to declare as public for installation.
set(MY_PUBLIC_HEADERS
    ${PROJECT_SOURCE_DIR}/include/pngwrap.hpp
)
set_target_properties(pngwrap PROPERTIES PUBLIC_HEADER "${MY_PUBLIC_HEADERS}")

# Test-Framework:
enable_testing()
add_subdirectory(test)

# Installation
install(TARGETS pngwrap EXPORT pngwrapConfig
  ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
export(TARGETS pngwrap FILE pngwrapTargets.cmake)