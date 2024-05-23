# This is free and unencumbered software released into the public domain.
# Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means. 

CC = gcc
AR = ar

CUSTOM_CFLAGS = -lm

LIBS := -lopengl32 -lgdi32 -lWinmm
EXT = dll

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

ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

ifeq ($(detected_OS),Windows)
	LIBS := -lopengl32 -lgdi32 -lWinmm
	EXT = dll
endif
ifeq ($(detected_OS),Darwin)        # Mac OS X
	LIBS := -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo -w
	EXT = dylib
endif
ifeq ($(detected_OS),Linux)
    LIBS := -lX11 -lGLX -lXrandr
	EXT = so
endif

all:
	make RSGL.o	
	$(CC) $(CUSTOM_CFLAGS) RSGL.o -shared -O3 $(LIBS) -lm -o libRSGL.$(EXT)
	$(AR) rcs libRSGL.a *.o
	make examples

debug:
	make
	cd examples && make debug CUSTOM_CFLAGS=$(CUSTOM_CFLAGS)

static:
	make RSGL.o
	$(AR) rcs libRSGL.a *.o
	
RSGL.o:
	$(CC) $(CUSTOM_CFLAGS) -x c -c RSGL.h -fPIC -DRSGL_IMPLEMENTATION -DRGFW_NO_JOYSTICK_CODES

build_examples:
	cd examples && make CUSTOM_CFLAGS=$(CUSTOM_CFLAGS)

clean:
	rm -f libRSGL.so RSGL.o libRSGL.a
	cd examples && make clean

install:
	sudo cp libRSGL.so /usr/lib
	sudo cp libRSGL.a /usr/local/lib
	sudo cp RSGL.h /usr/include
