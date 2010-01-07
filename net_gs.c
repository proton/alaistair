#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void gs_pc_enc(unsigned char* raw, int size, char* key) 
{
	int i,temp = 0;
	for (i=0; i<size; ++i) 
	{ 
		int temp2 = raw[i] &0xff; 
		raw[i] = (char)(temp2 ^ (key[i&7] &0xff) ^ temp); 
		temp = raw[i]; 
	}
	int old = key[0] &0xff; 
	old |= key[1] << 8 &0xff00; 
	old |= key[2] << 0x10 &0xff0000; 
	old |= key[3] << 0x18 &0xff000000; 
	old += size; 
	key[0] = (char)(old &0xff); 
	key[1] = (char)(old >> 0x08 &0xff); 
	key[2] = (char)(old >> 0x10 &0xff); 
	key[3] = (char)(old >> 0x18 &0xff);
}

void gs_pc_dec(unsigned char* raw, int size, char* key)
{
	int k,i = 0,j = 0;
	for(k=0; k<size; ++k)
	{
		int i1=raw[k] & 0xFF;
		raw[k]=(char)(i1 ^ key[j++] & 0xFF ^i);
		i=i1;
		if(j>7) j=0;
	}
	long l = key[0] & 0xFF;
	l |= key[1] << 8 & 0xFF00;
	l |= key[2] << 16 & 0xFF0000;
	l |= key[3] << 24 & 0xFF000000;
	l += size;
	key[0] = (char)(l & 255L);
	key[1] = (char)(l >> 8 & 255L);
	key[2] = (char)(l >> 16 & 255L);
	key[3] = (char)(l >> 24 & 255L);
}

int connect_to_gs(int* ps, char* ip, int port, unsigned char* rraw, unsigned char* sraw, char* cs, char* sc, int proto)
{
	struct sockaddr_in sock;
	int c;
	sock.sin_addr.s_addr=inet_addr(ip);
	sock.sin_port = htons(port);
	sock.sin_family = AF_INET;
	printf("[>] Попытка установки соединения с игровым сервером (%s:%d)...\n",ip,port);	
	*ps = socket(AF_INET, SOCK_STREAM, 0);
	if(connect(*ps, &sock, sizeof(sock))==0)
	{
		printf("[!] Соединение успешно установлено!\n");
		gs_build_protocol(sraw,proto);
		send(*ps, sraw, sraw[1]*256+sraw[0], 0);
		bzero(sraw,65536);
		bzero(rraw,65536);
		recv(*ps, rraw, 65536, 0);
		//print_packet(rraw);
		gs_recv_firstkey(rraw,sc,cs);
		return 0;
	}
	else
	{
		printf("[X] Соединение не установлено =(\n");
		return -1;
	}
}

int recv_gs_pc(int* ps, unsigned char* raw, char* sc)
{
	int l;
	bzero(raw,65536);
	//printf("[>] Пытаемся получить данные у сервера..\n");
	retry:
	l=recv(*ps, raw, 65536, 0);
	if(l < 0)
	{
		printf("[X] Ошибка при получении данных\n");
		return -1;
	}
	if(l > 0)
	{
		//printf("[!] Принято %i байт:\n", l);
		//print_packet(raw);
		gs_pc_dec(raw+2,raw[1]*256+raw[0]-2,sc);
		//print_packet(raw);
		if(raw[2]==0x13) gs_recv_charlst(raw);
	}
	if(l == 0) goto retry;
	return l;
}

int send_gs_pc(int* ps, unsigned char* raw, char* cs)
{
	int l;
	//printf("[!] Отправлено %d (%X) байт:\n", raw[1]*256+raw[0],raw[1]*256+raw[0]);
	//print_packet(raw);
	gs_pc_enc(raw+2,raw[1]*256+raw[0]-2,cs);
	//print_packet(raw);
	l=send(*ps, raw, raw[1]*256+raw[0], 0);
	if(l <= 0)
	{
		printf("[X] Ошибка при отправке данных =(\n");
		return -1;
	}
	bzero(raw,65536);
	return l;
}
