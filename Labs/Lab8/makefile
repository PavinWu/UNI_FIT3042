CFLAGS=`sdl2-config --cflags`
SDLLIBS=`sdl2-config --libs`

pixel: hello-pixel.c
	gcc -Wall -o pixel hello-pixel.c $(CFLAGS) $(SDLLIBS) 

window: hello-window.c
	gcc -Wall -o window hello-window.c $(CFLAGS) $(SDLLIBS)

events: hello-events.c
	gcc -Wall -o events hello-events.c $(CFLAGS) $(SDLLIBS)
timer:	timer.c
	gcc -Wall -o timer timer.c $(CFLAGS) $(SDLLIBS)
