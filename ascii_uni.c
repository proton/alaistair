void acu(unsigned char* in, unsigned char* out)
{
	int i,n=strlen(in);
	for(i=0; i<n; ++i)
	{
		if(in[i]<0x7F)
		{
			out[2*i]=in[i];
			out[2*i+1]=0x00;
		}
		else if(in[i]>=0xC0)
		{
			out[2*i]=in[i]-0xB0;
			out[2*i+1]=0x04;
		}
		else if(in[i]==0xA8)
		{
			out[2*i]=0x01;
			out[2*i+1]=0x04;
		}
		else if(in[i]==0xB8)
		{
			out[2*i]=0x51;
			out[2*i+1]=0x04;
		}
	}
//print_key(out,2*n);
}

void uca(unsigned char* in, unsigned char* out, int len)
{
	int i;
	for(i=0; i<len; ++i)
	{
		if(in[i*2+1]*256+in[i*2]<0x7F) out[i]=in[i*2];
		else if(in[i*2+1]*256+in[i*2]>=0x410) out[i]=in[i*2]+0xB0;
		else if(in[i*2+1]*256+in[i*2]==0x401) out[i]=0xA8;
		else if(in[i*2+1]*256+in[i*2]==0x451) out[i]=0xB8;
	}
}
