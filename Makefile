CUSTOM_CFLAGS =
CC = gcc

all: RSGL.h
	cd examples && make CC=$(CC) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

debug: RSGL.h
	cd examples && make debug CC=$(CC) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

EXAMPLE_OUTPUTS = \
	basics/basic \
	basics/text \
	basics/textures  \
	advanced/shader \
	basics/shapes  \
	basics/camera \
	advanced/compute \


$(EXAMPLE_OUTPUTS): %: examples/%.c RSGL.h
	cd examples && make $@ CC=$(CC) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

examples/advanced/glfw: examples/advanced/glfw.c RSGL.h 
	cd examples && make $@ CC=$(CC) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

clean:
	cd examples && make clean
