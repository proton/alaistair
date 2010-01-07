void game_td(unsigned char *raw, unsigned char* sraw, int pid)
{
	//printf("[!] тип полученного пакета (0x%.2X)\n",raw[2]);
	switch(raw[2]) {
		case 0x01:
		game_recv_move(raw);
		break;
		case 0x03:
		game_recv_charinfo(raw);
		break;
		case 0x04:
		game_recv_userinfo(raw);
		break;
		case 0x0E:
		game_recv_statusupd(raw);
		break;
		case 0x12:
		game_recv_objdel(raw);
		break;
		case 0x16:
		game_recv_npcinfo(raw);
		break;
		case 0x2D:
		game_recv_socact(raw);
		break;
		case 0x39:
		game_recv_partyinv(raw);
		break;
		case 0x4A:
		game_recv_say(raw);
		break;
		case 0x61:
		game_recv_validloc(raw);
		break;
		case 0xD3:
		game_recv_ping(raw);
		break;
		default:
		printf("[!] Невозможно определить тип полученного пакета (0x%.2X)\n",raw[2]);
		//print_packet(raw);
		break;
	}
}

void game_recv_validloc(unsigned char* raw)
{
	int l0,l1,l2,l3,l4;
	memcpy((char*)&l0,raw+3,4);
	memcpy((char*)&l1,raw+7,4);
	memcpy((char*)&l2,raw+11,4);
	memcpy((char*)&l3,raw+15,4);
	memcpy((char*)&l4,raw+19,4);
	printf("Validating location... (%d.%d.%d.%d.%d)\n",l0,l1,l2,l3,l4);
	//unsigned char nbuf[5];
	//nbuf[0]=0xA8;
	//memcpy(nbuf+1,raw+3,4);
	//gmtd(nbuf);
}

void game_recv_move(unsigned char* raw)
{
	int id,x0,x1,y0,y1,z0,z1;
	memcpy((char*)&id,raw+3,4);
	memcpy((char*)&x0,raw+7,4);
	memcpy((char*)&y0,raw+11,4);
	memcpy((char*)&z0,raw+15,4);
	memcpy((char*)&x1,raw+19,4);
	memcpy((char*)&y1,raw+23,4);
	memcpy((char*)&z1,raw+27,4);
	set_pl_xyz(id, x1, y1, z1);
	//printf(">> Объект #%d перемещается из (%d.%d.%d) в (%d.%d.%d)\n",id,x0,y0,z0,x1,y1,z1);
	//unsigned char nbuf[5];
	//nbuf[0]=0xA8;
	//memcpy(nbuf+1,raw+3,4);
	//gmtd(nbuf);
}

void game_recv_statusupd(unsigned char* raw)
{
	int id;
	memcpy((char*)&id,raw+3,4);
	//memcpy((char*)&actnum,raw+7,4);
	//printf(">> Объект #%d изменил статус\n",id);
	//print_packet(raw);
}

void game_recv_objdel(unsigned char* raw)
{
	int id;
	memcpy((char*)&id,raw+3,4);
	//printf(">> Объект #%d был удален\n",id);
	del_pl(id);
}

void game_recv_socact(unsigned char* raw)
{
	int id,actnum;
	memcpy((char*)&id,raw+3,4);
	memcpy((char*)&actnum,raw+7,4);
	printf(">> Объект #%d выполнил действие %d\n",id,actnum);
	//unsigned char nbuf[5];
	//nbuf[0]=0xA8;
	//memcpy(nbuf+1,raw+3,4);
	//gmtd(nbuf);
}

void game_recv_ping(unsigned char* raw)
{
	unsigned char nbuf[5];
	nbuf[0]=0xA8;
	memcpy(nbuf+1,raw+3,4);
	gmtd(nbuf);
}

void game_recv_partyinv(unsigned char* raw)
{
	int nlen=0;
	while( raw[3+nlen*2]!=0x00 || raw[4+nlen*2]!=0x00 ) nlen++;
	char nick[nlen];
	uca(raw+3,nick,nlen);
	
	unsigned char messa[nlen+18];
	memcpy(messa,"Party invite from ",18);
	memcpy(messa+18,nick,nlen);
	log_add(messa, nlen+18,'w');
	unsigned char nbuf[2];
	nbuf[0]=0x2A;
	nbuf[1]=1;
	gmtd(nbuf);
}

