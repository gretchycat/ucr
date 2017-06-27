#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include "../SDL_rotozoom.h"
#include "../SDL_prim.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "../SDL_smoothzoom.h"
#include "../2xsai.h"
#include "../applebasic.h"
#include "u0main.h"
#include "u0dungeon.h"
#include "u0images.h"
#include "pixmaps/u0font.xpm"

int U0_main(SDL_Surface *screen, int audio_open)
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

	U0_player player;
	Tfont font(u0font_xpm);
	player.SCREEN=screen;
	screen=SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 400, 32, rmask, gmask, bmask, amask);
	for(int x=0;x<25;x++)
		PRINT(screen, &font, 0, x, " Please wait while images are loaded...", 16,16,16);
	PRINT(screen, &font, 0, 11, " Please wait while images are loaded...", 255, 64, 64);
	refresh(screen, player.SCREEN);

	u0loadimages(&player);
	player.pn[0]=0;
	player.FRAMENUM=1;
	player.DIRECTION=1;
	player.AUDIOSTATUS=0;
	player.u0music = NULL;	
	player.AUDIOSTATUS=audio_open;
	int resurrected=1;
	while(resurrected==1)
	{
		if(player.config.game[0].music)
		{
			char temp[600];
			sprintf(temp, "%s/u0/midi/create.mid", player.config.datapath);
			player.u0music = Mix_LoadMUS(temp);
		}
		int looping=-1;
		if(player.u0music)
			Mix_FadeInMusic(player.u0music,looping,500);

		u0clear(screen, &player);

		if(player.config.game[0].svga)
		{
			SDL_BlitSurface(player.u0background, NULL, screen, NULL);
		}
		Tterminal term(screen, 0, (int)((screen->h/192.0)*150+15), 30, 4, 192, 192, 192, u0font_xpm);
		term.SCREEN=player.SCREEN;
		U0_chargen(screen, &player);
		u0clear(screen, &player);
		refresh(screen, player.SCREEN);
		player.zz=rnd(-abs(player.ln));
		player.level=0;
		PRINT(screen, &font, 0, 11, " WELCOME TO AKALABETH, WORLD OF DOOM!", 192, 192, 192);
		for(int x=0;x<=20;x++)
		{
			player.te[x][0]=1;
			player.te[0][x]=1;
			player.te[x][20]=1;
			player.te[20][x]=1;
		}
		PRINT(screen, &font, 0, 22, "  (PLEASE WAIT)", 192, 192, 192);
		int xpos=15;
		int ypos=22;
		for(int x=1;x<=19;x++)
		{
			for(int y=1;y<=19;y++)
			{
				player.te[x][y]=(int)(pow(rnd(1.0), 5)*4.5);
				if((player.te[x][y]==3)&&(rnd(1.0)>.50))
					player.te[x][y]=0;
			}
			PRINT(screen, &font, xpos, ypos, ".", 192, 192, 192);
			refresh(screen, player.SCREEN);
			xpos++;
			if(xpos>39)
			{
				xpos=0;
				ypos++;
			}
		}
		player.te[(int)(rnd(1.0)*19+1)][(int)(rnd(1.0)*19+1)]=5;
		player.tx=(int)(rnd(1.0)*19+1);
		player.ty=(int)(rnd(1.0)*19+1);
		player.te[player.tx][player.ty]=3;
		player.xx[0]=139;
		player.yy[0]=79;
		for(int x=2;x<=20;x+=2)
		{
			player.xx[x/2]=(int)(atn(1.0/x)/atn(1.0)*140+.5);
			player.yy[x/2]=(int)(player.xx[x/2]*4/7.0);
			player.per[x/2][0]=139-player.xx[x/2];
			player.per[x/2][1]=139+player.xx[x/2];
			player.per[x/2][2]=79-player.yy[x/2];
			player.per[x/2][3]=79+player.yy[x/2];
		}
		player.per[0][0]=0;
		player.per[0][1]=279;
		player.per[0][2]=0;
		player.per[0][3]=159; 
		for(int x=1;x<=10;x++)
		{
			player.cd[x][0]=139-player.xx[x]/3.0;
			player.cd[x][1]=139+player.xx[x]/3.0;
			player.cd[x][2]=79-player.yy[x]*.7;
			player.cd[x][3]=79+player.yy[x];
		}
		PRINT(screen, &font, xpos, ypos, ".", 192, 192, 192);
		refresh(screen, player.SCREEN);
		for(int x=0;x<=9;x++)
		{
			player.ld[x][0]=(player.per[x][0]*2+player.per[x+1][0])/3.0;
			player.ld[x][1]=(player.per[x][0]+2*player.per[x+1][0])/3.0;
			player.wI=player.ld[x][0]-player.per[x][0];
			player.ld[x][2]=player.per[x][2]+player.wI*4/7.0;
			player.ld[x][3]=player.per[x][2]+2*player.wI*4/7.0;
			player.ld[x][4]=(player.per[x][3]*2+player.per[x+1][3])/3.0;
			player.ld[x][5]=(player.per[x][3]+2*player.per[x+1][3])/3.0;
			player.ld[x][2]=player.ld[x][4]-(player.ld[x][4]-player.ld[x][2])*.8;
			player.ld[x][3]=player.ld[x][5]-(player.ld[x][5]-player.ld[x][3])*.8;
			if(player.ld[x][3]==player.ld[x][4])
				player.ld[x][3]=player.ld[x][3]-1;
		}
		for(int x=0;x<=9;x++)
		{
			player.ft[x][0]=139-player.xx[x]/3.0;
			player.ft[x][1]=139+player.xx[x]/3.0;
			player.ft[x][2]=139-player.xx[x+1]/3.0;
			player.ft[x][3]=139+player.xx[x+1]/3.0;
			player.ft[x][4]=79+(player.yy[x]*2+player.yy[x+1])/3.0;
			player.ft[x][5]=79+(player.yy[x]+2*player.yy[x+1])/3.0;
		}
		for(int x=0;x<=9;x++)
		{
			Mix_FadeOutMusic(1);
			player.lad[x][0]=(player.ft[x][0]*2+player.ft[x][1])/3.0;
			player.lad[x][1]=(player.ft[x][0]+2*player.ft[x][1])/3.0;
			player.lad[x][3]=player.ft[x][4];
			player.lad[x][2]=159-player.lad[x][3];
		}
		player.curcolor=SDL_MapRGBA(screen->format, 192, 192, 192, 255);
		u0clear(screen, &player);
		if ( player.u0music )
		{
			Mix_FadeOutMusic(1);
			Mix_FreeMusic(player.u0music);
			player.u0music = NULL;
		}
		U0_drawMap(screen, &player);
		char tempstr[10];
		sprintf(tempstr, "FOOD=%0.1f    ", player.pw[0]);
		PRINT(screen, &font, 30, 21, tempstr, 192, 192, 192);
		sprintf(tempstr, "H.P.=%d    ", player.c[0]);
		PRINT(screen, &font, 30, 22, tempstr, 192, 192, 192);
		sprintf(tempstr, "GOLD=%d    ", player.c[5]);
		PRINT(screen, &font, 30, 23, tempstr, 192, 192, 192);

		resurrected=U0_mainLoop(screen, &player, &term);
	}
	u0unloadimages(&player);	
	SDL_FreeSurface(screen);
	return 0;
}

