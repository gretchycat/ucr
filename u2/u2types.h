#include <SDL.h>

typedef unsigned char TU2_map[64][64];

class TU2_tiles
{
	public:
	int x;
	int y;
	SDL_Surface *tiles[500];
	unsigned char xdim;
	unsigned char ydim;
	unsigned char num;
};

