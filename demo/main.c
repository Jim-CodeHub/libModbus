/**-----------------------------------------------------------------------------------------------------------------
 * @file	main.c
 *
 * Copyright (c) 2019-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include "modbcd.h"


/*
--------------------------------------------------------------------------------------------------------------------
*
												   VARIABLES 
*
--------------------------------------------------------------------------------------------------------------------
*/


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

int main(void)
{
	uint8_t		*rsps_data; /**< Response data field pointer */
	uint16_t	 rsps_leng; /**< Response data field length  */

	( eMBErrorCode )eMBCDInit(0, 9600, MB_PAR_EVEN, 20);
	( eMBErrorCode )eMBCDEnable( );

	//Set other init here

	__asm__  volatile ("sei");

	while ( 1 )
	{
		( eMBErrorCode )eMBCDPoll( );

		uint8_t rqst_data[] = { 0x00, 0x00, 0x00, 0x13 }; //Request PDU = starting Address : 0x0000, Quantity of coils : 0x13

		if ( MB_ENOERR == eMBCDSend(0X0A, 0X01, rqst_data, sizeof(rqst_data)) )
		{
			eMBCDLoad( &rsps_data, &rsps_leng );

			//uint8_t byteCnt = rsps_data[0];

			//uint8_t CoilsB1 = rsps_data[1]; //0x00 ~ 0x07 
			//uint8_t CoilsB2 = rsps_data[2]; //0x08 ~ 0x0F 
			//uint8_t CoilsB3 = rsps_data[3]; //0X10 ~ 0X13

			//Do something below
		}
	}
}
