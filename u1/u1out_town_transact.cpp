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
#include "u1out.h"
#include "u1out_town.h"
#include "u1placenames.h"

int U1_transactKing(SDL_Surface *screen, Tterminal *term, TU1_game *game)
{
//	for(int w=0;w<50;w++)
//		printf("%d	\"%s\"\n", w, game->out.S_transact[w]);

	term->print("Transact with the king\n"); 
	int inp='s';
	int quest=U1_townname(game->user.x, game->user.y)-32;
	if(game->user.quest[quest]!=1)
	{
		term->print(" Dost thou offer pence\n");
		term->print(" or service: ");
		term->blit();
//	SDL_Delay(100);
		refresh(screen, game->screen);
		inp=term->waitKey();
	}
	if(inp=='p')
	{
		term->print("pence\n");
		term->print(" How much? ");
		term->blit();
		refresh(screen, game->screen);
		int inp2=term->getNum(0, 9999);
		if(inp2>game->user.coin)
		{
			if(inp2==0)
			{
				term->print("none\n");
				term->blit();
			}
			else
			{
				term->print("\n");
				term->print(" Thou hast not that much!\n");
				term->blit();
			}
		}
		else
		{
			if(inp2==0)
			{
				term->print("none\n");
				term->blit();
			}
			else
			{
				term->print("\n");
				term->blit();
				game->user.coin-=inp2;
				game->user.hits+=(int)((float)inp2/2.0*3.0);
				term->print(" In return I give unto\n");
				char temp[25];
				sprintf(temp, " thee %d hit points\n", (int)((float)inp2/2.0*3.0)); 
				term->print(temp);
				term->blit();
			}
		}
	}
	else
		if(inp=='s')
		{
			char txt[12][31];
			for(int x=0;x<12;x++)
				txt[x][0]=0;
		//	int quest=U1_townname(game->user.x, game->user.y)-32;
			switch(game->user.quest[quest])
			{
				case 0:	//no quest yet
					term->dropLine();
					term->print(" Press Space to continue:  ");
					game->user.quest[quest]=-1;
					if(quest%2==0)	//find a signpost
					{
						sprintf(txt[2], "%s", game->out.S_transact[11]);
						sprintf(txt[4], "%s", game->out.S_shrines[7-quest]);
					}
					else	//kill a monster
					{
						sprintf(txt[2], "%s", game->out.S_transact[6]);
						sprintf(txt[4], "%s", game->out.S_transact[7+(quest/2)]);
					}
					sprintf(txt[6], "%s", game->out.S_transact[12]);
					sprintf(txt[7], "%s", game->out.S_transact[13]);
					U1_msgBox(screen, term, &game->user, txt, 0);					
					term->print("\n");
					break;
				case -1:	//on a quest already
					term->print("\n");term->print(" ");
					term->print(game->out.S_transact[47]);
					term->print("\n");term->print(" ");					
					term->print(game->out.S_transact[48]); 
					term->print("\n");
					break;
				case 1:	//finished a quest
					game->user.quest[quest]=0;
//					term->dropLine();
					term->print(" Press Space to continue:  ");
					sprintf(txt[1], "%s%s%s", game->out.S_transact[33], game->user.name, game->out.S_transact[34]);
					sprintf(txt[2], "%s", game->out.S_transact[35]);
					if(quest%2==0)
					{
						sprintf(txt[4], "%s", game->out.S_transact[14]);
						int s=11;
						if(s>1)
							sprintf(txt[5], "%d%s%c%s", s, game->out.S_transact[15], 's', game->out.S_transact[16]);
						else
							sprintf(txt[5], "%d%s%s", s, game->out.S_transact[15], game->out.S_transact[16]);
						game->user.strength+=s;
						U1_msgBox(screen, term, &game->user, txt, 0);
					}
					else
					{
						sprintf(txt[4], "%s", game->out.S_transact[17]);
						int s=0;
						int o=1;
						if(quest==1)
						{
							s=1;
							o=0;
						}
						for(int x=s;x<4;x++)
						sprintf(txt[4+x+o], "%s", game->out.S_transact[17+(4*(quest/2))+x]);
						game->user.gem[quest/2]=1;
						U1_msgBox(screen, term, &game->user, txt, 0);
					}
					term->print("\n");
					break;					
			}
		}
		else
		{
			term->print("neither\n");
			term->blit();
		}
	return 0;
}

