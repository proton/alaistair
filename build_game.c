#include <stdlib.h> //Для рандома
//14 00
//04
//6B 1B A0 4C
//BF A4 00 00
//BD A0 00 00
//5D F2 FF FF
//00 

void game_build_ping(unsigned char *raw, unsigned char* msg)
{
	const short pc_len=15;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton, pc_len);
	packet_skeleton[2]=0xA8;
	memcpy(packet_skeleton+3, msg, 4);
	int ping=0x00000033;
	memcpy(packet_skeleton+7,(char*)&ping,4);
	ping=0x00001800;
	memcpy(packet_skeleton+11,(char*)&ping,4);
	memcpy(raw, packet_skeleton, pc_len);
}

void game_build_say(unsigned char* raw, unsigned char* msg)
{
	int msglen=2*strlen(msg+1);
	int nlen;
	if(msg[0]==2) nlen=2*strlen(msg+msglen/2+2);
	short pc_len=2+1+msglen+2+4;
	if(msg[0]==2) pc_len=2+1+msglen+2+4+nlen+2;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x38;
	char umsg[msglen];
	acu(msg+1,umsg);
	memcpy(packet_skeleton+3,umsg,msglen);
	packet_skeleton[msglen+3]=0x00;
	packet_skeleton[msglen+4]=0x00;
	int mt = msg[0];
	memcpy(packet_skeleton+msglen+5,(unsigned char*)&mt,4);
	if(msg[0]==2)
	{
		char unick[nlen];
		acu(msg+msglen/2+2,unick);
		memcpy(packet_skeleton+msglen+6+3,unick,nlen);
		packet_skeleton[msglen+nlen+6+3]=0x00;
		packet_skeleton[msglen+nlen+6+4]=0x00;
	}
	memcpy(raw,packet_skeleton,pc_len);
}

void game_build_socact(unsigned char* raw, unsigned char* msg)
{
	int action = msg[0];
	short pc_len=7;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x1B;
	memcpy(packet_skeleton+3,(unsigned char*)&action,4);
	memcpy(raw,packet_skeleton,pc_len);
}

void game_build_wtp(unsigned char* raw, unsigned char* msg)
{
	int state = msg[0];
	short pc_len=7;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x1D;
	memcpy(packet_skeleton+3,(unsigned char*)&state,4);
	memcpy(raw,packet_skeleton,pc_len);
}

void game_build_mtp(unsigned char* raw, unsigned char* msg)
{
	int state = msg[0];
	short pc_len=7;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x1C;
	memcpy(packet_skeleton+3,(unsigned char*)&state,4);
	memcpy(raw,packet_skeleton,pc_len);
}


void game_build_partyans(unsigned char* raw, unsigned char* msg)
{
	int state = msg[0];
	short pc_len=7;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x2A;
	memcpy(packet_skeleton+3,(unsigned char*)&state,4);
	memcpy(raw,packet_skeleton,pc_len);
}

void game_build_skilluse(unsigned char* raw, unsigned char* msg)
{
	short pc_len=15;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x2F;
	memcpy(packet_skeleton+3,msg,4);
	bzero(packet_skeleton+7,8);
	memcpy(raw,packet_skeleton,pc_len);
}

void game_build_partyreq(unsigned char* raw, unsigned char* msg)
{
	int msglen = 2*strlen(msg+1);
	short pc_len=2+1+msglen+2+4;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x29;
	char umsg[msglen];
	acu(msg+1,umsg);
	memcpy(packet_skeleton+3,umsg,msglen);
	packet_skeleton[msglen+3]=0x00;
	packet_skeleton[msglen+4]=0x00;
	int mt = msg[0];
	memcpy(packet_skeleton+msglen+5,(unsigned char*)&mt,4);
	memcpy(raw,packet_skeleton,pc_len);
}

void game_build_friendreq(unsigned char* raw, unsigned char* msg)
{
	int msglen = 2*strlen(msg);
	short pc_len=2+1+msglen+2;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x5E;
	char umsg[msglen];
	acu(msg,umsg);
	memcpy(packet_skeleton+3,umsg,msglen);
	packet_skeleton[msglen+3]=0x00;
	packet_skeleton[msglen+4]=0x00;
	memcpy(raw,packet_skeleton,pc_len);
}

void game_build_move(unsigned char* raw, unsigned char* msg)
{     
	srand(time(NULL));
	short pc_len=31;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x01;
	int i=0;
	i=43504 + 300.0 * (rand() / (RAND_MAX + 1.0))-150; //printf( "X: %d\n", i);
	memcpy(packet_skeleton+3,(char*)&i,4);
	i=40377 + 300.0 * (rand() / (RAND_MAX + 1.0))-150; //printf( "Y: %d\n", i);
	memcpy(packet_skeleton+7,(char*)&i,4);
	//i=100.0 * (rand() / (RAND_MAX + 1.0))-50-3448; //printf( "Z: %d\n", i);
	i=-3448; //printf( "Z: %d\n", i);	
	memcpy(packet_skeleton+11,(char*)&i,4);
	i=0;
	memcpy(packet_skeleton+15,(char*)&i,4);
	memcpy(packet_skeleton+19,(char*)&i,4);
	memcpy(packet_skeleton+23,(char*)&i,4);
	i=1;
	memcpy(packet_skeleton+27,(char*)&i,4);
	memcpy(raw,packet_skeleton,pc_len);
}
