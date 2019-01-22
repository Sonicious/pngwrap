cmake_minimum_required(VERSION 3.12.3 FATAL_ERROR)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

project(pngwrap
    VERSION 0.8
    LANGUAGES CXX
    DESCRIPTION "PNG Wrapper to simplify usage"
)
# use C++11
set(CMAKE_CXX_STANDARD 11)

find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(pngwrap)

add_subdirectory(pngwrap)

# Test-Framework:
enable_testing()
add_subdirectory(tests)
