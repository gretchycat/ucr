#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "globaltypes.h"
#include "globalfunctions.h"
#include "SDL.h"
#include "SDL_audio.h"
#include "SDL_endian.h"
#include <SDL_image.h>
#include "declarations.h"
#include "SDL_rotozoom.h"
#include "2xsai.h"
void refresh(SDL_Surface *s, SDL_Surface *s2)
{
	if(s==NULL)
		printf("refresh: s is VOID!\n");	
	if(s2==NULL)
		printf("refresh: s is VOID!\n");	
	if((s->w!=s2->w)||(s->h!=s2->h))
	{
		float xscl=(float)s2->w/(float)s->w;
		float yscl=(float)s2->h/(float)s->h;
		SDL_Surface *tmp;
		if((xscl==2.0) && (yscl == 2.0))
		{
			tmp=SDL_Scale_2xSai(s);
		}
		else
		{
			tmp=zoomSurface(s, xscl, yscl, 1);
		}
		SDL_BlitSurface(tmp, NULL, s2, NULL);
		SDL_FreeSurface(tmp);
	}
	else
		SDL_BlitSurface(s, NULL, s2, NULL);
	SDL_UpdateRect(s2, 0, 0, 0, 0);
}

void refreshbuffer(SDL_Surface *s, SDL_Surface *s2)
{
	SDL_BlitSurface(s2, NULL, s, NULL);
	SDL_UpdateRect(s, 0, 0, 0, 0);   
}
								

Tfont::Tfont()
{
	return;
}	

Tfont::Tfont(char **font_xpm)
{
	load(font_xpm);
	return;
}

int Tfont::load(char **font_xpm)
{
	SDL_Surface *buffer=IMG_ReadXPMFromArray(font_xpm);
	if(buffer==NULL)
		return 1;
	SDL_Surface *bfr2=NULL;
	SDL_Rect r, r2;
	int xsz=buffer->w/256;
	int ysz=buffer->h;
	r.w=xsz;r.h=ysz;r.y=0;
	r2.w=xsz;r2.h=ysz;r2.y=0;r2.x=0;
	
	Uint32 rmask=0, gmask=0, bmask=0, amask=0;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif  
	for(int x=0;x<256;x++)
	{
		bfr2=SDL_CreateRGBSurface(SDL_SWSURFACE, xsz, ysz, 32, rmask, gmask, bmask, amask);
		r.x=x*xsz;
		if(SDL_BlitSurface(buffer, &r, bfr2, &r2))
			printf("\nError with blit.\n");
		chr[x]=bfr2;
		SDL_UpdateRect(buffer, 0, 0, chr[x]->w, chr[x]->h);
	}
	if(buffer!=NULL)
		SDL_FreeSurface(buffer);
	return 0;
}

Tfont::Tfont(char *filename)

{
	load(filename);
	return;
}

int Tfont::load(char *filename)
{
	SDL_Surface *buffer=IMG_Load(filename);
	if(buffer==NULL)
		return 1;
	if(buffer==NULL)
	{
		printf("bad filename: %s\n", filename);
		return 0;
	}
	SDL_Surface *bfr2=NULL;
	SDL_Rect r, r2;
	int xsz=buffer->w/256;
	int ysz=buffer->h;
	r.w=xsz;r.h=ysz;r.y=0;
	r2.w=xsz;r2.h=ysz;r2.y=0;r2.x=0;
	
	Uint32 rmask=0, gmask=0, bmask=0, amask=0;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif  
	for(int x=0;x<256;x++)
	{
		bfr2=SDL_CreateRGBSurface(SDL_SWSURFACE, xsz, ysz, 32, rmask, gmask, bmask, amask);
		r.x=x*xsz;
		if(SDL_BlitSurface(buffer, &r, bfr2, &r2))
			printf("\nError with blit.\n");
		chr[x]=bfr2;
		SDL_UpdateRect(buffer, 0, 0, chr[x]->w, chr[x]->h);
	}
	if(buffer!=NULL)
		SDL_FreeSurface(buffer);
	return 0;
}

Tfont::~Tfont()
{
	for(int x=0;x<256;x++)
		SDL_FreeSurface(chr[x]);
}

