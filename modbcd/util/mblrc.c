/**-----------------------------------------------------------------------------------------------------------------
 * @file	mblrc.c
 * @brief	Modbus LRC algorithm	
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <modbcd/util/mblrc.h>


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
 *	@brief	    Modbus LRC calculation 
 *	@param[in]  auchMsg	  - message to calculate LRC upon
 *	@param[in]	usDataLen - the quantity of bytes in the message buffer
 *	@param[out] None
 *	@return		LRC value	
 **/
unsigned char mblrc(const unsigned char *auchMsg, unsigned short usDataLen)
{
	unsigned char uchLRC = 0;

	while (usDataLen--)
	{
		uchLRC += *auchMsg++; /*< add buffer byte without carry */
	}

	return ((unsigned char)(-((char)uchLRC)));  /*< twos complement */
}

