#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include "../globaltypes.h"
#include "u1types.h"

int U1_townname(int x, int y)
{
	int z=y*168+x;
	int n=0;
//	int towntype=0;
	switch(z)
	{
		case  38*168+40:	n=32;break;
		case  27*168+32:	n=33;break;
		case  39*168+39:	n=0; break;	//b
		case  41*168+66:	n=1; break;		//a
		case  61*168+25:	n=2; break;		//b
		case  28*168+46:	n=3; break;		//
		case  63*168+52:	n=4; break;	//b
		case  34*168+18:	n=5; break;		//a
		case  63*168+70:	n=6; break;		//b
		case  22*168+64:	n=7; break;		//a
		case 37*168+125:	n=34;break;
		case 29*168+114:	n=35;break;
		case 36*168+126:	n=8; break;
		case 63*168+128:	n=9; break;
		case 22*168+148:	n=10;break;
		case 43*168+115:	n=11;break;
		case 49*168+150:	n=12;break;
		case 15*168+121:	n=13;break;
		case 67*168+150:	n=14;break;
		case 61*168+109:	n=15;break;
		case 118*168+41:	n=36;break;
		case 126*168+30:	n=37;break;
		case 119*168+42:	n=16;break;
		case  92*168+44:	n=17;break;
		case 133*168+64:	n=18;break;
		case 112*168+31:	n=19;break;
		case 106*168+66:	n=20;break;
		case 140*168+37:	n=21;break;
		case  88*168+66:	n=22;break;
		case  94*168+25:	n=23;break;
		case 116*168+127:	n=38;break;
		case 105*168+135:	n=39;break;
		case 117*168+128:	n=24;break;
		case 119*168+101:	n=25;break;
		case 139*168+142:	n=26;break;
		case 106*168+121:	n=27;break;
		case 141*168+115:	n=28;break;
		case 112*168+149:	n=29;break;
		case 141*168+97:	n=30;break;
		case 100*168+103:	n=31;break;
		default:	n=0;break;
	}
	return n;
}

int U1_shrinename(int x, int y)
{
	int z=y*168+x;
	switch(z)
	{
		case 36 + 9   *168: return 0; //+2 agility  +2 agaility		pillars of protection
		case 69 + 10  *168: return 1; //+2 int. +2 int						tower of knowledge
  	case 96 + 33  *168: return 2; //Find an Axe rope&spikes		pillars of the argonauts
		case 97 + 66  *168: return 3; //+4 wisdom +5 wisdom +5 +6	pillar of ozymandias
		case 13 + 89  *168: return 4; //+2 stamina	+3 stamina		the signpost
		case 12 + 122 *168: return 5; //+2 charisma 							southern signpost
		case 131+ 87  *168: return 6; //  NOTHING - GO EAST!			eastern signpost
		case 98 + 88  *168: return 7; //+2 stamina  +3 stamina +3	grave of the lost soul
	}
	return 0;
}


int U1_dungeonname(int x, int y)
{
	int z=y*168+x;
	switch(z)
	{
		case 48 + 11  *168: return 0;
		case 18 + 13  *168: return 1;
  	case 53 + 22  *168: return 2;
		case 59 + 29  *168: return 3;
		case 29 + 37  *168: return 4;
		case 13 + 43  *168: return 5; 
		case 62 + 49  *168: return 6;
		case 39 + 60  *168: return 7;
		case 38 + 68  *168:	return 8;
		
		case 130+ 10  *168: return 9;
		case 100+ 15  *168: return 10;
  	case 124+ 26  *168: return 11;
		case 155+ 35  *168: return 12;
		case 147+ 36  *168: return 13;
		case 98 + 45  *168: return 14; 
		case 109+ 50  *168: return 15;
		case 116+ 56  *168: return 16;
		case 136+ 59  *168:	return 17;
		
		case 52 + 96  *168: return 18;
		case 32 + 99  *168: return 19;
  	case 25 + 105 *168: return 20;
		case 14 + 110 *168: return 21;
		case 63 + 119 *168: return 22;
		case 71 + 120 *168: return 23; 
		case 40 + 129 *168: return 24;
		case 16 + 140 *168: return 25;
		case 46 + 145 *168:	return 26;
		
		case 119+ 89  *168: return 27;
		case 149+ 91  *168: return 28;
  	case 114+ 100 *168: return 29;
		case 108+ 107 *168: return 30;
		case 138+ 115 *168: return 31;
		case 154+ 121 *168: return 32; 
		case 105+ 127 *168: return 33;
		case 128+ 138 *168: return 34;
		case 129+ 146 *168:	return 35;
		
	}
	return 0;
}

int U1_tshopname(int towntype)
{
	int s1=-1;
	int s2=-1;
	switch(towntype)
	{
		case 2: s1=0;s2=1;break;
		case 5: s1=2;break;
		case 7: s1=3;break;
		case 9: s1=4;s2=5;break;
	}
	return 10*(s1+1)+100*(s2+1);
}

int U1_ashopname(int towntype) 
{
	int s1=-1;
	int s2=-1;
	switch(towntype)
	{
		case 2: s1=0;s2=1;break;
		case 3: s1=2;break;
		case 4: s1=3;break;
		case 5: s1=4;break;
		case 7: s1=5;break;
		case 9: s1=6;break;
	}
	return 10*(s1+1)+100*(s2+1);
}

int U1_wshopname(int towntype)
{
	int s1=-1;
	int s2=-1;
	switch(towntype)
	{
		case 2:	s1=0;s2=8;break;
		case 3: s1=1;break;
		case 4: s1=2;break;
		case 5: s1=3;break;
		case 6: s1=4;break;
		case 7: s1=5;break;
		case 8: s1=6;break;
		case 9: s1=3;break;
	}
	return 10*(s1+1)+100*(s2+1);
}

int U1_mshopname(int towntype)
{
	int s1=-1;
	int s2=-1;
	switch(towntype)
	{
		case 2: s1=0;break;
		case 3: s1=1;break;
		case 4: s1=2;break;
		case 5: s1=3;break;
		case 9: s1=4;break;
	}
	return 10*(s1+1)+100*(s2+1);
}

int U1_fshopname(int towntype)
{
	int s1=-1;
	int s2=-1;
	switch(towntype)
	{
		case 2: s1=0;break;
		case 3: s1=1;break;
		case 5: s1=2;break;
		case 6: s1=3;break;
		case 7: s1=4;break;
		case 8: s1=5;break;
		case 9: s1=6;break;
	}
	return 10*(s1+1)+100*(s2+1);
}

int U1_pubname(int towntype) 
{
	int s1=-1;
	int s2=-1;
	switch(towntype)
	{
		case 2: s1=0;break;
		case 3: s1=1;break;
		case 4: s1=2;break;
		case 5: s1=3;break;
		case 7: s1=4;break;
		case 8: s1=5;break;
		case 9: s1=6;break;
	}
	return 10*(s1+1)+100*(s2+1);
}