int U1_transactFood(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype)
{
	term->print("Transact-Buy, Sell: ");
	refresh(screen, game->screen);
	int inp=term->waitKey();
	term->dropLine();
	U1_printWithTick(term, "");
	char txt[12][31];
	for(int x=0;x<11;x++)
		txt[x][0]=0;
	int offset=1;
//	int stuff=towntype%2;
	Uint16 options[20];
	for(int x=0;x<20;x++)
		options[x]=0;
//	int o=0;
//	int b=0;
	int cost=0;
	Uint16 q=0; 
	int s1=0;
	int s2=0;
	int s=U1_fshopname(towntype);
	s1=((s%100)/10)-1;
	s2=s/100-1;
	if(s1!=-1)
		strcpy(txt[0], game->out.S_foodshops[s1]);
	if(s2!=-1)
		strcpy(txt[1], game->out.S_foodshops[s2]);
	if(txt[1][0]==0)
		offset=0;
	switch(inp)
	{
		case 'B':
 		case 'b':	
 				term->print("Transact-Buy: ");
// insert buy code
		{
			char temp[30];
			sprintf(temp, "Packs of 10 food cost %d pence", U1_getFoodBuyPrice(game->user.intelligence));
			strcpy(txt[4], temp);
			strcpy(txt[5], "each.How many dost thou");
			strcpy(txt[6], "wish to purchase?");
		}
		refresh(screen, game->screen);
		q=U1_msgBox(screen, term, &game->user, txt, -1);
		cost=q*U1_getFoodBuyPrice(game->user.intelligence);
		if(game->user.coin>=cost)
		{
			term->print("\n");
			game->user.food+=q*10;
			game->user.coin-=cost;
		}
		else
		{
			term->dropLine();
			char temp2[40];
			U1_addTick(temp2, "Transact-Buy: nothing\n");
			term->print(temp2);
			U1_addTick(temp2, "");
			term->print(temp2);
			for(int x=2;x<11;x++)
			{
				txt[x][0]=0;
			}
			strcpy(txt[5], "Thou canst not afford it!");
			refresh(screen, game->screen);
			q=U1_msgBox(screen, term, &game->user, txt, 0);
			term->dropLine();
		}
		break;


		case 'S':
		case 's':	term->print("Transact-Sell: ");
//insert sell code

		term->print("nothing\n");
		strcpy(txt[5], "Used food?No thanks!");
		refresh(screen, game->screen);
		q=U1_msgBox(screen, term, &game->user, txt, 0);
		break;
		default:  term->print("Transact nothing\n");break;
	}	
	return 0;
}

