GXX = g++
LLIBS = source/linux/deps/drawtext/*.o source/linux/deps/X/*.o source/linux/deps/GLU/*.o
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
	ar rcs libRSGL.a *.o $(LLIBS)

buildLinuxFromObjShared:
	g++ -shared *.o source/linux/deps/libdrawtext.a -lX11 -lGLX -lGL  -lGLU -lfreetype -o libRSGL.so

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