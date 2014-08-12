// for xstrlen

// from azillionmonkeys.com/qed/asmexample.html

#define hasNulByte(x) ((x - 0x01010101) & ~x & 0x80808080)
#define SW (sizeof(int) / sizeof(char))


unsigned int xstrlen(const char *s)
{
	const char *p;
	unsigned int d;
	
	p = s - 1;
	do
	{
		p++;
		if ((((int)p)&(SW - 1)) == 0)
		{
			do
			{
				d = *((int *)p);
				p += SW;
			}while(!hasNulByte (d));
			p -= SW;
		}
	}while (*p != 0);
	return p - s;
}
