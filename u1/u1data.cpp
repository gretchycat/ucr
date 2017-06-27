#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_endian.h>
#include "../SDL_rotozoom.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "u1types.h"
#include "u1data.h"
#include "u1colors.h"
#include "../2xsai.h"

int U1_swapUserEndian(TU1_user *user)
{
	user->race=swapInt16Endian(user->race);
	user->clas=swapInt16Endian(user->clas);
	user->sex=swapInt16Endian(user->sex);
	user->hits=swapInt16Endian(user->hits);
	user->strength=swapInt16Endian(user->strength);
	user->agility=swapInt16Endian(user->agility);
	user->stamina=swapInt16Endian(user->stamina);
	user->charisma=swapInt16Endian(user->charisma);
	user->wisdom=swapInt16Endian(user->wisdom);
	user->intelligence=swapInt16Endian(user->intelligence);
	user->coin=swapInt16Endian(user->coin);
	user->exp=swapInt16Endian(user->exp);
	user->food=swapInt16Endian(user->food);
	user->weaponready=swapInt16Endian(user->weaponready);
	user->spellready=swapInt16Endian(user->spellready);
	user->craftready=swapInt16Endian(user->craftready);
	user->v5=swapInt16Endian(user->v5);
	user->x=swapInt16Endian(user->x);
	user->y=swapInt16Endian(user->y);
	user->v6=swapInt16Endian(user->v6);
	user->v7=swapInt16Endian(user->v7);
	for(int x=0;x<8;x++)
		user->quest[x]=swapInt16Endian(user->quest[x]);
	for(int x=0;x<4;x++)
		user->gem[x]=swapInt16Endian(user->gem[x]);
	for(int x=0;x<6;x++)
		user->armor[x]=swapInt16Endian(user->armor[x]);
	for(int x=0;x<16;x++)
		user->weapon[x]=swapInt16Endian(user->weapon[x]);
	for(int x=0;x<11;x++)
		user->spell[x]=swapInt16Endian(user->spell[x]);
	for(int x=0;x<8;x++)
		user->craft[x]=swapInt16Endian(user->craft[x]);
	user->v8=swapInt16Endian(user->v8);
	user->lastsignpost=swapInt16Endian(user->lastsignpost);
	user->enemies=swapInt16Endian(user->enemies);
	user->moves=swapInt16Endian(user->moves);
	user->v11=swapInt16Endian(user->v11);
	user->v12=swapInt16Endian(user->v12);
	user->v13=swapInt16Endian(user->v13);
	for(int x=0;x<40;x++)
	{
		user->npc[x].shape=swapInt16Endian(user->npc[x].shape);
		user->npc[x].flags=swapInt16Endian(user->npc[x].flags);
		user->npc[x].x=swapInt16Endian(user->npc[x].x);
		user->npc[x].y=swapInt16Endian(user->npc[x].y);
		user->npc[x].hits=swapInt16Endian(user->npc[x].hits);
		user->npc[x].v1=swapInt16Endian(user->npc[x].v1);
		user->npc[x].v2=swapInt16Endian(user->npc[x].v2);
		user->npc[x].v3=swapInt16Endian(user->npc[x].v3);
	}
	return 0;
}

