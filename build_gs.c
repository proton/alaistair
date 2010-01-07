void gs_build_protocol(unsigned char* raw, int protocol)
{
	short pc_len=0x107;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	memcpy(packet_skeleton+3,(unsigned char *)&protocol,4);
	memcpy(packet_skeleton+7,"\x09\x07\x54\x56\x03\x09\x0B\x01\x07\x02\x54\x54\x56\x07\x00\x02",16);
	memcpy(packet_skeleton+23,"\x55\x56\x00\x51\x00\x53\x57\x04\x07\x55\x08\x54\x01\x07\x01\x53",16);
	memcpy(packet_skeleton+39,"\x00\x56\x55\x56\x01\x06\x05\x04\x51\x03\x08\x51\x08\x51\x56\x04",16);
	memcpy(packet_skeleton+55,"\x54\x06\x55\x08\x02\x09\x51\x56\x01\x53\x06\x55\x04\x53\x00\x56",16);
	memcpy(packet_skeleton+71,"\x56\x53\x01\x09\x02\x09\x01\x51\x54\x51\x09\x55\x56\x09\x03\x04",16);
	memcpy(packet_skeleton+87,"\x07\x05\x55\x04\x06\x55\x04\x06\x09\x04\x51\x01\x08\x08\x06\x05",16);
	memcpy(packet_skeleton+103,"\x52\x06\x04\x01\x07\x54\x03\x06\x52\x55\x06\x55\x55\x51\x01\x02",16);
	memcpy(packet_skeleton+119,"\x04\x54\x03\x55\x54\x01\x57\x51\x55\x05\x52\x05\x54\x07\x51\x51",16);
	memcpy(packet_skeleton+135,"\x55\x07\x02\x53\x53\x00\x52\x05\x52\x07\x01\x54\x00\x03\x05\x05",16);
	memcpy(packet_skeleton+151,"\x08\x06\x05\x05\x06\x03\x00\x0D\x08\x01\x07\x09\x03\x51\x03\x07",16);
	memcpy(packet_skeleton+167,"\x53\x09\x51\x06\x07\x54\x0A\x50\x56\x02\x52\x04\x05\x55\x51\x02",16);
	memcpy(packet_skeleton+183,"\x53\x00\x08\x54\x04\x52\x56\x06\x02\x09\x00\x08\x03\x53\x56\x01",16);
	memcpy(packet_skeleton+199,"\x05\x00\x55\x06\x08\x56\x04\x0D\x06\x07\x52\x06\x07\x04\x0A\x06",16);
	memcpy(packet_skeleton+215,"\x01\x04\x54\x04\x00\x05\x02\x04\x54\x00\x09\x52\x53\x05\x04\x01",16);
	memcpy(packet_skeleton+231,"\x04\x05\x05\x01\x52\x51\x52\x0D\x06\x51\x08\x09\x54\x53\x00\x0D",16);
	memcpy(packet_skeleton+247,"\x01\x02\x03\x54\x53\x01\x05\x03\x08\x56\x54\x07\x02\x54\x0B\x06",16);
	memcpy(raw,packet_skeleton,pc_len);
}

void gs_build_auth(unsigned char* raw, char* log, unsigned char* sessionkey1_p1, unsigned char* sessionkey1_p2, unsigned char* sessionkey2_p1, unsigned char* sessionkey2_p2)
{	
	int loglen = 2*strlen(log);
	short pc_len=2+1+loglen+2+8+8+4;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x08;
	char ulog[loglen];
	acu(log,ulog);
	memcpy(packet_skeleton+3,ulog,loglen);
	memcpy(packet_skeleton+loglen+5,sessionkey2_p2,4);
	memcpy(packet_skeleton+loglen+9,sessionkey2_p1,4);
	memcpy(packet_skeleton+loglen+13,sessionkey1_p1,4);
	memcpy(packet_skeleton+loglen+17,sessionkey1_p2,4);
	packet_skeleton[loglen+21]=0x01;
	memcpy(raw,packet_skeleton,pc_len);
}

void gs_build_charselect(unsigned char *raw, char num)
{
	short pc_len=21;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x0D;
	packet_skeleton[3]=num;
	memcpy(raw,packet_skeleton,pc_len);
	printf("[>] Выполняется вход персанажем #%d...\n",num);
}

void gs_build_mlist(unsigned char *raw)
{
	short pc_len=5;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0xD0;
	packet_skeleton[3]=0x08;
	memcpy(raw,packet_skeleton,pc_len);	
}

void gs_build_qlist(unsigned char *raw)
{
	short pc_len=3;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x63;
	memcpy(raw,packet_skeleton,pc_len);	
}

void gs_build_enterworld(unsigned char *raw)
{
	short pc_len=13;
	char packet_skeleton[pc_len];
	add_len(packet_skeleton,pc_len);
	packet_skeleton[2]=0x03;
	memcpy(raw,packet_skeleton,pc_len);
	printf("[>] Входим в мир...\n");
}
