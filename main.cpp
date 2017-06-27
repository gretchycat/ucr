#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL_rotozoom.h"
#include "globaltypes.h"
#include "globalfunctions.h"
#include "u0/u0main.h"
#include "u1/u1main.h"
#include "u2/u2main.h"
#include "u3/u3main.h"
#include "u4/u4main.h"
#include "u5/u5main.h"
#include "u6/u6main.h"
#include "declarations.h"
#include "maininterface.h"
#include "pixmaps/icon.xpm"

#ifdef WIN32
#include <windows.h>
int APIENTRY WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) 
{
	return main(0, NULL);
}
#endif

int main(int argc, char **argv)
{
	TglobalConfig *config;
	config=new(TglobalConfig);
	int xres=config->xres;
	int yres=config->yres;
	delete(config);
	config=NULL;
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_VIDEO))
	{
		printf("Error Initializing SDL: %s\n", SDL_GetError());
		return 1;
	}
	SDL_Surface *screen;
	SDL_Surface *icon=IMG_ReadXPMFromArray(icon_xpm);
	SDL_WM_SetIcon(icon, NULL);
	SDL_FreeSurface(icon);
	SDL_WM_SetCaption("Ultima Classics Revisited", "UCR");
	screen = SDL_SetVideoMode(xres, yres, 0, WINDOWED);
	if ( screen == NULL ) 
	{
		fprintf(stderr, "Unable to set %dx%d video: %s\n", xres, yres, SDL_GetError());
		return 1;
	}
	int audio_open = 0;
	int audio_rate;
	Uint16 audio_format;
	int audio_channels; 
	int audio_buffers;

	/* Initialize variables */
	audio_rate = 44100;
	audio_format = AUDIO_S16;
	audio_channels = 2;
	audio_buffers = 1024;
	
	/* Initialize the SDL library */
	if ( SDL_Init(SDL_INIT_AUDIO) < 0 ) 
	{ 
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		return(255);
	}

//	atexit(CleanUp);
//	signal(SIGINT, IntHandler);
//	signal(SIGTERM, exit);


	/* Open the audio device */
//	int AUDIOSTATUS=-1;
	int AUDIOSTATUS=Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
	if (AUDIOSTATUS < 0) 
	{
		fprintf(stderr, "AUDIO ERROR!: %s\n", SDL_GetError());
	} 
	else 
	{
		Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
//		printf("Opened audio at %d Hz %d bit %s, %d bytes audio buffer\n", audio_rate,(audio_format&0xFF),(audio_channels > 1) ? "stereo" : "mono",audio_buffers );
		audio_open = 1;
	}
	int n=maininterface(screen, audio_open);
	SDL_ShowCursor(SDL_DISABLE);
	switch(n)
	{
		case 0: SDL_WM_SetCaption("UCR - Ultima 0: Akalabeth", "UCR");break;
		case 1: SDL_WM_SetCaption("UCR - Ultima 1: The First Age of Darkness", "UCR");break;
		case 2: SDL_WM_SetCaption("UCR - Ultima 2: Revenge of the Enchantress", "UCR");break;
		case 3: SDL_WM_SetCaption("UCR - Ultima 3: Exodus", "UCR");break;
		case 4: SDL_WM_SetCaption("UCR - Ultima 4: Quest of the Avatar", "UCR");break;
		case 5: SDL_WM_SetCaption("UCR - Ultima 5: Warriors of Destiny", "UCR");break;
		case 6: SDL_WM_SetCaption("UCR - Ultima 6: The False Prophet", "UCR");break;
		default: SDL_WM_SetCaption("Ultima Classics Revisited", "UCR");break;
	}
	switch(n)
	{
		case 0:U0_main(screen, audio_open);break;
		case 1:U1_main(screen, audio_open);break;
		case 2:U2_main(screen, audio_open);break;
		case 3:U3_main(screen, audio_open);break;
		case 4:U4_main(screen, audio_open);break;
		case 5:U5_main(screen, audio_open);break;
		case 6:U6_main(screen, audio_open);break;
	}
//	if ( audio_open ) 
//	{
//		Mix_CloseAudio();
//	 	audio_open = 0;
//		SDL_CloseAudio();
//	}
	SDL_ShowCursor(SDL_ENABLE);
	SDL_FreeSurface(screen);
	SDL_Quit();
	exit(0);
	return 0;
}

