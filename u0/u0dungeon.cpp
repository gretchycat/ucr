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
#include "u0types.h"
#include "../applebasic.h"

int U0_updatefood(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_drawDng(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_makeDungeon(U0_player *player);
int U0_addMonsters(U0_player *player);
int U0_dng_up(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_dng_right(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_dng_left(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_dng_down(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_dng_a(SDL_Surface *screen, U0_player *player, Tterminal *term);
int u0_enemy_Attack(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_dng_e(SDL_Surface *screen, U0_player *player, Tterminal *term);
void U0_draw3087(SDL_Surface *screen, float c, float b, float dis, Uint32 cl);

void u0cleargr(SDL_Surface *s)
{
	SDL_Rect rr;
	rr.w=s->w;
	rr.h=s->h-(16*4);
	rr.x=0;
	rr.y=0;
	SDL_FillRect(s, &rr, SDL_MapRGBA(s->format, 0, 0, 0, 255));
	SDL_UpdateRect(s, 0, 0, 0, 0);
}

void u0clear(SDL_Surface *s, U0_player *player)
{
	SDL_FillRect(s, NULL, SDL_MapRGBA(s->format, 0, 0, 0, 255));
		if(player->config.game[0].svga)
		{
			SDL_Surface *tmp2=setBrightness(player->u0title, .25);
			SDL_BlitSurface(tmp2, NULL, s, NULL);
			SDL_FreeSurface(tmp2);
		}
	SDL_UpdateRect(s, 0, 0, 0, 0);
}

//void u0refresh(SDL_Surface *s, U0_player *p)
//{
//	float scl=240.0/640.0;
//	printf("new size: %fx%f\n", 640*scl, 400*scl);
//	SDL_Surface *tmp=zoomSurface(s, scl, scl, 1);
//	SDL_BlitSurface(tmp, NULL, p->SCREEN, NULL);
//	SDL_FreeSurface(tmp);
//	SDL_BlitSurface(s, NULL, p->SCREEN, NULL);
//	SDL_UpdateRect(p->SCREEN, 0, 0, 0, 0);		
//}

//void u0refreshbuffer(SDL_Surface *s, U0_player *p)
//{
//	SDL_BlitSurface(p->SCREEN, NULL, s, NULL);
//	SDL_UpdateRect(s, 0, 0, 0, 0);		
//}

int U0_drawDngItem(SDL_Surface *screen, SDL_Surface *xpm, int minx, int miny, int maxx, int maxy)
{
	//sanity check
	if(minx>maxx)
	{
		int temp=minx;
		minx=maxx;
		maxx=temp;
	}
	if(miny>maxy)
	{
		int temp=miny;
		miny=maxy;
		maxy=temp;
	}
	float xscl=(maxx-minx)/(xpm->w*280.0/640.0);
	float yscl=(maxy-miny)/(xpm->h*192.0/400.0);
	SDL_Surface *itm=zoomSurface(xpm, xscl, yscl, 1);
//	SDL_FreeSurface(tmp);
//	SDL_SetColorKey(itm, SDL_SRCCOLORKEY, SDL_MapRGB(itm->format, 255,255,255));
	SDL_Rect loc;
	loc.x=(int)(minx*640.0/280.0);
	loc.y=(int)(miny*400.0/192.0);
	loc.w=0;
	loc.h=0;
	SDL_BlitSurface(itm, NULL, screen, &loc);
	SDL_FreeSurface(itm);
	return 0;
}

int U0_drawDngItemTruncX(SDL_Surface *screen, SDL_Surface *xpm, int minx, int miny, int maxx, int maxy)
{
	//sanity check
	if(minx>maxx)
	{
		int temp=minx;
		minx=maxx;
		maxx=temp;
	}
	if(miny>maxy)
	{
		int temp=miny;
		miny=maxy;
		maxy=temp;
	}
//	float xscl=(maxx-minx)/(xpm->w*280.0/640.0);
	float yscl=(maxy-miny)/(xpm->h*192.0/400.0);
	SDL_Surface *itm=zoomSurface(xpm, yscl, yscl, 1);
//	SDL_SetColorKey(itm, SDL_SRCCOLORKEY, SDL_MapRGB(itm->format, 255,255,255));
	SDL_Rect loc;
	SDL_Rect src;
	loc.x=(int)(minx*640.0/280.0);
	loc.y=(int)(miny*400.0/192.0);
	loc.w=0;
	loc.h=0;
	src.x=0;
	src.y=0;
	src.w=(int)((maxx-minx)*640.0/280.0);
	src.h=(int)((maxy-miny)*400.0/192.0);
	SDL_BlitSurface(itm, &src, screen, &loc);
	SDL_FreeSurface(itm);
	return 0;
}

int U0_drawSkeleton(SDL_Surface *screen, int b, int c, U0_player *player)
{
	float dis=player->dis;
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(c-30/dis); int miny=(int)(b-80/dis); 
		int maxx=(int)(c+20/dis); int maxy=b; 
		U0_drawDngItem(screen, player->u0skeleton, minx, miny, maxx, maxy);
	}
	else
	{
		hplot(screen, c-23/dis, b, c-15/dis, b, player->curcolor);
		hplot(screen, c-15/dis, b,c-15/dis,b-15/dis, player->curcolor);
		hplot(screen, c-15/dis,b-15/dis, c-8/dis, b-30/dis, player->curcolor);
		hplot(screen, c-8/dis, b-30/dis, c+8/dis,b-30/dis, player->curcolor);
		hplot(screen, c+8/dis,b-30/dis, c+15/dis, b-15/dis, player->curcolor);
		hplot(screen, c+15/dis, b-15/dis,c+15/dis, b, player->curcolor);
		hplot(screen, c+15/dis, b, c+23/dis, b, player->curcolor);
		hplot(screen, c, b-26/dis, c, b-65/dis, player->curcolor);
		hplot(screen, c-2/dis+.5, b-38/dis, c+2/dis+.5, b-38/dis, player->curcolor);
		hplot(screen, c-3/dis+.5, b-45/dis, c+3/dis+.5, b-45/dis, player->curcolor);
		hplot(screen, c-5/dis+.5, b-53/dis, c+5/dis+.5, b-53/dis, player->curcolor);
		hplot(screen, c-23/dis, b-56/dis, c-30/dis, b-53/dis, player->curcolor);
		hplot(screen, c-30/dis, b-53/dis, c-23/dis, b-45/dis, player->curcolor);
		hplot(screen, c-23/dis, b-45/dis, c-23/dis, b-53/dis, player->curcolor);
		hplot(screen, c-23/dis, b-53/dis, c-8/dis, b-38/dis, player->curcolor);
		hplot(screen, c-15/dis, b-45/dis, c-8/dis, b-60/dis, player->curcolor);
		hplot(screen, c-8/dis, b-60/dis, c+8/dis, b-60/dis, player->curcolor);
		hplot(screen, c+8/dis, b-60/dis, c+15/dis, b-45/dis, player->curcolor);
		hplot(screen, c+15/dis, b-45/dis, c+15/dis, b-42/dis, player->curcolor);
		hplot(screen, c+15/dis, b-42/dis, c+15/dis, b-57/dis, player->curcolor);
		hplot(screen, c+15/dis, b-42/dis, c+15/dis, b-57/dis, player->curcolor);
		hplot(screen, c+12/dis, b-45/dis, c+20/dis, b-45/dis, player->curcolor);
		hplot(screen, c, b-75/dis, c-5/dis+.5, b-80/dis, player->curcolor);
		hplot(screen, c-5/dis+.5, b-80/dis, c-8/dis, b-75/dis, player->curcolor);
		hplot(screen, c-8/dis, b-75/dis, c-5/dis+.5, b-65/dis, player->curcolor);
		hplot(screen, c-5/dis+.5, b-65/dis, c+5/dis+.5, b-65/dis, player->curcolor);
		hplot(screen, c+5/dis+.5, b-65/dis, c+5/dis+.5, b-68/dis, player->curcolor);
		hplot(screen, c+5/dis+.5, b-68/dis, c-5/dis+.5, b-68/dis, player->curcolor);
		hplot(screen, c-5/dis+.5, b-68/dis, c-5/dis+.5, b-65/dis, player->curcolor);
		hplot(screen, c+5/dis+.5, b-65/dis, c+8/dis, b-75/dis, player->curcolor);
		hplot(screen, c+8/dis, b-75/dis, c+5/dis+.5, b-80/dis, player->curcolor);
		hplot(screen, c+5/dis+.5, b-80/dis, c-5/dis+.5, b-80/dis, player->curcolor);
		hplot(screen, c-5/dis+.5, b-72/dis, c-5/dis+.5, b-72/dis, player->curcolor);
		hplot(screen, c+5/dis+.5, b-72/dis, c+5/dis+.5, b-72/dis, player->curcolor);
	}
	return 0;
}				 


int U0_drawThief(SDL_Surface *screen, int b, int c, U0_player *player)
{
	float dis=player->dis;
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(c-30/dis);
		int miny=(int)(b-83/dis);
		int maxx=(int)(c+30/dis);
		int maxy=b;
		U0_drawDngItem(screen, player->u0thief, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, c, b-56/dis, c, b-8/dis, player->curcolor);
		hplot(screen, c, b-8/dis, c+10/dis, b, player->curcolor);
		hplot(screen, c+10/dis, b, c+30/dis, b, player->curcolor);
		hplot(screen, c+30/dis, b, c+30/dis, b-45/dis, player->curcolor);
		hplot(screen, c+30/dis, b-45/dis, c+10/dis, b-64/dis, player->curcolor);
		hplot(screen, c+10/dis, b-64/dis, c, b-56/dis, player->curcolor);
		hplot(screen, c, b-56/dis, c-10/dis, b-64/dis, player->curcolor);
		hplot(screen, c-10/dis, b-64/dis, c-30/dis, b-45/dis, player->curcolor);
		hplot(screen, c-30/dis, b-45/dis, c-30/dis, b, player->curcolor);
		hplot(screen, c-30/dis, b, c-10/dis, b, player->curcolor);
		hplot(screen, c-10/dis, b, c, b-8/dis, player->curcolor);
		hplot(screen, c-10/dis, b-64/dis, c-10/dis, b-75/dis, player->curcolor);
		hplot(screen, c-10/dis, b-75/dis, c, b-83/dis, player->curcolor);
		hplot(screen, c, b-83/dis, c+10/dis, b-75/dis, player->curcolor);
		hplot(screen, c+10/dis, b-75/dis, c, b-79/dis, player->curcolor);
		hplot(screen, c, b-79/dis, c-10/dis, b-75/dis, player->curcolor);
		hplot(screen, c-10/dis, b-75/dis, c, b-60/dis, player->curcolor);
		hplot(screen, c, b-60/dis, c+10/dis, b-75/dis, player->curcolor);
		hplot(screen, c+10/dis, b-75/dis, c+10/dis, b-64/dis, player->curcolor);
	}
	return 0;
}

int U0_drawRat(SDL_Surface *screen, int b, int c, U0_player *player)
{
	float dis=player->dis;
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(c-15/dis);
		int miny=(int)(b-40/dis);
		int maxx=(int)(c+20/dis);
		int maxy=b;
		U0_drawDngItem(screen, player->u0giantrat, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, c+5/dis, b-30/dis, c, b-25/dis, player->curcolor);
		hplot(screen, c, b-25/dis, c-5/dis, b-30/dis, player->curcolor);
		hplot(screen, c-5/dis, b-30/dis, c-15/dis, b-5/dis, player->curcolor);
		hplot(screen, c-15/dis, b-5/dis, c-10/dis, b, player->curcolor);
		hplot(screen, c-10/dis, b, c+10/dis, b, player->curcolor);
		hplot(screen, c+10/dis, b, c+15/dis, b-5/dis, player->curcolor);
		hplot(screen, c+15/dis, b-5/dis,c+20/dis, b-5/dis, player->curcolor);
		hplot(screen, c+20/dis, b-5/dis, c+10/dis, b, player->curcolor);
		hplot(screen, c+10/dis, b, c+15/dis, b-5/dis, player->curcolor);
		hplot(screen, c+15/dis, b-5/dis, c+5/dis, b-30/dis, player->curcolor);
		hplot(screen, c+5/dis, b-30/dis, c+10/dis, b-40/dis, player->curcolor);
		hplot(screen, c+10/dis, b-40/dis, c+3/dis+.5, b-35/dis, player->curcolor);
		hplot(screen, c+3/dis+.5, b-35/dis, c-3/dis+.5, b-35/dis, player->curcolor);
		hplot(screen, c-3/dis+.5, b-35/dis, c-10/dis, b-40/dis, player->curcolor);
		hplot(screen, c-10/dis, b-40/dis, c-5/dis, b-30/dis, player->curcolor);
		hplot(screen, c-5/dis, b-33/dis, c-3/dis+.5, b-30/dis, player->curcolor);
		hplot(screen, c+5/dis, b-33/dis, c+3/dis+.5, b-30/dis, player->curcolor);
		hplot(screen, c-5/dis, b-20/dis, c-5/dis+.5, b-15/dis, player->curcolor);
		hplot(screen, c+5/dis, b-20/dis, c+5/dis, b-15/dis, player->curcolor);
		hplot(screen, c-7/dis, b-20/dis, c-7/dis, b-15/dis, player->curcolor);
		hplot(screen, c+7/dis, b-20/dis, c+7/dis, b-15/dis, player->curcolor);
	}
	return 0;
}

int U0_drawOrc(SDL_Surface *screen, int b, int c, U0_player *player)
{
	float dis=player->dis;
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(c-23/dis);
		int miny=(int)(b-75/dis);
		int maxx=(int)(c+23/dis);
		int maxy=b;
		U0_drawDngItem(screen, player->u0orc, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, c, b, c-15/dis, b, player->curcolor);
		hplot(screen, c-15/dis, b, c-8/dis, b-8/dis, player->curcolor);
		hplot(screen, c-8/dis, b-8/dis, c-8/dis, b-15/dis, player->curcolor);
		hplot(screen, c-8/dis, b-15/dis, c-15/dis, b-23/dis, player->curcolor);
		hplot(screen, c-15/dis, b-23/dis, c-15/dis, b-15/dis, player->curcolor);
		hplot(screen, c-15/dis, b-15/dis, c-23/dis, b-23/dis, player->curcolor);
		hplot(screen, c-23/dis, b-23/dis, c-23/dis, b-45/dis, player->curcolor);
		hplot(screen, c-23/dis, b-45/dis, c-15/dis, b-53/dis, player->curcolor);
		hplot(screen, c-15/dis, b-53/dis, c-8/dis, b-53/dis, player->curcolor);
		hplot(screen, c-8/dis, b-53/dis, c-15/dis, b-68/dis, player->curcolor);
		hplot(screen, c-15/dis, b-68/dis, c-8/dis, b-75/dis, player->curcolor);
		hplot(screen, c-8/dis, b-75/dis, c, b-75/dis, player->curcolor);
		hplot(screen, c, b, c+15/dis, b, player->curcolor);
		hplot(screen, c+15/dis, b, c+8/dis, b-8/dis, player->curcolor);
		hplot(screen, c+8/dis, b-8/dis, c+8/dis, b-15/dis, player->curcolor);
		hplot(screen, c+8/dis, b-15/dis, c+15/dis, b-23/dis, player->curcolor);
		hplot(screen, c+15/dis, b-23/dis, c+15/dis, b-15/dis, player->curcolor);
		hplot(screen, c+15/dis, b-15/dis, c+23/dis, b-23/dis, player->curcolor);
		hplot(screen, c+23/dis, b-23/dis, c+23/dis, b-45/dis, player->curcolor);
		hplot(screen, c+23/dis, b-45/dis, c+15/dis, b-53/dis, player->curcolor);
		hplot(screen, c+15/dis, b-53/dis, c+8/dis, b-53/dis, player->curcolor);
		hplot(screen, c+8/dis, b-53/dis, c+15/dis, b-68/dis, player->curcolor);
		hplot(screen, c+15/dis, b-68/dis, c+8/dis, b-75/dis, player->curcolor);
		hplot(screen, c+8/dis, b-75/dis, c, b-75/dis, player->curcolor);
		hplot(screen, c-15/dis, b-68/dis, c+15/dis, b-68/dis, player->curcolor);
		hplot(screen, c-8/dis, b-53/dis, c+8/dis, b-53/dis, player->curcolor);
		hplot(screen, c-23/dis, b-15/dis, c+8/dis, b-45/dis, player->curcolor);
		hplot(screen, c-8/dis, b-68/dis, c, b-60/dis, player->curcolor);
		hplot(screen, c, b-60/dis, c+8/dis, b-68/dis, player->curcolor);
		hplot(screen, c+8/dis, b-68/dis, c+8/dis, b-60/dis, player->curcolor);
		hplot(screen, c+8/dis, b-60/dis, c-8/dis, b-60/dis, player->curcolor);
		hplot(screen, c-8/dis, b-60/dis, c-8/dis, b-68/dis, player->curcolor);
		hplot(screen, c, b-38/dis, c-8/dis, b-38/dis, player->curcolor);
		hplot(screen, c-8/dis, b-38/dis, c+8/dis, b-53/dis, player->curcolor);
		hplot(screen, c+8/dis, b-53/dis, c+8/dis, b-45/dis, player->curcolor);
		hplot(screen, c+8/dis, b-45/dis, c+15/dis, b-45/dis, player->curcolor);
		hplot(screen, c+15/dis, b-45/dis, c, b-30/dis, player->curcolor);
		hplot(screen, c, b-30/dis, c, b-38/dis, player->curcolor);
	}
	return 0;
}

int U0_drawViper(SDL_Surface *screen, int b, int c, U0_player *player)
{
	float dis=player->dis;
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(c-15/dis);
		int miny=(int)(b-45/dis);
		int maxx=(int)(c+15/dis);
		int maxy=b;
		U0_drawDngItem(screen, player->u0viper, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, c-10/dis, b-15/dis, c-10/dis, b-30/dis, player->curcolor);
		hplot(screen, c-10/dis, b-30/dis, c-15/dis, b-20/dis, player->curcolor);
		hplot(screen, c-15/dis, b-20/dis, c-15/dis, b-15/dis, player->curcolor);
		hplot(screen, c-15/dis, b-15/dis, c-15/dis, b, player->curcolor);
		hplot(screen, c-15/dis, b, c+15/dis, b, player->curcolor);
		hplot(screen, c+15/dis, b, c+15/dis, b-15/dis, player->curcolor);
		hplot(screen, c+15/dis, b-15/dis, c-15/dis, b-15/dis, player->curcolor);
		hplot(screen, c-15/dis, b-10/dis, c+15/dis, b-10/dis, player->curcolor);
		hplot(screen, c+15/dis, b-5/dis, c+15/dis, b-5/dis, player->curcolor);
		HPLOT(screen, c, b-15/dis);
		TO(screen, c-5/dis, b-20/dis, player->curcolor);
		TO(screen, c-5/dis, b-35/dis, player->curcolor);
		TO(screen, c+5/dis, b-35/dis, player->curcolor);
		TO(screen, c+5/dis, b-20/dis, player->curcolor);
		TO(screen, c+10/dis, b-15/dis, player->curcolor);
		hplot(screen, c-5/dis, b-20/dis, c+5/dis, b-20/dis, player->curcolor);
		hplot(screen, c-5/dis, b-25/dis, c+5/dis, b-25/dis, player->curcolor);
		hplot(screen, c-5/dis, b-30/dis, c+5/dis, b-30/dis, player->curcolor); 
		HPLOT(screen, c-10/dis, b-35/dis);
		TO(screen, c-10/dis, b-40/dis, player->curcolor);
		TO(screen, c-5/dis, b-45/dis, player->curcolor);
		TO(screen, c+5/dis, b-45/dis, player->curcolor);
		TO(screen, c+10/dis, b-40/dis, player->curcolor);
		TO(screen, c+10/dis, b-35/dis, player->curcolor);
		HPLOT(screen, c-10/dis, b-40/dis);
		TO(screen, c, b-45/dis, player->curcolor);
		TO(screen, c+10/dis, b-40/dis, player->curcolor);
		HPLOT(screen, c-5/dis, b-40/dis);
		TO(screen, c+5/dis, b-40/dis, player->curcolor);
		TO(screen, c+15/dis, b-30/dis, player->curcolor);
		TO(screen, c, b-40/dis, player->curcolor);
		TO(screen, c-15/dis, b-30/dis, player->curcolor);
		TO(screen, c-5/dis+.5, b-40/dis, player->curcolor);
	}
	return 0;
}

int U0_drawCrawler(SDL_Surface *screen, int b, int c, U0_player *player)
{
	float dis=player->dis;
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(c-30/dis);
		int miny=(int)(79-player->yy[(int)dis]);
		int maxx=(int)(c+30/dis);
		int maxy=(int)(b-40/dis);
		U0_drawDngItem(screen, player->u0crawler, minx, miny, maxx, maxy);		
	}
	else
	{
		HPLOT(screen, c-20/dis, 79-player->yy[(int)dis]);
		TO(screen, c-20/dis, b-88/dis, player->curcolor);
		TO(screen, c-10/dis, b-83/dis, player->curcolor);
		TO(screen, c+10/dis, b-83/dis, player->curcolor);
		TO(screen, c+20/dis, b-88/dis, player->curcolor);
		TO(screen, c+20/dis, 79-player->yy[(int)dis], player->curcolor);
		TO(screen, c-20/dis, 79-player->yy[(int)dis], player->curcolor);
		HPLOT(screen, c-20/dis, b-88/dis);
		TO(screen, c-30/dis, b-83/dis, player->curcolor);
		TO(screen, c-30/dis, b-78/dis, player->curcolor);
		HPLOT(screen, c+20/dis, b-88/dis);
		TO(screen, c+30/dis, b-83/dis, player->curcolor);
		TO(screen, c+40/dis, b-83/dis, player->curcolor);
		HPLOT(screen, c-15/dis, b-86/dis);
		TO(screen, c-20/dis, b-83/dis, player->curcolor);
		TO(screen, c-20/dis, b-78/dis, player->curcolor);
		TO(screen, c-30/dis, b-73/dis, player->curcolor);
		TO(screen, c-30/dis, b-68/dis, player->curcolor);
		TO(screen, c-20/dis, b-63/dis, player->curcolor);
		HPLOT(screen, c-10/dis, b-83/dis);
		TO(screen, c-10/dis, b-58/dis, player->curcolor);
		TO(screen, c, b-50/dis, player->curcolor);
		HPLOT(screen, c+10/dis, b-83/dis);
		TO(screen, c+10/dis, b-78/dis, player->curcolor);
		TO(screen, c+20/dis, b-73/dis, player->curcolor);
		TO(screen, c+20/dis, b-40/dis, player->curcolor);
		HPLOT(screen, c+15/dis, b-85/dis);
		TO(screen, c+20/dis, b-78/dis, player->curcolor);
		TO(screen, c+30/dis, b-76/dis, player->curcolor);
		TO(screen, c+30/dis, b-60/dis, player->curcolor);
		HPLOT(screen, c, b-83/dis);
		TO(screen, c, b-73/dis, player->curcolor);
		TO(screen, c+10/dis, b-68/dis, player->curcolor);
		TO(screen, c+10/dis, b-63/dis, player->curcolor);
		TO(screen, c, b-58/dis, player->curcolor);
	}
	return 0;
}

int U0_drawGremlin(SDL_Surface *screen, int b, int c, U0_player *player)
{
	float dis=player->dis;
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(c-8/dis);
		int miny=(int)(b-20/dis);
		int maxx=(int)(c+10/dis);
		int maxy=(int)b;
		U0_drawDngItem(screen, player->u0gremlin, minx, miny, maxx, maxy);		
	}
	else
	{
		HPLOT(screen, c+5/dis+.5, b-10/dis);
		TO(screen, c-5/dis+.5, b-10/dis, player->curcolor);
		TO(screen, c, b-15/dis, player->curcolor);
		TO(screen, c+10/dis, b-20/dis, player->curcolor);
		TO(screen, c+5/dis+.5, b-15/dis, player->curcolor);
		TO(screen, c+5/dis+.5, b-10/dis, player->curcolor);
		TO(screen, c+7/dis+.5, b-6/dis, player->curcolor);
		TO(screen, c+5/dis+.5, b-3/dis, player->curcolor);
		TO(screen, c-5/dis+.5, b-3/dis, player->curcolor);
		TO(screen, c-7/dis+.5, b-6/dis, player->curcolor);
		TO(screen, c-5/dis+.5, b-10/dis, player->curcolor); 
		HPLOT(screen, c+2/dis+.5, b-3/dis);
		TO(screen, c+5/dis+.5, b, player->curcolor);
		TO(screen, c+8/dis, b, player->curcolor);
		HPLOT(screen, c-2/dis+.5, b-3/dis);
		TO(screen, c-5/dis+.5, b, player->curcolor);
		TO(screen, c-8/dis, b, player->curcolor);
		hplot(screen, c+3/dis+.5, b-8/dis, c+3/dis+.5, b-8/dis, player->curcolor);
		hplot(screen, c-3/dis+.5, b-8/dis, c-3/dis+.5, b-8/dis, player->curcolor);
		hplot(screen, c+3/dis+.5, b-5/dis, c-3/dis+.5, b-5/dis, player->curcolor);
	}
	return 0;
}