SDL_Surface *Tfont::print(char *text, Uint32 R, Uint32 G, Uint32 B)
{
	int blank=0;
	if((text[0]==' ')&&(text[1]==0))
		blank=1;
	int length=0;
	int P=0;
	while(text[P]!=0)
	{
		if(text[P]==0x03)
		{
			P+=4;
		}
		else
		{
			length++;
			P++;
		}
	}
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff*0;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000*0;
	#endif  

	SDL_Surface *buffer=SDL_CreateRGBSurface(SDL_SWSURFACE, length*chr[0]->w, chr[0]->h, 32, rmask, gmask, bmask, amask);

	SDL_Surface *cbuf=NULL;
	SDL_Rect r;
	int xsz=chr[0]->w;
	int ysz=chr[0]->h;
	r.w=xsz;r.h=ysz;r.y=0;
	int x=0;
	int px=0;
//	return buffer;
	while(text[x]!=0)
	{
		if(text[x]==3)
		{
			x++;
			R=text[x];
			x++;
			G=text[x];
			x++;
			B=text[x];
			x++;
		}
		else
		{

		cbuf=SDL_CreateRGBSurface(SDL_SWSURFACE, chr[0]->w, chr[0]->h, 32, rmask, gmask, bmask, amask);
		for(int y=0;y<chr[(int)text[x]]->h;y++)
		{
			for(int z=0;z<chr[(int)text[x]]->w;z++)
			{
				Uint32 *n=(Uint32*)chr[(int)text[x]]->pixels;
				Uint32 val=n[z+chr[(int)text[x]]->w*y];
			//	if(val!=SDL_MapRGB(chr[(int)text[x]]->format, 247, 2, 247))
			//	{
			//		Uint8 Rn, Gn, Bn;
			//		SDL_GetRGB(val, chr[(int)text[x]]->format, &Rn, &Gn, &Bn);
			//		DrawPixel(cbuf, z, y, Rn&R, Gn&G, Bn&B, 0xff);
			//	}
			//	else
			//	{
					Uint8 Rn, Gn, Bn;
					SDL_GetRGB(val, chr[(int)text[x]]->format, &Rn, &Gn, &Bn);
					DrawPixel(cbuf, z, y, Rn&R, Gn&G, Bn&B, 0xff);
			//	}
			}
		}
		r.x=px*xsz;
		if(SDL_BlitSurface(cbuf, NULL, buffer, &r))
			printf("\nError with blit.\n");
		if(cbuf)
			SDL_FreeSurface(cbuf);
		x++;
		px++;
		}
	}
	if(!blank)
		SDL_SetColorKey(buffer, SDL_SRCCOLORKEY, SDL_MapRGB(buffer->format, 0, 0, 0));
	SDL_UpdateRect(buffer, 0, 0, buffer->w, buffer->h);
	return buffer;
}

SDL_Surface *setBrightness(SDL_Surface *screen, float brightness)
{
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff*0;
	#else
	       	rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000*0;
	#endif
	SDL_Surface *temp=SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 32, rmask, gmask, bmask, amask);
//	SDL_Surface *temp2=SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(temp, NULL, 0);
//	SDL_SetAlpha(temp, SDL_SRCALPHA|SDL_RLEACCEL, (int)(255.0*(1-brightness)));
//	SDL_BlitSurface(screen, NULL, temp2, NULL);
//	SDL_BlitSurface(temp, NULL, temp2, NULL);
	Uint32 *n=(Uint32*)screen->pixels;
	for(int y=0;y<screen->h;y++)
		for(int x=0;x<screen->w;x++)
		{
			Uint32 val=n[x+screen->w*y];
			Uint8 Rn, Gn, Bn;
			SDL_GetRGB(val, screen->format, &Rn, &Gn, &Bn);
			DrawPixel(temp, x, y, (Uint8)(Rn*brightness), (Uint8)(Gn*brightness), (Uint8)(Bn*brightness), 0xff);
		}	
//	SDL_FreeSurface(temp2);
	return temp;
}

SDL_Surface *Tfont::print(char *text)
{
	return print(text, 255, 255, 255);
}

int reinitializescreen(SDL_Surface *screen, Uint32 VMODE)
{
	TglobalConfig config;
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
	       	rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif
//	SDL_Rect rect;
//	rect.x=0;rect.y=0;rect.w=screen->w;rect.h=screen->h;
	SDL_Surface *temp=SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, NULL, temp, NULL);
	SDL_UpdateRect(temp, 0, 0, 0, 0);
	screen = SDL_SetVideoMode(config.xres, config.yres, 0, VMODE);
	refresh(temp, screen);
//	SDL_BlitSurface(temp, NULL, screen, NULL);
//	SDL_UpdateRect(screen, 0, 0, 0, 0);
	SDL_FreeSurface(temp);
	return 0;	
}

int checkSpecialEvents(SDL_Surface *screen, SDL_Event *event)
{
	if(event->key.keysym.mod & KMOD_LCTRL)
	{
		if(event->key.keysym.sym==SDLK_MINUS)
		{	//windowed
			reinitializescreen(screen, WINDOWED);
		}
		if((event->key.keysym.sym==SDLK_EQUALS) || (event->key.keysym.sym==SDLK_PLUS))
		{	//fullscreen
			reinitializescreen(screen, FULLSCREEN);
		}
	}
	if(event->type == SDL_QUIT)
		return SDL_QUIT;
	return 0;
}

int output(SDL_Surface *screen, Tfont *font, int x, int y, char *text, Uint32 R, Uint32 G, Uint32 B)
{
	SDL_Surface *buffer=font->print(text, R, G, B);
	SDL_Rect r;
	r.h=font->chr[0]->h;
	r.w=font->chr[0]->w;
	r.x=x*font->chr[0]->w;
	r.y=y*font->chr[0]->h;
	if(SDL_BlitSurface(buffer, NULL, screen, &r))
		printf("Error with text blit.\n");
	SDL_FreeSurface(buffer);
	return 0;
}

int Tterminal::clearMouseEvents()
{
	int oldx=0;
	int oldy=0; 
	SDL_MouseMotionEvent *e = (SDL_MouseMotionEvent*)event;
	while(event->type==SDL_MOUSEMOTION)
	{
//		printf("blah\n");
		SDL_PollEvent(event);
		if((oldx==e->x) && (oldy==e->y))
			break;
		oldx=e->x;
		oldy=e->y;
	}
//	delete(event);
//	event=new(SDL_Event);
//	SDL_PollEvent(event);
	return 0;
}

Tterminal::Tterminal()
{
	printf("You must specify how many lines and columns when declaring a terminal type");
}

