#include "u0types.h"

int U0_main(SDL_Surface *screen, int audio_open);
int U0_drawMap(SDL_Surface *screen, U0_player *player);
int U0_mainLoop(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_chargen(SDL_Surface *screen, U0_player *player);
int U0_printWeapons(SDL_Surface *screen, U0_player *player);
int U0_adventureShop(SDL_Surface *screen, U0_player *player);
int U0_updatefood(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_ter_up(SDL_Surface *screen, U0_player *player, Tterminal *term); 
int U0_ter_right(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_ter_left(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_ter_down(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_ter_a(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_ter_e(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_z(SDL_Surface *screen, U0_player *player, Tterminal *term);
int U0_castle(SDL_Surface *screen, U0_player *player);
int U0_resurrection(SDL_Surface *screen, U0_player *player);

