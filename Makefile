GXX = g++
LText = source/linux/deps/drawtext/*.o
LLIBS = $(LText) -lX11 -lGLX -lGL -lfreetype
LSOURCE = source/linux/*.cpp

all:
	make buildLinux
	make installLinux

buildLinux:
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
	mkdir RSGL
	cp -r include RSGL.hpp RSGL 
	cp -r RSGL/* /usr/include/RSGL
	rm -r RSGL
	chmod +x RSGL-static
	cp RSGL-static /usr/bin 
	make clean

clean:
	rm *.o *.a *.so


buildWindows:
	echo something lithuanian
