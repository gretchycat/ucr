#include <SDL.h>
#include <SDL_image.h>
#include "../globaltypes.h"
#include "u0types.h"

int u0loadimages(U0_player *p)
{
  char temp[512];
  sprintf(temp, "%s/u0background.png", p->config.pixmaps);
    p->u0background=IMG_Load(temp);
  sprintf(temp, "%s/door_b.png", p->config.pixmaps);
    p->door_b=IMG_Load(temp);
  sprintf(temp, "%s/door_l.png", p->config.pixmaps);
    p->door_l=IMG_Load(temp);
  sprintf(temp, "%s/door_r.png", p->config.pixmaps);
    p->door_r=IMG_Load(temp);
  sprintf(temp, "%s/wall_b.png", p->config.pixmaps);
    p->wall_b=IMG_Load(temp);
  sprintf(temp, "%s/u0rightwall.png", p->config.pixmaps);
    p->u0rightwall=IMG_Load(temp);
  sprintf(temp, "%s/u0leftwall.png", p->config.pixmaps);
    p->u0leftwall=IMG_Load(temp);
  sprintf(temp, "%s/u0dngback.png", p->config.pixmaps);
    p->u0dngback=IMG_Load(temp);
  sprintf(temp, "%s/u0trapdoor.png", p->config.pixmaps);
    p->u0trapdoor=IMG_Load(temp);
  sprintf(temp, "%s/u0tophole.png", p->config.pixmaps);
    p->u0tophole=IMG_Load(temp);
  sprintf(temp, "%s/u0ladder.png", p->config.pixmaps);
    p->u0ladder=IMG_Load(temp);
  sprintf(temp, "%s/u0title.png", p->config.pixmaps);
    p->u0title=IMG_Load(temp);
  sprintf(temp, "%s/u0giantrat.png", p->config.pixmaps);
    p->u0giantrat=IMG_Load(temp);
  sprintf(temp, "%s/u0chest.png", p->config.pixmaps);
    p->u0chest=IMG_Load(temp);
  sprintf(temp, "%s/u0mimic.png", p->config.pixmaps);
    p->u0mimic=IMG_Load(temp);
  sprintf(temp, "%s/u0skeleton.png", p->config.pixmaps);
    p->u0skeleton=IMG_Load(temp);
  sprintf(temp, "%s/u0gremlin.png", p->config.pixmaps);
    p->u0gremlin=IMG_Load(temp);
  sprintf(temp, "%s/u0thief.png", p->config.pixmaps);
    p->u0thief=IMG_Load(temp);
  sprintf(temp, "%s/u0orc.png", p->config.pixmaps);
    p->u0orc=IMG_Load(temp);
  sprintf(temp, "%s/u0viper.png", p->config.pixmaps);
    p->u0viper=IMG_Load(temp);
  sprintf(temp, "%s/u0daemon.png", p->config.pixmaps);
    p->u0daemon=IMG_Load(temp);
  sprintf(temp, "%s/u0crawler.png", p->config.pixmaps);
    p->u0crawler=IMG_Load(temp);
  sprintf(temp, "%s/u0balrog.png", p->config.pixmaps);
    p->u0balrog=IMG_Load(temp);
  sprintf(temp, "%s/u0tree.png", p->config.pixmaps);
    p->u0tree=IMG_Load(temp);
  sprintf(temp, "%s/u0dungeon.png", p->config.pixmaps);
    p->u0dungeon=IMG_Load(temp);
  sprintf(temp, "%s/u0village.png", p->config.pixmaps);
    p->u0village=IMG_Load(temp);
  sprintf(temp, "%s/u0castle.png", p->config.pixmaps);
    p->u0castle=IMG_Load(temp);
  sprintf(temp, "%s/u0mountain.png", p->config.pixmaps);
    p->u0mountain=IMG_Load(temp);
  sprintf(temp, "%s/u0grass.png", p->config.pixmaps);
    p->u0grass=IMG_Load(temp);
  sprintf(temp, "%s/u0walku1.png", p->config.pixmaps);
    p->u0walku1=IMG_Load(temp);
  sprintf(temp, "%s/u0walku2.png", p->config.pixmaps);
    p->u0walku2=IMG_Load(temp);
  sprintf(temp, "%s/u0walku3.png", p->config.pixmaps);
    p->u0walku3=IMG_Load(temp);
  sprintf(temp, "%s/u0walkd1.png", p->config.pixmaps);
    p->u0walkd1=IMG_Load(temp);
  sprintf(temp, "%s/u0walkd2.png", p->config.pixmaps);
    p->u0walkd2=IMG_Load(temp);
  sprintf(temp, "%s/u0walkd3.png", p->config.pixmaps);
    p->u0walkd3=IMG_Load(temp);
  sprintf(temp, "%s/u0walkl1.png", p->config.pixmaps);
    p->u0walkl1=IMG_Load(temp);
  sprintf(temp, "%s/u0walkl2.png", p->config.pixmaps);
    p->u0walkl2=IMG_Load(temp);
  sprintf(temp, "%s/u0walkl3.png", p->config.pixmaps);
    p->u0walkl3=IMG_Load(temp);
  sprintf(temp, "%s/u0walkr1.png", p->config.pixmaps);
    p->u0walkr1=IMG_Load(temp);
  sprintf(temp, "%s/u0walkr2.png", p->config.pixmaps);
    p->u0walkr2=IMG_Load(temp);
  sprintf(temp, "%s/u0walkr3.png", p->config.pixmaps);
    p->u0walkr3=IMG_Load(temp);
  return 0;
}

