# CUSTOM ARGS :
# RGFW_WAYLAND=1 -> use wayland

CC ?= gcc
AR ?= ar

# used for compiling RGFW.o
CUSTOM_CFLAGS ?=
# used for the examples
CFLAGS =

LIBS := -static -lgdi32 -lm -lopengl32 -lwinmm -ggdb
# -ld3d11 -ldxgi -ldxguid -ld3dcompiler
EXT = .exe
LIB_EXT = .dll

OS_DIR = \\

WARNINGS = -Wall -Werror
detected_OS = windows

NO_GLES = 1

# not using a cross compiler
ifeq (,$(filter $(CC),x86_64-w64-mingw32-gcc i686-w64-mingw32-gcc x86_64-w64-mingw32-g++ /opt/msvc/bin/x64/cl.exe /opt/msvc/bin/x86/cl.exe))
	detected_OS := $(shell uname 2>/dev/null || echo Unknown)

	ifeq ($(detected_OS),Darwin)        # Mac OS X
		LIBS := -D GL_SILENCE_DEPRECATION -lm -framework IOKit -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo
		EXT =
		OS_DIR = /
		LIB_EXT = .dylib
		WARNINGS = -Wall
	endif
	ifeq ($(detected_OS),Linux)
		NO_GLES = 0
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
	NO_GLES = 0
	LINK_GL1 = -s LEGACY_GL_EMULATION -D LEGACY_GL_EMULATION -sGL_UNSAFE_OPTS=0
	LINK_GL3 = -s FULL_ES3
	LINK_GL2 = -s FULL_ES2
	PRELOAD = --embed-file logo.png --embed-file COMICSANS.ttf
	EXPORTED_JS = $(PRELOAD) -s EXPORTED_RUNTIME_METHODS="['stringToNewUTF8']"
	LIBS = -s WASM=1 -s ASYNCIFY -s GL_SUPPORT_EXPLICIT_SWAP_CONTROL=1 $(EXPORTED_JS)
	EXT = .js
	NO_GLES = 0
	detected_OS = web
else ifeq (,$(filter $(CC), g++ clang++ x86_64-w64-mingw32-g++))
	LIBS += -std=c99
endif

EXAMPLE_OUTPUTS = \
	basics/basic \
	basics/textures  \
	basics/rfont \
	advanced/shader \
	basics/shapes  \
	basics/cube \
	advanced/compute \
	basics/stack \
	basics/buffers \
	basics/gl11 \
	basics/framebuffer \
	basics/gl2 \

EXAMPLE_OUTPUTS_CUSTOM = \
	microui_demo/microui_demo \
	basics/gles2 \
	basics/gles3 \


all: xdg-shell.c $(EXAMPLE_OUTPUTS) $(EXAMPLE_OUTPUTS_CUSTOM)

examples: $(EXAMPLE_OUTPUTS) $(EXAMPLE_OUTPUTS_CUSTOM)

basics/gles2: basics/gles2.c ../RSGL.h ../renderers/*.h
ifneq ($(NO_GLES), 1)
	$(CC) $(CFLAGS) -g2 $(WARNINGS) -I../renderers/ -I../ -I./deps $< $(LIBS) -o $@$(EXT)
else
	echo no OpenGL ES
endif

basics/gles3: basics/gles3.c ../RSGL.h ../renderers/*.h
ifneq ($(NO_GLES), 1)
	$(CC) $(CFLAGS) -g2 $(WARNINGS) -I../renderers/ -I../ -I./deps $< $(LIBS) -o $@$(EXT)
else
	echo no OpenGL ES
endif

microui_demo/microui_demo:  microui_demo/microui_demo.c microui_demo/*.c ../RSGL.h ../renderers/*.h
	$(CC) $(CFLAGS) -g2 $(WARNINGS) microui_demo/microui.c -I../renderers/ -I../ -I./deps $< $(LIBS) -o $@$(EXT)

advanced/glfw: advanced/glfw.c ../RSGL.h ../renderers/*.h
	$(CC) $(CFLAGS) -g2 $(WARNINGS) -I../renderers/ -I../ -I./deps $< $(LIBS) -lglfw -o $@$(EXT)

$(EXAMPLE_OUTPUTS): %: %.c ../RSGL.h ../renderers/*.h
	$(CC) $(WARNINGS) -g2 -I../renderers/ -I../ -I./deps $< $(LIBS) $(LINK_GL3) $(CFLAGS)  -o $@$(EXT)

debug: all
	@for exe in $(EXAMPLE_OUTPUTS); do \
		echo "Running $$exe..."; \
		./$$exe$(EXT); \
	done

	make advanced/glfw
	.$(OS_DIR)advanced$(OS_DIR)glfw
	make clean

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
	rm -f */*.exe *.o *.obj *.dll .dylib *.a *.so $(EXAMPLE_OUTPUTS) $(EXAMPLE_OUTPUTS_CUSTOM)  .$(OS_DIR)examples$(OS_DIR)*$(OS_DIR)*.exe .$(OS_DIR)examples$(OS_DIR)*$(OS_DIR)*.js .$(OS_DIR)examples$(OS_DIR)*$(OS_DIR)*.wasm


.PHONY: all examples clean

