# teensy-libcurve
nice curves for st7735_t3 library (work in progess)

* wouldnt it be awesome if we could display SVG files on teensy? 
* also, I really hate lines that are not antialiased...  (but there is an option to turn it on and off)
* and thirdly I'd like to be able to compile and debug on linux...
* This is not an efficient way of calculating a bezier curve. 
* Some work to do when antialiasing and curve changes from steep to not steep or visa versa. 

![curves - no antialiasing](docs/curves-noantialiasing.gif)  ![curves - antialiazing](docs/curves.gif)
![curves - heart](docs/curves-heart.gif) 

## download
* download and uncompress .zip
* or ```git clone https://github.com/newdigate/teensy-libcurve```

## build
### teensy
```
  > cd teensy-libcurve
  > mkdir cmake-build-teensy && cd cmake-build-teensy
  > cmake ../src -DCMAKE_BUILD_TYPE=Debug
  > make
```
### linux
```
  > cd teensy-libcurve
  > mkdir cmake-build-linux && cd cmake-build-linux
  > cmake ../src -DCMAKE_BUILD_TYPE=Debug -DOPT_BUILD_FOR_LINUX=ON
  > make
```

## install 
### arduino library
* create shortcut/symlink to ```src``` folder in your Arduino Libraries folder
* there is a cmake target to creates a symlink to ```~/Documents/Arduino/Libraries``` (for macos)
``` shell
  > cd teensy-libcurve/src
  > mkdir cmake-build-debug
  > cd cmake-build-debug
  > cmake -DCMAKE_BUILD_TYPE=Debug ..
  > make installarduino
```
### linux library
```
  > cd teensy-libcurve
  > mkdir cmake-build-linux && cd cmake-build-linux
  > cmake ../src -DCMAKE_BUILD_TYPE=Debug -DOPT_BUILD_FOR_LINUX=ON
  > sudo make install
```

### Xiaolin Wu's line algorithm
* [wikipedia](https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm)
