all: RSGL.h
	cd examples && make CUSTOM_CFLAGS=$(CUSTOM_CFLAGS)

debug: RSGL.h
	cd examples && make debug CUSTOM_CFLAGS=$(CUSTOM_CFLAGS)

libRSGL.a: RSGL.h
	cd examples && make libRSGL.a CUSTOM_CFLAGS=$(CUSTOM_CFLAGS)

RSGL.o: RSGL.h
	$(CC) $(CUSTOM_CFLAGS) -x c -c RSGL.h -fPIC -DRSGL_IMPLEMENTATION
	cd examples && make CUSTOM_CFLAGS=$(CUSTOM_CFLAGS)

build_examples: RSGL.h
	cd examples && make CUSTOM_CFLAGS=$(CUSTOM_CFLAGS)

clean:
	cd examples && make clean	
