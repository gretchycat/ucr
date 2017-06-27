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
#include "../applebasic.h"
#include "u1global.h"
#include "u1placenames.h"
#include "u1out_town_transact.h"

int U1_clearTownBuf(int townBuf[38][18])
{
	for(int x=0;x<38;x++)
		for(int y=0;y<18;y++)
			townBuf[x][y]=-1;
	return 0;
}

int U1_shopString( char *dest, char *src, int num, int price)
{
	char temp[50];
	sprintf(temp, "%c) %s               ", num+'a', src);	   
	temp[17]='-';
	char temp2[7];
	sprintf(temp2, "%d", price);
	int l=strlen(temp2);
 	for(int x=0;x<l;x++) 
		temp[21-x]=temp2[l-x-1];
	temp[22]=0;
	strcpy(dest, temp);
	return 0;
}

int U1_hasWater(int x, int y, TU1_map map)
{
	int b=0;
	for(int xx=-1;xx<=1;xx++)
		for(int yy=-1;yy<=1;yy++)
			b=b||(map[y+yy][x+xx]==0);
	return b;
}
                
int U1_craftPlacable(int shape, int x, int y, TU1_game *game, TU1_map map)
{
	int p=1;
//	if(map[y][x]==1)
	
	return p;
}                
                
int U1_getFoodBuyPrice(int intelligence) 
{
	return 3;
}
        						 					
int U1_getWeaponBuyPrice(int wp, int intelligence)
{	//CLOSE -- NOT EXACT! FIX ME!
	int base=4;
	for(int x=0;x<=wp;x++)
		base+=x*2-1;
	base++;
	int subt=0;
	if(base>5)
		subt=(intelligence/(256/(base-5)));
	else
		base=5;
	return base-subt;
}

int U1_getWeaponSellPrice(int wp, int charisma)
{	//CLOSE -- NOT EXACT! FIX ME!
	int min=0;
	int max=0;
	switch(wp)
	{
		case 1:	min=1;max=5;break;
		case 2: min=1;max=5;break;
		case 3: min=2;max=11;break;
		case 4: min=3;max=19;break;
		case 5:	min=4;max=29;break;
		case 6: min=6;max=42;break;
		case 7:	min=8;max=57;break;
		case 8:	min=11;max=75;break;
		case 9:	min=13;max=94;break;
		case 10: min=16;max=116;break;
		case 11: min=20;max=138;break;
		case 12: min=24;max=160;break;
		case 13: min=28;max=185;break;
		case 14: min=32;max=208;break;
		case 15: min=36;max=235;break;
		
	}
	return max-((256-charisma)*(max-min)/256);
}

int U1_getArmorBuyPrice(int ar, int intelligence)
{	//CORRECT
	int base=ar*50;
	return base-(ar*(((intelligence)+3)/4));
}

int U1_getArmorSellPrice(int ar, int charisma)
{	//CORRECT
	return charisma/4*ar;
}

int U1_getSpellBuyPrice(int sp, int wisdom)
{	//CORRECT
	if(wisdom>26)
		wisdom=26;
	int base=sp*6;
	return base-(wisdom/9*sp);
}

int U1_getCraftBuyPrice(int cr, int intelligence)
{	//CORRECT
	int base=(2*cr)*(2*cr)*10;
	return base-((cr*cr)*((intelligence+3)/5));
}

int U1_getSpellSellPrice(int sp, int charisma)
{	
	return 0;
}

int U1_getCraftSellPrice(int cr, int charisma)
{
	return 0;
}

