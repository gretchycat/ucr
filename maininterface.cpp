#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include "SDL_rotozoom.h"
#include "globaltypes.h"
#include "globalfunctions.h"
#include "SDL_smoothzoom.h"
#include "u1/u1types.h"
#include "u2/u2types.h"
#include "u3/u3types.h"
#include "u4/u4types.h"
#include "u5/u5types.h"
#include "u6/u6types.h"
#include "u1/u1data.h"
#include "u2/u2data.h"
#include "u3/u3data.h"
#include "u4/u4data.h"
#include "u5/u5data.h"
#include "u6/u6data.h"
#include "declarations.h"
#include "2xsai.h"

int maininterface(SDL_Surface *screen, int audio_open)
{
	char interface_xpm[512];
	char u0title_xpm[512];
	TglobalConfig conf;
	sprintf(interface_xpm, "%s/interface.png", conf.pixmaps);
	sprintf(u0title_xpm, "%s/u0title.png", conf.pixmaps);
	int looping=-1;
	Mix_SetMusicCMD(getenv("MUSIC_CMD"));
	Mix_Music *music = NULL;	
	if(audio_open)
	{
		char temp[572];
		sprintf(temp, "%s/stones.mid", conf.datapath);
		music = Mix_LoadMUS(temp);
	}
	if(audio_open)
		Mix_FadeInMusic(music,looping,500);
	else
		printf("COULDNT OPEN!\n");
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff*0;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000*0;
	#endif
	SDL_Surface *background=IMG_Load(interface_xpm);
	if(!background)
	{
		printf("Can not load %s\n", interface_xpm);
		return 'q';
	}
	float xscl=(float)screen->w/(float)background->w;
	float yscl=(float)screen->h/(float)background->h;
	SDL_Surface *tempbg=zoomSurface(background, xscl, yscl, 1);
	SDL_FreeSurface(background);
	background=tempbg;
	tempbg=NULL;
	const int NUMICONS=7;
	const int YVAR=(int)(350.0*yscl);
	SDL_Surface *image[10];
	SDL_Surface *Uicons[10];
	for(int x=0;x<NUMICONS;x++)
		image[x]=SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 200, 32, rmask, gmask, bmask, amask);
	SDL_Surface *u0temp=IMG_Load(u0title_xpm);
	float scale=200.0/u0temp->h;
	SDL_Surface *u0temp2=zoomSurface(u0temp, scale, scale, 1);
	SDL_Rect u0rect;
	u0rect.x=160-u0temp2->w/2;
	u0rect.y=0;
	u0rect.w=u0temp2->w;
	u0rect.h=u0temp2->h;
	SDL_BlitSurface(u0temp2, NULL, image[0], &u0rect);
	SDL_FreeSurface(u0temp2);
	SDL_FreeSurface(u0temp);
	U1_EGAImage(&conf, image[1], "castle.16", 320, 200);
	U2_CGAImage(&conf, image[2], "picdra", 320, 200);
	CGAtoComposite(image[2], 0);
	U3_CGAImage(&conf, image[3], "exod.ibm", 320, 200);
	CGAtoComposite(image[3], 0);
	U4_EGAImage(&conf, image[4], "title.ega", 320, 200);
	SDL_Surface *temp=U5_EGAImage(&conf, "ultima.16", 0);
	SDL_Surface *temp2=U5_EGAImage(&conf, "ultima.16", 1);
	U6_VGAImage(&conf, image[6], "worldmap.bmp");
	if(temp && temp2)
	{
		SDL_FillRect(image[5], NULL, 0);
		SDL_Rect tempR;
		tempR.x=160-(temp2->w/2);
		tempR.y=temp->h;tempR.w=temp2->w;tempR.h=temp2->h;
		if(SDL_BlitSurface(temp, NULL, image[5], NULL))
			printf("bad temp blit\n");
		if(SDL_BlitSurface(temp2, NULL, image[5], &tempR))
			printf("bad temp2 blit\n");
		SDL_FreeSurface(temp);
		SDL_FreeSurface(temp2);
	}

	SDL_WM_SetCaption("Ultima Classics Revisited", "UCR");
	if(SDL_BlitSurface(background, NULL, screen, NULL))
		printf("\nError with blit.\n");
	for(int x=0;x<NUMICONS;x++)
	{
		Uicons[x]=zoomSurface(image[x], .5, .5, 1);
	}
	for(int x=0;x<NUMICONS;x++)
	{	
		SDL_Surface *temp1=SDL_smooth(image[x]);
		SDL_FreeSurface(image[x]);
		image[x]=zoomSurface(temp1, .5, .5, 1);
		SDL_FreeSurface(temp1);
	}
 	
	int go=1;
	float sintable[100];
	float max=0;
	for(int x=0;x<100;x++)
	{
		sintable[x]=((cos(float(x)*(3.1415926535/100.0)))+3)/4.0*xscl;
		if(sintable[x]>max)
			max=sintable[x];
	}
	int xx=99;int yy=99;
	int lindex=0;
	SDL_UpdateRect(screen, 0, 0, 0, 0);
	while(go)
	{
		SDL_Event event;
		SDL_Delay(1);	//keep it CPU friendly
		int k=0;
		for(int x=0;x<3;x++)
		{
			int kk=getKey(screen, &event);
			if(kk)
				k=kk;
			if(k==SDL_QUIT) //	keep from having an input buffer cache..
			{
				if( Mix_PlayingMusic() )
				{
					Mix_FadeOutMusic(500);
				}
				if ( audio_open ) 
				{
					Mix_FreeMusic(music);
					music = NULL;
				}			
				return SDL_QUIT;
			}
		}
		if(k==SDLK_q)
		{
			if( Mix_PlayingMusic() )
			{
				Mix_FadeOutMusic(500);
			}
			if ( audio_open ) 
			{
				Mix_FreeMusic(music);
				music = NULL;
			}
			return SDL_QUIT;
		}
		else
			if((k>='0')&&(k<='9'))
			{
				SDL_WM_SetIcon(image[k-'0'], NULL);
				for(int x=0;x<NUMICONS;x++)
					SDL_FreeSurface(image[x]);
				if( Mix_PlayingMusic() )
				{
					Mix_FadeOutMusic(500);
				}
				if ( audio_open ) 
				{
					Mix_FreeMusic(music);
					music = NULL;
				}
				return k-'0';
			}
		if(!((xx==(int)(event.motion.x))&&(yy==(int)(event.motion.y))))
		{		
			SDL_Rect destrect[NUMICONS];
			float scale[NUMICONS];
			SDL_Surface *buffer[NUMICONS];
			xx=(int)(event.motion.x);
			yy=(int)(event.motion.y);
			if ((xx<0)||(xx>screen->w))
				xx=0;
			if ((yy<0)||(yy>screen->h))
				yy=0;
			int delta[NUMICONS];
			int lowest=9999;
			lindex=0;
			
			for(int x=0;x<NUMICONS;x++)
			{
				int ix=(int)(screen->w/2-(image[x]->w*sintable[99]*(((float)NUMICONS/2.0)-(x+1)))-(image[x]->w*sintable[99])/2);
				int iy=YVAR;
				delta[x]=int(sqrt(float((ix-xx)*(ix-xx)+(iy-yy)*(iy-yy))));
				if(delta[x]<lowest)
				{
					lowest=delta[x];
					lindex=x;
				}
				int z=delta[x];
				if(z>99)
					z=99;
				scale[x]=sintable[z];	
			}
			int max=0;
			for(int x=0;x<NUMICONS;x++)
			{
				buffer[x]=zoomSurface(image[x], scale[x], scale[x], 1);
				int ix=screen->w/(NUMICONS+1);
				destrect[x].y=YVAR-buffer[x]->h/2;
				destrect[x].x=ix*x*0;
				for(int z=0;z<x;z++)
					destrect[x].x+=buffer[z]->w;
				max+=buffer[x]->w;
			}
			int D=(screen->w-max)/2;
			for(int z=0;z<NUMICONS;z++)
				destrect[z].x+=D;
			if(SDL_BlitSurface(background, NULL, screen, NULL))
				printf("\nError with blit.\n");
			for(int x=0;x<NUMICONS;x++)
			{
				if(SDL_BlitSurface(buffer[x], NULL, screen, &destrect[x]))
					printf("\nError with blit.\n");
			}
			for(int x=0;x<NUMICONS;x++)
				SDL_FreeSurface(buffer[x]);
			SDL_UpdateRect(screen, 0,0,0,0);
			//SDL_UpdateRect(screen, 0, (YVAR-50)*yscl, screen->w, 100*xscl);
		}
		if(event.button.state==SDL_PRESSED)
			if(yy>YVAR-50)
			{
				SDL_WM_SetIcon(image[lindex], NULL);
				for(int x=0;x<NUMICONS;x++)
					SDL_FreeSurface(image[x]);
				if( Mix_PlayingMusic() )
				{
					Mix_FadeOutMusic(500);
				}
				if ( audio_open ) 
				{
					Mix_FreeMusic(music);
					music = NULL;
				}					
				return lindex;
			}
//		refresh(screen, screen );

	}
	SDL_WM_SetIcon(image[lindex], NULL);
	for(int x=0;x<NUMICONS;x++)
		SDL_FreeSurface(image[x]);
	if( Mix_PlayingMusic() )
	{
		Mix_FadeOutMusic(500);
	//	SDL_Delay(150);
	}
	if ( audio_open ) 
	{
		Mix_FreeMusic(music);
		music = NULL;
	}
	return lindex;
}
