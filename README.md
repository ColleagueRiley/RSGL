# RSGL 
### Riley's Simple GUI Library
![The RSGL logo](https://github.com/ColleagueRiley/RSGL/blob/main/RSGL_logo.png?raw=true)

RSGL is a modular simple-to-use cross-platform software development and GUI library. It attempts to combine the freedoms of low libraries with modern C techniques. Thus offering simplicity and convenience.

RSGL currently supports Linux, BSD, Windows and MacOS.

# Build statuses
![Linux workflow](https://github.com/ColleagueRiley/RSGL/actions/workflows/linux.yml/badge.svg)
![Windows workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/windows.yml/badge.svg)
![MacOS workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/macos.yml/badge.svg)

# Features
- No external dependencies, all the libraries required are lightweight and bundled in
- Supports multiple platforms, Windows, MacOS, Linux, etc
- Supports multiple versions of OpenGL (even allowing you to switch during runtime)
- Basic shape drawing, collisions and drawing operations
- OpenGL abstraction layer, RGL, which can also be used independently as a single-header library
- straightforward window management via RGFW
- Supports multiple font, image and audio formats via `stb_truetype.h`, `stb_image.h`, and `miniaudio.h`
- Dynamic GUI Widgets via a convenient styling system 
- Many examples included
- Allows you to use other libraries instead of the default RSGL backend.
- Free and Open Source

# Modules 
## RSGL-Architecture diagram
![The RSGL Architecture](https://github.com/ColleagueRiley/RSGL/blob/main/RSGL-Architecture.png?raw=true)

## Defines
  - `#define RSGL_NO_WIDGETS` - makes it so RSGL doesn't include widget functions
  - `#define RSGL_NO_AUDIO` - makes it so RSGL doesn't include audio functions
  - `#define RSGL_NO_WINDOW` - makes it so RSGL_window is no longer included, RSGL_graphics is used instead. This is for using a different windowing library other than RGFW
  - `#define RSGL_NO_TEXT` - makes it so RSGL does not include text rendering functions
  - `#define RGFW_NO_WIDGETS` - makes it so RSGL does not include widgets
  - `#define RSGL_NO_AUDIO` - makes it so RSGL does not include audio functions
  - `#define RSGL_NO_MINIAUDIO_IMPLEMENTATION` - makes it so RSGL does not link Miniaudio. You'll have to link Miniaudio yourself to use audio
  `#define RSGL_NO_SAVE_IMAGE` - makes it so RSGL does not save/load images (don't use RSGL_drawImage if you use this). This is here because RSGL_drawImage saves the file name with its loaded texture so it can load the texture when you use the same file. \

# Widgets 
Widgets are WIP, the supported widgets include\
RSGL_alignRect, RSGL_button (checkboxes, toggle buttons, combo boxes, sliders and radio buttons) and widget containers \

RSGL_textBox and RSGL_expandableRect are currently WIP.

# Using RSGL 

## header-only
  To link RSGL you must add the line.
  ```c
  #define RSGL_IMPLEMENTATION
  ```
  in exactly one file which includes the `RSGL.h` header.
  
  You could also use the command line argument `-D RSGL_IMPLEMENTATION`.

  Optionally, RSGL can be compiled into a .o or .so file and link it.

## Compiling RSGL
  Compiling RSGL isn't required. But if you want to, here's how.

  ```sh
  make
  ```
  
  This will compile RSGL into a static library into a shared library.
  You can also compile RSGL by hand.

### compiling by hand
1) `cp RSGL.h RSGL.c`. 
2) Compile the library into an object file by running `gcc -c RSGL.c -fPIC`.
3) After you compile the library into an object file, you can also turn the object file into a static or shared library.
4) To compile statically run `ar rcs RSGL.a RSGL.o`
5) To compile RSGL into a shared library, run `gcc -shared RSGL.o <system libs>`
```
  windows:
    gcc -shared RSGL.o  -lshell32 -lgdi32 -o RSGL.dll
  linux:
    gcc -shared RSGL.o -lX11 -lXcursor -o RSGL.so
  macos:
    gcc -shared RSGL.o -framework Foundation -framework AppKit -framework CoreVideo
```

# Examples

## a basic example

