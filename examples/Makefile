ARGS = -lm -I../
LIBS = -lX11 -lGLX

all:
	make events
	make glfw
	make rect

glfw:
	gcc glfw.c -lglfw $(ARGS) -o glfw

rect:
	gcc rect.c $(LIBS) $(ARGS) -o rect

events:
	gcc events.c $(LIBS) $(ARGS) -o events

clean:
	rm events glfw rect