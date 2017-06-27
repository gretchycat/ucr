#include <stdio.h>   
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_endian.h>
#include "../SDL_rotozoom.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "u3types.h"
#include "u3data.h"

int U3_CGAImage(TglobalConfig *conf, SDL_Surface *screen, char *filename, int xx, int yy)
{
	FILE *in;
	Tcolor cga[4];
	setCGAcolors(cga);
	char tempfn[512];
	sprintf(tempfn, "%s/%s", conf->game[3].gamepath, filename);
	if((in=fopen(tempfn, "rb"))!=NULL)
	{
		for(int y=0;y<yy/2;y++)
			for(int x=0;x<xx/4;x++)
			{
				unsigned char c;
				fread(&c, 1, 1, in);
				Uint8 color=(c/16)/4;
				DrawPixel(screen, x*4, y*2,cga[color].R, cga[color].G, cga[color].B);
				color=(c/16)%4;
				DrawPixel(screen, x*4+1, y*2,cga[color].R, cga[color].G, cga[color].B);
				color=(c%16)/4;
				DrawPixel(screen, x*4+2, y*2,cga[color].R, cga[color].G, cga[color].B);
				color=(c%16)%4;
				DrawPixel(screen, x*4+3, y*2,cga[color].R, cga[color].G, cga[color].B);
			}
		for(int z=0;z<(512-xx);z++)
		{
			unsigned char c;
			fread(&c, 1, 1, in);
		}
		for(int y=0;y<yy/2;y++)
			for(int x=0;x<xx/4;x++)
			{
				unsigned char c;
				fread(&c, 1, 1, in);
				Uint8 color=(c/16)/4;
				DrawPixel(screen, x*4, y*2+1,cga[color].R, cga[color].G, cga[color].B);
				color=(c/16)%4;
				DrawPixel(screen, x*4+1, y*2+1,cga[color].R, cga[color].G, cga[color].B);
				color=(c%16)/4;
				DrawPixel(screen, x*4+2, y*2+1,cga[color].R, cga[color].G, cga[color].B);
				color=(c%16)%4;
				DrawPixel(screen, x*4+3, y*2+1,cga[color].R, cga[color].G, cga[color].B);
			}
		fclose(in);
	}
	else
		return 1;
	return 0;
}
