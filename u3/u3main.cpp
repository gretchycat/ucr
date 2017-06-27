#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_audio.h>
#include "../SDL_rotozoom.h"
#include "../SDL_smoothzoom.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "u3types.h"
#include "u3data.h"
#include "../2xsai.h"

int U3_main(SDL_Surface *screen, int audio_open)
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

	SDL_Surface *dragonimage=SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 200, 32, rmask, gmask, bmask, amask);
	TglobalConfig config;
	U3_CGAImage(&config, dragonimage, "exod.ibm", 320, 200);
CGAtoComposite(dragonimage, 0);
	SDL_Surface *buffer=SDL_Scale_2xSai(dragonimage);//zoomSurface(dragonimage, 2, 2, 1);
	SDL_Rect destrect;
	destrect.x=0;destrect.y=0;destrect.w=640;destrect.h=400;
	if(SDL_BlitSurface(buffer, NULL, screen, &destrect))
		printf("\nError with blit.\n");
	SDL_FreeSurface(dragonimage);
	SDL_FreeSurface(buffer);
	SDL_UpdateRect(screen, 0, 0, 640, 400);
	SDL_Delay(20000);
	return 0;
}
