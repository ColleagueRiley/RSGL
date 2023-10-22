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
endif
ifeq ($(detected_OS),Darwin)        # Mac OS X
	LIBS := -lm -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo
endif
ifeq ($(detected_OS),Linux)
    LIBS := -I./include -lX11 -lGLX -lm
endif

OUTPUT = text

ARGS = RSGL.o -I../

all:
	make RSGL.o
	#make glfw
	make shapes
	make events

debug:
	make RSGL.o
	gcc source/main.c -lGL -g -I./include $(LIBS) RSGL.o -o $(OUTPUT)
	./$(OUTPUT)

RSGL.o:
	cp ../RSGL.h RSGL.c
	gcc -c RSGL.c -D RSGL_IMPLEMENTATION -D RGFW_NO_JOYSTICK_CODES -I../
	rm RSGL.c

glfw:
	gcc glfw.c -lglfw -lm -I../ -o glfw

shapes:
	gcc shapes.c $(LIBS) $(ARGS) -o shapes

events:
	gcc events.c $(LIBS) $(ARGS) -o events

clean:
	rm events glfw shapes RSGL.o