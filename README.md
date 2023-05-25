## RSGL
### Riley's Simple GUI Library 
#

![alt text](https://github.com/ColleagueRiley/RSGL/blob/main/logo.png?raw=true)

A simple GUI library currently for Linux, Windows and MacOS

# Building
  to built simply run `make build`

  to customize the build you can put pre-defined vars after `make build`
  ex.
  
  `make GXX=g++(c++ compiler) COMPILE=false (just install headers) HEADERS-ONLY=false (don't move source files with headers (don't support installed header-only option)` 

# header-only
  To "link" RSGL with the header-only option simply add `#define RSGL_IMPLEMENTATION` before you include RSGL ex.

  ```cpp
  #define RSGL_IMPLEMENTATION
  #include <RSGL.hpp>
  ```

  However, compiling a code like this takes a lot longer, so it may be a good idea to link compiled sources instead. 


# installing
  There's two ways to install, either using the Makefile or by hand, installing by hand is a good idea if you're crosscompiling

  to install with the make file run `make install` or `sudo make install`, if you're on linux and need the perms

  installing also has the custom variable `INSTALL_PATH`, this var is required if you're using windows

  ex.

  `make install INSTALL_PATH=/usr(where to find lib and include folder)`

  to install by hand simply,
    move ./build/lib/*.a to the proper *.a lib folder (ex. /usr/local/lib)
    `cp ./build/lib/*.a /usr/local/lib`

    move ./build/lib/*.so (or *.dll for windows) to the proper lib folder (ex. /usr/lib)
    `cp ./build/lib/*.so /usr/lib`

    move the contents of ./build/bin and ./bin/include to the proper include (.ex /usr/include) and (.ex /usr/bin) folders 
    `cp -r ./build/include/* /usr/include`

    `cp ./build/bin/* /usr/bin`

# Modules 
  #define  RSGL_NO_WIDGETS (makes it so RSGL doesn't include widget functions)
  #define  RSGL_NO_AUDIO (makes it so RSGL doesn't include audio functions)

# Dependencies
  All of RSGL's (non-native) dependencies are built-in, 

# compiling
  you can compile two ways, with the shared library, which is as simple as linking `libRSGL.so` or `-lRSGL` (if it's installed), you can also do `$((pkg-config RSGL -libs)`

  to compile statically, you can either link with `libRSGL.a` with the proper dependencies yourself or with RSGL-static

  linking statically with pkg-config is as simple as adding `$(pkg-config RSGL -libs -static)` to your compiler command

  ex. `g++ main.cpp $(pkg-config RSGL -libs -static)`

  in a Makefile, you can simply add a line such as `LIBS = $(pkg-config RSGL -libs -static)` and link with `$(LIBS)`

# RGFW
  RGFW, Riley's Gui library FrameWork is used as a backend for RSGL.

  RGFW is made as an very small, single-header-file alternative to GLFW. \
  RGFW also tries to be even more flexable than GLFW. 

  [![AltText](https://github.com/ColleagueRiley/ColleagueRiley/blob/main/rgfw.png?raw=true)](https://github.com/ColleagueRiley/RGFW)\
  (this is a button that leads to the RGFW repo)

  RGFW is considered apart of the "RSGL Project".

# credits

# RLGL
  RLGL is an OpenGL abstraction layer that makes opengl version managing and rendering a lot easier. 

  It allows RSGL render using a simplified pipeline system while raylib does advanced shader and preformance work in the background

# stb_image 
  stb_image is an public domain single header image loader library, RSGL uses this for loading images

  repo : https://github.com/nothings/stb/

# fontstatsh
  Fontstash is light-weight font texture atlas and text rendering. RSGL uses this for text rendering

  repo : https://github.com/memononen/fontstash

# nanosvgrast
  nanosvgrast is a library for simply parsing rasterizing .SVG files, RSGL uses this for drawing SVG files as well as for loading any other vector graphics

  repo : https://github.com/memononen/nanosvg

# miniaudio
  mini audio is an public domain audio playback and capture library. RSGL uses this for playing/loading music

  miniaudio webpage : https://miniaud.io/

# Silicon
  Silicon is a C binding for MacOS's Cocoas library. This library makes working with apple's API a lot better because you don't have to directly work with Objective-C.
  
  RSGL uses silicon via RGFW

  ![https://github.com/EimaMei/Silicon](Silicon Github)

# Raylib
  Because this project was created and the majority of features were created before my learning of Raylib, I am very surprised to see how simular Raylib is to RSGL. 
  Raylib and it's community seems to have a very simular view of what a good GUI library should be like as RSGL does. So thank you Raylib for showing me that RSGL is not alone.

  However, some newer additions were inspired by Raylib including it's modular design, use of RLGL (which is created for raylib) and verbos examples. 

# Eima
  Since I started RSGL, ![Eima](https://github.com/EimaMei) has helped boost the morale of the project, has helped with coming up with ideas for the project, has helped when debating new featues of the project
  and even has contributed some code to the project. 

  Eima also made Silicon, without Silicon RSGL/RGFW probably would not support native macOS

  ![Eima's own programming launage for HOI 4](https://github.com/EimaMei/HPL/)
  
# RSGL-Architecture diagram

![alt text](https://github.com/ColleagueRiley/RSGL/blob/development/RSGL-Architecture.png?raw=true)