#include <modbcd/util/mbcrc.h>
#include <stdio.h>

/**
 *	NOTE : size = sizeof(xxx) - 1 (TO DELETE '\0')
 * */
int main(void)
{
	//unsigned char msg[] = "hello modbcd";

	//unsigned short crc = mbcrc(msg, sizeof(msg)/sizeof(unsigned char) - 1);
	
	unsigned char data[] = {1, 2, 3, 4};

	unsigned short crc = mbcrc(data, sizeof(data));

	printf("crc = %#X\n", crc);

	return 0;
}

