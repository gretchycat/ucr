#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "u1types.h"
#include "u1placenames.h"
#include "../SDL_prim.h"

int U1_addTick(char *dest, char *src)
{
	sprintf(dest, "%c%c%c%c>%c%c%c%c%s", 3, 255, 255, 255, 3, 0, 255, 255, src);
	return 0;
}

int U1_printWithTick(Tterminal *term, char *str)	//SHOULDNT NEED THIS  --- FIX THE BUG!
{
	char temp[512];
	U1_addTick(temp, "");
	term->print(temp);
	term->print(str);
	return 0;
}

int U1_stats(SDL_Surface *screen, Tfont *font, TU1_user *user)
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
	SDL_Surface *buf=SDL_CreateRGBSurface(SDL_SWSURFACE, 146, 80, 32, rmask, gmask, bmask, amask);
	SDL_Rect bufferrect;

	SDL_Rect rect;
	bufferrect.x=494+2;bufferrect.w=146-2;
	bufferrect.h=4*16;bufferrect.y=331+5;
	rect.x=0;rect.y=0;rect.w=bufferrect.w;rect.h=bufferrect.h;
	SDL_FillRect(buf, &rect, 0);
	output(buf, font, 0, 0, "Hits:", 0, 255, 255);
	output(buf, font, 0, 1, "Food:", 0, 255, 255);
	output(buf, font, 0, 2, "Exp.:", 0, 255, 255);
	output(buf, font, 0, 3, "Coin:", 0, 255, 255);
	char temp[5];
	if(user->hits>9999)
		user->hits=9999;
	if(user->food>9999)
		user->food=9999;
	if(user->exp>9999)
		user->exp=9999;
	if(user->coin>9999)
		user->coin=9999;
	Tcolor c;
	c.R=0;c.G=255;c.B=255;
	sprintf(temp, "%d", user->hits);
	int p=5;
	if(user->hits<1000)
		p=6;
	if(user->hits<100)
		p=7;
	if(user->hits<10)
		p=8;
	if(user->hits<0)
		user->hits=0;	
	if(user->hits<100)
	{
		c.R=255;c.G=87;c.B=87;
	}
	output(buf, font, p, 0, temp, c.R, c.G, c.G);
	c.R=0;c.G=255;c.B=255;
	sprintf(temp, "%d", user->food);
	p=5;
	if(user->food<1000)
		p=6;
	if(user->food<100)
		p=7;
	if(user->food<10)
		p=8;
	
	if(user->food<100)
	{
		c.R=255;c.G=87;c.B=87;
	}
	output(buf, font, p, 1, temp, c.R, c.G, c.G);
	c.R=0;c.G=255;c.B=255;
	sprintf(temp, "%d", user->exp);
	p=5;
	if(user->exp<1000)
		p=6;
	if(user->exp<100)
		p=7;
	if(user->exp<10)
		p=8;
	output(buf, font, p, 2, temp, c.R, c.G, c.G);
	c.R=0;c.G=255;c.B=255;
	sprintf(temp, "%d", user->coin);
	p=5;
	if(user->coin<1000)
		p=6;
	if(user->coin<100)
		p=7;
	if(user->coin<10)
		p=8;
	output(buf, font, p, 3, temp, c.R, c.G, c.G);
	SDL_FillRect(screen, &bufferrect, 0);
	SDL_BlitSurface(buf, &rect, screen, &bufferrect);
	SDL_FreeSurface(buf);
	SDL_UpdateRect(screen, bufferrect.x, bufferrect.y, bufferrect.w, bufferrect.h);
	return 0;
}

int U1_weaponName(char *name, int num)
{
	switch(num)
	{
		case 0: strcpy(name, "Hands");break;
		case 1: strcpy(name, "Dagger");break;
		case 2:	strcpy(name, "Mace");break;
		case 3: strcpy(name, "Axe");break;
		case 4: strcpy(name, "Rope & Spikes");break;
		case 5: strcpy(name, "Sword");break;
		case 6: strcpy(name, "Great Sword");break;
		case 7: strcpy(name, "Bow & Arrows");break;
		case 8: strcpy(name, "Amulet");break;
		case 9: strcpy(name, "Wand");break;
		case 10: strcpy(name, "Staff");break;
		case 11: strcpy(name, "Triangle");break;
		case 12: strcpy(name, "Pistol");break;
		case 13: strcpy(name, "Light Sword");break;
		case 14: strcpy(name, "Phazor");break;
		case 15: strcpy(name, "Blaster");break;
		default: name[0]=0;break;
	}
	return 0;
}

