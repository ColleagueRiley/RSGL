all:
	make RSGL.o	
	$(CC) RSGL.o -lX11 -lGLX -lm -shared -o libRSGL.so

	@if [ $(shell uname) = Windows_NT ] ; then\
		$(CXX) -shared -lopengl32 -lshell32 -lgdi32 RSGL.o -o libRSGL.dll;\
	fi

	@if [ $(shell uname) = Darwin ]; then\
		$(CC) -c ./deps/Silicon/source/*m -I./deps/Silicon/;\
		$(CC) -I./deps/Silicon ../deps/Silicon/source/*.o -shared -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo RSGL.o -o libRSGL.dynlib;\
	fi

	@if [ $(shell uname) != Windows_NT ] && [ $(shell uname -s) != Darwin ]; then\
		$(CC) -shared -lX11 -lGLX RSGL.o -I/deps/Silicon/ -o libRSGL.so;\
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
