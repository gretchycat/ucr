#include <SDL.h>
#include <SDL_mixer.h>

struct U0_player
{
	int ln;	//lucky number
	int lp;	//level of play
	int c[7];	//character attributes (HP, STR, DEX, STA, WIS, GP)
	char pt;	//player type (fighter or mage)
	char pn[80];	//player name
	int dis;
	int pa;
	float pw[6];
	int inout;
	char cs[6][30];
	char ms[10][30];	//monster names
	int ml[11][2];
	int mz[11][2];
	float w[7];	//food, rapier, axe, shield, bow and arrows, magic amulet
	char ws[7][30];
	float wI;
	int dn[11][11];
	int te[21][21];
	int xx[11];
	int yy[11];
	int per[11][4];
	float ld[11][6];
	float cd[11][4];
	float ft[11][6];
	float lad[11][4];
	int tx;
	int ty;
	int dx;
	int dy;
	int px;
	int py;
	int level;
	float zz;
	Uint32 curcolor;
	int task;
	int mr;
	int mc;	
	double cent;
	double l1;
	double l2;
	double t1;
	double t2;
	double r1;
	double r2;
	double b1;
	double b2;
	double left;
	double righ;
	int en;
	int lk;	
	int nm;
	int AUDIOSTATUS;
	int FRAMENUM;
	int DIRECTION;
	SDL_Surface *SCREEN;
	Mix_Music *u0music;
	TglobalConfig config;
	SDL_Surface *u0background;
	SDL_Surface *door_b;
	SDL_Surface *door_l;
	SDL_Surface *door_r;
	SDL_Surface *wall_b;
	SDL_Surface *u0rightwall;
	SDL_Surface *u0leftwall;
	SDL_Surface *u0dngback;
	SDL_Surface *u0trapdoor;
	SDL_Surface *u0tophole;
	SDL_Surface *u0ladder;
	SDL_Surface *u0title;
	SDL_Surface *u0giantrat;
	SDL_Surface *u0chest;
	SDL_Surface *u0mimic;
	SDL_Surface *u0skeleton;
	SDL_Surface *u0gremlin;
	SDL_Surface *u0thief;
	SDL_Surface *u0orc;
	SDL_Surface *u0viper;
	SDL_Surface *u0daemon;
	SDL_Surface *u0crawler;
	SDL_Surface *u0balrog;
	SDL_Surface *u0tree;
	SDL_Surface *u0dungeon;
	SDL_Surface *u0village;
	SDL_Surface *u0castle;
	SDL_Surface *u0mountain;
	SDL_Surface *u0grass;
	SDL_Surface *u0walku1;
	SDL_Surface *u0walku2;
	SDL_Surface *u0walku3;
	SDL_Surface *u0walkd1;
	SDL_Surface *u0walkd2;
	SDL_Surface *u0walkd3;
	SDL_Surface *u0walkl1;
	SDL_Surface *u0walkl2;
	SDL_Surface *u0walkl3;
	SDL_Surface *u0walkr1;
	SDL_Surface *u0walkr2;
	SDL_Surface *u0walkr3;
};