int U0_drawPlayer(SDL_Surface *screen, int x1, int y1, Uint32 curcolor, U0_player *player)
{
	if(player->config.game[0].svga==0)
	{
		hplot(screen, 138, 75, 142, 75, curcolor);
		hplot(screen, 140, 73, 140, 77, curcolor);
	}
	else
	{
		SDL_Rect destrect;
		destrect.x=(int)(115*screen->w/280.0);
		destrect.y=(int)(50*screen->h/192.0);
		destrect.w=115;
		destrect.h=105;
		SDL_Surface *tmpsurface=NULL;
		switch(player->DIRECTION)
		{
			case 0:
				switch(player->FRAMENUM)
				{
					case 0:	tmpsurface=player->u0walku1;player->FRAMENUM++;break;
					case 1:	tmpsurface=player->u0walku2;player->FRAMENUM++;break;
					case 2:	tmpsurface=player->u0walku3;player->FRAMENUM++;break;
					case 3:	tmpsurface=player->u0walku2;player->FRAMENUM=0;break;
				};break;
			case 1:
				switch(player->FRAMENUM)
				{
					case 0:	tmpsurface=player->u0walkd1;player->FRAMENUM++;break;
					case 1:	tmpsurface=player->u0walkd2;player->FRAMENUM++;break;
					case 2:	tmpsurface=player->u0walkd3;player->FRAMENUM++;break;
					case 3:	tmpsurface=player->u0walkd2;player->FRAMENUM=0;break;
				};break;
			case 2:
				switch(player->FRAMENUM)
				{
					case 0:	tmpsurface=player->u0walkl1;player->FRAMENUM++;break;
					case 1:	tmpsurface=player->u0walkl2;player->FRAMENUM++;break;
					case 2:	tmpsurface=player->u0walkl3;player->FRAMENUM++;break;
					case 3:	tmpsurface=player->u0walkl2;player->FRAMENUM=0;break;
				};break;
			case 3:
				switch(player->FRAMENUM)
				{
					case 0:	tmpsurface=player->u0walkr1;player->FRAMENUM++;break;
					case 1:	tmpsurface=player->u0walkr2;player->FRAMENUM++;break;
					case 2:	tmpsurface=player->u0walkr3;player->FRAMENUM++;break;
					case 3:	tmpsurface=player->u0walkr2;player->FRAMENUM=0;break;
				};break;
		};
		SDL_BlitSurface(tmpsurface, NULL, screen, &destrect);
	}
	return 0;
}

int U0_drawTree(SDL_Surface *screen, int x1, int y1, Uint32 curcolor, U0_player *player)
{
	if(player->config.game[0].svga==0)
	{
		hplot(screen, x1+20,y1+20, x1+30, y1+20, curcolor);
		hplot(screen, x1+30, y1+20, x1+30, y1+30, curcolor);
		hplot(screen, x1+30, y1+30, x1+20, y1+30, curcolor);
		hplot(screen, x1+20, y1+30, x1+20, y1+20, curcolor);
	}
	else
	{
		SDL_Rect destrect;
		destrect.x=(int)(x1*screen->w/280.0);
		destrect.y=(int)(y1*screen->h/192.0);
		SDL_Surface *tmpsurface=player->u0tree;
		SDL_BlitSurface(tmpsurface, NULL, screen, &destrect);
		return 0;
	}
	return 0;
}

int U0_drawVillage(SDL_Surface *screen, int x1, int y1, Uint32 curcolor, U0_player *player)
{
	if(player->config.game[0].svga==0)
	{
		hplot(screen, x1+10, y1+10, x1+20, y1+10, curcolor);
		hplot(screen, x1+20, y1+10, x1+20, y1+40, curcolor);
		hplot(screen, x1+20, y1+40, x1+10, y1+40, curcolor);
		hplot(screen, x1+10, y1+40, x1+10, y1+30, curcolor);
		hplot(screen, x1+10, y1+30, x1+40, y1+30, curcolor);
		hplot(screen, x1+40, y1+30, x1+40, y1+40, curcolor);
		hplot(screen, x1+40, y1+40, x1+30, y1+40, curcolor);
		hplot(screen, x1+30, y1+40, x1+30, y1+10, curcolor);
		hplot(screen, x1+30, y1+10, x1+40, y1+10, curcolor);
		hplot(screen, x1+40, y1+10, x1+40, y1+20, curcolor);
		hplot(screen, x1+40, y1+20, x1+10, y1+20, curcolor);
		hplot(screen, x1+10, y1+20, x1+10, y1+10, curcolor);
	}
	else
	{
		SDL_Rect destrect;
		destrect.x=(int)(x1*screen->w/280.0);
		destrect.y=(int)(y1*screen->h/192.0);
		SDL_Surface *tmpsurface=player->u0village;
		SDL_BlitSurface(tmpsurface, NULL, screen, &destrect);
	}

	return 0;
}

int U0_drawDungeon(SDL_Surface *screen, int x1, int y1, Uint32 curcolor, U0_player *player)
{
	if(player->config.game[0].svga==0)
	{
		hplot(screen, x1+20, y1+20, x1+30, y1+30, curcolor);
		hplot(screen, x1+20, y1+30, x1+30, y1+20, curcolor);
	}
	else
	{
		SDL_Rect destrect;
		destrect.x=(int)(x1*screen->w/280.0);
		destrect.y=(int)(y1*screen->h/192.0);
		SDL_Surface *tmpsurface=player->u0dungeon;
		SDL_BlitSurface(tmpsurface, NULL, screen, &destrect);
	}
	return 0;
};

int U0_drawCastle(SDL_Surface *screen, int x1, int y1, Uint32 curcolor, U0_player *player)
{
	if(player->config.game[0].svga==0)
	{
		hplot(screen, x1, y1, x1+50, y1, curcolor);
		hplot(screen, x1+50, y1, x1+50, y1+50, curcolor);
		hplot(screen, x1+50, y1+50, x1, y1+50, curcolor);
		hplot(screen, x1, y1+50, x1, y1, curcolor);
		hplot(screen, x1+10, y1+10, x1+10, y1+40, curcolor);
		hplot(screen, x1+10, y1+40, x1+40, y1+40, curcolor);
		hplot(screen, x1+40, y1+40, x1+40, y1+10, curcolor);
		hplot(screen, x1+40, y1+10, x1+10, y1+10, curcolor);
		hplot(screen, x1+10, y1+10, x1+40, y1+40, curcolor);
		hplot(screen, x1+10, y1+40, x1+40, y1+10, curcolor);
	}
	else
	{
		SDL_Rect destrect;
		destrect.x=(int)(x1*screen->w/280.0);
		destrect.y=(int)(y1*screen->h/192.0);
		SDL_Surface *tmpsurface=player->u0castle;
		SDL_BlitSurface(tmpsurface, NULL, screen, &destrect);
	}
	return 0;
}

