#include <SDL.h>

void refresh(SDL_Surface *s, SDL_Surface *s2);
void refreshbuffer(SDL_Surface *s, SDL_Surface *s2);

class Tfont
{
	public:
	SDL_Surface *chr[256];
	Tfont();
	int load(char *filename);
	Tfont(char *filename);
	int load(char **u1font_xpm);
	Tfont(char **u1font_xpm);
	~Tfont();	
	SDL_Surface *print(char *text, Uint32 R, Uint32 G, Uint32 B);
	SDL_Surface *print(char *text);
};

class Tterminal
{
	public:
	char line[25][80];
	int lines;
	int columns;
	int xpos;
	int ypos;
	int xx;
	int yy;
	int dR;
	int dG;
	int dB;
	int startframe;
	int numcursorframes;
	int cursorframe;
	int lastkey;
	int waitloops;
	int waitdelay;
	int waitct;
	SDL_Surface *SCREEN;
	SDL_Surface *screen;
	SDL_Surface *background;
	SDL_Event *event;
	int clearMouseEvents();
	Tfont *font;
	Tterminal();
	Tterminal(SDL_Surface *scrn, int x, int y, int cls, int lns, Uint32 R, Uint32 G, Uint32 B, char *fontfilename);
	Tterminal(SDL_Surface *scrn, int x, int y, int cls, int lns, Uint32 R, Uint32 G, Uint32 B, char **font_xpm);
	~Tterminal();
	int clear(){for(int x=0;x<lines;x++)line[x][0]=0;xx=0;yy=0;return 0;}
	int append(char *text);
	int print(char *text);
	int dropLine();
	int output(SDL_Surface *scrn, int x, int y, char *text, Uint32 R, Uint32 G, Uint32 B);
	int blit();
	int getKey();
	int waitKey();
	int getNum(int min, int max);
	int getWord(char *text, int max);
};

int TERMINAL_STRCPY(char *dest, char *source);
int output(SDL_Surface *screen, Tfont *font, int x, int y, char *text, Uint32 R, Uint32 G, Uint32 B);
Uint16 swapInt16Endian(Uint16 n);
int getKey(SDL_Surface *screen);
int getKey(SDL_Surface *screen, SDL_Event *event);
int waitKey(SDL_Surface *screen);
int waitKey(SDL_Surface *screen, int maxwait);
int getNum(SDL_Surface *buffer, SDL_Surface *screen, Tfont *font, int x, int y, Uint8 R, Uint8 G, Uint8 B, int min, int max);
int getWord(SDL_Surface *buffer, SDL_Surface *screen, Tfont *font, char *word, int x, int y, Uint8 R, Uint8 G, Uint8 B, int max);
SDL_Surface *setBrightness(SDL_Surface *screen, float brightness);
Uint32 getpixel(SDL_Surface *surface, int x, int y);
void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B);
int SDL_addAlpha(SDL_Surface *mask, SDL_Surface *s);
void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 A);
int getAvgColor(SDL_Surface *s, Uint8 *R, Uint8 *G, Uint8 *B);
int setEGAcolors(Tcolor ega[16]);
int setCGAcolors(Tcolor cga[4]);
int setCGAcolors0(Tcolor cga[4]);
int setCGAcolors1(Tcolor cga[4]);
int setCGAcolors2(Tcolor cga[4]);
int setCGAcompositecolors0(Tcolor cga[16]);
int setCGAcompositecolors1(Tcolor cga[16]);
int setCGAcompositecolors2(Tcolor cga[16]);
int setCGAcompositecolors3(Tcolor cga[16]);
int setCGAcompositecolors(Tcolor cga[16]);
int setVGAcolors(Tcolor vga[256]);
int CGAtoComposite(SDL_Surface *surface, int pal);

