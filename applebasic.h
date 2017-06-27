//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <math.h>
#include <SDL.h>
//#include "SDL_prim.h"
//#include "globaltypes.h"
//#include "globalfunctions.h"


void PRINT(SDL_Surface *screen, Tfont *font, int x, int y, char *s, int r, int g, int b);
float atn(float n);
float rnd(float n);
void hplot(SDL_Surface *screen, double x1, double y1, double x2, double y2, Uint32 color);
void HPLOT(SDL_Surface *screen, double x1, double y1);
void TO(SDL_Surface *screen, double x2, double y2, Uint32 color);
int sgn(int n);
