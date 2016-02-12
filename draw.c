#include "SDL2/SDL.h"
#include <stdio.h>
#include "tsplib.h"

int main(){
	SDL_Window * window = NULL;
	SDL_Renderer* renderer = NULL;
	tspfile file;
	int i =0;
	SDL_Init( SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("SPEEEEEED", SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED, 1000, 750, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		
	parse_file("tsp_lib/pla85900.tsp",&file);

	SDL_RenderClear(renderer);
	for(i = 0; i<file.dimension;i++){
		SDL_RenderDrawPoint(renderer, (file.nodes[i].x/1200)-450, (file.nodes[i].y/1200)- 450);
	}
	
	SDL_RenderPresent(renderer);

	SDL_Delay(16);
	//}
	getchar();
	}