#include "globaltypes.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

TglobalConfig::TglobalConfig()
{
	load();
}

TglobalConfig::~TglobalConfig()
{
	save();
}

int TglobalConfig::load()
{
	xres=640;
	yres=400;
	splashmusic=1;
	strcpy(datapath, "data/");
	strcpy(pixmaps, "pixmaps/");
	for(int x=0;x<7;x++)
	{
		sprintf(game[x].gamepath, "games/u%d/", x);
		game[x].music=1;
		game[x].sfx=1;
		game[x].svga=1;
	}
	FILE *in=fopen("etc/ucr.conf", "rb");
	if(in==NULL)
	{	
	  char temp[256];
	  sprintf(temp, "%s/.ucr/ucr.conf", getenv("HOME"));
	  in=fopen(temp, "rb");
	}
	if(in==NULL)
		in=fopen("/etc/ucr/ucr.conf", "rb");
	if(in==NULL)
	{
		printf("Cannot open the config file!\n");
		return 1;
	}
	char stringbuf[512];
	char c=0;
	int pos=0;
	int linenum=0;
	while(!feof(in))
	{
		linenum++;
		while((c!='\n')&&(c!=13))	//take care of buffer overflow, unix and windows EOL
		{
			fread(&c, 1, 1, in);
			if(pos<511)
				stringbuf[pos]=c;
			pos++;
			if(feof(in))
				break;
		}
		if(pos>=511)
			printf("WARNING: config line %d is too long!\n", linenum);

		stringbuf[pos]=0;
		if(stringbuf[0]!='#')
		{
			pos=0;
			int pos2=0;
			char var[511];
			char data[511];
			while((stringbuf[pos]!='=')&&(stringbuf[pos]!=0))
			{
				if(isalnum(stringbuf[pos]))
				{
					var[pos2]=stringbuf[pos];
					pos2++;
				}
				pos++;
			}
			var[pos2]=0;
			pos2=0;
			if(stringbuf[pos]!=0)
			{
				pos++;	//skip the = character
				while(stringbuf[pos]!=0)
				{
					if(isgraph(stringbuf[pos]))
					{
						data[pos2]=stringbuf[pos];
						pos2++;
					}
					pos++;
				}
				data[pos2]=0;
			}
			//super-sloppy -- will fix this later.
			if(!strcmp(var, "xres"))
				xres=atoi(data);
			else
			if(!strcmp(var, "yres"))
				yres=atoi(data);
			else
			if(!strcmp(var, "splashmusic"))
				splashmusic=atoi(data);
			else
			if(!strcmp(var, "datapath"))
				strcpy(datapath, data);
			else
			if(!strcmp(var, "pixmaps"))
				strcpy(pixmaps, data);
			else
			if(!strcmp(var, "u1path"))
				strcpy(game[1].gamepath, data);
			else
			if(!strcmp(var, "u2path"))
				strcpy(game[2].gamepath, data);
			else
			if(!strcmp(var, "u3path"))
				strcpy(game[3].gamepath, data);
			else
			if(!strcmp(var, "u4path"))
				strcpy(game[4].gamepath, data);
			else
			if(!strcmp(var, "u5path"))
				strcpy(game[5].gamepath, data);
			else
			if(!strcmp(var, "u6path"))
				strcpy(game[6].gamepath, data);
			else
			if(!strcmp(var, "u0sound"))
				game[0].music=atoi(data);
			else
			if(!strcmp(var, "u0sfx"))
				game[0].sfx=atoi(data);
			else
			if(!strcmp(var, "u0svga"))
				game[0].svga=atoi(data);
			else
			if(!strcmp(var, "u1sound"))
				game[1].music=atoi(data);
			else
			if(!strcmp(var, "u1sfx"))
				game[1].sfx=atoi(data);
			else
			if(!strcmp(var, "u1svga"))
				game[1].svga=atoi(data);
			else
			if(!strcmp(var, "u2sound"))
				game[2].music=atoi(data);
			else
			if(!strcmp(var, "u2sfx"))
				game[2].sfx=atoi(data);
			else
			if(!strcmp(var, "u2svga"))
				game[2].svga=atoi(data);
			else				
			if(!strcmp(var, "u3sound"))
				game[3].music=atoi(data);
			else
			if(!strcmp(var, "u3sfx"))
				game[3].sfx=atoi(data);
			else
			if(!strcmp(var, "u3svga"))
				game[3].svga=atoi(data);
			else
			if(!strcmp(var, "u4sound"))
				game[4].music=atoi(data);
			else
			if(!strcmp(var, "u4sfx"))
				game[4].sfx=atoi(data);
			else
			if(!strcmp(var, "u4svga"))
				game[4].svga=atoi(data);
			else
			if(!strcmp(var, "u5sound"))
				game[5].music=atoi(data);
			else
			if(!strcmp(var, "u5sfx"))
				game[5].sfx=atoi(data);
			else
			if(!strcmp(var, "u5svga"))
				game[5].svga=atoi(data);
			else
			if(!strcmp(var, "u6sound"))
				game[6].music=atoi(data);
			else
			if(!strcmp(var, "u6sfx"))
				game[6].sfx=atoi(data);
			else
			if(!strcmp(var, "u6svga"))
				game[6].svga=atoi(data);
//			printf("LINE %d	:	%s:=%s\n", linenum, var, data);
		}
		pos=0;
		c=0;
		
	}

	return 0;
}

int TglobalConfig::save()
{
	return 0;
}
