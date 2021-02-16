cmake_minimum_required(VERSION 3.5)
project(teensy_libcurve C CXX)
set(CMAKE_CXX_STANDARD 11)

set(BUILD_FOR_LINUX 1)

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