int U1_gemName(char *name, int num)
{
	switch(num)
	{
		case 0: strcpy(name, "Red Gem");break;
		case 1: strcpy(name, "Green Gem");break;
		case 2:	strcpy(name, "Blue Gem");break;
		case 3: strcpy(name, "White Gem");break;
		default: name[0]=0;break;
	}
	return 0;
}
int U1_armorName(char *name, int num)
{
	switch(num)
	{
		case 0:	strcpy(name, "Skin");break;
		case 1: strcpy(name, "Leather armor");break;
		case 2: strcpy(name, "Chain mail");break;
		case 3: strcpy(name, "Plate mail");break;
		case 4: strcpy(name, "Vacuum suit");break;
		case 5: strcpy(name, "Reflect suit");break;
		default:name[0]=0;break;
	}
	return 0;
}

int U1_spellName(char *name, int num)
{
	switch(num)
	{
		case 0:	strcpy(name, "Prayer");break;	//a
		case 1:	strcpy(name, "Open");break;	//b
		case 2:	strcpy(name, "Unlock");break;	//c
		case 3:	strcpy(name, "Magic Missile");break;	//d
		case 4: strcpy(name, "Steal");break;		//e
		case 5:	strcpy(name, "Ladder Down");break;	//f
		case 6:	strcpy(name, "Ladder Up");break;	//g
		case 7:	strcpy(name, "Blink");break;	//h
		case 8:	strcpy(name, "Create");break;	//i
		case 9:	strcpy(name, "Destroy");break;	//j
		case 10:	strcpy(name, "Kill");break;	//k
		default:name[0]=0;break;
	}
	return 0;
}

int U1_raceName(char *name, int num)
{
	switch(num)
	{
		case 0:	strcpy(name, "Human");break;
		case 1: strcpy(name, "Elf");break;
		case 2: strcpy(name, "Dwarf");break;
		case 3: strcpy(name, "Bobbit");break;
		default:name[0]=0;break; 
	}
	return 0;
}

int U1_className(char *name, int num)
{
	switch(num)
	{
		case 0:	strcpy(name, "Fighter");break;
		case 1: strcpy(name, "Cleric");break;
		case 2: strcpy(name, "Wizard");break;
		case 3: strcpy(name, "Thief");break;
		default:name[0]=0;break;
	}
	return 0;
}

int U1_sexName(char *name, int num)
{
	switch(num)
	{
		case 0:	strcpy(name, "Male");break;
		case 1: strcpy(name, "Female");break;
		default: name[0]=0;
	}
	return 0;
}

int U1_craftName(char *name, int num)
{
	switch(num)
	{
		case 0:	strcpy(name, "Foot");break;
		case 1: strcpy(name, "Horse");break;
		case 2: strcpy(name, "Cart");break;
		case 3: strcpy(name, "Raft");break;
		case 4: strcpy(name, "Frigate");break;
		case 5: strcpy(name, "Aircar");break;
		case 6: strcpy(name, "Shuttle");break;
		case 7: strcpy(name, "Time Machine");break;
		default: strcpy(name, "what?");break;;break;
	}
	return 0;
}

int U1_craftSteps(int num)
{
	int steps=2;
	switch(num)
	{
		case 0:	steps=2;break;
		case 1: steps=3;break;
		case 2: steps=3;break;
		case 3: steps=6;break;
		case 4: steps=5;break;
		case 5: steps=6;break;
		case 6: steps=0;break;
		case 7: steps=0;break;
	}
	return steps;
}

int U1_enemyName(char *name, int num)
{
	switch(num)
	{
		case 10: sprintf(name, "Player");break;
		case 11: sprintf(name, "Horse");break;
		case 12: sprintf(name, "Carriage");break;
		case 13: sprintf(name, "Raft");break;
		case 14:
		case 15: sprintf(name, "Frigate");break;
		case 16: sprintf(name, "Aircar");break;
		case 17: sprintf(name, "Space shuttle");break;
		case 18: sprintf(name, "Time machine");break;
		case 19:
		case 20: sprintf(name, "Ness creature");break;
		case 21:
		case 22: sprintf(name, "Giant Squid");break;
		case 23:
		case 24: sprintf(name, "Dragon turtle");break;
		case 25:
		case 26: sprintf(name, "Pirate ship");break;
		case 27:
		case 28: sprintf(name, "Hood");break;
		case 29:
		case 30: sprintf(name, "Bear");break;
		case 31:
		case 32: sprintf(name, "Hidden archer");break;
		case 33:
		case 34: sprintf(name, "Dark knight");break;
		case 35:
		case 36: sprintf(name, "Evil trent");break;
		case 37: 
		case 38: sprintf(name, "Thief");break;
		case 39:
		case 40: sprintf(name, "Orc");break;
		case 41:
		case 42: sprintf(name, "Knight");break;
		case 43:
		case 44: sprintf(name, "Necromancer");break;
		case 45:
		case 46: sprintf(name, "Evil ranger");break;
		case 47:
		case 48: sprintf(name, "Wandering warlock");break;
		default: sprintf(name, "Unknown warrior");break;
	}
	return 0;
}

