# teensy-libcurve
[![teensy-libcurve](https://github.com/newdigate/teensy-libcurve/actions/workflows/teensy.yml/badge.svg)](https://github.com/newdigate/teensy-libcurve/actions/workflows/teensy.yml)
[![teensy-libcurve-ubuntu-x86](https://github.com/newdigate/teensy-libcurve/actions/workflows/ubuntu_x64_cmake.yml/badge.svg)](https://github.com/newdigate/teensy-libcurve/actions/workflows/ubuntu_x64_cmake.yml)

nice curves for [PaulStoffregen/ST7735_t3](https://github.com/PaulStoffregen/ST7735_t3) and [newdigate/teensy-st7735-linux-stubs](https://github.com/newdigate/teensy-st7735-linux-stubs) 

* work in progress 
* antialiased lines and bezier curves  (there is an option to turn antialiasing on / off)
* compile for teensy or linux
* disclaimer - this is not an efficient way of calculating a bezier curve. 
* potential improvements
  * tiny distortion when antialiasing and curve changes from steep to not steep or visa versa

![curves - no antialiasing](docs/curves-noantialiasing.gif)  ![curves - antialiazing](docs/curves.gif)
![curves - heart](docs/curves-heart.gif) 

## contents
* [download](#download)
* [teensy](#teensy)
  * [dependencies](#dependencies)
  * [build](#build)
  * [install arduino library](#install-arduino-library)
* [linux](#linux)
  * [dependencies](#dependencies) 
  * [build](#build)
  * [install](#install) 
* [Xiaolin Wu's line algorithm](#xiaolin-wus-line-algorithm)

## download
* download and uncompress .zip
* or ```git clone https://github.com/newdigate/teensy-libcurve```

## teensy
### dependencies
* [arm-eabi-none-gcc](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
* [newdigate](https://github.com/newdigate)
  * [teensy-cmake-macros](https://github.com/newdigate/teensy-cmake-macros) 
* [PaulStoffregen](https://github.com/PaulStoffregen)
  * [cores](https://github.com/PaulStoffregen/cores)
  * [ST7735_t3](https://github.com/PaulStoffregen/ST7735_t3)
  * [SPI](https://github.com/PaulStoffregen/SPI)
### build
* clone the dependencies above to a chosen folder (in this case ```~/temp_dep```)
``` shell
 > mkdir ~/temp_dep && cd ~/temp_dep
 > git clone https://github.com/PaulStoffregen/cores
 > git clone https://github.com/PaulStoffregen/ST7735_t3
 > git clone https://github.com/PaulStoffregen/SPI
```
* open ```src/CMakeLists.teensy.cmake.in```
  * update ```set(COMPILERPATH "/Applications/ARM/bin/")``` 
    * point to installation bin folder of [arm-eabi-none-gcc](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
  * update ```set(DEPSPATH "/Users/xxx/temp_dep")``` 
    * point to dependencies folder     
* build 
``` shell
  > cd teensy-libcurve
  > mkdir cmake-build-teensy && cd cmake-build-teensy
  > cmake ../src -DCMAKE_BUILD_TYPE=Debug
  > make
```

### install arduino library
* create shortcut/symlink to ```src``` folder in your Arduino Libraries folder
* there is a cmake target to creates a symlink to ```~/Documents/Arduino/Libraries``` (for macos)
``` shell
  > cd teensy-libcurve/src
  > mkdir cmake-build-debug
  > cd cmake-build-debug
  > cmake -DCMAKE_BUILD_TYPE=Debug ..
  > make installarduino
``` 


## linux
### dependencies
 * [newdigate](https://github.com/newdigate)
   * [teensy-x86-stubs](https://github.com/newdigate/teensy-x86-stubs)
   * [teensy-st7735-linux-stubs](https://github.com/newdigate/teensy-st7735-linux-stubs)
### build
  * install dependencies
    * install each dependency as per instructions on repository ```README.md``` 
  * build
``` shell
  > cd teensy-libcurve
  > mkdir cmake-build-linux && cd cmake-build-linux
  > cmake ../src -DCMAKE_BUILD_TYPE=Debug -DOPT_BUILD_FOR_LINUX=ON
  > make
```
### install 
``` shell
  > cd teensy-libcurve
  > mkdir cmake-build-linux && cd cmake-build-linux
  > cmake ../src -DCMAKE_BUILD_TYPE=Debug -DOPT_BUILD_FOR_LINUX=ON
  > sudo make install
```

## Xiaolin Wu's line algorithm
* [wikipedia](https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm)
