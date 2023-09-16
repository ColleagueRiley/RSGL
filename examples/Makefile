ARGS = -lm -I../
LIBS = -lX11 -lGLX 

all:
	make events
	#make glfw
	make shapes

glfw:
	gcc glfw.c -lglfw $(ARGS) -o glfw

shapes:
	gcc shapes.c $(LIBS) $(ARGS) -o shapes

events:
	gcc events.c $(LIBS) $(ARGS) -o events

clean:
	rm events glfw shapes