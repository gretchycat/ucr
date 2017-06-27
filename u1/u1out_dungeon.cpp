#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../2xsai.h"
#include "../globaltypes.h"
#include "u1types.h"
#include "u1data.h"
#include "../globalfunctions.h"
#include "../declarations.h"
#include "u1global.h"
#include "u1placenames.h"
#include "u1dungeon.h"

int U1_dungeonDirAndFloor(SDL_Surface *screen, Tterminal *term, TU1_game *game)
{
	Tcolor ega[16];
	setEGAcolors(ega);
	SDL_Rect r;
	r.x=16*term->font->chr[0]->w;
	r.y=19*term->font->chr[0]->h;
	r.w=9*term->font->chr[0]->w;
	r.h=1*term->font->chr[0]->h;
	SDL_FillRect(screen, &r, SDL_MapRGBA(screen->format, 0, 0, 0, 255));
	char tempstr[40];
	sprintf(tempstr, "> %c%c%c%c%s%c%c%c%c <", 3, ega[11].R, ega[11].G, ega[11].B, "South", 3, ega[15].R, ega[15].G, ega[15].B);
	output(screen, term->font,16, 19, tempstr, ega[15].R, ega[15].G, ega[15].B);
	r.x=15*term->font->chr[0]->w;
	r.y=0*term->font->chr[0]->h;
	r.w=12*term->font->chr[0]->w;
	r.h=1*term->font->chr[0]->h;
	SDL_FillRect(screen, &r, SDL_MapRGBA(screen->format, 0, 0, 0, 255));
	sprintf(tempstr, "> %c%c%c%cLevel %2d%c%c%c%c <", 3, ega[11].R, ega[11].G, ega[11].B, game->dungeon.inout, 3, ega[15].R, ega[15].G, ega[15].B);
	output(screen, term->font,15, 0, tempstr, ega[15].R, ega[15].G, ega[15].B);
	return 0;
}