int U0_drawMimic(SDL_Surface *screen, int b, int c, U0_player *player)
{
	float dis=player->dis;
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(139-10/dis);
		int miny=(int)(player->per[player->dis][3]-15/dis);
		int maxx=(int)(139+15/dis);
		int maxy=(int)(player->per[player->dis][3]);
		U0_drawDngItem(screen, player->u0mimic, minx, miny, maxx, maxy);		
	}
	else
	{
		HPLOT(screen, 139-10/dis, player->per[player->dis][3]);
		TO(screen, 139-10/dis, player->per[player->dis][3]-10/dis, player->curcolor);
		TO(screen, 139+10/dis, player->per[player->dis][3]-10/dis, player->curcolor);
		TO(screen, 139+10/dis, player->per[player->dis][3], player->curcolor);
		TO(screen, 139-10/dis, player->per[player->dis][3], player->curcolor);
		HPLOT(screen, 139-10/dis, player->per[player->dis][3]-10/dis);
		TO(screen, 139-5/dis, player->per[player->dis][3]-15/dis, player->curcolor);
		TO(screen, 139+15/dis, player->per[player->dis][3]-15/dis, player->curcolor);
		TO(screen, 139+15/dis, player->per[player->dis][3]-5/dis, player->curcolor);
		TO(screen, 139+10/dis, player->per[player->dis][3], player->curcolor);
		hplot(screen, 139+10/dis, player->per[player->dis][3]-10/dis, 139+15/dis, player->per[player->dis][3]-15/dis, player->curcolor);
	}
	return 0;
}

