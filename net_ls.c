#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/blowfish.h>

typedef struct
{
	unsigned char id;
	unsigned char ip[16];
	unsigned int port;
	unsigned int online;
	unsigned int players;
	unsigned char status;
} ServerList;

int connect_to_ls(int* ps, char* ip, int port)
{
	struct sockaddr_in sock;
	int c;
	sock.sin_addr.s_addr=inet_addr(ip);
	sock.sin_port = htons(port);
	sock.sin_family = AF_INET;
	printf("[>] Попытка установки соединения с сервером авторизации (%s:%d)...\n",ip,port);
	*ps = socket(AF_INET, SOCK_STREAM, 0);
	if(connect(*ps, &sock, sizeof(sock))==0){
		printf("[!] Соединение успешно установлено!\n");
		unsigned char buf[12];	
		c = recv(*ps, buf, sizeof(buf), 0);
		printf("[!] Получено приветствие от сервера!\n");
		print_packet(buf);
		return 0;
	}
	else
	{
		printf("[X] Соединение не установлено =(\n");
		return -1;
	}
}

int recv_ls_pc(int* ps, unsigned char* raw, unsigned char* sessionkey1_p1, unsigned char* sessionkey1_p2, unsigned char* sessionkey2_p1, unsigned char* sessionkey2_p2, BF_KEY bfkey, ServerList* servs)
{
	int i,l;
	bzero(raw,65536);
	printf("[>] Пытаемся получить данные у сервера..\n");
	retry:
	l=recv(*ps, raw, 65536, 0);
	if(l < 0)
	{
		printf("[X] Ошибка при получении данных\n");
		return -1;
	}
	if(l > 0)
	{
		printf("[!] Принято %i байт:\n", l);
		print_packet(raw);
		for(i = 0; i < (raw[1]*256+raw[0])/8; ++i) BF_decrypt((BF_LONG *)((short*)raw+1+i*4), &bfkey);
		print_packet(raw);
		l = ls_pc_td(raw,sessionkey1_p1,sessionkey1_p2,sessionkey2_p1,sessionkey2_p2,servs);
	}
	if(l == 0)
		goto retry;
	return l;
}

int send_ls_pc(int* ps, unsigned char* raw, BF_KEY bfkey)
{
	int i,l;

	//printf("[!] Отправлено %d байт:\n", raw[1]*256+raw[0]);
	//print_packet(raw);
	for(i = 0; i < (raw[1]*256+raw[0])/8; ++i) BF_encrypt((BF_LONG *)((short*)raw+1+i*4), &bfkey);
	l=send(*ps, raw, raw[1]*256+raw[0], 0);
	if(l <= 0)
	{
		printf("[X] Ошибка при отправке данных =(\n");
		return -1;
	}
	bzero(raw,65536);
	return l;
}