int U0_drawMountain(SDL_Surface *screen, int x1, int y1, Uint32 curcolor, U0_player *player)
{
	if(player->config.game[0].svga==0)
	{
    		hplot(screen, x1+10, y1+50, x1+10, y1+40, curcolor);
    		hplot(screen, x1+10, y1+40, x1+20, y1+30, curcolor);
		hplot(screen, x1+20, y1+30, x1+40, y1+30, curcolor);
		hplot(screen, x1+20, y1+30, x1+40, y1+30, curcolor);
		hplot(screen, x1+40, y1+30, x1+40, y1+50, curcolor);
		hplot(screen, x1, y1+10, x1+10, y1+10, curcolor);
		hplot(screen, x1+50, y1+10, x1+40, y1+10, curcolor);
		hplot(screen, x1, y1+40, x1+10, y1+40, curcolor);
		hplot(screen, x1+40, y1+40, x1+50, y1+40, curcolor);
		hplot(screen, x1+10, y1, x1+10, y1+20, curcolor);
		hplot(screen, x1+10, y1+20, x1+20, y1+20, curcolor);
		hplot(screen, x1+20, y1+20, x1+20, y1+30, curcolor);
		hplot(screen, x1+20, y1+30, x1+30, y1+30, curcolor);
		hplot(screen, x1+30, y1+30, x1+30, y1+10, curcolor);
		hplot(screen, x1+30, y1+10, x1+40, y1+10, curcolor);
		hplot(screen, x1+40, y1+10, x1+40, y1, curcolor);
	}
	else
	{
		SDL_Rect destrect;
		destrect.x=(int)(x1*screen->w/280.0);
		destrect.y=(int)(y1*screen->h/192.0);
		SDL_Surface *tmpsurface=player->u0mountain;
		SDL_BlitSurface(tmpsurface, NULL, screen, &destrect);
;	}
	return 0;
}

int U0_drawGrass(SDL_Surface *screen, int x1, int y1, Uint32 curcolor, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{
		SDL_Rect destrect;
		destrect.x=(int)(x1*screen->w/280.0);
		destrect.y=(int)(y1*screen->h/192.0);
		SDL_Surface *tmpsurface=player->u0grass;
		SDL_BlitSurface(tmpsurface, NULL, screen, &destrect);
	}
	return 0;
}

int U0_scrollMap(SDL_Surface *s, U0_player *player, int direction)
{
/*	if(player->config.game[0].svga==0)
	{
		return 0;
	} */
	float xscaler=(s->w/280.0);
	float yscaler=(s->h/192.0);
	player->DIRECTION=direction;
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

	SDL_Surface *screen=SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 400, 32, rmask, gmask, bmask, amask);
	int ys=-1;
	int ye=1;
	int xs=-1;
	int xe=1;
	switch(direction)
	{
		case 0:	//up
			ye=2;
			break;
		case 1:	//down
			ys=-2;
			break;
		case 2:	//left
			xe=2;
			break;
		case 3:	//right
			xs=-2;
			break;
	}
	
	for(int y=ys;y<=ye;y++)
	{
		for(int x=xs;x<=xe;x++)
		{
	//PLAYER
			player->zz=player->te[player->tx+x][player->ty+y];
			int x1=65+(x-xs)*50;
			int y1=(y-ys)*50;
			switch((int)player->zz)
			{
				case 2:	//TREE
					U0_drawTree(screen, x1, y1, player->curcolor, player);
				break;
				case 3:	//VILLAGE
					U0_drawVillage(screen, x1, y1, player->curcolor, player);
				break;
				case 4:	//DUNGEON
					U0_drawDungeon(screen, x1, y1, player->curcolor, player);
				break;
				case 5:	//CASTLE
					U0_drawCastle(screen, x1, y1, player->curcolor, player);
				break;
				case 1:	//MOUNTAIN
					U0_drawMountain(screen, x1, y1, player->curcolor, player);
				break;
				default:
					U0_drawGrass(screen, x1, y1, player->curcolor, player);
				break;
			}
		}		
	}
	int xadder=0;
	int yadder=0;
	int startx=0;
	int starty=0;
	switch(direction)
	{
		case 0:	yadder=-1;
			starty=1;
			break;
		case 1:	yadder=1;
			break;
		case 2:	xadder=-1;
			startx=1;
			break;
		case 3:	xadder=1;
			break;
	}
	SDL_Rect dest;
	dest.y=0;
	dest.x=(int)(65*xscaler);
	dest.w=345;
	dest.h=315;
	SDL_Rect src;
	src.w=dest.w;
	src.h=dest.h;
	int basex=(int)((65+startx*50)*xscaler);
	int basey=(int)(starty*50*yscaler);
	int divs=8;
	float addpixels=50/divs;
	for(int z=0;z<=divs;z++)
	{
		src.x=basex+(int)(z*xadder*addpixels*xscaler);
		src.y=basey+(int)(z*yadder*addpixels*yscaler);
		SDL_BlitSurface(screen, &src, s, &dest);
		U0_drawPlayer(s, 0, 0, player->curcolor, player);
		refresh(s, player->SCREEN);
		SDL_Delay(10);
	}
	SDL_FreeSurface(screen);
	return 0;
}

int U0_drawMap(SDL_Surface *screen, U0_player *player)
{
	SDL_Rect rr;
	rr.w=screen->w;
	rr.h=screen->h-(16*4);
	rr.x=0;
	rr.y=0;
	if(player->config.game[0].svga==0)
		u0cleargr(screen);

	for(int y=-1;y<=1;y++)
	{
		for(int x=-1;x<=1;x++)
		{
	//PLAYER
			player->zz=player->te[player->tx+x][player->ty+y];
			int x1=65+(x+1)*50;
			int y1=(y+1)*50;
			switch((int)player->zz)
			{
				case 2:	//TREE
					U0_drawTree(screen, x1, y1, player->curcolor, player);
				break;
				case 3:	//VILLAGE
					U0_drawVillage(screen, x1, y1, player->curcolor, player);
				break;
				case 4:	//DUNGEON
					U0_drawDungeon(screen, x1, y1, player->curcolor, player);
				break;
				case 5:	//CASTLE
					U0_drawCastle(screen, x1, y1, player->curcolor, player);
				break;
				case 1:	//MOUNTAIN
					U0_drawMountain(screen, x1, y1, player->curcolor, player);
				break;
				default:
					U0_drawGrass(screen, x1, y1, player->curcolor, player);
				break;
			}

		}
		
	}
	U0_drawPlayer(screen, 0,0,player->curcolor, player);
	return 0;
}

