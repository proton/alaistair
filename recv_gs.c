void gs_recv_firstkey(unsigned char* raw, char* sc, char* cs)
{
	sc[0] = raw[4];
        sc[1] = raw[5];
        sc[2] = raw[6];
        sc[3] = raw[7];
        sc[4] = (char)0xA1;
        sc[5] = (char)0x6C;
        sc[6] = (char)0x54;
        sc[7] = (char)0x87;	
	printf("[!] Задан оригнальный ключ XorKey: ");
	print_key(sc, 8);
	printf("\n");
	memcpy(cs,sc,8);
}

void gs_recv_charlst(unsigned char* raw)
{	
	printf("[!] Получен список персонажей на аккаунте (%d штучег)\n",raw[3]);
}