int U1_npcName(int name)
{
	int s=0;
	switch(name)      
	{      
		case 17:  s=10;break; //guard        
		case 19:  s=13;break; //jester(14)/bard(13)        
		case 20:  s=9;break;  //king        
		case 21:  s=11;break; //merchant        
		case 22:  s=12;break; //princess        
		case 50:  s=15;break; //wench(15)/lecher(16)        
	}      
	return s;
}

int numChars(int z)
{
	char num[10];
	sprintf(num, "%d", z);
	return strlen(num);
}

int addNumberToString(char *str, int z)
{
	str[strlen(str)-numChars(z)]=0;
	sprintf(str, "%s%d", str, z);
	return 0;
}

int U1_ztats(SDL_Surface *screen, Tterminal *term, TU1_game *game)
{
	Uint32 rmask, gmask, bmask, amask;
	Tcolor ega[16];
	setEGAcolors(ega);
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
	SDL_Rect invrect;
	SDL_Rect morerect;
	invrect.x=term->font->chr[0]->w*13;
	invrect.y=0;
	invrect.w=term->font->chr[0]->w*13;
	invrect.h=term->font->chr[0]->h;
	SDL_Surface *inventorytextbackground=SDL_CreateRGBSurface(SDL_SWSURFACE, invrect.w, invrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &invrect, inventorytextbackground, NULL);
	SDL_FillRect(screen, &invrect, 0);
	char tempstr[30];
	sprintf(tempstr, "> %c%c%c%cInventory%c%c%c%c <", 3, ega[11].R, ega[11].G, ega[11].B, 3, ega[15].R, ega[15].G, ega[15].B);
	output(screen, term->font, 13, 0, tempstr, ega[15].R, ega[15].G, ega[15].B);
	SDL_UpdateRect(screen, 0,0,0,0);
	char str[41];
	bufferrect.x=16;bufferrect.w=640-32;
	bufferrect.h=18*16;bufferrect.y=16;
	rect.x=0;rect.y=0;rect.w=bufferrect.w;rect.h=bufferrect.h;
	SDL_Surface *background=SDL_CreateRGBSurface(SDL_SWSURFACE, bufferrect.w, bufferrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &bufferrect, background, &rect);
	term->print(" Press Space to continue: ");
	char strlist[52][22];
	for(int z=0;z<52;z++)
		strlist[z][0]=0;
	int z=0;
	if(game->user.hits)
	{
		sprintf(strlist[z], "Hit Points.......");
		addNumberToString(strlist[z], game->user.hits);
	}
	z++;
	if(game->user.strength)
	{
		sprintf(strlist[z], "Strength.........");
		addNumberToString(strlist[z], game->user.strength);		
	}
	z++;
	if(game->user.agility)
	{
		sprintf(strlist[z], "Agility..........");
		addNumberToString(strlist[z], game->user.agility);		
	}
	z++;
	if(game->user.stamina)
	{
		sprintf(strlist[z], "Stamina..........");
		addNumberToString(strlist[z], game->user.stamina);		
	}
	z++;
	if(game->user.charisma)
	{
		sprintf(strlist[z], "Charisma.........");
		addNumberToString(strlist[z], game->user.charisma);		
	}
	z++;
	if(game->user.wisdom)
	{
		sprintf(strlist[z], "Wisdom...........");
		addNumberToString(strlist[z], game->user.wisdom);		
	}
	z++;
	if(game->user.intelligence)
	{
		sprintf(strlist[z], "Intelligence.....");
		addNumberToString(strlist[z], game->user.intelligence);		
	}
	z++;
	if(game->user.coin%10)
	{
		sprintf(strlist[z], "Copper pence.....");
		addNumberToString(strlist[z], game->user.coin%10);		
		z++;
	}
	if(game->user.coin/10%10)
	{
		sprintf(strlist[z], "Silver pieces....");
		addNumberToString(strlist[z], game->user.coin/10%10);		
		z++;
	}
	if(game->user.coin/100)
	{
		sprintf(strlist[z], "Gold crowns......");
		addNumberToString(strlist[z], game->user.coin/100);		
		z++;
	}
	for(int x=1;x<6;x++)
	{
		if(game->user.armor[x])
		{
			U1_armorName(strlist[z], x);
			for(int y=strlen(strlist[z]);y<17;y++)
			{
				strlist[z][y]='.';
			}
			strlist[z][17]=0;
			addNumberToString(strlist[z], game->user.armor[x]);
			if(x==game->user.armorready)
			{
				char temp[30];
				sprintf(temp, "%c%c%c%c%s", 3, ega[12].R, ega[12].G, ega[12].B, strlist[z]);
				strcpy(strlist[z], temp);
			}
			z++;
		}
	}
	for(int x=1;x<16;x++)
	{
		if(game->user.weapon[x])
		{
			U1_weaponName(strlist[z], x);
			for(int y=strlen(strlist[z]);y<17;y++)
			{
				strlist[z][y]='.';
			}
			strlist[z][17]=0;
			addNumberToString(strlist[z], game->user.weapon[x]);
			if(x==game->user.weaponready)
			{
				char temp[30];
				sprintf(temp, "%c%c%c%c%s", 3, ega[12].R, ega[12].G, ega[12].B, strlist[z]);
				strcpy(strlist[z], temp);
			}
			z++;
		}
	}
	for(int x=1;x<11;x++)
	{
		if(game->user.spell[x])
		{
			U1_spellName(strlist[z], x);
			for(int y=strlen(strlist[z]);y<17;y++)
			{
				strlist[z][y]='.';
			}
			strlist[z][17]=0;
			addNumberToString(strlist[z], game->user.spell[x]);
			if(x==game->user.spellready)
			{
				char temp[30];
				sprintf(temp, "%c%c%c%c%s", 3, ega[12].R, ega[12].G, ega[12].B, strlist[z]);
				strcpy(strlist[z], temp);
			}
			z++;
		}
	}

	for(int x=1;x<8;x++)
	{
		if(game->user.craft[x])
		{
			U1_craftName(strlist[z], x);
			for(int y=strlen(strlist[z]);y<17;y++)
			{
				strlist[z][y]='.';
			}
			strlist[z][17]=0;
			addNumberToString(strlist[z], game->user.craft[x]);
			if(x==game->user.craftready)
			{
				char temp[30];
				sprintf(temp, "%c%c%c%c%s", 3, ega[12].R, ega[12].G, ega[12].B, strlist[z]);
				strcpy(strlist[z], temp);
			}
			z++;
		}
	}
	for(int x=0;x<4;x++)
	{
		if(game->user.gem[x])
		{
			U1_gemName(strlist[z], x);
			for(int y=strlen(strlist[z]);y<17;y++)
			{
				strlist[z][y]='.';
			}
			strlist[z][17]=0;
			addNumberToString(strlist[z], game->user.gem[x]);
			z++;
		}
	}

	strlist[z][0]=0;
	int go=1;
	z=0;
	int RETVAL=0;	
	morerect.x=term->font->chr[0]->w*16;
	morerect.y=term->font->chr[0]->h*19;
	morerect.w=term->font->chr[0]->w*8;
	morerect.h=term->font->chr[0]->h;
	SDL_Surface *moretextbackground=SDL_CreateRGBSurface(SDL_SWSURFACE, invrect.w, invrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &morerect, moretextbackground, NULL);
	while(go)
	{
		SDL_Surface *tinted=setBrightness(background, .2);
		sprintf(str, "%c%c%c%cPlayer: %s\n", 3,255,255,255,game->user.name);
		output(tinted, term->font, 1, 1, str, ega[15].R, ega[15].G, ega[15].B);
		char SEX[20];
		char RACE[20];
		char CLASS[20];
		U1_sexName(SEX, game->user.sex);
		U1_raceName(RACE, game->user.race);
		U1_className(CLASS, game->user.clas);
		sprintf(str, "%c%c%c%cA Level %d %s %s %s\n", 3,255,255,255,game->user.exp/1000+1, SEX, RACE, CLASS);
		output(tinted, term->font, 1, 2, str, ega[15].R, ega[15].G, ega[15].B);	
		for(int x=z;x<z+26;x++)
		{
			if(strlist[x][0])
			{
				output(tinted, term->font, 1+(((x/13)%2)*19), x%13+4, strlist[x], ega[11].R, ega[11].G, ega[11].B);
			}
			else
			{
				go=0;
				break;
			}
		}
		z+=26;
		if(go)
		{
			SDL_FillRect(screen, &morerect, 0);
			char tempstr[30];
			sprintf(tempstr, "> %c%c%c%cMore%c%c%c%c <", 3, ega[11].R, ega[11].G, ega[11].B, 3, ega[15].R, ega[15].G, ega[15].B);
			output(screen, term->font, 16, 19, tempstr, ega[15].R, ega[15].G, ega[15].B);
			SDL_UpdateRect(screen, 0,0,0,0);
		}		
		SDL_BlitSurface(tinted, &rect, screen, &bufferrect);		
		SDL_UpdateRect(screen, 0,0,0,0);
		refresh(screen, game->screen);
		char n=0;
		while(n!=SDLK_SPACE)
		{
			n=term->waitKey();
			if(n==SDL_QUIT)
			{
				RETVAL=n;
				go=0;
				break;
			}
		}
		SDL_FreeSurface(tinted);
		SDL_BlitSurface(background, &rect, screen, &bufferrect);
	}
	term->dropLine();
	SDL_BlitSurface(background, &rect, screen, &bufferrect);
	SDL_BlitSurface(inventorytextbackground, NULL, screen, &invrect);
	SDL_BlitSurface(moretextbackground, NULL, screen, &morerect);
	SDL_UpdateRect(screen, 0,0,0,0);
//	refresh(screen, game->screen);
	SDL_FreeSurface(background);	
	SDL_FreeSurface(inventorytextbackground);
	SDL_FreeSurface(moretextbackground);
	return RETVAL;
}

