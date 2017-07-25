/* This program adapted from http://lazyfoo.net/tutorials/SDL/01_hello_SDL */
/* by Robyn McNamara, April 2016 */

#include <SDL.h>
#include <stdlib.h>

/* Setting up screen size*/
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[]) {
	char *endp;
	/*if (!isdigit(argv[1])){
		printf("input is not a number\n");
		exit(-1);
	}*/
	int len;
	if (argv[1]==NULL || !(len = (int) strtol(argv[1], &endp, 10))){
		printf("input is not a number\n");
		exit(-1);
	}
	if (len >= SCREEN_HEIGHT || len >= SCREEN_WIDTH){
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

			/* Fill surface with white */
			SDL_FillRect(screenSurface, NULL, 
				SDL_MapRGB(screenSurface->format, 0xfe, 0xfe, 0xfe));


          	/* Make p point to the place we want to draw the pixel */  
          	int i, j;
          	for (i=0; i<len; i++){
				for (j=0; j<len; j++){
					int *p = (int *)((char *)screenSurface->pixels 	//pointer to first pixel
								+ i * screenSurface->pitch 		// y*linelen
								+ j * screenSurface->format->BytesPerPixel);  //x*pixelsize?          

					/* Draw the pixel! */     
					*p=SDL_MapRGB(screenSurface->format, 0, 0, 0);            
					
				}
			}
			SDL_UpdateWindowSurface(window);
			
			/* Wait two seconds */
			SDL_Delay(2000); 
		}

		/* Destroy the window */
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	return 0;
}