int U0_drawDaemon(SDL_Surface *screen, int b, int c, U0_player *player)
{
	float dis=player->dis;
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(c-40/dis);
		int miny=(int)(b-70/dis);
		int maxx=(int)(c+50/dis);
		int maxy=(int)(b);
		U0_drawDngItem(screen, player->u0daemon, minx, miny, maxx, maxy);		
	}
	else
	{
		HPLOT(screen, c-14/dis, b-46/dis);
		TO(screen, c-12/dis, b-37/dis, player->curcolor);
		TO(screen, c-20/dis, b-32/dis, player->curcolor);
		TO(screen, c-30/dis, b-32/dis, player->curcolor);
		TO(screen, c-22/dis, b-24/dis, player->curcolor);
		TO(screen, c-40/dis, b-17/dis, player->curcolor);
		TO(screen, c-40/dis, b-7/dis, player->curcolor);
		TO(screen, c-38/dis, b-5/dis, player->curcolor); 
		TO(screen, c-40/dis, b-3/dis, player->curcolor);
		TO(screen, c-40/dis, b, player->curcolor);
		TO(screen, c-36/dis, b, player->curcolor);
		TO(screen, c-34/dis, b-2/dis, player->curcolor);
		TO(screen, c-32/dis, b, player->curcolor);
		TO(screen, c-28/dis, b, player->curcolor);
		TO(screen, c-28/dis, b-3/dis, player->curcolor);
		TO(screen, c-30/dis, b-5/dis, player->curcolor);
		TO(screen, c-28/dis, b-7/dis, player->curcolor);
		TO(screen, c-28/dis, b-15/dis, player->curcolor);
		TO(screen, c, b-27/dis, player->curcolor);
		HPLOT(screen, c+14/dis, b-46/dis);
		TO(screen, c+12/dis, b-37/dis, player->curcolor);
		TO(screen, c+20/dis, b-32/dis, player->curcolor);
		TO(screen, c+30/dis, b-32/dis, player->curcolor);
		TO(screen, c+22/dis, b-24/dis, player->curcolor);
		TO(screen, c+40/dis, b-17/dis, player->curcolor);
		TO(screen, c+40/dis, b-7/dis, player->curcolor);
		TO(screen, c+38/dis, b-5/dis, player->curcolor);
		TO(screen, c+40/dis, b-3/dis, player->curcolor);
		TO(screen, c+40/dis, b, player->curcolor);
		TO(screen, c+36/dis, b, player->curcolor);
		TO(screen, c+34/dis, b-2/dis, player->curcolor);
		TO(screen, c+32/dis, b, player->curcolor);
		TO(screen, c+28/dis, b, player->curcolor);
		TO(screen, c+28/dis, b-3/dis, player->curcolor);
		TO(screen, c+30/dis, b-5/dis, player->curcolor);
		TO(screen, c+28/dis, b-7/dis, player->curcolor);
		TO(screen, c+28/dis, b-15/dis, player->curcolor);
		TO(screen, c, b-27/dis, player->curcolor);				
		HPLOT(screen, c+6/dis, b-48/dis);
		TO(screen, c+38/dis, b-27/dis, player->curcolor);
		TO(screen, c+38/dis, b-41/dis, player->curcolor);
		TO(screen, c+40/dis, b-42/dis, player->curcolor);
		TO(screen, c+18/dis, b-56/dis, player->curcolor);
		TO(screen, c+12/dis, b-56/dis, player->curcolor);
		TO(screen, c+10/dis, b-57/dis, player->curcolor);
		TO(screen, c+8/dis, b-56/dis, player->curcolor);
		TO(screen, c-8/dis, b-56/dis, player->curcolor);
		TO(screen, c-10/dis, b-58/dis, player->curcolor);
		TO(screen, c+14/dis, b-58/dis, player->curcolor);
		TO(screen, c+16/dis, b-59/dis, player->curcolor);
		TO(screen, c+8/dis, b-63/dis, player->curcolor);
		TO(screen, c+6/dis, b-63/dis, player->curcolor);
		TO(screen, c+2/dis+.5, b-70/dis, player->curcolor);
		TO(screen, c+2/dis+.5, b-63/dis, player->curcolor);
		TO(screen, c-2/dis+.5, b-63/dis, player->curcolor);
		TO(screen, c-2/dis+.5, b-70/dis, player->curcolor);
		TO(screen, c-6/dis, b-63/dis, player->curcolor);
		TO(screen, c-8/dis, b-63/dis, player->curcolor);
		TO(screen, c-16/dis, b-59/dis, player->curcolor);
		TO(screen, c-14/dis, b-58/dis, player->curcolor);
		TO(screen, c-10/dis, b-57/dis, player->curcolor);
		TO(screen, c-12/dis, b-56/dis, player->curcolor);
		TO(screen, c-18/dis, b-56/dis, player->curcolor);
		TO(screen, c-36/dis, b-47/dis, player->curcolor);
		TO(screen, c-36/dis, b-39/dis, player->curcolor);
		TO(screen, c-28/dis, b-41/dis, player->curcolor);
		TO(screen, c-28/dis, b-46/dis, player->curcolor);
		TO(screen, c-20/dis, b-50/dis, player->curcolor);
		TO(screen, c-18/dis, b-50/dis, player->curcolor);
		TO(screen, c-14/dis, b-46/dis, player->curcolor);
		U0_draw3087(screen, c, b, dis, player->curcolor);
	}
	return 0;
}

