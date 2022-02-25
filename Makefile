ARGS = -Wall -fPIC -O -g
OS = linux
SOURCEDIR = source/$(OS)
SOURCE = $(SOURCEDIR)/rsgl.cpp $(SOURCEDIR)/draw.cpp $(SOURCEDIR)/collide.cpp $(SOURCEDIR)/other.cpp
LIBDIR = $(SOURCEDIR)/deps
LIBS = $(LIBDIR)/libX11.so.6.4.0 $(LIBDIR)/libpng16.so.16.37.0  $(LIBDIR)/libpthread-2.33.so  $(LIBDIR)/libGL.so.1.7.0 $(LIBDIR)/libGLX.so.0.0.0

GXX = g++
build:
	@$(GXX) -c $(ARGS) $(SOURCE) -c --no-warnings
	@$(GXX) -shared rsgl.o draw.o collide.o other.o  $(LIBS) -o libRSGL.so 
	@rm rsgl.o draw.o collide.o other.o;	

install:
	@make build
	@sudo cp libRSGL.so /usr/lib/libRSGL.so
	@rm libRSGL.so
	@mkdir ./RSGL
	@cp -r include RSGL.hpp ./Examples ./RSGL
	@sudo cp -r ./RSGL /usr/include/
	@sudo rm -r ./RSGL

all:
	build
	install

uninstall:
	rm /usr/lib/libRSGL.so

update:
	rm /usr/lib/libRSGL.so
	@make install

WARGS = -Wall -fPIC -O2 -Wno-unknown-pragmas -g
WSOURCEDIR = source/windows
WSOURCE = $(WSOURCEDIR)/rsgl.cpp $(WSOURCEDIR)/collide.cpp $(WSOURCEDIR)/device.cpp $(WSOURCEDIR)/draw.cpp $(WSOURCEDIR)/xinput.cpp
WLIBS = -L"$(CURDIR)/source/windows/deps" -lcomdlg32 -lgdiplus -lgdi32 -lmsimg32 -ldiscord -lopengl32 -lxinput -lpng -ldpi

windowsBuild: 
	@$(GXX) -C $(WARGS) $(WSOURCE) -c --no-warnings
	@$(GXX) $(WARGS) -shared rsgl.o draw.o collide.o device.o xinput.o $(WLIBS) -lmingw32 -static-libgcc -static-libstdc++ -static -o libRSGL.lib
	@rm rsgl.o draw.o collide.o device.o  xinput.o;	
windowsBuildRSGL: 
	@$(GXX) -C $(WARGS) $(WSOURCEDIR)/rsgl.cpp -c --no-warnings
	@$(GXX) $(WARGS) -shared rsgl.o draw.o collide.o device.o xinput.o $(WLIBS) -lmingw32 -static-libgcc -static-libstdc++ -static -o libRSGL.lib
	make windowsCompile
windowsBuildXinput: 
	@$(GXX) -C $(WARGS) $(WSOURCEDIR)/xinput.cpp -c --no-warnings
	@$(GXX) $(WARGS) -shared rsgl.o draw.o collide.o device.o xinput.o $(WLIBS) -lmingw32 -static-libgcc -static-libstdc++ -static -o libRSGL.lib
	make windowsCompile
windowsCompile:
	@$(GXX) $(WARGS) source/main.cpp libRSGL.lib -o RSGL.exe
	./RSGL.exe
	
help:
	@echo "make help : runs this help tab"
	@echo "make : runs both build and install"
	@echo "make build : builds libRSGL.so in the local dir"
	@echo "make install : installs the build into /usr/lib, then removes in, runs buld if it's not already build"
	@echo "make update : update preexisting RSGL"
	@echo "make uninstall : uninstall RSGL"