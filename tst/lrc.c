#include <modbcd/util/mblrc.h>
#include <stdio.h>

/**
 *	NOTE : size = sizeof(xxx) - 1 (TO DELETE '\0')
 * */
int main(void)
{
	unsigned char msg[] = "hello modbcd";

	unsigned short lrc = mblrc(msg, sizeof(msg)/sizeof(unsigned char) - 1);

	printf("lrc = %#X\n", lrc);

	return 0;
}