Tterminal::Tterminal(SDL_Surface *scrn, int x, int y, int cls, int lns, Uint32 r, Uint32 g, Uint32 b, char *fontfilename)
{
	if(lns>25)
		lns=25;
	for(int z=0;z<lns;z++)
		line[z][0]=0;
	lines=lns;
	startframe=4;
	numcursorframes=4;
	cursorframe=startframe;
	xx=0;
	yy=0;
	dR=r;
	dG=g;
	dB=b;
	if(cls>80)
		cls=80;
	columns=cls;
	for(int yp=0;yp<lines;yp++)
		line[yp][0]=0;
	event=new(SDL_Event);
	xpos=x;
	ypos=y;
	screen=scrn;
	font=new(Tfont);
	font->load(fontfilename);
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif
	SDL_Rect bufferrect;
	SDL_Rect rect;
	bufferrect.x=xpos;bufferrect.w=font->chr[0]->w*cls;
	bufferrect.h=font->chr[0]->h*lns;bufferrect.y=ypos;
	rect.x=0;rect.y=0;rect.w=bufferrect.w;rect.h=bufferrect.h;
//	printf("%d, %d, %d, %d\n", cls, lns, font->chr[0]->w, font->chr[0]->h); 
//	printf("%d, %d, %d, %d\n", bufferrect.x, bufferrect.y, bufferrect.w, bufferrect.h);
	background=SDL_CreateRGBSurface(SDL_SWSURFACE, bufferrect.w, bufferrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &bufferrect, background, &rect);
}

