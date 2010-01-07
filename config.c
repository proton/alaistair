#include <stdio.h>

void config(unsigned char* address, int* port, unsigned char* key, int* protocol, char* login, char* pwd, char* servnum, char* charnum)
{
	unsigned char gbuf[200];
	FILE* file = fopen("config", "r");
	fseek (file , 0 , SEEK_END);
	size_t fsize = ftell (file);
	rewind (file);
	fread (gbuf,1,fsize,file);
	fclose(file);
	unsigned char i=0,n=0,ibuff[5];
	while(gbuf[i]!=0x0A) ++i;
	memcpy(address,gbuf+n,i-n);
	n=++i;
	while(gbuf[i]!=0x0A) ++i;
	memcpy(ibuff,gbuf+n,i-n);
	*port = atoi(ibuff);
	n=++i;
	while(gbuf[i]!=0x0A) ++i;
	memcpy(key,gbuf+n,i-n);
	n=++i;
	while(gbuf[i]!=0x0A) ++i;
	bzero(ibuff,sizeof(ibuff));
	memcpy(ibuff,gbuf+n,i-n);
	*protocol = atoi(ibuff);
	n=++i;
	while(gbuf[i]!=0x0A) ++i;
	memcpy(login,gbuf+n,i-n);
	n=++i;
	while(gbuf[i]!=0x0A) ++i;
	memcpy(pwd,gbuf+n,i-n);
	*servnum=gbuf[++i]-48;
	*charnum=gbuf[i+2]-48;
}