int U0_drawBalrog(SDL_Surface *screen, int b, int c, U0_player *player)
{
	float dis=player->dis;
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(c-60/dis);
		int miny=(int)(b-92/dis);
		int maxx=(int)(c+60/dis);
		int maxy=(int)(b);
		U0_drawDngItem(screen, player->u0balrog, minx, miny, maxx, maxy);		
	}
	else
	{
		HPLOT(screen, c+6/dis, b-60/dis);
		TO(screen, c+30/dis, b-90/dis, player->curcolor);
		TO(screen, c+60/dis, b-30/dis, player->curcolor);
		TO(screen, c+60/dis, b-10/dis, player->curcolor);
		TO(screen, c+30/dis, b-40/dis, player->curcolor);
		TO(screen, c+15/dis, b-40/dis, player->curcolor);
		HPLOT(screen, c-6/dis, b-60/dis);
		TO(screen, c-30/dis, b-90/dis, player->curcolor);
		TO(screen, c-60/dis, b-30/dis, player->curcolor);
		TO(screen, c-60/dis, b-10/dis, player->curcolor);
		TO(screen, c-30/dis, b-40/dis, player->curcolor);
		TO(screen, c-15/dis, b-40/dis, player->curcolor);
		HPLOT(screen, c, b-25/dis);
		TO(screen, c+6/dis, b-25/dis, player->curcolor);
		TO(screen, c+10/dis, b-20/dis, player->curcolor);
		TO(screen, c+12/dis, b-10/dis, player->curcolor);
		TO(screen, c+10/dis, b-6/dis, player->curcolor);
		TO(screen, c+10/dis, b, player->curcolor);
		TO(screen, c+14/dis, b, player->curcolor);
		TO(screen, c+15/dis, b-5/dis, player->curcolor);
		TO(screen, c+16/dis, b, player->curcolor);
		TO(screen, c+20/dis, b, player->curcolor);
		TO(screen, c+20/dis, b-6/dis, player->curcolor);
		TO(screen, c+18/dis, b-10/dis, player->curcolor);
		TO(screen, c+18/dis, b-20/dis, player->curcolor);
		TO(screen, c+15/dis, b-30/dis, player->curcolor);
		TO(screen, c+15/dis, b-45/dis, player->curcolor);
		TO(screen, c+40/dis, b-60/dis, player->curcolor);
		TO(screen, c+40/dis, b-70/dis, player->curcolor);
		TO(screen, c+10/dis, b-55/dis, player->curcolor);
		TO(screen, c+6/dis, b-60/dis, player->curcolor);
		TO(screen, c+10/dis, b-74/dis, player->curcolor);
		TO(screen, c+6/dis, b-80/dis, player->curcolor);
		TO(screen, c+4/dis+.5, b-80/dis, player->curcolor);
		TO(screen, c+3/dis+.5, b-82/dis, player->curcolor);
		TO(screen, c+2/dis+.5, b-80/dis, player->curcolor);
		TO(screen, c, b-80/dis, player->curcolor);
		HPLOT(screen, c, b-25/dis);
		TO(screen, c-6/dis, b-25/dis, player->curcolor);
		TO(screen, c-10/dis, b-20/dis, player->curcolor);
		TO(screen, c-12/dis, b-10/dis, player->curcolor);
		TO(screen, c-10/dis, b-6/dis, player->curcolor);
		TO(screen, c-10/dis, b, player->curcolor);
		TO(screen, c-14/dis, b, player->curcolor);
		TO(screen, c-15/dis, b-5/dis, player->curcolor);
		TO(screen, c-16/dis, b, player->curcolor);
		TO(screen, c-20/dis, b, player->curcolor);
		TO(screen, c-20/dis, b-6/dis, player->curcolor);
		TO(screen, c-18/dis, b-10/dis, player->curcolor);
		TO(screen, c-18/dis, b-20/dis, player->curcolor);
		TO(screen, c-15/dis, b-30/dis, player->curcolor);
		TO(screen, c-15/dis, b-45/dis, player->curcolor);
		TO(screen, c-40/dis, b-60/dis, player->curcolor);
		TO(screen, c-40/dis, b-70/dis, player->curcolor);
		TO(screen, c-10/dis, b-55/dis, player->curcolor);
		TO(screen, c-6/dis, b-60/dis, player->curcolor);
		TO(screen, c-10/dis, b-74/dis, player->curcolor);
		TO(screen, c-6/dis, b-80/dis, player->curcolor);
		TO(screen, c-4/dis+.5, b-80/dis, player->curcolor);
		TO(screen, c-3/dis+.5, b-82/dis, player->curcolor);
		TO(screen, c-2/dis+.5, b-80/dis, player->curcolor);
		TO(screen, c, b-80/dis, player->curcolor);
		HPLOT(screen, c-6/dis, b-25/dis);
		TO(screen, c, b-6/dis, player->curcolor);
		TO(screen, c+10/dis, b, player->curcolor);
		TO(screen, c+4/dis+.5, b-8/dis, player->curcolor);
		TO(screen, c+6/dis, b-25/dis, player->curcolor);
		HPLOT(screen, c-40/dis, b-64/dis);
		TO(screen, c-40/dis, b-90/dis, player->curcolor);
		TO(screen, c-52/dis, b-80/dis, player->curcolor);
		TO(screen, c-52/dis, b-40/dis, player->curcolor);
		HPLOT(screen, c+40/dis, b-86/dis);
		TO(screen, c+38/dis, b-92/dis, player->curcolor);
		TO(screen, c+42/dis, b-92/dis, player->curcolor);
		TO(screen, c+40/dis, b-86/dis, player->curcolor);
		TO(screen, c+40/dis, b-50/dis, player->curcolor);
		hplot(screen, c+4/dis+.5, b-70/dis, c+6/dis, b-74/dis, player->curcolor);
		hplot(screen, c-4/dis+.5, b-70/dis, c-6/dis, b-74/dis, player->curcolor);
		hplot(screen, c, b-64/dis, c, b-60/dis, player->curcolor);
	}
	return 0;
}