Tterminal::Tterminal(SDL_Surface *scrn, int x, int y, int cls, int lns, Uint32 r, Uint32 g, Uint32 b, char **font_xpm)
{
	if(lns>25)
		lns=25;
	for(int z=0;z<lns;z++)
		line[z][0]=0;
	lines=lns;
	startframe=4;
	numcursorframes=4;
	cursorframe=startframe;
	xx=0;
	yy=0;
	dR=r;
	dG=g;
	dB=b;
	waitct=0;
	waitdelay=12;
	waitloops=8;
	lastkey=0;
	if(cls>80)
		cls=80;
	columns=cls;
	for(int yp=0;yp<lines;yp++)
		line[yp][0]=0;
	event=new(SDL_Event);
	xpos=x;
	ypos=y;
	screen=scrn;
	font=new(Tfont);
	font->load(font_xpm);
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif
	SDL_Rect bufferrect;
	SDL_Rect rect;
	bufferrect.x=xpos;bufferrect.w=font->chr[0]->w*cls;
	bufferrect.h=font->chr[0]->h*lns;bufferrect.y=ypos;
	rect.x=0;rect.y=0;rect.w=bufferrect.w;rect.h=bufferrect.h;
//	printf("%d, %d, %d, %d\n", cls, lns, font->chr[0]->w, font->chr[0]->h); 
//	printf("%d, %d, %d, %d\n", bufferrect.x, bufferrect.y, bufferrect.w, bufferrect.h);
	background=SDL_CreateRGBSurface(SDL_SWSURFACE, bufferrect.w, bufferrect.h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(screen, &bufferrect, background, &rect);
}

int Tterminal::getKey()
{
	int val=0;
	int n=0;
	SDL_Delay(waitdelay);
	clearMouseEvents();
	SDL_PollEvent(event);
	if(event->type == SDL_KEYUP)
	{
		lastkey=0;
	}
	if(event->type == SDL_KEYDOWN)
	{
		n=event->key.keysym.sym;
	}	
	if((waitct%waitloops==0)||(lastkey!=n))
	{
		waitct=0;
		if(event->type == SDL_KEYDOWN)
		{
			val=n;
			if(lastkey==val)
				waitct=-1;
			lastkey=val;
		}	
	}
	if(checkSpecialEvents(screen, event)==SDL_QUIT)
		return SDL_QUIT;
	waitct++;
	return  val;
}

int Tterminal::waitKey()
{

//	int wait=8;
//	int ct=0;
	clearMouseEvents();
	int n=0;
	if(screen && SCREEN)
		refresh(screen, SCREEN);
	int t=0;
	while(n==0)
	{
		SDL_Delay(50);
		t=getKey();
//		if((ct>wait)||(t!=lastkey))
			n=t;
//		ct++;		
		if(n==SDL_QUIT)
			return SDL_QUIT;
		blit();
	}
//	clearEvents();
//	lastkey=n;
	return n;
}

int getNum(SDL_Surface *buffer, SDL_Surface *screen, Tfont *font, int x, int y, Uint8 R, Uint8 G, Uint8 B, int min, int max)
{
	int go=1;
	char number[16];
	int p=0;
	while(go)
	{
		char n=0;
		SDL_Delay(2);
		n=waitKey(screen);
		if(n==SDL_QUIT)
			return SDL_QUIT;
		if((n<='9')&&(n>='0'))
		{
			if(p<14)
			{	
				number[p]=n;
				if(number[0]=='0')
					number[0]=0;
				else
				{
					number[p+1]=0;
					p++;
					if(atoi(number)>max)
					{
					//	print("\n");
						number[p-1]=0;
						return atoi(number);
					}
					char ttt[2];
					ttt[0]=n;ttt[1]=0;
					output(buffer, font, x+p, y, ttt, R, G, B);
					SDL_UpdateRect(buffer, 0, 0, 0, 0);
					refresh(buffer, screen);
				}
			}
		}
		if(n==SDLK_RETURN)
			go=0;
		if(n==SDLK_BACKSPACE)
		{
			if(p>0)
			{
				output(buffer, font, x+p, y, " ", R, G, B);
				number[p]=0;
				p--;
				SDL_UpdateRect(buffer, 0, 0, 0, 0);
				refresh(buffer, screen);
			}
			go=1;
		}
		n=0;
	}
	int n=atoi(number);
	if(n<min)
		n=0;
//	print("\n");
	return atoi(number);
}

int getWord(SDL_Surface *buffer, SDL_Surface *screen, Tfont *font, char *word, int x, int y, Uint8 R, Uint8 G, Uint8 B, int max)
{
	int go=1;
	if(max>79)
		max=79;
	int p=0;
	while(go)
	{
		SDL_Delay(2);
		Uint16 n=waitKey(screen);
		int shifted=((SDL_GetModState()&KMOD_LSHIFT)||(SDL_GetModState()&KMOD_RSHIFT));
		if((n<='z')&&(n>='a')&&(shifted))
			n=n-('a'-'A');
		if(n==SDL_QUIT)
			return SDL_QUIT;
		if(isprint(n))
		{
			if(p<max)
			{	
				word[p]=n;
				word[p+1]=0;
				p++;
				char ttt[3];
				ttt[0]=n;ttt[1]=0;
				output(buffer, font, x+p, y, ttt, R, G, B);
				SDL_UpdateRect(buffer, 0, 0, 0, 0);
				refresh(buffer, screen);
			}
		}
		if(n==SDLK_RETURN)
			go=0;
		if(n==SDLK_BACKSPACE)
		{
			if(p>0)
			{
				output(buffer, font, x+p, y, " ", R, G, B);
				word[p]=0;
				p--;
				SDL_UpdateRect(buffer, 0, 0, 0, 0);
				refresh(buffer, screen);
			}
			go=1;	
		}
		n=0;
	}
	return 0;
}

int Tterminal::getNum(int min, int max)
{
	int go=1;
	char number[16];
	int p=0;
	while(go)
	{
		char n=0;
		SDL_Delay(2);
		n=waitKey();
		if(n==SDL_QUIT)
			return SDL_QUIT;
		if((n<='9')&&(n>='0'))
		{
			if(p<14)
			{	
				number[p]=n;
				if(number[0]=='0')
					number[0]=0;
				else
				{
					number[p+1]=0;
					p++;
					if(atoi(number)>max)
					{
					//	print("\n");
						number[p-1]=0;
						return atoi(number);
					}
					char ttt[2];
					ttt[0]=n;ttt[1]=0;
					print(ttt);
					blit();
				}
			}
		}
		if(n==SDLK_RETURN)
			go=0;
		if(n==SDLK_BACKSPACE)
		{
			if(p>0)
			{
				number[p]=0;
				p--;
			}
			go=1;	
		}
		n=0;
	}
	int n=atoi(number);
	if(n<min)
		n=0;
//	print("\n");
	return atoi(number);
}

int Tterminal::getWord(char *word, int max)
{
	int go=1;
	int p=0;
	while(go)
	{
		char n=waitKey();
		if(n==SDL_QUIT)
			return SDL_QUIT;
		if((n>=' ')&&(n<127))
		{
			if(p<max)
			{
				word[p]=n;
				p++;
				char ttt[2];
				ttt[0]=word[p];ttt[1]=0;
				print(ttt);
				blit();
			}
		}
		if(n==SDLK_BACKSPACE)
		{
			if(p>0)
			{
//				output(screen, font, x+p, y, " ", R, G, B);
				word[p]=0;
				p--;
			}
			go=1;	
		}
		if(n==SDLK_RETURN)
			go=0;
	}
//	print("\n");
	return 0;
}

Tterminal::~Tterminal()
{
	SDL_Rect rect;
	rect.x=xpos;
	rect.y=ypos;
	rect.w=background->w;
	rect.h=background->h;
	if(SDL_BlitSurface(background, NULL, screen, &rect))
		printf("Error in blit\n");

	SDL_FreeSurface(background);
	delete(font);
	return;
}

int Tterminal::output(SDL_Surface *scrn, int x, int y, char *text, Uint32 R, Uint32 G, Uint32 B)
{
	SDL_Surface *buffer=font->print(text, R, G, B);
	SDL_Rect r;
	r.h=font->chr[0]->h;
	r.w=font->chr[0]->w;
	r.x=x*font->chr[0]->w;
	r.y=y*font->chr[0]->h;
	if(SDL_BlitSurface(buffer, NULL, scrn, &r))
		printf("Error with text blit.\n");
	SDL_FreeSurface(buffer);
	return 0;
}

int TERMINAL_NUMCHARS(char *text)
{
	int dx=0;
	while(text[dx]!=0)
	{
		if(text[dx]==0x03)
			dx+=4;
		else
			dx++;
	}
	return dx;
}

int TERMINAL_NUMPCHARS(char *text)
{
	int x=0;
	int dx=0;
	while(text[dx]!=0)
	{
		if(text[dx]==0x03)
			dx+=4;
		else
		{
			dx++;
			x++;
		}
	}
	return x;
}

int TERMINAL_STRCPY(char *dest, char *source)
{
	int x=0;
	while(source[x]!=0)
	{
		if(source[x]==0x03)
		{
			dest[x]=source[x];	//0x03
			x++;
			dest[x]=source[x];	//R
			x++;
			dest[x]=source[x];	//G
			x++;
			dest[x]=source[x];	//B			
			x++;
		}
		else
		{
			dest[x]=source[x];
			x++;
		}
	}
	dest[x]=0;
	return 0;
}

int TERMINAL_SCROLLDOWN(char str[25][80], int lines)
{
	for(int y=1;y<lines;y++)
		TERMINAL_STRCPY(str[y-1], str[y]);
	str[lines-1][0]=0;
	return 0;
}

int TERMINAL_APPEND(char *dest, char *source, int max)
{
	int x=0;
	int dx=TERMINAL_NUMCHARS(dest);
	if(TERMINAL_NUMCHARS(source))
	while(source[x]!=0)
	{
		if(source[x]==0x03)
		{
			dest[dx]=source[x];	//0x03
			x++;dx++;
			dest[dx]=source[x];	//R
			x++;dx++;
			dest[dx]=source[x];	//G
			x++;dx++;
			dest[dx]=source[x];	//B			
			x++;dx++;
		}
		else
		{
			dest[dx]=source[x];
			x++;dx++;
		}
	}
	dest[dx]=0;
	return 0;
}

int Tterminal::print(char *text)
{
	TERMINAL_APPEND(line[yy], text, columns);
	if((line[yy][TERMINAL_NUMCHARS(line[yy])-1]=='\n')||(TERMINAL_NUMPCHARS(line[yy])>=columns))
	{
		yy++;
		xx=0;
		if(yy>=lines)
		{
			TERMINAL_SCROLLDOWN(line, lines);
			yy=lines-1;
		}
	}
	return 0;
	
}

int Tterminal::dropLine()
{
	line[yy][0]=0;
	return 0;
}

int Tterminal::blit()
{
	SDL_Rect rect;
	rect.x=xpos;
	rect.y=ypos;
	rect.w=background->w;
	rect.h=background->h;
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif
	SDL_Rect bufferrect;
	bufferrect.x=0;bufferrect.w=rect.w;
	bufferrect.h=rect.w;bufferrect.y=0;
	SDL_Surface *buffer=SDL_CreateRGBSurface(SDL_SWSURFACE, background->w, background->h, 32, rmask, gmask, bmask, amask);
	if(font!=NULL)
	for(int yvar=0;yvar<lines;yvar++)
	{
		output(buffer, 0, yvar, line[yvar], dR, dG, dB);
	}
	int X=TERMINAL_NUMPCHARS(line[yy]);
	char cursor[2];cursor[1]=0;cursor[0]=cursorframe;
	output(buffer, X, yy, cursor, dR, dG, dB);
	cursorframe++;
	if(cursorframe>=(startframe+numcursorframes))
		cursorframe=startframe;
	if(SDL_BlitSurface(background, NULL, screen, &rect))
		printf("Error in blit\n");
	if(SDL_BlitSurface(buffer, NULL, screen, &rect))
		printf("Error in blit\n");
	SDL_UpdateRect(screen, rect.x, rect.y, rect.w, rect.h);
	SDL_FreeSurface(buffer);
	if(screen && SCREEN)
		refresh(screen, SCREEN);
	return 0;
	
}

Uint16 swapInt16Endian(Uint16 n)
{
	return (n<<8)+(n>>8);
}

int clearMouseEvents(SDL_Event *event)
{
	int oldx=0;
	int oldy=0; 
	SDL_MouseMotionEvent *e = (SDL_MouseMotionEvent*)event;
	while(event->type==SDL_MOUSEMOTION)
	{
//		printf("blah\n");
		SDL_PollEvent(event);
		if((oldx==e->x) && (oldy==e->y))
			break;
		oldx=e->x;
		oldy=e->y;
	}
//	delete(event);
//	event=new(SDL_Event);
//	SDL_PollEvent(event);
	return 0;
}

int getKey(SDL_Surface *screen, SDL_Event *event)
{
	int val=0;
	SDL_PollEvent(event);
	clearMouseEvents(event);
	if(event->type == SDL_KEYDOWN)
		val=event->key.keysym.sym;
	if(checkSpecialEvents(screen, event)==SDL_QUIT)
		return SDL_QUIT;
	return  val;
}

int getKey(SDL_Surface *screen)
{
	SDL_Event event;
	return getKey(screen, &event);
}

int waitKey(SDL_Surface *screen)
{
	int c=0;
	while(c==0)
	{
		SDL_Delay(50);
		c=getKey(screen);
	}
	return c;
}

int waitKey(SDL_Surface *screen, int maxwait)
{
	int c=0;
	int i=maxwait/25;
	for(int x=0;x<i;x++)
	{
		SDL_Delay(25);
		c=getKey(screen);
		if(c)
			break;
	}
	return c;
}


void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 A)
{
	Uint32 color = SDL_MapRGBA(screen->format, R, G, B, A);

	if ( SDL_MUSTLOCK(screen) ) {
		if ( SDL_LockSurface(screen) < 0 ) {
			return;
		}
	}
	switch (screen->format->BytesPerPixel) {
		case 1: { /* Assuming 8-bpp */
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
			*bufp = color;
		}
		break;

		case 2: { /* Probably 15-bpp or 16-bpp */
			Uint16 *bufp;
			bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
			*bufp = color;
		}
		break;

		case 3: { /* Slow 24-bpp mode, usually not used */
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
				bufp[0] = color;
				bufp[1] = color >> 8;
				bufp[2] = color >> 16;
			} else {
				bufp[2] = color;
				bufp[1] = color >> 8;
				bufp[0] = color >> 16;
			}
		}
		break;

		case 4: { /* Probably 32-bpp */
			Uint32 *bufp;
			bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
			*bufp = color;
		}
		break;
	}
	if ( SDL_MUSTLOCK(screen) ) {
		SDL_UnlockSurface(screen);
	}
	SDL_UpdateRect(screen, x, y, 1, 1);
}


