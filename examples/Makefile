# CUSTOM ARGS :
# RGFW_WAYLAND=1 -> use wayland 

CC = gcc
AR = ar

# used for compiling RGFW.o
CUSTOM_CFLAGS ?=
# used for the examples
CFLAGS =  

LIBS := -static -lgdi32 -lm -lopengl32 -lwinmm -ggdb
EXT = .exe
LIB_EXT = .dll

OS_DIR = \\

WARNINGS = -Wall -Werror
detected_OS = windows

# not using a cross compiler
ifeq (,$(filter $(CC),x86_64-w64-mingw32-gcc i686-w64-mingw32-gcc x86_64-w64-mingw32-g++ /opt/msvc/bin/x64/cl.exe /opt/msvc/bin/x86/cl.exe))
	detected_OS := $(shell uname 2>/dev/null || echo Unknown)

	ifeq ($(detected_OS),Darwin)        # Mac OS X
		LIBS := -lm -framework IOKit -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo
		EXT =
		OS_DIR = /
		LIB_EXT = .dylib
		WARNINGS = -Wall 
	endif
	ifeq ($(detected_OS),Linux)
    	LIBS := -lXrandr -lX11 -lm -lGL -ldl -lpthread
		EXT =
		OS_DIR = /
		NO_GLES = 0
		LIB_EXT = .so
	endif
else
	OS_DIR = /
endif

ifeq ($(RGFW_WAYLAND),1)
	LIBS = -D RGFW_WAYLAND relative-pointer-unstable-v1-client-protocol.c xdg-decoration-unstable-v1.c xdg-shell.c -lwayland-cursor -lwayland-client -lEGL -lxkbcommon -lGL -lwayland-egl -lm
endif

LINK_GL1 =
LINK_GL3 =
LINK_GL2 =

ifneq (,$(filter $(CC),cl /opt/msvc/bin/x64/cl.exe /opt/msvc/bin/x86/cl.exe))
	WARNINGS =
	LIBS = /static
else ifeq ($(CC),emcc)
	LINK_GL1 = -s LEGACY_GL_EMULATION -D LEGACY_GL_EMULATION -sGL_UNSAFE_OPTS=0
	LINK_GL3 = -s FULL_ES3
	LINK_GL2 = -s FULL_ES2
	EXPORTED_JS = -s EXPORTED_RUNTIME_METHODS="['stringToNewUTF8']" 
	LIBS = -s WASM=1 -s ASYNCIFY -s USE_WEBGL2 -s GL_SUPPORT_EXPLICIT_SWAP_CONTROL=1 $(EXPORTED_JS)
	EXT = .js
	NO_GLES = 0
	detected_OS = web
	
	LIBS += -DRSGL_GL_NO_LEGACY 
else
	LIBS += -std=c99 
endif

EXAMPLE_OUTPUTS = \
	basics/basic \
	basics/text \
	ui/container \
	advanced/glVer \
	basics/textures  \
	ui/styles \
	advanced/shader \
	basics/shapes  \
	ui/button \
	ui/textbox

all: xdg-shell.c $(EXAMPLE_OUTPUTS) advanced/rsoft ../libRSGL$(LIB_EXT) ../libRSGL.a

examples: $(EXAMPLE_OUTPUTS)


advanced/rsoft: advanced/rsoft.c ../RSGL.h
ifeq ($(CC),emcc)
	$(CC) $(CFLAGS) $(WARNIGNS) -I../ -I./deps $< -s LEGACY_GL_EMULATION -D LEGACY_GL_EMULATION -sGL_UNSAFE_OPTS=0 -s EXPORTED_RUNTIME_METHODS="['stringToNewUTF8']" -o $@$(EXT)
else
	$(CC) $(CFLAGS) $(WARNIGNS) -I../ -I./deps $< $(LIBS) -o $@$(EXT)
endif

advanced/glfw: advanced/glfw.c ../RSGL.h 
	$(CC) $(CFLAGS) $(WARNIGNS) -I../ -I./deps $< $(LIBS) -lglfw -o $@$(EXT)

advanced/glfw_ui: advanced/glfw_ui.c ../RSGL.h 
	$(CC) $(CFLAGS) $(WARNIGNS) -I../ -I./deps $< $(LIBS) -lglfw -o $@$(EXT)

$(EXAMPLE_OUTPUTS): %: %.c ../RSGL.h
	$(CC) $(WARNINGS) -I../ -I./deps $< $(LIBS) $(LINK_GL3) $(CFLAGS)  -o $@$(EXT)

debug: all
	@for exe in $(EXAMPLE_OUTPUTS); do \
		echo "Running $$exe..."; \
		.$(OS_DIR)$$exe$(EXT); \
	done
	
	make advanced/rsoft
	.$(OS_DIR)advanced$(OS_DIR)rsoft

	make advanced/glfw
	make advanced/glfw_ui
	.$(OS_DIR)advanced$(OS_DIR)glfw
	.$(OS_DIR)advanced$(OS_DIR)glfw_ui
	make clean


../RSGL.o: ../RSGL.h
	make initwayland
	#$(CC) -x c $(CUSTOM_CFLAGS) -c ../RSGL.h -D RSGL_IMPLEMENTATION -fPIC -D RSGL_EXPORT -D RGFW_EXPORT
	cp ../RSGL.h RSGL.c
	$(CC) -o ../RSGL.o $(CUSTOM_CFLAGS) -I../ -I./deps -c RSGL.c -D RSGL_IMPLEMENTATION -fPIC -D RSGL_EXPORT 
	rm RSGL.c

../libRSGL$(LIB_EXT): ../RSGL.h ../RSGL.o
	make ../RSGL.o
	$(CC) $(CUSTOM_CFLAGS) -shared ../RSGL.o $(LIBS) -o ../libRSGL$(LIB_EXT)

../libRSGL.a: ../RSGL.h ../RSGL.o
	make ../RSGL.o
	$(AR) rcs ../libRSGL.a ../RSGL.o

xdg-shell.c:
	make initwayland

initwayland:
ifeq ($(RGFW_WAYLAND),1)
	wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml xdg-shell.h
	wayland-scanner public-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml xdg-shell.c
	wayland-scanner client-header /usr/share/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml xdg-decoration-unstable-v1.h
	wayland-scanner public-code /usr/share/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml xdg-decoration-unstable-v1.c	
	wayland-scanner client-header /usr/share/wayland-protocols/unstable/relative-pointer/relative-pointer-unstable-v1.xml relative-pointer-unstable-v1-client-protocol.h 
	wayland-scanner client-header /usr/share/wayland-protocols/unstable/relative-pointer/relative-pointer-unstable-v1.xml relative-pointer-unstable-v1-client-protocol.c
else
		
endif

clean:
	rm -f *.o *.obj *.dll .dylib *.a *.so $(EXAMPLE_OUTPUTS) $(EXAMPLE_OUTPUTS_CUSTOM)  .$(OS_DIR)examples$(OS_DIR)*$(OS_DIR)*.exe .$(OS_DIR)examples$(OS_DIR)*$(OS_DIR)*.js .$(OS_DIR)examples$(OS_DIR)*$(OS_DIR)*.wasm 
	

.PHONY: all examples clean