int u0unloadimages(U0_player *p)
{
  if(p->u0background)
    SDL_FreeSurface(p->u0background);
  if(p->door_b)
    SDL_FreeSurface(p->door_b);
  if(p->door_l)
    SDL_FreeSurface(p->door_l);
  if(p->door_r)
    SDL_FreeSurface(p->door_r);
  if(p->wall_b)
    SDL_FreeSurface(p->wall_b);
  if(p->u0rightwall)
    SDL_FreeSurface(p->u0rightwall);
  if(p->u0leftwall)
    SDL_FreeSurface(p->u0leftwall);
  if(p->u0dngback)
    SDL_FreeSurface(p->u0dngback);
  if(p->u0trapdoor)
    SDL_FreeSurface(p->u0trapdoor);
  if(p->u0tophole)
    SDL_FreeSurface(p->u0tophole);
  if(p->u0ladder)
    SDL_FreeSurface(p->u0ladder);
  if(p->u0title)
    SDL_FreeSurface(p->u0title);
  if(p->u0giantrat)
    SDL_FreeSurface(p->u0giantrat);
  if(p->u0chest)
    SDL_FreeSurface(p->u0chest);
  if(p->u0mimic)
    SDL_FreeSurface(p->u0mimic);
  if(p->u0skeleton)
    SDL_FreeSurface(p->u0skeleton);
  if(p->u0gremlin)
    SDL_FreeSurface(p->u0gremlin);
  if(p->u0thief)
    SDL_FreeSurface(p->u0thief);
  if(p->u0orc)
    SDL_FreeSurface(p->u0orc);
  if(p->u0viper)
    SDL_FreeSurface(p->u0viper);
  if(p->u0daemon)
    SDL_FreeSurface(p->u0daemon);
  if(p->u0crawler)
    SDL_FreeSurface(p->u0crawler);
  if(p->u0balrog)
    SDL_FreeSurface(p->u0balrog);
  if(p->u0tree)
    SDL_FreeSurface(p->u0tree);
  if(p->u0dungeon)
    SDL_FreeSurface(p->u0dungeon);
  if(p->u0village)
    SDL_FreeSurface(p->u0village);
  if(p->u0castle)
    SDL_FreeSurface(p->u0castle);
  if(p->u0mountain)
    SDL_FreeSurface(p->u0mountain);
  if(p->u0grass)
    SDL_FreeSurface(p->u0grass);
  if(p->u0walku1)
    SDL_FreeSurface(p->u0walku1);
  if(p->u0walku2)
    SDL_FreeSurface(p->u0walku2);
  if(p->u0walku3)
    SDL_FreeSurface(p->u0walku3);
  if(p->u0walkd1)
    SDL_FreeSurface(p->u0walkd1);
  if(p->u0walkd2)
    SDL_FreeSurface(p->u0walkd2);
  if(p->u0walkd3)
    SDL_FreeSurface(p->u0walkd3);
  if(p->u0walkl1)
    SDL_FreeSurface(p->u0walkl1);
  if(p->u0walkl2)
    SDL_FreeSurface(p->u0walkl2);
  if(p->u0walkl3)
    SDL_FreeSurface(p->u0walkl3);
  if(p->u0walkr1)
    SDL_FreeSurface(p->u0walkr1);
  if(p->u0walkr2)
    SDL_FreeSurface(p->u0walkr2);
  if(p->u0walkr3)
    SDL_FreeSurface(p->u0walkr3);
  return 0;  
}