void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
	Uint32 color = SDL_MapRGBA(screen->format, R, G, B, 0xFF);

	if ( SDL_MUSTLOCK(screen) ) {
		if ( SDL_LockSurface(screen) < 0 ) {
			return;
		}
	}
	switch (screen->format->BytesPerPixel) {
		case 1: { /* Assuming 8-bpp */
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
			*bufp = color;
		}
		break;

		case 2: { /* Probably 15-bpp or 16-bpp */
			Uint16 *bufp;
			bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
			*bufp = color;
		}
		break;

		case 3: { /* Slow 24-bpp mode, usually not used */
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
				bufp[0] = color;
				bufp[1] = color >> 8;
				bufp[2] = color >> 16;
			} else {
				bufp[2] = color;
				bufp[1] = color >> 8;
				bufp[0] = color >> 16;
			}
		}
		break;

		case 4: { /* Probably 32-bpp */
			Uint32 *bufp;
			bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
			*bufp = color;
		}
		break;
	}
	if ( SDL_MUSTLOCK(screen) ) {
		SDL_UnlockSurface(screen);
	}
	SDL_UpdateRect(screen, x, y, 1, 1);
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;

		default:
			return 0;   /* shouldn't happen, but avoids warnings */
	}
	return 0;
}

int getAvgColor(SDL_Surface *s, Uint8 *R, Uint8 *G, Uint8 *B)
{
	Uint32 r=0;
	Uint32 g=0;
	Uint32 b=0;
	if ( SDL_MUSTLOCK(s) )
		if ( SDL_LockSurface(s) < 0 )
			return 1;
	for(int x=0;x<s->w;x++)
		for(int y=0;y<s->h;y++)
		{
			Uint32 C=getpixel(s, x, y);
			Uint8 rr=0;
			Uint8 gg=0;
			Uint8 bb=0;
			SDL_GetRGB(C, s->format, &rr, &gg, &bb);
			r+=rr;
			g+=gg;
			b+=bb;
		}
	if ( SDL_MUSTLOCK(s) ) {
		SDL_UnlockSurface(s);
	}
	*R=r/(s->w*s->h);
	*G=g/(s->w*s->h);
	*B=b/(s->w*s->h);
	return 0;
}

