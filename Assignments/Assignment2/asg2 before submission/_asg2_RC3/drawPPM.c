/* 
 * drawPPM.c:
 * 
 * Draw ppm file with input as an argument on to the screen.
 * Adapted from hello-pixel.c by Robyn Mcnamara.
 * This is a part of FIT3042 assignment2.
 * 
 * By: Pavin Wu (26916592)
 * Date: 24/4/17
 */

#include <SDL.h>
#include <stdio.h>
#include "asg1fn.h"

int main(int argc, char *argv[]){
	/* checked argument */
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
		FILE *fppm = fopen(argv[1], "r");
		if (fppm == NULL){
			return awMan(4);
		}

		int *width = (int *) malloc(sizeof(int));
		int *height = (int *) malloc(sizeof(int));
		int *colorDepth = (int *) malloc(sizeof(int));
		char *buffer = (char *) malloc(sizeof(char));
		
		headerChecking(fppm, width, height, colorDepth, buffer);	
		// Do this just to get values. Already checked header in hidev2.c.
		
		/* Create the window */
		window = SDL_CreateWindow(argv[1], SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, *width, *height, SDL_WINDOW_SHOWN);

		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			/* Get screen surface */
			screenSurface = SDL_GetWindowSurface(window);

          	/* Make p point to the place we want to draw the pixel */
          	char color[3];
          	color[0] = *buffer;
          	  
			int x, y;
			int i = 1;
			for (y=0; y<*height; y++){
				for (x=0; x<*width; x++){
					/* define pixel position */
					int *p = (int *)((char *)screenSurface->pixels
								+ y * screenSurface->pitch 		
								+ x * screenSurface->format->BytesPerPixel);
					
					/* get color (1byte each channel) */
					if (!feof(fppm)){
						for (; i<3; i++){
							fread(&color[i], 1, 1, fppm);
						}
						i = 0;
					}
					
					/* Draw the pixel! */
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
