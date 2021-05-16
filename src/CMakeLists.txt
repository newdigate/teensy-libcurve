cmake_minimum_required(VERSION 3.5)
project(teensy_libcurve C CXX)
set(teensy_libcurve_VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

option(OPT_BUILD_FOR_LINUX "Build for linux emulator" OFF)
if (OPT_BUILD_FOR_LINUX)
    set(BUILD_FOR_LINUX 1)
endif()

set(SOURCE_FILES
        DrawingCanvas.cpp
        DrawingCanvas_st7735.cpp)

set(HEADER_FILES
        DrawingCanvas.h
        DrawingCanvas_st7735.h)

if (DEFINED BUILD_FOR_LINUX)
    include(CMakeLists.linux.cmake.in)
else()
    include(CMakeLists.teensy.cmake.in)
endif()

add_custom_target(installarduino
        "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/install_arduino_library.cmake")