#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../2xsai.h"
#include "../SDL_rotozoom.h"
#include "../globaltypes.h"
#include "u1types.h"
#include "u1data.h"
#include "../globalfunctions.h"
#include "../declarations.h"
#include "u1global.h"
#include "../applebasic.h"
#include "u1out_town.h"
#include "u1out_dungeon.h"
#include "u1out_dungeon.h"
#include "u1out_shrine.h"

int U1_nextString(char *str, FILE *in, int maxsz)
{
	int p=0;
	char c=1;
	while((c!=0)&&(!feof(in)))
	{
		fread(&c, 1, 1, in);
		if(isprint(c)||(c==0))
		{
			str[p]=c;
		}
		else
		{
			str[p]='?';
		}
		if(p<maxsz)
			p++;
	}
	if(p==0)
		return 1;
	return 0;
}

int U1_getOutEXE_Strings(TglobalConfig *config, TU1_out_strings *u1outstrings)
{
	FILE *in; char tempfn[512]; sprintf(tempfn, "%s/out.exe",
	config->game[1].gamepath); 
	in=fopen(tempfn, "rb"); 
	int x=0; 
	if(in) {
		fseek(in, 0xd812, SEEK_SET);	//point to the start of text data
		for(x=0;x<4;x++)
			U1_nextString(u1outstrings->S_currency[x], in, 40);
		for(x=0;x<16;x++)
			U1_nextString(u1outstrings->S_hittype[x], in, 40);
		for(x=0;x<10;x++)
			U1_nextString(u1outstrings->S_board[x], in, 40);
		for(x=0;x<17;x++)
			U1_nextString(u1outstrings->S_spell[x], in, 40);
		for(x=0;x<12;x++)
			U1_nextString(u1outstrings->S_drop[x], in, 40);
		for(x=0;x<20;x++)
			U1_nextString(u1outstrings->S_towntexts[x], in, 40);
		for(x=0;x<6;x++)
			U1_nextString(u1outstrings->S_transportshops[x], in, 40);
		for(x=0;x<7;x++)
			U1_nextString(u1outstrings->S_armorshops[x], in, 40);
		for(x=0;x<7;x++)
			U1_nextString(u1outstrings->S_weaponshops[x], in, 40);
		for(x=0;x<5;x++)
			U1_nextString(u1outstrings->S_spellshops[x], in, 40);
		for(x=0;x<7;x++)
			U1_nextString(u1outstrings->S_foodshops[x], in, 40);
		for(x=0;x<7;x++)
			U1_nextString(u1outstrings->S_pubs[x], in, 40);
		U1_nextString(u1outstrings->S_mimic, in, 40);
		for(x=0;x<10;x++)
			U1_nextString(u1outstrings->S_directions[x], in, 40);
		for(x=0;x<8;x++)
			U1_nextString(u1outstrings->S_princess[x], in, 40);
		for(x=0;x<26;x++)
			U1_nextString(u1outstrings->S_shrinetexts[x], in, 40);
		for(x=0;x<3;x++)
			U1_nextString(u1outstrings->S_enter[x], in, 40);
		for(x=0;x<5;x++)
			U1_nextString(u1outstrings->S_fire[x], in, 40);
		for(x=0;x<5;x++)
			U1_nextString(u1outstrings->S_get[x], in, 40);
		U1_nextString(u1outstrings->S_hyperjump, in, 40);
		for(x=0;x<7;x++)
			U1_nextString(u1outstrings->S_inform[x], in, 40);
		for(x=0;x<7;x++)
			U1_nextString(u1outstrings->S_klimb[x], in, 40);
		for(x=0;x<3;x++)
			U1_nextString(u1outstrings->S_noise[x], in, 40);
		for(x=0;x<3;x++)
			U1_nextString(u1outstrings->S_open[x], in, 40);
		for(x=0;x<4;x++)
			U1_nextString(u1outstrings->S_quit[x], in, 40);
		for(x=0;x<6;x++)
			U1_nextString(u1outstrings->S_steal[x], in, 40);
		u1outstrings->S_steal[1][strlen(u1outstrings->S_steal[1])-1]=0;
		for(x=0;x<3;x++)
			U1_nextString(u1outstrings->S_shopbuy[x], in, 40);
		for(x=0;x<41;x++)
			U1_nextString(u1outstrings->S_pubtext[x], in, 40);
		for(x=0;x<2;x++)
			U1_nextString(u1outstrings->S_weaponsell[x], in, 40);
		U1_nextString(u1outstrings->S_armorsell, in, 40);
		U1_nextString(u1outstrings->S_spellsell, in, 40);
		U1_nextString(u1outstrings->S_foodsell, in, 40);
		for(x=0;x<7;x++)
			U1_nextString(u1outstrings->S_foodbuy[x], in, 40);
		for(x=0;x<3;x++)
			U1_nextString(u1outstrings->S_transportsell[x], in, 40);
		for(x=0;x<50;x++)
			U1_nextString(u1outstrings->S_transact[x], in, 40);
		U1_nextString(u1outstrings->S_view, in, 40);
		for(x=0;x<7;x++)
			U1_nextString(u1outstrings->S_unlock[x], in, 40);
		for(x=0;x<3;x++)
			U1_nextString(u1outstrings->S_xit[x], in, 40);
		U1_nextString(u1outstrings->S_pass, in, 40);
		U1_nextString(u1outstrings->S_huh, in, 40);
		fseek(in, 0xed60, SEEK_SET);		
		for(x=0;x<8;x++)
			U1_nextString(u1outstrings->S_attack[x], in, 40);
		fseek(in, 0xfcfe, SEEK_SET);
		for(x=0;x<7;x++)
			U1_nextString(u1outstrings->S_ready[x], in, 40);
		for(x=0;x<4;x++)
			U1_nextString(u1outstrings->S_stats[x], in, 40);
		fseek(in, 0xfd7e, SEEK_SET);		
		for(x=0;x<16;x++)
			U1_nextString(u1outstrings->S_weapons[x], in, 40);
		fseek(in, 0xfe1e, SEEK_SET);
		for(x=0;x<16;x++)
			U1_nextString(u1outstrings->S_weaponsshort[x], in, 40);
		fseek(in, 0xfea4, SEEK_SET);
		for(x=0;x<4;x++)
			U1_nextString(u1outstrings->S_races[x], in, 40);
		fseek(in, 0xfec4, SEEK_SET);
		for(x=0;x<4;x++)
			U1_nextString(u1outstrings->S_classes[x], in, 40);
		fseek(in, 0xff38, SEEK_SET);
		for(x=0;x<11;x++)
			U1_nextString(u1outstrings->S_spells[x], in, 40);
		fseek(in, 0xffa6, SEEK_SET);
		for(x=0;x<6;x++)
			U1_nextString(u1outstrings->S_armors[x], in, 40);
		fseek(in, 0xfff4, SEEK_SET);
		for(x=0;x<8;x++)
			U1_nextString(u1outstrings->S_crafts[x], in, 40);
		fseek(in, 0x1003e, SEEK_SET);
		for(x=0;x<4;x++)
			U1_nextString(u1outstrings->S_gems[x], in, 40);
		fseek(in, 0x1006c, SEEK_SET);
		for(x=0;x<4;x++)
			U1_nextString(u1outstrings->S_saves[x], in, 40);
		fseek(in, 0x100a0, SEEK_SET);
		for(x=0;x<14;x++)
			U1_nextString(u1outstrings->S_enemies[x], in, 40);
		fseek(in, 0x10156, SEEK_SET);
		for(x=0;x<11;x++)
			U1_nextString(u1outstrings->S_spellchants[x], in, 40);
		fseek(in, 0x101ee, SEEK_SET);
		for(x=0;x<32;x++)
			U1_nextString(u1outstrings->S_towns[x], in, 40);
		for(x=0;x<8;x++)
			U1_nextString(u1outstrings->S_castles[x], in, 40);
		for(x=0;x<8;x++)
			U1_nextString(u1outstrings->S_shrines[x], in, 40);
		for(x=0;x<36;x++)
			U1_nextString(u1outstrings->S_dungeons[x], in, 40);
		fseek(in, 0x107a6, SEEK_SET);
		for(x=0;x<4;x++)
			U1_nextString(u1outstrings->S_lands[x], in, 40);
		fseek(in, 0x107fc, SEEK_SET);
		for(x=0;x<8;x++)
			U1_nextString(u1outstrings->S_ladiesnames[x], in, 40);
		fseek(in, 0x10d98, SEEK_SET);
		for(x=0;x<7;x++)
			U1_nextString(u1outstrings->S_collisions[x], in, 40);
		fseek(in, 0x10e3d, SEEK_SET);
			U1_nextString(u1outstrings->S_nsew[x], in, 40);
		fseek(in, 0x10ee0, SEEK_SET);
		for(x=0;x<23;x++)
			U1_nextString(u1outstrings->S_dungeonenemies[x], in, 40);
		fseek(in, 0x10fe3, SEEK_SET);
		for(x=0;x<11;x++)
		fseek(in, 0x1107a, SEEK_SET);
		for(x=0;x<2;x++)
			U1_nextString(u1outstrings->S_resurrect[x], in, 40);
		if(in)
			fclose(in);
	}
	else
		printf("Cannot open %s\n", tempfn);
	return 0;
}

