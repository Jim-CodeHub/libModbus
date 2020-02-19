/**-----------------------------------------------------------------------------------------------------------------
 * @file	mblrc.c
 * @brief	Modbus LRC algorithm	
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include "mblrc.h"


#if  MBCD_CFG_MOD_ASCII_EN > 0 //Ascii mode enabled

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
UCHAR mblrc(const UCHAR *auchMsg, USHRT usDataLen)
{
	UCHAR uchLRC = 0;

	while (usDataLen--)
	{
		uchLRC += *auchMsg++; /*< add buffer byte without carry */
	}

	return ((UCHAR)(-((char)uchLRC)));  /*< twos complement */
}


#endif //MBCD_CFG_MOD_ASCII_EN > 0


