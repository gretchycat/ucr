#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "u1types.h"
#include "u1global.h"

int U1_charGen(SDL_Surface *screen, Tterminal *term, TU1_game *game)
{
	int pleft=30;
	game->user.strength=10;game->user.agility=10;game->user.stamina=10;
	game->user.charisma=10;game->user.wisdom=10;game->user.intelligence=10;
	int pos=0;
	int stepnum=0;
	int go=1;
	//MAKE SURE TO ADD BONUSES BASED ON CLASS RACE, SEX!
	term->clear();
	while(1)
	{
		go=1;
		SDL_Rect rr;
		rr.x=16;rr.w=screen->w-32;
		rr.y=16;rr.h=screen->h-32;
		char temp[50];
		SDL_FillRect(screen, &rr, 0);
		sprintf(temp, "Strength........%d\n", game->user.strength);
		output(screen, term->font, 12, 6, temp, 92, 255, 255);
		sprintf(temp, "Agility.........%d\n", game->user.agility);
		output(screen, term->font, 12, 7, temp, 92, 255, 255);
		sprintf(temp, "Stamina.........%d\n", game->user.stamina);
		output(screen, term->font, 12, 8, temp, 92, 255, 255);
		sprintf(temp, "Charisma........%d\n", game->user.charisma);
		output(screen, term->font, 12, 9, temp, 92, 255, 255);
		sprintf(temp, "Wisdom..........%d\n", game->user.wisdom);
		output(screen, term->font, 12, 10, temp, 92, 255, 255);
		sprintf(temp, "Intelligence....%d\n", game->user.intelligence);
		output(screen, term->font, 12, 11, temp, 92, 255, 255);
		if((stepnum==0)&&(go))
		{
			sprintf(temp, "Points left to distribute: %d\n", pleft);
			output(screen, term->font, 6, 4, temp, 92, 255, 255);
			output(screen, term->font, 2, 16, "Move cursor with up and down arrows;", 92, 255, 255); 
			output(screen, term->font, 2, 17, "increase and decrease attributes", 92, 255, 255); 
			output(screen, term->font, 2, 18, "with left and right arrows.  Press", 92, 255, 255); 
			output(screen, term->font, 2, 19, "space bar when finished, or escape", 92, 255, 255); 
			output(screen, term->font, 2, 20, "to return to the main menu.", 92, 255, 255); 
			char marker[2];
			marker[0]='>';marker[1]=0;
			output(screen, term->font, 11, pos+6, marker, 255, 255, 255);
			marker[0]='<';
			output(screen, term->font, 30, pos+6, marker, 255, 255, 255);
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			refresh(screen, game->screen);
			Uint16 c=waitKey(screen);
			if(c==SDLK_SPACE)
			{
				if(pleft==0)
				{
					stepnum=1;
					go=0;
				//	statgo=0;
				}
			}
			if(c==SDLK_ESCAPE)
				return 0;
			if(c==SDL_QUIT)
				return SDL_QUIT;
			if(c==SDLK_UP)
			{
				pos--;
				if(pos<0)
					pos=5;
			}
			if(c==SDLK_DOWN)
			{
				pos++;
				if(pos>5)
					pos=0;
			}
			if(c==SDLK_LEFT)
			{
				switch(pos)
				{
					case 0: if(game->user.strength>10)
						{
							pleft++;
							game->user.strength--;
						}
						break;
					case 1: if(game->user.agility>10)
						{
							pleft++;
							game->user.agility--;
						}
						break;
					case 2: if(game->user.stamina>10)
						{
							pleft++;
							game->user.stamina--;
						}
						break;
					case 3: if(game->user.charisma>10)
						{
							pleft++;
							game->user.charisma--;
						}
						break;
					case 4: if(game->user.wisdom>10)
						{
							pleft++;
							game->user.wisdom--;
						}
						break;
					case 5: if(game->user.intelligence>10)
						{
							pleft++;
							game->user.intelligence--;
						}
						break;
				}
			}
			if(c==SDLK_RIGHT)
			{
				if(pleft>0)
				{
					switch(pos)
					{
						case 0: if(game->user.strength<25)
							{
								pleft--;
								game->user.strength++;
							}
							break;
						case 1: if(game->user.agility<25)
							{
								pleft--;
								game->user.agility++;
							}
							break;
						case 2: if(game->user.stamina<25)
							{
								pleft--;
								game->user.stamina++;
							}
							break;
						case 3: if(game->user.charisma<25)
							{
								pleft--;
								game->user.charisma++;
							}
							break;
						case 4: if(game->user.wisdom<25)
							{
								pleft--;
								game->user.wisdom++;
							}
							break;
						case 5: if(game->user.intelligence<25)
							{
								pleft--;
								game->user.intelligence++;
							}
							break;
					}
				}
			}
			//stepnum=1;
		}
		char temp2[25];
		if((stepnum==1)&&(go))
		{
		//	SDL_Rect rr;
		//	rr.x=16;rr.w=screen->w-32;
		//	rr.y=term->font->chr[0]->h*4;rr.h=term->font->chr[0]->h;
		//	SDL_FillRect(screen, &rr, 0);
		//	rr.x=16;rr.w=screen->w-32;
		//	rr.y=term->font->chr[0]->h*16;rr.h=screen->h-16-rr.y;
		//	SDL_FillRect(screen, &rr, 0);
			output(screen, term->font, 3, 17, "Select thy Race: ", 92, 255, 255); 
			for(int x=0;x<4;x++)
			{
				char temp[50];
				char rc[25];
				U1_raceName(rc, x);
				sprintf(temp, "%c) %s", 'a'+x, rc);
				output(screen, term->font, 12, 19+x, temp, 92, 255, 255);
			}
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			refresh(screen, game->screen);
			while(1)
			{
				Uint16 c=waitKey(screen);
				if(c==SDL_QUIT)
					return SDL_QUIT;
				int race=c-SDLK_a;
				if((race>=0)&&(race<=3))
				{
					game->user.race=race;
					switch(game->user.race)
					{
						case 0:	game->user.intelligence+=5;break;
						case 1: game->user.agility+=5;break;
						case 2: game->user.strength+=5;break;
						case 3: game->user.wisdom+=10;game->user.strength-=5;break;
					}
					stepnum=2;
					go=0;
					break;
				}	
			}
		}
		
		if((stepnum>1)&&(go))
		{
			U1_raceName(temp, game->user.race);
			sprintf(temp2, "  Race: %s", temp);
			output(screen, term->font, 12, 13, temp2, 92, 255, 255);
		}
//		printf(":P %d\n", stepnum);
		if((stepnum==2)&&(go))
		{
		//SDL_FillRect(screen, &rr, 0);
			output(screen, term->font, 3, 17, "Select thy Sex: ", 92, 255, 255); 
			for(int x=0;x<2;x++)
			{
				char temp[50];
				char sx[25];
				U1_sexName(sx, x);
				sprintf(temp, "%c) %s", 'a'+x, sx);
				output(screen, term->font, 12, 19+x, temp, 92, 255, 255);
			}
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			refresh(screen, game->screen);
			while(1)
			{
				Uint16 c=waitKey(screen);
				if(c==SDL_QUIT)
					return SDL_QUIT;
				int sex=c-SDLK_a;
				if((sex>=0)&&(sex<=1))
				{
					game->user.sex=sex;
					stepnum=3;
					go=0;
					break;
				}	
			}
		}
		if((stepnum>2)&&(go))
		{
			U1_sexName(temp, game->user.sex);
			sprintf(temp2,   "   Sex: %s", temp);
			output(screen, term->font, 12, 14, temp2, 92, 255, 255);
		}
//			SDL_FillRect(screen, &rr, 0);
		if((stepnum==3)&&(go))
		{		
			output(screen, term->font, 3, 17, "Select thy Class: ", 92, 255, 255); 
			for(int x=0;x<4;x++)
			{
				char temp[50];
				char cl[25];
				U1_className(cl, x);
				sprintf(temp, "%c) %s", 'a'+x, cl);
				output(screen, term->font, 12, 19+x, temp, 92, 255, 255);
			}
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			refresh(screen, game->screen);
			while(1)
			{
				Uint16 c=waitKey(screen);
				if(c==SDL_QUIT)
					return SDL_QUIT;
				int clas=c-SDLK_a;
				if((clas>=0)&&(clas<=3))
				{
					game->user.clas=clas;
					switch(game->user.clas)
					{
						case 0: game->user.strength+=10;game->user.agility+=10;break;
						case 1: game->user.wisdom+=10;break;
						case 2: game->user.intelligence+=10;break;
						case 3: game->user.agility+=10;break;
					}
					stepnum=4;
					go=0;
					break;
				}	
			}
		}
		if((stepnum>3)&&(go))
		{
			U1_className(temp, game->user.clas);
			sprintf(temp2,   " Class: %s", temp);
			output(screen, term->font, 12, 15, temp2, 92, 255, 255);
		}
	//		SDL_FillRect(screen, &rr, 0);
		if((stepnum==4)&&(go))
		{
			output(screen, term->font, 3, 17, "Enter thy name: ", 92, 255, 255); 
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			refresh(screen, game->screen);			
	//		char name[80];
			if(getWord(screen, game->screen, term->font, game->user.name, 19, 17, 92, 255, 255, 14)==SDL_QUIT)
				return SDL_QUIT;
			stepnum=5;
			go=0;
		}
		if((stepnum>4)&&(go))
		{
			output(screen, term->font, 12, 4, game->user.name, 92, 255, 255);
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			refresh(screen, game->screen);
		}
		if((stepnum==5)&&(go))
		{
			output(screen, term->font, 3, 22, "Save this character? (Y-N) ", 92, 255, 255);
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			refresh(screen, game->screen);			
			Uint16 c=waitKey(screen);
			if(c==SDL_QUIT)
				return SDL_QUIT;
			if(c==SDLK_n)
			{
				go=0;
				pleft=30;
				game->user.strength=10;game->user.agility=10;game->user.stamina=10;
				game->user.charisma=10;game->user.wisdom=10;game->user.intelligence=10;
				pos=0;
				stepnum=0;
			}
			if(c==SDLK_y)
				return 1;			
		}
		SDL_UpdateRect(screen, 0, 0, 0, 0);
		refresh(screen, game->screen);		
	}		
	SDL_Delay(2000);
		//Select thy race
		//Select they sex
		//Select thy class
		//Enter they name
		//ok?
	return 0;
}

