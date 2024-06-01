# RSGL 
### Riley's Simple Graphics Library
![The RSGL logo](https://github.com/ColleagueRiley/RSGL/blob/main/RSGL_logo.png?raw=true)

RSGL is a modular simple-to-use cross-platform graphics library. It attempts to combine the freedoms of low libraries with modern C techniques. Thus offering simplicity and convenience.

Although RSGL is packaged with RGFW by default, it can be used with any windowing system. (See `examples/glfw.c`)

The RSGL.h header itself does not handle rendering itself, instead rendering must be handled externally.  `RSGL_gl.h` is used by default for rendering, it renders via OpenGL 1.0 - 4.4.

RSGL currently supports Linux, BSD, Windows and MacOS via RGFW.

# Build statuses
![Linux workflow](https://github.com/ColleagueRiley/RSGL/actions/workflows/linux.yml/badge.svg)
![Windows workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/windows.yml/badge.svg)
![MacOS workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/macos.yml/badge.svg)

# Features
- No external dependencies, all dependencies are lightweight, bundled in and optional
- *Can* be used as a single-header file
- (RGFW.h) Supports multiple platforms, Windows, MacOS, Linux, etc
- (RSGL_gl.h) Supports multiple versions of OpenGL (even allowing you to switch during runtime)
- Easily swappable rendering module 
- Basic shape drawing, collisions and drawing operations
- Straightforward window management via RGFW.h
- Dynamic GUI Widgets via a convenient styling system 
- Many examples included
- Supports many image file types via stb_image.h
- Very modular and can easily be changed to suit your needs
- Free and Open Source

# Contacts
- email : ColleagueRiley@gmail.com 
- discord : ColleagueRiley
- discord server : https://discord.gg/pXVNgVVbvh

# Supporting RSGL
  Things you can do if you want to support the development of RSGL:

  - File issues of bugs, potential bugs, potential performance issues you find or any suggestions you have.
  - Code reviews, code reviews are always accepted
  - Feature requests 
  - Pull requests, fixing issues, bugs, misspellings, etc. 

  If you want to contribute to RSGL but don't know what to contribute, you can check the `TODO` file.

# Modules 
## RSGL-Architecture diagram
![The RSGL Architecture](https://github.com/ColleagueRiley/RSGL/blob/main/RSGL-Architecture.png?raw=true)

## Defines
  - `#define RSGL_NO_WIDGETS` - makes it so RSGL doesn't include widget functions
  - `#define RSGL_NO_RGFW` - This is for using a different windowing library other than RGFW. RSGL_graphics is used instead, as seen in `examples/glfw.c`
  - `#define RSGL_NO_TEXT` - makes it so RSGL does not include text rendering functions
  - `#define RGFW_NO_WIDGETS` - makes it so RSGL does not include widgets
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
1) Compile the library into an object file by running `gcc -x c -c RSGL.h -fPIC`.
2) After you compile the library into an object file, you can also turn the object file into a static or shared library.
3) To compile statically run `ar rcs RSGL.a RSGL.o`
4) To compile RSGL into a shared library, run `gcc -shared RSGL.o <system libs>`
```
  windows:
    gcc -shared RSGL.o  -lshell32 -lgdi32 -lwinmm -o RSGL.dll
  linux:
    gcc -shared RSGL.o -lX11 -lXcursor -lXrandr -o RSGL.so
  macos:
    gcc -shared RSGL.o -framework Foundation -framework AppKit -framework CoreVideo
```

# Examples

## a basic example

```c
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

Ensure you're running the example in the `./examples` folder so the fonts are properly loaded. 

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
`examples/glfw.c` is an example that shows how you can use RSGL with GLFW instead of with RGFW.

This example requires GLFW to be installed.\
You can download GLFW [here](https://www.glfw.org/download)

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

## container.c
`examples/container.c` is an example that shows how to create and manage a widget container

## custom_render_gl1.c 
`examples/custom_render_gl1.c` is an example that shows how you'd implement a custom rendering system.

## custom_render_template.c
`examples/custom_render_template.c` and `examples/custom_render_template.h` can be used as an example/template for implementing a completely custom rendering system.

# creating a custom rendering module
`examples/custom_render_template.h` is an example/template for implementing a completely custom rendering system.

When creating a custom renderer you need to implement these functions.

`void RSGL_renderBatch(RSGL_RENDER_INFO* info);` renders the current batches based on the info data\
`void RSGL_renderInit(void* proc, RSGL_RENDER_INFO* info);` do whatever to init render backend\
`void RSGL_renderFree(void);`  do whatever to free render backend\
`void RSGL_renderClear(float r, float g, float b, float a);` clears the screen based on a given color\
`void RSGL_renderViewport(i32 x, i32 y, i32 w, i32 h);` sets viewport\
`u32 RSGL_renderCreateTexture(u8* bitmap, RSGL_area memsize,  u8 channels);` create a texture based on given bitmap data\
`void RSGL_renderUpdateTexture(u32 texture, u8* bitmap, RSGL_area memsize, u8 channels);` updates a texture based on its id with new data info\
`void RSGL_renderDeleteTexture(u32 tex);` - deletes a texture based on its id

# Dependencies
  All of RSGL's (non-native) dependencies are built-in.

  All of RSGL's dependencies can also be cut out and/or replaced.\
  In theory, you could use RSGL as a single-header file without the other headers.

## RGFW
  RGFW, Riley's Graphics FrameWork.\
  RSGL is built off of RGFW, it handles window management for RSGL.

  RGFW is a flexible, lightweight, single-header-file alternative to GLFW. \ 

  [![AltText](https://github.com/ColleagueRiley/ColleagueRiley/blob/main/rgfw.png?raw=true)](https://github.com/ColleagueRiley/RGFW)\
  (this is a button that leads to the RGFW repo)

## RFont
  [RFont](https://github.com/colleagueRiley/RFont) is a super lightweight and modular font library for text rendering that I created. 


## stb_image 
  [stb_image](https://github.com/nothings/stb/) is a public domain single header image loader library.

# other credits

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