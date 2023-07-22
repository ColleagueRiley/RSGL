INSTALL_PATH = /usr

ifeq ($(OS),Windows_NT)
OS = Windows_NT
else
OS = linux
endif

build:
	@if [ $(CXX)false = false ]; then\
		CXX := g++;\
		echo $(CXX);\
	fi

	@mkdir -p build build/include build/include/RSGL
	@cp RSGL.h build/include/RSGL
	@if [ $(HEADERS-ONLY)false = false ]; then\
		cp -r ./source build/include/RSGL;\
	fi
	
	@if [ $(COMPILE)true = true ] && [ $(shell uname) != Darwin ]; then\
		cp RSGL.h ./build/RSGL.c;\
		mkdir -p build/obj build/lib;\
		$(CXX) -I./ -fPIC -c ./build/RSGL.c -D RSGL_IMPLEMENTATION -o ./build/obj/RSGL.o;\
		rm ./build/RSGL.c;\
		ar rcs ./build/lib/libRSGL.a ./build/obj/RSGL.o;\
	fi

	@if [ $(OS) = Windows_NT ] && [ $(COMPILE)true = true ] ; then\
		$(CXX) -shared -lopengl32 -lshell32 -lgdi32 ./build/obj/RSGL.o -o ./build/lib/libRSGL.dll;\
	fi

	@if [ $(shell uname) = Darwin ] && [ $(COMPILE)true = true ]; then\
		cp RSGL.h ./build/RSGL.c;\
		mkdir -p build/obj build/lib;\
		gcc ./source/deps/RGFW.h -D RSGL_IMPLEMENTATION -I./source/deps -o ./build/obj/RGFW.o;\
		$(CXX) -I./ -fPIC -c ./build/RSGL.c ./build/obj/RGFW.o -o ./build/obj/RSGL.o;\
		rm ./build/RSGL.c;\
		ar rcs ./build/lib/libRSGL.a ./build/obj/RSGL.o;\
		make silicon;\
		$(CXX) -shared  -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo ./build/obj/*.o -o ./build/lib/libRSGL.dynlib;\
		ar rcs ./build/lib/libRSGL.a ./build/obj/*.o;\
	fi

	@if [ $(OS) != Windows_NT ] && [ $(shell uname -s) != Darwin ] && [ $(COMPILE)true = true ]; then\
		echo hi;\
		$(CXX) -shared -lX11 -lGLX ./build/obj/RSGL.o -o ./build/lib/libRSGL.so;\
	fi


install:
	@if [ $(INSTALL_PATH)TRUE = /usrTRUE ] && [ `uname` =  Windows_NT ]; then\
		echo If you want to install RSGL on linux you have to manually set your install path;\
		echo "you can do this by adding INSTALL_PATH=<path> to your make command";\
		echo your install path is the folder that olds your /lib and /include folders;\
		echo ex. /usr on linux;\
		exit 1;\
	fi

	@if [ $(INSTALL_PATH)TRUE = /usrTRUE ] && [ `uname` =  Darwin ]; then\
		INSTALL_PATH = /Library/Developer/CommandLineTools/usr;\
	fi

	@echo installing RSGL headers
	@cp build/include/* -r $(INSTALL_PATH)/include
	
	@if [ $(COMPILE)true = true ]; then\
		cp build/lib/libRSGL.so $(INSTALL_PATH)/lib;\
		cp build/lib/libRSGL.a $(INSTALL_PATH)/local/lib;\
	fi

silicon:
	cd Silicon && make

clean:
	rm -r build -p