int U1_clearMapBuf(int mapBuf[19][9])
{
	for(int x=0;x<19;x++)
		for(int y=0;y<9;y++)
			mapBuf[x][y]=-1;
	return 0;
}

int U1_peer(SDL_Surface *screen, TU1_tiles tiles, TU1_map map, TU1_game *game)
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
	SDL_Rect bufferrect;
	SDL_Rect rect;
	bufferrect.x=16;bufferrect.w=640-32;
	bufferrect.h=288;bufferrect.y=16; 				
	rect.x=0;rect.y=0;rect.w=bufferrect.w;rect.h=bufferrect.h;
	SDL_Surface *background=SDL_CreateRGBSurface(SDL_SWSURFACE, bufferrect.w, bufferrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &bufferrect, background, &rect);
	SDL_Surface *tinted=setBrightness(background, .2);				  
	SDL_FreeSurface(background);
	Uint8 R[100];
	Uint8 G[100];
	Uint8 B[100];
	for(int c=0;c<tiles.num;c++)
	{
		getAvgColor(tiles.tiles[c], &R[c], &G[c], &B[c]);
	}
	
	SDL_Surface *bigmap=SDL_CreateRGBSurface(SDL_SWSURFACE, 168, 156, 32, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
	for(int x=0;x<168;x++)
		for(int y=0;y<156;y++)
		{
			DrawPixel(bigmap, x, y, R[map[y][x]], G[map[y][x]], B[map[y][x]]);
		}	
	SDL_Surface *s=zoomSurface(bigmap, (256.-16)/156.0, (256.0-16)/156.0, 1);
	SDL_FreeSurface(bigmap);
  SDL_Rect n;
  n.x=(int)(640/2-((256-16)/156.0*168/2)+8);
  n.y=24;
  n.w=s->w;n.h=s->h;
  U1_addBorder(tinted, n.x-8-3, n.y-8-3, n.w+8+n.x+8-5, n.h+8+n.y+8-5, 8);
  SDL_BlitSurface(s, NULL, tinted, &n);
  SDL_BlitSurface(tinted, NULL, screen, &bufferrect);
  SDL_FreeSurface(tinted);
  SDL_FreeSurface(s);	
	return 0; 
}

