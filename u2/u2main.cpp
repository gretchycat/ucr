#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_audio.h>
#include "../SDL_rotozoom.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "../SDL_smoothzoom.h"
#include "u2types.h"
#include "u2data.h"
#include "u2draw.h"
#include "../2xsai.h"

int U2_main(SDL_Surface *screen, int audio_open)
{
	TglobalConfig conf;
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif  

	SDL_Surface *dragonimage=SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 200, 32, rmask, gmask, bmask, amask);
	U2_CGAImage(&conf, dragonimage, "picdra", 320, 200);
CGAtoComposite(dragonimage, 0);
	
//	SDL_Surface *temp=SDL_smooth(dragonimage);
//	SDL_FreeSurface(dragonimage);
//	dragonimage=temp;	
	SDL_Surface *buffer=SDL_Scale_2xSai(dragonimage);//zoomSurface(dragonimage, 2, 2, 1);
	SDL_Rect destrect;
	destrect.x=0;destrect.y=0;destrect.w=640;destrect.h=400;
	if(SDL_BlitSurface(buffer, NULL, screen, &destrect))
		printf("\nError with dragon blit.\n");
	SDL_FreeSurface(dragonimage);
	SDL_FreeSurface(buffer);
	SDL_UpdateRect(screen, 0, 0, 640, 400);
	SDL_Delay(2000);
	TU2_map map;
		//30
	U2_loadmap(&conf, map, "mapx32");

/*	for(int x=0;x<64;x++)
	{
		for(int y=0;y<64;y++)
			printf("%02x", map[x][y]);
		printf("\n");
	}*/
	TU2_tiles tiles;
	if(U2_loadtiles(&conf, &tiles))
		return SDL_QUIT;
	
//	printf("Tiles: %d\n", tiles.num);
	int go=1;
	for(int x=0;x<tiles.num;x++)
	{
		if(tiles.tiles[x])
		{
		destrect.x=(x*tiles.tiles[0]->w)%640;
		destrect.y=(x*tiles.tiles[0]->w)/640*tiles.tiles[0]->h;
		destrect.w=tiles.tiles[0]->w;
		destrect.h=tiles.tiles[0]->h;
		//printf("%d: %d, %d, %d, %d  --  %d\n", x, destrect.x, destrect.y, destrect.w, destrect.h, z);
		//printf("%d - %d\n", tiles.xdim, tiles.ydim);
		if(SDL_BlitSurface(tiles.tiles[x], NULL, screen, &destrect))
			printf("ERROR WITH BLIT!!\n");
		}
		else
			printf("NULL POINTER AT %d\n", x);
	}
	int x=0;
	int y=0;
	SDL_Event event;
	while(go)
	{
		int c=getKey(screen, &event);
		switch(c)
		{
			case SDLK_q: go=0;break;
			case SDLK_LEFT: x--;break;
			case SDLK_RIGHT: x++;break;
			case SDLK_UP: y--;break;
			case SDLK_DOWN: y++;break;
			default: break;
		}
		y=(y+64)%64;x=(x+64)%64;
		U2_DrawMap(screen, map, &tiles, x,y);
		SDL_UpdateRect(screen, 0, 0, 640, 400);
		SDL_Delay(50);
	}
	return 0;
}