int printTables()
{
	for(int i=0;i<64;i++)
	{
		printf("%d: ", i);
		for(int wp=0;wp<12;wp++)
		{
			printf("%d	", U1_getWeaponBuyPrice(wp, i));
		}
		printf("\n");
	}
	for(int c=0;c<64;c++)
	{
		printf("%d: ", c);
		
		for(int wp=0;wp<12;wp++)
		{
			printf("%d	", U1_getWeaponSellPrice(wp, c));
		}
		printf("\n");
	}
	for(int i=0;i<64;i++)
	{
		printf("%d: ", i);
		
		for(int ar=0;ar<12;ar++)
		{
			printf("%d	", U1_getArmorBuyPrice(ar, i));
		}
		printf("\n");
	}
	for(int c=0;c<64;c++)
	{
		printf("%d: ", c);
		
		for(int ar=0;ar<12;ar++)
		{
			printf("%d	", U1_getArmorSellPrice(ar, c));
		}
		printf("\n");
	}
	for(int w=0;w<64;w++)
	{
		printf("%d: ", w);
		
		for(int sp=0;sp<12;sp++)
		{
			printf("%d	", U1_getSpellBuyPrice(sp, w));
		}
		printf("\n");
	}
	for(int i=0;i<64;i++)
	{
		printf("%d: ", i);
		
		for(int cr=0;cr<12;cr++)
		{
			printf("%d	", U1_getCraftBuyPrice(cr, i));
		}
		printf("\n");
	}
	return 0;
}

