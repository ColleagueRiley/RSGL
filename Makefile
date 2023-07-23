all:
	make RSGL.o	
	$(CC) RSGL.o -lX11 -lGLX -lm -shared -o libRSGL.so

	@if [ $(shell uname) = Windows_NT ] ; then\
		$(CXX) -shared -lopengl32 -lshell32 -lgdi32 RSGL.o -o libRSGL.dll;\
	fi

	@if [ $(shell uname) = Darwin ]; then\
		cd deps/Silicon/source && make\
		$(CC) -I./deps/Silicon ../deps/Silicon/source/*.o -shared -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo RSGL.o -o libRSGL.dynlib;\
	fi

	@if [ $(shell uname) != Windows_NT ] && [ $(shell uname -s) != Darwin ]; then\
		$(CC) -shared -lX11 -lGLX RSGL.o -o libRSGL.so;\
	fi

	cp RSGL.o libRSGL.a

RSGL.o:
	cp RSGL.h RSGL.c
	$(CC) -c RSGL.c -fPIC -DRSGL_IMPLEMENTATION -DRGFW_NO_JOYSTICK_CODES
	rm RSGL.c

clean:
	rm libRSGL.so RSGL.o

install:
	sudo cp libRSGL.so /usr/lib
	sudo cp libRSGL.a /usr/local/lib
	sudo cp RSGL.h /usr/include