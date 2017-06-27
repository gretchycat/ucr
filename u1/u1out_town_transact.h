#include <SDL.h>

int U1_transactKing(SDL_Surface *screen, Tterminal *term, TU1_game *game);
int U1_transactFood(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype);
int U1_transactWeapon(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype);
int U1_transactArmor(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype);
int U1_transactMagic(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype);
int U1_transactTransport(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype, int craftshopclosed, TU1_map map);
int U1_transactPub(SDL_Surface *screen, Tterminal *term, TU1_game *game, int towntype);