```c
#define RSGL_NO_AUDIO /* RSGL uses miniaudio.h, and I don't want to compile it if I'm not using it */
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

This example can be compiled with :\
Linux:\
`gcc <file.c> -lX11 -lGLX -lm -o example`\
Windows :\
`gcc <file.c> -lopengl32 -lgdi32 -lshell32 -o example`\
MacOS :\
`gcc <file.c> -lm -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo -o example`

Both of the included makefiles  (`Makefile` or `examples/Makefile`) use the Unlicense license so feel free to copy from either of them if you wish. 

## compiling 
these can be compiled by running `make examples` in the current directory\

or by using the makefile in `examples/`\

The Makefile in `examples/`  allows you to compile all the examples using `make`,\
compile one specific example using `make <example>` or\
run `make debug` which compiles and runs each example in debug mode

![example screenshot](https://github.com/ColleagueRiley/RSGL/blob/main/screenshot.PNG?raw=true)
![example screenshot 2](https://github.com/ColleagueRiley/RSGL/blob/main/screenshot2.PNG?raw=true)
![example screenshot 3](https://github.com/ColleagueRiley/RSGL/blob/main/screenshot3.PNG?raw=true)
![example screenshot 4](https://github.com/ColleagueRiley/RSGL/blob/main/screenshot4.PNG?raw=true)
![example screenshot 5](https://github.com/ColleagueRiley/RSGL/blob/main/screenshot5.PNG?raw=true)

## basic.c 
`examples/basic.c` is a basic example that shows many of the window managing features RSGL has\
It also shows multiple ways of drawing a triangle

## events.c
`example/events.c` is an example that shows off all the events that RSGL has and prints out their event data into the terminal

## glfw.c
`examples/glfw.c` is an example that shows off how you can use RSGL's rendering functions separate from the windowing backend

## glVer.c
`examples/glVer.c` is an example that shows how RSGL can switch between legacy and modern OpenGL\
in the example, you can toggle between OpenGL 3.0+ and OpenGL 2.0- by pressing the spacebar

## shapes.c
`examples/shapes.c` is an example that shows off shape rendering using RSGL, the shapes rotate and switch between being filled and unfilled 

## text.c
`examples/text.c` is an example of text rendering using RSGL

## textures.c
`examples/textures.c` is an example that shows off rendering textures on a shape, the textures switch between being enabled and disabled as the shapes spin

## button.c 
`examples/button.c` is an example that shows off how to create and manage buttons using RSGL,
these include, a default style button, a checkbox, a toggle button, radio buttons, a combo box, a slider and a custom button

## styles.c
`examples/button.c` is an example that shows off button styles, there are groups of buttons for each style.
There is also a switch button that allows you to toggle dark mode.

# container.c
`examples/container.c` is an example that shows how to create and manage a widget container

# Dependencies
  All of RSGL's (non-native) dependencies are built-in.

## RGFW
  RGFW, Riley's Gui library FrameWork.\
  RSGL is built off of RGFW, it handles window management for RSGL.

  RGFW is a flexible, lightweight, single-header-file alternative to GLFW. \ 

  [![AltText](https://github.com/ColleagueRiley/ColleagueRiley/blob/main/rgfw.png?raw=true)](https://github.com/ColleagueRiley/RGFW)\
  (this is a button that leads to the RGFW repo)

## RGL
  [RGL](https://github.com/ColleagueRiley/RGL) is an OpenGL abstraction layer (that I created), it handles OpenGl version managing and rendering. 

  It also allows RSGL to render using a simplified pipeline system.\
  RGL is heavily based on RLGL but it's designed to be more lightweight, with better performance.

## RFont
  [RFont](https://github.com/colleagueRiley/RFont) is a super lightweight and modular font library for text rendering that I created. 


## stb_image 
  [stb_image](https://github.com/nothings/stb/) is a public domain single header image loader library.

## Miniaudio
  Miniaudio is a public-domain audio playback and capture library. RSGL uses this for playing/loading audio

  Miniaudio's website, https://miniaud.io/

## Silicon
  Silicon is a single-header C binding for MacOS's Cocoas library. This library makes working with Apple's API a lot better because you don't have to directly work with Objective-C.
  
  RSGL uses silicon via RGFW

  [Silicon on Github](https://github.com/EimaMei/Silicon)

# other credits

## Raylib
  Because this project was created before I knew about Raylib, I am very surprised to see how similar RSGL is to Raylib.

  Raylib and its community seem to have a very similar view of what a good GUI library should be like. So thank you Raylib for showing me that RSGL is not alone.

  Some newer additions were inspired by Raylib including its modular design and RLGL (which RGL is based on).

  Why use RSGL instead of Raylib? RSGL has more dynamic features and is more lightweight.\

## Eima
  Since I started RSGL, ![Eima](https://github.com/EimaMei) has helped boost the morale of the project and has helped me come up with new ideas and improvements for the project. He also contributed some code to the project. 

  Eima is the original creator of Silicon, without Silicon RSGL/RGFW probably would not support native macOS

  ![Eima's C toolkit / STL (sili)](https://github.com/Eimamei/sili)
  
# License
RSGL uses the Zlib/libPNG license, this means you can use RSGL freely as long as you do not claim you wrote this software, mark altered versions as such and keep the license included with the header.

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