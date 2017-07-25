CFLAGS=`sdl2-config --cflags`
SDLLIBS=`sdl2-config --libs`

all: hide unhide drawPPM

hide: hidev2.c asg1fn.c asg1fn.h asg2fn.c asg2fn.h extendhidev2.c extendhidev2.h
	gcc -Wall -o hide hidev2.c asg1fn.c asg2fn.c extendhidev2.c 
	
unhide: unhidev2.c asg1fn.c asg1fn.h asg2fn.c asg2fn.h
	gcc -Wall -o unhide unhidev2.c asg1fn.c asg2fn.c 

drawPPM: drawPPM.c asg1fn.c asg1fn.h
	gcc -Wall -o drawPPM asg1fn.c drawPPM.c $(CFLAGS) $(SDLLIBS)

all-g: hide-g unhide-g

hide-g: hidev2.c asg1fn.c asg1fn.h asg2fn.c asg2fn.h extendhidev2.c extendhidev2.h
	gcc -Wall -g -o hide hidev2.c asg1fn.c asg2fn.c extendhidev2.c
	
unhide-g: unhidev2.c asg1fn.c asg1fn.h asg2fn.c asg2fn.h
	gcc -Wall -g -o unhide unhidev2.c asg1fn.c asg2fn.c 
	
drawPPM-g: drawPPM.c asg1fn.c asg1fn.h
	gcc -Wall -g -o drawPPM asg1fn.c drawPPM.c $(CFLAGS) $(SDLLIBS)
