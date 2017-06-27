#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_audio.h>
#include "../SDL_rotozoom.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "../SDL_smoothzoom.h"
#include "u1colors.h"
#include "u1types.h"
#include "u1data.h"
#include "u1main.h"
#include "u1gen.h"
#include "u1out.h"
#include "u1space.h"
#include "u1mondain.h"
#include "u1global.h"
#include "../2xsai.h"
//#include "pixmaps/u1border.xpm"
#include "pixmaps/u1font.xpm"

int U1_charChooser(SDL_Surface *screen, TU1_game *game)
{
	TU1_user user[4];
	SDL_Surface *buffer;
	SDL_FillRect(screen, NULL, 0);
	SDL_Surface *origin=U1_loadOriginPic(&game->config);
	SDL_Rect oR;
	buffer=SDL_Scale_2xSai(origin);
	SDL_FreeSurface(origin);
	origin=buffer;
	oR.x=320-origin->w/2;
	oR.y=40;oR.h=origin->h;oR.w=origin->w;
	if(SDL_BlitSurface(origin, NULL, screen, &oR))
		printf("error with origin pic blit.\n");
	SDL_FreeSurface(origin);
	Tfont font(u1font_xpm);
	output(screen, &font, 1, 17, "ULTIMA and LORD BRITISH are registered", 255, 255, 255);
	output(screen, &font, 2, 18, "trademarks of Richard Garriott and", 255, 255, 255);
	output(screen, &font, 11, 19, "Origin Systems, Inc.", 255, 255, 255);
	output(screen, &font, 4, 23, "DOS conversion by: John Fachini", 255, 255, 255);
	output(screen, &font, 6, 24, "SDL rewrite by: Greg Maculo", 255, 255, 255);
	refresh(screen, game->screen);
	int c=waitKey(screen, 4000);
	if(c==SDL_QUIT)
		return 0;
	int go=1;
	while(go)
	{
		go=0;
		SDL_FillRect(screen, NULL, 0);
		U1_addBorder(screen);
		Tterminal term(screen, (font.chr[0]->w*4), (font.chr[0]->h*6), 33, 13, 255, 255, 255, u1font_xpm);
		term.SCREEN=game->screen;
		term.print("        *** Ultima I ***\n");
			term.print("\n");
		term.print("\n");
		term.print("          from darkest\n");
		term.print("          dungeons, to\n");
		term.print("         deepest space!\n");
		term.print("\n");
		term.print("\n");
	
		char temp[40];
		sprintf(temp, "%c%c%c%c    a) Generate new character\n", 3, 92, 255, 255);
		term.print(temp);
		sprintf(temp, "%c%c%c%c    b) Continue previous game\n", 3, 92, 255, 255);
		term.print(temp);
		term.print("\n");
		term.print("\n");
		sprintf(temp, "%c%c%c%c         Thy choice: ", 3, 92, 255, 255);
		term.print(temp);
		term.blit();
		Uint16 c=0;
		refresh(screen, game->screen);
		while((c!=SDLK_b)&&(c!=SDLK_a))
		{
			
			c=term.waitKey();
			if(c==SDL_QUIT)
				return 0;
		}
		if(c==SDLK_a)
		{
				for(int x=1;x<=4;x++)
				{
					user[x-1].name[0]=0;
					U1_loaduser(&game->config, &user[x-1], x);
				}
		
				go=1;
				int full=1;
				int number=-1;
				for(int x=0;x<4;x++)
				{
					if(user[x].name[0]==0)
					{
						number=x;
						break;
					}
					full=full&&user[x].name[0];
				}
				if(full)
				{
					term.clear();
					for(int	x=1;x<=4;x++)
					{
						sprintf(temp, "           %d) %s\n", x, user[x-1].name);
						term.print(temp);
					}
					for(int	x=1;x<=4;x++)
					{
						term.print("\n");
					}
					sprintf(temp, "%c%c%c%cThe roster is full.  Type the\n", 3, 92, 255, 255);
					term.print(temp);
					sprintf(temp, "%c%c%c%cnumber of the character to wish\n", 3, 92, 255, 255);
					term.print(temp);
					sprintf(temp, "%c%c%c%cto replace with a new one.\n", 3, 92, 255, 255);
					term.print(temp);
					sprintf(temp, "%c%c%c%cPress ESCAPE to return to the\n", 3, 92, 255, 255);
					term.print(temp);
					sprintf(temp, "%c%c%c%cmain menu...", 3, 92, 255, 255);
					term.print(temp);
					term.blit();
					refresh(screen, game->screen);
					while(number==-1)
					{
						c=term.waitKey();
						if(c==SDL_QUIT)
							return 0;
						if(c==SDLK_ESCAPE)
						{
							go=1;
							break;
						}
						if((c<='4')&&(c>='1'))
							if(user[c-'0'-1].name[0])
								number=c-'0'-1;
					}
				}
				if((number>-1)&&(number<4))
				{
					U1_setDefaults(&game->user);
					int z=U1_charGen(screen, &term, game);
					if(z==SDL_QUIT)
						return SDL_QUIT;
					if(z)
					{
						U1_saveuser(&game->config, &game->user, number+1);
					}
					else
						U1_loaduser(&game->config, &game->user, number+1);
				}
		}
		else
		{
			for(int x=1;x<=4;x++)
			{
				user[x-1].name[0]=0;
				U1_loaduser(&game->config, &user[x-1], x);
			}
			term.clear();
			for(int	x=1;x<=4;x++)
			{
				sprintf(temp, "           %d) %s\n", x, user[x-1].name);
				term.print(temp);
			}
			for(int	x=1;x<=4;x++)
			{
				term.print("\n");
			}
			sprintf(temp, "%c%c%c%cType number of character you wish\n", 3, 92, 255, 255);
			term.print(temp);
			sprintf(temp, "%c%c%c%cto use in Ultima I.  Press ESCAPE\n", 3, 92, 255, 255);
			term.print(temp);
			sprintf(temp, "%c%c%c%cto go to the main menu...", 3, 92, 255, 255);
			term.print(temp);
			term.blit();
			refresh(screen, game->screen);
			while(1)
			{				
				c=term.waitKey();
				if(c==SDL_QUIT)
					return 0;
				if(c==SDLK_ESCAPE)
				{
					go=1;
					break;
				}
				if((c<='4')&&(c>='1'))
					if(user[c-'0'-1].name[0])
						return c-'0';
			}
		}
	}
	return 0;	
}

