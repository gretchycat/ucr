#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_audio.h>
#include "../SDL_rotozoom.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "../SDL_smoothzoom.h"
#include "u6types.h"
#include "u6data.h"
#include "../2xsai.h"

int U6_main(SDL_Surface *screen, int audio_open)
{
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
	TglobalConfig conf;
	SDL_Surface *image=SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 200, 32, rmask, gmask, bmask, amask);
	SDL_Rect destrect;
	destrect.x=0;destrect.y=0;destrect.w=640;destrect.h=400;
	U6_VGAImage(&conf, image, "worldmap.bmp");
	SDL_Surface *buffer=SDL_Scale_2xSai(image);
	if(SDL_BlitSurface(buffer, NULL, screen, &destrect))
		printf("\nError with blit.\n");
//	SDL_FreeSurface(image);
	SDL_FreeSurface(buffer);
	SDL_UpdateRect(screen, 0, 0, 640, 400);
	SDL_UpdateRect(screen, 0, 0, 640, 400);
	SDL_Delay(2000);
//	U6_VGAImage(image, "games/u6/titles.tga");
//	buffer=SDL_Scale_2xSai(image);
//	if(SDL_BlitSurface(buffer, NULL, screen, &destrect))
//		printf("\nError with blit.\n");
//	SDL_FreeSurface(image);
//	SDL_FreeSurface(buffer);
//	SDL_UpdateRect(screen, 0, 0, 640, 400);
//	SDL_UpdateRect(screen, 0, 0, 640, 400);
//	SDL_Delay(2000);
	U6_VGAImage(&conf, image, "paper.bmp");
	buffer=SDL_Scale_2xSai(image);
	if(SDL_BlitSurface(buffer, NULL, screen, &destrect))
		printf("\nError with blit.\n");
//	SDL_FreeSurface(image);
	SDL_FreeSurface(buffer);
	SDL_UpdateRect(screen, 0, 0, 640, 400);
	SDL_UpdateRect(screen, 0, 0, 640, 400);
	SDL_Delay(2000);
	U6_VGAImage(&conf, image, "newmagic.bmp");
	buffer=SDL_Scale_2xSai(image);
	if(SDL_BlitSurface(buffer, NULL, screen, &destrect))
		printf("\nError with blit.\n");
	SDL_FreeSurface(image);
	SDL_FreeSurface(buffer);
	SDL_UpdateRect(screen, 0, 0, 640, 400);
	SDL_UpdateRect(screen, 0, 0, 640, 400);
	SDL_Delay(2000);
	return 0;
}
