#include <stdio.h>   
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_endian.h>
#include "../SDL_rotozoom.h"
#include "../lzw.h"
#include "../globaltypes.h"
#include "../globalfunctions.h"
#include "u4types.h"
#include "u4data.h"

long getFilesize(FILE *input_file)
{   
	long file_length;
	fseek(input_file, 0, SEEK_END);   /* move file pointer to file end */
	file_length = ftell(input_file);
	fseek(input_file, 0, SEEK_SET);   /* move file pointer to file start */
	return(file_length);
}
 
unsigned char mightBeValidCompressedFile(FILE *input_file)
{
	unsigned char firstByte;
	unsigned char c1, c2, c3; /* booleans */
	long input_filesize;

	/*check if the input file has a valid size */
	/*the compressed file is made up of 12-bit codewords,*/
	/*so there are either 0 or 4 bits of wasted space*/
	input_filesize = getFilesize(input_file);
	c1 = (input_filesize * 8) % 12 == 0;
	c2 = (input_filesize * 8 - 4) % 12 == 0;

	/* read first byte */
	fseek(input_file, 0, SEEK_SET); /* move file pointer to file start */
	fread(&firstByte, 1, 1, input_file);
	fseek(input_file, 0, SEEK_SET); /* move file pointer to file start */
	c3 = (firstByte >> 4) == 0;

	/* check if upper 4 bits are 0 */
	return ((c1 || c2) && c3);
}

int U4_isLZWCompressed(char *filename)
{
	int status=0;
	status=(status || (strcmp(filename, "title.ega")==0));
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

int U4_EGAImage(TglobalConfig *conf, SDL_Surface *screen, char *filename, int xx, int yy)
{
	FILE *in;
	Tcolor ega[16];
	setEGAcolors(ega);
	char tempfn[512];
	sprintf(tempfn, "%s/%s", conf->game[4].gamepath, filename);
	if(U4_isLZWCompressed(filename))
	{
		FILE *compressed_file=NULL;
		unsigned char *compressed_mem=NULL;
		unsigned char *decompressed_mem=NULL;
		long compressed_filesize=0;
		long decompressed_filesize=0;
		long errorCode=0;
		if ((compressed_file=fopen(tempfn,"rb"))==NULL)
		{
			printf("cannot open %s\n", filename);
			return -1;
		}
		compressed_filesize = getFilesize(compressed_file);
		if (compressed_filesize == 0)
		{
			printf("Input file has a length of 0 bytes.\n");
			return -1;
		}
		if (!mightBeValidCompressedFile(compressed_file))
		{
			printf("The input file is not a valid LZW-compressed file.\n");
			fclose(compressed_file);
			return -1;
		}
		compressed_mem = (unsigned char *) malloc(compressed_filesize);
		fread(compressed_mem, 1, compressed_filesize, compressed_file);
		fclose(compressed_file);
		decompressed_filesize = lzwGetDecompressedSize(compressed_mem,compressed_filesize);
		decompressed_mem = (unsigned char *) malloc(decompressed_filesize);
		for (int i = 0; i < decompressed_filesize; i++)
			decompressed_mem[i] = 0;
		errorCode = lzwDecompress(compressed_mem, decompressed_mem, compressed_filesize);
		//generate surface
		for(int y=0;y<yy;y++)
			for(int x=0;x<xx/2;x++)
			{
				long int pos=y*(xx/2)+x;
				if(pos<decompressed_filesize)
				{
					int color=decompressed_mem[pos]/16;
					DrawPixel(screen, x*2, y, ega[color].R, ega[color].G, ega[color].B);
					color=decompressed_mem[pos]%16;
					DrawPixel(screen, x*2+1, y, ega[color].R, ega[color].G, ega[color].B);
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
		return 1;
	}
	return 0;
}