int U0_mainLoop(SDL_Surface *screen, U0_player *player, Tterminal *term)
{

	if(player->config.game[0].music)
		if(player->inout==0)
		{
			char temp[300];
			sprintf(temp, "%s/u0/midi/world.mid", player->config.datapath);
			player->u0music = Mix_LoadMUS(temp);
		}
//			player->u0music = Mix_LoadMUS("data/u0/midi/world.mid");
		else
		{
			char temp[300];
			sprintf(temp, "%s/u0/midi/dungeon.mid", player->config.datapath);
			player->u0music = Mix_LoadMUS(temp);
		}
//			player->u0music = Mix_LoadMUS("data/u0/midi/dungeon.mid");
	if(player->u0music)
		Mix_FadeInMusic(player->u0music,-1,500);
	player->px=0;
	player->py=0;
	Uint16 c=1;
	if(player->config.game[0].svga)
	{
		SDL_Surface *tmpsurface=player->u0background;
		SDL_BlitSurface(tmpsurface, NULL, screen, NULL);
	}
	while(1)
	{
		if(c)
		{
			term->print("COMMAND? ");
		}
 		term->blit();
 	//	refresh(screen, player->SCREEN);
		SDL_Delay(25);	//keep it cpu friendly
		c=getKey(player->SCREEN);
		if(c==SDL_QUIT)
			exit(-1);
		if(c)
			switch(c)
			{
				case SDL_QUIT:
						if ( player->u0music )
						{
							Mix_FadeOutMusic(500);
							Mix_FreeMusic(player->u0music);
							player->u0music = NULL;
						}

						exit(-1);
						break;
				case SDLK_UP:	if(player->inout==0)
						{
							if(U0_ter_up(screen, player, term))
								return 1;
								
						}
						else
						{
							//u0clear(screen, player);
							if(U0_dng_up(screen, player, term))
								return 1;
						}
						break;
				case SDLK_RIGHT:if(player->inout==0)
						{
							if(U0_ter_right(screen, player, term))
								return 1;
						}
						else
						{
							//u0clear(screen, player);
							if(U0_dng_right(screen, player, term))
								return 1;
						}
						break;
				case SDLK_LEFT:if(player->inout==0)
						{
							if(U0_ter_left(screen, player, term))
								return 1;
						}
						else
						{
							//u0clear(screen, player);
							if(U0_dng_left(screen, player, term))
								return 1;
						}
						break;
				case SDLK_DOWN:if(player->inout==0)
						{
							if(U0_ter_down(screen, player, term))
								return 1;
						}
						else
						{
							//u0clear(screen, player);
							if(U0_dng_down(screen, player, term))
								return 1;
						}
						break;
				case SDLK_e:if(player->inout==0)
						{
							player->DIRECTION=1;
							//u0clear(screen, player);
							if ( player->u0music )
							{
								Mix_FadeOutMusic(500);
								Mix_FreeMusic(player->u0music);
								player->u0music = NULL;
							}
							if(U0_ter_e(screen, player, term))
								return 1;
							if(player->config.game[0].music)
								if(player->inout==0)
								{
									char temp[300];
									sprintf(temp, "%s/u0/midi/world.mid", player->config.datapath);
									player->u0music = Mix_LoadMUS(temp);
								}
						//			player->u0music = Mix_LoadMUS("data/u0/midi/world.mid");
								else
								{
									char temp[300];
									sprintf(temp, "%s/u0/midi/dungeon.mid", player->config.datapath);
									player->u0music = Mix_LoadMUS(temp);
								}
								//	player->u0music = Mix_LoadMUS("data/u0/midi/dungeon.mid");
							if(player->u0music)
								Mix_FadeInMusic(player->u0music,-1,500);
								
							if(player->config.game[0].svga)
								if(player->inout==0)
								{
									SDL_Surface *tmpsurface=player->u0background;
									SDL_BlitSurface(tmpsurface, NULL, screen, NULL);
								}
						}
						else
						{
							//u0clear(screen, player);
							
							if ( player->u0music )
							{
								Mix_FadeOutMusic(500);
								Mix_FreeMusic(player->u0music);
								player->u0music = NULL;
							}
							u0clear(screen, player);
							if(U0_dng_e(screen, player, term))
								return 1;
							if(player->config.game[0].music)
								if(player->inout==0)
								{
									char temp[300];
									sprintf(temp, "%s/u0/midi/world.mid", player->config.datapath);
									player->u0music = Mix_LoadMUS(temp);
								}
								//	player->u0music = Mix_LoadMUS("data/u0/midi/world.mid");
								else
								{
									char temp[300];
									sprintf(temp, "%s/u0/midi/dungeon.mid", player->config.datapath);
									player->u0music = Mix_LoadMUS(temp);
								}
								//	player->u0music = Mix_LoadMUS("data/u0/midi/dungeon.mid");
							if(player->u0music)
								Mix_FadeInMusic(player->u0music,-1,500);

							if(player->config.game[0].svga)
								if(player->inout==0)
								{
									SDL_Surface *tmpsurface=player->u0background;
									SDL_BlitSurface(tmpsurface, NULL, screen, NULL);
								}
						}
						break;
				case SDLK_a:if(player->inout==0)
						{	if(U0_ter_a(screen, player, term))
								return 1;
						}
						else
						{
							//u0clear(screen, player);
							if(U0_dng_a(screen, player, term))
								return 1;
						}
						break;
				case SDLK_SPACE:	
							//u0clear(screen, player);
							player->DIRECTION=1;
							term->print("PASS\n");
							if(U0_updatefood(screen ,player,term))
								return 1;
							break;
				case SDLK_s:	
						//u0clear(screen, player);
						U0_z(screen, player, term);
						term->dropLine();
						if(player->config.game[0].svga)
							if(player->inout==0)
							{
								SDL_Surface *tmpsurface=player->u0background;
								SDL_BlitSurface(tmpsurface, NULL, screen, NULL);
							}
						break;
				case SDLK_p:	if(player->pa)
						{
							player->pa=0;
							term->print("PAUSE OFF\n");
						}
						else
						{
							player->pa=1;
							term->print("PAUSE ON\n");
						}
						break;
				case SDLK_F11:	if(player->config.game[0].svga)
						{
							u0cleargr(screen);
							player->config.game[0].svga=0;
							term->print("APPLE GRAPHICS\n");
						}
						else
						{
							player->config.game[0].svga=1;
							SDL_Surface *tmpsurface=player->u0background;
							SDL_BlitSurface(tmpsurface, NULL, screen, NULL);
						}
						if(player->inout==0)
							U0_drawMap(screen, player);
						else
							U0_drawDng(screen, player, term);
						refresh(screen, player->SCREEN);

					//	if(U0_updatefood(screen ,player,term))
					//		return 1;
						
						break;
				case SDLK_F12:	if(player->config.game[0].music)
						{
							if ( player->u0music )
							{
								Mix_FadeOutMusic(500);
								Mix_FreeMusic(player->u0music);
								player->u0music = NULL;
							}
							player->config.game[0].music=0;
							term->print("SOUND OFF\n");
						}
						else
						{						
							player->config.game[0].music=1;
							if(player->config.game[0].music)
								if(player->inout==0)
								{
									char temp[300];
									sprintf(temp, "%s/u0/midi/world.mid", player->config.datapath);
									player->u0music = Mix_LoadMUS(temp);
								}
								//	player->u0music = Mix_LoadMUS("data/u0/midi/world.mid");
								else
								{
									char temp[300];
									sprintf(temp, "%s/u0/midi/dungeon.mid", player->config.datapath);
									player->u0music = Mix_LoadMUS(temp);
								}
								//	player->u0music = Mix_LoadMUS("data/u0/midi/dungeon.mid");
							if(player->u0music)
								Mix_FadeInMusic(player->u0music,-1,500);
							term->print("SOUND ON\n");
						}
						break;
				default:	term->print("HUH?\n");
						break;
			}	
	}
	return 0;
}



