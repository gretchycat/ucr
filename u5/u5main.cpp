#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_audio.h>
#include "../SDL_rotozoom.h"
#include "../SDL_smoothzoom.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "u5types.h"
#include "u5data.h"
#include "../2xsai.h"

int U5_main(SDL_Surface *screen, int audio_open)
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
	TglobalConfig config;
	SDL_FillRect(screen, NULL, 0);
	SDL_Surface *ultima=U5_EGAImage(&config, "ultima.16", 0);
	SDL_Surface *temp=SDL_smooth_checker(ultima);
	SDL_FreeSurface(ultima);
	ultima=temp;
	SDL_Surface *animation[4];
	SDL_Surface *buffer=SDL_Scale_2xSai(ultima);
	SDL_Rect destrect;
	destrect.x=0;destrect.y=0;destrect.w=640;destrect.h=400;
	if(SDL_BlitSurface(buffer, NULL, screen, &destrect))
		printf("\nError with blit.\n");
	for(int x=0;x<4;x++)
	{
		SDL_Surface *temp=U5_EGAImage(&config, "ultima.16", x+1);
		SDL_Surface *temp2=SDL_smooth_checker(temp);
		animation[x]=SDL_Scale_2xSai(temp2);
		SDL_FreeSurface(temp2);
		SDL_FreeSurface(temp);
	}
	SDL_Rect tempR;tempR.x=320-(animation[0]->w/2);tempR.y=buffer->h;tempR.w=animation[0]->w;tempR.h=animation[0]->h;
	while(getKey(screen)!='q')
	{
		for(int z=0;z<4;z++)
		{
			if(SDL_BlitSurface(animation[z], NULL, screen, &tempR))
				printf("blit error\n");
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			SDL_Delay(100);
		}
	}
	SDL_FreeSurface(ultima);
	SDL_FreeSurface(buffer);
	for(int x=0;x<4;x++)
		SDL_FreeSurface(animation[x]);
//	SDL_UpdateRect(screen, 0, 0, 640, 400);
//	SDL_Delay(15000);
	return 0;
}
