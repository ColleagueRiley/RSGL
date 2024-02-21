## RSGL
### Riley's Simple GUI Library 
#

![The RSGL logo](https://github.com/ColleagueRiley/RSGL/blob/main/logo.png?raw=true)

A simple GUI library currently for Linux, Windows and MacOS

# Build statuses
![cplus workflow](https://github.com/ColleagueRiley/RSGL/actions/workflows/linux.yml/badge.svg)
![cplus workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/windows.yml/badge.svg)
![cplus workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/macos.yml/badge.svg)

# header-only
  In order to "link" RSGL you must add the line 
  ```c
  #define RSGL_IMPLEMENTATION
  ```
  before you include RSGL

  you can also use the command line argument `-DRSGL_IMPLEMENTATION`

  or have RSGL's defined in a seperate file\
  which I would suggest you do if you're going to use any optimization args\
  or compile with miniaudio

# compiling without RSGL_audio
If you want to compile without RSGL_AUDIO you have to add the line

```c
#define RSGL_NO_AUDIO
```

before you include RSGL\
however, if you're not linking audio because it is compiling slower

I'd suggest you add `#define RSGL_NO_MINIAUDIO_IMPLEMENTATION`\
and then link miniaudio in your own way\
OR, add `  #define RSGL_IMPLEMENTATION` to a seperate source file and link RSGL on it's own

or building RSGL...

# Building
  Building RSGL isn't required in any sense

  but if you wanted to, here's how

  ```sh
  ./configure
  make
  ```

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
  #define  RSGL_NO_WIDGETS (makes it so RSGL doesn't include widget functions)\
  #define  RSGL_NO_AUDIO (makes it so RSGL doesn't include audio functions)\
  #define RSGL_NO_WINDOW - no RSGL_window, RSGL_graphics is used instead [this is for using a differnt window manager other than RGFW ]\
  #define RSGL_NO_TEXT - do not include text rendering functions\
  #define RGFW_NO_WIDGETS - do not include widgets\
  #define RSGL_NO_AUDIO - do not include audio functions\
  #define RSGL_NO_MINIAUDIO_IMPLEMENTATION - do not have `#define MINIAUDIO_IMPLEMENTATION` in this header (you'll have to link miniaudio some other way to use audio)\
  #define RSGL_NO_SAVE_IMAGE - do not save/load images (don't use RSGL_drawImage if you use this), \
                                  RSGL_drawImage saves the file name + texture so it can load it\
                                  when you ask for it later. This disables that\

# Dependencies
  All of RSGL's (non-native) dependencies are built-in, 

# compiling
if you wish to compile the library all you have to do is create a new file with this in it

RSGL.c
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

then you can use gcc (or whatever compile you wish to use) to compile the library into object file

ex. gcc -c RSGL.c -fPIC

after you compile the library into an object file, you can also turn the object file into an static or shared library

(commands ar and gcc can be replaced with whatever equivalent your system uses)
static : ar rcs RSGL.a RSGL.o
shared :
  windows:
    gcc -shared RSGL.o  -lshell32 -lgdi32 -o RSGL.dll
  linux:
    gcc -shared RSGL.o -lX11 -lXcursor -o RSGL.so
  macos:
    <Silicon/include> can be replaced to where you have the Silicon headers stored
    <libSilicon.a> can be replaced to wherever you have libSilicon.a
    gcc -shared RSGL.o -framework Foundation <libSilicon.a> -framework AppKit -framework CoreVideo -I<Silicon/include>

installing/building silicon (macos)

Silicon does not need to be installde per se.
I personally recommended that you use the Silicon included using RGFW

to build this version of Silicon simplly run

cd Silicon && make

you can then use Silicon/include and libSilicon.a for building RGFW projects

ex.
gcc main.c -framework Foundation -lSilicon -framework AppKit -framework CoreVideo -ISilicon/include

I also suggest you compile Silicon (and RGFW if applicable)
per each time you compile your application so you know that everything is compiled for the same architecture.

# RGFW
  RGFW, Riley's Gui library FrameWork.

  RSGL's window management is built off of RGFW!

  RGFW is made as an very small, single-header-file alternative to GLFW. \
  RGFW also tries to be even more flexable than GLFW. 

  [![AltText](https://github.com/ColleagueRiley/ColleagueRiley/blob/main/rgfw.png?raw=true)](https://github.com/ColleagueRiley/RGFW)\
  (this is a button that leads to the RGFW repo)

  RGFW is considered apart of the "RSGL Project".

# credits

# RGL
  [RGL](https://github.com/ColleagueRiley/RGL) is an OpenGL abstraction layer (that I created) that makes opengl version managing and rendering a lot easier. 

  It allows RSGL render using a simplified pipeline system while raylib does advanced shader and preformance work in the background

  RGL is heavily based on RLGL but it's designed to be more lightweight and have better performance and optimization 

# RFont
  [RFont](https://github.com/colleagueRiley/RFont) is a super light-weight and modular font library for texture atlas and text rendering that I created. RSGL uses this for text rendering.


# stb_image 
  [stb_image](https://github.com/nothings/stb/) is an public domain single header image loader library, RSGL uses this for loading images

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

  However, some newer additions were inspired by Raylib including it's modular design, RLGL (which RGL is based on) and verbos examples. 

# Eima
  Since I started RSGL, ![Eima](https://github.com/EimaMei) has helped boost the morale of the project, has helped with coming up with ideas for the project, has helped when debating new featues of the project
  and even has contributed some code to the project. 

  Eima also the original creator of Silicon, without Silicon RSGL/RGFW probably would not support native macOS

  ![Eima's C toolkit / STL (sili)](https://github.com/Eimamei/sili)
  
# RSGL-Architecture diagram

![alt text](https://github.com/ColleagueRiley/RSGL/blob/development/RSGL-Architecture.png?raw=true)