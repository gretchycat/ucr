#include <stdio.h>   
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_endian.h>
#include "../SDL_rotozoom.h"
#include "../u5/u5lzw.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "u6types.h"
#include "u6data.h"



int U6_VGAImage(TglobalConfig *conf, SDL_Surface *screen, char *filename)
{
	Tcolor vga[256];
	FILE *pal=NULL;
	char tempfn[512];
	sprintf(tempfn, "%s/u6pal", conf->game[6].gamepath);
	pal=fopen(tempfn, "rb");
	if(pal)
	{
		for(int x=0;x<256;x++)
		{
			Uint8 c;
			fread(&c, 1, 1, pal); 
			vga[x].R=c*4;
			fread(&c, 1, 1, pal); 
			vga[x].G=c*4;
			fread(&c, 1, 1, pal); 
			vga[x].B=c*4;
			
		}
		fclose(pal);
	}
	FILE *compressed_file=NULL;
	unsigned char *compressed_mem=NULL;
	unsigned char *decompressed_mem=NULL;
	long int compressed_filesize=0;
	long int decompressed_filesize=0;
	sprintf(tempfn, "%s/%s", conf->game[6].gamepath, filename);
	if ((compressed_file=fopen(tempfn,"rb"))==NULL)
	{
		printf("cannot open %s\n", filename);
		return -1;
	}
	compressed_filesize = get_filesize(compressed_file);
	if (compressed_filesize == 0)
	{
		printf("Input file has a length of 0 bytes.\n");
		return -1;
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
	if(compressed_mem)
		free(compressed_mem);
	//generate surface
	Uint16 xx=decompressed_mem[1]*256+decompressed_mem[0];
	Uint16 yy=decompressed_mem[3]*256+decompressed_mem[2];
	for(int y=0;y<yy;y++)
		for(int x=0;x<xx;x++)
		{
			long int pos=y*(xx)+x+4;
			if(pos<decompressed_filesize)
			{
				int color=decompressed_mem[pos];
				DrawPixel(screen, x, y, vga[color].R, vga[color].G, vga[color].B);
			}
		}
			
	//done generating surface
	if(decompressed_mem)
		free(decompressed_mem);
	return 0;
}
