void ls_build_auth(unsigned char *raw, char *log, char *pass)
{
	short pc_len=0x30;
	char packet_skeleton[] =
		"\x32\x00" // размерчег 0x30 + 0x02
		"\x00" // (0x00 - RequestAuthLogin)
		"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // login
		"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // password
		"\x08"
		"\x00\x00\x00\x00\x00\x00\x00\x00"
		"\x00\x00\x00\x00"
		"\x00\x00\x00\x00";

	memcpy(packet_skeleton+3,log,strlen(log));
	memcpy(packet_skeleton+17,pass,strlen(pass));

	add_ckecksum((unsigned char*)packet_skeleton + 2, pc_len - 8);

	memcpy(raw,packet_skeleton,pc_len+2);
}

void ls_build_servlst(unsigned char *raw, unsigned char *sessionkey_p1, unsigned char *sessionkey_p2)
{
	short pc_len=0x18;
	char packet_skeleton[] =
		"\x1A\x00" // размерчег 0x18 + 0x02
		"\x05" // (0x05 - RequestServerList)
		"\x00\x00\x00\x00" // SessionKey1_p1
		"\x00\x00\x00\x00" // SessionKey1_p2
		"\x04\x00\x00\x00"
		"\x00\x00\x00"
		"\x00\x00\x00\x00"
		"\x00\x00\x00\x00";

	memcpy(packet_skeleton+3,sessionkey_p1,strlen(sessionkey_p1));
	memcpy(packet_skeleton+7,sessionkey_p2,strlen(sessionkey_p2));

	add_ckecksum((unsigned char*)packet_skeleton + 2, pc_len - 8);

	memcpy(raw,packet_skeleton,pc_len+2);
}

void ls_build_servlogin(unsigned char *raw, unsigned char *sessionkey_p1, unsigned char *sessionkey_p2, char num)
{
	printf("[>] Проверка возможности подключения к серверу #%d...\n",num);
	short pc_len=0x18;
	char packet_skeleton[] =
		"\x1A\x00" // размерчег 0x18 + 0x02
		"\x02" // (0x05 - RequestServerLogin)
		"\x00\x00\x00\x00" // SessionKey1_p1
		"\x00\x00\x00\x00" // SessionKey1_p2
		"\x00"
		"\x00\x00\x00\x00\x00\x00"
		"\x00\x00\x00\x00"
		"\x00\x00\x00\x00";

	memcpy(packet_skeleton+3,sessionkey_p1,strlen(sessionkey_p1));
	memcpy(packet_skeleton+7,sessionkey_p2,strlen(sessionkey_p2));
	packet_skeleton[11]=num;

	add_ckecksum((unsigned char*)packet_skeleton + 2, pc_len - 8);

	memcpy(raw,packet_skeleton,pc_len+2);
}