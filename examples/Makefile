# This is free and unencumbered software released into the public domain.
# Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means. 

CC = gcc

FLAGS = -Wall -Werror -Wstrict-prototypes -Wextra

LIBS := -I.. -lopengl32 -lgdi32 -lWinmm
ifneq (,$(filter $(CC) $(FLAGS) $(CUSTOM_CFLAGS),winegcc x86_64-w64-mingw32-gcc))
    detected_OS := Windows
else
ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif
endif

ifeq ($(detected_OS),Windows)
	LIBS := -I.. -lopengl32 -lgdi32 -lWinmm
endif
ifeq ($(detected_OS),Darwin)        # Mac OS X
	LIBS := -I.. -lm -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo
endif
ifeq ($(detected_OS),Linux)
    LIBS := -I.. -I./include -lX11 -lGL -lm -lXrandr
endif

ifeq ($(detected_OS),Linux)
    LIBS := -I.. -I./include -lX11 -lGL -lm -lXrandr
endif

ifneq (,$(filter $(CC),emcc))
	LIBS = -I.. -s WASM=1 -s ASYNCIFY -s USE_WEBGL2 -s GL_SUPPORT_EXPLICIT_SWAP_CONTROL=1 -s FULL_ES3 -D RSGL_GL_NO_LEGACY
	EXT = .js
endif

all:
	make shapes
	make events
	make textures
	make text
	make button
	make styles
	make container
	make textbox
	@echo The GLFW example needs GLFW to be installed to compile
	make glfw

debug:
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) basics/basic.c $(LIBS) $(ARGS) -o basic$(EXT)
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) basics/shapes.c $(LIBS) $(ARGS) -o shapes$(EXT)
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) basics/events.c $(LIBS) $(ARGS) -o events$(EXT)
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) basics/textures.c $(LIBS) $(ARGS) -o textures$(EXT)
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) basics/text.c $(LIBS) $(ARGS) -o text$(EXT)
ifeq (,$(filter $(CC),emcc))
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) advanced/glVer.c $(LIBS) $(ARGS) -o glVer$(EXT)
endif
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) widgets/button.c $(LIBS) $(ARGS) -o button$(EXT)
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) widgets/styles.c $(LIBS) $(ARGS) -o styles$(EXT)
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) widgets/container.c $(LIBS) $(ARGS) -o container$(EXT)
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) widgets/textbox.c $(LIBS) $(ARGS) -o textbox$(EXT)
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) advanced/shader.c $(LIBS) $(ARGS) -o shader$(EXT)

ifeq (,$(filter $(CC),emcc))
	./events
	./shapes
	./textures
	./text
	./glVer
	./basic
	./button
	./styles
	./container
	./textbox
endif

GLFW := -lglfw3 -lopengl32 -lgdi32 -lWinmm

ifeq ($(detected_OS),Darwin)        # Mac OS X
	GLFW := -lglfw -framework OpenGL
endif
ifeq ($(detected_OS),Linux)
    GLFW := -lglfw -lGL 
endif

glfw: advanced/glfw.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) advanced/glfw.c $(GLFW) -lm -I../ -o $@

shapes: basics/shapes.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) basics/shapes.c $(LIBS) $(ARGS) -o $@

shader: advanced/shader.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) advanced/shader.c $(LIBS) $(ARGS) -o $@

events: basics/events.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) basics/events.c $(LIBS) $(ARGS) -o $@

textures: basics/textures.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) basics/textures.c $(LIBS) $(ARGS) -o $@

text: basics/text.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) basics/text.c $(LIBS) $(ARGS) -o $@

textbox: widgets/textbox.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) widgets/textbox.c $(LIBS) $(ARGS) -o $@

glVer: advanced/glVer.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) advanced/glVer.c $(LIBS) $(ARGS) -o $@

basic: basics/basic.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) basics/basic.c $(LIBS) $(ARGS) -o $@

button: widgets/button.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) widgets/button.c $(LIBS) $(ARGS) -o $@

styles: widgets/styles.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) widgets/styles.c $(LIBS) $(ARGS) -o $@

container: widgets/container.c ../RSGL.h ../RSGL_gl.h
	$(CC) $(FLAGS) $(CUSTOM_CFLAGS) widgets/container.c $(LIBS) $(ARGS) -o $@

clean:
	rm -f events glfw shapes textures *.exe *.wasm *.js *.exe
