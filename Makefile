all:
	make RSGL.o	

	@if [ $(shell uname) = Windows_NT ] ; then\
		gcc RSGL.o -shared -O3 -I./include -lopengl32 -lshell32 -lgdi32 -lm -o libRSGL.dll;\
	fi

	@if [ $(shell uname) = Darwin ]; then\
		gcc RSGL.o -shared -O3 -I./include -I./include/deps/Silicon *.o -lm -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo -o libRSGL.dylib;\
	fi

	@if [ $(shell uname) != Windows_NT ] && [ $(shell uname -s) != Darwin ]; then\
		gcc RSGL.o -shared -O3 -I./include -lX11 -lGLX -lm -o libRSGL.so;\
	fi

	ar rcs libRSGL.a *.o

RSGL.o:
	cp RSGL.h RSGL.c
	gcc -c RSGL.c -fPIC -DRSGL_IMPLEMENTATION -DRGFW_NO_JOYSTICK_CODES
	rm RSGL.c

clean:
	rm libRSGL.so RSGL.o libRSGL.a

install:
	sudo cp libRSGL.so /usr/lib
	sudo cp libRSGL.a /usr/local/lib
	sudo cp RSGL.h /usr/include