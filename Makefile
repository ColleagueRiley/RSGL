GXX = g++
LText = source/linux/deps/drawtext/*.o #source/linux/deps/miniaudio.o
LLIBS = $(LText) -lX11 -lGL -lfreetype
LSOURCE = source/linux/*.cpp

all:
	make source/linux/deps/miniaudio.o
	mkdir -p source/linux/deps/drawtext
	cd source/linux/deps/drawtext && ar x ../libdrawtext.a && cd ../../../../
	make buildLinux
	make installLinux

source/linux/deps/miniaudio.o:
	gcc -c source/linux/deps/miniaudio.c -o source/linux/deps/miniaudio.o

RSGLStatic:
	cd RSGL-static && make

buildLinux:
	make RSGLStatic
	make buildLinuxFromSource
	make buildLinuxFromObj

buildLinuxFromObj:
	make buildLinuxFromObjStatic
	make buildLinuxFromObjShared

buildLinuxFromSource:
	$(GXX) -fPIC $(LSOURCE) -c 

buildLinuxFromObjStatic:
	ar rcs libRSGL.a *.o $(LText)

buildLinuxFromObjShared:
	g++ -shared *.o $(LLIBS) -o libRSGL.so

installLinux:
	cp libRSGL.so /usr/lib
	cp libRSGL.a /usr/local/lib
	mkdir -p /usr/include/RSGL
	cp -r include RSGL.hpp /usr/include/RSGL
	chmod +x RSGL-static
	cd RSGL-static && make install
	make clean

clean:
	rm -r *.o *.a *.so source/linux/deps/drawtext


buildWindows:
	echo something lithuanian
