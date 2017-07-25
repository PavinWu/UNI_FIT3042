#include <SDL.h>
#include <stdio.h>
#include "asg1fn.h"

/* Function: drawPPM
 * ------------------
 * draw PPM with name PPMname out to the screen
 * 
 * */
int main(int argc, char *argv[]){
	if(argc!=2){
		printf("One argument only!! (excluding program's name)\n");
		return awMan(1);
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
		int *width = (int *) malloc(sizeof(int));
		int *height = (int *) malloc(sizeof(int));
		int *colorDepth = (int *) malloc(sizeof(int));
		char *buffer = (char *) malloc(sizeof(char));
		FILE *fppm = fopen(argv[1], "r");
		if (fppm == NULL){
			return awMan(4);
		}
		headerChecking(fppm, width, height, colorDepth, buffer);	
		//already checked header. Now just get the values.
		
		/* Create the window */
		window = SDL_CreateWindow(argv[1], SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, *width, *height, SDL_WINDOW_SHOWN);

		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			/* Get screen surface */
			screenSurface = SDL_GetWindowSurface(window);

			/* Fill surface with white */
			/*
			SDL_FillRect(screenSurface, NULL, 
				SDL_MapRGB(screenSurface->format, 0xfe, 0xfe, 0xfe));
			*/

          	/* Make p point to the place we want to draw the pixel */
          	char color[3];
          	color[0] = *buffer;
          	  
			int x, y;
			int i = 1;
			for (y=0; y<*height; y++){
				for (x=0; x<*width; x++){
					int *p = (int *)((char *)screenSurface->pixels
								+ y * screenSurface->pitch 		
								+ x * screenSurface->format->BytesPerPixel);
					/* Draw the pixel! */
					if (!feof(fppm)){
						for (; i<3; i++){
							fread(&color[i], 1, 1, fppm);
						}
						i = 0;
					}
					
					*p=SDL_MapRGB(screenSurface->format, color[0], 
							color[1], color[2]);            
					SDL_UpdateWindowSurface(window);
				}
			}
			SDL_Delay(10000); //wait 10 seconds
		}
		
		free(width); free(height); free(colorDepth); free(buffer);
		fclose(fppm);
	}
	return 0;
}
