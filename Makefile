ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

ifeq ($(detected_OS),Windows)
	LIBS := -lopengl32 -lshell32 -lgdi32
	EXT = dll
endif
ifeq ($(detected_OS),Darwin)        # Mac OS X
	LIBS := -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo -w
	EXT = dylib
endif
ifeq ($(detected_OS),Linux)
    LIBS := -lX11 -lGLX
	EXT = so
endif

all:
	make RSGL.o	
	gcc RSGL.o -shared -O3 $(LIBS) -lm -o libRSGL.$(EXT)
	ar rcs libRSGL.a *.o
	make examples

RSGL.o:
	cp RSGL.h RSGL.c
	gcc -c -Wall RSGL.c -fPIC -DRSGL_IMPLEMENTATION -DRGFW_NO_JOYSTICK_CODES
	rm RSGL.c

examples:
	cd examples && make

clean:
	rm -f libRSGL.so RSGL.o libRSGL.a
	cd examples && make clean

install:
	sudo cp libRSGL.so /usr/lib
	sudo cp libRSGL.a /usr/local/lib
	sudo cp RSGL.h /usr/include
