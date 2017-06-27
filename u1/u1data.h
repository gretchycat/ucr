int U1_loadmap(TglobalConfig *config, TU1_map map);
int U1_setDefaults(TU1_user *user);
int U1_loaduser(TglobalConfig *config, TU1_user *user, int num);
int U1_saveuser(TglobalConfig *config, TU1_user *user, int num);
int U1_loadtowns(TglobalConfig *config, TU1_towns towns);
int U1_loadtiles(TglobalConfig *config, TU1_tiles *tiles, char *filename, int xsize, int ysize);
int U1_loadtiles(TglobalConfig *config, TU1_tiles *tiles, char *tiletype, unsigned char tileset);
int U1_loadnif(TglobalConfig *config, TU1_nif nif);
int U1_loadnpc(TglobalConfig *config, TU1_npc npc[10][15]);
int U1_EGAImage(TglobalConfig *config, SDL_Surface *screen, char *filename, int xx, int yy);
SDL_Surface *U1_loadOriginPic(TglobalConfig *config);

