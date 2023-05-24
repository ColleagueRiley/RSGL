## RSGL
### Riley's Simple GUI Library 
#

![alt text](https://github.com/ColleagueRiley/RSGL/blob/main/logo.png?raw=true)

A simple GUI library currently for Linux and Windows

# Building
  to built simply run `./configure && make`

  to customize the build you can put pre-defined vars before ./configure 
  ex.
  
  `WINDIR="this is what comes before the /lib /include, /lib (ex. /usr for linux)" OS="(Linux or WindowsNT)" LIBA="this is where the .a file goes, ex. local/lib" LIB=this is where the .so/dll goes, ex. /lib" BIN="ex. /bin" INCLUDE="ex. include" CCEXT="this goes before g++ or gcc, ex. wine" ./configure` 

# MACOS / GLFW
  RSGL does not have direct support for macos via (RGFW), to use RSGL for macos, you must install the GLFW-Backend branch of RSGL

  Even if RSGL supports your system, you can install the GLFW-Backend if you want to for whatever reason.
  Though the GLFW backend works essentally the same as the RGFW backend except it's bigger in size.

# single-header
  When you build RSGL, it by default makes a single header file, if you want to use that file a lone, after you build the file is in `./build/include`

  To "link" RSGL with the single header file simply add `#define RSGL_IMPLEMENTATION` before you include RSGL ex.

  ```cpp
  #define RSGL_IMPLEMENTATION
  #include <RSGL.hpp>
  ```

  However, compiling a code like this takes a lot longer, so it may be a good idea to link compiled sources instead. 


# installing
  There's two ways to install, either using the Makefile or by hand, installing by hand is a good idea if you're crosscompiling

  to install with the make file run `make install` or `sudo make install`, if you're on linux and need the perms

  to install by hand simply,
    move ./build/lib/*.a to the proper *.a lib folder (ex. /usr/local/lib)
    `cp ./build/lib/*.a /usr/local/lib`

    move ./build/lib/*.so (or *.dll for windows) to the proper lib folder (ex. /usr/lib)
    `cp ./build/lib/*.so /usr/lib`

    move the contents of ./build/bin and ./bin/include to the proper include (.ex /usr/include) and (.ex /usr/bin) folders 
    `cp -r ./build/include/* /usr/include`

    `cp ./build/bin/* /usr/bin`

# Modules 
  RSGL also has optional modules inwhich you can include specific modules alone, for example

  RSGL_CORE - window handling

  RSGL_SHAPES - sturcts shape and collision handling

  RSGL_GL - RSGL Graphics Library, draw functions

  RSGL_WIDGETS - Widgets

  RSGL_OTHER - Other non-gui functions

  RSGL_AUDIO - Audio functions

  you can select specific modules by using `#define MOUDLE` before including RSGL

  ex. 

  ```cpp
  #define RSGL_GL
  #include <RSGL.hpp>
  ```

# Dependencies
  Almost all of RSGL's dependencies are built-in, 
  
  depencies :

  opengl

  linux:
    libX11 (xlib) - (not reqired if you're doing a glfw build)

  windows:
   libgdi, win32 api headers

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

# GLFW
  RSGL uses GLFW as an optional backend on a seperate branch. Though RSGL for MacOS requires the GLFW Branch. This may change in the future if and when MacOS support is added to RGFW

# Raylib
  Because this project was created and the majority of features were created before my learning of Raylib, I am very surprised to see how simular Raylib is to RSGL. 
  Raylib and it's community seems to have a very simular view of what a good GUI library should be like as RSGL does. So thank you Raylib for showing me that RSGL is not alone.

  However, some newer additions were inspired by Raylib including it's modular design and verbos examples. 

# Eima
  Since I started RSGL, ![Eima](https://github.com/EimaMei) has helped boost the morale of the project, has helped with coming up with ideas for the project, has helped when debating new featues of the project
  and even has contributed some code to the project. 

  ![Eima's own programming launage for HOI 4](https://github.com/EimaMei/HPL/)
  
# RSGL-Architecture diagram

![alt text](https://github.com/ColleagueRiley/RSGL/blob/development/RSGL-Architecture.png?raw=true)