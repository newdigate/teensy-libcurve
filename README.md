# teensy-libcurve
nice curves for st7735_t3 library (work in progess)

* wouldnt it be awesome if we could display SVG files on teensy? 
* also, I really hate lines that are not antialiased...  (but there is an option to turn it on and off)
* and thirdly I'd like to be able to compile and debug on linux...
* This is not an efficient way of calculating a bezier curve. 
* Some work to do when antialiasing and curve changes from steep to not steep or visa versa. 

![curves - no antialiasing](docs/curves-noantialiasing.gif)  ![curves - antialiazing](docs/curves.gif)
![curves - heart](docs/curves-heart.gif) 

## install as arduino library
you need to create shortcut/symlink to ```src``` folder in your Arduino Libraries folder

* there is a cmake target to creates a symlink to ```~/Documents/Arduino/Libraries``` (for macos)
``` shell
  > git clone https://github.com/newdigate/teensy-libcurve
  > cd teensy-libcurve/src
  > mkdir cmake-build-debug
  > cd cmake-build-debug
  > cmake -DCMAKE_BUILD_TYPE=Debug ..
  > make installarduino
```
* this will create a symlink to the `src` folder in your Arduino Libraries folder


### Xiaolin Wu's line algorithm
* [wikipedia](https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm)