int U1_transactWeapon(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype)
{
	term->print("Transact-Buy, Sell: ");
	refresh(screen, game->screen);
	int inp=term->waitKey();
	term->dropLine();
	U1_printWithTick(term, "");
	char txt[12][31];
	for(int x=0;x<11;x++)
		txt[x][0]=0;
	int offset=1;
	int stuff=towntype%2;
	Uint16 options[20];
	for(int x=0;x<20;x++)
	options[x]=0;
	int o=0;
	int b=0;
	int cost=0;
	Uint16 q=0; 
	int s1=0;
	int s2=0;
	int s=U1_wshopname(towntype);
	s1=((s%100)/10)-1;
	s2=s/100-1;
	if(s1!=-1)
		strcpy(txt[0], game->out.S_weaponshops[s1]);
	if(s2==8)
	{
		strcpy(txt[1], game->out.S_armorshops[1]);
		s2=-1;
	}
	if(s2!=-1)
		strcpy(txt[1], game->out.S_weaponshops[s2]);
	if(txt[1][0]==0)
		offset=0;
	switch(inp)
	{
		case 'B':
 		case 'b':	
 				term->print("Transact-Buy: ");
		o=0;
		for(int x=2-stuff;x<(11-stuff);x+=2)
		{
			char temp[20];
			U1_weaponName(temp, x);
			int price=U1_getWeaponBuyPrice(x, game->user.intelligence);
			U1_shopString(txt[x/2+2+offset-(stuff==0)], temp, x, price);
			options[o]=x+'a';
			o++;
		}
		refresh(screen, game->screen);
		q=U1_msgBox(screen, term, &game->user, txt, 0);
		o=0;
		b=0;
		while(o<20)
		{
			b=b||(options[o]==q);
			o++;
		}
		if(b)
		{
			char temp[25];
			U1_weaponName(temp, q-'a');
			cost=U1_getWeaponBuyPrice(q-'a', game->user.intelligence);
			if(game->user.coin>=cost)
			{
				term->print(temp);
				term->print("\n");
				game->user.coin-=cost;
				game->user.weapon[q-'a']++;
			}
			else
			{
				term->print("nothing\n");
				for(int x=2;x<11;x++)
				{
					txt[x][0]=0;
				}
				strcpy(txt[5], "Thou canst not afford it!");
				refresh(screen, game->screen);
				q=U1_msgBox(screen, term, &game->user, txt, 0);
			}
		}
			else
				term->print("nothing\n");
			term->blit();
		break;
		case 'S':
		case 's':	term->print("Transact-Sell: ");
		o=1;
		for(int x=1;x<16;x++)
		{		
			int z=game->user.weapon[x];
	//		if(game->user.weaponready==x)
	//			z--;
			if((z>0)&&(o<9))
			{
				int price=U1_getWeaponSellPrice(x, game->user.charisma);
				char temp[30];
				U1_weaponName(temp, x);
				U1_shopString(txt[o+offset+1], temp, x, price);
				options[o-1]=x+'a';
				o++;
			}
		}
		if(options[0]==0)
		{
			strcpy(txt[5], "Thou hast no weaponry to sell!");
			
		}
		refresh(screen, game->screen);
		q=U1_msgBox(screen, term, &game->user, txt, 0);
		b=0;
		for(int x=0;x<16;x++)
			b=b||(options[x]==q);
		if(b&&q)
		{
			if((q-'a')==(game->user.weaponready))
				game->user.weaponready=0;
			game->user.weapon[q-'a']--;
			game->user.coin+=U1_getWeaponSellPrice(q-'a', game->user.charisma);
			char temp[30];
			U1_weaponName(temp, q-'a');
			term->print(temp);term->print("\n");
		}
		else
			term->print("nothing\n");
		break;
		default:  term->print("Transact nothing\n");break;
	}
	return 0;
}

int U1_transactArmor(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype)
{
	term->print("Transact-Buy, Sell: ");
	refresh(screen, game->screen);
	int inp=term->waitKey();
	term->dropLine();
	U1_printWithTick(term, "");
	char txt[12][31];
	for(int x=0;x<11;x++)
		txt[x][0]=0;
	int offset=1;
//	int stuff=towntype%2;
	Uint16 options[20];
	for(int x=0;x<20;x++)
	options[x]=0;
	int o=0;
	int b=0;
	int cost=0;
	Uint16 q=0; 
	int s1=0;
	int s2=0;
	int s=U1_ashopname(towntype);
	s1=((s%100)/10)-1;
	s2=s/100-1;
	if(s1!=-1)
		strcpy(txt[0], game->out.S_armorshops[s1]);
	if(s2!=-1)
		strcpy(txt[1], game->out.S_armorshops[s2]);
	if(txt[1][0]==0)
		offset=0;
	switch(inp)
	{
		case 'B':
 		case 'b':	
 				term->print("Transact-Buy: ");
				o=0;
				for(int x=1;x<=5;x++)
				{
					char temp[20];
					U1_armorName(temp, x);
					int price=U1_getArmorBuyPrice(x, game->user.intelligence);
					U1_shopString(txt[x+1+offset], temp, x, price);
					options[o]=x+'a';
					o++;
				}
				refresh(screen, game->screen);
				q=U1_msgBox(screen, term, &game->user, txt, 0);
				o=0;
				b=0;
				while(o<20)
				{
					b=b||(options[o]==q);
					o++;
				}
				if(b)
				{
					char temp[25];
					U1_armorName(temp, q-'a');
					cost=U1_getArmorBuyPrice(q-'a', game->user.intelligence);
					if(game->user.coin>=cost)
					{
						term->print(temp);
						term->print("\n");
						game->user.coin-=cost;
						game->user.armor[q-'a']++;
					}
					else
					{
						term->print("nothing\n");
						for(int x=2;x<11;x++)
						{
							txt[x][0]=0;
						}
						strcpy(txt[5], "Thou canst not afford it!");
						refresh(screen, game->screen);
						q=U1_msgBox(screen, term, &game->user, txt, 0);
					}
				}
				else
					term->print("nothing\n");
				term->blit();
				break;
		case 'S':
		case 's':	term->print("Transact-Sell: ");
				o=1;
				for(int x=1;x<6;x++)
				{		
					int z=game->user.armor[x];
					if(game->user.armorready==x)
						z--;
					if(z>0)
					{
						int price=U1_getArmorSellPrice(x, game->user.charisma);
						char temp[30];
						U1_armorName(temp, x);
						U1_shopString(txt[o+offset+1], temp, x, price);
						options[o-1]=x+'a';
						o++;
					}
				}
				if(options[0]==0)
				{
					strcpy(txt[5], "Thou hast no armour to sell!");
				}
				refresh(screen, game->screen);
				q=U1_msgBox(screen, term, &game->user, txt, 0);
				b=0;
				for(int x=0;x<6;x++)
					b=b||(options[x]==q);
				if(b&&q)
				{
					game->user.armor[q-'a']--;
					game->user.coin+=U1_getArmorSellPrice(q-'a', game->user.charisma);
					char temp[30];
					U1_armorName(temp, q-'a');
					term->print(temp);term->print("\n");
				}
				else
					term->print("nothing\n");
				break;
				default:  term->print("Transact nothing\n");break;
	}
	return 0;

}

