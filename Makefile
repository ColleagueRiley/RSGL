GXX = g++
LText = source/linux/deps/drawtext/*.o
LLIBS = $(LText) -lX11 -lGLX -lGL -lfreetype
LSOURCE = source/linux/*.cpp

all:
	mkdir -p source/linux/deps/drawtext
	cd source/linux/deps/drawtext && ar x ../libdrawtext.a && cd ../../../../
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
	cp libRSGL.so /usr/liba
	cp libRSGL.a /usr/local/lib
	mkdir -p /usr/include/RSGL
	cp -r include RSGL.hpp /usr/include/RSGL
	chmod +x RSGL-static
	cp RSGL-static /usr/bin 
	make clean

clean:
	rm -r *.o *.a *.so source/linux/deps/drawtext


buildWindows:
	echo something lithuanian
