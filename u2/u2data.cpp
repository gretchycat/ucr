#include <stdio.h>   
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_endian.h>
#include "../SDL_rotozoom.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "u2types.h"
#include "u2data.h"
#include "../2xsai.h"

int U2_loadmap(TglobalConfig *conf, TU2_map map, char *filename)
{
	FILE *in;
	char tempfn[512];
	sprintf(tempfn, "%s/%s", conf->game[2].gamepath, filename);
	if((in=fopen(tempfn, "rb"))!=NULL)
	{
		for(int x=0;x<64;x++)
			for(int y=0;y<64;y++)
			{
				char n;
				fread(&n, sizeof(char), 1, in);
				map[x][y]=n;
			} 
		fclose(in);
	}
	else
	{
		return 1;
	}
	return 0;
}

int U2_CGAImage(TglobalConfig *conf, SDL_Surface *screen, char *filename, int xx, int yy)
{
	FILE *in;
	Tcolor cga[4];
	setCGAcolors(cga);
	char tempfn[512];
	sprintf(tempfn, "%s/%s", conf->game[2].gamepath, filename);
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


int U2_loadtiles(TglobalConfig *conf, TU2_tiles *tiles)
{
	int ysize=16;
	int xsize=16;
	if((ysize>0)&&(ysize<=64))
		tiles->ydim=ysize;
	if((xsize>0)&&(xsize<=64))
		tiles->xdim=xsize;
	Uint32 rmask, gmask, bmask, amask; 
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask =	0x00ff0000;
		amask =	0xff000000;
	#endif 	 
	SDL_Surface *alltiles=NULL;
	if(conf->game[2].svga)
	{   
		ysize=2*ysize;
		xsize=2*xsize;
		tiles->xdim=32;
		tiles->ydim=32;
		char tempfn[512];
		sprintf(tempfn, "%s/u2/u2tiles.bmp", conf->datapath);
		alltiles=SDL_LoadBMP(tempfn);
		if(alltiles==NULL)
			return 1;
		tiles->num=alltiles->w/tiles->ydim;
		for(int z=0;z<tiles->num;z++)
		{
			SDL_Surface *src=SDL_CreateRGBSurface(SDL_SWSURFACE, tiles->xdim, tiles->ydim, 32, rmask, gmask, bmask, amask);
			SDL_Rect destrect;
			destrect.x=0;destrect.y=0;destrect.w=tiles->xdim;destrect.h=tiles->ydim;
			SDL_Rect srcrect;
			srcrect.x=z*tiles->xdim;srcrect.y=0;srcrect.w=tiles->xdim;srcrect.h=tiles->ydim;
			if(SDL_BlitSurface(alltiles, &srcrect, src, &destrect))
				printf("\nError with blit.\n");
			SDL_SetColorKey(src, SDL_SRCCOLORKEY, SDL_MapRGBA(src->format, 247, 2, 247, 255));
			tiles->tiles[z]=SDL_DisplayFormatAlpha(src);
			SDL_FreeSurface(src);
		}
	}
	else	
	{
		Tcolor cga[4]; 
		setCGAcolors(cga); 
		FILE *in;
		char tempfn[512];
		sprintf(tempfn, "%s/ultimaii.exe", conf->game[2].gamepath);
		if((in=fopen(tempfn, "rb"))!=NULL) {
			Uint32 POS;
			long file_length;
			fseek(in, 0, SEEK_END);   /* move file pointer to file end */
			file_length = ftell(in);
			POS=file_length-(((4*16+2)*64)+1312);
			fseek(in, POS, SEEK_SET);   /* move file pointer to tiles */
			SDL_Surface *temp=SDL_CreateRGBSurface(SDL_SWSURFACE, tiles->xdim, tiles->ydim, 32, rmask, gmask, bmask, amask);

			alltiles=SDL_CreateRGBSurface(SDL_SWSURFACE, tiles->xdim*500, tiles->ydim, 32, rmask, gmask, bmask, amask);
			int z=0;
			for(z=0;z<64;z++)
			{
				Uint16 X;
				POS+=2;
				fread(&X, sizeof(Uint16), 1, in);
				//fread(&Y, sizeof(Uint16), 1, in);
				
				for(int y=0;y<tiles->ydim;y++)
				{
					for(int x=0;x<tiles->xdim/4;x++)
					{
						unsigned char c;
						POS++;
						fread(&c, sizeof(char), 1, in);
						Uint8 color=c/16/4;
					//	DrawPixel(alltiles, x*4+z*tiles->xdim, y, cga[color].R, cga[color].G, cga[color].B);
					//	color=c/16%4;
					//	DrawPixel(alltiles, x*4+1+z*tiles->xdim, y, cga[color].R, cga[color].G, cga[color].B);
					//	color=c%16/4;
					//	DrawPixel(alltiles, x*4+2+z*tiles->xdim, y, cga[color].R, cga[color].G, cga[color].B);
					//	color=c%16%4;
					//	DrawPixel(alltiles, x*4+3+z*tiles->xdim, y, cga[color].R, cga[color].G, cga[color].B);
						DrawPixel(temp, x*4, y, cga[color].R, cga[color].G, cga[color].B);
						color=c/16%4;
						DrawPixel(temp, x*4+1, y, cga[color].R, cga[color].G, cga[color].B);
						color=c%16/4;
						DrawPixel(temp, x*4+2, y, cga[color].R, cga[color].G, cga[color].B);
						color=c%16%4;
						DrawPixel(temp, x*4+3, y, cga[color].R, cga[color].G, cga[color].B);
					}
		//			printf("\n");
				}
				tiles->tiles[z]=SDL_Scale_2xSai(temp);//zoomSurface(temp, (double)2, (double)2, 0);
//				printf("---------------- -- %d @ %d\n", z, tiles->tiles[z]);
//				z++;
//				if(z>=500) 
//					break;
			}   
			tiles->num=64;
			printf("%d\n", POS);
			fclose(in);
			double factor=2.0;
/*			for(int z=0;z<tiles->num;z++)
			{
				SDL_Surface *src=SDL_CreateRGBSurface(SDL_SWSURFACE, tiles->xdim, tiles->ydim, 32, rmask, gmask, bmask, amask);
				SDL_Rect destrect;
				destrect.x=0;destrect.y=0;destrect.w=tiles->xdim;destrect.h=tiles->ydim;
				SDL_Rect srcrect;
				srcrect.x=z*tiles->xdim;srcrect.y=0;srcrect.w=tiles->xdim;srcrect.h=tiles->ydim;
				if(SDL_BlitSurface(alltiles, &srcrect, src, &destrect))
					printf("\nError with blit.\n");
				tiles->tiles[z]=src;
				tiles->tiles[z]=zoomSurface(src, (double)factor, (double)factor, antialias);
				SDL_FreeSurface(src);
			}*/
			SDL_FreeSurface(temp);
			tiles->xdim=tiles->xdim*(int)factor;
			tiles->ydim=tiles->ydim*(int)factor;
		}
		else
		{   
			return 1;
		}
	}	
	SDL_FreeSurface(alltiles);
	return 0;	
}	