int U1_transactMagic(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype)
{
	term->print("Transact-Buy, Sell: ");
	refresh(screen, game->screen);
	int inp=term->waitKey();
	term->dropLine();
	U1_printWithTick(term, "");
	char txt[12][31];
	for(int x=0;x<11;x++)
		txt[x][0]=0;
	int offset=1;
	int stuff=towntype%2;
	Uint16 options[20];
	for(int x=0;x<20;x++)
	options[x]=0;
	int o=0;
	int b=0;
	int cost=0;
	Uint16 q=0; 
	int s1=0;
	int s2=0;
	int s=U1_mshopname(towntype);
	s1=((s%100)/10)-1;
	s2=s/100-1;
	if(s1!=-1)
		strcpy(txt[0], game->out.S_spellshops[s1]);
	if(s2!=-1)
		strcpy(txt[1], game->out.S_spellshops[s2]);
	if(txt[1][0]==0)
		offset=0;
	int w=0;
	switch(inp)
	{
		case 'B':
 		case 'b':	
 				term->print("Transact-Buy: ");
// insert buy code

		o=0;
		w=6;
		if(game->user.clas==2)
			w=10;
		for(int x=stuff+1;x<w+stuff;x+=2)
		{
			char temp[20];
			U1_spellName(temp, x);
			int price=U1_getSpellBuyPrice(x, game->user.wisdom);
			U1_shopString(txt[x/2+2+offset], temp, x, price);
			options[o]=x+'a';
			o++;
		}
		refresh(screen, game->screen);
		q=U1_msgBox(screen, term, &game->user, txt, 0);
		o=0;
		b=0;
		while(o<20)
		{
			b=b||(options[o]==q);
			o++;
		}
		if(b)
		{
			char temp[25];
			U1_spellName(temp, q-'a');
			cost=U1_getSpellBuyPrice(q-'a', game->user.wisdom);
			if(game->user.coin>=cost)
			{
				term->print(temp);
				term->print("\n");
				game->user.coin-=cost;
				game->user.spell[q-'a']++;
			}
			else
			{
				term->print("nothing\n");
				for(int x=2;x<11;x++)
				{
					txt[x][0]=0;
				}
				strcpy(txt[5], "Thou canst not afford it!");
				refresh(screen, game->screen);
				q=U1_msgBox(screen, term, &game->user, txt, 0);
			}
		}
			else
				term->print("nothing\n");
			term->blit();
		break;

		case 'S':
		case 's':	term->print("Transact-Sell: ");
//insert sell code

		term->print("nothing\n");
		strcpy(txt[5], "Sorry, we don't buy spells!");
		refresh(screen, game->screen);
		q=U1_msgBox(screen, term, &game->user, txt, 0);
		break;
		default:  term->print("Transact nothing\n");break;
	}	
	return 0;
}