SDL_Surface *U1_loadOriginPic(TglobalConfig *config)
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
	int XDIM=277;
	int YDIM=62;
	SDL_Surface *img=SDL_CreateRGBSurface(SDL_SWSURFACE, XDIM,YDIM, 32, rmask, gmask, bmask, amask);
	//ultima.exe/0x4389
	char tempfn[512];
	sprintf(tempfn, "%s/ultima.exe", config->game[1].gamepath);
	FILE *in=fopen(tempfn, "rb");
	if(in)
	{
		char c;
		for(int x=0;x<0x4389;x++)
		{
			fread(&c, 1, 1, in);
		}
		for(int y=0;y<YDIM;y++)
		{
			for(int x=0;x<200;x++)
			{
				fread(&c, 1, 1, in);
				if(c!=0x20)
					if(c==0)
						break;
					else
						if(x!=199)
						{
							if(c==0x2A)
							{
								DrawPixel(img, x, y, 92, 92, 255);
								if(y==47)
									DrawPixel(img, x, y+1, 92, 92, 255);
							}
						}
			}	
			if(y==47)
				y++;
		}

		for(int x=0;x<129;x++)
			fread(&c, 1, 1, in);
		for(int y=0;y<YDIM;y++)
		{
			for(int x=200;x<XDIM;x++)
			{
				fread(&c, 1, 1, in);
				if(c!=0x20)
				{
					if(c==0)
						break;
					else
						if(x!=200)
						{
							if(c==0x2A)
							{
								DrawPixel(img, x-2, y, 92, 92, 255);
								if((y==2)||(y==14)||(y==32)||(y==34)||(y==36)||(y==54)||(y==56)||(y==58))
									DrawPixel(img, x-2, y+1, 92, 92, 255);
							}
						}
				}
			}	

			if((y==2)||(y==14)||(y==32)||(y==34)||(y==36)||(y==54)||(y==56)||(y==58))
				y++;
		}
	}
	else
	{
		printf("%s\n", tempfn);
	}
	return img;
}
int U1_swapMapNpcEndian(TU1_npc npc[10][15])
{
	for(int x=0;x<10;x++)
		for(int y=0;y<15;y++)
		{
			npc[x][y].shape=swapInt16Endian(npc[x][y].shape);
			npc[x][y].x=swapInt16Endian(npc[x][y].x);
			npc[x][y].y=swapInt16Endian(npc[x][y].y);
			npc[x][y].hits=swapInt16Endian(npc[x][y].hits);
		}
	return 0;
}

int U1_loadmap(TglobalConfig *config, TU1_map map)
{
	FILE *in;
	char tempfn[512];
	sprintf(tempfn, "%s/map.bin", config->game[1].gamepath);
	if((in=fopen(tempfn, "rb"))!=NULL) {
//		fread(map, 1, 156*84, in);
		for(int x=0;x<156;x++)
			for(int y=0;y<84;y++)
			{
				char n;
				fread(&n, sizeof(char), 1, in);
				map[x][y*2]=n/16;
				map[x][y*2+1]=n%16;
			}
		fclose(in);
	}
	else
	{
        	printf("Error opening %s\n", tempfn);
		return 1;
	}
	return 0;

}

int U1_setDefaults(TU1_user *user)
{
	for(int x=0;x<15;x++)
		user->name[x]=0;
	user->race=0;user->clas=0;user->sex=0;
	user->hits=150;
	user->strength=10;
	user->agility=10;
	user->stamina=10;
	user->charisma=10;
	user->wisdom=10;
	user->intelligence=10;
	user->coin=100;
	user->exp=0;
	user->food=200;
	user->weaponready=1;
	user->spellready=0;
	user->armorready=1;
	user->craftready=0;
	user->v5=3979;
	user->x=49;
	user->y=40;
	user->v6=1;
	user->v7=0;
	for(int x=0;x<8;x++)
		user->quest[x]=0;
	for(int x=0;x<4;x++)
		user->gem[x]=0;
	for(int x=2;x<6;x++)
		user->armor[x]=0;
	user->armor[0]=65535;
	user->armor[1]=1;
	for(int x=2;x<16;x++)
		user->weapon[x]=0;
	user->weapon[0]=65535;
	user->weapon[1]=2;
	for(int x=1;x<11;x++)
		user->spell[x]=0;
	user->spell[0]=65535;
	for(int x=0;x<8;x++)
		user->craft[x]=0;
	user->craft[0]=65535;
	user->v8=0;
	user->lastsignpost=65535;
	user->enemies=0;
	user->moves=0;
	user->v11=0;
	user->v12=0;
	user->v13=0;
	for(int x=0;x<40;x++)
	{
		user->npc[x].shape=0;
		user->npc[x].flags=0;
		user->npc[x].x=0;
		user->npc[x].y=0;
		user->npc[x].hits=0;
		user->npc[x].v1=0;
		user->npc[x].v2=0;
		user->npc[x].v3=0;
	}
	user->npc[0].shape=10;
	user->npc[0].flags=1;
	user->npc[0].x=user->x;
	user->npc[0].y=user->y;
	return 0;
}