int U1_navigateDungeon(SDL_Surface *screen, Tterminal *term, TU1_game *game)
{
	term->print("Entering...\n");
	char tmp[40];
	sprintf(tmp, " %s\n", game->out.S_dungeons[U1_dungeonname(game->user.x, game->user.y)]);
	term->print(tmp);
	Mix_Music *music = NULL;
	char songpath[512];
	sprintf(songpath, "%s/u1/midi/dungeon.mid", game->config.datapath);
	if(game->config.game[1].music)
		music = Mix_LoadMUS(songpath);
	int looping=-1;
	if(music)
		Mix_FadeInMusic(music,looping,500);
//	U1_initPlayer(screen, game);
game->dungeon.inout=1;
//	U1_makeDungeon(game);
//	U1_dungeonMainLoop(screen, game, term);
	int go=1;
	while(go)
	{
		U1_dungeonDirAndFloor(screen, term, game);
		U1_printWithTick(term, "");
		int i=term->waitKey();
		switch(i)
		{
				case SDL_QUIT:	
						if( Mix_PlayingMusic() )
							Mix_FadeOutMusic(500);
						if ( music ) 
						{
							Mix_FreeMusic(music);
							music = NULL;
						}
						return -1;break;
				case SDLK_LEFT:	
						break;
				case SDLK_RIGHT:
						break;
				case SDLK_UP:	
						break;
				case SDLK_DOWN:	
						break;
				case SDLK_SPACE:
						term->print("Pass\n");						
						break;
				case SDLK_a:
						break;
				case SDLK_b:
						{
							char temp[40];
							sprintf(temp, "%s\n", game->out.S_board[0]);
							term->print(temp);
						}
						break;
				case SDLK_c:
						{
							char temp[40];
							sprintf(temp, "%s%s\n", game->out.S_spell[13], game->out.S_spell[10]);
							term->print(temp);
						}
						break;
				case SDLK_d:	{
//						U1_droptown(screen, term, game, towns, towntype, npc, xx, yy);
						break;
						}
				case SDLK_e:
						{
							char temp[40];
							sprintf(temp, "%s?\n", game->out.S_enter[0]);
							term->print(temp);
						}
						break;
				case SDLK_f:
						{
							char temp[40];
							sprintf(temp, "%s\n", game->out.S_fire[0]);
							term->print(temp);
						}
						break;
				case SDLK_g:
						{
							char temp[40];
							sprintf(temp, "Get\n");
							term->print(temp);
						}
						break;
				case SDLK_h:
						{
							char temp[40];
						//	for(int x=0;x<5;x++)
						//	{
								sprintf(temp, "%s\n", game->out.S_hyperjump);
						//		printf("%d:	%s", x, temp);
						//	}
							term->print(temp);
						}
						break;
				case SDLK_i:
						{
//							char temp[40];
	//						sprintf(temp, "%s\n", game->out.S_inform[2]);
		//					term->print(temp);
			//				term->print(townname);
						}
						break;
			case SDLK_k:
						if(1)	//upladder
						{
							game->dungeon.inout--;
							char t[40];
							sprintf(t, "K-Limb up to level %d\n", game->dungeon.inout);
							term->print(t);
						} 
						else
						if(1)	//downladder
						{
							game->dungeon.inout++;
							char t[40];
							sprintf(t, "K-Limb down to level %d\n", game->dungeon.inout);
							term->print(t);
						} 
						if(game->dungeon.inout==0)
						{
							go=0;
						}
					break;
				case SDLK_o:	//open
							break;
				case SDLK_p:	term->print("Peer - only allowed outdoors!\n");
						break;
				case SDLK_q:	term->print("Quit - only allowed outdoors!\n");
						break;
									
				case SDLK_r:	//ready
						U1_ready(screen, term, game);
						break;
				case SDLK_s:
					break;
				case SDLK_t:	
						break;
				case SDLK_u:	
						{
//							if(U1_unlocktown(screen, term, game, towns, towntype, npc, xx, yy))
//								updated=0;
						}
						break;
				case SDLK_v:	//view
							{
								char temp[40];
								sprintf(temp, "%s\n", game->out.S_view);
								term->print(temp);
							}
							break;
				case SDLK_x:	//x-it
							{
								char temp[40];
								sprintf(temp, "%s\n", game->out.S_xit[0]);
								term->print(temp);
							}
							break;
				case SDLK_z:	go=1;
						term->print("Ztats\n");
						if(U1_ztats(screen, term, game)==SDL_QUIT)
						{
							if( Mix_PlayingMusic() )
							{
								Mix_FadeOutMusic(500);
							//	SDL_Delay(150);
							}
							if ( music ) 
							{
								Mix_FreeMusic(music);
								music = NULL;
							}
							return -1;
						}
						break; 


				case SDLK_F11:	if(game->config.game[1].svga)
						{
//							updated=0;
							game->config.game[1].svga=0;
							term->print("OLD GRAPHICS\n");
						}
						else
						{
	//						updated=0;
							game->config.game[1].svga=1;
							term->print("NEW GRAPHICS\n");
						}
						break;
				case SDLK_n:
				case SDLK_F12:	if(game->config.game[1].music)
						{
							if ( music )
							{
								Mix_FadeOutMusic(0);
								Mix_FreeMusic(music);
								music = NULL;
							}
							game->config.game[1].music=0;
							char temp[256];
							sprintf(temp, "%s%s\n", game->out.S_noise[0], game->out.S_noise[2]);
							term->print(temp);
						}
						else
						{						
							game->config.game[1].music=1;
							music = Mix_LoadMUS(songpath);	
							Mix_FadeInMusic(music,looping,500);
							char temp[256];
							sprintf(temp, "%s%s\n", game->out.S_noise[0], game->out.S_noise[1]);
							term->print(temp);
						}
						break;
			default: term->print("Huh?\n");
					break;					
		}
	}
	if ( music )
	{
		Mix_FadeOutMusic(500);
		Mix_FreeMusic(music);
		music = NULL;
	}
	char u1border_xpm[512];
	sprintf(u1border_xpm, "%s/u1border.xpm", game->config.pixmaps);
	SDL_Surface *buffer=IMG_Load(u1border_xpm);
	if(SDL_BlitSurface(buffer, NULL, screen, NULL))
		printf("\nerror with border blit.\n"); 
	SDL_FreeSurface(buffer);		 		    
	//SDL_UpdateRect(screen, 0, 0, 640, 400);
	refresh(screen, game->screen);
	return 0;
}

