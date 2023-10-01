ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

ifeq ($(detected_OS),Windows)
	LIBS := -lopengl32 -lshell32 -lgdi32 -lm
	EXT = dll
endif
ifeq ($(detected_OS),Darwin)        # Mac OS X
	LIBS := ./deps/Silicon/source/*.o -lm -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo
	EXT = dylib
endif
ifeq ($(detected_OS),Linux)
    LIBS := -I./include -lX11 -lGLX -lm
	EXT = so
endif

all:
	@if [ $(shell uname) = Darwin ]; then\
		make deps/Silicon/source/mac.o;\
	fi
	make RSGL.o	
	gcc RSGL.o -shared -O3 -I./include $(LIBS) -o libRSGL.$(EXT)
	ar rcs libRSGL.a *.o

RSGL.o:
	cp RSGL.h RSGL.c
	gcc -c -Wall RSGL.c -I./deps/Silicon -fPIC -DRSGL_IMPLEMENTATION -DRGFW_NO_JOYSTICK_CODES
	rm RSGL.c

clean:
	rm libRSGL.so RSGL.o libRSGL.a

deps/Silicon/source/mac.o:
	cd deps/Silicon/source/ && gcc -c *.m -I../

install:
	sudo cp libRSGL.so /usr/lib
	sudo cp libRSGL.a /usr/local/lib
	sudo cp RSGL.h /usr/include
