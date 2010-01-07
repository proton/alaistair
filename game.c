//#include <sys/time.h>
#include "game.h"

#define objmax 500

object players[objmax];

void init_pl()
{
	short i;
	for( i=0; i<objmax; ++i) players[i].id = 0;
}

short free_pl()
{
	short i=0;
	while(players[i].id!=0 & i<objmax) ++i;
	if (players[i].id==0) return i;
	else return -1;
}

short id_pl(int id)
{
	short i = 0;
	while(players[i].id!=id && i<objmax) ++i;
	if(players[i].id==id) return i;
	else return -1;
}

void set_pl_stat(int id, int type, char* name, char nlen, short wspeed, short rspeed, int x, int y, int z)
{
	if(id_pl(id)==-1)
	{
		short i = free_pl();
		if(i!=-1)
		{
			players[i].id = id;
			players[i].type = type;
			memcpy(players[i].name,name,nlen);
			players[i].nlen = nlen;
			players[i].state = 1;
			players[i].wtype = 1;
			players[i].wspeed = wspeed;
			players[i].rspeed = rspeed;
			players[i].move = 0;
			players[i].x = x;
			players[i].y = y;
			players[i].z = z;
		}
		else printf("[X] Достигнуто максимальное количество объектов, пожалуйста увеличте параметр objmax!");
	}
	else
	{
		
	}
}

void set_pl_xyz(int id, int x, int y, int z)
{
	short i = id_pl(id);
	if(i!=-1)
	{
		players[i].x = x;
		players[i].y = y;
		players[i].z = z;
	}
}

void del_pl(int id)
{
	short i=id_pl(id);
	if(i!=-1) players[i].id = 0;
}
