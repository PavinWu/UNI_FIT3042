/* This program adapted from http://lazyfoo.net/tutorials/SDL/01_hello_SDL */
/* by Robyn McNamara, April 2016 */

#include <SDL.h>
#include <stdlib.h>

/* Setting up screen size*/
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int DrawRect(SDL_Surface *screenSurface, SDL_Window *window, int stop_px, int start_x, int start_y){	
	
	/* Fill surface with white */
	SDL_FillRect(screenSurface, NULL, 
		SDL_MapRGB(screenSurface->format, 0x42, 0x5f, 0xf4));

	int i, j, *p;
	int stop_px_y = stop_px;
	if (start_x < 0){
		start_x = 0;
	}
	if (stop_px_y + start_x > SCREEN_WIDTH){
		stop_px_y = SCREEN_WIDTH-start_x;
	}
	if (stop_px + start_y > SCREEN_HEIGHT){
		stop_px = SCREEN_HEIGHT-start_y;
	}
	for (i=start_y; i<stop_px+start_y; i++){
		for (j=start_x; j<stop_px_y+start_x; j++){
			/* Make p point to the place we want to draw the pixel */  
			p = (int *)((char *)screenSurface->pixels 	//pointer to first pixel
						+ i * screenSurface->pitch 		// y*linelen
						+ j * screenSurface->format->BytesPerPixel);  //x*pixelsize?          

			/* Draw the pixel! */     
			*p=SDL_MapRGB(screenSurface->format, 0xf4, 0x42, 0x42);            
		}
	}
	SDL_UpdateWindowSurface(window);
	return 0;
}

void timer_action(void *ignored) {
	// print a message to demonstrate that the timer has gone off
	printf("Time passed\n");
}

Uint32 timercallback(Uint32 interval, void *parameter) {
	// create an event to let main() know it needs to do something
	SDL_Event event;
	SDL_UserEvent user;
	user.type=SDL_USEREVENT;
	user.code=0;
	
	// function we want main event loop to call, and its data
	user.data1=&timer_action;
	user.data2=NULL;
	event.type=SDL_USEREVENT;

	// the UserEvent struct sits inside the Event
	event.user=user;
	SDL_PushEvent(&event);

	// we could change the interval between timer ticks if we wanted to
	return interval;
}


int main(int argc, char *argv[]) {
	char *endp;
	/*if (!isdigit(argv[1])){
		printf("input is not a number\n");
		exit(-1);
	}*/
	int stop_px;
	if (argv[1]==NULL || !(stop_px = (int) strtol(argv[1], &endp, 10))){
		printf("input is not a number\n");
		exit(-1);
	}
	if (stop_px >= SCREEN_HEIGHT || stop_px >= SCREEN_WIDTH){
		printf("Specified side length exceeds width or height\n");
		exit(-2);
	}
          	
	/* The window to render to */
	SDL_Window *window = NULL;

	/* The surface contained by the window */
	SDL_Surface *screenSurface = NULL;

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	else {
		/* Create the window */
		window = SDL_CreateWindow("Hello Pixel",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			/* Get screen surface */
			screenSurface = SDL_GetWindowSurface(window);

			SDL_Event event;
			int start_x = -10;
			int start_y	= 0;
			SDL_AddTimer(500, timercallback, NULL);
			
			while (SDL_WaitEvent(&event)){
				switch (event.type){
					case SDL_KEYDOWN:
						switch(event.key.keysym.sym){
							case SDLK_d:
								if (start_x < SCREEN_WIDTH){
									start_x+=10;
									DrawRect(screenSurface, window, stop_px, start_x, start_y);
								}
								else {
									/* Destroy the window */
									SDL_DestroyWindow(window);
									SDL_Quit();
								}
								break;
							case SDLK_a:
								if (start_x+stop_px > 0){
									start_x-=10;
									DrawRect(screenSurface, window, stop_px, start_x, start_y);
								}
								else {
									/* Destroy the window */
									SDL_DestroyWindow(window);
									SDL_Quit();
								}
								break;
						}
						break;
					case SDL_USEREVENT:
						// function the timer wants us to call, and its data
						/*
						fptr = event.user.data1;		// NOT user.data1!
						void *arg = event.user.data2;
						fptr(arg);						
						*/
						DrawRect(screenSurface, window, stop_px, start_x, start_y);
						start_y+=30;
						if (start_y >= SCREEN_HEIGHT){
							start_y = 0;
						}
						break;
					default:
						break;
				}
			}
		}

	}
	return 0;
}