int U1_main(SDL_Surface *screen, int audio_open)
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
	SDL_Surface *buffer;
	SDL_Surface *buffer2;
	TU1_game game;
	game.screen=screen;
	screen=SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 400, 32, rmask, gmask, bmask, amask*0);

	SDL_Rect destrect;
	{
		Tfont font(u1font_xpm);
		while(1)
		{
			SDL_FillRect(screen, NULL, 0);
			SDL_Surface *origin=U1_loadOriginPic(&game.config);
			SDL_Rect oR;
			buffer2=SDL_Scale_2xSai(origin);
			SDL_FreeSurface(origin);
			origin=buffer2;
			oR.x=320-origin->w/2;
			oR.y=40;oR.h=origin->h;oR.w=origin->w;
			if(SDL_BlitSurface(origin, NULL, screen, &oR))
				printf("error with origin pic blit.\n");
			SDL_FreeSurface(origin);
			output(screen, &font, 14, 13, "Presents...", 92, 255, 255);
			refresh(screen, game.screen);
			int c=waitKey(screen, 4000);
			if(c==SDL_QUIT)
				return 0;
			if(c)
				break;
			oR.x=font.chr[0]->w*14;oR.w=font.chr[0]->w*11;
			oR.y=font.chr[0]->h*13;oR.h=font.chr[0]->h;
			SDL_FillRect(screen, &oR, 0);
			output(screen, &font, 4, 12, "...a new release of the best", 92, 255, 255);
			output(screen, &font, 4, 13, "   selling personal computer", 92, 255, 255);
			output(screen, &font, 4, 14, "   role-playing adventure...", 92, 255, 255);
			refresh(screen, game.screen);
			c=waitKey(screen, 4000);
			if(c==SDL_QUIT)
				return 0;
			if(c)
				break;
			oR.x=font.chr[0]->w*4;oR.w=font.chr[0]->w*28;
			oR.y=font.chr[0]->h*12;oR.h=font.chr[0]->h*3;
			SDL_FillRect(screen, &oR, 0);
			output(screen, &font, 6, 12, "...Lord British's original", 92, 255, 255);
			output(screen, &font, 6, 13, "   fantasy masterpiece...", 92, 255, 255);
			refresh(screen, game.screen);
			c=waitKey(screen, 4000);
			if(c==SDL_QUIT)
				return 0;
			if(c)
				break;

			oR.x=font.chr[0]->w*6;oR.w=font.chr[0]->w*26;
			oR.y=font.chr[0]->h*12;oR.h=font.chr[0]->h*2;
			SDL_FillRect(screen, &oR, 0);
			SDL_Surface *castleimage=SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 200, 32, rmask, gmask, bmask, amask);
			U1_EGAImage(&game.config, castleimage, "castle.16",320,200);
			buffer=SDL_smooth_checker(castleimage);	//zoomSurface(castleimage, 2, 2, 1);
//			buffer=SDL_smooth_hline(castleimage);	
//			buffer2=zoomSurface(buffer, 2, 2, 0);
			buffer2=SDL_Scale_2xSai(buffer);
			SDL_FreeSurface(buffer);
			buffer=buffer2;
			destrect.x=0;destrect.y=0;destrect.w=640;destrect.h=400;
			if(SDL_BlitSurface(buffer, NULL, screen, NULL))
				printf("\nError with castle blit.\n");
			
			SDL_FreeSurface(castleimage);
			SDL_FreeSurface(buffer);
			refresh(screen, game.screen);
			c=waitKey(screen, 20000);
			if(c==SDL_QUIT)
				return 0;
			if(c)
				break;
		}
	}
	int y=U1_charChooser(screen, &game);
	if(y)
	{
		U1_loaduser(&game.config, &game.user, y);
		char u1border_xpm[512];
		sprintf(u1border_xpm, "%s/u1border.png", game.config.pixmaps);
		buffer=IMG_Load(u1border_xpm);
		if(SDL_BlitSurface(buffer, NULL, screen, NULL))
			printf("\nerror with border blit.\n");
		SDL_FreeSurface(buffer);
		Tterminal term(screen, 0, 336, 30, 4, 0, 255, 255, u1font_xpm);
		term.SCREEN=game.screen;
		term.print("\n");
		term.print("\n");
		term.print("\n");
		int z=U1_out(screen, &term, &game);
		if(z==0)
			U1_saveuser(&game.config, &game.user, y);
	}
	SDL_FreeSurface(screen);
	return 0;
}