int U1_loaduser(TglobalConfig *config, TU1_user *user, int num)
{
	FILE *in;
	char filename[256];
	sprintf(filename, "%s/player%d.u1", config->game[1].gamepath, num);
	if((in=fopen(filename, "rb"))!=NULL)
	{
		fread(user, 1, sizeof(TU1_user), in);
		fclose(in);
	}
	else
	{
		U1_setDefaults(user);
		return 1;
	}
//	#ifdef DEBUG	 
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	 	printf("Big endian system detected.\n");
	 	printf("Converting to big endian.\n");
	 	U1_swapUserEndian(user);
	#else
	 	printf("Litle endian system detected.\n");
	 	printf("No endian conversion necessary.\n");
	#endif
//	user->coin=9999;
//user->clas=0;
	printf("--------------------------------------------------------------\n");
	printf("Name: %s	Race:   %d	Class:   %d	Sex:     %d\n", user->name, user->race, user->clas, user->sex);
	printf("Hits:     %d	Str:    %d	Agility: %d	Stamina: %d\n", user->hits, user->strength, user->agility, user->stamina);
	printf("Charisma: %d	Wisdom: %d	Int:     %d	Coin:    %d\n", user->hits, user->wisdom, user->intelligence, user->coin);
	printf("EXP:      %d	Food:   %d	Weapon:  %d	Spell:   %d\n", user->exp, user->food, user->weaponready, user->spellready);
	printf("Armor:    %d	Craft:  %d	V5:      %d	----------\n", user->armorready, user->craftready, user->v5);
	printf("x:        %d    y:      %d	V6:      %d	V7:      %d\n", user->x, user->y, user->v6, user->v7);
	printf("Quests: ");
	for(int x=0;x<8;x++)
		printf("%d ", user->quest[x]);
	printf("\n");
	printf("Gems: ");
	for(int x=0;x<4;x++)
		printf("%d ", user->gem[x]);
	printf("\n");
	printf("Armor: ");
	for(int x=0;x<6;x++)
		printf("%d ", user->armor[x]);
	printf("\n");
	printf("Weapon: ");
	for(int x=0;x<16;x++)
		printf("%d ", user->weapon[x]);
	printf("\n");
	printf("Spell: ");
	for(int x=0;x<11;x++)
		printf("%d ", user->spell[x]);
	printf("\n");
	printf("Craft: ");
	for(int x=0;x<8;x++)
		printf("%d ", user->craft[x]);
	printf("\n");
	printf("v8:         %d    lastsignpost: %d	enemies: %d	moves: %d\n", user->v8, user->lastsignpost, user->enemies, user->moves);
	printf("v12:        %d    v13:      %d	V14:     %d\n", user->v11, user->v12, user->v13);
	for(int x=0;x<40;x++)
	{
		printf("NPC %d\n", x);
		printf("	Shape: %d	Flags: %d	x: %d	y:%d\n", user->npc[x].shape, user->npc[x].flags, user->npc[x].x, user->npc[x].y); 
		printf("	hits: %d	V1: %d	V2: %d	V3: %d\n", user->npc[x].hits, user->npc[x].v1, user->npc[x].v2, user->npc[x].v3); 
	}
	printf("--------------------------------------------------------------\n");
//	#endif	
	return 0;
}

int U1_saveuser(TglobalConfig *config, TU1_user *user, int num)
{
	FILE *out;
	char filename[256];
	sprintf(filename, "%s/player%d.u1", config->game[1].gamepath, num);
	#ifdef DEBUG	 
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	 	printf("Big endian system detected.\n");
	 	printf("Converting to big endian.\n");
	 	U1_swapUserEndian(user);
	#else
	 	printf("Litle endian system detected.\n");
	 	printf("No endian conversion necessary.\n");
	#endif
	#endif
	if((out=fopen(filename, "wb"))!=NULL)
	{
		fwrite(user, 1, sizeof(TU1_user), out);
		fclose(out);
	}
	else
	{
	        printf("Error reading %s\n", filename);
		return 1;
	}
        #ifdef DEBUG	 
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	 	printf("Big endian system detected.\n");
	 	printf("Converting to big endian.\n");
	 	U1_swapUserEndian(user);
	#else
	 	printf("Litle endian system detected.\n");
	 	printf("No endian conversion necessary.\n");
	#endif
	#endif
	return 0;
}
	