int U0_updatefood(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	Tfont font(u0font_xpm);
	player->pw[0]=(double)player->pw[0]-1.0+(double)sgn(player->inout)*.9;
	if(player->pw[0]<0)
	{
		term->print("\n");
		player->c[0]=0;
		term->print("YOU HAVE STARVED!!!!!\n");
		term->blit();
		refresh(screen, player->SCREEN);
	}
	else
	{
		char tempstr[40];
		sprintf(tempstr, "FOOD=%0.1f", player->pw[0]);
		PRINT(screen, &font, 30, 21, tempstr, 192, 192, 192);
		sprintf(tempstr, "H.P.=%d", player->c[0]);
		PRINT(screen, &font, 30, 22, tempstr, 192, 192, 192);
		sprintf(tempstr, "GOLD=%d", player->c[5]);
		PRINT(screen, &font, 30, 23, tempstr, 192, 192, 192);
		refresh(screen, player->SCREEN);
	}
	if(player->c[0]<=0)
	{
		U0_resurrection(screen, player);
		return 1;
	}	
	if(player->inout>0)
	{
		u0_enemy_Attack(screen, player, term);
	}
	if(player->c[0]<=0)
	{
	
		U0_resurrection(screen, player);
		return 1;
	}		
	char tempstr[40];
	sprintf(tempstr, "FOOD=%0.1f    ", player->pw[0]);
	PRINT(screen, &font, 30, 21, tempstr, 192, 192, 192);
	sprintf(tempstr, "H.P.=%d    ", player->c[0]);
	PRINT(screen, &font, 30, 22, tempstr, 192, 192, 192);
	sprintf(tempstr, "GOLD=%d    ", player->c[5]);
	PRINT(screen, &font, 30, 23, tempstr, 192, 192, 192);
//	refresh(screen, player->SCREEN);
	if(player->inout==0)
		U0_drawMap(screen, player);
	else
		U0_drawDng(screen, player, term);
	refresh(screen, player->SCREEN);
	return 0;
}


int U0_ter_up(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	term->print("NORTH\n");
	if (player->te[player->tx][player->ty-1] == 1)
		term->print("YOU CAN'T PASS THE MOUNTAINS\n");
	else
	{
		player->ty--;
		U0_scrollMap(screen, player, 0);
		if(U0_updatefood(screen, player, term))
			return 1;
	}
	return 0;
}

int U0_ter_right(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	term->print("EAST\n");
	if (player->te[player->tx+1][player->ty] == 1)
		term->print("YOU CAN'T PASS THE MOUNTAINS\n");
	else
	{
		player->tx++;
		U0_scrollMap(screen, player, 3);
		if(U0_updatefood(screen, player, term))
			return 1;
	}
	return 0;
}

int U0_ter_left(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	term->print("WEST\n");
	if (player->te[player->tx-1][player->ty] == 1)
		term->print("YOU CAN'T PASS THE MOUNTAINS\n");
	else
	{
		player->tx--;
		U0_scrollMap(screen, player, 2);
		if(U0_updatefood(screen, player, term))
			return 1;
	}
	
	return 0;
}

int U0_ter_down(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	term->print("SOUTH\n");
	if (player->te[player->tx][player->ty+1] == 1)
		term->print("YOU CAN'T PASS THE MOUNTAINS\n");
	else
	{
		player->ty++;
		U0_scrollMap(screen, player, 1);
		if(U0_updatefood(screen, player, term))
			return 1;
	}	
	return 0;
}

int U0_ter_e(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	if(player->te[player->tx][player->ty] == 3)
	{
		term->clear();
		if(player->config.game[0].music)
		{
			char temp[300];
			sprintf(temp, "%s/u0/midi/shop.mid", player->config.datapath);
			player->u0music = Mix_LoadMUS(temp);
		}
		//	player->u0music = Mix_LoadMUS("data/u0/midi/shop.mid");
		int looping=-1;
		if(player->u0music)
			Mix_FadeInMusic(player->u0music,looping,500);
		U0_printWeapons(screen, player);
		U0_adventureShop(screen, player);
		if ( player->u0music )
		{
			Mix_FadeOutMusic(500);
			Mix_FreeMusic(player->u0music);
			player->u0music = NULL;
		}
		if(U0_updatefood(screen, player, term))
			return 1;
	}
	else
		
	if((player->te[player->tx][player->ty] == 4)&&(player->inout==0))
	{
		term->print("GO DUNGEON\n");
		term->print("PLEASE WAIT \n");
		player->inout=1;
		U0_makeDungeon(player);
		player->dx=1;player->dy=0;player->px=1;player->py=1;
		if(U0_updatefood(screen, player, term))
			return 1;
	}
	else
	if(player->te[player->tx][player->ty] == 5)
	{
		term->clear();
		if(player->config.game[0].music)
		{
			char temp[300];
			sprintf(temp, "%s/u0/midi/castle.mid", player->config.datapath);
			player->u0music = Mix_LoadMUS(temp);
		}
		//	player->u0music = Mix_LoadMUS("data/u0/midi/castle.mid");
		int looping=-1;
		if(player->u0music)
			Mix_FadeInMusic(player->u0music,looping,500);
		U0_castle(screen, player);
		if ( player->u0music )
		{
			Mix_FadeOutMusic(500);
			Mix_FreeMusic(player->u0music);
			player->u0music = NULL;
		}

		if(U0_updatefood(screen, player, term))
			return 1;
	}
	else
		term->print("HUH?\n");
	return 0;
}

int U0_ter_a(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	if(U0_updatefood(screen, player, term))
		return 1;
	return 0;
}

int U0_z(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	U0_printWeapons(screen, player);
	Tfont font(u0font_xpm);	
	PRINT(screen, &font, 0, 0, "PRESS -CR- TO CONTINUE", 192, 192, 192);
	refresh(screen, player->SCREEN);
	char c=0;
	while(c!=SDLK_RETURN)
	{
		c=waitKey(player->SCREEN);
		if(c==SDL_QUIT)
			exit(-1);

	}
	u0clear(screen, player);
//	refresh(screen, player->SCREEN);
	if(U0_updatefood(screen, player, term))
		return 1;
	return 0;
}