void game_recv_npcinfo(unsigned char* raw)
{
	int x,y,z,id,rspeed,wspeed,npctype;
	memcpy((char*)&id,raw+3,4);
	memcpy((char*)&npctype,raw+7,4);
	memcpy((char*)&x,raw+15,4);
	memcpy((char*)&y,raw+19,4);
	memcpy((char*)&z,raw+23,4);
	memcpy((char*)&rspeed,raw+43,4);
	memcpy((char*)&wspeed,raw+47,4);
	char nlen=0;
	while( raw[123+nlen*2]!=0x00 || raw[124+nlen*2]!=0x00 ) nlen++;
	char nick[nlen];
	uca(raw+23,nick,nlen);	
	//print_packet(raw);
	//printf(">> Получена информация об объекте (id:%d координаты:(%d.%d.%d) скорость:%d/%d)\n",id,x,y,z,wspeed,rspeed);
	set_pl_stat(id, npctype+1, nick, nlen, wspeed, rspeed, x, y, z);
}

void game_recv_charinfo(unsigned char* raw)
{
	int x,y,z,id,rspeed,wspeed;
	memcpy((char*)&x,raw+3,4);
	memcpy((char*)&y,raw+7,4);
	memcpy((char*)&z,raw+11,4);
	memcpy((char*)&id,raw+19,4);
	char nlen=0;
	while( raw[23+nlen*2]!=0x00 || raw[24+nlen*2]!=0x00 ) nlen++;
	char nick[nlen];
	uca(raw+23,nick,nlen);
	memcpy((char*)&rspeed,raw+105+2*nlen,4);
	memcpy((char*)&wspeed,raw+109+2*nlen,4);
	int tlen=strlen("Detected character  (id:1234567890 (1234567.1234567.1234567))   ")+nlen;
	unsigned char messa[tlen];
	memcpy(messa,"Detected character ",19);
	memcpy(messa+19,nick,nlen);
	sprintf(messa+19+nlen," (id:%d (%d.%d.%d))                         ",id,x,y,z);
	log_add(messa, tlen, 'w');
	//print_packet(raw);
	//printf(">> Получена информация о персонаже (id:%d координаты:(%d.%d.%d) скорость:%d/%d)\n",id,x,y,z,wspeed,rspeed);
	set_pl_stat(id, 0, nick, nlen, wspeed, rspeed, x, y, z);
}

void game_recv_userinfo(unsigned char* raw)
{
	int x,y,z,id,rspeed,wspeed;
	memcpy((char*)&x,raw+3,4);
	memcpy((char*)&y,raw+7,4);
	memcpy((char*)&z,raw+11,4);
	memcpy((char*)&id,raw+19,4);
	char nlen=0;
	while( raw[23+nlen*2]!=0x00 || raw[24+nlen*2]!=0x00 ) nlen++;
	char nick[nlen];
	uca(raw+23,nick,nlen);
	memcpy((char*)&rspeed,raw+277+2*nlen,4);
	memcpy((char*)&wspeed,raw+281+2*nlen,4);
	//printf(">> Получена информация о пользователе (id:%d координаты:(%d.%d.%d) скорость:%d/%d)\n",id,x,y,z,wspeed,rspeed);
	set_pl_stat(id, 0, nick, nlen, wspeed, rspeed, x, y, z);
}

void game_recv_say(unsigned char* raw)
{
	char mtype;
	memcpy((char*)&mtype,raw+7,4);
	char nlen=0;
	while( raw[11+nlen*2]!=0x00 || raw[12+nlen*2]!=0x00 ) nlen++;
	char nick[nlen];
	int mlen=0;
	while( raw[11+nlen*2+2+mlen*2]!=0x00 || raw[12+nlen*2+2+mlen*2]!=0x00 ) mlen++;
	short msg[mlen];
	//print_packet(raw);
	uca(raw+11,nick,nlen);
	uca(raw+11+nlen*2+2,msg,mlen);
	short tlen = nlen + mlen + 2;
	unsigned char messa[tlen];
	memcpy(messa,nick,nlen);
	messa[nlen]=0x3A;
	messa[nlen+1]=0x20;
	memcpy(messa+nlen+2,msg,mlen);
	log_add(messa, tlen, 'm');
	if(mlen != 0 && msg[0]==46) game_recv_cmd(msg, nick, mlen, nlen, mtype);
}

