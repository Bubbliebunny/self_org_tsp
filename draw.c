#include "SDL2/SDL.h"
#include <stdio.h>
#include "tsplib.h"
#include "init.h"
//#include "SDL2/SDL_ttf.h"

#define SCALE 10

static void draw_point(struct city* point, SDL_Renderer * renderer);
static void draw_line(struct city* point1, struct city* point2,
		SDL_Renderer * renderer);

struct SDL_Window * window = NULL;
struct SDL_Renderer* renderer = NULL;

void draw_map(struct list *all_list) {
	struct list_elem *curr = all_list->head;

	/*if(TTF_Init()==-1) {
	    printf("TTF_Init: %s\n", TTF_GetError());
	    //exit(2);
	}*/
	if (window == NULL)
		window = SDL_CreateWindow("Map", SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED, 1500, 750, SDL_WINDOW_SHOWN);
	if (renderer == NULL)
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(renderer);
	do {
		draw_line(curr->data->id, curr->right->data->id, renderer);
		draw_point(curr->data->id, renderer);
		curr = curr->right;
	} while (curr != all_list->head);
	SDL_RenderPresent(renderer);
	//SDL_Delay(60);
//	/getchar();
}

static void draw_point(struct city* point, SDL_Renderer * renderer) {
	/*printf("hi\n");
	TTF_Font* Sans;
	if((Sans = TTF_OpenFont("Sans.ttf", 24)) == NULL){
		printf("oh no\n");
	}
	printf("hi\n");
	SDL_Color White = {255, 255, 255};
	printf("hi\n");
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "1", White);
	printf("hi\n");
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	printf("hi\n");
	SDL_Rect Message_rect; //create a rect
	Message_rect.x = 0;  //controls the rect's x coordinate
	Message_rect.y = 0; // controls the rect's y coordinte
	Message_rect.w = 100; // controls the width of the rect
	Message_rect.h = 100; // controls the height of the rect
	printf("hi\n");
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
	TTF_CloseFont(Sans);*/
	SDL_RenderDrawPoint(renderer, point->x * SCALE, point->y * SCALE);
}

static void draw_line(struct city* point1, struct city* point2,
		SDL_Renderer * renderer) {
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, point1->x * SCALE, point1->y * SCALE,
			point2->x * SCALE, point2->y * SCALE);
}
