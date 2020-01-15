#include <modbcd/frame/serial/mbrtu.h>
#include <modbcd/frame/serial/mbascii.h>
#include <stdio.h>

int main(void)
{
#if 0 //RTU Frame

	unsigned char data[] = {1, 2, 3, 4};

	struct mb_rtu_frame frame = mbrtu_set_frame(0X01, 0X02, data, sizeof(data));

	printf("address = %#X\n, funCode = %#X\n, crc = %#X\n, size = %d\n",
			frame.address,   frame.funCode,   frame.crc,   frame._size);

	puts("---------------------------------------------------");

	unsigned char buff[] = {0x01, 0x02, 1, 2, 3, 4, 0XD9, 0X05};

	struct mb_rtu_frame fram2 = mbrtu_get_frame(buff, sizeof(buff)); 

	printf("address = %#X\n, funCode = %#X\n, crc = %#X\n, size = %d\n",
			fram2.address,   fram2.funCode,   fram2.crc,   fram2._size);

#else //ASCII Frame

	unsigned char data[] = {1, 2, 3, 4};

	struct mb_ascii_frame_send frame = mbascii_set_frame(0X01, 0X02, data, sizeof(data));

	printf("address[0] = %d\n, address[1] = %d\n, funCode[0] = %d\n, funCode[1] = %d\n, lrc[0] = %d\n, lrc[1] = %d\n , size = %d\n", 
							
			frame.address[0], frame.address[1], frame.funCode[0], frame.funCode[1], frame.lrc[0], frame.lrc[1], frame._size);

	puts("---------------------------------------------------");

	unsigned char buff[] = {48, 49, 48, 50, 48, 49, 48, 50, 48, 51, 48, 52, 66, 55};

	struct mb_ascii_frame_recv fram2 = mbascii_get_frame(buff, sizeof(buff)); 

	printf("address = %#X\n, funCode = %#X\n, lrc = %#X\n, size = %d\n" ,
			fram2.address,   fram2.funCode,   fram2.lrc , fram2._size );

	int size = fram2._size; 

	int inx= 0;
	while (size--)
	{
		printf("get buff[%d] = %d\n", inx, buff[inx]);
		inx++;
	}

#endif

	return 0;
}

