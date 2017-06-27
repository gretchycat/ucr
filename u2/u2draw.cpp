#include <SDL.h>
#include "u2types.h"

int U2_DrawMap(SDL_Surface *screen, TU2_map map, TU2_tiles *tiles, int x, int y)
{
	for(int xx=0;xx<20;xx++)
	{
		for(int yy=0;yy<10;yy++)
		{
			SDL_Rect rect;
			rect.x=xx*tiles->tiles[0]->w;
			rect.y=yy*tiles->tiles[0]->h;
			rect.w=tiles->tiles[0]->w;
			rect.h=tiles->tiles[0]->h;
			int mx=(x+xx-10+64)%64;
			int my=(y+yy-5+64)%64;
//			printf("%d, %d\n", mx, my);
//			printf("%3X", map[my][mx]);
			SDL_BlitSurface(tiles->tiles[map[my][mx]/4], NULL, screen, &rect);
		}
//		printf("\n");
	}
	return 0;
}

