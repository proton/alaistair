#include <time.h>
#include <stdio.h>

int print_packet(unsigned char *pkgout)
{
	int i,c = 0, len=pkgout[1]*256+pkgout[0];
	for(i=0;i<54;i++) printf("_");
	
	for(i=0;i<len;i++) {
		if((c % 0x10)==0) printf("\n0x%.2X | ", c);
		printf("%.2X ",pkgout[i] & 0xFF);
		c++;		
	}
	printf("\n\n");
}

int print_key(char *pkgout, int len)
{
	int i;	
	for(i=0;i<len;i++) printf("%.2X ",pkgout[i] & 0xFF);
}

void log_add(unsigned char *msg, int len, char type)
{
	time_t mytime;
	time(&mytime);
	struct tm myti = *localtime(&mytime);
	unsigned char messa[len+21];
	strftime(messa,len+21, "[%2d/%2m/%4Y %2H:%2M] ", &myti);
	switch(type)
	{
		case 'm':
		messa[19]='>';
		break;
		case 'a':
		messa[19]='#';
		break;
		case 'w':
		messa[19]='*';
		break;
		case 'c':
		messa[19]='!';
		break;
		default:
		messa[19]=' ';
		break;
	}
	memcpy(messa+20,msg,len);
	messa[len+20]=0x0A;
	FILE *file = fopen("out", "a");
	fwrite (messa,1,len+21,file);
	fclose(file);
}
