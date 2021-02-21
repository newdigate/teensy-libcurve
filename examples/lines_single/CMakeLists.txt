cmake_minimum_required(VERSION 3.5)
project(teensy_st7735_linux_extras_opengl_examples_lines C CXX)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES lines_single.cpp)
set(APP_NAME lines_single)
option(OPT_BUILD_FOR_LINUX "Build the example for linux" ON)
option(OPT_BUILD_FOR_OPENGL_EMULATOR "Build the example for linux/opengl" ON)
option(OPT_BUILD_EXAMPLES_USING_INSTALLED_LIB "Build examples using installed teensy-libcurve" ON)

IF (OPT_BUILD_FOR_LINUX)
    add_definitions(-DBUILD_FOR_LINUX)
    message(STATUS "BUILD_FOR_LINUX: Build the example for linux is ON")
    set(APP_SOURCES ${SOURCE_FILES})

    find_package(teensy_x86_stubs)
    include_directories(${teensy_x86_stubs_INCLUDE_DIR})

    find_package(teensy_st7735_linux_stubs)
    include_directories(${teensy_st7735_linux_stubs_INCLUDE_DIR})

    if (OPT_BUILD_FOR_OPENGL_EMULATOR)
        add_definitions(-DBUILD_FOR_OPENGL_EMULATOR)
        message(STATUS "BUILD_FOR_OPENGL_EMULATOR: Build the example for linux/opengl emulator is ON")
        include(../../cmake/opengl.cmake.in)
        find_package(teensy_st7735_linux_opengl)
        include_directories(${teensy_st7735_linux_extras_opengl_INCLUDE_DIR})
    endif()

    if (OPT_BUILD_EXAMPLES_USING_INSTALLED_LIB)
        message(STATUS "BUILD_EXAMPLES_USING_INSTALLED_LIB: Build the example for linux/opengl emulator using install teensy-libcurve is ON")
        find_package(teensy_libcurve)
        include_directories(${teensy_libcurve_INCLUDE_DIR})
    else()
        include_directories(../../src/)
        set(APP_SOURCES ${SOURCE_FILES} ../../src/DrawingCanvas_st7735.cpp ../../src/DrawingCanvas.cpp)
    endif()

    add_executable(${APP_NAME} ${APP_SOURCES})

    target_link_libraries(${APP_NAME} ${teensy_x86_stubs_LIBS})
    target_link_libraries(${APP_NAME} ${teensy_st7735_linux_stubs_LIBS})
    if (OPT_BUILD_FOR_OPENGL_EMULATOR)
        target_link_libraries(${APP_NAME} ${teensy_st7735_linux_extras_opengl_LIBS} ${LIBS})
    endif()
    if (OPT_BUILD_EXAMPLES_USING_INSTALLED_LIB)
        message(STATUS "OPT_BUILD_EXAMPLES_USING_INSTALLED_LIB: ${teensy_libcurve_LIBS}")
        target_link_libraries(${APP_NAME} ${teensy_libcurve_LIBS})
    endif()
    set(CMAKE_VERBOSE_MAKEFILE 1)
    set(CMAKE_AUTOGEN_VERBOSE 1)
ELSE()
    set(TEENSY_VERSION 40 CACHE STRING "Set to the Teensy version corresponding to your board (30 or 31 allowed)" FORCE)
    set(CPU_CORE_SPEED 600000000 CACHE STRING "Set to 24000000, 48000000, 72000000 or 96000000 to set CPU core speed" FORCE) # Derived variables
    set(CPU cortex-m7 CACHE STRING "Set to cortex-m7, cortex-m4")

    set(COMPILERPATH "/Applications/ARM/bin/")
    set(DEPSPATH "/Users/moolet/Development/newdigate/temp_dep")
    set(COREPATH "${DEPSPATH}/cores/teensy4/")

    find_package(teensy_cmake_macros)

    import_arduino_library(${DEPSPATH} SPI)
    import_arduino_library(${DEPSPATH} SdFat/src)
    import_arduino_library(${DEPSPATH} SdFat/src/common)
    import_arduino_library(${DEPSPATH} SdFat/src/DigitalIO)
    import_arduino_library(${DEPSPATH} SdFat/src/ExFatLib)
    import_arduino_library(${DEPSPATH} SdFat/src/FatLib)
    import_arduino_library(${DEPSPATH} SdFat/src/FsLib)
    import_arduino_library(${DEPSPATH} SdFat/src/iostream)
    import_arduino_library(${DEPSPATH} SdFat/src/SdCard)
    import_arduino_library(${DEPSPATH} SdFat/src/SpiDriver)
    import_arduino_library(${DEPSPATH} SD/src)
    import_arduino_library(${DEPSPATH} ST7735_t3)
    import_arduino_library(${DEPSPATH} Wire)
    import_arduino_library(${DEPSPATH} arm_math/src)

    import_arduino_library_absolute(${CMAKE_SOURCE_DIR}/../../src)

    teensy_add_executable(teensy_libcurve_lines_example
            lines_single.cpp)
ENDIF()


