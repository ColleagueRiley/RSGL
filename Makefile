all3:
	@make Build
	#@make install install

Build:
	@mkdir -p build build/include build/include/RSGL
	@cp RSGL.hpp build/include/RSGL
	@if [ $(HEADERS-ONLY)false = falsefalse ]; then\
		cp -r ./source build/include/RSGL;\
	fi
	
	@if [ $(COMPILE)true = true ]; then\
		mkdir -p build/obj build/lib;\
		cp RSGL.hpp RSGL.cpp;\
		g++ -fPIC -c RSGL.cpp -D RSGL_IMPLEMENTATION -o ./build/obj/RSGL.o;\
		rm RSGL.cpp;\
		g++ -shared ./build/obj/RSGL.o -o ./build/lib/libRSGL.so;\
		ar rcs ./build/lib/libRSGL.a ./build/obj/RSGL.o;\
	fi

install:
	@echo installing RSGL headers
	@cp build/include/* -r /usr/include