int U1_listWeapons(SDL_Surface *screen, Tterminal *term, TU1_game *game, char *headstr, int drop)
{
	Uint32 rmask, gmask, bmask, amask;
	Tcolor ega[16];
	setEGAcolors(ega);
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
	SDL_Rect invrect;
	int hx=(20-(strlen(headstr)/2+2));
	invrect.x=term->font->chr[0]->w*hx;
	invrect.y=0;
	invrect.w=term->font->chr[0]->w*(strlen(headstr)+4);
	invrect.h=term->font->chr[0]->h;
	SDL_Surface *inventorytextbackground=SDL_CreateRGBSurface(SDL_SWSURFACE, invrect.w, invrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &invrect, inventorytextbackground, NULL);
	SDL_FillRect(screen, &invrect, 0);
	char tempstr[30];
	sprintf(tempstr, "> %c%c%c%c%s%c%c%c%c <", 3, ega[11].R, ega[11].G, ega[11].B, headstr, 3, ega[15].R, ega[15].G, ega[15].B);
	output(screen, term->font, hx, 0, tempstr, ega[15].R, ega[15].G, ega[15].B);
	SDL_UpdateRect(screen, 0,0,0,0);
	char t[41];
	bufferrect.x=16;bufferrect.w=640-32;
	bufferrect.h=18*16;bufferrect.y=16;
	rect.x=0;rect.y=0;rect.w=bufferrect.w;rect.h=bufferrect.h;
	SDL_Surface *background=SDL_CreateRGBSurface(SDL_SWSURFACE, bufferrect.w, bufferrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &bufferrect, background, &rect);
	SDL_Surface *tinted=setBrightness(background, .2);
	int s=0;
	if(drop)
		s=1; 
	int zz=0; 
	for(int xx=s;xx<16;xx++)
			if(game->user.weapon[xx]!=0)
				zz++;
		int yy=0;
		for(int xx=s;xx<16;xx++)
		{
			if(game->user.weapon[xx]!=0)
			{
				sprintf(t, "%c) %s", xx+'a', game->out.S_weapons[xx]);
				output(tinted, term->font, 13, yy+9-(int)(.5*zz), t, ega[11].R, ega[11].G, ega[11].B);
				yy++;
			}
		}
//		output(tinted, term->font, 1, 1, str, ega[15].R, ega[15].G, ega[15].B);
		SDL_BlitSurface(tinted, &rect, screen, &bufferrect);		
		SDL_UpdateRect(screen, 0,0,0,0);
		refresh(screen, game->screen);
		SDL_FreeSurface(tinted);
	int w=term->waitKey()-'a';
	if((w>=16)||(w<s))
		w=-1;//game->user.weaponready;          	
	SDL_BlitSurface(background, &rect, screen, &bufferrect);
	SDL_BlitSurface(background, &rect, screen, &bufferrect);
	SDL_BlitSurface(inventorytextbackground, NULL, screen, &invrect);
//	SDL_UpdateRect(screen, 0,0,0,0);
//	refresh(screen, game->screen);
	SDL_FreeSurface(background);	
	SDL_FreeSurface(inventorytextbackground);
	return w; 
}

