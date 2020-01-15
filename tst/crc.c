#include <modbcd/util/mbcrc.h>
#include <stdio.h>

/**
 *	NOTE : size = sizeof(xxx) - 1 (TO DELETE '\0')
 * */
int main(void)
{
	unsigned char msg[] = "hello modbcd";

	unsigned short crc = mbcrc(msg, sizeof(msg)/sizeof(unsigned char) - 1);

	printf("crc = %#X\n", crc);

	return 0;
}