int U1_drawTown(SDL_Surface *screen, SDL_Surface *townbufsurface, int *updated, TU1_tiles tiles, TU1_towns towns, int tnum, int xx, int yy, TU1_user *user, TU1_npc npc[10][15])
{
	SDL_Surface *buffer=SDL_CreateRGBSurface(SDL_SWSURFACE, tiles.xdim*38, tiles.ydim*18, 32, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
	SDL_Rect destrect;
	destrect.h=tiles.ydim;
	destrect.w=tiles.xdim;
	if(*updated==0)
	{
		for(int x=0;x<38;x++)
		{
			for(int y=0;y<18;y++)
			{
					{
						int tilenum=1;
						if(towns[tnum][x][y]<(tiles.num-1))
							tilenum=towns[tnum][x][y];
						destrect.x=x*tiles.xdim;
						destrect.y=y*tiles.ydim;
						if(SDL_BlitSurface(tiles.tiles[tilenum], NULL, buffer, &destrect))
							printf("\nError with Blit.\n");	
						*updated=1;
					}
			}
		}
		SDL_BlitSurface(buffer, NULL, townbufsurface, NULL);
	}
	else
		SDL_BlitSurface(townbufsurface, NULL, buffer, NULL);
	for(int x=0;x<15;x++)
		if(npc[tnum][x].shape>0)
		{
			if(npc[tnum][x].shape<tiles.num)
			{
				destrect.x=npc[tnum][x].x*tiles.xdim;
				destrect.y=npc[tnum][x].y*tiles.ydim;
				if(SDL_BlitSurface(tiles.tiles[npc[tnum][x].shape], NULL, buffer, &destrect))
					printf("\nError with Blit.\n");
			}
		}
	destrect.x=xx*tiles.xdim;
	destrect.y=yy*tiles.ydim;
	if(SDL_BlitSurface(tiles.tiles[18], NULL, buffer, &destrect))
		printf("\nError with Blit.\n");

	if((tiles.xdim==8)&&(tiles.ydim==8))
	{
		SDL_Surface *buffer2=SDL_Scale_2xSai(buffer);
		SDL_FreeSurface(buffer);
		buffer=buffer2;
	}
	SDL_Rect n;
	n.x=16;n.y=16;
	n.w=608;n.h=288;
	SDL_BlitSurface(buffer, NULL, screen, &n);
	SDL_FreeSurface(buffer);
	return 0;
}

int U1_townCollision(TU1_towns towns, int tnum, TU1_npc npc[10][15], int xx, int yy, int x, int y, int npcnum)
{
	if(((towns[tnum][x][y]!=1)&&(towns[tnum][x][y]<51))||((x<0)||(x>37)||(y<0)||(y>17)))
	{
		return -2;
	}
	for(int z=0;z<15;z++)
		if(npc[tnum][z].shape>0)
		{
			if((x==npc[tnum][z].x)&&(y==npc[tnum][z].y))
			{
				return z+1;
			}
			if((x==xx)&&(y==yy))
			{
				return -3;
			}
		}
	if(npcnum>-1)
	{
		if((npc[tnum][npcnum].shape==50)&&(towns[tnum][x][y]==63))
			return -1;
	}
	return 0;
}

int U1_attacktown(SDL_Surface *screen, Tterminal *term, TU1_game *game, TU1_towns towns, int towntype, TU1_npc npc[10][15], int xx, int yy)
{
	int hit=0;
	char attackstr[30];
	char weaponstr[20];
	U1_weaponName(weaponstr, game->user.weaponready);
	sprintf(attackstr, "Attack with %s: ", weaponstr);
	SDL_Delay(100);
	term->print(attackstr);						
	term->blit();
	int attacked=0;
	refresh(screen, game->screen);
	Uint32 inp0=term->waitKey();
	int dis=1;
	switch(game->user.weaponready)
	{
		case 7:
		case 12:
		case 14:
		case 15: dis=4;break;
	}
	int dx=0;
	int dy=0;
	switch(inp0)
	{
		case SDLK_LEFT:	term->print("West\n");
				dx=-1;
				break;
		case SDLK_RIGHT:term->print("East\n");
				dx=1;
				break;
		case SDLK_UP:	term->print("North\n");
				dy=-1;
				break;
		case SDLK_DOWN:	term->print("South\n");
				dy=1;
				break;
		default:	term->print("nothing\n");
				break;
	}
	for(int x=1;x<dis;x++)
	{
		attacked=U1_townCollision(towns, towntype, npc, xx, yy, xx+dx*x, yy+dy*x, -1);
		if(attacked>=1)
			x=dis+1;
	}
	term->blit();
	if(attacked<1)
	{
		term->print(" Missed!\n");
		
		term->blit();
	}
	else
	{
		hit=1;
		//DO ATTACK!
		int dmg=(int)((rnd(1)*(game->user.weaponready+1)*(game->user.agility-10)/10.0)+rnd(1)*game->user.strength/5);
		if(dmg)
		{
			char tmp[80];
			npc[towntype][attacked-1].hits-=dmg;
			char en[20];
//for(int w=0;w<20;w++)
//printf("%d:	\"%s\"\n", w, game->out.S_towntexts[w]);
			int s=U1_npcName(npc[towntype][attacked-1].shape);
			if((s==13)&&(towntype<2))
				s++;
			strcpy(en, game->out.S_towntexts[s]);
			if(npc[towntype][attacked-1].hits<=0)
			{
				sprintf(tmp, " %s%s%s\n", game->out.S_towntexts[8], en, game->out.S_towntexts[17]);
				npc[towntype][attacked-1].x=0;
				npc[towntype][attacked-1].y=0;
				npc[towntype][attacked-1].shape=0;
				if(s==14)
					game->key=rand()%2+1;
			}
			else
				sprintf(tmp, " %s%s%d damage!\n", game->out.S_towntexts[8], en, dmg);
			term->print(tmp);
			
			
		}
		
	}
//	SDL_Delay(100);
//		printf("HIT: %d\n", hit);
	return hit;
}

int U1_droptown(SDL_Surface *screen, Tterminal *term, TU1_game *game, TU1_towns towns, int towntype, TU1_npc npc[10][15], int xx, int yy)
{
						term->print("Drop Pence,Weapon,Armour:");
						refresh(screen, game->screen);
						Uint32 inp0=term->waitKey();
						switch(inp0)
						{
							case SDLK_p:
							{
								term->dropLine();
								U1_printWithTick(term, "");
								term->print("Drop pence:");
								refresh(screen, game->screen);
								Uint32 inp1=term->getNum(0, 9999);
								if(inp1>game->user.coin)
								{
									term->print("\n");
									term->print(" Thou hast not that much!\n");
									term->blit();
								}
								else
								{
									if(inp1==0)
									{
										term->print(" none\n");
									}
									else
									{
										term->print("\n");
										game->user.coin-=inp1;
										switch(towns[towntype][xx][yy])
										{
											case 51:	{
														term->print(" Shazam!\n");
														game->user.hits+=(int)((double)inp1/2.0*3.0);
														break;
														}
											case 52:	{
														term->print(" Shazam!\n");
														break;
														}
											case 53:	{
														term->print(" Shazam!\n");
														game->user.food+=(int)((double)inp1*3.0);
														break;
														}
											default:	{
														term->print(" Ok!\n");
														break;
														}
										}
									}
								}
							break;
							}
							case SDLK_w:
									{
										term->dropLine();
										U1_printWithTick(term, "");
										term->print("Drop weapon: ");
										refresh(screen, game->screen);
										int n=U1_listWeapons(screen, term, game, "Drop", 1);
										if((n==-1)||(game->user.weapon[n]<=0))
										{
											term->print("none\n");
											break;
										}
										term->print(game->out.S_weapons[n]);
										term->print("\n");
										game->user.weapon[n]--;
										if((game->user.weapon[n]<=0)&&(n==game->user.weaponready))
											game->user.weaponready=0;
									}
									break;
							case SDLK_a:
									{
										term->dropLine();
										U1_printWithTick(term, "");
										term->print("Drop armor: ");
										refresh(screen, game->screen);
										int n=U1_listArmors(screen, term, game, "Drop", 1);
										if((n==-1)||(game->user.armor[n]<=0))
										{
											term->print("none\n");
											break;
										}
										term->print(game->out.S_armors[n]);
										term->print("\n");
										game->user.armor[n]--;
										if((game->user.armor[n]<=0)&&(n==game->user.armorready))
											game->user.armorready=0;
									}
									break;
							default: 
								{
									term->dropLine();
									U1_printWithTick(term, "");
									term->print("Drop nothing\n");
									break;	
								}	
						}

	return 0;
}

int U1_stealtown(SDL_Surface *screen, Tterminal *term, TU1_game *game, TU1_towns towns, int towntype, TU1_npc npc[10][15], int xx, int yy)
{
					int caught=(game->user.agility<(rand()%75));
					term->print("Steal");
					if((caught)&&((towns[towntype][xx][yy]==55)||(towns[towntype][xx][yy]==57)||(towns[towntype][xx][yy]==59)))
					{
						char temp[40];
						sprintf(temp, " %s\n", game->out.S_steal[0]);
						term->print(temp);
					}
					else
						switch(towns[towntype][xx][yy])
						{
							case 55: {
										term->print("\n");
										char temp[40];
										int num=rand()%5+1;
										sprintf(temp, " %s%s\n", game->out.S_steal[1], game->out.S_armors[num]);
										term->print(temp);
										game->user.armor[num]++;
									}
									break;
							case 57: {
										term->print("\n");
										char temp[40];
										int num=rand()%29+1;
										sprintf(temp, " %s%d%s\n", game->out.S_steal[1], num, game->out.S_steal[2]);
										term->print(temp);
										sprintf(temp, " %s\n", game->out.S_steal[3]);
										term->print(temp);
										game->user.food+=(num*10);
									}
									break;
							case 59: {
										term->print("\n");
										char temp[40];
										int num=rand()%15+1;
										sprintf(temp, " %s%s%s\n", game->out.S_steal[1], game->out.S_steal[4], game->out.S_weapons[num]);
										if((int)strlen(temp)<(int)term->columns-1)
											term->print(temp);
										else
										{
											sprintf(temp, " %s%s\n", game->out.S_steal[1], game->out.S_steal[4]);
											term->print(temp);
											sprintf(temp, " %s\n", game->out.S_weapons[num]);
											term->print(temp);
										}
										game->user.weapon[num]++;
									}
									break;
							default: 
									term->dropLine();
									U1_printWithTick(term, "");
									term->print("Steal - nothing here!\n");
									break;
							
						}
	return caught;
}

int U1_unlocktown(SDL_Surface *screen, Tterminal *term, TU1_game *game, TU1_towns towns, int towntype, TU1_npc npc[10][15], int xx, int yy)
{
	int u=0;
//	for(int w=0;w<7;w++)
//		printf("%d	\"%s\"\n", w, game->out.S_unlock[w]);
	
	switch(towns[towntype][xx][yy])
	{
		case 60:
		case 61:
				{
					if(game->key==0)
					{
						char temp[40];
						sprintf(temp, "%s\n", game->out.S_unlock[6]);
						term->print(temp);
						sprintf(temp, " %s\n", game->out.S_unlock[2]);
						term->print(temp);
					}
					else
					{
						if((game->key+59)==(towns[towntype][xx][yy]))
						{
							char temp[40];
							sprintf(temp, "%s\n", game->out.S_unlock[6]);
							term->print(temp);
							sprintf(temp, " %s\n", game->out.S_unlock[5]);
							term->print(temp);
	//						printf("%d\n", towns[towntype][xx][yy-1]);
							towns[towntype][xx][yy-1]=1;
		//					printf("%d\n", towns[towntype][xx][yy-1]);
							u=1;
						}
						else
						{
							char temp[40];
							sprintf(temp, "%s\n", game->out.S_unlock[6]);
							term->print(temp);
							sprintf(temp, " %s\n", game->out.S_unlock[3]);
							term->print(temp);
							sprintf(temp, " %s\n", game->out.S_unlock[4]);
							term->print(temp);

						}
					}
				}
				break;
		default:
				{
					char temp[40];
					sprintf(temp, "%s what?\n", game->out.S_unlock[6]);
					term->print(temp);
				}
				break;
	}
	return u;			
}

int U1_freePrincess(SDL_Surface *screen, Tterminal *term, TU1_game *game)
{
  char txt[12][31];
  for(int x=0;x<11;x++)
		txt[x][0]=0;
	int t=3;
	int ace=0;
	if(ace)
		t=1;        
	//printf("FREEDOM FOR THE PRINCESS!\n");
	for(int w=0;w<4;w++)
	{
		strcpy(txt[w+t], game->out.S_princess[w]);
	}
	game->user.hits+=500;
	game->user.exp+=500;
	game->user.coin+=500;
//	U1_msgBox(screen, term, &game->user, txt, 0);
//  for(int x=0;x<11;x++)
//		txt[x][0]=0;
	if(ace)	//space ace
	{
		for(int w=4;w<8;w++)
		{
			strcpy(txt[w+2], game->out.S_princess[w]);
		}
	}
	U1_msgBox(screen, term, &game->user, txt, 0);
	return 0;
}

int U1_navigateTown(SDL_Surface *screen, Tterminal *term, TU1_game *game, TU1_map map)
{
	SDL_EnableKeyRepeat(50, 0);
	int towntype=0;
	game->key=0;
	char name[50];
	name[0]=0;
	int craftshopclosed=0;
	int townnmidx=U1_townname(game->user.x, game->user.y);
  if(townnmidx<32)
  {
    strcpy(name, game->out.S_towns[townnmidx]);
    towntype=townnmidx%8+2;
  }
  else
  {
    strcpy(name, game->out.S_castles[townnmidx-32]);
    towntype=townnmidx%2;
  }
                            
	char songpath[80];
	Mix_Music *music = NULL;		
	switch(towntype)
	{
		case 0: 
		case 1: sprintf(songpath, "%s/u1/midi/thecastl.mid", game->config.datapath);break;
		case 2: 
		case 3: 
		case 4: 
		case 5: 
		case 6: 
		case 7: 
		case 8: 
		case 9: 
		default: sprintf(songpath, "%s/u1/midi/towns.mid", game->config.datapath);break;
	}
	if(game->config.game[1].music)
		music=Mix_LoadMUS(songpath);
	int looping=-1;
	if(music)
		Mix_FadeInMusic(music,looping,500);
	else
		printf("COULDNT OPEN!\n");
	TU1_tiles tiles;
	if(U1_loadtiles(&game->config, &tiles, "town", game->config.game[1].svga))
	{
		return -1;
	}
	TU1_towns towns;	
	U1_loadtowns(&game->config,  towns);
	char townname[40];
	term->print("Entering...\n");
	if(towntype>1)
		sprintf(townname, " The City of %s\n", name);
	else
		sprintf(townname, " %s\n", name);
	term->print(townname);
	term->blit();
	TU1_npc npc[10][15];
	U1_loadnpc(&game->config, npc);
	int xx;
	int yy;
	if(towntype>1)
	{
		xx=19;
		yy=17;
	}
	else
	{
		xx=0;
		yy=9;
	}
	int k=0;
	int go=0;
	int bad=0; 
	int quit=0;
	int lastmove[15];
	U1_printWithTick(term, "");
	for(int z=0;z<15;z++)
		lastmove[z]=1;
		
	SDL_Surface *townbufsurface=SDL_CreateRGBSurface(SDL_SWSURFACE, 16*38, 16*18, 32, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
	int updated=0;
	while(quit==0)
	{		
		U1_drawTown(screen, townbufsurface, &updated, tiles, towns, towntype, xx, yy, &game->user, npc);
		refresh(screen, game->screen);
		k=term->waitKey();
		go=0;
		if(k)
		{
			switch(k)
			{
				case SDL_QUIT:	
						if( Mix_PlayingMusic() )
							Mix_FadeOutMusic(500);
						if ( music ) 
						{
							Mix_FreeMusic(music);
							music = NULL;
						}
						for(int x=0;x<tiles.num;x++)
							if(tiles.tiles[x]!=NULL)
								SDL_FreeSurface(tiles.tiles[x]);

						return -1;break;
				case SDLK_LEFT:	if(xx==0)
						{
							quit=1;
							//if princess' x==1, princess reward
							int p=-1;
							for(int w=0;w<15;w++)
								if(npc[towntype][w].shape==22)
									p=w;
							if(p!=-1)
								if(npc[towntype][p].x==1)
									U1_freePrincess(screen, term, game);
							break;
						}
						if(U1_townCollision(towns, towntype, npc, xx, yy, xx-1, yy, -1)==0)
						{
							xx--;
							term->print("West\n");							
						}
						else
						{
							term->print("Blocked!\n");							
						}
						go=1;
						break;
				case SDLK_RIGHT:if(xx==37)
						{
							quit=1;
							break;
						}
						if(U1_townCollision(towns, towntype, npc, xx, yy, xx+1, yy, -1)==0)
						{
							xx++;
							term->print("East\n");							
						}
						else
						{
							term->print("Blocked!\n");							
						}
						go=1;
						break;
				case SDLK_UP:	if(yy==0)
						{
							quit=1;
							break;
						}
						if(U1_townCollision(towns, towntype, npc, xx, yy, xx, yy-1, -1)==0)
						{
							yy--;
							term->print("North\n");							
						}
						else
						{
							term->print("Blocked!\n");							
						}
						go=1;
						break;
				case SDLK_DOWN:	if(yy==17)
						{
							quit=1;
							break;
						}
						if(U1_townCollision(towns, towntype, npc, xx, yy, xx, yy+1, -1)==0)
						{
							yy++;
							term->print("South\n");							
						}
						else
						{
							term->print("Blocked!\n");							
						}
						go=1;
						break;
				case SDLK_SPACE:	go=1;
						term->print("Pass\n");						
						break;
				case SDLK_a:	{
						go=1;
						int hit=U1_attacktown(screen, term, game, towns, towntype, npc, xx, yy);
						if(hit==SDL_QUIT)
							quit=1;
						if((bad==0)&&(hit==1))
						{
							sprintf(songpath, "%s/u1/midi/combat.mid", game->config.datapath);
							if(game->config.game[1].music)
							{
								if( Mix_PlayingMusic() )
								{
									Mix_FadeOutMusic(500);
								}
								if ( music ) 
								{
									Mix_FreeMusic(music);
									music = NULL;
								}
								if(game->config.game[1].music)
									music=Mix_LoadMUS(songpath);
								if(music)
									Mix_FadeInMusic(music,looping,500);
							}
							bad=1;
						}	
						}
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
						U1_droptown(screen, term, game, towns, towntype, npc, xx, yy);
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
							sprintf(temp, "%s%s?\n", game->out.S_get[3], game->out.S_get[4]);
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
							char temp[40];
							sprintf(temp, "%s\n", game->out.S_inform[2]);
							term->print(temp);
							term->print(townname);
						}
						break;
				case SDLK_k:
						{
							char temp[40];
							sprintf(temp, "%s?\n", game->out.S_klimb[4]);
							term->print(temp);
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
					{
						go=1;
						int hit=0;
						if(bad==0)
							hit=U1_stealtown(screen, term, game, towns, towntype, npc, xx, yy);
						else
						{
							char temp[40];
							sprintf(temp, " %s\n", game->out.S_steal[0]);
							term->print(temp);			
						}
						if((bad==0)&&(hit==1))
						{
							sprintf(songpath, "%s/u1/midi/combat.mid", game->config.datapath);
							if(game->config.game[1].music)
							{
								if( Mix_PlayingMusic() )
								{
									Mix_FadeOutMusic(500);
								}
								if ( music ) 
								{
									Mix_FreeMusic(music);
									music = NULL;
								}
								if(game->config.game[1].music)
									music=Mix_LoadMUS(songpath);
								if(music)
									Mix_FadeInMusic(music,looping,500);
							}
							bad=1;
						}	
					
					}
					break;
				case SDLK_t:	
						go=1;
						if(towntype<2)
						{
							if(towns[towntype][xx][yy]==62)
							{
								if(bad==0)
								{
									if(music)
										Mix_FadeOutMusic(500);
									Mix_Music *msc = NULL;
									char tempfn[512];
									sprintf(tempfn, "%s/u1/midi/rulebrit.mid", game->config.datapath);
									if(game->config.game[1].music)
										msc = Mix_LoadMUS(tempfn);
									int looping=-1;
									if(msc)
										Mix_FadeInMusic(msc,looping,500);

									U1_transactKing(screen, term, game);							
									if ( msc )
									{
										Mix_FadeOutMusic(500);
										Mix_FreeMusic(msc);
										msc = NULL;
										if(music)
											Mix_FadeInMusic(music,looping,500);
									}
								}
								else
								{
									term->print(game->out.S_transact[36]);
									term->print("\n");
									term->print(" ");
									term->print(game->out.S_transact[38]);
									term->print("\n");
								}
 							}
							else
							{
								term->print(game->out.S_transact[36]);
								term->print("\n");
								term->print(" ");
								term->print(game->out.S_transact[37]);
								term->print("\n");
								break;
							}
						}
						else
						{

							if((towns[towntype][xx][yy]<=62)&&(towns[towntype][xx][yy]>=54))
							{
								if(bad==0)
								{
								if(music)
									Mix_FadeOutMusic(500);

								Mix_Music *msc = NULL;
								char tempfn[512];
								sprintf(tempfn, "%s/u1/midi/shoppin.mid", game->config.datapath);
								if(game->config.game[1].music)
									msc=Mix_LoadMUS(tempfn);
								int looping=-1;
								if(msc)
									Mix_FadeInMusic(msc,looping,500);
											switch(towns[towntype][xx][yy])
											{
												case 54:	
													U1_transactArmor(screen, term, game, towntype);
														break;
												case 56:	
													U1_transactFood(screen, term, game, towntype);
														break;
												case 58:
													U1_transactWeapon(screen, term, game, towntype);
														break;
												case 60:	
													U1_transactMagic(screen, term, game, towntype);
														break;
												case 61:	
													U1_transactPub(screen, term, game, towntype);
														break;
												case 62:	
													craftshopclosed=U1_transactTransport(screen, term, game, towntype, craftshopclosed, map);
														break;
								}
								if ( msc )
								{
									Mix_FadeOutMusic(500);
									Mix_FreeMusic(msc);
									msc = NULL;
									if(music)
										Mix_FadeInMusic(music,looping,500);

								}
								}
								else
								{
									term->print("Transact\n");
									term->print(" None will talk to thee!\n");
								}

							}
							else
							{
								term->print("Transact\n");
								term->print(" Thou art not by a counter!\n");
							}
						}
						break;
				case SDLK_u:	go=1;
						{
							if(U1_unlocktown(screen, term, game, towns, towntype, npc, xx, yy))
								updated=0;
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
							for(int x=0;x<tiles.num;x++)
								if(tiles.tiles[x]!=NULL)
									SDL_FreeSurface(tiles.tiles[x]);

							return -1;
						}
						break; 


				case SDLK_F11:	if(game->config.game[1].svga)
						{
							updated=0;
							game->config.game[1].svga=0;
							term->print("OLD GRAPHICS\n");
						}
						else
						{
							updated=0;
							game->config.game[1].svga=1;
							term->print("NEW GRAPHICS\n");
						}
						tiles.xdim=0;
						tiles.ydim=0;
						for(int x=0;x<tiles.num;x++)
							if(tiles.tiles[x])
								SDL_FreeSurface(tiles.tiles[x]);
						tiles.num=0;
						if(U1_loadtiles(&game->config, &tiles, "town", game->config.game[1].svga))	//w00t
							return -1;
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

				default:
				term->print("Huh?\n");go=1;break;
			}
			if(quit)
				term->clearMouseEvents();
		}
		if(go)
		{
			for(int x=0;x<15;x++)
			{
				int a=(npc[towntype][x].y-yy)*2;
				int b=npc[towntype][x].x-xx;
				if(((npc[towntype][x].shape==17)||(npc[towntype][x].shape==22))&&(bad==1)&&(sqrt(pow(a, 2)+pow(b, 2))<13))
				{
					if(lastmove[x])
					{
						if(npc[towntype][x].x>xx)
						{
							if(U1_townCollision(towns, towntype, npc, xx, yy, npc[towntype][x].x-1, npc[towntype][x].y, x)==0)
								npc[towntype][x].x--;
							lastmove[x]=0;
						}
						else
						if(npc[towntype][x].x<xx)
						{
							if(U1_townCollision(towns, towntype, npc, xx, yy, npc[towntype][x].x+1, npc[towntype][x].y, x)==0)
								npc[towntype][x].x++;
							lastmove[x]=0;
						}
						if((abs(xx-npc[towntype][x].x)==1)&&(abs(xx-npc[towntype][x].x)==1))
						{
							if(npc[towntype][x].shape==17)
							{
								char t[20];
								sprintf(t, " %s\n", game->out.S_towntexts[6]);
								term->print(t);
								int dmg=(int)(35-rnd(1)*game->user.agility);
								if(dmg>0)
								{
								sprintf(t, " %s%d damage!\n", game->out.S_towntexts[7], dmg);
									game->user.hits-=dmg;
									if(game->user.hits<=0)
										return 0;	
								}
								else
								{
									sprintf(t, "  %s\n", "Missed!");
								}
								term->print(t);
							}
						}
					}
					else
					{
						if(npc[towntype][x].y>yy)
						{
							if(U1_townCollision(towns, towntype, npc, xx, yy, npc[towntype][x].x, npc[towntype][x].y-1, x)==0)
								npc[towntype][x].y--;
							lastmove[x]=1;
						}
						else
						if(npc[towntype][x].y<yy)
						{
	 						if(U1_townCollision(towns, towntype, npc, xx, yy, npc[towntype][x].x, npc[towntype][x].y+1, x)==0)
								npc[towntype][x].y++;
							lastmove[x]=1;
						}
					}
					if(npc[towntype][x].y==yy)
						lastmove[x]=1;
					if(npc[towntype][x].x==xx)
						lastmove[x]=0;
					
				}
				if(((npc[towntype][x].shape==19)||(npc[towntype][x].shape==22)||(npc[towntype][x].shape==50))&&(bad==0))
				{
					int xmove=rand()%3-1;
					int ymove=rand()%3-1;
					if(U1_townCollision(towns, towntype, npc, xx, yy, npc[towntype][x].x+xmove, npc[towntype][x].y+ymove, x)==0)
					{
						npc[towntype][x].x+=xmove;
						npc[towntype][x].y+=ymove;
					}
					if((npc[towntype][x].x+xmove==xx)&&(npc[towntype][x].y+ymove==yy)&&(npc[towntype][x].shape==19))
					{
						int item=rand()%15+1;
						if((game->user.weapon[item]>0)&&(item!=game->user.weaponready))
						{
							term->print(" Iolo stole something!\n");
							game->user.weapon[item]--;
						}
					}
					if((npc[towntype][x].shape==19)&&(rand()%20==0))
					{
						int s=0;
						if(towntype<2)
						{
							s=4;	//Gwino
						}
						else
						{
							s=2;	//Iolo
						}
						char t[80];
						sprintf(t, " %s\n", game->out.S_towntexts[s]);
						term->print(t);
						sprintf(t, " %s\n", game->out.S_towntexts[s+1]);
						term->print(t);
					}
				}
			}
		}
		U1_stats(screen, term->font, &game->user);
		if(quit==0)
		{
			U1_printWithTick(term, "");
		}
		term->blit();
	}
	SDL_FreeSurface(townbufsurface);
	if( Mix_PlayingMusic() )
	{
		Mix_FadeOutMusic(500);
	}
	if ( music ) 
	{
		Mix_FreeMusic(music);
		music = NULL;
	}
	for(int x=0;x<tiles.num;x++)
		if(tiles.tiles[x]!=NULL)
			SDL_FreeSurface(tiles.tiles[x]);
	return 0;
}