int U1_listArmors(SDL_Surface *screen, Tterminal *term, TU1_game *game, char *headstr, int drop)
{
	Uint32 rmask, gmask, bmask, amask;
	Tcolor ega[16];
	setEGAcolors(ega);
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
	SDL_Rect invrect;
	int hx=(20-(strlen(headstr)/2+2));
	invrect.x=term->font->chr[0]->w*hx;
	invrect.y=0;
	invrect.w=term->font->chr[0]->w*(strlen(headstr)+4);
	invrect.h=term->font->chr[0]->h;
	SDL_Surface *inventorytextbackground=SDL_CreateRGBSurface(SDL_SWSURFACE, invrect.w, invrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &invrect, inventorytextbackground, NULL);
	SDL_FillRect(screen, &invrect, 0);
	char tempstr[30];
	sprintf(tempstr, "> %c%c%c%c%s%c%c%c%c <", 3, ega[11].R, ega[11].G, ega[11].B, headstr, 3, ega[15].R, ega[15].G, ega[15].B);
	output(screen, term->font, hx, 0, tempstr, ega[15].R, ega[15].G, ega[15].B);
	SDL_UpdateRect(screen, 0,0,0,0);
	char t[41];
	bufferrect.x=16;bufferrect.w=640-32;
	bufferrect.h=18*16;bufferrect.y=16;
	rect.x=0;rect.y=0;rect.w=bufferrect.w;rect.h=bufferrect.h;
	SDL_Surface *background=SDL_CreateRGBSurface(SDL_SWSURFACE, bufferrect.w, bufferrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &bufferrect, background, &rect);
	SDL_Surface *tinted=setBrightness(background, .2);
	int s=0;
	if(drop)
		s=1; 
		int zz=0;
		for(int xx=s;xx<6;xx++)
			if(game->user.armor[xx]!=0)
				zz++;
		int yy=0;
		for(int xx=s;xx<6;xx++)
		{
			if(game->user.armor[xx]!=0)
			{
				sprintf(t, "%c) %s", xx+'a', game->out.S_armors[xx]);
				output(tinted, term->font, 13, yy+9-(int)(.5*zz), t, ega[11].R, ega[11].G, ega[11].B);
				yy++;
			}
		}
//		output(tinted, term->font, 1, 1, str, ega[15].R, ega[15].G, ega[15].B);
		SDL_BlitSurface(tinted, &rect, screen, &bufferrect);		
		SDL_UpdateRect(screen, 0,0,0,0);
		refresh(screen, game->screen);
		SDL_FreeSurface(tinted);
	int w=term->waitKey()-'a';
	if((w>=6)||(w<s))        
		w=-1;//game->user.spellready;          	
	SDL_BlitSurface(background, &rect, screen, &bufferrect);
	SDL_BlitSurface(background, &rect, screen, &bufferrect);
	SDL_BlitSurface(inventorytextbackground, NULL, screen, &invrect);
//	SDL_UpdateRect(screen, 0,0,0,0);
//	refresh(screen, game->screen);
	SDL_FreeSurface(background);	
	SDL_FreeSurface(inventorytextbackground);
	return w; 
}

