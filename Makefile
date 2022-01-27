ARGS = -Wall -fPIC -O -g
OS = linux
SOURCEDIR = source/$(OS)
SOURCE = $(SOURCEDIR)/rsgl.cpp
LIBDIR = $(SOURCEDIR)/deps
LIBS = $(LIBDIR)/libX11.so.6.4.0 $(LIBDIR)/libpng16.so.16.37.0  $(LIBDIR)/libpthread-2.33.so  $(LIBDIR)/libGL.so.1.7.0 $(LIBDIR)/libGLX.so.0.0.0

GXX = g++
WGXX= wineg++
WSOURCEDIR = source/windows
WSOURCE = $(WSOURCEDIR)/rsgl.cpp
WLIBDIR = $(WSOURCEDIR)/deps
WLIBS = $(WLIBDIR)/libSDL2.a $(WLIBDIR)/libSDL2.dll.a $(WLIBDIR)/libSDL2.a

build:
	@$(GXX) -c $(ARGS) $(SOURCE) -c -o rsgl.o --no-warnings
	@$(GXX) -shared rsgl.o $(LIBS) -o libRSGL.so
	@rm rsgl.o;	

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

windowsBuild:
	@$(WGXX) -c $(ARGS) $(WSOURCE) -c -o rsgl.o 
	@$(WGXX) rsgl.o $(WLIBS) -o libRSGL.so
	@rm rsgl.o;	

windowsInstall:

windows:
	make windowsBuild
	make windowsInstall

help:
	@echo "make help : runs this help tab"
	@echo "make : runs both build and install"
	@echo "make build : builds libRSGL.so in the local dir"
	@echo "make install : installs the build into /usr/lib, then removes in, runs buld if it's not already build"
	@echo "make update : update preexisting RSGL"
	@echo "make uninstall : uninstall RSGL"