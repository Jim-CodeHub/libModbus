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
	uint8_t		*rsps_data; /**< Response frame PDU (FunCode + DataFeild) pointer */
	uint16_t	 rsps_leng; /**< Response frame PDU length */

	( eMBCD_ErrorCode )eMBCD_Init(0, 9600, PARITY_EVEN, 20);
	( eMBCD_ErrorCode )eMBCD_Enable( );

	//Set other init here

	__asm__  volatile ("sei");

	while ( 1 )
	{
		uint8_t rqst_data[] = { 0x00, 0x13, 0x00, 0x13 }; //Request PDU = starting Address : 0x0013, Quantity of coils : 0x13

		if ( ERR_NOERR == eMBCD_Send(0X0A, 0X01, rqst_data, sizeof(rqst_data)) )
		{
					//Do something
		}

		eMBCD_ErrorCode eErr = eMBCD_Load( &rsps_data, &rsps_leng );

		switch ( eErr )
		{
			case ERR_NOERR:
				//uint8_t FunCode = rsps_data[0];
				//uint8_t byteCnt = rsps_data[1];

				//uint8_t CoilsB1 = rsps_data[2]; //0x13 ~ 0x1A	 
				//uint8_t CoilsB2 = rsps_data[3]; //0x1B ~ 0x22 
				//uint8_t CoilsB3 = rsps_data[4]; //0X23 ~ 0X2A
				//Do something
				break;
			case ERR_TIMEDOUT:
				//Do something
				break;
			default: 
				//Do something
				;
		}

		//Set other code
	}
}

