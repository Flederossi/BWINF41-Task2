#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "config.h"

typedef struct{
	int grow[4];
	int spawn;
	int ori;
}Crystal;

typedef struct{
	int x;
	int y;
}Position;

Crystal map[W][H];
Crystal nextmap[W][H];

void copy(int back){
	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			if(back){
				nextmap[x][y] = map[x][y];
			}else{
				map[x][y] = nextmap[x][y];
			}
		}
	}
}

int main(int argc, char *argv[]){
	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			map[x][y] = (Crystal){{0, 0, 0, 0}, 0, -1};
		}
	}
	
	Crystal init_cry[INITNUM];
	Position init_pos[INITNUM];
	srand(time(NULL));
	for(int i = 0; i < INITNUM; i++){
		init_cry[i] = (Crystal){{rand() % GMAX+GMIN, rand() % GMAX+GMIN, rand() % GMAX+GMIN, rand() % GMAX+GMIN}, rand() % SMAX+SMIN, greyscale[rand() % 4]};
		init_pos[i] = (Position){rand() % W, rand() % H};
		printf("Number: %d\tGrowingrates: %d, %d, %d, %d\tPosition: %d, %d\tSpawn: %d\tOrientation: %d\n", i, init_cry[i].grow[0], init_cry[i].grow[1], init_cry[i].grow[2], init_cry[i].grow[3], init_pos[i].x, init_pos[i].y, init_cry[i].spawn, init_cry[i].ori);
	}

	for(int i = 0; i < ITER; i++){
		for(int e = 0; e < INITNUM; e++){
			if(i >= init_cry[e].spawn && map[init_pos[e].x][init_pos[e].y].ori == -1){
				map[init_pos[e].x][init_pos[e].y] = init_cry[e];
			}
		}
		copy(1);
		for(int y = 0; y < H; y++){
			for(int x = 0; x < W; x++){
				Crystal curr = map[x][y];
				if(curr.ori > -1 && i >= curr.spawn){
					for(int dir = 0; dir < 4; dir++){
						for(int dist = 0; dist < curr.grow[dir]; dist++){
							int check_pos_x[4] = {x, x + dist + 1, x, x - dist - 1};
							int check_pos_y[4] = {y - dist - 1, y, y + dist + 1, y};
							int check_border[4] = {y - dist - 1 >= 0, x + dist + 1 < W, y + dist + 1 < H, x - dist - 1 >= 0};
							if(check_border[dir] && nextmap[check_pos_x[dir]][check_pos_y[dir]].ori == -1){
								nextmap[check_pos_x[dir]][check_pos_y[dir]] = curr;
							}else{
								break;
							}
						}
					}
				}
			}
		}
		copy(0);
	}
	
	SDL_Event event;
	SDL_Renderer *renderer;
	SDL_Window *window;
	int c;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(W, H, 0, &window, &renderer);

	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			if((c = map[x][y].ori) != -1){
				SDL_SetRenderDrawColor(renderer, c, c, c, 255);
			}
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}

	SDL_RenderPresent(renderer);
	while(1){
		if(SDL_PollEvent(&event) && event.type == SDL_QUIT){
			break;
		}
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}