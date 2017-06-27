#include <SDL.h>
#include <stdlib.h>
#include "globaltypes.h"
#include "globalfunctions.h"


static inline Uint32 SDL_getPixel(SDL_Surface *surface, int x, int y)
{
	if((x<0)||(y>=surface->h)||(y<0)||(x>=surface->w))
		return 0;
	Uint32 *n=(Uint32*)surface->pixels;
	return n[y*surface->w+x];
}

SDL_Surface *SDL_smooth(SDL_Surface *surface)
{
	if(surface==NULL)
		return NULL;
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
	
//	printf("zooming to %dx%d\n", surface->w*2, surface->h*2);
	SDL_Surface *temp=SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, 32, rmask, gmask, bmask, amask);
	for(int y=0;y<surface->h;y++)
	{
		for(int x=0;x<surface->w;x++)
		{
			int checkerboard=((SDL_getPixel(surface, x,y)==SDL_getPixel(surface, x+1,y+1))&&(SDL_getPixel(surface, x,y+1)==SDL_getPixel(surface, x+1,y)));
			int vlines=((SDL_getPixel(surface, x,y)==SDL_getPixel(surface, x,y+1))&&(SDL_getPixel(surface, x+1,y)==SDL_getPixel(surface, x+1,y+1)));
			Uint32 c;
			if(vlines||checkerboard)
			{
				Uint8 r1, r2, g1, g2, b1, b2, a1, a2;
				Uint32 c1=SDL_getPixel(surface, x,y);
				Uint32 c2=SDL_getPixel(surface, x+1,y);
				SDL_GetRGBA(c1, surface->format, &r1, &g1, &b1, &a1);
				SDL_GetRGBA(c2, surface->format, &r2, &g2, &b2, &a2);
				c=SDL_MapRGBA(surface->format, (r1+r2)/2, (g1+g2)/2, (b1+b2)/2, (a1+a2)/2);
			}
			else
			{
				c=SDL_getPixel(surface, x,y);
			}
			Uint8 r, g, b, a;
			SDL_GetRGBA(c, surface->format, &r, &g, &b, &a);
			DrawPixel(temp, x, y, r, g, b, a);
		}
	}
	SDL_UpdateRect(temp, 0,0,0,0);
	return temp;
}

int colordif(SDL_Surface *surface, int x, int y, int x2, int y2)
{
				Uint8 r, g, b, a;
				Uint32 c=SDL_getPixel(surface, x,y);
				SDL_GetRGBA(c, surface->format, &r, &g, &b, &a);
				Uint8 r2, g2, b2, a2;
				Uint32 c2=SDL_getPixel(surface, x2,y2);
				SDL_GetRGBA(c2, surface->format, &r2, &g2, &b2, &a2);
				return (abs(r2-r)+abs(g2-g)+abs(b2-b))/3;
}

SDL_Surface *SDL_smooth_checker(SDL_Surface *surface)
{
	if(surface==NULL)
		return NULL;
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
	
	SDL_Surface *temp=SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, 32, rmask, gmask, bmask, amask);
	int lasttime=0;
	for(int y=0;y<surface->h;y++)
	{
		for(int x=0;x<surface->w;x++)
		{
			int checkerboard=0;
			int dx=0;
			int dy=0;
			for(dx=-1;dx<=0;dx++)
				for(dy=-1;dy<=0;dy++)
			checkerboard=checkerboard||(((SDL_getPixel(surface, x+dx,y+dy)==SDL_getPixel(surface, x+1+dx,y+1+dy))&&(SDL_getPixel(surface, x+dx,y+1+dy)==SDL_getPixel(surface, x+1+dx,y+dy))));
			Uint32 c;

			if((lasttime||checkerboard)&&colordif(surface, x, y, x+1, y)<104)	//&&(greenorblue(surface, x,y)&&greenorblue(surface, x+1,y))
			{
				Uint8 r1, r2, g1, g2, b1, b2, a1, a2;
				Uint8 r3, r4, g3, g4, b3, b4, a3, a4;
				Uint32 c1=SDL_getPixel(surface, x,y);
				Uint32 c2=SDL_getPixel(surface, x+1,y);
				Uint32 c3=SDL_getPixel(surface, x,y+1);
				Uint32 c4=SDL_getPixel(surface, x+1,y+1);
				SDL_GetRGBA(c1, surface->format, &r1, &g1, &b1, &a1);
				SDL_GetRGBA(c2, surface->format, &r2, &g2, &b2, &a2);
				SDL_GetRGBA(c3, surface->format, &r3, &g3, &b3, &a3);
				SDL_GetRGBA(c4, surface->format, &r4, &g4, &b4, &a4);
				c=SDL_MapRGBA(surface->format, (r1+r2+r3+r4)/4, (g1+g2+g3+g4)/4, (b1+b2+b3+b4)/4, (a1+a2+a3+a4)/4);
				lasttime=0;
			}
			else
			{
				lasttime=0;
				c=SDL_getPixel(surface, x,y);
			}
		//	if(checkerboard)
		//		lasttime=1;
			Uint8 r, g, b, a;
			SDL_GetRGBA(c, surface->format, &r, &g, &b, &a);
			DrawPixel(temp, x, y, r, g, b, a);
		}
	}
	SDL_UpdateRect(temp, 0,0,0,0);
	return temp;
}

