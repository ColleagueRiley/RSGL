## RSGL
### Riley's Simple GUI Library 
#

![The RSGL logo](https://github.com/ColleagueRiley/RSGL/blob/main/RSGL_logo.png?raw=true)

RSGL is A modular simple-to-use cross-platform GUI library for easily creating GUI apps and games. It combines the freedom of lower-level GUI libraries with modern C techniques, offering both simplicity and convenience. 

It currently supports Linux, Windows and MacOS

# Build statuses
![cplus workflow](https://github.com/ColleagueRiley/RSGL/actions/workflows/linux.yml/badge.svg)
![cplus workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/windows.yml/badge.svg)
![cplus workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/macos.yml/badge.svg)

# Features
- No external dependencies, all the libraries required are included in RSGL and are also very lightweight\
- Supports multiple platforms, windows, MacOS, linux, ect
- Supports multiple versions of OpenGL (even allowing you to switch during runtime)
- Uses other small lightweight dependencies
- OpenGL abstraction layer : RGL (which is its own single-header library too)
- Supports multiple font and image formats due to `stb_truetype.h` and `stb_image.h`
- Supporst multiple audio formats due to `miniaudio.h`
- Many examples included
- Free and Open Source with a very flexible license

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

# Modules 
  #define RSGL_NO_WIDGETS (makes it so RSGL doesn't include widget functions)\
  #define RSGL_NO_AUDIO (makes it so RSGL doesn't include audio functions)\
  #define RSGL_NO_WINDOW - no RSGL_window, RSGL_graphics is used instead [this is for using a differnt window manager other than RGFW ]\
  #define RSGL_NO_TEXT - do not include text rendering functions\
  #define RGFW_NO_WIDGETS - do not include widgets\
  #define RSGL_NO_AUDIO - do not include audio functions\
  #define RSGL_NO_MINIAUDIO_IMPLEMENTATION - do not have `#define MINIAUDIO_IMPLEMENTATION` in this header (you'll have to link miniaudio some other way to use audio)\
  #define RSGL_NO_SAVE_IMAGE - do not save/load images (don't use RSGL_drawImage if you use this), \
                                  RSGL_drawImage saves the file name + texture so it can load it\
                                  when you ask for it later. This disables that\

# Examples

## a basic start up example

```c
#define RSGL_NO_AUDIO /* RSGL uses miniaudio.h, and I don't wanna compile it if I'm not using it */
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main() {
    RSGL_window* win = RSGL_createWindow("name", RSGL_RECT(0, 0, 500, 500), RSGL_CENTER);

    bool running = true;

    while (running) {
      while (RSGL_window_checkEvent(win)) {
          if (win->event.type == RSGL_quit) {
            running = false;
            break;
          }
      }

      RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
      RSGL_window_clear(win, RSGL_RGB(255, 255, 255));
    }

    RSGL_window_close(win);
}
```

this can be compiled with :\
linux:
`gcc <file> -lX11 -lGLX -lm -o example`
windows :
`gcc <file> -lopengl32 -lgdi32 -lshell32 -o example`
macos :
`gcc <file> -lm -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo -o example`

Feel free to copy from either of the included makefiles (`Makefile` and `examples/Makefile`) if you wish.

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

## compiling 
these examples can be compiled using the make file in `examples/`\
or running `make examples` in the current directory\

you can either compile all the example using `make`,\
compile one specific example using `make <example>` or\
running `make debug` which compiles and runs each example in debug mode

## basic.c 
`examples/basic.c` is a basic example which goes through many of the window managing features RSGL has\
It also shows multiple ways of drawing a triangle

## events.c
`example/events.c` is a example which shows off all the events that RSGL has and prints out their event data into the terminal

## glfw.c
`examples/glfw.c` is a example which shows off how you can use RSGL's rendering functions seperate from the windowing backend

## glVer.c
`examples/glVer.c` is a example which shows how RSGL can switch between legacy and modern opengl\
in the example, you can toggle between opengl 3.0+ and opengl 2.0- by pressing the spacebar

## shapes.c
`examples/shapes.c` is a example which shows off shape rendering using RSGL, the shapes rotate and switch between being filled and unfilled 

## text.c
`examples/text.c` is an example of text rendering using RSGL

## textures.c
`examples/textures.c` is an example which shows off rendering textures on a shape, the textures switch between being enabled and disabled as the shapes spin

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

  Why use RSGL instead of Raylib? RSGL has more dynamic features than raylib and is more lightweight than raylib.\
  RSGL's dependencies are also very lightweight and modular as compared to Raylib's which are bloated in one way or the other.

# Eima
  Since I started RSGL, ![Eima](https://github.com/EimaMei) has helped boost the morale of the project, has helped with coming up with ideas for the project, has helped when debating new featues of the project
  and even has contributed some code to the project. 

  Eima also the original creator of Silicon, without Silicon RSGL/RGFW probably would not support native macOS

  ![Eima's C toolkit / STL (sili)](https://github.com/Eimamei/sili)
  
# RSGL-Architecture diagram

![alt text](https://github.com/ColleagueRiley/RSGL/blob/development/RSGL-Architecture.png?raw=true)

# License
RSGL uses the libpng license, this means you can use RSGL freely as long as you do not claim you wrote this software, mark altered versions as such and keep the license included with the header.

```
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
  
1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required. 
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
```