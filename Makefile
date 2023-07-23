all:
	make RSGL.o	
	$(CC) RSGL.o -lX11 -lGLX -lm -shared -o libRSGL.so

	@if [ $(shell uname) = Windows_NT ] ; then\
		$(CXX) -shared -lopengl32 -lshell32 -lgdi32 RSGL.o -o libRSGL.dll;\
	fi

	@if [ $(shell uname) = Darwin ]; then\
		cp RSGL.hpp ./build/RSGL.cpp;\
		mkdir -p build/obj build/lib;\
		gcc ./source/deps/RGFW.h -D RSGL_IMPLEMENTATION -I./source/deps -o ./build/obj/RGFW.o;\
		$(CC) -I./ -fPIC -c ./build/RSGL.cpp ./build/obj/RGFW.o -o RSGL.o;\
		rm ./build/RSGL.cpp;\
		make silicon;\
		$(CC) -shared  -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo ./build/obj/*.o -o libRSGL.dynlib;\
	fi

	@if [ $(shell uname) != Windows_NT ] && [ $(shell uname -s) != Darwin ]; then\
		$(CC) -shared -lX11 -lGLX RSGL.o -o libRSGL.so;\
	fi

	cp RSGL.o libRSGL.a

RSGL.o:
	$(CC) -c RSGL.c -fPIC -DRSGL_IMPLEMENTATION -DRGFW_NO_JOYSTICK_CODES

clean:
	rm libRSGL.so RSGL.o

install:
	sudo cp libRSGL.so /usr/lib
	sudo cp libRSGL.a /usr/local/lib