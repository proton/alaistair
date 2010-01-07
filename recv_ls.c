#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct
{
	unsigned char id;
	unsigned char ip[16];
	unsigned int port;
	unsigned int online;
	unsigned int players;
	unsigned char status;
} ServerList;

int ls_pc_td(unsigned char* raw, unsigned char* sessionkey1_p1, unsigned char* sessionkey1_p2, unsigned char* sessionkey2_p1, unsigned char* sessionkey2_p2, ServerList* servs)
{
	//printf("[>] Определение типа полученного пакета...\n");
	int l=1;
	switch(raw[2]) {
		case 0x1:
		ls_recv_logfail(raw);
		l = -1;
		break;
		case 0x3:
		ls_recv_logok(raw,sessionkey1_p1,sessionkey1_p2);
		l = 3;
		break;
		case 0x4:
		ls_recv_servlst(raw,servs);
		l = 4;
		break;
		case 0x6:
		ls_recv_playfail(raw);
		l = -1;
		break;
		case 0x7:
		ls_recv_playok(raw,sessionkey2_p1,sessionkey2_p2);
		l = 7;
		break;
		case 0xF:
		ls_recv_error();
		l = -1;
		break;
		default:
		//printf("[!] Невозможно определить тип полученного пакета (0x%.2X)\n",raw[2]);
		break;
	}
	return l;
}

void ls_recv_logok(unsigned char* raw, unsigned char* sessionkey_p1, unsigned char* sessionkey_p2)
{
	printf("[!] Авторизация на логин-сервере успешно произведена!\n");
	memcpy(sessionkey_p1,raw+3,4);
	memcpy(sessionkey_p2,raw+7,4);
	printf("[!] Задан SessionKey1: ");	
	print_key(sessionkey_p1,4);
	print_key(sessionkey_p2,4);
	printf("\n");
}

void ls_recv_logfail(unsigned char *raw)
{
	switch(raw[3])
	{
		case 0x1:
		printf("[X] Невозможно выполнить авторизацию из-за системной ошибки =(\n");
		break;
		case 0x2:
		printf("[X] Невозможно выполнить авторизацию, неверный пароль =(\n");
		break;
		case 0x3:
		printf("[X] Невозможно выполнить авторизацию, логин или пароль неверны =(\n");
		break;
		case 0x4:
		printf("[X] Невозможно выполнить авторизацию, доступ запрещен =(\n");
		break;
		case 0x5:
		printf("[X] Невозможно выполнить авторизацию, информация на аккаунте неверна =(\n");
		break;
		case 0x7:
		printf("[X] Невозможно выполнить авторизацию, аккаунт уже используется =(\n");
		break;
		case 0x9:
		printf("[X] Невозможно выполнить авторизацию, аккаунт забанен =(\n");
		break;
		case 0x10:
		printf("[X] Невозможно выполнить авторизацию, на сервере идут сервисные работы =(\n");
		break;
		case 0x12:
		printf("[X] Невозможно выполнить авторизацию, срок действия аккаунта истек =(\n");
		break;
		default:
		printf("[X] Невозможно выполнить авторизацию по неизвестной причине =( (код причины: %d)\n",raw[3]);
		break;
	}
}

void ls_recv_playok(unsigned char *raw, unsigned char *sessionkey_p1, unsigned char *sessionkey_p2)
{	
	printf("[!] Проверка возможности подключения успешно произведена! \n");
	memcpy(sessionkey_p1,raw+3,4);
	memcpy(sessionkey_p2,raw+7,4);
	printf("[!] Задан SessionKey2: ");
	print_key(sessionkey_p1,4);
	print_key(sessionkey_p2,4);
	printf("\n");
}

void ls_recv_playfail(unsigned char *raw)
{
	printf("[X] Невозможно подключиться к данному игровому серверу =( (код причины: %d)\n",raw[3]);
}

void ls_recv_servlst(unsigned char* raw, ServerList* servs)
{	
	int i;
	printf("[!] Получен список серверов (%d):\n",raw[3]);
	for(i=0; i<47; i++) printf("-"); printf("\n");
	printf(" # |         Адрес        |   Онлайн  | Статус\n");
	for(i=0; i<47; i++) printf("-"); printf("\n");
	struct in_addr inn;
	for(i=0;i<raw[3];i++){
		servs[i].id=raw[i*21+5];
		memcpy((char *)&inn.s_addr,raw+i*21+6,4);
			memcpy((char *)&servs[i].ip,inet_ntoa(inn),15);
		memcpy((char *)&servs[i].port,raw+i*21+10,4);
		memcpy((char *)&servs[i].online,raw+i*21+16,2);
		memcpy((char *)&servs[i].players,raw+i*21+18,2);
		servs[i].status=raw[i*21+20];
		printf("%2i | %15s:%4i | %4i/%.4i | ",servs[i].id, servs[i].ip, servs[i].port, servs[i].online, servs[i].players);
		if(servs[i].status==0) printf("Offline\n");
		if(servs[i].status==1) printf("Online\n");
	}
	for(i=0; i<47; i++) printf("-"); printf("\n");
}

void ls_recv_error()
{
	printf("[X] При попытке подключения возникла критическая ошибка, возможно вы соединяетесь с несовместимым/неработающим сервером, проверьте ip =)\n");
}