int U0_resurrection(SDL_Surface *screen, U0_player *player)
{
	player->DIRECTION=1;
	if ( player->u0music )
	{
		Mix_FadeOutMusic(500);
		Mix_FreeMusic(player->u0music);
		player->u0music = NULL;
	}
	if(player->config.game[0].music)
	{
		char temp[300];
		sprintf(temp, "%s/u0/midi/res.mid", player->config.datapath);
		player->u0music = Mix_LoadMUS(temp);
	}
	//	player->u0music = Mix_LoadMUS("data/u0/midi/res.mid");
	if(player->u0music)
		Mix_FadeInMusic(player->u0music,-1,500);
	u0clear(screen, player);
	Tfont font(u0font_xpm);
	PRINT(screen, &font, 0, 2, "        WE MOURN THE PASSING OF", 192, 192, 192);
	char pn[80];
	strcpy(pn, player->pn);
	if(strlen(pn)>22)
	{
		pn[0]=0;
	}
	if(pn[0]==0)
		strcpy(pn, "THE PEASANT");		
	char temp[40];
	sprintf(temp, "%s AND HIS COMPUTER", pn);	
	PRINT(screen, &font, 20-(strlen(temp)/2), 3, temp, 192, 192, 192);
	PRINT(screen, &font, 0, 4, "  TO INVOKE A MIRACLE OF RESSURECTION", 192, 192, 192);
	PRINT(screen, &font, 0, 5, "           <HIT ESC KEY>", 192, 192, 192);
	char c=0;
	refresh(screen, player->SCREEN);
	
	while(c!=SDLK_ESCAPE)
	{
		c=waitKey(player->SCREEN);
		if(c==SDL_QUIT)
			exit(-1);
	}
	if ( player->u0music )
	{
		Mix_FadeOutMusic(500);
		Mix_FreeMusic(player->u0music);
		player->u0music = NULL;
	}
	return 0;
}

int U0_castle(SDL_Surface *screen, U0_player *player)
{
	int curpos=0;
	Tfont font(u0font_xpm);
	u0clear(screen, player);
	if(player->pn[0]==0)
	{
		curpos++;
		PRINT(screen, &font, 0, curpos++, "    WELCOME PEASANT INTO THE HALLS OF", 192, 192, 192);
		PRINT(screen, &font, 0, curpos++, "THE MIGHTY LORD BRITISH. HEREIN THOU MAY", 192, 192, 192);
		PRINT(screen, &font, 0, curpos++, "CHOOSE TO DARE BATTLE WITH THE EVIL", 192, 192, 192);
		PRINT(screen, &font, 0, curpos++, "CREATURES OF THE DEPTHS, FOR GREAT", 192, 192, 192);
		PRINT(screen, &font, 0, curpos++, "REWARD!", 192, 192, 192);curpos++;curpos++;

		PRINT(screen, &font, 0, curpos, "WHAT IS THY NAME PEASANT ", 192,192,192);
		refresh(screen, player->SCREEN);
		
		getWord(screen, player->SCREEN, &font, player->pn, 25, curpos++, 192,192,192, 15);
		PRINT(screen, &font, 0,curpos++, "DOEST THOU WISH FOR GRAND ADVENTURE ? ", 192, 192, 192);curpos++;
		refresh(screen, player->SCREEN);
		char c=waitKey(player->SCREEN);
		if(c==SDL_QUIT)
			exit(-1);
		if(c!=SDLK_y)
		{
			curpos++;
			PRINT(screen, &font, 0, curpos++, "THEN LEAVE AND BEGONE!", 192, 192, 192); 
			player->pn[0]=0;
			PRINT(screen, &font, 0, curpos++, "         PRESS -SPACE- TO CONT.", 192, 192, 192);
			c=waitKey(player->SCREEN);
			u0clear(screen, player);
			if(c==SDL_QUIT)
				exit(-1);
			return 0;
		}
		else
		{
			curpos+=2;
			PRINT(screen, &font, 0, curpos++, "GOOD! THOU SHALT TRY TO BECOME A", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "KNIGHT!!!", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "THY FIRST TASK IS TO GO INTO THE", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "DUNGEONS AND TO RETURN ONLY AFTER", 192, 192, 192);
			char temp[40];
			player->task=player->c[4]/3;
			sprintf(temp, "KILLING A(N) %s", player->ms[player->task]);
			PRINT(screen, &font, 0, curpos++, temp, 192, 192, 192);
			
	

			curpos++;
			PRINT(screen, &font, 0, curpos++, "    GO NOW UPON THIS QUEST, AND MAY", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "LADY LUCK BE FAIR UNTO YOU.....", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, ".....ALSO I, BRITISH, HAVE INCREASED", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "EACH OF THY ATTRIBUTES BY ONE!", 192, 192, 192);


			curpos++;
			PRINT(screen, &font, 0, curpos++, "         PRESS -SPACE- TO CONT.", 192, 192, 192);
			refresh(screen, player->SCREEN);
			char c=waitKey(player->SCREEN);
			u0clear(screen, player);
			if(c==SDL_QUIT)
				exit(-1);
			for(int x=0;x<5;x++)
				player->c[x]++;
			return 0;
		}		
	}
	if(player->task>0)
	{
		curpos+=2;
		PRINT(screen, &font, 0, curpos++, " WHY HAST THOU RETURNED?", 192, 192, 192);
		char temp[40];sprintf(temp, "THOU MUST KILL A(N) %s", player->ms[player->task]);
		PRINT(screen, &font, 0, curpos++, temp, 192, 192, 192);
		PRINT(screen, &font, 0, curpos++, "GO NOW AND COMPLETE THY QUEST!", 192, 192, 192);
		PRINT(screen, &font, 0, curpos++, "         PRESS -SPACE- TO CONT.", 192, 192, 192);
		refresh(screen, player->SCREEN);
		char c=waitKey(player->SCREEN);
		u0clear(screen, player);
		if(c==SDL_QUIT)
			exit(-1);
		return 0;		
	}
	else
	{
		curpos+=2;
		PRINT(screen, &font, 0, curpos++, "AAHH!!.....", 192, 192, 192);
		PRINT(screen, &font, 11, curpos, player->pn, 192, 192, 192);
		PRINT(screen, &font, 0, curpos++, "THOU HAST ACOMPLISHED THY QUEST!", 192, 192, 192);
		if(abs(player->task) == 10)
		{
			u0clear(screen, player);
			curpos=2;
			char temp[40];sprintf(temp, " %s,", player->pn);
			PRINT(screen, &font, 0, curpos++, temp, 192, 192, 192); 
			PRINT(screen, &font, 0, curpos++, "	 THOU HAST PROVED THYSELF WORTHY", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "OF KNIGHTHOOD, CONTINUE PLAY IF THOU", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "DOTH WISH, BUT THOU HAST ACOMPLISHED", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "THE MAIN OBJECTIVE OF THIS GAME...", 192, 192, 192);

			if(player->lp!=10)
			{
				curpos++;
				PRINT(screen, &font, 0, curpos++, "   NOW MAYBE THOU ART FOOLHEARTY", 192, 192, 192);
				sprintf(temp, "ENOUGH TO TRY DIFFICULTY LEVEL %d", player->lp+1);
				PRINT(screen, &font, 0, curpos++, temp, 192, 192, 192);
			}
			else

			{
				curpos++;
				PRINT(screen, &font, 0, curpos++, "...CALL CALIFORNIA PACIFIC COMPUTER", 192, 192, 192);
				PRINT(screen, &font, 0, curpos++, "AT (415)-569-9126 TO REPORT THIS", 192, 192, 192);
				PRINT(screen, &font, 0, curpos++, "AMAZING FEAT!", 192, 192, 192);
			}

			curpos++;
			PRINT(screen, &font, 0, curpos++, "         PRESS -SPACE- TO CONT.", 192, 192, 192);
			refresh(screen, player->SCREEN);
			char c=waitKey(player->SCREEN);
			u0clear(screen, player);
			if(c==SDL_QUIT)
				exit(-1);
			for(int x=0;x<5;x++)
				player->c[x]++;
			return 0;

		}
		else
		{
			PRINT(screen, &font, 0, curpos++, "UNFORTUNATELY, THIS IS NOT ENOUGH TO", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "BECOME A KNIGHT.", 192, 192, 192);
			player->task=abs(player->task)+1;
			char temp[40];
			sprintf(temp, "NOW THOU MUST KILL A(N) %s", player->ms[player->task]);
			PRINT(screen, &font, 0, curpos++, temp, 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "    GO NOW UPON THIS QUEST, AND MAY", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "LADY LUCK BE FAIR UNTO YOU.....", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, ".....ALSO I, BRITISH, HAVE INCREASED", 192, 192, 192);
			PRINT(screen, &font, 0, curpos++, "EACH OF THY ATTRIBUTES BY ONE!", 192, 192, 192);
			curpos++;
			PRINT(screen, &font, 0, curpos++, "         PRESS -SPACE- TO CONT.", 192, 192, 192);
			refresh(screen, player->SCREEN);
			char c=waitKey(player->SCREEN);
			u0clear(screen, player);
			if(c==SDL_QUIT)
				exit(-1);
			for(int x=0;x<5;x++)
				player->c[x]++;
			return 0;
		}
	}			
	return 0;
}

