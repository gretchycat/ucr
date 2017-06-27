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
#include "u1types.h"
#include "../applebasic.h"

//int U1_updatefood(SDL_Surface *screen, TU1_game *game, Tterminal *term);
int U1_drawDng(SDL_Surface *screen, TU1_game *game, Tterminal *term);
//int U1_makeDungeon(TU1_game *game);
//int U1_addMonsters(TU1_game *game);
//int U1_dng_up(SDL_Surface *screen, TU1_game *game, Tterminal *term);
//int U1_dng_right(SDL_Surface *screen, TU1_game *game, Tterminal *term);
//int U1_dng_left(SDL_Surface *screen, TU1_game *game, Tterminal *term);
//int U1_dng_down(SDL_Surface *screen, TU1_game *game, Tterminal *term);
//int U1_dng_a(SDL_Surface *screen, TU1_game *game, Tterminal *term);
//int u1_enemy_Attack(SDL_Surface *screen, TU1_game *game, Tterminal *term);
//int U1_dng_e(SDL_Surface *screen, TU1_game *game, Tterminal *term);
void U1_draw3087(SDL_Surface *screen, float c, float b, float dis, Uint32 cl);

void u1cleargr(SDL_Surface *s)
{
	SDL_Rect rr;
	rr.w=s->w;
	rr.h=s->h-(16*4);
	rr.x=0;
	rr.y=0;
	SDL_FillRect(s, &rr, SDL_MapRGBA(s->format, 0, 0, 0, 255));
	SDL_UpdateRect(s, 0, 0, 0, 0);
}

/*void u1clear(SDL_Surface *s, TU1_game *game)
{
	SDL_FillRect(s, NULL, SDL_MapRGBA(s->format, 0, 0, 0, 255));
		if(game->config.game[0].svga)
		{
			SDL_Surface *tmp2=setBrightness(game->dungeon.u1title, .25);
			SDL_BlitSurface(tmp2, NULL, s, NULL);
			SDL_FreeSurface(tmp2);
		}
	SDL_UpdateRect(s, 0, 0, 0, 0);
}
*/
//void u1refresh(SDL_Surface *s, TU1_game *p)
//{
//	float scl=240.0/640.0;
//	printf("new size: %fx%f\n", 640*scl, 400*scl);
//	SDL_Surface *tmp=zoomSurface(s, scl, scl, 1);
//	SDL_BlitSurface(tmp, NULL, p->screen, NULL);
//	SDL_FreeSurface(tmp);
//	SDL_BlitSurface(s, NULL, p->screen, NULL);
//	SDL_UpdateRect(p->screen, 0, 0, 0, 0);		
//}

//void u1refreshbuffer(SDL_Surface *s, TU1_game *p)
//{
//	SDL_BlitSurface(p->screen, NULL, s, NULL);
//	SDL_UpdateRect(s, 0, 0, 0, 0);		
//}

int U1_drawDngItem(SDL_Surface *screen, SDL_Surface *xpm, int minx, int miny, int maxx, int maxy)
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
	SDL_Surface *itm=zoomSurface(xpm, xscl, yscl, 0);
//	SDL_FreeSurface(tmp);
	SDL_SetColorKey(itm, SDL_SRCCOLORKEY, SDL_MapRGB(itm->format, 255,255,255));
	SDL_Rect loc;
	loc.x=(int)(minx*640.0/280.0);
	loc.y=(int)(miny*400.0/192.0);
	loc.w=0;
	loc.h=0;
	SDL_BlitSurface(itm, NULL, screen, &loc);
	SDL_FreeSurface(itm);
	return 0;
}

int U1_drawDngItemTruncX(SDL_Surface *screen, SDL_Surface *xpm, int minx, int miny, int maxx, int maxy)
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
	SDL_Surface *itm=zoomSurface(xpm, yscl, yscl, 0);
	SDL_SetColorKey(itm, SDL_SRCCOLORKEY, SDL_MapRGB(itm->format, 255,255,255));
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

int U1_drawSkeleton(SDL_Surface *screen, int b, int c, TU1_game *game)
{
	float dis=game->dungeon.dis;
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(c-30/dis); int miny=(int)(b-80/dis); 
		int maxx=(int)(c+20/dis); int maxy=b; 
		U1_drawDngItem(screen, game->dungeon.u1skeleton, minx, miny, maxx, maxy);
	}
	else
	{
		hplot(screen, c-23/dis, b, c-15/dis, b, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b,c-15/dis,b-15/dis, game->dungeon.curcolor);
		hplot(screen, c-15/dis,b-15/dis, c-8/dis, b-30/dis, game->dungeon.curcolor);
		hplot(screen, c-8/dis, b-30/dis, c+8/dis,b-30/dis, game->dungeon.curcolor);
		hplot(screen, c+8/dis,b-30/dis, c+15/dis, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-15/dis,c+15/dis, b, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b, c+23/dis, b, game->dungeon.curcolor);
		hplot(screen, c, b-26/dis, c, b-65/dis, game->dungeon.curcolor);
		hplot(screen, c-2/dis+.5, b-38/dis, c+2/dis+.5, b-38/dis, game->dungeon.curcolor);
		hplot(screen, c-3/dis+.5, b-45/dis, c+3/dis+.5, b-45/dis, game->dungeon.curcolor);
		hplot(screen, c-5/dis+.5, b-53/dis, c+5/dis+.5, b-53/dis, game->dungeon.curcolor);
		hplot(screen, c-23/dis, b-56/dis, c-30/dis, b-53/dis, game->dungeon.curcolor);
		hplot(screen, c-30/dis, b-53/dis, c-23/dis, b-45/dis, game->dungeon.curcolor);
		hplot(screen, c-23/dis, b-45/dis, c-23/dis, b-53/dis, game->dungeon.curcolor);
		hplot(screen, c-23/dis, b-53/dis, c-8/dis, b-38/dis, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b-45/dis, c-8/dis, b-60/dis, game->dungeon.curcolor);
		hplot(screen, c-8/dis, b-60/dis, c+8/dis, b-60/dis, game->dungeon.curcolor);
		hplot(screen, c+8/dis, b-60/dis, c+15/dis, b-45/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-45/dis, c+15/dis, b-42/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-42/dis, c+15/dis, b-57/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-42/dis, c+15/dis, b-57/dis, game->dungeon.curcolor);
		hplot(screen, c+12/dis, b-45/dis, c+20/dis, b-45/dis, game->dungeon.curcolor);
		hplot(screen, c, b-75/dis, c-5/dis+.5, b-80/dis, game->dungeon.curcolor);
		hplot(screen, c-5/dis+.5, b-80/dis, c-8/dis, b-75/dis, game->dungeon.curcolor);
		hplot(screen, c-8/dis, b-75/dis, c-5/dis+.5, b-65/dis, game->dungeon.curcolor);
		hplot(screen, c-5/dis+.5, b-65/dis, c+5/dis+.5, b-65/dis, game->dungeon.curcolor);
		hplot(screen, c+5/dis+.5, b-65/dis, c+5/dis+.5, b-68/dis, game->dungeon.curcolor);
		hplot(screen, c+5/dis+.5, b-68/dis, c-5/dis+.5, b-68/dis, game->dungeon.curcolor);
		hplot(screen, c-5/dis+.5, b-68/dis, c-5/dis+.5, b-65/dis, game->dungeon.curcolor);
		hplot(screen, c+5/dis+.5, b-65/dis, c+8/dis, b-75/dis, game->dungeon.curcolor);
		hplot(screen, c+8/dis, b-75/dis, c+5/dis+.5, b-80/dis, game->dungeon.curcolor);
		hplot(screen, c+5/dis+.5, b-80/dis, c-5/dis+.5, b-80/dis, game->dungeon.curcolor);
		hplot(screen, c-5/dis+.5, b-72/dis, c-5/dis+.5, b-72/dis, game->dungeon.curcolor);
		hplot(screen, c+5/dis+.5, b-72/dis, c+5/dis+.5, b-72/dis, game->dungeon.curcolor);
	}
	return 0;
}				 