int SDL_addAlpha(SDL_Surface *mask, SDL_Surface *s)
{
	Uint8 rr=0;
	Uint8 gg=0;
	Uint8 bb=0;
	Uint8 aa=0;
	Uint8 rm=0;
	Uint8 gm=0;
	Uint8 bm=0;
	Uint8 am=0;
	if ( SDL_MUSTLOCK(s) )
		if ( SDL_LockSurface(s) < 0 )
			return 1;
	for(int x=0;x<s->w;x++)
		for(int y=0;y<s->h;y++)
		{
			Uint32 C=getpixel(s, x, y);
			Uint32 M=getpixel(mask, x, y);
			SDL_GetRGBA(C, s->format, &rr, &gg, &bb, &aa);
			SDL_GetRGBA(M, mask->format, &rm, &gm, &bm, &am);
			DrawPixel(s, x, y, rr, gg, bb, (rm+gm+bm)/3);
		}
	if ( SDL_MUSTLOCK(s) ) {
		SDL_UnlockSurface(s);
	}
	return 0;
}

int setEGAcolors(Tcolor ega[16])
{
	ega[0].R=0;	//ok
	ega[0].G=0;
	ega[0].B=0;
	
	ega[1].R=0;	//ok
	ega[1].G=0;
	ega[1].B=172;
	
	ega[2].R=0;	//ok
	ega[2].G=172;
	ega[2].B=0;
	
	ega[3].R=0;	//ok
	ega[3].G=172;
	ega[3].B=172;
	
	ega[4].R=172;	//ok
	ega[4].G=0;
	ega[4].B=0;
	
	ega[5].R=172;	//ok
	ega[5].G=0;
	ega[5].B=172;
	
	ega[6].R=172;	//ok
	ega[6].G=87;
	ega[6].B=0;
	
	ega[7].R=172;	//ok
	ega[7].G=172;
	ega[7].B=172;
	
	ega[8].R=87;	//ok
	ega[8].G=87;
	ega[8].B=87;
	
	ega[9].R=87;	//ok
	ega[9].G=87;
	ega[9].B=255;
	
	ega[10].R=87;	//ok
	ega[10].G=255;
	ega[10].B=87;
	
	ega[11].R=87;	//ok
	ega[11].G=255;
	ega[11].B=255;
	
	ega[12].R=255;	//ok
	ega[12].G=87;
	ega[12].B=87;
	
	ega[13].R=255;	//ok
	ega[13].G=87;
	ega[13].B=255;
	
	ega[14].R=255;	//ok
	ega[14].G=255;
	ega[14].B=87;
	
	ega[15].R=255;	//ok
	ega[15].G=255;
	ega[15].B=255;
	return 0;
}

