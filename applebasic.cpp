#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include "SDL_prim.h"
#include "globaltypes.h"
#include "globalfunctions.h"

float DX=0.00;
float DY=0.00;

void PRINT(SDL_Surface *screen, Tfont *font, int x, int y, char *s, int r, int g, int b)
{
	SDL_Rect rr;
	rr.w=strlen(s)*font->chr[0]->w;
	rr.h=font->chr[0]->h;
	rr.x=font->chr[0]->w*x;
	rr.y=font->chr[0]->h*y;
	SDL_FillRect(screen, &rr, SDL_MapRGBA(screen->format, 0, 0, 0, 255));	
	output(screen, font, x, y, s, r, g, b);
}

float atn(float n)
{
	return atan(n)*360/(2*3.1415926535);
}

float rnd(float n)
{
	return rand()%100/100.0*n;
}


void hplot(SDL_Surface *screen, double x1, double y1, double x2, double y2, Uint32 color)
{
	float mx=screen->w/280.0;
	float my=screen->h/192.0;
	SDL_drawLine_AA(screen, (int)(x1*mx), (int)(y1*my), (int)(x2*mx), (int)(y2*my), color);
}

void HPLOT(SDL_Surface *screen, double x1, double y1)
{
	float mx=screen->w/280.0;
	float my=screen->h/192.0;
	DX=x1*mx;
	DY=y1*my;
}

void TO(SDL_Surface *screen, double x2, double y2, Uint32 color)
{
	float mx=screen->w/280.0;
	float my=screen->h/192.0;
	SDL_drawLine_AA(screen, (int)DX, (int)DY, (int)(x2*mx), (int)(y2*my), color);
	DX=x2*mx;
	DY=y2*my;
}

int sgn(int n)
{
	if(n<0)
		return -1;
	if(n>0)
		return 1;
	return 0;
}

