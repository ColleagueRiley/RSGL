# RSGL
### Riley's Simple Graphics Library
<img align="left" style="width:300px" src="https://github.com/ColleagueRiley/RSGL/blob/main/RSGL_logo.png?raw=true" width="300px">

**RSGL** is a modular simple-to-use cross-platform graphics library. It attempts to combine the freedoms of low libraries with modern C techniques. Thus offering simplicity and convenience.

The RSGL.h header itself does not handle rendering nor input devices itself, both must be handled externally.

`RSGL_gl.h` is used by default for rendering, it renders via OpenGL 1.0 - 4.4.

UI elements have been moved to [`RSGL_ui`](https://github.com/ColleagueRiley/RSGL_ui/), in it's own repo.

---

<br>

[![Discord Members](https://img.shields.io/discord/829003376532258816.svg?label=Discord&logo=discord)](https://discord.gg/pXVNgVVbvh)

# Build statuses
![Linux workflow](https://github.com/ColleagueRiley/RSGL/actions/workflows/linux.yml/badge.svg)
![Windows workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/windows.yml/badge.svg)
![MacOS workflow windows](https://github.com/ColleagueRiley/RSGL/actions/workflows/macos.yml/badge.svg)

# Features
- No external dependencies, all dependencies are lightweight, bundled in and optional
- *Can* be used as a single-header file
- (RSGL_gl.h) Supports multiple versions of OpenGL (even allowing you to switch during runtime)
- Easily swappable rendering module
- Bring Your Own Backend (windowing)
- Basic shape drawing, collisions and drawing operations
- Many examples included
- Supports many image file types via stb_image.h
- Very modular and can easily be changed to suit your needs
- Free and Open Source

# Contacts
- email : ColleagueRiley@gmail.com
- discord : ColleagueRiley
- discord server : https://discord.gg/pXVNgVVbvh

# Modules
## RSGL-Architecture diagram
![The RSGL Architecture](https://github.com/ColleagueRiley/RSGL/blob/main/RSGL-Architecture.png?raw=true)

## Defines
  - `#define RSGL_NO_TEXT` - makes it so RSGL does not include text rendering functions
  `#define RSGL_NO_SAVE_IMAGE` - makes it so RSGL does not save/load images (don't use RSGL_drawImage if you use this). This is here because RSGL_drawImage saves the file name with its loaded texture so it can load the texture when you use the same file. \


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
// (see application in basic.c)
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

// include graphics backend header here too
#include "RSGL_gl.h"


int main() {
    // create window with your API choice here

    u32 WIDTH = //??? window width
    u32 HEIGHT = //??? window height
    void* procAddress = //??? renderer proc address (ex. RGFW_getProcAddress/glfwGetProcAddress)

    RSGL_init(RSGL_AREA(WIDTH, HEIGHT), procAddress);

    bool running = true;
    while (running) {
      // (handle your API's events here)

      RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
      RSGL_clear(RSGL_RGBA(255, 255, 255));
      RSGL_draw();

      // swap draw buffer with API here
      // for example RGFW_window_swapBuffer or glfwSwapBuffers
    }

    RSGL_free();
    // free window API here
}
```

Both of the included makefiles  (`Makefile` or `examples/Makefile`) use the Unlicense license so feel free to copy from either of them if you wish.

## compiling
these can be compiled by running `make examples` in the current directory\

or by using the makefile in `examples/`\

The Makefile in `examples/`  allows you to compile all the examples using `make`,\
compile one specific example using `make <example>` or\
run `make debug` which compiles and runs each example in debug mode

Ensure you're running the example in the `./examples` folder so the fonts are properly loaded.

![example screenshot](https://github.com/ColleagueRiley/RSGL/blob/main/screenshot.PNG?raw=true)

NOTE: Most of these examples use RGFW, but the library used can easily be changed.

## basics/basic.c
`examples/basics/basic.c` is a basic example that shows many of the window managing features RSGL has\
It also shows multiple ways of drawing a triangle

## advanced/glfw.c
`examples/advanced/glfw.c` is an example that shows how you can use RSGL with GLFW.

This example requires GLFW to be installed.\
You can download GLFW [here](https://www.glfw.org/download)

## basics/shapes.c
`examples/basics/shapes.c` is an example that shows off shape rendering using RSGL, the shapes rotate and switch between being filled and unfilled

## basics/text.c
`examples/basics/text.c` is an example of text rendering using RSGL

## basics/textures.c
`examples/basics/textures.c` is an example that shows off rendering textures on a shape, the textures switch between being enabled and disabled as the shapes spin

## advanced/shader.c
`examples/advanced/shader.c` is an example of how you'd use shaders with RSGL. It renders a circle, using a grid shader with changing colors, that follows the mouse.

## custom_render_samples/custom_render_gl1.c
`examples/custom_render_samples/custom_render_gl1.c` is an example that shows how you'd implement a custom rendering system.

## custom_render_samples/custom_render_template.c
`examples/custom_render_samples/custom_render_template.c` and `examples/custom_render_samples/custom_render_template.h` can be used as an example/template for implementing a completely custom rendering system.

# creating a custom rendering module
`examples/custom_render_samples/custom_render_template.h` is an example/template for implementing a completely custom rendering system.

When creating a custom renderer you need to implement these functions.

`void RSGL_renderBatch(RSGL_RENDER_INFO* info);` renders the current batches based on the info data\
`void RSGL_renderInit(void* proc, RSGL_RENDER_INFO* info);` do whatever to init render backend\
`void RSGL_renderFree(void);`  do whatever to free render backend\
`void RSGL_renderClear(float r, float g, float b, float a);` clears the screen based on a given color\
`void RSGL_renderViewport(i32 x, i32 y, i32 w, i32 h);` sets viewport\
`u32 RSGL_renderCreateTexture(u8* bitmap, RSGL_area memsize,  u8 channels);` create a texture based on given bitmap data\
`void RSGL_renderUpdateTexture(u32 texture, u8* bitmap, RSGL_area memsize, u8 channels);` updates a texture based on its id with new data info\
`void RSGL_renderDeleteTexture(u32 tex);` - deletes a texture based on its id

# Supporting RSGL
  Things you can do if you want to support the development of RSGL:

  - File issues of bugs, potential bugs, potential performance issues you find or any suggestions you have.
  - Code reviews, code reviews are always accepted
  - Feature requests
  - Pull requests, fixing issues, bugs, misspellings, etc.
  - Starring RSGL, a small thing but it means a lot to me
  - Sharing RSGL with others is the best way for RSGL to build and grow a community
  - Listing RSGL as a dependency if you use RSGL for a project

  If you want to contribute to RSGL but don't know what to contribute, you can check the `TODO` file.

# Dependencies
  All of RSGL's (non-native) dependencies are built-in.
  They can also be avoided or replaced.\

## RFont
  [RFont](https://github.com/colleagueRiley/RFont) is a super lightweight and modular font library for text rendering that I created.


## stb_image
  [stb_image](https://github.com/nothings/stb/) is a public domain single header image loader library.

# other credits

## Eima
  Since I started RSGL, ![Eima](https://github.com/EimaMei) has helped boost the morale of the project and has helped me come up with new ideas and improvements for the project. He also contributed some code to the project.

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