int U1_loadtowns(TglobalConfig *config, TU1_towns towns)
{
	FILE *in;
	char tempfn[512];
	sprintf(tempfn, "%s/tcd.bin", config->game[1].gamepath);
	if((in=fopen(tempfn, "rb"))!=NULL)
	{
		fread(towns, 1, 10*38*18, in);
		fclose(in);
	}
	else
	{
	        printf("Error reading %s\n", tempfn);
		return 1;
	}
	return 0;

}
int U1_loadnif(TglobalConfig *config, TU1_nif nif)
{
	FILE *in;
	char tempfn[512];
	sprintf(tempfn, "%s/nif.bin", config->game[1].gamepath);
	if((in=fopen(tempfn, "rb"))!=NULL)
	{
		fread(nif, 1, 80*84, in);
		fclose(in);
	}
	else
	{   
	        printf("Error reading %s\n", tempfn);
		return 1;
	}
	return 0;
}

int U1_loadtiles(TglobalConfig *config, TU1_tiles *tiles, char *filename, int xsize, int ysize)
{
	if((ysize>0)&&(ysize<=64))
		tiles->ydim=ysize;
	if((xsize>0)&&(xsize<=64))
		tiles->xdim=xsize;
	char prefix[4];
	prefix[0]=filename[0];prefix[1]=filename[1];prefix[2]=filename[2];prefix[3]=0;
	char fn[512];
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
	SDL_Surface *alltiles=NULL;
	if(!strcmp(prefix, "t1k"))
	{
	        sprintf(fn, "%s/%s", config->game[1].gamepath, filename);
		Tcolor ega[16];
		setEGAcolors(ega);
		FILE *in;
		if((in=fopen(fn, "rb"))!=NULL)
		{
			alltiles=SDL_CreateRGBSurface(SDL_SWSURFACE, tiles->xdim*100, tiles->ydim, 32, rmask, gmask, bmask, amask);
			int z=0;
			while(!feof(in))
			{
				for(int y=0;y<tiles->ydim;y++)
				{
					for(int x=0;x<tiles->xdim/2;x++)
					{
						unsigned char c;
						fread(&c, sizeof(char), 1, in);
						Uint8 color=c/16;
						DrawPixel(alltiles, x*2+z*tiles->xdim, y, ega[color].R, ega[color].G, ega[color].B);
						color=c%16;
						DrawPixel(alltiles, x*2+1+z*tiles->xdim, y, ega[color].R, ega[color].G, ega[color].B);
						
//						tiles->tiles[z][y][x*2]=c/16;
//						tiles->tiles[z][y][x*2+1]=c%16;
					}
				}
				z++;
			}
			fclose(in);
			if(!strcmp(fn, "t1ktown.bin"))
				tiles->num=z;
			else
				tiles->num=z-1;
		//	int antialias=0;
		//	double factor=2.0;
			for(int z=0;z<tiles->num;z++)
			{
				SDL_Surface *src=SDL_CreateRGBSurface(SDL_SWSURFACE, tiles->xdim, tiles->ydim, 32, rmask, gmask, bmask, amask);
				SDL_Rect destrect;
				destrect.x=0;destrect.y=0;destrect.w=tiles->xdim;destrect.h=tiles->ydim;
				SDL_Rect srcrect;
				srcrect.x=z*tiles->xdim;srcrect.y=0;srcrect.w=tiles->xdim;srcrect.h=tiles->ydim;
				if(SDL_BlitSurface(alltiles, &srcrect, src, &destrect))
					printf("\nError with blit.\n");
				tiles->tiles[z]=src;
				//tiles->tiles[z]=SDL_Scale_2xSai(src);
				//tiles->tiles[z]=zoomSurface(src, (double)factor, (double)factor, antialias);
				//SDL_FreeSurface(src);
			}
		//	tiles->xdim=tiles->xdim*(int)factor;
		//	tiles->ydim=tiles->ydim*(int)factor; 
		}
		else
		{
		        
			return 1;
		}
	}
	else
	{		
	        sprintf(fn, "%s/u1/%s", config->datapath, filename);

		alltiles=SDL_LoadBMP(fn);
		if(alltiles==NULL)
		{
        		printf("error opening %s for reading\n", filename);
		        return -1;
                }
		tiles->num=alltiles->w/tiles->ydim;
		for(int z=0;z<tiles->num;z++)
		{
			SDL_Surface *src=SDL_CreateRGBSurface(SDL_SWSURFACE, tiles->xdim, tiles->ydim, 32, rmask, gmask, bmask, amask);
			SDL_Rect destrect;
			destrect.x=0;destrect.y=0;destrect.w=tiles->xdim;destrect.h=tiles->ydim;
			SDL_Rect srcrect;
			srcrect.x=z*tiles->xdim;srcrect.y=0;srcrect.w=tiles->xdim;srcrect.h=tiles->ydim;
			if(SDL_BlitSurface(alltiles, &srcrect, src, &destrect))
				printf("\nError with blit.\n");
			SDL_SetColorKey(src, SDL_SRCCOLORKEY, SDL_MapRGBA(src->format, 247, 2, 247, 255));
			//SDL_SetAlpha(alltiles, SDL_SRCALPHA|SDL_RLEACCEL, 100);
			tiles->tiles[z]=SDL_DisplayFormatAlpha(src);
			SDL_FreeSurface(src);
		}
	}
	SDL_FreeSurface(alltiles);
	return 0;
}

