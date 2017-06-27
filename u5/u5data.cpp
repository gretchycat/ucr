#include <stdio.h>   
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_endian.h>
#include "../SDL_rotozoom.h"
#include "u5lzw.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "u5types.h"
#include "u5data.h"


int U5_isLZWCompressed(char *filename)
{
return 1;
	int status=0;
	status=(status || (strcmp(filename, "ultima.16")==0));
	status=(status || (strcmp(filename, "animate.ega")==0));
	status=(status || (strcmp(filename, "tree.ega")==0));
	status=(status || (strcmp(filename, "portal.ega")==0));
	status=(status || (strcmp(filename, "outside.ega")==0));
	status=(status || (strcmp(filename, "inside.ega")==0));
	status=(status || (strcmp(filename, "wagon.ega")==0));
	status=(status || (strcmp(filename, "gypsy.ega")==0));
	status=(status || (strcmp(filename, "abacus.ega")==0));
	status=(status || (strcmp(filename, "honcom.ega")==0));
	status=(status || (strcmp(filename, "valjus.ega")==0));
	status=(status || (strcmp(filename, "sachonor.ega")==0));
	status=(status || (strcmp(filename, "spirhum.ega")==0));
	return status;
}

SDL_Surface *U5_EGAImage(TglobalConfig *conf, char *filename, int framenum)
{
	int xx=0;
	int yy=0;
	FILE *in;
	char tempfn[512];
	sprintf(tempfn, "%s/%s", conf->game[5].gamepath, filename);
	SDL_Surface *screen=NULL;
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
	
	Tcolor ega[16];
	setEGAcolors(ega);
	if(U5_isLZWCompressed(filename))
	{
		FILE *compressed_file=NULL;
		unsigned char *compressed_mem=NULL;
		unsigned char *decompressed_mem=NULL;
		long int compressed_filesize=0;
		long int decompressed_filesize=0;
		if ((compressed_file=fopen(tempfn,"rb"))==NULL)
		{
			printf("cannot open %s\n", filename);
			return NULL;
		}
		compressed_filesize = get_filesize(compressed_file);
		if (compressed_filesize == 0)
		{
			printf("Input file has a length of 0 bytes.\n");
			return NULL;
		}
		unsigned char cc;
		fread(&cc, 1, 1, compressed_file);
		decompressed_filesize = cc;
		fread(&cc, 1, 1, compressed_file);
		decompressed_filesize += cc*256;
		fread(&cc, 1, 1, compressed_file);
		decompressed_filesize += cc*256*256;
		fread(&cc, 1, 1, compressed_file);
		decompressed_filesize += cc*256*256*256*256;
		compressed_mem = (unsigned char *) malloc(compressed_filesize);
		fread(compressed_mem, 1, compressed_filesize, compressed_file);
		fclose(compressed_file);
		decompressed_mem = (unsigned char *) malloc(decompressed_filesize);
		for (int i = 0; i < decompressed_filesize; i++)
			decompressed_mem[i] = 0;
		lzw_decompress(compressed_mem, compressed_filesize, decompressed_mem, decompressed_filesize);
		long int base=0;
		//generate surface
		int go=3;
		long int framepos[20];
		int frames=256*decompressed_mem[1]+decompressed_mem[0];
		for(int x=0;x<frames;x++)
		{
			framepos[x]=256*decompressed_mem[1+x*4+2]+decompressed_mem[0+x*4+2];			
		}
	//	framenum=-1;
		int startval=0;
		int endval=frames;
		if(framenum>frames)
			framenum=frames-1;
		if(framenum>-1)
		{
			startval=framenum;
			endval=framenum+1;
		}
//		printf("%4d\n", framepos[startval]);
//		for(int x=0;x<26;x++)
//			printf("%2x ", decompressed_mem[framepos[startval]+x]);
//		printf("\n"); 


//		for(int z=startval;z<endval;z++)
		int z=startval;
		{
//			printf("z:= %d\n", z);
			base=framepos[z];
			
			xx=256*decompressed_mem[base+1]+decompressed_mem[base]+1;
			yy=256*decompressed_mem[base+3]+decompressed_mem[base+2];
			screen=SDL_CreateRGBSurface(SDL_SWSURFACE, xx, yy, 32, rmask, gmask, bmask, amask);
			base+=4;
//			printf("\n\n--%04x, %04x--\n\n", xx, yy);
			for(int y=0;y<yy;y++)
				for(int x=0;x<xx/2;x++)
				{
					long int pos=y*(xx/2)+x+base;
					if(pos<decompressed_filesize)
					{
						int color=decompressed_mem[pos]/16;
						DrawPixel(screen, x*2, y, ega[color].R, ega[color].G, ega[color].B);
						color=decompressed_mem[pos]%16;
						DrawPixel(screen, x*2+1, y, ega[color].R, ega[color].G, ega[color].B);
					}
					else
					{
						go=0;
						y=yy;
						break;
					}
				}
		}
					
			//done generating surface
		free(decompressed_mem);
	}
	else
	{
	if((in=fopen(tempfn, "rb"))!=NULL)
	{
		int x=0;
		int y=0;
		while(!feof(in))
		{
			unsigned char c;
			fread(&c, 1, 1, in);
			if(c==0x02)
			{
				unsigned char ct;
				unsigned char value;

				fread(&ct, 1, 1, in);
				fread(&value, 1, 1, in);
				for(unsigned char z=0;z<ct;z++)
				{
					x++;
					if(x==xx)
					{
						x=0;
						y++;
					}
					Uint8 color=(value/16);
					DrawPixel(screen, x, y,ega[color].R, ega[color].G, ega[color].B);
					x++;
					if(x==xx)
					{
						x=0;
						y++;
					}
					color=(value%16);
					DrawPixel(screen, x, y,ega[color].R, ega[color].G, ega[color].B);
				}
			}
			else
			{
				x++;
				if(x==xx)
				{
					x=0;
					y++;
				}
				Uint8 color=(c/16);
				DrawPixel(screen, x, y,ega[color].R, ega[color].G, ega[color].B);
				x++;
				if(x==xx)
				{
					x=0;
					y++;
				}
				color=(c%16);
				DrawPixel(screen, x, y,ega[color].R, ega[color].G, ega[color].B);
			}
		}
		fclose(in);
	}
	else
		return screen;
	}
	return screen;
}