int U1_listSpells(SDL_Surface *screen, Tterminal *term, TU1_game *game, char *headstr, int drop )
{
	Uint32 rmask, gmask, bmask, amask;
	Tcolor ega[16];
	setEGAcolors(ega);
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
	SDL_Rect invrect;
	int hx=(20-(strlen(headstr)/2+2));
	invrect.x=term->font->chr[0]->w*hx;
	invrect.y=0;
	invrect.w=term->font->chr[0]->w*(strlen(headstr)+4);
	invrect.h=term->font->chr[0]->h;
	SDL_Surface *inventorytextbackground=SDL_CreateRGBSurface(SDL_SWSURFACE, invrect.w, invrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &invrect, inventorytextbackground, NULL);
	SDL_FillRect(screen, &invrect, 0);
	char tempstr[30];
	sprintf(tempstr, "> %c%c%c%c%s%c%c%c%c <", 3, ega[11].R, ega[11].G, ega[11].B, headstr, 3, ega[15].R, ega[15].G, ega[15].B);
	output(screen, term->font, hx, 0, tempstr, ega[15].R, ega[15].G, ega[15].B);
	SDL_UpdateRect(screen, 0,0,0,0);
	char t[41];
	bufferrect.x=16;bufferrect.w=640-32;
	bufferrect.h=18*16;bufferrect.y=16;
	rect.x=0;rect.y=0;rect.w=bufferrect.w;rect.h=bufferrect.h;
	SDL_Surface *background=SDL_CreateRGBSurface(SDL_SWSURFACE, bufferrect.w, bufferrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &bufferrect, background, &rect);
	SDL_Surface *tinted=setBrightness(background, .2);
	int s=0;
	if(drop)
		s=1; 
	int zz=0;
		for(int xx=s;xx<11;xx++)
			if(game->user.spell[xx]!=0)
				zz++;
		int yy=0;
		for(int xx=s;xx<11;xx++)
		{
			if(game->user.spell[xx]!=0)
			{
				sprintf(t, "%c) %s", xx+'a', game->out.S_spells[xx]);
				output(tinted, term->font, 13, yy+9-(int)(.5*zz), t, ega[11].R, ega[11].G, ega[11].B);
				yy++;
			}
		}
//		output(tinted, term->font, 1, 1, str, ega[15].R, ega[15].G, ega[15].B);
		SDL_BlitSurface(tinted, &rect, screen, &bufferrect);		
		SDL_UpdateRect(screen, 0,0,0,0);
		refresh(screen, game->screen);
		SDL_FreeSurface(tinted);
	int w=term->waitKey()-'a';
	if((w>=11)||(w<s))        
		w=-1;//game->user.spellready;          	
	SDL_BlitSurface(background, &rect, screen, &bufferrect);
	SDL_BlitSurface(background, &rect, screen, &bufferrect);
	SDL_BlitSurface(inventorytextbackground, NULL, screen, &invrect);
//	SDL_UpdateRect(screen, 0,0,0,0);
//	refresh(screen, game->screen);
	SDL_FreeSurface(background);	
	SDL_FreeSurface(inventorytextbackground);
	return w; 
}