int U1_drawMap(SDL_Surface *screen, TU1_tiles tiles, SDL_Surface *water, TU1_map map, int mapBuf[19][9], TU1_user *user)
{
	SDL_Surface *buffer;
	buffer=SDL_CreateRGBSurface(SDL_SWSURFACE, tiles.xdim*19, tiles.ydim*9, 32, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
	int mapBuf2[19][9];
	//map tile ID -> shape number conversion (with frame animation)
	for(int x=0;x<19;x++)
		for(int y=0;y<9;y++)
		{
			int yy=(y+user->y-4+156)%156;
			int xx=(x+user->x-9+168)%168;
			
			int tile=map[yy][xx];
			switch(tile)
			{
				case 4: tile=4+rand()%2;break;
				case 5: tile=6;break;
				case 6: tile=7+rand()%2;break;
				case 7: tile=9;break;
				default: break;
			}
			mapBuf2[x][y]=tile;
		}
	//draw
	SDL_Rect rect;
	rect.w=tiles.xdim;
	rect.h=tiles.ydim;
	for(int x=0;x<19;x++)
		for(int y=0;y<9;y++)
			{
				rect.x=x*tiles.xdim;
				rect.y=y*tiles.ydim;
				if(mapBuf2[x][y]==0)
				{
					mapBuf[x][y]=-1;
					if(SDL_BlitSurface(water, NULL, buffer, &rect))
						printf("\nError with blit in drawmap(1).\n");

				}
				else
				{
					mapBuf[x][y]=mapBuf2[x][y];
					if(SDL_BlitSurface(tiles.tiles[mapBuf2[x][y]], NULL, buffer, &rect))
						printf("\nError with blit in drawmap(2).\n");

				}
			}
	//Add map npc's to the map buffer
	for(int x=user->enemies;x>=0;x--)
	{
		if((user->npc[x].shape!=0)&&(user->npc[x].x>user->x-10)&&(user->npc[x].x<user->x+10)&&(user->npc[x].y>user->y-5)&&(user->npc[x].y<user->y+5))
		{
			int frame=0;
			if((user->npc[x].shape>18)||(user->npc[x].shape==14))
				frame=rand()%2;
			rect.x=(user->npc[x].x-user->x+9)*tiles.xdim;
			rect.y=(user->npc[x].y-user->y+4)*tiles.ydim;				
			mapBuf[user->npc[x].x-user->x+9][user->npc[x].y-user->y+4]=user->npc[x].shape+frame;
			if(SDL_BlitSurface(tiles.tiles[user->npc[x].shape+frame], NULL, buffer, &rect))
				printf("\nError with blit in drawmap(3).\n");
		}
	}
	if((tiles.xdim==16)&&(tiles.ydim==16))
	{
		SDL_Surface *buffer2=SDL_Scale_Super2xSai(buffer);
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

int tileScrollDownFrames(SDL_Surface *anim[64], int xdim, int ydim)
{
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
	SDL_Rect destrect;
	SDL_Surface *framet1=SDL_CreateRGBSurface(SDL_HWSURFACE, xdim, ydim-1, 32, rmask, gmask, bmask, amask);
	SDL_Surface *framet2=SDL_CreateRGBSurface(SDL_HWSURFACE, xdim, 1, 32, rmask, gmask, bmask, amask);	
	for(int x=0;x<ydim-1;x++)
	{
		destrect.x=0;destrect.w=xdim;
		destrect.y=-ydim+1;destrect.h=1;
		SDL_BlitSurface(anim[x], NULL, framet2, &destrect);
		destrect.y=0;destrect.h=ydim;
		SDL_BlitSurface(anim[x], NULL, framet1, &destrect);
		destrect.y=1;destrect.h=ydim-1;
		anim[x+1]=SDL_CreateRGBSurface(SDL_HWSURFACE, xdim, ydim, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(framet1, NULL, anim[x+1], &destrect);
		destrect.y=0;destrect.h=1;
		SDL_BlitSurface(framet2, NULL, anim[x+1], &destrect);
	}
	SDL_FreeSurface(framet1);
	SDL_FreeSurface(framet2);
	return 0;
}

int U1_isPassable(TU1_map map, int x, int y, int shape, int landing)
{
	y=(y+156)%156;
	x=(x+168)%168;
	int npcPassable[5][10]={ { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 },	//water
				{ 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 },	//land
				{ 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 },	//forest only
				{ 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },	//aircar
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };	//nothing
	if(landing)
	{
		npcPassable[3][0]=1;
		for(int x=0;x<5;x++)
			for(int y=4;y<=9;y++)
				npcPassable[x][y]=1;
	}
	int passableIdx=1;
	switch(shape)
	{
		case 13:
		case 14:
		case 15:
		case 19:
		case 20:
		case 21:
		case 22: 
		case 23:
		case 24: 
		case 25:
		case 26: passableIdx=0;break;
		case 16: passableIdx=3;break;
		case 31:
		case 32:
		case 35:
		case 36: passableIdx=2;break;
		default: passableIdx=1;break;
	}
	return npcPassable[passableIdx][map[y][x]];
}

int U1_enemyBlock(TU1_user *user, int x, int y)
{
	y=(y+156)%156;
	x=(x+168)%168;
	for(int z=0;z<=user->enemies;z++)
		if((user->npc[z].x==x)&&(user->npc[z].y==y))
			return z;
	return -1;
}

int U1_mapCollision(TU1_map map, TU1_user *user, int x, int y, int npc, char *blockage)
{
	y=(y+156)%156;
	x=(x+168)%168;
	int pass=U1_isPassable(map, x, y, user->npc[npc].shape, 0);
	blockage[0]=0;

	int z=U1_enemyBlock(user, x, y);
	if(z>=0)
		if((user->npc[z].x==x)&&(user->npc[z].y==y))
		{
			if(npc==0)
			{
				pass=2*(user->npc[z].shape>18);
				if(pass==2)
				{
					char enemy[15];
					U1_enemyName(enemy, user->npc[z].shape);
					sprintf(blockage, "Blocked by a %s\n", enemy);
				}
			}
			else
				pass=2;
		}
	if((pass==1)&&(npc==0))
	{
		switch(map[y][x])
		{
			case 0: if(user->npc[0].shape<13)
					sprintf(blockage, "You can't walk on water!\n");
				else
					sprintf(blockage, "Can't move on land!\n");
				break;
			case 1: if(user->npc[0].shape==13)
					sprintf(blockage, "Rafts like water!\n");	
				else
				if((user->npc[0].shape==14)||(user->npc[0].shape==15))
					sprintf(blockage, "Frigates like water!\n");
				else
					sprintf(blockage, "Can't move on land!\n");
				break;
			case 2: if(user->npc[0].shape==16)
					sprintf(blockage, "Aircars can't pass woods!\n");
				else
					sprintf(blockage, "Can't move on land!\n");
				break;
			case 3: if(user->npc[0].shape<17)
					sprintf(blockage, "Mountains are impassable!\n");
				else
					sprintf(blockage, "Can't move on land!\n"); 
				break;
			default:sprintf(blockage, "Can't move on land!\n");break;
		}
	}	
	return pass;
}

int U1_mapattack(SDL_Surface *screen, Tterminal *term, TU1_game *game, TU1_tiles tiles, TU1_map map)
{
	int tickupdate=1;
	char atxt[40];
	if((game->user.weaponready==4)||(game->user.weaponready==8)||(game->user.weaponready==9)||(game->user.weaponready==10))
	{
		sprintf(atxt, "%s %s?\n", "Attack with", game->out.S_weaponsshort[game->user.weaponready]);
		term->print(atxt);
		return 1;
	}
	sprintf(atxt,  "%s %s: ", "Attack with", game->out.S_weaponsshort[game->user.weaponready]);
	term->print(atxt);
	int k=term->waitKey();
	int x=0;
	int y=0;
	switch(k)
	{
		case SDLK_UP:
			y=-1; 
			term->print("North");
			break;
		case SDLK_DOWN:
			y=1;
			term->print("South");
			break;
		case SDLK_LEFT:
			x=-1;
			term->print("West");
			break;
		case SDLK_RIGHT:
			x=1;
			term->print("East");
			break;
		default:
			term->print("nothing");
	}
	term->print("\n");
	int found=0;
	int dis=1;
	switch(game->user.weaponready)
	{
		case 7:
		case 12:
		case 14:
		case 15: dis=4;break;
		default: dis=1;break;
	}
	if(x+y!=0)
	{
		int w=1;
		for(w=1;w<=dis;w++)
		{
			if(w>0)
			{
//eventually, optimize this part to be placed in the map drawing function
//need to fix bug where a shot is fired outside the map
			int scaler=1;	
			SDL_Rect rect;
				if(tiles.xdim==16)
					scaler=2;
				rect.x=16+tiles.xdim*scaler*(9+(w*x));
				rect.y=16+tiles.ydim*scaler*(4+(w*y));
				rect.w=tiles.xdim*scaler;
				rect.h=tiles.ydim*scaler;
				SDL_Surface *ts=NULL;
				if(scaler==2)
					ts=SDL_Scale_Super2xSai(tiles.tiles[50]);
				else
					ts=tiles.tiles[50];
				SDL_BlitSurface(ts, NULL, screen, &rect);
				refresh(screen, game->screen);
				if(scaler==2)
				{
					SDL_FreeSurface(ts); 
					ts=SDL_Scale_Super2xSai(tiles.tiles[map[game->user.y+w*y][game->user.x+w*x]]);
				}
				else
					ts=tiles.tiles[map[game->user.y+w*y][game->user.x+w*x]];
				SDL_Delay(10);
				SDL_BlitSurface(ts, NULL, screen, &rect);
				if(scaler==2)
					SDL_FreeSurface(ts);				
			}
			if(map[game->user.y+w*y][game->user.x+w*x]==3)
				break;
			for(int z=0;z<=game->user.enemies;z++)
				if((game->user.npc[z].x==w*x+game->user.x)&&(game->user.npc[z].y==w*y+game->user.y))
				{
				w=dis+1;
				if(game->user.npc[z].shape>18)
				{
					char enemy[15];
					U1_enemyName(enemy, game->user.npc[z].shape);
					char temp[40];
					int dmg=(int)((rnd(1)*(game->user.weaponready+1)*(game->user.agility-10)/10.0)+rnd(1)*game->user.strength/5);
					// hands:		2-10
					// dagger:	2-17
					// mace:		2-26
					// axe:			2-34
					if(dmg)
					{
						sprintf(temp, "Hit the %s! %d damage\n", enemy, dmg);
						term->print(temp);
						game->user.npc[z].hits-=dmg;
						if(game->user.npc[z].hits<=0)
						{
							int earned=int(rnd(1)*40);
							int gainex=int(rnd(1)*20)+2;
							game->user.exp+=gainex;
							game->user.coin+=earned;
							sprintf(temp, "Killed the %s!", enemy);
								term->print(temp);
							if(strlen(temp) >20)
							{
 								term->print("\n");
							}
							
							char temp2[80];
							if(earned%10!=0)
							{
								sprintf(temp2, " %d copper", earned%10);
								term->print(temp2);
							}
							if(strlen(temp)<=20)
								term->print("\n");
							if((earned/10)%10!=0)
							{
								sprintf(temp2, " %d silver", (earned/10)%10);
								term->print(temp2);
							}
							if(earned/100!=0)
							{
								sprintf(temp2, " %d gold", earned/100);
								term->print(temp2);
							}
							term->print("\n");
							//%d copper %d silver
							game->user.eraseNPC(z);
						}

					}
					found=1;
				}
			}
			}
		if(found==0)
			term->print(" Miss!\n");
	}
	return tickupdate;
}

int U1_mapcast(SDL_Surface *screen, Tterminal *term, TU1_game *game, TU1_map map, TU1_tiles tiles)
{
	term->print(game->out.S_spell[14]);
	term->print(game->out.S_spells[game->user.spellready]);
	switch(game->user.spellready)
	{
		case 0:	//prayer
					term->print("\n");
					term->print(" ");
					term->print(game->out.S_spellchants[game->user.spellready]);
					term->print("\n");
//					term->print(" ");
					if(0)
					{
						//cast prayer
						term->print("something\n");
					}
					else
					{
						term->print(game->out.S_spell[10]);
						term->print("\n");
						return 1;
					}
					break;
		case 1:	//Open
		case 2:	//Unlock			
		case 4:	//Steal
		case 5:	//Ladder down
		case 6:	//Ladder up
		case 7:	//Blink
		case 8: //Create
		case 9:	//Destroy
					if(game->user.spell[game->user.spellready]<=0) 
					{
					term->print("\n");
						term->print(" ");
						term->print(game->out.S_spell[15]);
						term->print("\n");
						return 1;					
					}
					game->user.spell[game->user.spellready]--;
					term->print("\n");
					term->print(" ");
					term->print(game->out.S_spell[16]);
					term->print("\n");
					return 1;
					break;
		case 3:	//Magic missile
		case 10:	//Kill
					if(game->user.spell[game->user.spellready]<=0) 
					{
						term->print("\n");
						term->print(" ");
						term->print(game->out.S_spell[15]);
						term->print("\n");
						return 1;					
					}
					game->user.spell[game->user.spellready]--;
					term->print(": ");
					break;
	}
	int k=term->waitKey();
	int x=0;
	int y=0;
	switch(k)
	{
		case SDLK_UP:
			y=-1; 
			term->print("North");
			break;
		case SDLK_DOWN:
			y=1;
			term->print("South");
			break;
		case SDLK_LEFT:
			x=-1;
			term->print("West");
			break;
		case SDLK_RIGHT:
			x=1;
			term->print("East");
			break;
		default:
			term->print("nothing");
	}
	term->print("\n");
	term->print(game->out.S_spellchants[game->user.spellready]);
	term->print("\n");
	int found=0;
	int dis=4;
	if(x+y!=0)
	{
		int w=1;
		for(w=1;w<=dis;w++)
		{
			if(w>0)
			{
//eventually, optimize this part to be placed in the map drawing function
//need to fix bug where a shot is fired outside the map
			int scaler=1;	
			SDL_Rect rect;
				if(tiles.xdim==16)
					scaler=2;
				rect.x=16+tiles.xdim*scaler*(9+(w*x));
				rect.y=16+tiles.ydim*scaler*(4+(w*y));
				rect.w=tiles.xdim*scaler;
				rect.h=tiles.ydim*scaler;
				SDL_Surface *ts=NULL;
				if(scaler==2)
					ts=SDL_Scale_Super2xSai(tiles.tiles[49]);
				else
					ts=tiles.tiles[49];
				SDL_BlitSurface(ts, NULL, screen, &rect);
				refresh(screen, game->screen);
				if(scaler==2)
				{
					SDL_FreeSurface(ts); 
					ts=SDL_Scale_Super2xSai(tiles.tiles[map[game->user.y+w*y][game->user.x+w*x]]);
				}
				else
					ts=tiles.tiles[map[game->user.y+w*y][game->user.x+w*x]];
				SDL_Delay(10);
				SDL_BlitSurface(ts, NULL, screen, &rect);
				if(scaler==2)
					SDL_FreeSurface(ts);				
			}
			if(map[game->user.y+w*y][game->user.x+w*x]==3)
				break;
			for(int z=0;z<=game->user.enemies;z++)
				if((game->user.npc[z].x==w*x+game->user.x)&&(game->user.npc[z].y==w*y+game->user.y))
				{
				w=dis+1;
				if(game->user.npc[z].shape>18)
				{
					char enemy[15];
					U1_enemyName(enemy, game->user.npc[z].shape);
					char temp[40];
					int dmg=(int)((rnd(1)*(game->user.spellready+1)*(game->user.intelligence-10)/10.0)+rnd(1)*game->user.strength/5);
					// hands:		2-10
					// dagger:	2-17
					// mace:		2-26
					// axe:			2-34
					if(dmg)
					{
						sprintf(temp, "Hit the %s! %d damage\n", enemy, dmg);
						term->print(temp);
						game->user.npc[z].hits-=dmg;
						if(game->user.npc[z].hits<=0)
						{
							int earned=int(rnd(1)*40);
							int gainex=int(rnd(1)*20)+2;
							game->user.exp+=gainex;
							game->user.coin+=earned;
							sprintf(temp, "Killed the %s!", enemy);
								term->print(temp);
							if(strlen(temp) >20)
							{
 								term->print("\n");
							}
							
							char temp2[80];
							if(earned%10!=0)
							{
								sprintf(temp2, " %d copper", earned%10);
								term->print(temp2);
							}
							if(strlen(temp)<=20)
								term->print("\n");
							if((earned/10)%10!=0)
							{
								sprintf(temp2, " %d silver", (earned/10)%10);
								term->print(temp2);
							}
							if(earned/100!=0)
							{
								sprintf(temp2, " %d gold", earned/100);
								term->print(temp2);
							}
							term->print("\n");
							//%d copper %d silver
							game->user.eraseNPC(z);
						}

					}
					found=1;
				}
			}
			}
		if(found==0)
			term->print(" Miss!\n");
	}
	return 1;
}

int U1_mapfire(SDL_Surface *screen, Tterminal *term, TU1_game *game, TU1_map map, TU1_tiles tiles)
{
	term->print(game->out.S_fire[1]);
	switch(game->user.craftready)
	{
		case 4:	//frigate
			term->print(game->out.S_fire[3]);
			break;
		case 5:	//aircar
			term->print(game->out.S_fire[4]);
			break;
		default:
			term->print(game->out.S_fire[2]);
			term->print("\n");
			return 1;
			break;
	}
	int k=term->waitKey();
	int x=0;
	int y=0;
	switch(k)
	{
		case SDLK_UP:
			y=-1; 
			term->print("North");
			break;
		case SDLK_DOWN:
			y=1;
			term->print("South");
			break;
		case SDLK_LEFT:
			x=-1;
			term->print("West");
			break;
		case SDLK_RIGHT:
			x=1;
			term->print("East");
			break;
		default:
			term->print("nothing");
	}
	term->print("\n");
	int found=0;
	int dis=4;
	if(x+y!=0)
	{
		int w=1;
		for(w=1;w<=dis;w++)
		{
			if(w>0)
			{
//eventually, optimize this part to be placed in the map drawing function
//need to fix bug where a shot is fired outside the map
			int scaler=1;	
			SDL_Rect rect;
				if(tiles.xdim==16)
					scaler=2;
				rect.x=16+tiles.xdim*scaler*(9+(w*x));
				rect.y=16+tiles.ydim*scaler*(4+(w*y));
				rect.w=tiles.xdim*scaler;
				rect.h=tiles.ydim*scaler;
				SDL_Surface *ts=NULL;
				if(scaler==2)
					ts=SDL_Scale_Super2xSai(tiles.tiles[50]);
				else
					ts=tiles.tiles[50];
				SDL_BlitSurface(ts, NULL, screen, &rect);
				refresh(screen, game->screen);
				if(scaler==2)
				{
					SDL_FreeSurface(ts); 
					ts=SDL_Scale_Super2xSai(tiles.tiles[map[game->user.y+w*y][game->user.x+w*x]]);
				}
				else
					ts=tiles.tiles[map[game->user.y+w*y][game->user.x+w*x]];
				SDL_Delay(10);
				SDL_BlitSurface(ts, NULL, screen, &rect);
				if(scaler==2)
					SDL_FreeSurface(ts);				
			}
			if(map[game->user.y+w*y][game->user.x+w*x]==3)
				break;
			for(int z=0;z<=game->user.enemies;z++)
				if((game->user.npc[z].x==w*x+game->user.x)&&(game->user.npc[z].y==w*y+game->user.y))
				{
				w=dis+1;
				if(game->user.npc[z].shape>18)
				{
					char enemy[15];
					U1_enemyName(enemy, game->user.npc[z].shape);
					char temp[40];
					int dmg=(int)((rnd(1)*(game->user.craftready+1)*(game->user.agility-10)/10.0)+rnd(1)*game->user.strength/5);
					// hands:		2-10
					// dagger:	2-17
					// mace:		2-26
					// axe:			2-34
					if(dmg)
					{
						sprintf(temp, "Hit the %s! %d damage\n", enemy, dmg);
						term->print(temp);
						game->user.npc[z].hits-=dmg;
						if(game->user.npc[z].hits<=0)
						{
							int earned=int(rnd(1)*40);
							int gainex=int(rnd(1)*20)+2;
							game->user.exp+=gainex;
							game->user.coin+=earned;
							sprintf(temp, "Killed the %s!", enemy);
								term->print(temp);
							if(strlen(temp) >20)
							{
 								term->print("\n");
							}
							
							char temp2[80];
							if(earned%10!=0)
							{
								sprintf(temp2, " %d copper", earned%10);
								term->print(temp2);
							}
							if(strlen(temp)<=20)
								term->print("\n");
							if((earned/10)%10!=0)
							{
								sprintf(temp2, " %d silver", (earned/10)%10);
								term->print(temp2);
							}
							if(earned/100!=0)
							{
								sprintf(temp2, " %d gold", earned/100);
								term->print(temp2);
							}
							term->print("\n");
							//%d copper %d silver
							game->user.eraseNPC(z);
						}

					}
					found=1;
				}
			}
			}
		if(found==0)
			term->print(" Miss!\n");
	}
	return 1;
}

int U1_mapenter(SDL_Surface *screen, Tterminal *term, TU1_game *game, TU1_map map, int mapBuf[19][9], Mix_Music *music)
{
	int looping=-1;
	int tickupdate=1;
	if(game->user.craftready)
	{
		term->print("You must X-it first!\n");				
		return tickupdate;
	}
	switch(map[game->user.y][game->user.x])
	{
		case 4: Mix_FadeOutMusic(500);
			tickupdate=0;
			if(U1_navigateTown(screen, term, game, map) == -1)
				return -1;
			U1_clearMapBuf(mapBuf);
			if(game->config.game[1].music)
				Mix_FadeInMusic(music,looping,500);
			break;
		case 5: 
				Mix_FadeOutMusic(500);
				if(U1_navigateSignpost(screen, term, game)== -1)
					return -1;
				U1_clearMapBuf(mapBuf);
				if(game->config.game[1].music)
					Mix_FadeInMusic(music,looping,500);
				break;
		case 6: Mix_FadeOutMusic(500);
			tickupdate=0;
			if(U1_navigateTown(screen, term, game, map)==-1)
				return -1;
			U1_clearMapBuf(mapBuf);
			if(game->config.game[1].music)
				Mix_FadeInMusic(music,looping,500);
			break;
		case 7:	
			Mix_FadeOutMusic(500);
			if(U1_navigateDungeon(screen, term, game)== -1)
				return -1;
			U1_clearMapBuf(mapBuf);
			if(game->config.game[1].music)
				Mix_FadeInMusic(music,looping,500);
			break;
		default: term->print("Enter?\n");break;
	}
	return tickupdate;
}

int U1_newNPC(TU1_user *user, TU1_map map)
{
	int x=0;
	int y=0;
	int zx=0;
	int zy=0;
	int shape=0;
	int ok=0;
	while(ok==0)
	{
		x=int(rnd(20)-10);
		y=int(rnd(20)-10);
		while((zx==0)&&(zy==0))
		{
			zx=int(rnd(2));
			zy=int(rnd(2));
		}
		x=(int)(user->x+zx*sgn(x)*10+x);
		y=(int)(user->x+zy*sgn(y)*5+y);
		if(map[y][x]!=3)
			ok=1;
		if(map[y][x]==0)
			shape=(int)(19+2*(int)rnd(4));
		else
			shape=(int)(27+2*(int)rnd(11));
		if((shape==31)||(shape==35))
			if(map[y][x]!=2)
				ok=0;
	}
	//check for valid shape per terrain;
	user->newNPC(shape, x, y);
	return 0;
}

int U1_enemymove(SDL_Surface *screen, TU1_game *game, TU1_map map, int lastmove[40], Tterminal *term, TU1_tiles tiles, int emove)
{
	int attacked=0;
		for(int z=1;z<=game->user.enemies;z++)
			if(game->user.npc[z].shape>18)
			{			
				int dis=1;
				switch(game->user.npc[z].shape)
				{
					case 23:
					case 25:
					case 31:
					case 47:
									dis=3;break;
					default: dis=1;break;
				}
				int dx=0;
				int dy=0;
				int inrange=(abs(game->user.x-game->user.npc[z].x)<=dis)&&(abs(game->user.y-game->user.npc[z].y)<=dis);
				if(inrange)
				{
					attacked=1;
					int xx=0;
					int yy=0;
					if(game->user.x>game->user.npc[z].x)
						xx=1;
					if(game->user.x<game->user.npc[z].x)
						xx=-1;
					if(game->user.y>game->user.npc[z].y)
						yy=1;
					if(game->user.y<game->user.npc[z].y)
						yy=-1;
					int hit=0;
					char tempstr[80];
          char enemy[15];
          U1_enemyName(enemy, game->user.npc[z].shape);
                    
					sprintf(tempstr, " %s attacks!\n", enemy);
 					term->print(tempstr);
					for(int zz=1;zz<=dis;zz++)
					{
						if((game->user.npc[z].x+zz*xx==game->user.x)&&(game->user.npc[z].y+zz*yy==game->user.y))
						{
							hit=(int)(rnd(1)*21-rnd(1)*game->user.agility);
						}

						if(zz>0)
	 					{
	 					//eventually, optimize this part to be placed in the map drawing function
	 					//need to fix bug where a shot is fired outside the map
							int scaler=1;	
							SDL_Rect rect;
							if(tiles.xdim==16)
								scaler=2;
							rect.x=16+tiles.xdim*scaler*(9+(zz*xx)+(game->user.npc[z].x-game->user.x));
							rect.y=16+tiles.ydim*scaler*(4+(zz*yy)+(game->user.npc[z].y-game->user.y));
							rect.w=tiles.xdim*scaler;
							rect.h=tiles.ydim*scaler;
							SDL_Surface *ts=NULL;
							if(scaler==2)
								ts=SDL_Scale_Super2xSai(tiles.tiles[50]);
							else
								ts=tiles.tiles[50];
							SDL_BlitSurface(ts, NULL, screen, &rect);
							refresh(screen, game->screen);
							if(scaler==2)
							{
								SDL_FreeSurface(ts); 
								ts=SDL_Scale_Super2xSai(tiles.tiles[map[game->user.npc[z].y+zz*yy][game->user.npc[z].x+zz*xx]]);
							}
							else
								ts=tiles.tiles[map[game->user.npc[z].y+zz*yy][game->user.npc[z].x+zz*xx]];
							SDL_Delay(10);
							SDL_BlitSurface(ts, NULL, screen, &rect);
							if(scaler==2)
								SDL_FreeSurface(ts);				
						}
						
					}					
					if(hit<=0)
						term->print("Missed!\n");
					else
					{
						term->print("Hit!\n");
						sprintf(tempstr, "%d damage\n", hit);
						game->user.hits-=hit;
						term->print(tempstr);
					}

				}
				else
				{
					if(game->user.moves%emove==0)
					{
	 					if(lastmove[z])
						{
							if(game->user.npc[z].x<game->user.x)
							{
								dx=1;
								lastmove[z]=0;
							}
							if(game->user.npc[z].x>game->user.x)
							{
								dx=-1;
							lastmove[z]=0;	
							}
						}
						else
						{
							if(game->user.npc[z].y<game->user.y)
							{
								dy=1;
								lastmove[z]=1;
							}
							if(game->user.npc[z].y>game->user.y)
							{
								dy=-1;
								lastmove[z]=1;
							}
						}	
					
						if(game->user.npc[z].y==game->user.y)
							lastmove[z]=1;
						if(game->user.npc[z].x==game->user.x)
							lastmove[z]=0;
						char blockage[40];	
						if(U1_mapCollision(map, &game->user, game->user.npc[z].x+dx, game->user.npc[z].y+dy, z, blockage)==0)
						{
							game->user.npc[z].x=game->user.npc[z].x+dx;
							game->user.npc[z].y=game->user.npc[z].y+dy;
						}
					}
				}
			}
	if(attacked)
		U1_printWithTick(term, "");		
	return 0;
}

int U1_navigateMap(SDL_Surface *screen, Tterminal *term, TU1_game *game, TU1_map map)
{
	int foodconsume=2;
	int enemymove=1;
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
	U1_printWithTick(term, "");
	SDL_EnableKeyRepeat(50, 0);
	int mapBuf[19][9];
	U1_clearMapBuf(mapBuf);
	TU1_tiles tiles;
	if(U1_loadtiles (&game->config, &tiles, "tiles", game->config.game[1].svga))
		return -1;
	SDL_Surface *water[64];
	water[0]=SDL_CreateRGBSurface(SDL_SWSURFACE, tiles.tiles[0]->w, tiles.tiles[0]->h, 32, rmask, gmask, bmask, amask*0);
	SDL_BlitSurface(tiles.tiles[0], NULL, water[0], NULL);
	tileScrollDownFrames(water, tiles.xdim, tiles.ydim);
	int frame=0;
	int quit=0;
	int go=0;
	int lastmove[40];
	int collision=0;
	int tickupdate=1;
	int k=0;
	int looping=-1;
	Mix_Music *music = NULL;	
	while(quit==0)
	{
		if((game->config.game[1].music )&&(music==NULL))
		{
			char tempfn[512];
			sprintf(tempfn, "%s/u1/midi/wander.mid", game->config.datapath);
			music = Mix_LoadMUS(tempfn);	
			Mix_FadeInMusic(music,looping,500);
		}
		int zz=game->user.npc[0].shape-10;
		if(zz>4)
			zz--;
		foodconsume=U1_craftSteps(zz);
		enemymove=(U1_craftSteps(zz)+1)/2;	
		k=term->getKey();
		if(k)
		{
			char blockage[40];
			blockage[0]=0;
			switch(k)
			{
				case SDL_QUIT:	return -1;break;
				case SDLK_LEFT:	collision=U1_mapCollision(map, &game->user, game->user.x-1, game->user.y, 0, blockage);
						go=1;
						if(collision==0)
						{
							game->user.x--;
							game->user.npc[0].x=game->user.x;
							term->print("West\n");
						}
						break;
				case SDLK_RIGHT:collision=U1_mapCollision(map, &game->user, game->user.x+1, game->user.y, 0, blockage); 
						go=1;
						if(collision==0)
						{
							game->user.x++;
							game->user.npc[0].x=game->user.x;
							term->print("East\n");
						}
						break;		
				case SDLK_UP:	collision=U1_mapCollision(map, &game->user, game->user.x, game->user.y-1, 0, blockage);
						go=1;
						if(collision==0)
						{
							game->user.y--;
							game->user.npc[0].y=game->user.y;
							term->print("North\n");
						}	
						break;		
				case SDLK_DOWN:	collision=U1_mapCollision(map, &game->user, game->user.x, game->user.y+1, 0, blockage);
						go=1;
						if(collision==0)
						{
							game->user.y++;
							game->user.npc[0].y=game->user.y;
							term->print("South\n");
						}
						break;	
				case SDLK_SPACE:{
							go=1;
							term->print("Pass\n");
						}
						break;
				case SDLK_a:	{
							tickupdate=U1_mapattack(screen, term, game, tiles, map);
							go=1;
						}
						break;
				case SDLK_b:	{
							go=1;
							if(game->user.craftready)
							{
								term->print("X-it thy craft first!\n");
								
								break;
							}
							
							int changed=0;
							for(int x=1;x<=game->user.enemies;x++)
							{
								if((game->user.x==game->user.npc[x].x)&&(game->user.y==game->user.npc[x].y)&&(changed==0))
								{
									if((game->user.npc[x].shape>10)&&(game->user.npc[x].shape<19))
									{
										game->user.npc[0].shape=game->user.npc[x].shape;
										int zz=game->user.npc[x].shape-10;
										if(zz>4)
											zz--;
										game->user.craftready=zz;
										if(zz<=7)
										{
											term->print("Board ");
											changed=1;
											char vehicle[20];
											U1_craftName(vehicle, zz);
											term->print(vehicle);
											term->print("\n");
											game->user.eraseNPC(x);
										}
										else
											term->print("Nothing to board!\n");
									}
								}
							}
							if(changed==0)
							{
								term->print("Nothing to board!\n");	
							}
						}
						break;
				case SDLK_c:	{
							tickupdate=U1_mapcast(screen, term, game, map, tiles);
							go=1;
						}
						break;						
				case SDLK_d:	{
							tickupdate=1;
							term->print("Drop?\n");
							go=1;
						}
						break;
				case SDLK_e:	{
							tickupdate=U1_mapenter(screen, term, game, map, mapBuf, music);
							go=1;
						}
						break;
				case SDLK_f:	{
							tickupdate=U1_mapfire(screen, term, game, map, tiles);
							go=1;
						}
						break;						
				case SDLK_g:	{
							tickupdate=1;
							term->print("Get?\n");
							go=1;
						}
						break;
				case SDLK_h:	{
							tickupdate=1;
							term->print("Hyperjump?\n");
							go=1;
						}
						break;
				case SDLK_i:	{
							tickupdate=1;
							U1_inform(screen, game, term, map);
							go=1;
						}
						break;				
				case SDLK_k:	{
							tickupdate=1;
							term->print("K-Limb?\n");
							go=1;
						}
						break;
				case SDLK_o:	{
							tickupdate=1;
							term->print("Open?\n");
							go=1;
						}
						break;
				case SDLK_p:	{
							tickupdate=1;
							go=1;
							U1_peer(screen, tiles, map, game);
							term->print("Peer into gem ");
							term->waitKey();							 
							term->print("\n");
						}
						break;
				case SDLK_q:	quit=1;break;
				case SDLK_r:	{
							tickupdate=U1_ready(screen, term, game);
							go=1;
						}
						break;
				case SDLK_s:	{
							tickupdate=1;
							term->print("Steal?\n");
							go=1;
						}
						break;
				case SDLK_t:	{
							tickupdate=1;
							term->print("Transact?\n");
							go=1;
						}
						break;
				case SDLK_u:	{
							tickupdate=1;
							term->print("Unlock?\n");
							go=1;
						}
						break;
				case SDLK_v:	{
							tickupdate=1;
							term->print("View?\n");
							go=1;
						}
						break;
				case SDLK_x:	{
							go=1;
							if(map[game->user.y][game->user.x]>3)
							{
								term->print("Can't X-it here!\n");
								
								break;
							}
							term->print("X-it");
							if(game->user.npc[0].shape==10)
							{
								if(game->user.craftready==0)
									term->print(" what?\n");
								else
									term->print(" ERROR IN SAVEGAME!\n");
							}
							else
							{
								game->user.newNPC(game->user.npc[0].shape, game->user.x, game->user.y);
								term->print("\n");
								game->user.npc[0].shape=10;
								game->user.craftready=0;
							}
						}
						break; 
				case SDLK_z:	{
							term->print("Ztats\n");
							if(U1_ztats(screen, term, game)==SDL_QUIT)
								return -1;
							go=1;
						}
						break;

				case SDLK_F11:	if(game->config.game[1].svga)
						{
							game->config.game[1].svga=0;
							term->print("OLD GRAPHICS\n");
						}
						else
						{
							game->config.game[1].svga=1;
							term->print("NEW GRAPHICS\n");
						}
						go=1;
						for(int x=0;x<tiles.num;x++)
							if(tiles.tiles[x])
								SDL_FreeSurface(tiles.tiles[x]);
						if(U1_loadtiles(&game->config, &tiles, "tiles", game->config.game[1].svga))	//w00t
							return -1;
						for(int x=0;x<32;x++)
							if(water[x])
							{
								SDL_FreeSurface(water[x]);
								water[x]=NULL;
							}
						water[0]=SDL_CreateRGBSurface(SDL_SWSURFACE, tiles.tiles[0]->w, tiles.tiles[0]->h, 32, rmask, gmask, bmask, amask*0);
						SDL_BlitSurface(tiles.tiles[0], NULL, water[0], NULL);
						frame=0;
						tileScrollDownFrames(water, tiles.xdim, tiles.ydim);
						break;
				case SDLK_n:
				case SDLK_F12:	if(game->config.game[1].music)
						{
							go=1;
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
							go=1;
							game->config.game[1].music=1;
							char tempfn[512];
							sprintf(tempfn, "%s/u1/midi/wander.mid", game->config.datapath);
							music = Mix_LoadMUS(tempfn);	
							Mix_FadeInMusic(music,looping,500);
							char temp[256];
							sprintf(temp, "%s%s\n", game->out.S_noise[0], game->out.S_noise[1]);
							term->print(temp);
						}
						break;

				default:	{
							term->print("Huh?\n");
							go=1;
							break;
						}
			}
			if((k)&&(blockage[0]))
			{
				term->print(blockage);
				
			}
			blockage[0]=0;
		}
		game->user.y=(game->user.y+156)%156;
		game->user.x=(game->user.x+168)%168;
		game->user.npc[0].x=game->user.x;
		game->user.npc[0].y=game->user.y;
		if(go)
		{
			if(tickupdate)
				U1_printWithTick(term, "");
			game->user.moves++;
			tickupdate=1;
			go=0;
			if((int)(rnd(1)*40)==1)
				U1_newNPC(&game->user, map);
			U1_enemymove(screen, game, map, lastmove, term, tiles, enemymove);
			if(game->user.food>0)
				if(game->user.moves%foodconsume==0)		
					game->user.food--;
			if(game->user.food<=0)
				game->user.hits=0;
			if(game->user.hits<=0)
			{
				Mix_FadeOutMusic(500);
				                                              

				game->user.food=0;
				game->user.coin=0;
				game->user.hits=0;
				U1_stats(screen, term->font, &game->user);
				//tint screen
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
				SDL_Rect bufferrect;
				SDL_Rect rect;
				bufferrect.x=16;bufferrect.w=640-32;
				bufferrect.h=18*16;bufferrect.y=16; 				
				rect.x=0;rect.y=0;rect.w=bufferrect.w;rect.h=bufferrect.h;
				SDL_Surface *background=SDL_CreateRGBSurface(SDL_SWSURFACE, bufferrect.w, bufferrect.h, 32, rmask, gmask, bmask, amask);
				SDL_BlitSurface(screen, &bufferrect, background, &rect);
				SDL_Surface *tinted=setBrightness(background, .2);				  
				//blit skull
				SDL_Surface *skull=NULL;
				if(tiles.xdim==16)
				{
					char str[80];
					sprintf(str, "%s/u1/u1skull_m.png", game->config.datapath);
					SDL_Surface *skull2=IMG_Load(str);
					skull=SDL_Scale_2xSai(skull2);
				}
				else
				{
					char str[80];
					sprintf(str, "%s/u1/u1skull_c.png", game->config.datapath);
					skull=IMG_Load(str);
				}
				SDL_Rect r;
				r.x=tinted->w/2-skull->w/2;
				r.y=tinted->h/2-skull->h/2;
				r.w=skull->w;
				r.h=skull->h;
				SDL_BlitSurface(skull, NULL, tinted, &r);
				SDL_BlitSurface(tinted, &rect, screen, &bufferrect);
				refresh(screen, game->screen);
				SDL_FreeSurface(tinted);
				SDL_FreeSurface(skull);
				SDL_FreeSurface(background);
				
				term->dropLine();
				char t[80];
				sprintf(t, " %s, thou art dead\n", game->user.name);
				term->print(t);term->blit();
				SDL_Delay(5000);
				term->print(" Attempting resurrection!\n");term->blit();
				SDL_Delay(1000);
				for(int z=0;z<99;z++)
				{
					game->user.hits++;
					U1_stats(screen, term->font, &game->user);
					refresh(screen, game->screen);
					SDL_Delay(25);
				}
				for(int z=0;z<99;z++)
				{
					game->user.food++;
					U1_stats(screen, term->font, &game->user);
					refresh(screen, game->screen);
					SDL_Delay(25);
				}
				U1_printWithTick(term, "");
				
				//move user 10 random spaces
				int nx=0;
				int ny=0;
				while(U1_mapCollision(map, &game->user, nx, ny, 0, t))
				{
					nx=(int)(rnd(1)*20-10+game->user.x);
					ny=(int)(rnd(1)*20-10+game->user.y);
				}
				game->user.x=nx;
				game->user.npc[0].x=nx;
				game->user.y=ny;
				game->user.npc[0].y=ny;
				//remove all map npc's				
				//if on a craft, lose it
				for(int z=game->user.enemies;z>0;z--)	//npc0 is the player 
				{
					if(game->user.npc[z].shape>18)
					{
						game->user.eraseNPC(z);
					}
				}
				game->user.craft[game->user.craftready]--;
 				game->user.craftready=0;
				game->user.npc[0].shape=10;
				//ready hands, skin, prayer
				game->user.weaponready=0;
				game->user.armorready=0;
				game->user.spellready=0;
				
				if(game->config.game[1].music)
					Mix_FadeInMusic(music,looping,500);
			}

		}
		U1_drawMap(screen, tiles, water[frame], map, mapBuf, &game->user);
//char crdnt[80];
//sprintf(crdnt, "(0x%0xh, 0x%0xh) (%d, %d)\n", game->user.x, game->user.y, game->user.x, game->user.y);
//output(screen, term->font, 1, 1, crdnt, 192,0,0); 
		term->blit();
		U1_stats(screen, term->font, &game->user);
		refresh(screen, game->screen);
		SDL_Delay(20);
		frame++;
		frame=frame%water[0]->h;
		term->clearMouseEvents();
	}
	if( Mix_PlayingMusic() )
	{
		Mix_FadeOutMusic(500);
//		SDL_Delay(150);
	}
	if ( music ) 
	{
		Mix_FreeMusic(music);
		music = NULL;
	}


	//free memory for map tiles
	for(int x=0;x<tiles.num;x++)
		SDL_FreeSurface(tiles.tiles[x]);
	//free memory for water animation
	for(int x=0;x<32;x++)
		if(water[x])
			 SDL_FreeSurface(water[x]);
	return 0;
}

int U1_out(SDL_Surface *screen, Tterminal *term, TU1_game *game)
{
	TU1_map map;
	U1_loadmap(&game->config, map);
//	game->screen=screen;
	U1_getOutEXE_Strings(&game->config, &game->out);
//	printf("navigatemap\n");
	int retval=U1_navigateMap(screen, term, game, map);
	return retval;
}

