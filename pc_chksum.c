void add_ckecksum(unsigned char *raw, int count)
{
	unsigned long chksum = 0;
	unsigned long ecx;
	int i;
	for(i = 0; i < count; i += 4) {
		ecx  = (unsigned long)raw[i];
		ecx |= (unsigned long)raw[i + 1]<<8;
		ecx |= (unsigned long)raw[i + 2]<<16;
		ecx |= (unsigned long)raw[i + 3]<<24;
		chksum ^= ecx;
	}
	memcpy(raw+count, (char *)&chksum, 4);
}

void add_len(unsigned char *raw, int len)
{	
	bzero (raw,len);
	raw[1]=len/256;
	raw[0]=len-raw[1]*256;
}