int U0_drawChest(SDL_Surface *screen, U0_player *player)
{
	float dis=player->dis;
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(139-10/dis);
		int miny=(int)(player->per[player->dis][3]-15/dis);
		int maxx=(int)(139+15/dis);
		int maxy=(int)(player->per[player->dis][3]);
		U0_drawDngItem(screen, player->u0chest, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, 139-10/dis, player->per[player->dis][3], 139-10/dis, player->per[player->dis][3]-10/dis, player->curcolor);
		hplot(screen, 139-10/dis, player->per[player->dis][3]-10/dis, 139+10/dis, player->per[player->dis][3]-10/dis, player->curcolor);
		hplot(screen, 139+10/dis, player->per[player->dis][3]-10/dis, 139+10/dis, player->per[player->dis][3], player->curcolor);
		hplot(screen, 139+10/dis, player->per[player->dis][3], 139-10/dis, player->per[player->dis][3], player->curcolor);
		hplot(screen, 139-10/dis, player->per[player->dis][3]-10/dis, 139-5/dis, player->per[player->dis][3]-15/dis, player->curcolor);
		hplot(screen, 139-5/dis, player->per[player->dis][3]-15/dis, 139+15/dis, player->per[player->dis][3]-15/dis, player->curcolor);
		hplot(screen, 139+15/dis, player->per[player->dis][3]-15/dis, 139+15/dis, player->per[player->dis][3]-5/dis, player->curcolor);
		hplot(screen, 139+15/dis, player->per[player->dis][3]-5/dis, 139+10/dis, player->per[player->dis][3], player->curcolor);
		hplot(screen, 139+10/dis, player->per[player->dis][3]-10/dis, 139+15/dis, player->per[player->dis][3]-15/dis, player->curcolor);
	}
	return 0;
}

int U0_drawLadder(SDL_Surface *screen, U0_player *player)
{
	int base=(int)player->lad[player->dis][3];
	int tp=(int)player->lad[player->dis][2];
	int lx=(int)player->lad[player->dis][0];
	int rx=(int)player->lad[player->dis][1];
//	hplot(screen, lx, base, lx, tp, player->curcolor);
//	hplot(screen, rx, tp, rx, base, player->curcolor);
	int y1=(int)((double)(base*4+tp)/5.0);
	int y2=(int)((double)(base*3+tp*2)/5.0);
	int y3=(int)((double)(base*2+tp*3)/5.0);
	int y4=(int)((double)(base+tp*4)/5.0);
	if(player->config.game[0].svga==1)
	{
		int minx=lx;
		int miny=tp;
		int maxx=rx;
		int maxy=base;
		U0_drawDngItem(screen, player->u0ladder, minx, miny, maxx, maxy);
	}
	else
	{
		hplot(screen, lx, base, lx, tp, player->curcolor);
		hplot(screen, rx, tp, rx, base, player->curcolor);
		hplot(screen, lx, y1, rx, y1, player->curcolor);
		hplot(screen, lx, y2, rx, y2, player->curcolor);
		hplot(screen, lx, y3, rx, y3, player->curcolor);
		hplot(screen, lx, y4, rx, y4, player->curcolor);
	}
	return 0;
}

int U0_drawTopHole(SDL_Surface *screen, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(player->ft[player->dis][0]);
		int miny=(int)(158-player->ft[player->dis][4]);
		int maxx=(int)(player->ft[player->dis][1]);
		int maxy=(int)(158-player->ft[player->dis][5]);
		U0_drawDngItem(screen, player->u0tophole, minx, miny, maxx, maxy);
	}
	else
	{

		hplot(screen, (int)player->ft[player->dis][0], 158-(int)player->ft[player->dis][4], (int)player->ft[player->dis][2], 158-(int)player->ft[player->dis][5], player->curcolor);
		hplot(screen, (int)player->ft[player->dis][2], 158-(int)player->ft[player->dis][5], (int)player->ft[player->dis][3], 158-(int)player->ft[player->dis][5], player->curcolor);
		hplot(screen, (int)player->ft[player->dis][3], 158-(int)player->ft[player->dis][5], (int)player->ft[player->dis][1], 158-(int)player->ft[player->dis][4], player->curcolor);
		hplot(screen, (int)player->ft[player->dis][1], 158-(int)player->ft[player->dis][4], (int)player->ft[player->dis][0], 158-(int)player->ft[player->dis][4], player->curcolor);
	}
	return 0;
}

int U0_drawBottomHole(SDL_Surface *screen, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(player->ft[player->dis][0]);
		int miny=(int)(player->ft[player->dis][5]);
		int maxx=(int)(player->ft[player->dis][1]);
		int maxy=(int)(player->ft[player->dis][4]);
		U0_drawDngItem(screen, player->u0trapdoor, minx, miny, maxx, maxy);
	}
	else
	{
		hplot(screen, (int)player->ft[player->dis][0], (int)player->ft[player->dis][4], (int)player->ft[player->dis][2], (int)player->ft[player->dis][5], player->curcolor);
		hplot(screen, (int)player->ft[player->dis][2], (int)player->ft[player->dis][5], (int)player->ft[player->dis][3], (int)player->ft[player->dis][5], player->curcolor);
		hplot(screen, (int)player->ft[player->dis][3], (int)player->ft[player->dis][5], (int)player->ft[player->dis][1], (int)player->ft[player->dis][4], player->curcolor);
		hplot(screen, (int)player->ft[player->dis][1], (int)player->ft[player->dis][4], (int)player->ft[player->dis][0], (int)player->ft[player->dis][4], player->curcolor);
	}		
	return 0;
}

int U0_drawLeftWallPerp(SDL_Surface *screen, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(player->l1);
		int miny=(int)(player->t2);
		int maxx=(int)(player->l2)+1;
		int maxy=(int)(player->b2);
		U0_drawDngItemTruncX(screen, player->wall_b, minx, miny, maxx, maxy);
	}
	else
	{
		if(player->dis!=0)
			hplot(screen, player->l1, player->t1, player->l1, player->b1, player->curcolor);
		hplot(screen, player->l1, player->t2, player->l2, player->t2, player->curcolor);
		hplot(screen, player->l2, player->t2, player->l2, player->b2, player->curcolor);
		hplot(screen, player->l2, player->b2, player->l1, player->b2, player->curcolor);
	}
	return 0;
}

int U0_drawRightWallPerp(SDL_Surface *screen, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(player->r2);
		int miny=(int)(player->t2);
		int maxx=(int)(player->r1);
		int maxy=(int)(player->b2);
		U0_drawDngItemTruncX(screen, player->wall_b, minx, miny, maxx, maxy);		
	}
	else
	{
		if(player->dis!=0)
			hplot(screen, player->r1, player->t1, player->r1, player->b1, player->curcolor);
		hplot(screen, player->r1, player->t2, player->r2, player->t2, player->curcolor);
		hplot(screen, player->r2, player->t2, player->r2, player->b2, player->curcolor);
		hplot(screen, player->r2, player->b2, player->r1, player->b2, player->curcolor);
	}
	return 0;
}

int U0_drawRightDoor0(SDL_Surface *screen, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{	
		int minx=(int)(273-player->ld[player->dis][1]);
		int miny=(int)(player->ld[player->dis][2]-3);
		int maxx=(int)(279);
		int maxy=(int)(player->b1+2);
		U0_drawDngItem(screen, player->door_r, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, 279, (int)player->ld[player->dis][2]-3, 279-(int)player->ld[player->dis][1], (int)player->ld[player->dis][3], player->curcolor);
		hplot(screen, 279-(int)player->ld[player->dis][1], (int)player->ld[player->dis][3], 279-(int)player->ld[player->dis][1], (int)player->ld[player->dis][5], player->curcolor);
	}
	return 0;
}

int U0_drawLeftDoor0(SDL_Surface *screen, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{	
		int minx=(int)(0);
		int miny=(int)(player->ld[player->dis][2]-3);
		int maxx=(int)(player->ld[player->dis][1]+6);
		int maxy=(int)(player->b1+1);
		U0_drawDngItem(screen, player->door_l, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, 0, (int)player->ld[player->dis][2]-3, (int)player->ld[player->dis][1], (int)player->ld[player->dis][3], player->curcolor);
		hplot(screen, (int)player->ld[player->dis][1], (int)player->ld[player->dis][3], (int)player->ld[player->dis][1], (int)player->ld[player->dis][5], player->curcolor);
	}
	return 0;
}

int U0_drawRightDoor(SDL_Surface *screen, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{	
		int minx=(int)(279-player->ld[player->dis][1]-6/player->dis); int
		miny=(int)(player->ld[player->dis][2]); int
		maxx=(int)(279-player->ld[player->dis][0]+6/player->dis); int
		maxy=(int)(player->ld[player->dis][4]+8/player->dis);
		U0_drawDngItem(screen, player->door_r, minx, miny, maxx, maxy);
	}
	else
	{
		hplot(screen, 279-(int)player->ld[player->dis][0], (int)player->ld[player->dis][4], 279-(int)player->ld[player->dis][0], (int)player->ld[player->dis][2], player->curcolor);
		hplot(screen, 279-(int)player->ld[player->dis][0], (int)player->ld[player->dis][2], 279-(int)player->ld[player->dis][1], (int)player->ld[player->dis][3], player->curcolor);
		hplot(screen, 279-(int)player->ld[player->dis][1], (int)player->ld[player->dis][3], 279-(int)player->ld[player->dis][1], (int)player->ld[player->dis][5], player->curcolor);
	}
	return 0;
}