int setCGAcolors0(Tcolor cga[4])
{
	cga[0].R=0;	//ok
	cga[0].G=0;
	cga[0].B=0;
	
	cga[1].R=0;	//ok
	cga[1].G=0xA0;
	cga[1].B=0xA0;
	
	cga[2].R=0xA0;	//ok
	cga[2].G=0;
	cga[2].B=0xA0;
	
	cga[3].R=0xA0;	//ok
	cga[3].G=0xA0;
	cga[3].B=0xA0;
	return 0;
}

int setCGAcolors1(Tcolor cga[4])
{
	cga[0].R=0;	//ok
	cga[0].G=0;
	cga[0].B=0;
	
	cga[1].R=0;	//ok
	cga[1].G=0xA0;
	cga[1].B=0;
	
	cga[2].R=0xA0;	//ok
	cga[2].G=0;
	cga[2].B=0;
	
	cga[3].R=0xA0;	//ok
	cga[3].G=0xA0;
	cga[3].B=0;
	return 0;

}

int setCGAcolors2(Tcolor cga[4])
{
	cga[0].R=0;	//ok
	cga[0].G=0;
	cga[0].B=0;
	
	cga[1].R=0;	//ok
	cga[1].G=0xA0;
	cga[1].B=0xA0;
	
	cga[2].R=0xA0;	//ok
	cga[2].G=0;
	cga[2].B=0;
	
	cga[3].R=0xA0;	//ok
	cga[3].G=0xA0;
	cga[3].B=0xA0;
	return 0;
}

int setCGAcolors3(Tcolor cga[4])
{
	cga[0].R=0;	//ok
	cga[0].G=0;
	cga[0].B=0;
	
	cga[1].R=0;	//ok
	cga[1].G=0xA0;
	cga[1].B=0xA0;
	
	cga[2].R=0xA0;	//ok
	cga[2].G=0;
	cga[2].B=0xA0;
	
	cga[3].R=0xA0;	//ok
	cga[3].G=0xA0;
	cga[3].B=0xA0;
	return 0;
}

int setCGAcolors(Tcolor cga[4])
{
	setCGAcolors0(cga);
	return 0;
}

int setCGAcompositecolors1(Tcolor cga[16])
{
	cga[0].R=0x00;
	cga[0].G=0x00;
	cga[0].B=0x00;

	cga[1].R=0x00;
	cga[1].G=0x75;
	cga[1].B=0xC8;

	cga[2].R=0x00;
	cga[2].G=0x4D;
	cga[2].B=0xDB;

	cga[3].R=0x00;
	cga[3].G=0x62;
	cga[3].B=0xAE;

	cga[4].R=0xA6;
	cga[4].G=0x3A;
	cga[4].B=0x00;

	cga[5].R=0x7C;
	cga[5].G=0xC3;
	cga[5].B=0x5F;

	cga[6].R=0x9B;
	cga[6].G=0x96;
	cga[6].B=0x71;

	cga[7].R=0x83;
	cga[7].G=0xAF;
	cga[7].B=0x45;

	cga[8].R=0xE3;
	cga[8].G=0x2B;
	cga[8].B=0x00;

	cga[9].R=0xB9;
	cga[9].G=0xB4;
	cga[9].B=0x90;

	cga[10].R=0xD8;
	cga[10].G=0x87;
	cga[10].B=0xA2;

	cga[11].R=0xC0;
	cga[11].G=0xA1;
	cga[11].B=0x75;

	cga[12].R=0xC5;
	cga[12].G=0x3F;
	cga[12].B=0x00;

	cga[13].R=0x9B;
	cga[13].G=0xC8;
	cga[13].B=0x5D;

	cga[14].R=0xBA;
	cga[14].G=0x9B;
	cga[14].B=0x70;

	cga[15].R=0xA2;
	cga[15].G=0xB5;
	cga[15].B=0x43;

	return 0;
}

int setCGAcompositecolors0(Tcolor cga[16])
{
	cga[0].R=0x00;
	cga[0].G=0x00;
	cga[0].B=0x00;

	cga[1].R=0x00;
	cga[1].G=0x7E;
	cga[1].B=0xF5;

	cga[2].R=0x00;
	cga[2].G=0x51;
	cga[2].B=0xF5;

	cga[3].R=0x00;
	cga[3].G=0xA0;
	cga[3].B=0xFF;

	cga[4].R=0x93;
	cga[4].G=0x2C;
	cga[4].B=0x00;

	cga[5].R=0x6D;
	cga[5].G=0xBD;
	cga[5].B=0xA2;

	cga[6].R=0x8B;
	cga[6].G=0x90;
	cga[6].B=0xB5;

	cga[7].R=0x6E;
	cga[7].G=0xE0;
	cga[7].B=0xFF;

	cga[8].R=0xD0;
	cga[8].G=0x1E;
	cga[8].B=0x00;

	cga[9].R=0xA9;
	cga[9].G=0xAF;
	cga[9].B=0xD3;

	cga[10].R=0xC8;
	cga[10].G=0x82;
	cga[10].B=0xE6;

	cga[11].R=0xAA;
	cga[11].G=0xD1;
	cga[11].B=0xFF;

	cga[12].R=0xFF;
	cga[12].G=0x4C;
	cga[12].B=0x00;

	cga[13].R=0xFF;
	cga[13].G=0xDE;
	cga[13].B=0x8B;

	cga[14].R=0xFF;
	cga[14].G=0xB1;
	cga[14].B=0x9E;

	cga[15].R=0xFF;
	cga[15].G=0xFF;
	cga[15].B=0xFF;

	return 0;
}

