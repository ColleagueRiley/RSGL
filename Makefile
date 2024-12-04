CUSTOM_CFLAGS =
CC = gcc

all: RSGL.h
	cd examples && make CC=$(CC) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

debug: RSGL.h
	cd examples && make debug CC=$(CC) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

libRSGL.a: RSGL.h
	cd examples && make ../libRSGL.a CC=$(CC) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

RSGL.o: RSGL.h
	cd examples && make ../RSGL.o CC=$(CC) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

build_examples: RSGL.h
	cd examples && make CC=$(CC) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

clean:
	cd examples && make clean	