int U0_drawLeftDoor(SDL_Surface *screen, U0_player *player)
{//FOO!@!
	if(player->config.game[0].svga==1)
	{	
		int minx=(int)(player->ld[player->dis][0]-6/player->dis);
		int miny=(int)(player->ld[player->dis][2]);
		int maxx=(int)(player->ld[player->dis][1]+6/player->dis);
		int maxy=(int)(player->ld[player->dis][4]+8/player->dis);
		U0_drawDngItem(screen, player->door_l, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, (int)player->ld[player->dis][0], (int)player->ld[player->dis][4], (int)player->ld[player->dis][0], (int)player->ld[player->dis][2], player->curcolor);
		hplot(screen, (int)player->ld[player->dis][0], (int)player->ld[player->dis][2], (int)player->ld[player->dis][1], (int)player->ld[player->dis][3], player->curcolor);
		hplot(screen, (int)player->ld[player->dis][1], (int)player->ld[player->dis][3], (int)player->ld[player->dis][1], (int)player->ld[player->dis][5], player->curcolor);
	}
	return 0;
}

int U0_drawRightWall(SDL_Surface *screen, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{	
		SDL_Surface *wall=player->u0rightwall;
		int r1=(int)(player->r1*640.0/280.0);
		int r2=(int)(player->r2*640.0/280.0);
		SDL_Rect src;
		SDL_Rect dst;
		src.x=r2-(640-wall->w)+6;
		src.y=0;
		src.w=r1-r2;
		src.h=wall->h;
		dst.x=r2;
		dst.y=0;
		dst.h=wall->h;
		dst.w=r1-r2;
		SDL_BlitSurface(wall, &src, screen, &dst);
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}
	else
	{
		hplot(screen, (int)player->r1, (int)player->t1, (int)player->r2, (int)player->t2, player->curcolor);
		hplot(screen, (int)player->r1, (int)player->b1, (int)player->r2, (int)player->b2, player->curcolor);
	}
	return 0;
}

int U0_drawLeftWall(SDL_Surface *screen, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{	
		SDL_Surface *wall=player->u0leftwall;
		int l1=(int)(player->l1*640.0/280.0);
		int l2=(int)(player->l2*640.0/280.0);
		SDL_Rect src;
		SDL_Rect dst;
		src.x=l1;
		src.y=0;
		src.w=l2-l1;
		src.h=wall->h;
		dst.x=l1;
		dst.y=0;
		dst.h=wall->h;
		dst.w=l2-l1;
		SDL_BlitSurface(wall, &src, screen, &dst);
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}
	else
	{
		hplot(screen, (int)player->l1, (int)player->t1, (int)player->l2, (int)player->t2, player->curcolor);
		hplot(screen, (int)player->l1, (int)player->b1, (int)player->l2, (int)player->b2, player->curcolor);
	}
	return 0;
}

int U0_drawMidDoor(SDL_Surface *screen, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(player->cd[player->dis][0]);
		int miny=(int)(player->cd[player->dis][2]);
		int maxx=(int)(player->cd[player->dis][1]);
		int maxy=(int)(player->cd[player->dis][3]);
		U0_drawDngItem(screen, player->door_b, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, (int)player->cd[player->dis][0], (int)player->cd[player->dis][3], (int)player->cd[player->dis][0], (int)player->cd[player->dis][2], player->curcolor);
		hplot(screen, (int)player->cd[player->dis][0], (int)player->cd[player->dis][2], (int)player->cd[player->dis][1], (int)player->cd[player->dis][2], player->curcolor);
		hplot(screen, (int)player->cd[player->dis][1], (int)player->cd[player->dis][2], (int)player->cd[player->dis][1], (int)player->cd[player->dis][3], player->curcolor);
	}
	return 0;
}

int U0_drawMidWallPerp(SDL_Surface *screen, U0_player *player)
{
	if(player->config.game[0].svga==1)
	{
		int minx=(int)(player->l1);
		int miny=(int)(player->t1);
		int maxx=(int)(player->r1)+1;
		int maxy=(int)(player->b1);
		U0_drawDngItem(screen, player->wall_b, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, player->l1, player->t1, player->r1, player->t1, player->curcolor);
		hplot(screen, player->r1, player->t1, player->r1, player->b1, player->curcolor);
		hplot(screen, player->r1, player->b1, player->l1, player->b1, player->curcolor);
		hplot(screen, player->l1, player->b1, player->l1, player->t1, player->curcolor);
	}
	return 0;
}

int U0_drawDng(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	player->dis=1;
	
	if(player->config.game[0].svga==1)
	{
		SDL_BlitSurface(player->u0background, NULL, screen, NULL);
		SDL_BlitSurface(player->u0dngback, NULL, screen, NULL);
	}
	else
		u0cleargr(screen);
		
	while(1)
	{
		if(player->dis!=0)
		{
		player->cent=player->dn[player->px+player->dx*player->dis][player->py+player->dy*player->dis];
		player->mc=(int)(player->cent/10.0);
		player->cent=player->cent-player->mc*10;
			if((player->cent==1)||(player->cent==3)||(player->cent==4))
			{	//mid wall perp
				player->en=1;
			}
		}
		if(player->en==1)
		{
			player->en=0;
			break;
		}
		player->dis++;
	}
	while(1)
	{
		float dis=player->dis;
		player->cent=player->dn[player->px+player->dx*player->dis][player->py+player->dy*player->dis];
		player->left=player->dn[player->px+player->dx*player->dis+player->dy][player->py+player->dy*player->dis-player->dx];
		player->righ=player->dn[player->px+player->dx*player->dis-player->dy][player->py+player->dy*player->dis+player->dx];	
	
		player->l1=player->per[player->dis][0];
		player->r1=player->per[player->dis][1];
		player->t1=player->per[player->dis][2];
		player->b1=player->per[player->dis][3];
		player->l2=player->per[player->dis+1][0];
		player->r2=player->per[player->dis+1][1];
		player->t2=player->per[player->dis+1][2];
		player->b2=player->per[player->dis+1][3];
		player->mc=(int)(player->cent/10.0);
		player->cent=player->cent-player->mc*10;
		player->left=(int)(((double)player->left/10.0-(int)((double)player->left/10.0))*10.0+.1);
		player->righ=(int)(((double)player->righ/10.0-(int)((double)player->righ/10.0))*10.0+.1);
		if(dis!=0)
		{
			if((player->cent==1)||(player->cent==3)||(player->cent==4))
			{	//mid wall perp
				U0_drawMidWallPerp(screen, player);
			}
	//		if((player->cent==1)||(player->cent==3))
	//		{
	//			player->en=1;
	//		}
			if(player->cent==4)
			{	//mid door
				U0_drawMidDoor(screen, player);
	//			player->en=1;
			}
		}

		if(((player->cent!=4)&&(player->cent!=1)&&(player->cent!=3))||(dis==0))
		{
			if((player->left==1)||(player->left==3)||(player->left==4))
			{	//left wall
				U0_drawLeftWall(screen, player);
			}
			if((player->righ==1)||(player->righ==3)||(player->righ==4))
			{	//right wall
				U0_drawRightWall(screen, player);
			}

			if((player->left==4) && (dis>0))
			{	//left door
				U0_drawLeftDoor(screen, player);
			}
			if((player->left==4) && (dis==0))
			{	//left door dis==0
				U0_drawLeftDoor0(screen, player);
			}
			if((player->righ==4) && (dis>0))
			{	//right door
				U0_drawRightDoor(screen, player);
			}
			if((player->righ==4) && (dis==0))
			{	//right door dis==0
				U0_drawRightDoor0(screen, player);
			}
			if(!((player->left==3)||(player->left==1)||(player->left==4)))
			{
				//left wall perp
				U0_drawLeftWallPerp(screen, player);
			}
			if(!((player->righ==3)||(player->righ==1)||(player->righ==4)))
			{	//right wall perp
				U0_drawRightWallPerp(screen, player);
			}
			if((player->cent==7)||(player->cent==9))
			{	//bottom opening
				U0_drawBottomHole(screen, player);
			}
			if(player->cent==8)
			{	//top opening
				U0_drawTopHole(screen, player);
			}
			if((player->cent==7)||(player->cent==8))
			{	//ladder
				U0_drawLadder(screen, player);
			}
			if((dis>0)&&(player->cent==5))
			{	//chest
				term->print("CHEST!\n");
				U0_drawChest(screen, player);
			}
		}

	if(player->mc>=1)
	{
		int b=79+player->yy[player->dis];
		int c=139;
		if(player->mc==8)
		{
			term->print("CHEST!\n");
			term->print("\n");
		}
		else
		{
			term->print(player->ms[player->mc]);
			term->print("\n");
		}
		if(dis!=0)
			switch(player->mc)
			{
				case 1:		//skeleton
					U0_drawSkeleton(screen, b, c, player);
				break;


				case 2:		//theif
					U0_drawThief(screen, b, c, player);
				break;


				case 3:		//giant rat
					U0_drawRat(screen, b, c, player);
				break;


				case 4:		//orc
					U0_drawOrc(screen, b, c, player);
				break;

				case 5:		//viper
					U0_drawViper(screen, b, c, player);
				break;


				case 6:		//carrion crawler
					U0_drawCrawler(screen, b, c, player);
				break;


				case 7:		//gremlin
					U0_drawGremlin(screen, b, c, player);
				break;


				case 8:		//mimic
					U0_drawMimic(screen, b, c, player);
				break;


				case 9:		//daemon
					U0_drawDaemon(screen, b, c, player);
				break;

				case 10:	//balrog
					U0_drawBalrog(screen, b, c, player);
 				break;
			}
		}
		if(player->dis==0)
		{
			player->en=0;
			return 0;
		}
//		if(player->en==1)
//		{
		//	u0refresh(screen);
//			player->en=0;
//			return 0;
//		}
//		player->dis++;
		player->dis--;
	}
	return 0;
}