int U1_transactTransport(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype, int craftshopclosed, TU1_map map)
{
	int landPlacable=0;
	for(int x=-1;x<=1;x++)
		for(int y=-1;y<=1;y++)
		{
			landPlacable=landPlacable||((U1_isPassable(map, game->user.x+x, game->user.y+y, 10, 1)==0)&&(U1_enemyBlock(&game->user, game->user.x+x, game->user.y+y)==-1));
		}
	int waterPlacable=0;
	for(int x=-1;x<=1;x++)
		for(int y=-1;y<=1;y++)
		{
			waterPlacable=waterPlacable||((U1_isPassable(map, game->user.x+x, game->user.y+y, 13, 1)==0)&&(U1_enemyBlock(&game->user, game->user.x+x, game->user.y+y)==-1));
		}
	int aircarPlacable=0;
	for(int x=-1;x<=1;x++)
		for(int y=-1;y<=1;y++)
		{
			aircarPlacable=aircarPlacable||((U1_isPassable(map, game->user.x+x, game->user.y+y, 16, 1)==0)&&(U1_enemyBlock(&game->user, game->user.x+x, game->user.y+y)==-1));
		}
	term->print("Transact-Buy, Sell: ");
	refresh(screen, game->screen);
	int inp=term->waitKey();
	term->dropLine();
	U1_printWithTick(term, "");
	char txt[12][31];
	for(int x=0;x<11;x++)
		txt[x][0]=0;
	int offset=1;
//	int stuff=towntype%2;
	Uint16 options[20];
	for(int x=0;x<20;x++)
		options[x]=0;
	int o=0;
	int b=0;
	int cost=0;
	Uint16 q=0; 
	int s1=0;
	int s2=0;
	int s=U1_tshopname(towntype);
	s1=((s%100)/10)-1;
	s2=s/100-1;
	if(s1!=-1)
		strcpy(txt[0], game->out.S_transportshops[s1]);
	if(s2!=-1)
		strcpy(txt[1], game->out.S_transportshops[s2]);
	if(txt[1][0]==0)
		offset=0;
	switch(inp)
	{
		case 'B':
 		case 'b':	
 				term->print("Transact-Buy: ");
// insert buy code

		o=0;
		if(craftshopclosed==0)
		{
			for(int x=1;x<7;x++)
			{
//				if((x<3)||(x>4)||(((x==3)||(x==4))&&(U1_hasWater(game->user.x, game->user.y, map))))
				if(((x<3)&&(landPlacable))||(((x==3)||(x==4))&&(waterPlacable))||((x==5)&&(aircarPlacable))||((x==6)&&(0)))
				{
					char temp[20];
					U1_craftName(temp, x);
					int price=U1_getCraftBuyPrice(x, game->user.intelligence);
					U1_shopString(txt[o+1+offset+1], temp, x-1, price);
					options[o]=x-1+'a';
					o++;
				}
			}
			if(o>0)
			{
				refresh(screen, game->screen);
				q=U1_msgBox(screen, term, &game->user, txt, 0);
			}
			else
			{
				craftshopclosed=1;
			}
		}
		o=0;
		b=0;
		while(o<20)
		{
			b=b||(options[o]==q);
			o++;
		}
		if(options[0]!=0)
		{
			if(b)
			{
				char temp[25];
				U1_craftName(temp, q-'a'+1);
				cost=U1_getCraftBuyPrice(q-'a'+1, game->user.intelligence);
				if(game->user.coin>=cost)
				{
					term->print(temp);
					term->print("\n");
					craftshopclosed=1;
					game->user.coin-=cost;
					game->user.craft[q-'a'+1]++;
					int shape=0;
					switch(q)
					{
						case 'a':	shape=11;break;	//horse
						case 'b':	shape=12;break;	//cart
						case 'c':	shape=13;break;	//raft
						case 'd':	shape=14;break;	//frigate
						case 'e':	shape=16;break;	//aircar
						case 'f':	shape=17;break;	//shuttle
						case 'g':	shape=18;break;	//time machine
					}
					int cx=game->user.x;
					int cy=game->user.y;
					for(int x=-1;x<=1;x++)
						for(int y=-1;y<=1;y++)
						{
							if((U1_isPassable(map, game->user.x+x, game->user.y+y, shape, 1)==0)&&(U1_enemyBlock(&game->user, game->user.x+x, game->user.y+y)==-1))
							{
								cx=game->user.x+x;
								cy=game->user.y+y;
							}
						}
					game->user.newNPC(shape, cx, cy);
				}
				else
				{
					term->print("nothing\n");
					for(int x=2;x<11;x++)
					{
						txt[x][0]=0;
					}
					strcpy(txt[5], "Thou canst not afford it!");
					refresh(screen, game->screen);
					q=U1_msgBox(screen, term, &game->user, txt, 0);
				}
			}
			else
				term->print("nothing\n");
			term->blit();
		}
		else
		{
			strcpy(txt[5], "Closed for the day.");
			refresh(screen, game->screen);
			q=U1_msgBox(screen, term, &game->user, txt, 0);
 			term->print("\n");			
		}
		break;

		case 'S':
		case 's':	term->print("Transact-Sell: ");
//insert sell code

		term->print("nothing\n");
		strcpy(txt[5], "Sorry, we don't deal in");
		strcpy(txt[6], "used stuff.");
		refresh(screen, game->screen);
		q=U1_msgBox(screen, term, &game->user, txt, 0);
		break;
		default:  term->print("Transact nothing\n");break;
	}	
	
	return craftshopclosed;
}