int U1_addBorder(SDL_Surface *screen, int x1, int y1, int x2, int y2, int width)
{
	if(screen==NULL)
		return 1;
	for(int zzz=0;zzz<width;zzz+=1)
	{
		SDL_drawLine_AA(screen, x1+2, y1+zzz+1, x2-4, y1+zzz+1, SDL_MapRGBA(screen->format, 0, 0, 0, 255));
		SDL_drawLine_AA(screen, x2-zzz-2, y1+3, x2-zzz-2, y2-2, SDL_MapRGBA(screen->format, 0, 0, 0, 255));
		SDL_drawLine_AA(screen, x1+2, y2-zzz, x2-4, y2-zzz, SDL_MapRGBA(screen->format, 0, 0, 0, 255));
		SDL_drawLine_AA(screen, x1+zzz-1, y1+3, x1+zzz-1, y2-4, SDL_MapRGBA(screen->format, 0, 0, 0, 255));
	}
	for(int zzz=2;zzz<width-1;zzz++)
	{
		SDL_drawLine_AA(screen, x1+2, y1+zzz+1, x2-4, y1+zzz+1, SDL_MapRGBA(screen->format, 4, 100, 115, 255));
		SDL_drawLine_AA(screen, x2-zzz-2, y1+3, x2-zzz-2, y2-2, SDL_MapRGBA(screen->format, 4, 100, 115, 255));
		SDL_drawLine_AA(screen, x1+2, y2-zzz, x2-4, y2-zzz, SDL_MapRGBA(screen->format, 4, 100, 115, 255));
		SDL_drawLine_AA(screen, x1+zzz-1, y1+3, x1+zzz-1, y2-4, SDL_MapRGBA(screen->format, 4, 100, 115, 255));
	}
	SDL_drawLine_AA(screen, x1+1, y1, x2-2, y1, SDL_MapRGBA(screen->format, 170, 150, 64, 255));
	SDL_drawLine_AA(screen, x2-1, y1+1, x2-1, y2-2, SDL_MapRGBA(screen->format, 170, 150, 64, 255));
	SDL_drawLine_AA(screen, x1+1, y2-1, x2-2, y2-1, SDL_MapRGBA(screen->format, 170, 150, 64, 255));
	SDL_drawLine_AA(screen, x1, y1+1, x1, y2-2, SDL_MapRGBA(screen->format, 170, 150, 64, 255));

	SDL_drawLine_AA(screen, x1+width, y1+width, x2-(width+1), y1+width, SDL_MapRGBA(screen->format, 170, 150, 64, 255));
	SDL_drawLine_AA(screen, x2-(width+1), y1+width, x2-(width+1), y2-(width+1), SDL_MapRGBA(screen->format, 170, 150, 64, 255));
	SDL_drawLine_AA(screen, x1+width, y2-(width+1), x2-(width+1), y2-(width+1), SDL_MapRGBA(screen->format, 170, 150, 64, 255));
	SDL_drawLine_AA(screen, x1+width, y1+width, x1+width, y2-(width+1), SDL_MapRGBA(screen->format, 170, 150, 64, 255));
	return 0;
}

int U1_addBorder(SDL_Surface *screen)
{
	return U1_addBorder(screen, 0,0, screen->w, screen->h, 7);
}