int U1_loadtiles(TglobalConfig *config, TU1_tiles *tiles, char *tiletype, unsigned char tileset)
{
	int xsize=0;
	int ysize=0;
//	char path[240];
	char prefix[5];
	if(tileset)
		strcpy(prefix, "xga");
	else
		strcpy(prefix, "t1k");
//	strcpy(path, "data/u1/");
	if(!strcmp(tiletype, "tiles"))
	{
		xsize=(1+tileset)*16;
		ysize=(1+tileset)*16;
	}
	else
	if(!strcmp(tiletype, "town"))
	{
		xsize=(1+tileset)*8;
		ysize=(1+tileset)*8;
	}
	else
	if(!strcmp(tiletype, "space"))
	{
		xsize=(1+tileset)*32;
		ysize=(1+tileset)*19;
	}
	else
	if(!strcmp(tiletype, "fight"))
	{
		xsize=(1+tileset)*24;
		ysize=(1+tileset)*19;
	}
	else
	if(!strcmp(tiletype, "mond"))
	{
		xsize=(1+tileset)*16;
		ysize=(1+tileset)*16;
	}
	else
		return 1;
	char filename[255];
	sprintf(filename, "%s%s.bin", prefix, tiletype); 
//	printf("LOADING %s\n", filename);
	return U1_loadtiles(config, tiles, filename, xsize, ysize);
}

int U1_loadnpc(TglobalConfig *config, TU1_npc npc[10][15])
{
	FILE *in;
	char tempfn[512];
	sprintf(tempfn, "%s/out.exe", config->game[1].gamepath);
	if((in=fopen(tempfn, "rb"))!=NULL)
	{
		unsigned char dummy;
		for(int x=0;x<0x01085C;x++)
			fread(&dummy, sizeof(char), 1, in);
		for(int z=0;z<10;z++)
			for(int y=0;y<15;y++)
				fread(&npc[z][y], sizeof(TU1_npc), 1, in);
		fclose(in);
	}
	else
	{
		printf("failed to open out.exe.\n");
		return 1;
	}
	#ifdef DEBUG	 
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	 	printf("Big endian system detected.\n");
	 	printf("Converting to big endian.\n");
	 	U1_swapMapNpcEndian(npc);
	#else
	 	printf("Litle endian system detected.\n");
	 	printf("No endian conversion necessary.\n");
	#endif
	#endif
	return 0;
}

int U1_EGAImage(TglobalConfig *config, SDL_Surface *screen, char *filename, int xx, int yy)
{
	Tcolor ega[16];
	U1_setcastleEGAcolors(ega);
	char tempfn[512];
	sprintf(tempfn, "%s/%s", config->game[1].gamepath, filename);
	FILE *in=fopen(tempfn, "rb");
	if(in==NULL)
		return 1;
 	for(int y=0;y<yy;y++)
	 	for(int x=0;x<xx/2;x++)
 		{
 			unsigned char c;
		 	fread(&c, 1, 1, in);
 	 		Uint8 color=c/16;
		 	DrawPixel(screen, x*2, y,ega[color].R, ega[color].G, ega[color].B);
 		 	color=c%16;
 	 		DrawPixel(screen, x*2+1, y, ega[color].R, ega[color].G, ega[color].B);
 	 	}
 	 fclose(in);
 	 return 0;
}
 	 
