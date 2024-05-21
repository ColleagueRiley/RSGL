# This is free and unencumbered software released into the public domain.
# Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means. 

CC = gcc

LIBS := -I.. -lopengl32 -lgdi32 -lWinmm
ifneq (,$(filter $(CC) $(CUSTOM_CFLAGS),winegcc x86_64-w64-mingw32-gcc))
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
	LIBS := -I.. -lopengl -lgdi32
endif
ifeq ($(detected_OS),Darwin)        # Mac OS X
	LIBS := -I.. -lm -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo
endif
ifeq ($(detected_OS),Linux)
    LIBS := -I.. -I./include -lX11 -lGL -lm -lXrandr
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
	$(CC) $(CUSTOM_CFLAGS) basic.c $(LIBS) $(ARGS) -o basic
	$(CC) $(CUSTOM_CFLAGS) shapes.c $(LIBS) $(ARGS) -o shapes
	$(CC) $(CUSTOM_CFLAGS) events.c $(LIBS) $(ARGS) -o events
	$(CC) $(CUSTOM_CFLAGS) textures.c $(LIBS) $(ARGS) -o textures
	$(CC) $(CUSTOM_CFLAGS) text.c $(LIBS) $(ARGS) -o text
	$(CC) $(CUSTOM_CFLAGS) glVer.c $(LIBS) $(ARGS) -o glVer
	$(CC) $(CUSTOM_CFLAGS) button.c $(LIBS) $(ARGS) -o button
	$(CC) $(CUSTOM_CFLAGS) styles.c $(LIBS) $(ARGS) -o styles
	$(CC) $(CUSTOM_CFLAGS) container.c $(LIBS) $(ARGS) -o container
	$(CC) $(CUSTOM_CFLAGS) textbox.c $(LIBS) $(ARGS) -o textbox

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

GLFW := -lglfw3 -lopengl32 -lgdi32

ifeq ($(detected_OS),Darwin)        # Mac OS X
	GLFW := -lglfw -framework OpenGL
endif
ifeq ($(detected_OS),Linux)
    GLFW := -lglfw -lgl 
endif

glfw: glfw.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(GLFW) -lm -I../ -o $@

shapes: shapes.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(LIBS) $(ARGS) -o $@

events: events.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(LIBS) $(ARGS) -o $@

textures: textures.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(LIBS) $(ARGS) -o $@

text: text.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(LIBS) $(ARGS) -o $@

textbox: textbox.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(LIBS) $(ARGS) -o $@

glVer: glVer.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(LIBS) $(ARGS) -o $@

basic: basic.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(LIBS) $(ARGS) -o $@

button: button.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(LIBS) $(ARGS) -o $@

styles: styles.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(LIBS) $(ARGS) -o $@

container: container.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(LIBS) $(ARGS) -o $@

custom_render_gl1: custom_render_gl1.c ../RSGL.h
	$(CC) $(CUSTOM_CFLAGS) $^ $(LIBS) $(ARGS) -o $@

clean:
	rm -f events glfw shapes textures *.exe