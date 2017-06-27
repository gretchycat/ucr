int U1_printWithTick(Tterminal *term, char *str);
int U1_addBorder(SDL_Surface *screen);
int U1_addBorder(SDL_Surface *screen, int x1, int y1, int x2, int y2, int width);
int U1_msgBox(SDL_Surface *screen, Tterminal *term, TU1_user *user, char txt[11][31], int delay);
int U1_ztats(SDL_Surface *screen, Tterminal *term, TU1_game *game);
int U1_listWeapons(SDL_Surface *screen, Tterminal *term, TU1_game *game, char *headstr, int drop);
int U1_listArmors(SDL_Surface *screen, Tterminal *term, TU1_game *game, char *headstr, int drop);
int U1_listSpells(SDL_Surface *screen, Tterminal *term, TU1_game *game, char *headstr, int drop);
int U1_addTick(char *dest, char *src);
int U1_stats(SDL_Surface *screen, Tfont *font, TU1_user *user);
int U1_weaponName(char *name, int num);
int U1_armorName(char *name, int num);
int U1_spellName(char *name, int num);
int U1_raceName(char *name, int num);
int U1_className(char *name, int num);
int U1_sexName(char *name, int num);
int U1_craftName(char *name, int num);
int U1_craftSteps(int num);
int U1_enemyName(char *name, int num);
int U1_npcName(int name);
int U1_ready(SDL_Surface *screen, Tterminal *term, TU1_game *game);
int U1_inform(SDL_Surface *screen, TU1_game *game, Tterminal *term, TU1_map map);