int U1_msgBox(SDL_Surface *screen, Tterminal *term, TU1_user *user, char txt[12][31], int delay)
{
	Uint32 rmask, gmask, bmask, amask;
	Tcolor ega[16];
	setEGAcolors(ega);
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
	bufferrect.x=term->font->chr[0]->w*4;bufferrect.w=term->font->chr[0]->w*32;
	bufferrect.h=term->font->chr[0]->h*13;bufferrect.y=term->font->chr[0]->h*3;
	rect.x=0;rect.y=0;rect.w=bufferrect.w;rect.h=bufferrect.h;
	SDL_Surface *background=SDL_CreateRGBSurface(SDL_SWSURFACE, bufferrect.w, bufferrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &bufferrect, background, &rect);

	SDL_Surface *tinted=setBrightness(background, .2);
	U1_addBorder(tinted);
	for(int x=0;x<11;x++)
		output(tinted, term->font, 16-(strlen(txt[x])/2), x+1, txt[x], ega[11].R, ega[11].G, ega[11].B);
	SDL_BlitSurface(tinted, &rect, screen, &bufferrect);		
	SDL_UpdateRect(screen, 0,0,0,0);
	int RETVAL=0;
	if(delay!=-1)
		RETVAL=term->waitKey();
	else
		RETVAL=term->getNum(1, 999);
	SDL_FreeSurface(tinted);
	SDL_BlitSurface(background, &rect, screen, &bufferrect);
	SDL_FreeSurface(background);	
	return RETVAL;
}


int U1_ready(SDL_Surface *screen, Tterminal *term, TU1_game *game)
{
	term->print(game->out.S_ready[5]);
	int r=term->waitKey();
	term->dropLine();
	switch(r)
	{
		case 'w':
		{
			U1_printWithTick(term,"Ready weapon: ");
			int w=U1_listWeapons(screen, term, game, "Ready", 0);
			if(w==-1)
				w=game->user.weaponready;
			if(game->user.weapon[w]>0)
				game->user.weaponready=w;
			term->print(game->out.S_weapons[game->user.weaponready]);
			term->print("\n");
		}
		break;
		case 'a':
		{
			U1_printWithTick(term,"Ready armor: ");
			int w=U1_listArmors(screen, term, game, "Ready", 0);
			if(w==-1)
				w=game->user.armorready;
			if(game->user.armor[w]>0)
				game->user.armorready=w;
			term->print(game->out.S_armors[game->user.armorready]); 
			term->print("\n");
		}
		break;
		case 's':
		{
			U1_printWithTick(term,"Ready spell: ");
			int w=U1_listSpells(screen, term, game, "Ready", 0);
			if(w==-1)
				w=game->user.spellready;
			if(game->user.spell[w]>0)
				game->user.spellready=w;
			term->print(game->out.S_spells[game->user.spellready]);
			term->print("\n");
		}
		break;
		default:
			U1_printWithTick(term,"Ready nothing\n");
			break;
	}
	return 1;
}

int U1_inform(SDL_Surface *screen, TU1_game *game, Tterminal *term, TU1_map map)
{
	term->print(game->out.S_inform[2]);
	term->print("\n");
	term->print(" ");
	int tile=map[game->user.y][game->user.x];
	switch(tile)
	{
		case 0:	//sea
				term->print(game->out.S_inform[4]);
				term->print("\n");
				break;
		case 1:	//land 
				{
					int idx=(game->user.x/(168/2)+game->user.y/(156/2)*2);
					term->print(game->out.S_inform[6]);
					term->print("\n");
					term->print(" ");
					term->print(game->out.S_lands[idx]);
					term->print("\n");
				}
				break;
		case 2:	//woods
				term->print(game->out.S_inform[5]);
				term->print("\n");
				break;
		case 3:	//mountain
				term->print("CHEATER!");
				term->print("\n");
				break;
		case 4:	//castle
				{
					int n=U1_townname(game->user.x, game->user.y)-32;
					term->print(game->out.S_castles[n]);
					term->print("\n");
				}
				break;
		case 5:	//signpost
				{
					int n=U1_shrinename(game->user.x, game->user.y);
					term->print(game->out.S_shrines[n]);
					term->print("\n");
				}
				break;
		case 6:	//town
				{
					int n=U1_townname(game->user.x, game->user.y);
					term->print("the city of ");
					term->print(game->out.S_towns[n]);
					term->print("\n");
				}
				break;
		case 7:	//dungeon
				{
					int n=U1_dungeonname(game->user.x, game->user.y);
					term->print(game->out.S_dungeons[n]);
					term->print("\n");
				}
				break;
	}


	return 0;
}
