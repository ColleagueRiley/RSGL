# This is free and unencumbered software released into the public domain.
# Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means. 

CC = gcc

LIBS := -I.. -lopengl32 -lshell32 -lgdi32 -lm -lXInput -lShcore
ifneq (,$(filter $(CC),winegcc x86_64-w64-mingw32-gcc))
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
	LIBS := -I.. -lopengl32 -lXInput -lShcore -lshell32 -lgdi32 -lm
endif
ifeq ($(detected_OS),Darwin)        # Mac OS X
	LIBS := -I.. -lm -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo
endif
ifeq ($(detected_OS),Linux)
    LIBS := -I.. -I./include -lX11 -lGL -lm 
endif

all:
	make RSGL.o
	make shapes
	make events
	make textures
	make text
	make glfw
	make button
	make container

debug:
	gcc basic.c $(LIBS) $(ARGS) -o basic
	gcc shapes.c $(LIBS) $(ARGS) -o shapes
	gcc events.c $(LIBS) $(ARGS) -o events
	gcc textures.c $(LIBS) $(ARGS) -o textures
	gcc text.c $(LIBS) $(ARGS) -o text
	gcc glVer.c $(LIBS) $(ARGS) -o glVer
	gcc button.c $(LIBS) $(ARGS) -o button
	gcc container.c $(LIBS) $(ARGS) -o container

	./events
	./shapes
	./textures
	./text
	./glVer
	./basic
	./button
	./container
	
glfw:
	gcc glfw.c -lglfw -lm -I../ -o glfw

shapes:
	gcc shapes.c $(LIBS) $(ARGS) -o shapes

events:
	gcc events.c $(LIBS) $(ARGS) -o events

textures:
	gcc events.c $(LIBS) $(ARGS) -o textures

text:
	gcc events.c $(LIBS) $(ARGS) -o text

glVer:
	gcc events.c $(LIBS) $(ARGS) -o glVer

basic:
	gcc basic.c $(LIBS) $(ARGS) -o basic

button:
	gcc button.c $(LIBS) $(ARGS) -o button

container:
	gcc container.c $(LIBS) $(ARGS) -o container

clean:
	rm -f events glfw shapes textures RSGL.o *.exe