#include <SDL.h>

struct Tcolor{
	Uint8 R;
	Uint8 G;
	Uint8 B;
};

struct TgameConfig
{
  char gamepath[512];
  int music;
  int sfx;
  int svga;
};

class TglobalConfig
{
  public:
  TglobalConfig();
  ~TglobalConfig();
  int load();
  int save();
  int xres;
  int yres;
  char datapath[512];
  char pixmaps[512];
  int splashmusic;
  TgameConfig game[7];
};