int U1_drawThief(SDL_Surface *screen, int b, int c, TU1_game *game)
{
	float dis=game->dungeon.dis;
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(c-30/dis);
		int miny=(int)(b-83/dis);
		int maxx=(int)(c+30/dis);
		int maxy=b;
		U1_drawDngItem(screen, game->dungeon.u1thief, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, c, b-56/dis, c, b-8/dis, game->dungeon.curcolor);
		hplot(screen, c, b-8/dis, c+10/dis, b, game->dungeon.curcolor);
		hplot(screen, c+10/dis, b, c+30/dis, b, game->dungeon.curcolor);
		hplot(screen, c+30/dis, b, c+30/dis, b-45/dis, game->dungeon.curcolor);
		hplot(screen, c+30/dis, b-45/dis, c+10/dis, b-64/dis, game->dungeon.curcolor);
		hplot(screen, c+10/dis, b-64/dis, c, b-56/dis, game->dungeon.curcolor);
		hplot(screen, c, b-56/dis, c-10/dis, b-64/dis, game->dungeon.curcolor);
		hplot(screen, c-10/dis, b-64/dis, c-30/dis, b-45/dis, game->dungeon.curcolor);
		hplot(screen, c-30/dis, b-45/dis, c-30/dis, b, game->dungeon.curcolor);
		hplot(screen, c-30/dis, b, c-10/dis, b, game->dungeon.curcolor);
		hplot(screen, c-10/dis, b, c, b-8/dis, game->dungeon.curcolor);
		hplot(screen, c-10/dis, b-64/dis, c-10/dis, b-75/dis, game->dungeon.curcolor);
		hplot(screen, c-10/dis, b-75/dis, c, b-83/dis, game->dungeon.curcolor);
		hplot(screen, c, b-83/dis, c+10/dis, b-75/dis, game->dungeon.curcolor);
		hplot(screen, c+10/dis, b-75/dis, c, b-79/dis, game->dungeon.curcolor);
		hplot(screen, c, b-79/dis, c-10/dis, b-75/dis, game->dungeon.curcolor);
		hplot(screen, c-10/dis, b-75/dis, c, b-60/dis, game->dungeon.curcolor);
		hplot(screen, c, b-60/dis, c+10/dis, b-75/dis, game->dungeon.curcolor);
		hplot(screen, c+10/dis, b-75/dis, c+10/dis, b-64/dis, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawRat(SDL_Surface *screen, int b, int c, TU1_game *game)
{
	float dis=game->dungeon.dis;
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(c-15/dis);
		int miny=(int)(b-40/dis);
		int maxx=(int)(c+20/dis);
		int maxy=b;
		U1_drawDngItem(screen, game->dungeon.u1giantrat, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, c+5/dis, b-30/dis, c, b-25/dis, game->dungeon.curcolor);
		hplot(screen, c, b-25/dis, c-5/dis, b-30/dis, game->dungeon.curcolor);
		hplot(screen, c-5/dis, b-30/dis, c-15/dis, b-5/dis, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b-5/dis, c-10/dis, b, game->dungeon.curcolor);
		hplot(screen, c-10/dis, b, c+10/dis, b, game->dungeon.curcolor);
		hplot(screen, c+10/dis, b, c+15/dis, b-5/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-5/dis,c+20/dis, b-5/dis, game->dungeon.curcolor);
		hplot(screen, c+20/dis, b-5/dis, c+10/dis, b, game->dungeon.curcolor);
		hplot(screen, c+10/dis, b, c+15/dis, b-5/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-5/dis, c+5/dis, b-30/dis, game->dungeon.curcolor);
		hplot(screen, c+5/dis, b-30/dis, c+10/dis, b-40/dis, game->dungeon.curcolor);
		hplot(screen, c+10/dis, b-40/dis, c+3/dis+.5, b-35/dis, game->dungeon.curcolor);
		hplot(screen, c+3/dis+.5, b-35/dis, c-3/dis+.5, b-35/dis, game->dungeon.curcolor);
		hplot(screen, c-3/dis+.5, b-35/dis, c-10/dis, b-40/dis, game->dungeon.curcolor);
		hplot(screen, c-10/dis, b-40/dis, c-5/dis, b-30/dis, game->dungeon.curcolor);
		hplot(screen, c-5/dis, b-33/dis, c-3/dis+.5, b-30/dis, game->dungeon.curcolor);
		hplot(screen, c+5/dis, b-33/dis, c+3/dis+.5, b-30/dis, game->dungeon.curcolor);
		hplot(screen, c-5/dis, b-20/dis, c-5/dis+.5, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c+5/dis, b-20/dis, c+5/dis, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c-7/dis, b-20/dis, c-7/dis, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c+7/dis, b-20/dis, c+7/dis, b-15/dis, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawOrc(SDL_Surface *screen, int b, int c, TU1_game *game)
{
	float dis=game->dungeon.dis;
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(c-23/dis);
		int miny=(int)(b-75/dis);
		int maxx=(int)(c+23/dis);
		int maxy=b;
		U1_drawDngItem(screen, game->dungeon.u1orc, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, c, b, c-15/dis, b, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b, c-8/dis, b-8/dis, game->dungeon.curcolor);
		hplot(screen, c-8/dis, b-8/dis, c-8/dis, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c-8/dis, b-15/dis, c-15/dis, b-23/dis, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b-23/dis, c-15/dis, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b-15/dis, c-23/dis, b-23/dis, game->dungeon.curcolor);
		hplot(screen, c-23/dis, b-23/dis, c-23/dis, b-45/dis, game->dungeon.curcolor);
		hplot(screen, c-23/dis, b-45/dis, c-15/dis, b-53/dis, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b-53/dis, c-8/dis, b-53/dis, game->dungeon.curcolor);
		hplot(screen, c-8/dis, b-53/dis, c-15/dis, b-68/dis, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b-68/dis, c-8/dis, b-75/dis, game->dungeon.curcolor);
		hplot(screen, c-8/dis, b-75/dis, c, b-75/dis, game->dungeon.curcolor);
		hplot(screen, c, b, c+15/dis, b, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b, c+8/dis, b-8/dis, game->dungeon.curcolor);
		hplot(screen, c+8/dis, b-8/dis, c+8/dis, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c+8/dis, b-15/dis, c+15/dis, b-23/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-23/dis, c+15/dis, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-15/dis, c+23/dis, b-23/dis, game->dungeon.curcolor);
		hplot(screen, c+23/dis, b-23/dis, c+23/dis, b-45/dis, game->dungeon.curcolor);
		hplot(screen, c+23/dis, b-45/dis, c+15/dis, b-53/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-53/dis, c+8/dis, b-53/dis, game->dungeon.curcolor);
		hplot(screen, c+8/dis, b-53/dis, c+15/dis, b-68/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-68/dis, c+8/dis, b-75/dis, game->dungeon.curcolor);
		hplot(screen, c+8/dis, b-75/dis, c, b-75/dis, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b-68/dis, c+15/dis, b-68/dis, game->dungeon.curcolor);
		hplot(screen, c-8/dis, b-53/dis, c+8/dis, b-53/dis, game->dungeon.curcolor);
		hplot(screen, c-23/dis, b-15/dis, c+8/dis, b-45/dis, game->dungeon.curcolor);
		hplot(screen, c-8/dis, b-68/dis, c, b-60/dis, game->dungeon.curcolor);
		hplot(screen, c, b-60/dis, c+8/dis, b-68/dis, game->dungeon.curcolor);
		hplot(screen, c+8/dis, b-68/dis, c+8/dis, b-60/dis, game->dungeon.curcolor);
		hplot(screen, c+8/dis, b-60/dis, c-8/dis, b-60/dis, game->dungeon.curcolor);
		hplot(screen, c-8/dis, b-60/dis, c-8/dis, b-68/dis, game->dungeon.curcolor);
		hplot(screen, c, b-38/dis, c-8/dis, b-38/dis, game->dungeon.curcolor);
		hplot(screen, c-8/dis, b-38/dis, c+8/dis, b-53/dis, game->dungeon.curcolor);
		hplot(screen, c+8/dis, b-53/dis, c+8/dis, b-45/dis, game->dungeon.curcolor);
		hplot(screen, c+8/dis, b-45/dis, c+15/dis, b-45/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-45/dis, c, b-30/dis, game->dungeon.curcolor);
		hplot(screen, c, b-30/dis, c, b-38/dis, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawViper(SDL_Surface *screen, int b, int c, TU1_game *game)
{
	float dis=game->dungeon.dis;
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(c-15/dis);
		int miny=(int)(b-45/dis);
		int maxx=(int)(c+15/dis);
		int maxy=b;
		U1_drawDngItem(screen, game->dungeon.u1viper, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, c-10/dis, b-15/dis, c-10/dis, b-30/dis, game->dungeon.curcolor);
		hplot(screen, c-10/dis, b-30/dis, c-15/dis, b-20/dis, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b-20/dis, c-15/dis, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b-15/dis, c-15/dis, b, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b, c+15/dis, b, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b, c+15/dis, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-15/dis, c-15/dis, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c-15/dis, b-10/dis, c+15/dis, b-10/dis, game->dungeon.curcolor);
		hplot(screen, c+15/dis, b-5/dis, c+15/dis, b-5/dis, game->dungeon.curcolor);
		HPLOT(screen, c, b-15/dis);
		TO(screen, c-5/dis, b-20/dis, game->dungeon.curcolor);
		TO(screen, c-5/dis, b-35/dis, game->dungeon.curcolor);
		TO(screen, c+5/dis, b-35/dis, game->dungeon.curcolor);
		TO(screen, c+5/dis, b-20/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-15/dis, game->dungeon.curcolor);
		hplot(screen, c-5/dis, b-20/dis, c+5/dis, b-20/dis, game->dungeon.curcolor);
		hplot(screen, c-5/dis, b-25/dis, c+5/dis, b-25/dis, game->dungeon.curcolor);
		hplot(screen, c-5/dis, b-30/dis, c+5/dis, b-30/dis, game->dungeon.curcolor); 
		HPLOT(screen, c-10/dis, b-35/dis);
		TO(screen, c-10/dis, b-40/dis, game->dungeon.curcolor);
		TO(screen, c-5/dis, b-45/dis, game->dungeon.curcolor);
		TO(screen, c+5/dis, b-45/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-40/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-35/dis, game->dungeon.curcolor);
		HPLOT(screen, c-10/dis, b-40/dis);
		TO(screen, c, b-45/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-40/dis, game->dungeon.curcolor);
		HPLOT(screen, c-5/dis, b-40/dis);
		TO(screen, c+5/dis, b-40/dis, game->dungeon.curcolor);
		TO(screen, c+15/dis, b-30/dis, game->dungeon.curcolor);
		TO(screen, c, b-40/dis, game->dungeon.curcolor);
		TO(screen, c-15/dis, b-30/dis, game->dungeon.curcolor);
		TO(screen, c-5/dis+.5, b-40/dis, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawCrawler(SDL_Surface *screen, int b, int c, TU1_game *game)
{
	float dis=game->dungeon.dis;
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(c-30/dis);
		int miny=(int)(79-game->dungeon.yy[(int)dis]);
		int maxx=(int)(c+30/dis);
		int maxy=(int)(b-40/dis);
		U1_drawDngItem(screen, game->dungeon.u1crawler, minx, miny, maxx, maxy);		
	}
	else
	{
		HPLOT(screen, c-20/dis, 79-game->dungeon.yy[(int)dis]);
		TO(screen, c-20/dis, b-88/dis, game->dungeon.curcolor);
		TO(screen, c-10/dis, b-83/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-83/dis, game->dungeon.curcolor);
		TO(screen, c+20/dis, b-88/dis, game->dungeon.curcolor);
		TO(screen, c+20/dis, 79-game->dungeon.yy[(int)dis], game->dungeon.curcolor);
		TO(screen, c-20/dis, 79-game->dungeon.yy[(int)dis], game->dungeon.curcolor);
		HPLOT(screen, c-20/dis, b-88/dis);
		TO(screen, c-30/dis, b-83/dis, game->dungeon.curcolor);
		TO(screen, c-30/dis, b-78/dis, game->dungeon.curcolor);
		HPLOT(screen, c+20/dis, b-88/dis);
		TO(screen, c+30/dis, b-83/dis, game->dungeon.curcolor);
		TO(screen, c+40/dis, b-83/dis, game->dungeon.curcolor);
		HPLOT(screen, c-15/dis, b-86/dis);
		TO(screen, c-20/dis, b-83/dis, game->dungeon.curcolor);
		TO(screen, c-20/dis, b-78/dis, game->dungeon.curcolor);
		TO(screen, c-30/dis, b-73/dis, game->dungeon.curcolor);
		TO(screen, c-30/dis, b-68/dis, game->dungeon.curcolor);
		TO(screen, c-20/dis, b-63/dis, game->dungeon.curcolor);
		HPLOT(screen, c-10/dis, b-83/dis);
		TO(screen, c-10/dis, b-58/dis, game->dungeon.curcolor);
		TO(screen, c, b-50/dis, game->dungeon.curcolor);
		HPLOT(screen, c+10/dis, b-83/dis);
		TO(screen, c+10/dis, b-78/dis, game->dungeon.curcolor);
		TO(screen, c+20/dis, b-73/dis, game->dungeon.curcolor);
		TO(screen, c+20/dis, b-40/dis, game->dungeon.curcolor);
		HPLOT(screen, c+15/dis, b-85/dis);
		TO(screen, c+20/dis, b-78/dis, game->dungeon.curcolor);
		TO(screen, c+30/dis, b-76/dis, game->dungeon.curcolor);
		TO(screen, c+30/dis, b-60/dis, game->dungeon.curcolor);
		HPLOT(screen, c, b-83/dis);
		TO(screen, c, b-73/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-68/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-63/dis, game->dungeon.curcolor);
		TO(screen, c, b-58/dis, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawGremlin(SDL_Surface *screen, int b, int c, TU1_game *game)
{
	float dis=game->dungeon.dis;
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(c-8/dis);
		int miny=(int)(b-20/dis);
		int maxx=(int)(c+10/dis);
		int maxy=(int)b;
		U1_drawDngItem(screen, game->dungeon.u1gremlin, minx, miny, maxx, maxy);		
	}
	else
	{
		HPLOT(screen, c+5/dis+.5, b-10/dis);
		TO(screen, c-5/dis+.5, b-10/dis, game->dungeon.curcolor);
		TO(screen, c, b-15/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-20/dis, game->dungeon.curcolor);
		TO(screen, c+5/dis+.5, b-15/dis, game->dungeon.curcolor);
		TO(screen, c+5/dis+.5, b-10/dis, game->dungeon.curcolor);
		TO(screen, c+7/dis+.5, b-6/dis, game->dungeon.curcolor);
		TO(screen, c+5/dis+.5, b-3/dis, game->dungeon.curcolor);
		TO(screen, c-5/dis+.5, b-3/dis, game->dungeon.curcolor);
		TO(screen, c-7/dis+.5, b-6/dis, game->dungeon.curcolor);
		TO(screen, c-5/dis+.5, b-10/dis, game->dungeon.curcolor); 
		HPLOT(screen, c+2/dis+.5, b-3/dis);
		TO(screen, c+5/dis+.5, b, game->dungeon.curcolor);
		TO(screen, c+8/dis, b, game->dungeon.curcolor);
		HPLOT(screen, c-2/dis+.5, b-3/dis);
		TO(screen, c-5/dis+.5, b, game->dungeon.curcolor);
		TO(screen, c-8/dis, b, game->dungeon.curcolor);
		hplot(screen, c+3/dis+.5, b-8/dis, c+3/dis+.5, b-8/dis, game->dungeon.curcolor);
		hplot(screen, c-3/dis+.5, b-8/dis, c-3/dis+.5, b-8/dis, game->dungeon.curcolor);
		hplot(screen, c+3/dis+.5, b-5/dis, c-3/dis+.5, b-5/dis, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawMimic(SDL_Surface *screen, int b, int c, TU1_game *game)
{
	float dis=game->dungeon.dis;
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(139-10/dis);
		int miny=(int)(game->dungeon.per[game->dungeon.dis][3]-15/dis);
		int maxx=(int)(139+15/dis);
		int maxy=(int)(game->dungeon.per[game->dungeon.dis][3]);
		U1_drawDngItem(screen, game->dungeon.u1mimic, minx, miny, maxx, maxy);		
	}
	else
	{
		HPLOT(screen, 139-10/dis, game->dungeon.per[game->dungeon.dis][3]);
		TO(screen, 139-10/dis, game->dungeon.per[game->dungeon.dis][3]-10/dis, game->dungeon.curcolor);
		TO(screen, 139+10/dis, game->dungeon.per[game->dungeon.dis][3]-10/dis, game->dungeon.curcolor);
		TO(screen, 139+10/dis, game->dungeon.per[game->dungeon.dis][3], game->dungeon.curcolor);
		TO(screen, 139-10/dis, game->dungeon.per[game->dungeon.dis][3], game->dungeon.curcolor);
		HPLOT(screen, 139-10/dis, game->dungeon.per[game->dungeon.dis][3]-10/dis);
		TO(screen, 139-5/dis, game->dungeon.per[game->dungeon.dis][3]-15/dis, game->dungeon.curcolor);
		TO(screen, 139+15/dis, game->dungeon.per[game->dungeon.dis][3]-15/dis, game->dungeon.curcolor);
		TO(screen, 139+15/dis, game->dungeon.per[game->dungeon.dis][3]-5/dis, game->dungeon.curcolor);
		TO(screen, 139+10/dis, game->dungeon.per[game->dungeon.dis][3], game->dungeon.curcolor);
		hplot(screen, 139+10/dis, game->dungeon.per[game->dungeon.dis][3]-10/dis, 139+15/dis, game->dungeon.per[game->dungeon.dis][3]-15/dis, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawDaemon(SDL_Surface *screen, int b, int c, TU1_game *game)
{
	float dis=game->dungeon.dis;
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(c-40/dis);
		int miny=(int)(b-70/dis);
		int maxx=(int)(c+50/dis);
		int maxy=(int)(b);
		U1_drawDngItem(screen, game->dungeon.u1daemon, minx, miny, maxx, maxy);		
	}
	else
	{
		HPLOT(screen, c-14/dis, b-46/dis);
		TO(screen, c-12/dis, b-37/dis, game->dungeon.curcolor);
		TO(screen, c-20/dis, b-32/dis, game->dungeon.curcolor);
		TO(screen, c-30/dis, b-32/dis, game->dungeon.curcolor);
		TO(screen, c-22/dis, b-24/dis, game->dungeon.curcolor);
		TO(screen, c-40/dis, b-17/dis, game->dungeon.curcolor);
		TO(screen, c-40/dis, b-7/dis, game->dungeon.curcolor);
		TO(screen, c-38/dis, b-5/dis, game->dungeon.curcolor); 
		TO(screen, c-40/dis, b-3/dis, game->dungeon.curcolor);
		TO(screen, c-40/dis, b, game->dungeon.curcolor);
		TO(screen, c-36/dis, b, game->dungeon.curcolor);
		TO(screen, c-34/dis, b-2/dis, game->dungeon.curcolor);
		TO(screen, c-32/dis, b, game->dungeon.curcolor);
		TO(screen, c-28/dis, b, game->dungeon.curcolor);
		TO(screen, c-28/dis, b-3/dis, game->dungeon.curcolor);
		TO(screen, c-30/dis, b-5/dis, game->dungeon.curcolor);
		TO(screen, c-28/dis, b-7/dis, game->dungeon.curcolor);
		TO(screen, c-28/dis, b-15/dis, game->dungeon.curcolor);
		TO(screen, c, b-27/dis, game->dungeon.curcolor);
		HPLOT(screen, c+14/dis, b-46/dis);
		TO(screen, c+12/dis, b-37/dis, game->dungeon.curcolor);
		TO(screen, c+20/dis, b-32/dis, game->dungeon.curcolor);
		TO(screen, c+30/dis, b-32/dis, game->dungeon.curcolor);
		TO(screen, c+22/dis, b-24/dis, game->dungeon.curcolor);
		TO(screen, c+40/dis, b-17/dis, game->dungeon.curcolor);
		TO(screen, c+40/dis, b-7/dis, game->dungeon.curcolor);
		TO(screen, c+38/dis, b-5/dis, game->dungeon.curcolor);
		TO(screen, c+40/dis, b-3/dis, game->dungeon.curcolor);
		TO(screen, c+40/dis, b, game->dungeon.curcolor);
		TO(screen, c+36/dis, b, game->dungeon.curcolor);
		TO(screen, c+34/dis, b-2/dis, game->dungeon.curcolor);
		TO(screen, c+32/dis, b, game->dungeon.curcolor);
		TO(screen, c+28/dis, b, game->dungeon.curcolor);
		TO(screen, c+28/dis, b-3/dis, game->dungeon.curcolor);
		TO(screen, c+30/dis, b-5/dis, game->dungeon.curcolor);
		TO(screen, c+28/dis, b-7/dis, game->dungeon.curcolor);
		TO(screen, c+28/dis, b-15/dis, game->dungeon.curcolor);
		TO(screen, c, b-27/dis, game->dungeon.curcolor);				
		HPLOT(screen, c+6/dis, b-48/dis);
		TO(screen, c+38/dis, b-27/dis, game->dungeon.curcolor);
		TO(screen, c+38/dis, b-41/dis, game->dungeon.curcolor);
		TO(screen, c+40/dis, b-42/dis, game->dungeon.curcolor);
		TO(screen, c+18/dis, b-56/dis, game->dungeon.curcolor);
		TO(screen, c+12/dis, b-56/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-57/dis, game->dungeon.curcolor);
		TO(screen, c+8/dis, b-56/dis, game->dungeon.curcolor);
		TO(screen, c-8/dis, b-56/dis, game->dungeon.curcolor);
		TO(screen, c-10/dis, b-58/dis, game->dungeon.curcolor);
		TO(screen, c+14/dis, b-58/dis, game->dungeon.curcolor);
		TO(screen, c+16/dis, b-59/dis, game->dungeon.curcolor);
		TO(screen, c+8/dis, b-63/dis, game->dungeon.curcolor);
		TO(screen, c+6/dis, b-63/dis, game->dungeon.curcolor);
		TO(screen, c+2/dis+.5, b-70/dis, game->dungeon.curcolor);
		TO(screen, c+2/dis+.5, b-63/dis, game->dungeon.curcolor);
		TO(screen, c-2/dis+.5, b-63/dis, game->dungeon.curcolor);
		TO(screen, c-2/dis+.5, b-70/dis, game->dungeon.curcolor);
		TO(screen, c-6/dis, b-63/dis, game->dungeon.curcolor);
		TO(screen, c-8/dis, b-63/dis, game->dungeon.curcolor);
		TO(screen, c-16/dis, b-59/dis, game->dungeon.curcolor);
		TO(screen, c-14/dis, b-58/dis, game->dungeon.curcolor);
		TO(screen, c-10/dis, b-57/dis, game->dungeon.curcolor);
		TO(screen, c-12/dis, b-56/dis, game->dungeon.curcolor);
		TO(screen, c-18/dis, b-56/dis, game->dungeon.curcolor);
		TO(screen, c-36/dis, b-47/dis, game->dungeon.curcolor);
		TO(screen, c-36/dis, b-39/dis, game->dungeon.curcolor);
		TO(screen, c-28/dis, b-41/dis, game->dungeon.curcolor);
		TO(screen, c-28/dis, b-46/dis, game->dungeon.curcolor);
		TO(screen, c-20/dis, b-50/dis, game->dungeon.curcolor);
		TO(screen, c-18/dis, b-50/dis, game->dungeon.curcolor);
		TO(screen, c-14/dis, b-46/dis, game->dungeon.curcolor);
		U1_draw3087(screen, c, b, dis, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawBalrog(SDL_Surface *screen, int b, int c, TU1_game *game)
{
	float dis=game->dungeon.dis;
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(c-60/dis);
		int miny=(int)(b-92/dis);
		int maxx=(int)(c+60/dis);
		int maxy=(int)(b);
		U1_drawDngItem(screen, game->dungeon.u1balrog, minx, miny, maxx, maxy);		
	}
	else
	{
		HPLOT(screen, c+6/dis, b-60/dis);
		TO(screen, c+30/dis, b-90/dis, game->dungeon.curcolor);
		TO(screen, c+60/dis, b-30/dis, game->dungeon.curcolor);
		TO(screen, c+60/dis, b-10/dis, game->dungeon.curcolor);
		TO(screen, c+30/dis, b-40/dis, game->dungeon.curcolor);
		TO(screen, c+15/dis, b-40/dis, game->dungeon.curcolor);
		HPLOT(screen, c-6/dis, b-60/dis);
		TO(screen, c-30/dis, b-90/dis, game->dungeon.curcolor);
		TO(screen, c-60/dis, b-30/dis, game->dungeon.curcolor);
		TO(screen, c-60/dis, b-10/dis, game->dungeon.curcolor);
		TO(screen, c-30/dis, b-40/dis, game->dungeon.curcolor);
		TO(screen, c-15/dis, b-40/dis, game->dungeon.curcolor);
		HPLOT(screen, c, b-25/dis);
		TO(screen, c+6/dis, b-25/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-20/dis, game->dungeon.curcolor);
		TO(screen, c+12/dis, b-10/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-6/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b, game->dungeon.curcolor);
		TO(screen, c+14/dis, b, game->dungeon.curcolor);
		TO(screen, c+15/dis, b-5/dis, game->dungeon.curcolor);
		TO(screen, c+16/dis, b, game->dungeon.curcolor);
		TO(screen, c+20/dis, b, game->dungeon.curcolor);
		TO(screen, c+20/dis, b-6/dis, game->dungeon.curcolor);
		TO(screen, c+18/dis, b-10/dis, game->dungeon.curcolor);
		TO(screen, c+18/dis, b-20/dis, game->dungeon.curcolor);
		TO(screen, c+15/dis, b-30/dis, game->dungeon.curcolor);
		TO(screen, c+15/dis, b-45/dis, game->dungeon.curcolor);
		TO(screen, c+40/dis, b-60/dis, game->dungeon.curcolor);
		TO(screen, c+40/dis, b-70/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-55/dis, game->dungeon.curcolor);
		TO(screen, c+6/dis, b-60/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b-74/dis, game->dungeon.curcolor);
		TO(screen, c+6/dis, b-80/dis, game->dungeon.curcolor);
		TO(screen, c+4/dis+.5, b-80/dis, game->dungeon.curcolor);
		TO(screen, c+3/dis+.5, b-82/dis, game->dungeon.curcolor);
		TO(screen, c+2/dis+.5, b-80/dis, game->dungeon.curcolor);
		TO(screen, c, b-80/dis, game->dungeon.curcolor);
		HPLOT(screen, c, b-25/dis);
		TO(screen, c-6/dis, b-25/dis, game->dungeon.curcolor);
		TO(screen, c-10/dis, b-20/dis, game->dungeon.curcolor);
		TO(screen, c-12/dis, b-10/dis, game->dungeon.curcolor);
		TO(screen, c-10/dis, b-6/dis, game->dungeon.curcolor);
		TO(screen, c-10/dis, b, game->dungeon.curcolor);
		TO(screen, c-14/dis, b, game->dungeon.curcolor);
		TO(screen, c-15/dis, b-5/dis, game->dungeon.curcolor);
		TO(screen, c-16/dis, b, game->dungeon.curcolor);
		TO(screen, c-20/dis, b, game->dungeon.curcolor);
		TO(screen, c-20/dis, b-6/dis, game->dungeon.curcolor);
		TO(screen, c-18/dis, b-10/dis, game->dungeon.curcolor);
		TO(screen, c-18/dis, b-20/dis, game->dungeon.curcolor);
		TO(screen, c-15/dis, b-30/dis, game->dungeon.curcolor);
		TO(screen, c-15/dis, b-45/dis, game->dungeon.curcolor);
		TO(screen, c-40/dis, b-60/dis, game->dungeon.curcolor);
		TO(screen, c-40/dis, b-70/dis, game->dungeon.curcolor);
		TO(screen, c-10/dis, b-55/dis, game->dungeon.curcolor);
		TO(screen, c-6/dis, b-60/dis, game->dungeon.curcolor);
		TO(screen, c-10/dis, b-74/dis, game->dungeon.curcolor);
		TO(screen, c-6/dis, b-80/dis, game->dungeon.curcolor);
		TO(screen, c-4/dis+.5, b-80/dis, game->dungeon.curcolor);
		TO(screen, c-3/dis+.5, b-82/dis, game->dungeon.curcolor);
		TO(screen, c-2/dis+.5, b-80/dis, game->dungeon.curcolor);
		TO(screen, c, b-80/dis, game->dungeon.curcolor);
		HPLOT(screen, c-6/dis, b-25/dis);
		TO(screen, c, b-6/dis, game->dungeon.curcolor);
		TO(screen, c+10/dis, b, game->dungeon.curcolor);
		TO(screen, c+4/dis+.5, b-8/dis, game->dungeon.curcolor);
		TO(screen, c+6/dis, b-25/dis, game->dungeon.curcolor);
		HPLOT(screen, c-40/dis, b-64/dis);
		TO(screen, c-40/dis, b-90/dis, game->dungeon.curcolor);
		TO(screen, c-52/dis, b-80/dis, game->dungeon.curcolor);
		TO(screen, c-52/dis, b-40/dis, game->dungeon.curcolor);
		HPLOT(screen, c+40/dis, b-86/dis);
		TO(screen, c+38/dis, b-92/dis, game->dungeon.curcolor);
		TO(screen, c+42/dis, b-92/dis, game->dungeon.curcolor);
		TO(screen, c+40/dis, b-86/dis, game->dungeon.curcolor);
		TO(screen, c+40/dis, b-50/dis, game->dungeon.curcolor);
		hplot(screen, c+4/dis+.5, b-70/dis, c+6/dis, b-74/dis, game->dungeon.curcolor);
		hplot(screen, c-4/dis+.5, b-70/dis, c-6/dis, b-74/dis, game->dungeon.curcolor);
		hplot(screen, c, b-64/dis, c, b-60/dis, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawChest(SDL_Surface *screen, TU1_game *game)
{
	float dis=game->dungeon.dis;
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(139-10/dis);
		int miny=(int)(game->dungeon.per[game->dungeon.dis][3]-15/dis);
		int maxx=(int)(139+15/dis);
		int maxy=(int)(game->dungeon.per[game->dungeon.dis][3]);
		U1_drawDngItem(screen, game->dungeon.u1chest, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, 139-10/dis, game->dungeon.per[game->dungeon.dis][3], 139-10/dis, game->dungeon.per[game->dungeon.dis][3]-10/dis, game->dungeon.curcolor);
		hplot(screen, 139-10/dis, game->dungeon.per[game->dungeon.dis][3]-10/dis, 139+10/dis, game->dungeon.per[game->dungeon.dis][3]-10/dis, game->dungeon.curcolor);
		hplot(screen, 139+10/dis, game->dungeon.per[game->dungeon.dis][3]-10/dis, 139+10/dis, game->dungeon.per[game->dungeon.dis][3], game->dungeon.curcolor);
		hplot(screen, 139+10/dis, game->dungeon.per[game->dungeon.dis][3], 139-10/dis, game->dungeon.per[game->dungeon.dis][3], game->dungeon.curcolor);
		hplot(screen, 139-10/dis, game->dungeon.per[game->dungeon.dis][3]-10/dis, 139-5/dis, game->dungeon.per[game->dungeon.dis][3]-15/dis, game->dungeon.curcolor);
		hplot(screen, 139-5/dis, game->dungeon.per[game->dungeon.dis][3]-15/dis, 139+15/dis, game->dungeon.per[game->dungeon.dis][3]-15/dis, game->dungeon.curcolor);
		hplot(screen, 139+15/dis, game->dungeon.per[game->dungeon.dis][3]-15/dis, 139+15/dis, game->dungeon.per[game->dungeon.dis][3]-5/dis, game->dungeon.curcolor);
		hplot(screen, 139+15/dis, game->dungeon.per[game->dungeon.dis][3]-5/dis, 139+10/dis, game->dungeon.per[game->dungeon.dis][3], game->dungeon.curcolor);
		hplot(screen, 139+10/dis, game->dungeon.per[game->dungeon.dis][3]-10/dis, 139+15/dis, game->dungeon.per[game->dungeon.dis][3]-15/dis, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawLadder(SDL_Surface *screen, TU1_game *game)
{
	int base=(int)game->dungeon.lad[game->dungeon.dis][3];
	int tp=(int)game->dungeon.lad[game->dungeon.dis][2];
	int lx=(int)game->dungeon.lad[game->dungeon.dis][0];
	int rx=(int)game->dungeon.lad[game->dungeon.dis][1];
//	hplot(screen, lx, base, lx, tp, game->dungeon.curcolor);
//	hplot(screen, rx, tp, rx, base, game->dungeon.curcolor);
	int y1=(int)((double)(base*4+tp)/5.0);
	int y2=(int)((double)(base*3+tp*2)/5.0);
	int y3=(int)((double)(base*2+tp*3)/5.0);
	int y4=(int)((double)(base+tp*4)/5.0);
	if(game->config.game[0].svga==1)
	{
		int minx=lx;
		int miny=tp;
		int maxx=rx;
		int maxy=base;
		U1_drawDngItem(screen, game->dungeon.u1ladder, minx, miny, maxx, maxy);
	}
	else
	{
		hplot(screen, lx, base, lx, tp, game->dungeon.curcolor);
		hplot(screen, rx, tp, rx, base, game->dungeon.curcolor);
		hplot(screen, lx, y1, rx, y1, game->dungeon.curcolor);
		hplot(screen, lx, y2, rx, y2, game->dungeon.curcolor);
		hplot(screen, lx, y3, rx, y3, game->dungeon.curcolor);
		hplot(screen, lx, y4, rx, y4, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawTopHole(SDL_Surface *screen, TU1_game *game)
{
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(game->dungeon.ft[game->dungeon.dis][0]);
		int miny=(int)(158-game->dungeon.ft[game->dungeon.dis][4]);
		int maxx=(int)(game->dungeon.ft[game->dungeon.dis][1]);
		int maxy=(int)(158-game->dungeon.ft[game->dungeon.dis][5]);
		U1_drawDngItem(screen, game->dungeon.u1tophole, minx, miny, maxx, maxy);
	}
	else
	{

		hplot(screen, (int)game->dungeon.ft[game->dungeon.dis][0], 158-(int)game->dungeon.ft[game->dungeon.dis][4], (int)game->dungeon.ft[game->dungeon.dis][2], 158-(int)game->dungeon.ft[game->dungeon.dis][5], game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.ft[game->dungeon.dis][2], 158-(int)game->dungeon.ft[game->dungeon.dis][5], (int)game->dungeon.ft[game->dungeon.dis][3], 158-(int)game->dungeon.ft[game->dungeon.dis][5], game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.ft[game->dungeon.dis][3], 158-(int)game->dungeon.ft[game->dungeon.dis][5], (int)game->dungeon.ft[game->dungeon.dis][1], 158-(int)game->dungeon.ft[game->dungeon.dis][4], game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.ft[game->dungeon.dis][1], 158-(int)game->dungeon.ft[game->dungeon.dis][4], (int)game->dungeon.ft[game->dungeon.dis][0], 158-(int)game->dungeon.ft[game->dungeon.dis][4], game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawBottomHole(SDL_Surface *screen, TU1_game *game)
{
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(game->dungeon.ft[game->dungeon.dis][0]);
		int miny=(int)(game->dungeon.ft[game->dungeon.dis][5]);
		int maxx=(int)(game->dungeon.ft[game->dungeon.dis][1]);
		int maxy=(int)(game->dungeon.ft[game->dungeon.dis][4]);
		U1_drawDngItem(screen, game->dungeon.u1trapdoor, minx, miny, maxx, maxy);
	}
	else
	{
		hplot(screen, (int)game->dungeon.ft[game->dungeon.dis][0], (int)game->dungeon.ft[game->dungeon.dis][4], (int)game->dungeon.ft[game->dungeon.dis][2], (int)game->dungeon.ft[game->dungeon.dis][5], game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.ft[game->dungeon.dis][2], (int)game->dungeon.ft[game->dungeon.dis][5], (int)game->dungeon.ft[game->dungeon.dis][3], (int)game->dungeon.ft[game->dungeon.dis][5], game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.ft[game->dungeon.dis][3], (int)game->dungeon.ft[game->dungeon.dis][5], (int)game->dungeon.ft[game->dungeon.dis][1], (int)game->dungeon.ft[game->dungeon.dis][4], game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.ft[game->dungeon.dis][1], (int)game->dungeon.ft[game->dungeon.dis][4], (int)game->dungeon.ft[game->dungeon.dis][0], (int)game->dungeon.ft[game->dungeon.dis][4], game->dungeon.curcolor);
	}		
	return 0;
}

int U1_drawLeftWallPerp(SDL_Surface *screen, TU1_game *game)
{
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(game->dungeon.l1);
		int miny=(int)(game->dungeon.t2);
		int maxx=(int)(game->dungeon.l2)+1;
		int maxy=(int)(game->dungeon.b2);
		U1_drawDngItemTruncX(screen, game->dungeon.wall_b, minx, miny, maxx, maxy);
	}
	else
	{
		if(game->dungeon.dis!=0)
			hplot(screen, game->dungeon.l1, game->dungeon.t1, game->dungeon.l1, game->dungeon.b1, game->dungeon.curcolor);
		hplot(screen, game->dungeon.l1, game->dungeon.t2, game->dungeon.l2, game->dungeon.t2, game->dungeon.curcolor);
		hplot(screen, game->dungeon.l2, game->dungeon.t2, game->dungeon.l2, game->dungeon.b2, game->dungeon.curcolor);
		hplot(screen, game->dungeon.l2, game->dungeon.b2, game->dungeon.l1, game->dungeon.b2, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawRightWallPerp(SDL_Surface *screen, TU1_game *game)
{
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(game->dungeon.r2);
		int miny=(int)(game->dungeon.t2);
		int maxx=(int)(game->dungeon.r1);
		int maxy=(int)(game->dungeon.b2);
		U1_drawDngItemTruncX(screen, game->dungeon.wall_b, minx, miny, maxx, maxy);		
	}
	else
	{
		if(game->dungeon.dis!=0)
			hplot(screen, game->dungeon.r1, game->dungeon.t1, game->dungeon.r1, game->dungeon.b1, game->dungeon.curcolor);
		hplot(screen, game->dungeon.r1, game->dungeon.t2, game->dungeon.r2, game->dungeon.t2, game->dungeon.curcolor);
		hplot(screen, game->dungeon.r2, game->dungeon.t2, game->dungeon.r2, game->dungeon.b2, game->dungeon.curcolor);
		hplot(screen, game->dungeon.r2, game->dungeon.b2, game->dungeon.r1, game->dungeon.b2, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawRightDoor0(SDL_Surface *screen, TU1_game *game)
{
	if(game->config.game[0].svga==1)
	{	
		int minx=(int)(273-game->dungeon.ld[game->dungeon.dis][1]);
		int miny=(int)(game->dungeon.ld[game->dungeon.dis][2]-3);
		int maxx=(int)(279);
		int maxy=(int)(game->dungeon.b1+2);
		U1_drawDngItem(screen, game->dungeon.door_r, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, 279, (int)game->dungeon.ld[game->dungeon.dis][2]-3, 279-(int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][3], game->dungeon.curcolor);
		hplot(screen, 279-(int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][3], 279-(int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][5], game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawLeftDoor0(SDL_Surface *screen, TU1_game *game)
{
	if(game->config.game[0].svga==1)
	{	
		int minx=(int)(0);
		int miny=(int)(game->dungeon.ld[game->dungeon.dis][2]-3);
		int maxx=(int)(game->dungeon.ld[game->dungeon.dis][1]+6);
		int maxy=(int)(game->dungeon.b1+1);
		U1_drawDngItem(screen, game->dungeon.door_l, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, 0, (int)game->dungeon.ld[game->dungeon.dis][2]-3, (int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][3], game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][3], (int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][5], game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawRightDoor(SDL_Surface *screen, TU1_game *game)
{
	if(game->config.game[0].svga==1)
	{	
		int minx=(int)(279-game->dungeon.ld[game->dungeon.dis][1]-6/game->dungeon.dis); int
		miny=(int)(game->dungeon.ld[game->dungeon.dis][2]); int
		maxx=(int)(279-game->dungeon.ld[game->dungeon.dis][0]+6/game->dungeon.dis); int
		maxy=(int)(game->dungeon.ld[game->dungeon.dis][4]+8/game->dungeon.dis);
		U1_drawDngItem(screen, game->dungeon.door_r, minx, miny, maxx, maxy);
	}
	else
	{
		hplot(screen, 279-(int)game->dungeon.ld[game->dungeon.dis][0], (int)game->dungeon.ld[game->dungeon.dis][4], 279-(int)game->dungeon.ld[game->dungeon.dis][0], (int)game->dungeon.ld[game->dungeon.dis][2], game->dungeon.curcolor);
		hplot(screen, 279-(int)game->dungeon.ld[game->dungeon.dis][0], (int)game->dungeon.ld[game->dungeon.dis][2], 279-(int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][3], game->dungeon.curcolor);
		hplot(screen, 279-(int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][3], 279-(int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][5], game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawLeftDoor(SDL_Surface *screen, TU1_game *game)
{//FOO!@!
	if(game->config.game[0].svga==1)
	{	
		int minx=(int)(game->dungeon.ld[game->dungeon.dis][0]-6/game->dungeon.dis);
		int miny=(int)(game->dungeon.ld[game->dungeon.dis][2]);
		int maxx=(int)(game->dungeon.ld[game->dungeon.dis][1]+6/game->dungeon.dis);
		int maxy=(int)(game->dungeon.ld[game->dungeon.dis][4]+8/game->dungeon.dis);
		U1_drawDngItem(screen, game->dungeon.door_l, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, (int)game->dungeon.ld[game->dungeon.dis][0], (int)game->dungeon.ld[game->dungeon.dis][4], (int)game->dungeon.ld[game->dungeon.dis][0], (int)game->dungeon.ld[game->dungeon.dis][2], game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.ld[game->dungeon.dis][0], (int)game->dungeon.ld[game->dungeon.dis][2], (int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][3], game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][3], (int)game->dungeon.ld[game->dungeon.dis][1], (int)game->dungeon.ld[game->dungeon.dis][5], game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawRightWall(SDL_Surface *screen, TU1_game *game)
{
	if(game->config.game[0].svga==1)
	{	
		SDL_Surface *wall=game->dungeon.u1rightwall;
		int r1=(int)(game->dungeon.r1*640.0/280.0);
		int r2=(int)(game->dungeon.r2*640.0/280.0);
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
		hplot(screen, (int)game->dungeon.r1, (int)game->dungeon.t1, (int)game->dungeon.r2, (int)game->dungeon.t2, game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.r1, (int)game->dungeon.b1, (int)game->dungeon.r2, (int)game->dungeon.b2, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawLeftWall(SDL_Surface *screen, TU1_game *game)
{
	if(game->config.game[0].svga==1)
	{	
		SDL_Surface *wall=game->dungeon.u1leftwall;
		int l1=(int)(game->dungeon.l1*640.0/280.0);
		int l2=(int)(game->dungeon.l2*640.0/280.0);
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
		hplot(screen, (int)game->dungeon.l1, (int)game->dungeon.t1, (int)game->dungeon.l2, (int)game->dungeon.t2, game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.l1, (int)game->dungeon.b1, (int)game->dungeon.l2, (int)game->dungeon.b2, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawMidDoor(SDL_Surface *screen, TU1_game *game)
{
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(game->dungeon.cd[game->dungeon.dis][0]);
		int miny=(int)(game->dungeon.cd[game->dungeon.dis][2]);
		int maxx=(int)(game->dungeon.cd[game->dungeon.dis][1]);
		int maxy=(int)(game->dungeon.cd[game->dungeon.dis][3]);
		U1_drawDngItem(screen, game->dungeon.door_b, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, (int)game->dungeon.cd[game->dungeon.dis][0], (int)game->dungeon.cd[game->dungeon.dis][3], (int)game->dungeon.cd[game->dungeon.dis][0], (int)game->dungeon.cd[game->dungeon.dis][2], game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.cd[game->dungeon.dis][0], (int)game->dungeon.cd[game->dungeon.dis][2], (int)game->dungeon.cd[game->dungeon.dis][1], (int)game->dungeon.cd[game->dungeon.dis][2], game->dungeon.curcolor);
		hplot(screen, (int)game->dungeon.cd[game->dungeon.dis][1], (int)game->dungeon.cd[game->dungeon.dis][2], (int)game->dungeon.cd[game->dungeon.dis][1], (int)game->dungeon.cd[game->dungeon.dis][3], game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawMidWallPerp(SDL_Surface *screen, TU1_game *game)
{
	if(game->config.game[0].svga==1)
	{
		int minx=(int)(game->dungeon.l1);
		int miny=(int)(game->dungeon.t1);
		int maxx=(int)(game->dungeon.r1)+1;
		int maxy=(int)(game->dungeon.b1);
		U1_drawDngItem(screen, game->dungeon.wall_b, minx, miny, maxx, maxy);		
	}
	else
	{
		hplot(screen, game->dungeon.l1, game->dungeon.t1, game->dungeon.r1, game->dungeon.t1, game->dungeon.curcolor);
		hplot(screen, game->dungeon.r1, game->dungeon.t1, game->dungeon.r1, game->dungeon.b1, game->dungeon.curcolor);
		hplot(screen, game->dungeon.r1, game->dungeon.b1, game->dungeon.l1, game->dungeon.b1, game->dungeon.curcolor);
		hplot(screen, game->dungeon.l1, game->dungeon.b1, game->dungeon.l1, game->dungeon.t1, game->dungeon.curcolor);
	}
	return 0;
}

int U1_drawDng(SDL_Surface *screen, TU1_game *game, Tterminal *term)
{
	game->dungeon.dis=1;
	
	if(game->config.game[0].svga==1)
	{
//		SDL_BlitSurface(game->dungeon.u1background, NULL, screen, NULL);
//		SDL_BlitSurface(game->dungeon.u1dngback, NULL, screen, NULL);
	}
	else
		u1cleargr(screen);
		
	while(1)
	{
		if(game->dungeon.dis!=0)
		{
		game->dungeon.cent=game->dungeon.dn[game->dungeon.px+game->dungeon.dx*game->dungeon.dis][game->dungeon.py+game->dungeon.dy*game->dungeon.dis];
		game->dungeon.mc=(int)(game->dungeon.cent/10.0);
		game->dungeon.cent=game->dungeon.cent-game->dungeon.mc*10;
			if((game->dungeon.cent==1)||(game->dungeon.cent==3)||(game->dungeon.cent==4))
			{	//mid wall perp
				game->dungeon.en=1;
			}
		}
		if(game->dungeon.en==1)
		{
			game->dungeon.en=0;
			break;
		}
		game->dungeon.dis++;
	}
	while(1)
	{
		float dis=game->dungeon.dis;
		game->dungeon.cent=game->dungeon.dn[game->dungeon.px+game->dungeon.dx*game->dungeon.dis][game->dungeon.py+game->dungeon.dy*game->dungeon.dis];
		game->dungeon.left=game->dungeon.dn[game->dungeon.px+game->dungeon.dx*game->dungeon.dis+game->dungeon.dy][game->dungeon.py+game->dungeon.dy*game->dungeon.dis-game->dungeon.dx];
		game->dungeon.righ=game->dungeon.dn[game->dungeon.px+game->dungeon.dx*game->dungeon.dis-game->dungeon.dy][game->dungeon.py+game->dungeon.dy*game->dungeon.dis+game->dungeon.dx];	
	
		game->dungeon.l1=game->dungeon.per[game->dungeon.dis][0];
		game->dungeon.r1=game->dungeon.per[game->dungeon.dis][1];
		game->dungeon.t1=game->dungeon.per[game->dungeon.dis][2];
		game->dungeon.b1=game->dungeon.per[game->dungeon.dis][3];
		game->dungeon.l2=game->dungeon.per[game->dungeon.dis+1][0];
		game->dungeon.r2=game->dungeon.per[game->dungeon.dis+1][1];
		game->dungeon.t2=game->dungeon.per[game->dungeon.dis+1][2];
		game->dungeon.b2=game->dungeon.per[game->dungeon.dis+1][3];
		game->dungeon.mc=(int)(game->dungeon.cent/10.0);
		game->dungeon.cent=game->dungeon.cent-game->dungeon.mc*10;
		game->dungeon.left=(int)(((double)game->dungeon.left/10.0-(int)((double)game->dungeon.left/10.0))*10.0+.1);
		game->dungeon.righ=(int)(((double)game->dungeon.righ/10.0-(int)((double)game->dungeon.righ/10.0))*10.0+.1);
		if(dis!=0)
		{
			if((game->dungeon.cent==1)||(game->dungeon.cent==3)||(game->dungeon.cent==4))
			{	//mid wall perp
				U1_drawMidWallPerp(screen, game);
			}
	//		if((game->dungeon.cent==1)||(game->dungeon.cent==3))
	//		{
	//			game->dungeon.en=1;
	//		}
			if(game->dungeon.cent==4)
			{	//mid door
				U1_drawMidDoor(screen, game);
	//			game->dungeon.en=1;
			}
		}

		if(((game->dungeon.cent!=4)&&(game->dungeon.cent!=1)&&(game->dungeon.cent!=3))||(dis==0))
		{
			if((game->dungeon.left==1)||(game->dungeon.left==3)||(game->dungeon.left==4))
			{	//left wall
				U1_drawLeftWall(screen, game);
			}
			if((game->dungeon.righ==1)||(game->dungeon.righ==3)||(game->dungeon.righ==4))
			{	//right wall
				U1_drawRightWall(screen, game);
			}

			if((game->dungeon.left==4) && (dis>0))
			{	//left door
				U1_drawLeftDoor(screen, game);
			}
			if((game->dungeon.left==4) && (dis==0))
			{	//left door dis==0
				U1_drawLeftDoor0(screen, game);
			}
			if((game->dungeon.righ==4) && (dis>0))
			{	//right door
				U1_drawRightDoor(screen, game);
			}
			if((game->dungeon.righ==4) && (dis==0))
			{	//right door dis==0
				U1_drawRightDoor0(screen, game);
			}
			if(!((game->dungeon.left==3)||(game->dungeon.left==1)||(game->dungeon.left==4)))
			{
				//left wall perp
				U1_drawLeftWallPerp(screen, game);
			}
			if(!((game->dungeon.righ==3)||(game->dungeon.righ==1)||(game->dungeon.righ==4)))
			{	//right wall perp
				U1_drawRightWallPerp(screen, game);
			}
			if((game->dungeon.cent==7)||(game->dungeon.cent==9))
			{	//bottom opening
				U1_drawBottomHole(screen, game);
			}
			if(game->dungeon.cent==8)
			{	//top opening
				U1_drawTopHole(screen, game);
			}
			if((game->dungeon.cent==7)||(game->dungeon.cent==8))
			{	//ladder
				U1_drawLadder(screen, game);
			}
			if((dis>0)&&(game->dungeon.cent==5))
			{	//chest
				term->print("CHEST!\n");
				U1_drawChest(screen, game);
			}
		}

	if(game->dungeon.mc>=1)
	{
		int b=79+game->dungeon.yy[game->dungeon.dis];
		int c=139;
		if(game->dungeon.mc==8)
		{
			term->print("CHEST!\n");
			term->print("\n");
		}
		else
		{
			term->print(game->dungeon.ms[game->dungeon.mc]);
			term->print("\n");
		}
		if(dis!=0)
			switch(game->dungeon.mc)
			{
				case 1:		//skeleton
					U1_drawSkeleton(screen, b, c, game);
				break;


				case 2:		//theif
					U1_drawThief(screen, b, c, game);
				break;


				case 3:		//giant rat
					U1_drawRat(screen, b, c, game);
				break;


				case 4:		//orc
					U1_drawOrc(screen, b, c, game);
				break;

				case 5:		//viper
					U1_drawViper(screen, b, c, game);
				break;


				case 6:		//carrion crawler
					U1_drawCrawler(screen, b, c, game);
				break;


				case 7:		//gremlin
					U1_drawGremlin(screen, b, c, game);
				break;


				case 8:		//mimic
					U1_drawMimic(screen, b, c, game);
				break;


				case 9:		//daemon
					U1_drawDaemon(screen, b, c, game);
				break;

				case 10:	//balrog
					U1_drawBalrog(screen, b, c, game);
 				break;
			}
		}
		if(game->dungeon.dis==0)
		{
			game->dungeon.en=0;
			return 0;
		}
//		if(game->dungeon.en==1)
//		{
		//	u1refresh(screen);
//			game->dungeon.en=0;
//			return 0;
//		}
//		game->dungeon.dis++;
		game->dungeon.dis--;
	}
	return 0;
}


/*int U1_makeDungeon(TU1_game *game)
{
	game->dungeon.zz=rnd(-abs(game->dungeon.ln)-game->dungeon.tx*10-game->dungeon.ty*1000+game->dungeon.inout*31.4);
	for(int x=1;x<=9;x++)
		for(int y=1;y<=9;y++)
				game->dungeon.dn[x][y]=0;
	for(int x=0;x<=10;x++)
	{
		game->dungeon.dn[x][0]=1;
		game->dungeon.dn[x][10]=1;
		game->dungeon.dn[0][x]=1;
		game->dungeon.dn[10][x]=1;
	}	
	for(int x=2;x<=8;x+=2)
		for(int y=1;y<=9;y++)
		{
			game->dungeon.dn[x][y]=1;
			game->dungeon.dn[y][x]=1;
		}
	for(int x=2;x<=8;x+=2)
		for(int y=1;y<=9;y+=2)
		{
			if(rnd(1)>.95)
				game->dungeon.dn[x][y]=2;
			if(rnd(1)>.95)
				game->dungeon.dn[y][x]=2;
			if(rnd(1)>.6)
				game->dungeon.dn[y][x]=3;
			if(rnd(1)>.6)
				game->dungeon.dn[x][y]=3;
			if(rnd(1)>.6)
				game->dungeon.dn[x][y]=4;
			if(rnd(1)>.6)
				game->dungeon.dn[y][x]=4;
			if(rnd(1)>.97)
				game->dungeon.dn[y][x]=9;
			if(rnd(1)>.97)
				game->dungeon.dn[x][y]=9;
			if(rnd(1)>.94)
				game->dungeon.dn[x][y]=5;
			if(rnd(1)>.94)
				game->dungeon.dn[y][x]=5;
		}
		game->dungeon.dn[2][1]=0;
		if(((float)game->dungeon.inout/2.0)==((int)(game->dungeon.inout/2)))
		{
			game->dungeon.dn[7][3]=7;
			game->dungeon.dn[3][7]=8;
		}
		else
		{
			game->dungeon.dn[7][3]=8;
			game->dungeon.dn[3][7]=7;			
		}
		if(game->dungeon.inout==1)
		{
			game->dungeon.dn[1][1]=8;
			game->dungeon.dn[7][3]=0;
		}
		U1_addMonsters(game);
		return 0;
}*/

/*int U1_dng_up(SDL_Surface *screen, TU1_game *game, Tterminal *term)
{
	if((game->dungeon.dn[game->dungeon.px+game->dungeon.dx][game->dungeon.py+game->dungeon.dy]!=1)&&(game->dungeon.dn[game->dungeon.px+game->dungeon.dx][game->dungeon.py+game->dungeon.dy]<10))
	{
		game->dungeon.px=game->dungeon.px+game->dungeon.dx;
		game->dungeon.py=game->dungeon.py+game->dungeon.dy;
	}
		term->print("FORWARD\n");
	if(game->dungeon.dn[game->dungeon.px][game->dungeon.py]==2)
	{
		term->print("AAARRRGGGHHH!!! A TRAP!\n");
		game->dungeon.c[0]=game->dungeon.c[0]-(int)rnd(1)*game->dungeon.inout+3;
		game->dungeon.mr=1;
		game->dungeon.inout++;
		char tempstr[80];
		sprintf(tempstr, "FALLING TO LEVEL %d\n",game->dungeon.inout); 
		term->print(tempstr);
		U1_makeDungeon(game);
		if(U1_updatefood(screen, game, term))
			return 1;
		return 0;
	}
		int z=0;
	if(game->dungeon.dn[game->dungeon.px][game->dungeon.py]==5)
	{
		game->dungeon.dn[game->dungeon.px][game->dungeon.py]=0;
		term->print("GOLD!!!!!\n");
		int z=(int)(rnd(1)*5*game->dungeon.inout+game->dungeon.inout);
		char tempstr[80];
		sprintf(tempstr, "%d-PIECES OF EIGHT\n", z);
		term->print(tempstr);
		game->dungeon.c[5]+=z;
	}

	if(z > 0 )
	{
		z=int(rnd(1) * 6);
		char tempstr[80];
		sprintf(tempstr, "AND A %s", game->dungeon.ws[z]);
		term->print(tempstr);
		game->dungeon.pw[z]=game->dungeon.pw[z]+1;
	}
	if(U1_updatefood(screen, game, term))
		return 1;
	return 0;
}*/

/*int U1_dng_right(SDL_Surface *screen, TU1_game *game, Tterminal *term)
{
	term->print("TURN RIGHT\n");
	if(game->dungeon.dx!=0)
	{
		game->dungeon.dy=game->dungeon.dx;
		game->dungeon.dx=0;
	}
	else
	{
		game->dungeon.dx=-game->dungeon.dy;
		game->dungeon.dy=0;
	}
	if(U1_updatefood(screen, game, term))
		return 1;
	return 0;
}*/

/*int U1_dng_left(SDL_Surface *screen, TU1_game *game, Tterminal *term)
{
	term->print("TURN LEFT\n");
	if(game->dungeon.dx!=0)
	{
		game->dungeon.dy=-game->dungeon.dx;
		game->dungeon.dx=0;
	}
	else
	{
		game->dungeon.dx=game->dungeon.dy;
		game->dungeon.dy=0;
	}
	if(U1_updatefood(screen, game, term))
		return 1;
	return 0;
}*/

/*int U1_dng_down(SDL_Surface *screen, TU1_game *game, Tterminal *term)
{
	term->print("TURN AROUND\n");
	game->dungeon.dx=-game->dungeon.dx;
	game->dungeon.dy=-game->dungeon.dy;
	if(U1_updatefood(screen, game, term))
		return 1;
	return 0;
}*/

/*int U1_dng_e(SDL_Surface *screen, TU1_game *game, Tterminal *term)
{
	if((game->dungeon.dn[game->dungeon.px][game->dungeon.py]==7)||(game->dungeon.dn[game->dungeon.px][game->dungeon.py]==9))
	{
		char temp[40];
		sprintf(temp, "DO DOWN TO LEVEL %d\n", game->dungeon.inout+1);
		term->print(temp);
		game->dungeon.inout++;
		U1_makeDungeon(game);
		game->dungeon.mr=1;
		if(U1_updatefood(screen, game, term))
			return 1;
		return 0;
	}
	if(game->dungeon.dn[game->dungeon.px][game->dungeon.py]!=8)
	{
		term->print("HUH?\n");
		if(U1_updatefood(screen, game, term))
			return 1;
		return 0;
	}
	if(game->dungeon.inout==1)
	{
		term->print("LEAVE DUNGEON\n");
		game->dungeon.inout=0;
	}
	else
	{
		char temp[40];
		sprintf(temp, "GO UP TO LEVEL %d\n", game->dungeon.inout-1);
		term->print(temp);
		game->dungeon.inout--;
		U1_makeDungeon(game);
		game->dungeon.mr=1;
	}
	if(game->dungeon.inout==0)
	{
		term->print("THOU HAST GAINED\n");
		char temp[25];
		sprintf(temp, "%d HIT POINTS\n", game->dungeon.lk);
		term->print(temp);
		game->dungeon.c[0]+=game->dungeon.lk;
		game->dungeon.lk=0;
	}
	if(U1_updatefood(screen, game, term))
		return 1;
	return 0;
}*/

/*int U1_dng_a(SDL_Surface *screen, TU1_game *game, Tterminal *term)
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
			refresh(screen, game->screen);
			q=waitKey(game->screen);
			if(q==SDL_QUIT)
				exit(-1);
			switch(q)
			{
				case 'r':
					dam=10;
					term->print("RAPIER\n");
					if(game->dungeon.pw[1]<1)
					{
						term->print("NOT OWNED\n");go=0;
					}
				break;
				case 'a':
					dam=5;
					term->print("AXE\n");
					if(game->dungeon.pw[2]<1)
					{
						term->print("NOT OWNED\n");go=0;
					}
				break;
				case 's':
					dam=1;
					term->print("SHIELD\n");
					if(game->dungeon.pw[3]<1)
					{
						term->print("NOT OWNED\n");go=0;
					}
				break;
				case 'b':
					dam=4;
					term->print("BOW\n");
					if(game->dungeon.pw[4]<1)
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
					if((q=='b')&&(game->dungeon.pt=='M'))
					{
						term->print("MAGES CAN'T USE BOWS!");
						break;
					}
					if((q=='r')&&(game->dungeon.pt=='M'))
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
						refresh(screen, game->screen);
						char q=waitKey(game->screen);
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
							game->dungeon.pw[2]=game->dungeon.pw[2]-1;
						}
					}
				}
				if((dam==5)||(dam==4)||(magicattack==1))
				{
					for(int y=1;y<=5;y++)
					{
						if((game->dungeon.px+game->dungeon.dx*y<1)||(game->dungeon.px+game->dungeon.dx*y>9)||(game->dungeon.py+game->dungeon.dy*y>9)||(game->dungeon.py+game->dungeon.dy*y<0))
							break;
						mn=game->dungeon.dn[game->dungeon.px+game->dungeon.dx*y][game->dungeon.py+game->dungeon.dy*y];
						mn=(int)(mn/10);
						if(mn>0)
							break;
					}
				}		
				if(((dam!=5)&&(dam!=4))||(swing==1))
				{
					mn=game->dungeon.dn[game->dungeon.px+game->dungeon.dx][game->dungeon.py+game->dungeon.dy]/10;
				}
				if((mn<1)||(game->dungeon.c[2]-rnd(1)*25<mn+game->dungeon.inout))
				{
					term->print("YOU MISSED\n");
				}
				else
				{
					term->print("HIT!!!\n");
					dam=(int)(rnd(1)*dam+game->dungeon.c[1]/5);
					game->dungeon.mz[mn][1]=game->dungeon.mz[mn][1]-dam;
						char tempstr[40];
						sprintf(tempstr, "%s'S HIT POINTS=%d\n", game->dungeon.ms[mn], game->dungeon.mz[mn][1]);
						term->print(tempstr);
						if(game->dungeon.mz[mn][1]<1)
						{
							sprintf(tempstr, "THOU HAS KILLED A %s\n", game->dungeon.ms[mn]);
							term->print(tempstr);
							term->print("THOU SHALT RECEIVE\n");
							int da=mn+game->dungeon.inout;
							sprintf(tempstr, "%d PIECES OF EIGHT\n", da);
							term->print(tempstr);
							game->dungeon.c[5]=game->dungeon.c[5]+da;
							game->dungeon.dn[game->dungeon.ml[mn][0]][game->dungeon.ml[mn][1]]=game->dungeon.dn[game->dungeon.ml[mn][0]][game->dungeon.ml[mn][1]]-10*mn;
							game->dungeon.mz[mn][0]=0;
						}
						game->dungeon.lk=(int)(game->dungeon.lk+(mn*(game->dungeon.inout/2.0)));
						if(mn==game->dungeon.task)
							game->dungeon.task=-game->dungeon.task;
						
					}
					if(game->dungeon.pa==1)
					{
						term->print("-CR- TO CONT. \n");
						term->blit();
						refresh(screen, game->screen);
						waitKey(game->screen);
						if(q==SDL_QUIT)
							exit(-1);
						
					}
					if(U1_updatefood(screen, game, term))
						return 1;
					return 0;
					break;
				
				}
			}
		
		else
		{
			term->print("MAGIC AMULET\n");
			if(game->dungeon.pw[5]<1)
			{
				term->print("NONE OWNED\n");
			}
			else
			{
				int Q=0;
				if(game->dungeon.pt=='F')
				{
					Q=(int)(rnd(1)*4+1);	
				}
				else
				{
					term->print("1-LADDER-UP 2-LADDER-DN\n");
					term->print("3-KILL      4-BAD??\n");
					term->print("CHOICE ");
					term->blit();
					refresh(screen, game->screen);
					while((Q<1)||(Q>4))
					{
						Q=waitKey(game->screen)-'0';
						if(Q==SDL_QUIT-'0')
							exit(-1);
						
					}
				if(rnd(1)>.75)
					{
						term->print("LAST CHARGE ON THIS AMULET!\n");
						game->dungeon.pw[5]--;
					}
				}
				switch(Q)
				{
					case 1:
						term->print("LADDER UP\n");
						game->dungeon.dn[game->dungeon.px][game->dungeon.py]=8;
						if(U1_updatefood(screen, game, term))
							return 1;
						return 0;
						break;	
					case 2:
						term->print("LADDER DOWN\n");
						game->dungeon.dn[game->dungeon.px][game->dungeon.py]=7;
						if(U1_updatefood(screen, game, term))
							return 1;
						return 0;
						break;	
					case 3:
						term->print("MAGIC ATTACK\n");
						dam=10+game->dungeon.inout;
						magicattack=1;
						break;	
					case 4:
						switch((int)(rnd(1)*3+1))
						{
							case 1:
								term->print("YOU HAVE BEEN TURNED\n");
								term->print("INTO A TOAD!\n");
								for(int z2=1;z2<=4;z2++)
									game->dungeon.c[z2]=3;
								if(U1_updatefood(screen, game, term))
									return 1;
								return 0;
								break;	
							case 2:
								term->print("YOU HAVE BEEN TURNED\n");
								term->print("INTO A LIZARD MAN!\n");
								for(int y=0;y<=4;y++)
									game->dungeon.c[y]=(int)(game->dungeon.c[y]*2.5);
								if(U1_updatefood(screen, game, term))
									return 1;
								return 0;
								break;	
							case 3:
								term->print("BACKFIRE\n");
								game->dungeon.c[0]/=2;
								if(U1_updatefood(screen, game, term))
									return 1;
								return 0;
						}
				}
			}
		}
	}
	return 0;
}*/

/*int U1_addMonsters(TU1_game *game)
{
	game->dungeon.nm=0;
	for(int x=1;x<=10;x++)
	{
		game->dungeon.mz[x][0]=0;
		game->dungeon.mz[x][1]=x+3+game->dungeon.inout;
		if(!(((x-2)>game->dungeon.inout)||(rnd(1)>.4)))
		{
			game->dungeon.ml[x][0]=(int)(rnd(1)*9+1);
			game->dungeon.ml[x][1]=(int)(rnd(1)*9+1);
			while((game->dungeon.dn[game->dungeon.ml[x][0]][game->dungeon.ml[x][1]]!=0)||((game->dungeon.ml[x][0]==game->dungeon.px)&&(game->dungeon.ml[x][1]==game->dungeon.py)))
			{
				game->dungeon.ml[x][0]=(int)(rnd(1)*9+1);
				game->dungeon.ml[x][1]=(int)(rnd(1)*9+1);
			}

			game->dungeon.dn[game->dungeon.ml[x][0]][game->dungeon.ml[x][1]]=x*10;
			game->dungeon.mz[x][0]=1;
			game->dungeon.nm++;
			game->dungeon.mz[x][1]=x*2+game->dungeon.inout*2*game->dungeon.lp;
		}
	}
	return 0;
}*/

void U1_draw3087(SDL_Surface *s, float c, float b, float dis, Uint32 cl)
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

/*int u1_enemy_Attack(SDL_Surface *screen, TU1_game *game, Tterminal *term)
{
	for(int mm=1;mm<=10;mm++)
	{
		if(game->dungeon.mz[mm][0]!=0)
		{
			int skipto4081=0;
			int skipto4500=0;
			int skipto4999=0;
			int d=0;
			float ra=sqrt(pow(game->dungeon.px-game->dungeon.ml[mm][0], 2)+pow(game->dungeon.py-game->dungeon.ml[mm][1], 2));
			if(game->dungeon.mz[mm][1]>=game->dungeon.inout*game->dungeon.lp)
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
				int x1=sgn(game->dungeon.px-game->dungeon.ml[mm][0]);
				int y1=sgn(game->dungeon.py-game->dungeon.ml[mm][1]);
				if(game->dungeon.mz[mm][1]<game->dungeon.inout*game->dungeon.lp)
				{
					x1=-x1;
					y1=-y1;
				}
				if(y1!=0)
				{
					d=game->dungeon.dn[game->dungeon.ml[mm][0]][(int)(game->dungeon.ml[mm][1]+y1+.5)];
					if((d==1)||(d>9)||(d==2))
					{
						y1=0;
						if(x1==0)
							break;
						d=game->dungeon.dn[(int)(game->dungeon.ml[mm][0]+x1+.5)][game->dungeon.ml[mm][1]];
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
					d=game->dungeon.dn[(int)(game->dungeon.ml[mm][0]+x1+.5)][game->dungeon.ml[mm][1]];
					if((d==1)||(d>9)||(d==2))
					{
						x1=0;
						skipto4081=1;
					}
				}
				if(skipto4081==0)
				{
					game->dungeon.dn[game->dungeon.ml[mm][0]][game->dungeon.ml[mm][1]]-=(10*mm);
					if((game->dungeon.ml[mm][0]+x1==game->dungeon.px)&&(game->dungeon.ml[mm][1]+y1==game->dungeon.py))
					{
						skipto4999=1;
						break;
					}
	
				game->dungeon.ml[mm][0]+=x1;
					game->dungeon.ml[mm][1]+=y1;
					game->dungeon.dn[game->dungeon.ml[mm][0]][game->dungeon.ml[mm][1]]+=(int)(10*mm+.5);

				}
				if((skipto4500==0)&&(skipto4999==0))
				{
					if((x1!=0)&&(y1!=0))
					{
						skipto4999=1;
						break;
					}
					if((game->dungeon.mz[mm][1]<game->dungeon.inout*game->dungeon.lp)&&(ra<1.3))
					{
						skipto4500=0;
						break;
					}
					if(game->dungeon.mz[mm][1]<game->dungeon.inout*game->dungeon.lp)
						game->dungeon.mz[mm][1]+=(mm+game->dungeon.inout);
					skipto4999=1;
				}
			}
			if((skipto4999==0)||(skipto4500==1))
			{
				if((!((mm==2)||(mm==7)))||(rnd(1)<.5))
				{
					term->print("YOU ARE BEING ATTACKED\n");
					char tempstr[40];
					sprintf(tempstr, "BY A %s\n", game->dungeon.ms[mm]);
					term->print(tempstr);
					if(rnd(1)*20.0-sgn((int)game->dungeon.pw[3])-game->dungeon.c[3]+mm+game->dungeon.inout<0)
					{
						term->print("MISSED\n");
					}
					else
					{
						term->print("HIT\n");
						game->dungeon.c[0]-=(int)(rnd(1)*mm*game->dungeon.inout);
					}
					if(game->dungeon.pa==1)
					{
						term->print("-CR- TO CONT. \n");
						if(waitKey(game->screen)==SDL_QUIT)
							exit(-1);
					}
					skipto4999=1;
				}
				else
				{
					//taken care of by the if statement on line 4500
					if(mm==7)
					{
							game->dungeon.pw[0]=game->dungeon.pw[0]/2;
							term->print("A GREMLIN STOLE SOME FOOD\n");
							//4525 is a pause
					}
					else
					{
							int zz=(int)(rnd(1)*6);
							while(game->dungeon.pw[zz]<1)
								zz=(int)(rnd(1)*6);
							char tempstr[40];
							sprintf(tempstr, "A THIEF STOLE A %s\n", game->dungeon.ws[zz]);
							term->print(tempstr);
							game->dungeon.pw[zz]--;
							//4525 is a pause
					}
					if(game->dungeon.pa==1)
					{
						term->print("-CR- TO CONT. \n");
						if(waitKey(game->screen)==SDL_QUIT)
							exit(-1);
					}
					
				}
			}
		}
	}
	return 0;
}
*/
