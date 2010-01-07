typedef struct
{
	int id;
	int type;//0 - player
	char name[16];
	char nlen;
	char state;
	char wtype;
	short wspeed;
	short rspeed;
	char move;
	int x;
	int y;
	int z;
} object;

typedef struct
{
	int x;
	int y;
	int z;
} point;
