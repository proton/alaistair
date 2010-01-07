#include <openssl/blowfish.h>
#include <stdlib.h>
#include <stdio.h>

BF_KEY bfkey;

int s1,s2;
int m,g,w;

char login[15];
char pwd[17];
char servnum,charnum;
unsigned char key[21];
int protocol,port;
unsigned char address[16];

unsigned char login_key_p1[9];
unsigned char login_key_p2[9];
unsigned char play_key_p1[9];
unsigned char play_key_p2[9];

char key_sc[9];
char key_cs[9];

typedef struct
{
	unsigned char id;
	unsigned char ip[16];
	unsigned int port;
	unsigned int online;
	unsigned int players;
	unsigned char status;
} ServerList;
ServerList servers[256];

unsigned char rbuf[65536];
unsigned char sbuf[65536];
unsigned char gbuf[1000];
unsigned char mbuf[65536];

int pid;
