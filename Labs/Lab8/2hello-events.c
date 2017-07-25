/* Based on hello-window.c */
#include <SDL.h>

/* Setting up screen size*/
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[]) {
	/* The window to render to */
	SDL_Window *window = NULL;

	/* The surface contained by the window */
	SDL_Surface *screenSurface = NULL;

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	else {
		/* Create the window */
		window = SDL_CreateWindow("Hello Window",
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
							SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));

			/* Update the screen */
			SDL_UpdateWindowSurface(window);

			/* wait for keypress */
			SDL_Event event;
			while (SDL_WaitEvent(&event)) {
				switch(event.type) {
					case SDL_KEYDOWN:
						SDL_FillRect(screenSurface, NULL,
										SDL_MapRGB(screenSurface->format, 0, 0, 0));
						SDL_UpdateWindowSurface(window);
						
						/* handle special case: quit */
						if (event.key.keysym.sym == SDLK_q) {
							SDL_DestroyWindow(window);
							SDL_Quit();
							return 0;
						}

						break;

					case SDL_KEYUP:
						SDL_FillRect(screenSurface, NULL,
										SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
						SDL_UpdateWindowSurface(window);
				}
			}
		}
	}
	return 0;
}
