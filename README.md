## RSGL
### Riley's Simple GUI Library 
#

![The RSGL logo](https://github.com/ColleagueRiley/RSGL/blob/main/RSGL_logo.png?raw=true)

RSGL is a modular simple-to-use cross-platform GUI library for easily creating GUI apps and games. It combines the freedom of lower-level libraries with modern C techniques, thus offering both simplicity and convenience. 

It currently supports Linux, Windows and MacOS

# Build statuses
![cplus workflow](https://github.com/ColleagueRiley/RSGL/actions/workflows/linux.yml/badge.svg)
![cplus workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/windows.yml/badge.svg)
![cplus workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/macos.yml/badge.svg)

# Features
- No external dependencies, all the libraries required are included in RSGL
- Supports multiple platforms, windows, MacOS, linux, ect
- Supports multiple versions of OpenGL (even allowing you to switch during runtime)
- Uses other small lightweight dependencies
- OpenGL abstraction layer : RGL (which functions as its own single-header library)
- Supports multiple font, image and audio formats via `stb_truetype.h`, `stb_image.h`, and `miniaudio.h`
- Many examples included
- Free and Open Source

# header-only
  In order to 'link' RSGL you must add the line 
  ```c
  #define RSGL_IMPLEMENTATION
  ```
  in exactly one file which includes the `RSGL.h` header
  you can also use the command line argument `-D RSGL_IMPLEMENTATION`

  You can also compile RSGL into a .o or .so file and link it.

# Building
  Building RSGL isn't required. But if you wanted to, here's how.

  ```sh
  make
  ```

  This will compile RSGL into a static library and into a shared library.

  You can also compile RSGL by hand 

## compiling by hand
first, run `cp RSGL.h RSGL.c` 

then compile the library into object file\
ex. `gcc -c RSGL.c -fPIC`

after you compile the library into an object file, you can also turn the object file into an static or shared library

(commands ar and gcc can be replaced with whatever equivalent your system uses)
static : ar rcs RSGL.a RSGL.o
shared :
  windows:
    gcc -shared RSGL.o  -lshell32 -lgdi32 -o RSGL.dll
  linux:
    gcc -shared RSGL.o -lX11 -lXcursor -o RSGL.so
  macos:
    gcc -shared RSGL.o -framework Foundation -framework AppKit -framework CoreVideo

# compiling without RSGL_audio
If you want to compile without RSGL_AUDIO you have to add the line

```c
#define RSGL_NO_AUDIO
```

before you include RSGL\

If you want to keep using the RSGL_AUDIO features but compile miniaudio.h separately, add `#define RSGL_NO_MINIAUDIO_IMPLEMENTATION` instead \
and then link miniaudio your own way\

# Modules 
  #define RSGL_NO_WIDGETS (makes it so RSGL doesn't include widget functions)\
  #define RSGL_NO_AUDIO (makes it so RSGL doesn't include audio functions)\
  #define RSGL_NO_WINDOW - makes it so RSGL does RSGL_window is no longer included, RSGL_graphics is used instead [this is for using a different windowing library other than RGFW ]\
  #define RSGL_NO_TEXT - makes it so RSGL does not include text rendering functions\
  #define RGFW_NO_WIDGETS - makes it so RSGL does not include widgets\
  #define RSGL_NO_AUDIO - makes it so RSGL does not include audio functions\
  #define RSGL_NO_MINIAUDIO_IMPLEMENTATION - makes it so RSGL does not have `#define MINIAUDIO_IMPLEMENTATION` in this header (you'll have to link miniaudio some other way to use audio)\
  #define RSGL_NO_SAVE_IMAGE - makes it so RSGL does not save/load images (don't use RSGL_drawImage if you use this), \
                                  RSGL_drawImage saves the file name + texture so it can load it\
                                  when you ask for it later. This disables that\

# Widgets 
Note, widgets are WIP, the supported widgets include\
RSGL_alignRect, RSGL_button (checkboxes, toggleboxes, combo boxes, and radiobuttons) \

RSGL_textBox and RSGL_expandableRect are currently WIP

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

## compiling 
these can be compiled by running `make examples` in the current directory\

or by using the makefile in `examples/`\

with that makefile you can either compile all the example using `make`,\
compile one specific example using `make <example>` or\
running `make debug` which compiles and runs each example in debug mode

![example screenshot](https://github.com/ColleagueRiley/RSGL/blob/main/screenshot.PNG?raw=true)
![example screenshot](https://github.com/ColleagueRiley/RSGL/blob/main/screenshot2.PNG?raw=true)
![example screenshot 2](https://github.com/ColleagueRiley/RSGL/blob/main/screenshot3.PNG?raw=true)

## basic.c 
`examples/basic.c` is a basic example which goes through many of the window managing features RSGL has\
It also shows multiple ways of drawing a triangle

## events.c
`example/events.c` is a example which shows off all the events that RSGL has and prints out their event data into the terminal

## glfw.c
`examples/glfw.c` is a example which shows off how you can use RSGL's rendering functions separate from the windowing backend

## glVer.c
`examples/glVer.c` is a example which shows how RSGL can switch between legacy and modern opengl\
in the example, you can toggle between opengl 3.0+ and opengl 2.0- by pressing the spacebar

## shapes.c
`examples/shapes.c` is a example which shows off shape rendering using RSGL, the shapes rotate and switch between being filled and unfilled 

## text.c
`examples/text.c` is an example of text rendering using RSGL

## textures.c
`examples/textures.c` is an example which shows off rendering textures on a shape, the textures switch between being enabled and disabled as the shapes spin

## button.c 
`examples/button.c` is an example that shows off how to create and manage buttons using RSGL,
these include, a default style button, an checkbox, a toggle button, radio buttons, a comboBox and a custom button

# container.c
`examples/container.c` is an example which shows how to create and manage a widget container

# Dependencies
  All of RSGL's (non-native) dependencies are built-in, 

# RGFW
  RGFW, Riley's Gui library FrameWork.\
  RSGL is built off of RGFW, it handles window management for RSGL.

  RGFW is a flexible, lightweight, single-header-file alternative to GLFW. \ 

  [![AltText](https://github.com/ColleagueRiley/ColleagueRiley/blob/main/rgfw.png?raw=true)](https://github.com/ColleagueRiley/RGFW)\
  (this is a button that leads to the RGFW repo)

# credits

# RGL
  [RGL](https://github.com/ColleagueRiley/RGL) is an OpenGL abstraction layer (that I created), it handles opengl version managing and rendering. 

  It also allows RSGL to render using a simplified pipeline system.\
  RGL is heavily based on RLGL but it's designed to be more lightweight, with better performance.

# RFont
  [RFont](https://github.com/colleagueRiley/RFont) is a super light-weight and modular font library for text rendering that I created. 


# stb_image 
  [stb_image](https://github.com/nothings/stb/) is an public domain single header image loader library.

# miniaudio
  mini audio is an public domain audio playback and capture library. RSGL uses this for playing/loading audio

  miniaudio webpage : https://miniaud.io/

# Silicon
  Silicon is a single-header C binding for MacOS's Cocoas library. This library makes working with apple's API a lot better because you don't have to directly work with Objective-C.
  
  RSGL uses silicon via RGFW

  ![https://github.com/EimaMei/Silicon](Silicon Github)

# Raylib
  Because this project was created before I knew about Raylib, I am very surprised to see how simular RSGL is to Raylib.

  Raylib and it's community seems to have a very simular view of what a good GUI library should be like. So thank you Raylib for showing me that RSGL is not alone.

  Some newer additions were inspired by Raylib including it's modular design, RLGL (which RGL is based on) and verbos examples. 

  Why use RSGL instead of Raylib? RSGL has more dynamic features and is more lightweight.\

# Eima
  Since I started RSGL, ![Eima](https://github.com/EimaMei) has helped boost the morale of the project and has helped me come up with new ideas andd improvements for the project. He also contributed some code to the project. 

  Eima is the original creator of Silicon, without Silicon RSGL/RGFW probably would not support native macOS

  ![Eima's C toolkit / STL (sili)](https://github.com/Eimamei/sili)
  
# RSGL-Architecture diagram

![The RSGL Architecture](https://github.com/ColleagueRiley/RSGL/blob/main/RSGL-Architecture.png?raw=true)

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