int U0_makeDungeon(U0_player *player)
{
	player->zz=rnd(-abs(player->ln)-player->tx*10-player->ty*1000+player->inout*31.4);
	for(int x=1;x<=9;x++)
		for(int y=1;y<=9;y++)
				player->dn[x][y]=0;
	for(int x=0;x<=10;x++)
	{
		player->dn[x][0]=1;
		player->dn[x][10]=1;
		player->dn[0][x]=1;
		player->dn[10][x]=1;
	}	
	for(int x=2;x<=8;x+=2)
		for(int y=1;y<=9;y++)
		{
			player->dn[x][y]=1;
			player->dn[y][x]=1;
		}
	for(int x=2;x<=8;x+=2)
		for(int y=1;y<=9;y+=2)
		{
			if(rnd(1)>.95)
				player->dn[x][y]=2;
			if(rnd(1)>.95)
				player->dn[y][x]=2;
			if(rnd(1)>.6)
				player->dn[y][x]=3;
			if(rnd(1)>.6)
				player->dn[x][y]=3;
			if(rnd(1)>.6)
				player->dn[x][y]=4;
			if(rnd(1)>.6)
				player->dn[y][x]=4;
			if(rnd(1)>.97)
				player->dn[y][x]=9;
			if(rnd(1)>.97)
				player->dn[x][y]=9;
			if(rnd(1)>.94)
				player->dn[x][y]=5;
			if(rnd(1)>.94)
				player->dn[y][x]=5;
		}
		player->dn[2][1]=0;
		if(((float)player->inout/2.0)==((int)(player->inout/2)))
		{
			player->dn[7][3]=7;
			player->dn[3][7]=8;
		}
		else
		{
			player->dn[7][3]=8;
			player->dn[3][7]=7;			
		}
		if(player->inout==1)
		{
			player->dn[1][1]=8;
			player->dn[7][3]=0;
		}
		U0_addMonsters(player);
		return 0;
}

int U0_dng_up(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	if((player->dn[player->px+player->dx][player->py+player->dy]!=1)&&(player->dn[player->px+player->dx][player->py+player->dy]<10))
	{
		player->px=player->px+player->dx;
		player->py=player->py+player->dy;
	}
		term->print("FORWARD\n");
	if(player->dn[player->px][player->py]==2)
	{
		term->print("AAARRRGGGHHH!!! A TRAP!\n");
		player->c[0]=player->c[0]-(int)rnd(1)*player->inout+3;
		player->mr=1;
		player->inout++;
		char tempstr[80];
		sprintf(tempstr, "FALLING TO LEVEL %d\n",player->inout); 
		term->print(tempstr);
		U0_makeDungeon(player);
		if(U0_updatefood(screen, player, term))
			return 1;
		return 0;
	}
		int z=0;
	if(player->dn[player->px][player->py]==5)
	{
		player->dn[player->px][player->py]=0;
		term->print("GOLD!!!!!\n");
		int z=(int)(rnd(1)*5*player->inout+player->inout);
		char tempstr[80];
		sprintf(tempstr, "%d-PIECES OF EIGHT\n", z);
		term->print(tempstr);
		player->c[5]+=z;
	}

	if(z > 0 )
	{
		z=int(rnd(1) * 6);
		char tempstr[80];
		sprintf(tempstr, "AND A %s", player->ws[z]);
		term->print(tempstr);
		player->pw[z]=player->pw[z]+1;
	}
	if(U0_updatefood(screen, player, term))
		return 1;
	return 0;
}

int U0_dng_right(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	term->print("TURN RIGHT\n");
	if(player->dx!=0)
	{
		player->dy=player->dx;
		player->dx=0;
	}
	else
	{
		player->dx=-player->dy;
		player->dy=0;
	}
	if(U0_updatefood(screen, player, term))
		return 1;
	return 0;
}

int U0_dng_left(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	term->print("TURN LEFT\n");
	if(player->dx!=0)
	{
		player->dy=-player->dx;
		player->dx=0;
	}
	else
	{
		player->dx=player->dy;
		player->dy=0;
	}
	if(U0_updatefood(screen, player, term))
		return 1;
	return 0;
}

int U0_dng_down(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	term->print("TURN AROUND\n");
	player->dx=-player->dx;
	player->dy=-player->dy;
	if(U0_updatefood(screen, player, term))
		return 1;
	return 0;
}

int U0_dng_e(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	if((player->dn[player->px][player->py]==7)||(player->dn[player->px][player->py]==9))
	{
		char temp[40];
		sprintf(temp, "DO DOWN TO LEVEL %d\n", player->inout+1);
		term->print(temp);
		player->inout++;
		U0_makeDungeon(player);
		player->mr=1;
		if(U0_updatefood(screen, player, term))
			return 1;
		return 0;
	}
	if(player->dn[player->px][player->py]!=8)
	{
		term->print("HUH?\n");
		if(U0_updatefood(screen, player, term))
			return 1;
		return 0;
	}
	if(player->inout==1)
	{
		term->print("LEAVE DUNGEON\n");
		player->inout=0;
	}
	else
	{
		char temp[40];
		sprintf(temp, "GO UP TO LEVEL %d\n", player->inout-1);
		term->print(temp);
		player->inout--;
		U0_makeDungeon(player);
		player->mr=1;
	}
	if(player->inout==0)
	{
		term->print("THOU HAST GAINED\n");
		char temp[25];
		sprintf(temp, "%d HIT POINTS\n", player->lk);
		term->print(temp);
		player->c[0]+=player->lk;
		player->lk=0;
	}
	if(U0_updatefood(screen, player, term))
		return 1;
	return 0;
}

int U0_dng_a(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	int magicattack=0;
	int go=0;
	int mn=0;
	int dam=0;
	while(1)
	{
		char q=0;
		int swing=0;
		if(magicattack==0)
		{
			go=1;
			mn=0;
			dam=0;
			term->print("ATTACK\n");
			term->print("WHICH WEAPON ");
			term->blit();
			refresh(screen, player->SCREEN);
			q=waitKey(player->SCREEN);
			if(q==SDL_QUIT)
				exit(-1);
			switch(q)
			{
				case 'r':
					dam=10;
					term->print("RAPIER\n");
					if(player->pw[1]<1)
					{
						term->print("NOT OWNED\n");go=0;
					}
				break;
				case 'a':
					dam=5;
					term->print("AXE\n");
					if(player->pw[2]<1)
					{
						term->print("NOT OWNED\n");go=0;
					}
				break;
				case 's':
					dam=1;
					term->print("SHIELD\n");
					if(player->pw[3]<1)
					{
						term->print("NOT OWNED\n");go=0;
					}
				break;
				case 'b':
					dam=4;
					term->print("BOW\n");
					if(player->pw[4]<1)
					{
						term->print("NOT OWNED\n");go=0;
					}
				break;
			}
		}
		if((q!='m')||(magicattack==1))
		{
			if((go)||(magicattack==1))
			{
//           1650
				if(magicattack==0)
				{
					if((q=='b')&&(player->pt=='M'))
					{
						term->print("MAGES CAN'T USE BOWS!");
						break;
					}
					if((q=='r')&&(player->pt=='M'))
					{
						term->print("MAGES CAN'T USE BOWS!");
						break;
					}
					if(dam==0)
						term->print("HANDS\n");
					swing=0;
//moved this block of code so the logic flows better
					if(dam==5)
					{
						term->print("TO THROW OR SWING:");
						term->blit();
						refresh(screen, player->SCREEN);
						char q=waitKey(player->SCREEN);
						if(q==SDL_QUIT)
							exit(-1);
						if(q!='t')
						{
							term->print("SWING\n");
							swing=1;
//							break;
						}	
						else
						{
							term->print("THROW\n");
							player->pw[2]=player->pw[2]-1;
						}
					}
				}
				if((dam==5)||(dam==4)||(magicattack==1))
				{
					for(int y=1;y<=5;y++)
					{
						if((player->px+player->dx*y<1)||(player->px+player->dx*y>9)||(player->py+player->dy*y>9)||(player->py+player->dy*y<0))
							break;
						mn=player->dn[player->px+player->dx*y][player->py+player->dy*y];
						mn=(int)(mn/10);
						if(mn>0)
							break;
					}
				}		
				if(((dam!=5)&&(dam!=4))||(swing==1))
				{
					mn=player->dn[player->px+player->dx][player->py+player->dy]/10;
				}
				if((mn<1)||(player->c[2]-rnd(1)*25<mn+player->inout))
				{
					term->print("YOU MISSED\n");
				}
				else
				{
					term->print("HIT!!!\n");
					dam=(int)(rnd(1)*dam+player->c[1]/5);
					player->mz[mn][1]=player->mz[mn][1]-dam;
						char tempstr[40];
						sprintf(tempstr, "%s'S HIT POINTS=%d\n", player->ms[mn], player->mz[mn][1]);
						term->print(tempstr);
						if(player->mz[mn][1]<1)
						{
							sprintf(tempstr, "THOU HAS KILLED A %s\n", player->ms[mn]);
							term->print(tempstr);
							term->print("THOU SHALT RECEIVE\n");
							int da=mn+player->inout;
							sprintf(tempstr, "%d PIECES OF EIGHT\n", da);
							term->print(tempstr);
							player->c[5]=player->c[5]+da;
							player->dn[player->ml[mn][0]][player->ml[mn][1]]=player->dn[player->ml[mn][0]][player->ml[mn][1]]-10*mn;
							player->mz[mn][0]=0;
						}
						player->lk=(int)(player->lk+(mn*(player->inout/2.0)));
						if(mn==player->task)
							player->task=-player->task;
						
					}
					if(player->pa==1)
					{
						term->print("-CR- TO CONT. \n");
						term->blit();
						refresh(screen, player->SCREEN);
						waitKey(player->SCREEN);
						if(q==SDL_QUIT)
							exit(-1);
						
					}
					if(U0_updatefood(screen, player, term))
						return 1;
					return 0;
					break;
				
				}
			}
		
		else
		{
			term->print("MAGIC AMULET\n");
			if(player->pw[5]<1)
			{
				term->print("NONE OWNED\n");
			}
			else
			{
				int Q=0;
				if(player->pt=='F')
				{
					Q=(int)(rnd(1)*4+1);	
				}
				else
				{
					term->print("1-LADDER-UP 2-LADDER-DN\n");
					term->print("3-KILL      4-BAD??\n");
					term->print("CHOICE ");
					term->blit();
					refresh(screen, player->SCREEN);
					while((Q<1)||(Q>4))
					{
						Q=waitKey(player->SCREEN)-'0';
						if(Q==SDL_QUIT-'0')
							exit(-1);
						
					}
				if(rnd(1)>.75)
					{
						term->print("LAST CHARGE ON THIS AMULET!\n");
						player->pw[5]--;
					}
				}
				switch(Q)
				{
					case 1:
						term->print("LADDER UP\n");
						player->dn[player->px][player->py]=8;
						if(U0_updatefood(screen, player, term))
							return 1;
						return 0;
						break;	
					case 2:
						term->print("LADDER DOWN\n");
						player->dn[player->px][player->py]=7;
						if(U0_updatefood(screen, player, term))
							return 1;
						return 0;
						break;	
					case 3:
						term->print("MAGIC ATTACK\n");
						dam=10+player->inout;
						magicattack=1;
						break;	
					case 4:
						switch((int)(rnd(1)*3+1))
						{
							case 1:
								term->print("YOU HAVE BEEN TURNED\n");
								term->print("INTO A TOAD!\n");
								for(int z2=1;z2<=4;z2++)
									player->c[z2]=3;
								if(U0_updatefood(screen, player, term))
									return 1;
								return 0;
								break;	
							case 2:
								term->print("YOU HAVE BEEN TURNED\n");
								term->print("INTO A LIZARD MAN!\n");
								for(int y=0;y<=4;y++)
									player->c[y]=(int)(player->c[y]*2.5);
								if(U0_updatefood(screen, player, term))
									return 1;
								return 0;
								break;	
							case 3:
								term->print("BACKFIRE\n");
								player->c[0]/=2;
								if(U0_updatefood(screen, player, term))
									return 1;
								return 0;
						}
				}
			}
		}
	}
	return 0;
}

