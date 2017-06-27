#include <SDL.h>
//#include "globaltypes.h"
//#include "u1types.h"
//#include "u1global.h"

int U1_clearTownBuf(int townBuf[38][18]);
int U1_shopString( char *dest, char *src, int num, int price);
int U1_getFoodBuyPrice(int intelligence) ;
int U1_getWeaponBuyPrice(int wp, int intelligence);
int U1_getWeaponSellPrice(int wp, int charisma);
int U1_getArmorBuyPrice(int ar, int intelligence);
int U1_getArmorSellPrice(int ar, int charisma);
int U1_getSpellBuyPrice(int sp, int wisdom);
int U1_getCraftBuyPrice(int cr, int intelligence);
int U1_getSpellSellPrice(int sp, int charisma);
int U1_getCraftSellPrice(int cr, int charisma);
int U1_hasWater(int x, int y, TU1_map map);
int U1_craftPlacable(int shape, int x, int y, TU1_game *game, TU1_map map);
int printTables();
int U1_drawTown(SDL_Surface *screen, SDL_Surface *townbufsurface, int *updated, TU1_tiles tiles, TU1_towns towns, int tnum, int xx, int yy, TU1_user *user, TU1_npc npc[10][15]);
int U1_townCollision(TU1_towns towns, int tnum, TU1_npc npc[10][15], int xx, int yy, int x, int y, int npcnum);
int U1_navigateTown(SDL_Surface *screen, Tterminal *term, TU1_game *game, TU1_map map);
