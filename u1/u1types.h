typedef Sint16 TU1_nif[84][40];
typedef int TU1_map[156][168];
typedef unsigned char TU1_towns[10][38][18];

struct TU1_out_strings
{
	char S_currency[4][40];
	char S_hittype[16][40];
	char S_board[10][40];
	char S_spell[17][40];
	char S_drop[12][40];
	char S_towntexts[20][40];
	char S_transportshops[6][40];
	char S_armorshops[7][40];
	char S_weaponshops[7][40];
	char S_spellshops[5][40];
	char S_foodshops[7][40];
	char S_pubs[7][40];
	char S_mimic[40];
	char S_directions[10][40];
	char S_princess[8][40];
	char S_shrinetexts[26][40];
	char S_enter[3][40];
	char S_fire[5][40];
	char S_get[5][40];
	char S_hyperjump[40];
	char S_inform[7][40];
	char S_klimb[7][40];
	char S_noise[3][40];
	char S_open[3][40];
	char S_quit[4][40];
	char S_steal[6][40];
	char S_shopbuy[3][40];
	char S_pubtext[41][40];
	char S_weaponsell[2][40];
	char S_armorsell[40];
	char S_spellsell[40];
	char S_foodsell[40];
	char S_foodbuy[7][40];
	char S_transportsell[3][40];
	char S_transact[50][40];
	char S_view[40];
	char S_unlock[7][40];
	char S_xit[3][40];
	char S_pass[40];
	char S_huh[40];
	char S_attack[8][40];
	char S_ready[7][40];
	char S_stats[4][40];
	char S_weapons[16][40];
	char S_weaponsshort[16][40];
	char S_races[4][40];
	char S_classes[4][40];
	char S_attributes[7][40];
	char S_spells[11][40];
	char S_armors[6][40];
	char S_crafts[8][40];
	char S_gems[4][40];
	char S_saves[4][40];
	char S_enemies[14][40];
	char S_spellchants[11][40];
	char S_towns[32][40];
	char S_castles[8][40];
	char S_shrines[8][40];
	char S_dungeons[36][40];
	char S_lands[4][40];
	char S_ladiesnames[8][40];
	char S_collisions[7][40];
	char S_nsew[4][10];
	char S_dungeonenemies[23][40];
	char S_ztats[11][40];
	char S_resurrect[2][40];
};

class TU1_tiles
{
	public:
	int x;
	int y;
	SDL_Surface *tiles[100];
	unsigned char xdim;
	unsigned char ydim;
	unsigned char num;
};

struct TU1_npc
{
	Uint16 shape;
	Uint16 x;
	Uint16 y;
	Sint16 hits;
};

struct TU1_mapNPC
{
	Uint16 shape;
	Uint16 flags;
	Uint16 x;
	Uint16 y;
	Sint16 hits;	//hits?
	Uint16 v1;
	Uint16 v2;
	Uint16 v3;
};

class TU1_user
{
	public:
	char name[16];		//0000
	Uint16 race;		//0010
	Uint16 clas;		//0012
	Uint16 sex;		//0014
	Sint16 hits;		//0016
	Uint16 strength;	//0018
	Uint16 agility;		//001A
	Uint16 stamina;		//001C
	Uint16 charisma;	//001E
	Uint16 wisdom;		//0020
	Uint16 intelligence;	//0022
	Uint16 coin;		//0024
	Uint16 exp;		//0026
	Sint16 food;		//0028
	Sint16 weaponready;	//002A
	Sint16 spellready;	//002C
	Sint16 armorready;	//002E
	Sint16 craftready;	//0030
	Uint16 v5;		//0032 ???
	Sint16 x;		//0034
	Sint16 y;		//0036
	Uint16 v6; 		//0038 ???
	Uint16 v7;		//003A ???
	Sint16 quest[8];	//003C
	Sint16 gem[4];		//004C
	Sint16 armor[6];	//0054
	Sint16 weapon[16];	//0060
	Sint16 spell[11];	//0080
	Sint16 craft[8];	//0096
	Uint16 v8;		//00A6 ???
	Uint16 lastsignpost;	//00A8 last signpost
	Uint16 enemies;		//number of enemies
	Uint16 moves;		//number of moves
	Uint16 v11;		//00AE ???
	Uint16 v12;		//00B0 ???
	Uint16 v13;		//00B2 ???
	TU1_mapNPC npc[40];	//00B4-end
	int eraseNPC(int num);
	int newNPC(int shape, int xx, int yy);
};


struct TU1_dungeon	//stolen from aklabeth - for dungeon conversion
{
//	TU1_user *user;
//	int c[7];		//**character attributes (HP, STR, DEX, STA, WIS, GP)
//	char pt;		//**player type (fighter or mage) -- Uint16 clas
//	int task;		//**which monster to kill (quest[8])
//	float pw[6];		//**player weapons
//	int ln;			//lucky number
	int lp;			//level of play
	char ms[10][30];	//monster names
	char ws[7][30];		//weapon names
	int pa;			//pause
	int dis;		//distance (for dungeon rendering)
	int inout;		//dungeon level
	int dn[11][11];		//dungeon
	int ml[11][2];		//monster location
	int mz[11][2];		//monster stats
	int xx[11];		//perepective
	int yy[11];		//perspective
	int per[11][4];		//perspective
	float ld[11][6];	//perspective
	float cd[11][4];	//perspective
	float ft[11][6];	//perspective
	float lad[11][4];	//ladder
	int dx;			//dungeon X
	int dy;			//dungeon Y
	int px;			//
	int py;			//
	Uint32 curcolor;	//line color
	int mc;			//monster center
	float cent;		//center
	float l1;		//left1
	float l2;		//left2
	float t1;		//top1
	float t2;		//top2
	float r1;		//right1
	float r2;		//right2
	float b1;		//bottom1
	float b2;		//bottom2
	float left;		//left something
	float righ;		//right something
	int en;			//
	int lk;			//gain hitpoints on exit
	int nm;			//number of monsters
	SDL_Surface *door_b;
	SDL_Surface *door_l;
	SDL_Surface *door_r;
	SDL_Surface *wall_b;
	SDL_Surface *u1rightwall;
	SDL_Surface *u1leftwall;
	SDL_Surface *u1dngback;
	SDL_Surface *u1trapdoor;
	SDL_Surface *u1tophole;
	SDL_Surface *u1ladder;
	SDL_Surface *u1title;
	SDL_Surface *u1giantrat;
	SDL_Surface *u1chest;
	SDL_Surface *u1mimic;
	SDL_Surface *u1skeleton;
	SDL_Surface *u1gremlin;
	SDL_Surface *u1thief;
	SDL_Surface *u1orc;
	SDL_Surface *u1viper;
	SDL_Surface *u1daemon;
	SDL_Surface *u1crawler;
	SDL_Surface *u1balrog;
};


struct TU1_game
{
	TglobalConfig config;
	SDL_Surface *screen;
	TU1_out_strings out;
	TU1_dungeon dungeon;
	TU1_user user;
	int key;
};