int U0_chargen(SDL_Surface *screen, U0_player *player)
{
	player->lk=0;
	SDL_Rect r;
	u0clear(screen, player);
	
	Tfont font(u0font_xpm);
	PRINT(screen, &font, 0, 4, "TYPE THY LUCKY NUMBER.....", 192, 192, 192);
	refresh(screen, player->SCREEN);
	player->inout=0;
	player->ln=getNum(screen, player->SCREEN, &font, 26, 4, 192, 192, 192, 0, 999);
//	if(player->ln==SDL_QUIT)
//		exit(-1);
	PRINT(screen, &font, 0, 6, "LEVEL OF PLAY (1-10)......", 192, 192, 192);
	refresh(screen, player->SCREEN);
	player->lp=getNum(screen, player->SCREEN, &font, 26, 6, 192, 192, 192, 1, 10);
//	if(player->lp==SDL_QUIT)
//		exit(-1);	

	player->zz=rnd(-abs(player->ln));
	strcpy(player->cs[0], "HIT POINTS.....");
	strcpy(player->cs[1], "STRENGTH.......");
	strcpy(player->cs[2], "DEXTERITY......");
	strcpy(player->cs[3], "STAMINA........");
	strcpy(player->cs[4], "WISDOM.........");
	strcpy(player->cs[5], "GOLD...........");
	for(int pxx=0;pxx<=5;pxx++)
		player->pw[pxx]=0;
	strcpy(player->ms[0], ""); 
	strcpy(player->ms[1], "SKELETON");
	strcpy(player->ms[2], "THIEF");
	strcpy(player->ms[3], "GIANT RAT");
	strcpy(player->ms[4], "ORC");
	strcpy(player->ms[5], "VIPER");
	strcpy(player->ms[6], "CARRION CRAWLER");
	strcpy(player->ms[7], "GREMLIN");
	strcpy(player->ms[8], "MIMIC");
	strcpy(player->ms[9], "DAEMON");
	strcpy(player->ms[10], "BALROG");
 	Uint32 q=0;
	while(q!=SDLK_y)
	{
		r.x=0;r.y=8*font.chr[0]->h;r.w=18*font.chr[0]->w;r.h=6*font.chr[0]->h;
		SDL_FillRect(screen, &r, 0);
		for(int x=0;x<=5;x++)
		{
			player->c[x]=(int)(sqrt(rnd(1))*21+4);
		}
		int pos=8;
		for(int x=0;x<=5;x++)
		{
			char temp[50];
			sprintf(temp, "%s%d\n", player->cs[x], player->c[x]);
			PRINT(screen, &font, 0, pos+x, temp, 192, 192, 192);
		}
		PRINT(screen, &font, 0, 15, "SHALT THOU PLAY WITH THESE QUALITIES?", 192, 192, 192);
		refresh(screen, player->SCREEN);
		q=waitKey(player->SCREEN);
		if(q==SDL_QUIT)
			exit(-1);
	}
	int go=1;	
	PRINT(screen, &font, 0, 16, "AND SHALT THOU BE A FIGHTER OR A MAGE?", 192, 192, 192);
	refresh(screen, player->SCREEN);
	while(go)
	{
		player->pt=waitKey(player->SCREEN);
		if(player->pt==SDL_QUIT)
			exit(-1);
		if(player->pt=='m')
			player->pt='M';
		if(player->pt=='f')
			player->pt='F';
		if((player->pt=='F')||(player->pt=='M'))
			go=0;		
	}
	char tt[2];tt[1]=0;tt[0]=player->pt;
	PRINT(screen, &font, 39, 15, tt, 192,192,192);
	strcpy(player->ws[0], "FOOD");
	strcpy(player->ws[1], "RAPIER");
	strcpy(player->ws[2], "AXE");
	strcpy(player->ws[3], "SHIELD");
	strcpy(player->ws[4], "BOW AND ARROWS");
	strcpy(player->ws[5], "MAGIC AMULET");
	U0_printWeapons(screen, player);
	U0_adventureShop(screen, player);
	return 0;
}