void game_recv_cmd(unsigned char* msg, unsigned char* nick, short mlen, char nlen, char mtype)
{
	unsigned char nbuf[400];
	const unsigned char info[]="Invyl's frontend Alaistair ver.0.3.0-r3 (made by proton@invyl.ath.cx, 2007-2008)";
	const unsigned char hint[]="Menu:\n.! - info\n.? - menu\n.a[a-l] - social action\n.u [skill num] - use skill (1216 - Self Heal, 1322 - Common Craft)\n.s[ ,=,!,+,#,@,$][text] - say text\n.w [0/1] - sit/stand\n.g [0/1] - go/run\n.p - party request\n.f - friend request\n.m - move\n";
	bzero(nbuf,400);
	switch(msg[1])
	{
		case 's'://Say
			nbuf[0]=0x38;
			switch(msg[2])
			{
				case '=':
					nbuf[1]=0;
				break;
				case '!':
					nbuf[1]=1;
				break;
				case '#':
					nbuf[1]=3;
				break;
				case '@':
					nbuf[1]=4;
				break;
				case '$':
					nbuf[1]=9;
				break;
				case '+':
					nbuf[1]=8;
				break;
				default:
					nbuf[1]=mtype;
				break;
			}
			memcpy(nbuf+2,msg+3,mlen-3);
			//if(mtype==2) memcpy(nbuf+mlen,nick,nlen);
			gmtd(nbuf);
		break;
		case 'w'://Sit|Stand
			nbuf[0]=0x1D;
			if(msg[2]=='1' || msg[3]=='1') nbuf[1]=1;
			if(msg[2]=='0' || msg[3]=='0') nbuf[1]=0;
			gmtd(nbuf);
		break;
		case 'g'://Go|Run
			nbuf[0]=0x1C;
			if(msg[2]=='1' || msg[3]=='1') nbuf[1]=1;
			if(msg[2]=='0' || msg[3]=='0') nbuf[1]=0;
			gmtd(nbuf);
		break;
		case 'm'://GoTo
			nbuf[0]=0x01;
			gmtd(nbuf);
		break;
		case 'a'://Social action
			if(msg[2]>='a' && msg[2]<='l')
			{
				nbuf[0]=0x1B;
				nbuf[1]=msg[2]-95;
				gmtd(nbuf);
			}
		break;
		case 'u'://Use Skill
			nbuf[0]=0x2F;
			int skilln;
  			skilln = atoi(msg+2);
			printf("%d\n",skilln);
			memcpy(nbuf+1,(char*)&skilln,4);
			gmtd(nbuf);
		break;
		case 'p'://Request Party
			nbuf[0]=0x29;
			nbuf[1]=2;
			memcpy(nbuf+2,nick,nlen);
			nbuf[nlen+2]=0;
			gmtd(nbuf);
		break;
		case 'f'://Request Adding to Friends
			nbuf[0]=0x5E;
			memcpy(nbuf+1,nick,nlen);
			nbuf[nlen+1]=0;
			gmtd(nbuf);
		break;
		case '!'://Info
			nbuf[0]=0x38;
			nbuf[1]=mtype;
			memcpy(nbuf+2,info,strlen(info));
			if(mtype==2) memcpy(nbuf+strlen(info)+3,nick,nlen);
			gmtd(nbuf);
		break;
		case '?'://Help
			nbuf[0]=0x38;
			nbuf[1]=mtype;
			memcpy(nbuf+2,hint,strlen(hint));
			if(mtype==2) memcpy(nbuf+strlen(hint)+3,nick,nlen);
			gmtd(nbuf);
		break;
		default:
			nbuf[0]=0x38;
			nbuf[1]=mtype;
			memcpy(nbuf+2,"Sorry, I don't speak albanian =(",33);
			if(mtype==2) memcpy(nbuf+36,nick,nlen);
			gmtd(nbuf);
		break;
	}
}
