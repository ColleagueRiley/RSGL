CUSTOM_CFLAGS =
CC = gcc

all: RSGL.h
	cd examples && make CC=$(CC) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

debug: RSGL.h
	cd examples && make debug CC=$(CC) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

clean:
	cd examples && make clean	