int U0_printWeapons(SDL_Surface *screen, U0_player *player)
{
	Tfont font(u0font_xpm);
	u0clear(screen, player);
	PRINT(screen, &font, 5, 2, "STAT'S", 192, 192, 192);              
	PRINT(screen, &font, 25, 2, "WEAPONS", 192, 192, 192);	
	for(int x=0;x<=5;x++)
	{
		char temp[80];
		sprintf(temp, "%s %d", player->cs[x], player->c[x]);
		PRINT(screen, &font, 0, x+4, temp, 192, 192, 192);
		sprintf(temp, "0-%s", player->ws[x]);
		PRINT(screen, &font, 23, x+4, temp, 192, 192, 192);
	}	
	PRINT(screen, &font, 17, 10, "Q-QUIT", 192, 192, 192);
	
	for(int z=0;z<=5;z++)
	{
		char temp[10];
		sprintf(temp, "%0.0f", player->pw[z]);
		PRINT(screen, &font, 24-(strlen(temp)), 4+z, temp, 192, 192, 192);	//strlen counts the terminating character...
	}
	PRINT(screen, &font, 4, 16, "PRICE", 192, 192, 192);
	PRINT(screen, &font, 14, 16, "DAMAGE", 192, 192, 192);
	PRINT(screen, &font, 24, 16, "ITEM", 192, 192, 192);
	for(int x=0;x<=5;x++)
	{
		PRINT(screen, &font, 24, 18+x, player->ws[x], 192, 192, 192);
	}
	PRINT(screen, &font, 4, 18, "1 FOR 10", 192, 192, 192);
	PRINT(screen, &font, 14, 18, "N/A", 192, 192, 192);
	PRINT(screen, &font, 4, 19, "8", 192, 192, 192);
	PRINT(screen, &font, 14, 19, "1-10", 192, 192, 192);
	PRINT(screen, &font, 4, 20, "5", 192, 192, 192);
	PRINT(screen, &font, 14, 20, "1-5", 192, 192, 192);
	PRINT(screen, &font, 4, 21, "6", 192, 192, 192);
	PRINT(screen, &font, 14, 21, "1", 192, 192, 192);
	PRINT(screen, &font, 4, 22, "3", 192, 192, 192);
	PRINT(screen, &font, 14, 22, "1-4", 192, 192, 192);
	PRINT(screen, &font, 4, 23, "15", 192, 192, 192);
	PRINT(screen, &font, 14, 23, "?????", 192, 192, 192);
	refresh(screen, player->SCREEN);
	return 0;
}

int U0_adventureShop(SDL_Surface *screen, U0_player *player)
{
	Tfont font(u0font_xpm);
	int go=1;
	char tempterm[3][40];
	for(int ttx=0;ttx<3;ttx++)
		strcpy(tempterm[ttx], "");
		strcpy(tempterm[0], tempterm[1]);strcpy(tempterm[1], tempterm[2]);strcpy(tempterm[2], "WELCOME TO THE ADVENTURE SHOP");
	while(go)
	{

		strcpy(tempterm[0], tempterm[1]);strcpy(tempterm[1], tempterm[2]);strcpy(tempterm[2], "WHICH ITEM SHALT THOU BUY ");
		SDL_Rect rr;
		rr.x=0;rr.y=font.chr[0]->h*11;rr.h=font.chr[0]->h*3;rr.w=screen->w;
		SDL_FillRect(screen, &rr, 0);
		for(int ttx=0;ttx<3;ttx++)
			PRINT(screen, &font, 0, 11+ttx, tempterm[ttx], 192, 192, 192);
		refresh(screen, player->SCREEN);
		char q=waitKey
		(player->SCREEN);
		if(q==SDL_QUIT)
			exit(-1);
		if((q>='A')&&(q<='Z'))
			q=q+('a'-'A');
		if(q=='q')
		{
			strcpy(tempterm[0], tempterm[1]);strcpy(tempterm[1], tempterm[2]);strcpy(tempterm[2], "BYE");
			SDL_Rect rr;
			rr.x=0;rr.y=font.chr[0]->h*11;rr.h=font.chr[0]->h*3;rr.w=screen->w;
			SDL_FillRect(screen, &rr, 0);
			for(int ttx=0;ttx<3;ttx++)
				PRINT(screen, &font, 0, 11+ttx, tempterm[ttx], 192, 192, 192);
			refresh(screen, player->SCREEN);
			SDL_Delay(1000);
			u0clear(screen, player);
			return 0;
		}
		int z=-1;
		int p=0;
		int g=1;
		char tempstr[80];
			tempstr[0]=0;
		switch(q)
		{
			case 'f':	sprintf(tempstr, "%s%s", tempterm[2], "FOOD");z=0;p=1;break;
			case 'r':	sprintf(tempstr, "%s%s", tempterm[2], "RAPIER");z=1;p=8;break;
			case 'a':	sprintf(tempstr, "%s%s", tempterm[2], "AXE");z=2;p=5;break;
			case 's':	sprintf(tempstr, "%s%s", tempterm[2], "SHIELD");z=3;p=6;break;
			case 'b':	sprintf(tempstr, "%s%s", tempterm[2], "BOW");z=4;p=3;break;
			case 'm':	sprintf(tempstr, "%s%s", tempterm[2], "AMULET");z=5;p=15;break;
			default:	strcpy(tempterm[0], tempterm[1]);strcpy(tempterm[1], tempterm[2]);strcpy(tempterm[2], "I'M SORRY WE DON'T HAVE THAT.");
					g=0;p=0;break;
		}
	if(tempstr[0])
	{
		strcpy(tempterm[2], tempstr);		
	}
		if((q=='r')&&(player->pt=='M')&&(g))
		{
			strcpy(tempterm[0], tempterm[1]);strcpy(tempterm[1], tempterm[2]);strcpy(tempterm[2], "I'M SORRY MAGES");
			strcpy(tempterm[0], tempterm[1]);strcpy(tempterm[1], tempterm[2]);strcpy(tempterm[2], "CAN'T USE THAT!");
			g=0;
		}

		if((q=='b')&&(player->pt=='M')&&(g))
		{
			strcpy(tempterm[0], tempterm[1]);strcpy(tempterm[1], tempterm[2]);strcpy(tempterm[2], "I'M SORRY MAGES");
			strcpy(tempterm[0], tempterm[1]);strcpy(tempterm[1], tempterm[2]);strcpy(tempterm[2], "CAN'T USE THAT!");
			g=0;
		}

		if(player->c[5]-p<0)
		{
			strcpy(tempterm[0], tempterm[1]);strcpy(tempterm[1], tempterm[2]);strcpy(tempterm[2], "M'LORD THOU CAN NOT AFFORD THAT ITEM.");
			g=0;
		}

		if(g)
		{
			if(z==0)
				player->pw[z]=player->pw[z]+9.0;
			player->pw[z]=player->pw[z]+1;
			player->c[5]=player->c[5]-p;
			char temp2[40];
			sprintf(temp2, "%d  ", player->c[5]);
			PRINT(screen, &font, 16, 9, temp2, 192, 192, 192);
			char temp3[40];
			sprintf(temp3, "%0.0f", player->pw[z]);
			PRINT(screen, &font, 24-strlen(temp3), 4+z, temp3, 192, 192, 192);			
		}
		
	}
	return 0;

}