int U0_addMonsters(U0_player *player)
{
	player->nm=0;
	for(int x=1;x<=10;x++)
	{
		player->mz[x][0]=0;
		player->mz[x][1]=x+3+player->inout;
		if(!(((x-2)>player->inout)||(rnd(1)>.4)))
		{
			player->ml[x][0]=(int)(rnd(1)*9+1);
			player->ml[x][1]=(int)(rnd(1)*9+1);
			while((player->dn[player->ml[x][0]][player->ml[x][1]]!=0)||((player->ml[x][0]==player->px)&&(player->ml[x][1]==player->py)))
			{
				player->ml[x][0]=(int)(rnd(1)*9+1);
				player->ml[x][1]=(int)(rnd(1)*9+1);
			}

			player->dn[player->ml[x][0]][player->ml[x][1]]=x*10;
			player->mz[x][0]=1;
			player->nm++;
			player->mz[x][1]=x*2+player->inout*2*player->lp;
		}
	}
	return 0;
}

void U0_draw3087(SDL_Surface *s, float c, float b, float dis, Uint32 cl)
{
	hplot(s, c-28/dis, b-41/dis, c+30/dis, b-55/dis, cl);
	HPLOT(s, c+28/dis, b-58/dis);
	TO(s, c+22/dis, b-56/dis, cl);
	TO(s, c+22/dis, b-53/dis, cl);
	TO(s, c+28/dis, b-52/dis, cl);
	TO(s, c+34/dis, b-54/dis, cl);
	hplot(s, c+20/dis, b-50/dis, c+26/dis, b-47/dis, cl);
	HPLOT(s, c+10/dis, b-58/dis);
	TO(s, c+10/dis, b-61/dis, cl);
	TO(s, c+4/dis, b-58/dis, cl);
	HPLOT(s, c-10/dis, b-58/dis);
	TO(s, c-10/dis, b-61/dis, cl);
	TO(s, c-4/dis, b-58/dis, cl);
	HPLOT(s, c+40/dis, b-9/dis);
	TO(s, c+50/dis, b-12/dis, cl);
	TO(s, c+40/dis, b-7/dis, cl);
	HPLOT(s, c-8/dis, b-25/dis);
	TO(s, c+6/dis, b-7/dis, cl);
	TO(s, c+28/dis, b-7/dis, cl);
	TO(s, c+28/dis, b-9/dis, cl);
	TO(s, c+20/dis, b-9/dis, cl);
	TO(s, c+6/dis, b-25/dis, cl);
}

int u0_enemy_Attack(SDL_Surface *screen, U0_player *player, Tterminal *term)
{
	for(int mm=1;mm<=10;mm++)
	{
		if(player->mz[mm][0]!=0)
		{
			int skipto4081=0;
			int skipto4500=0;
			int skipto4999=0;
			int d=0;
			float ra=sqrt(pow(player->px-player->ml[mm][0], 2)+pow(player->py-player->ml[mm][1], 2));
			if(player->mz[mm][1]>=player->inout*player->lp)
			{
				if(ra<1.3)
				{
					skipto4500=1;
				}
				else
				{
				if((mm==8)&&(ra<3))
					skipto4999=1;
				}
			}
			if((skipto4500==0)&&(skipto4999==0))
			{
				int x1=sgn(player->px-player->ml[mm][0]);
				int y1=sgn(player->py-player->ml[mm][1]);
				if(player->mz[mm][1]<player->inout*player->lp)
				{
					x1=-x1;
					y1=-y1;
				}
				if(y1!=0)
				{
					d=player->dn[player->ml[mm][0]][(int)(player->ml[mm][1]+y1+.5)];
					if((d==1)||(d>9)||(d==2))
					{
						y1=0;
						if(x1==0)
							break;
						d=player->dn[(int)(player->ml[mm][0]+x1+.5)][player->ml[mm][1]];
						if((d==1)||(d>9)||(d==2))
						{
							x1=0;
							skipto4081=1;
						}
					}
					x1=0;
				}
				else
				{
					y1=0;
					if(x1==0)
						break;
					d=player->dn[(int)(player->ml[mm][0]+x1+.5)][player->ml[mm][1]];
					if((d==1)||(d>9)||(d==2))
					{
						x1=0;
						skipto4081=1;
					}
				}
				if(skipto4081==0)
				{
					player->dn[player->ml[mm][0]][player->ml[mm][1]]-=(10*mm);
					if((player->ml[mm][0]+x1==player->px)&&(player->ml[mm][1]+y1==player->py))
					{
						skipto4999=1;
						break;
					}
					player->ml[mm][0]+=x1;
					player->ml[mm][1]+=y1;
					player->dn[player->ml[mm][0]][player->ml[mm][1]]+=(int)(10*mm+.5);

				}
				if((skipto4500==0)&&(skipto4999==0))
				{
					if((x1!=0)&&(y1!=0))
					{
						skipto4999=1;
						break;
					}
					if((player->mz[mm][1]<player->inout*player->lp)&&(ra<1.3))
					{
						skipto4500=0;
						break;
					}
					if(player->mz[mm][1]<player->inout*player->lp)
						player->mz[mm][1]+=(mm+player->inout);
					skipto4999=1;
				}
			}
			if((skipto4999==0)||(skipto4500==1))
			{
				if((!((mm==2)||(mm==7)))||(rnd(1)<.5))
				{
					term->print("YOU ARE BEING ATTACKED\n");
					char tempstr[40];
					sprintf(tempstr, "BY A %s\n", player->ms[mm]);
					term->print(tempstr);
					if(rnd(1)*20.0-sgn((int)player->pw[3])-player->c[3]+mm+player->inout<0)
					{
						term->print("MISSED\n");
					}
					else
					{
						term->print("HIT\n");
						player->c[0]-=(int)(rnd(1)*mm*player->inout);
					}
					if(player->pa==1)
					{
						term->print("-CR- TO CONT. \n");
						if(waitKey(player->SCREEN)==SDL_QUIT)
							exit(-1);
					}
					skipto4999=1;
				}
				else
				{
					//taken care of by the if statement on line 4500
					if(mm==7)
					{
							player->pw[0]=player->pw[0]/2;
							term->print("A GREMLIN STOLE SOME FOOD\n");
							//4525 is a pause
					}
					else
					{
							int zz=(int)(rnd(1)*6);
							while(player->pw[zz]<1)
								zz=(int)(rnd(1)*6);
							char tempstr[40];
							sprintf(tempstr, "A THIEF STOLE A %s\n", player->ws[zz]);
							term->print(tempstr);
							player->pw[zz]--;
							//4525 is a pause
					}
					if(player->pa==1)
					{
						term->print("-CR- TO CONT. \n");
						if(waitKey(player->SCREEN)==SDL_QUIT)
							exit(-1);
					}
					
				}
			}
		}
	}
	return 0;
}
