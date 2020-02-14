/**-----------------------------------------------------------------------------------------------------------------
 * @file	trans.c
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <modbcd/util/trans.h>


/*
--------------------------------------------------------------------------------------------------------------------
*
		                                      FUNCTION PROTOTYPES
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

/**
 *	@brief	    Trans 8bit Hex value to two Ascii (Capital & alphanum) characters 
 *	@param[in]  Hex 
 *	@param[out] ascii	
 *	@return		None	
 **/
void hexToAscii(unsigned char Hex, char ascii[2])
{
	char H = Hex >> 4  ;
	char L = Hex & 0X0F;

	*ascii = H + ((H < 10)?48:55); ascii++;
	*ascii = L + ((L < 10)?48:55);
	
	return;
}

/**
 *	@brief	    Trans two Ascii (Capital & alphanum) characters to 8bit Hex value
 *	@param[in]  ascii_1 
 *	@param[in]  ascii_2 
 *	@param[out] None 
 *	@return		None	
 *	@note		Param ascii_1, ascii_2 is continuous
 **/
unsigned char asciiToHex(char ascii_1, char ascii_2)
{
	char H = ((ascii_1 >= '0') && (ascii_1 <= '9'))?(ascii_1-48):(ascii_1-55);
	char L = ((ascii_2 >= '0') && (ascii_2 <= '9'))?(ascii_2-48):(ascii_2-55);

	return (H << 4)|L;
}

