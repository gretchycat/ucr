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

int U1_getShrineTextStart(int z)
{
	switch(z)
	{
		case 0: return 0;
		case 1: return 2;
		case 2: return 4;
		case 3: return 6;
		case 4: return 14;
		case 5: return 18;
		case 6: return 19;
		case 7: return 20;
	}
	return 22;
}

int U1_navigateSignpost(SDL_Surface *screen, Tterminal *term, TU1_game *game)
{	
	Mix_Music *music = NULL;
        char tempfn[512];
	sprintf(tempfn, "%s/u1/midi/shrines.mid", game->config.datapath);
                
	if(game->config.game[1].music)
		music = Mix_LoadMUS(tempfn);
	int looping=-1;
	if(music)
		Mix_FadeInMusic(music,looping,500);
	term->print("Entering...\n");
//	refresh(screen, game->screen);	
	char txt[12][31];
	for(int x=0;x<11;x++)
		txt[x][0]=0;
	int sn=U1_shrinename(game->user.x, game->user.y);
	int s1=U1_getShrineTextStart(sn);
	int s2=U1_getShrineTextStart(sn+1);
	int z=s1;
	for(int x=(11/2)-((s2-s1)/2);x<(11/2)-((s2-s1)/2)+(s2-s1);x++)
	{
		strcpy(txt[x], game->out.S_shrinetexts[z]);
		z++;
	}
	char temp[85];
	sprintf(temp, " %s\n", game->out.S_shrines[sn]);
	term->print(temp);
	if(sn%2==1)
	{
		if(game->user.quest[7-(sn)]==-1)
		{
			game->user.quest[7-(sn)]=1;
			term->print(" A quest has been completed!\n");
		}
		
	}
	if(sn==game->user.lastsignpost)
	{
		if(sn!=6)
		{
			char temp2[85];
			sprintf(temp2, " %s\n", game->out.S_shrinetexts[25]);
			term->print(temp2);
		}
	}
	else
	{
		int adder=0;
		char temp2[512];
		switch(sn)
		{
			case 0:	if(game->user.agility<99)
				{
					adder=(game->user.agility+8)/10;
					game->user.agility+=adder;
					sprintf(temp2, "%s%d %s\n", game->out.S_shrinetexts[24], adder, "agility.");
					if(game->user.agility>99)
						game->user.agility=99;			
				}
				else
				{
					sprintf(temp2, " %s\n", game->out.S_shrinetexts[25]);
				}
				break;
			case 1:	if(game->user.intelligence<99) 
				{
					adder=(game->user.intelligence+8)/10;
					game->user.intelligence+=adder;
					sprintf(temp2, "%s%d %s\n", game->out.S_shrinetexts[24], adder, "intelligence."); 
					if(game->user.intelligence>99)
						game->user.intelligence=99;			
				}
				else
				{
					sprintf(temp2, " %s\n", game->out.S_shrinetexts[25]);
				}
				break;
			case 2:	adder=0;
				while((game->user.weapon[adder]!=0)&&(adder<16))
					adder++;
				if(adder<16)
				{
					game->user.weapon[adder]++;
					sprintf(temp2, "%s %s\n", game->out.S_shrinetexts[23], game->out.S_weapons[adder]);
				}
				else
				{
					sprintf(temp2, " %s\n", game->out.S_shrinetexts[25]);
				}
				break;
			case 3:	if(game->user.wisdom<99) 
				{
					adder=(game->user.wisdom+8)/10;
					game->user.wisdom+=adder;
					sprintf(temp2, "%s%d %s\n", game->out.S_shrinetexts[24], adder, "wisdom.");
					if(game->user.wisdom>99)
						game->user.wisdom=99;			
				}
				else
				{
					sprintf(temp2, " %s\n", game->out.S_shrinetexts[25]);
				}
				break;
			case 4:	if(game->user.stamina<99) 
				{
					adder=(game->user.stamina+8)/10;
					game->user.stamina+=adder;
					sprintf(temp2, "%s%d %s\n", game->out.S_shrinetexts[24], adder, "stamina.");
					if(game->user.stamina>99)
						game->user.stamina=99;			
				}
				else
				{
					sprintf(temp2, " %s\n", game->out.S_shrinetexts[25]);
				}
				break;
			case 5:	if(game->user.charisma<99) 
				{
					adder=(game->user.charisma+8)/10;
					game->user.charisma+=adder;
					sprintf(temp2, "%s%d %s\n", game->out.S_shrinetexts[24], adder, "charisma.");
					if(game->user.charisma>99)
						game->user.charisma=99;			
				}
				else
				{
					sprintf(temp2, " %s\n", game->out.S_shrinetexts[25]);
				}
				break;
			case 6:	break;	//NOTHING
			case 7:	if(game->user.stamina<99) 
				{
					adder=(game->user.stamina+8)/10;
					game->user.stamina+=adder;
					sprintf(temp2, "%s%d %s\n", game->out.S_shrinetexts[24], adder, "stamina.");
					if(game->user.stamina>99)
						game->user.stamina=99;			
				}
				else
				{
					sprintf(temp2, " %s\n", game->out.S_shrinetexts[25]);
				}
				break;
		}
		term->print(temp2);
		game->user.lastsignpost=sn;
	}
	U1_msgBox(screen, term, &game->user, txt, 0);
	if ( music )
	{
		Mix_FadeOutMusic(500);
		Mix_FreeMusic(music);
		music = NULL;
	}
		 		    
	return 0;
}