int U1_transactPub(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype)
{
	term->print("Transact-Buy, Sell: ");
	refresh(screen, game->screen);
	int inp=term->waitKey();
	term->dropLine();
	U1_printWithTick(term, "");
	char txt[12][31];
	for(int x=0;x<11;x++)
		txt[x][0]=0;
	int offset=1;
//	int stuff=towntype%2;
	Uint16 options[20];
	for(int x=0;x<20;x++)
	options[x]=0;
//	int o=0; 
//	int b=0;
//	int cost=0;
	Uint16 q=0; 
	int s1=0;
	int s2=0;
	int s=U1_pubname(towntype);
	s1=((s%100)/10)-1;
	s2=s/100-1;
	if(s1!=-1)
		strcpy(txt[0], game->out.S_pubs[s1]);
	if(s2!=-1)
		strcpy(txt[1], game->out.S_pubs[s2]);
	if(txt[1][0]==0)
		offset=0;
	switch(inp)
	{
 		case SDLK_b:	
 				term->print("Transact-Buy: ");
// insert buy code

			if(game->user.coin)
			{
				game->user.coin--;
				char temp[10];
				sprintf(temp, "%s\n", game->out.S_pubtext[38]);
				term->print(temp);
				int response=rand()%16;
				int base=0;
				int max=0;
				switch(response)
				{
					case 0:	base=1;max=3;game->user.coin/=2;break;
					case 1:	base=4;max=7;break;
					case 2: base=8;max=8;break;
					case 3:	base=11;max=14;break;
					case 4:	base=15;max=17;break;
					case 5: base=18;max=19;break;
					case 6:	base=20;max=20;break;
					case 7:	base=21;max=32;break;
					default:	base=36;max=37;break;
				}
				if((response<7)&&(response!=2))
				{
					strcpy(txt[5-((max-base)/2)], game->out.S_pubtext[0]);
					for(int x=0;x<=(max-base);x++)
					{
						strcpy(txt[6-((max-base)/2)+x], game->out.S_pubtext[x+base]);
					}
				}
				else
				if(response==7)
				{
					strcpy(txt[3], game->out.S_pubtext[0]);
					for(int x=0;x<=4;x++)
						strcpy(txt[4+x], game->out.S_pubtext[x+base]);
					refresh(screen, game->screen);
					U1_msgBox(screen, term, &game->user, txt, 0);
					for(int x=1;x<12;x++)
						txt[x][0]=0;
					for(int x=5;x<=(max-base);x++)
						strcpy(txt[6-((max-base-5)/2)+(x-5)], game->out.S_pubtext[x+base]);
				}
				else
				if(response==2)
				{
					strcpy(txt[5-((max-base)/2)], game->out.S_pubtext[0]);
					char temp[40];
					sprintf(temp, "%s%s", game->out.S_pubtext[base], game->out.S_pubtext[base+1+towntype%2]);
					strcpy(txt[6-((max-base)/2)], temp);
				}
				else
				{
					for(int x=0;x<=(max-base);x++)
						strcpy(txt[5-((max-base)/2)+x], game->out.S_pubtext[x+base]);
				}
			}
			else
			{
				term->print("nothing\n");
				for(int x=0;x<=2;x++)
					strcpy(txt[x+4], game->out.S_pubtext[x+33]);
			}
			refresh(screen, game->screen);
			U1_msgBox(screen, term, &game->user, txt, 0);
		break;

		case SDLK_s:	term->print("Transact-Sell: ");
//insert sell code

		term->print("nothing\n");
		strcpy(txt[5], "We have plenty of booze");
		strcpy(txt[6], "already!");
		refresh(screen, game->screen);
		q=U1_msgBox(screen, term, &game->user, txt, 0);
		break;
		default:  term->print("Transact nothing\n");break;
	}	
	return inp;
}