int setCGAcompositecolors2(Tcolor cga[16])
{
	cga[0].R=0x00;
	cga[0].G=0x00;
	cga[0].B=0x00;

	cga[1].R=0x00;
	cga[1].G=0x87;
	cga[1].B=0x0F;

	cga[2].R=0x1E;
	cga[2].G=0x05;
	cga[2].B=0xFF;

	cga[3].R=0x00;
	cga[3].G=0xA0;
	cga[3].B=0xFF;

	cga[4].R=0xCD;
	cga[4].G=0x00;
	cga[4].B=0x53;

	cga[5].R=0x76;
	cga[5].G=0x76;
	cga[5].B=0x76;

	cga[6].R=0xFF;
	cga[6].G=0x00;
	cga[6].B=0xFF;

	cga[7].R=0xA8;
	cga[7].G=0x8F;
	cga[7].B=0xFF;

	cga[8].R=0x44;
	cga[8].G=0x5E;
	cga[8].B=0x00;

	cga[9].R=0x00;
	cga[9].G=0xF8;
	cga[9].B=0x00;

	cga[10].R=0x76;
	cga[10].G=0x76;
	cga[10].B=0x76;

	cga[11].R=0x1F;
	cga[11].G=0xFF;
	cga[11].B=0x99;

	cga[12].R=0xFF;
	cga[12].G=0x4C;
	cga[12].B=0x00;

	cga[13].R=0xCE;
	cga[13].G=0xE7;
	cga[13].B=0x00;

	cga[14].R=0xFF;
	cga[14].G=0x65;
	cga[14].B=0xDD;

	cga[15].R=0xFF;
	cga[15].G=0xFF;
	cga[15].B=0xFF;

	return 0;
}

int setCGAcompositecolors3(Tcolor cga[16])
{
	cga[0].R=0x00;
	cga[0].G=0x00;
	cga[0].B=0x00;

	cga[1].R=0x00;
	cga[1].G=0x75;
	cga[1].B=0xC8;

	cga[2].R=0x00;
	cga[2].G=0x4D;
	cga[2].B=0xDB;

	cga[3].R=0x00;
	cga[3].G=0x62;
	cga[3].B=0xAE;

	cga[4].R=0xA6;
	cga[4].G=0x3A;
	cga[4].B=0x00;

	cga[5].R=0x7C;
	cga[5].G=0xC3;
	cga[5].B=0x5F;

	cga[6].R=0x9B;
	cga[6].G=0x96;
	cga[6].B=0x71;

	cga[7].R=0x83;
	cga[7].G=0xAF;
	cga[7].B=0x45;

	cga[8].R=0xE3;
	cga[8].G=0x2B;
	cga[8].B=0x00;

	cga[9].R=0xB9;
	cga[9].G=0xB4;
	cga[9].B=0x90;

	cga[10].R=0xD8;
	cga[10].G=0x87;
	cga[10].B=0xA2;

	cga[11].R=0xC0;
	cga[11].G=0xA1;
	cga[11].B=0x75;

	cga[12].R=0xC5;
	cga[12].G=0x3F;
	cga[12].B=0x00;

	cga[13].R=0x9B;
	cga[13].G=0xC8;
	cga[13].B=0x5D;

	cga[14].R=0xBA;
	cga[14].G=0x9B;
	cga[14].B=0x70;

	cga[15].R=0xA2;
	cga[15].G=0xB5;
	cga[15].B=0x43;

	return 0;
}

int setCGAcompositecolors(Tcolor cga[16])
{
	return setCGAcompositecolors0(cga);
}

int setVGAcolors(Tcolor vga[256])
{
	setEGAcolors(vga);
	for(int x=16;x<256;x++)
	{
		vga[x].R=x;
		vga[x].G=x;
		vga[x].B=x;
	}
	return 0;	
}

int CGAtoComposite(SDL_Surface *surface, int pal)
{
	Tcolor cga[4];
	Tcolor comp[16];
	switch(pal)
	{
		case 0:	setCGAcolors0(cga);
						setCGAcompositecolors0(comp);
						break;
		case 1:	setCGAcolors1(cga);
						setCGAcompositecolors1(comp);
						break;
		case 2:	setCGAcolors2(cga);
						setCGAcompositecolors2(comp);
						break;
		case 3:	setCGAcolors3(cga);
						setCGAcompositecolors3(comp);
						break;	
		default:	setCGAcolors0(cga);
						setCGAcompositecolors0(comp);
						break;
	}
	for(int y=0;y<surface->h;y++)
	{
		for(int x=0;x<surface->w;x+=2)
		{
			Uint32 col1=getpixel(surface, x, y);
			Uint32 col2=getpixel(surface, x+1, y);
			Uint8 R1, R2, G1, G2, B1, B2;
			SDL_GetRGB(col1, surface->format, &R1, &G1, &B1);
			SDL_GetRGB(col2, surface->format, &R2, &G2, &B2);
			int v1=0;
			for(int z=0;z<4;z++)
				if((R1==cga[z].R)&&(G1==cga[z].G)&&(B1==cga[z].B))
					v1=z;
			int v2=0;
			for(int z=0;z<4;z++)
				if((R2==cga[z].R)&&(G2==cga[z].G)&&(B2==cga[z].B))
					v2=z;
			int i=v2+4*v1;
			DrawPixel(surface, x, y, comp[i].R, comp[i].G, comp[i].B);
			DrawPixel(surface, x+1, y, comp[i].R, comp[i].G, comp[i].B);
		}
	}

	return 0;
}

