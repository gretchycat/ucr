#include <SDL.h>
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "../applebasic.h"
#include "u1types.h"


int TU1USERPACKNPC(TU1_mapNPC *npc)
{
	for(int x=1;x<40;x++)
	{
		int y=0;
		if(npc[x].shape==0)
		{
			for(y=x+1;y<40;y++)
			{
				if(npc[y].shape!=0)
					break;
			}
			if(y==40)
				return 0;
			TU1_mapNPC TMP=npc[x];
			npc[x]=npc[y];
			npc[y]=TMP;
		}
	}
	return 0;
}

int TU1COUNTENEMIES(TU1_mapNPC *npc)
{
	TU1USERPACKNPC(npc);
	int ct=0;
	while(npc[ct].shape!=0)
		ct++;
	return ct-1;
}

int TU1_user::eraseNPC(int num)
{
	npc[num].shape=0;
	npc[num].flags=0;
	npc[num].x=0;
	npc[num].y=0;
	npc[num].hits=0;
	npc[num].v1=0;
	npc[num].v2=0;
	npc[num].v3=0;
	enemies=TU1COUNTENEMIES(npc);
	return 0;
}

int TU1_user::newNPC(int shape, int xx, int yy)
{
	enemies=TU1COUNTENEMIES(npc);
	if(enemies>=38)
	  if(shape>18)
  	  return 1;
 	  else
 	    enemies=(int)(rnd(1)*36+1);
	enemies++;
	if(enemies<1)
  	enemies=1;
	npc[enemies].shape=shape;
	npc[enemies].x=xx;
	npc[enemies].y=yy;
	switch(shape)
	{
    case 19:
    	npc[enemies].hits=42;
    	break;
    case 21:
    	npc[enemies].hits=62;
    	break;
	  case 23:
    	npc[enemies].hits=82;
    	break;
    case 25: 
      npc[enemies].hits=102; 
      break;
    case 27: 
      npc[enemies].hits=12; 
      break;
    case 29: 
      npc[enemies].hits=22; 
      break;
    case 31:
    	npc[enemies].hits=42;
    	break;
    case 33: 
      npc[enemies].hits=62; 
      break;
    case 35: 
      npc[enemies].hits=82; 
      break;
    case 37:
    	npc[enemies].hits=22;
    	break;
    case 39:
    	npc[enemies].hits=42;
    	break;
    case 41:
    	npc[enemies].hits=62;
    	break;
    case 43:
    	npc[enemies].hits=82;
    	break;
    case 45:
    	npc[enemies].hits=92;
    	break;
    case 47:
    	npc[enemies].hits=102;
    	break;
	      
  	default:
    	npc[enemies].hits=0;
    	break;
	}
  npc[enemies].flags=0;
	npc[enemies].v1=0;
	npc[enemies].v2=0;
	npc[enemies].v3=0;
	enemies=TU1COUNTENEMIES(npc);
	return 0;
}
  	  	
