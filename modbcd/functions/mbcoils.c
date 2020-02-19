/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbcoils.c
 * @brief   Read and Write modbus coils (function code -> 0x01, 0x05, 0x0F supported)	
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include "mbcoils.h"


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
 *	@brief		Read coils (function code = 0x01) 
 *	@param[in]	data - Frame buffer which has been parsed
 *	@param[out] None
 *	@return		
 *	@note		
 **/
UCHAR mb_read_coils(const UCHAR *data, UCHAR *size)
{
#if  MBCD_CFG_COD_RD_COILS_EN > 0 

	/**--------------------- Check the Validate data address -----------------------*/
		
	UINT16 Quantity = (data[4] << 8) | (data[5]);

	if (!((Quantity >= 0x0001) && (Quantity <= 0X07D))) { goto _EXCEPT_03; }

	/**--------------------- Check the Validate data value -------------------------*/

	UINT16 StartAdr = (data[1] << 8) | (data[2]);
	//Note 1 address 0

	if (!(StartAdr >= 0x0000) && (StartAdr <= 0XFFFF)) 
	{
		//这里 start地址是否OK 和 start+线圈数量是否OK 在freeModbus中 放在 excute中高定了，excute定义返回值（最好用enum）
	}

	/**--------------------- Check the Execute MB result ---------------------------*/
_EXCEPT_01:
	return MBCD_EXPT_ILLEGAL_FUNCTION;

_EXCEPT_02:
	return MBCD_EXPT_ILLEGAL_DATA_ADDRESS;

_EXCEPT_03:
	return MBCD_EXPT_ILLEGAL_DATA_VALUE;

_EXCEPT_04:
	return MBCD_EXPT_SERVER_DEVICE_FAILURE;

#else
	return MBCD_EXPT_ILLEGAL_FUNCTION; /**< EXCEPTION 0x01 */

#endif //MBCD_CFG_COD_RD_COILS_EN > 0 
}

/**
 *	@brief		Write single coils (function code = 0x05) 
 *	@param[in]  
 *	@param[out] 
 *	@return		
 *	@note		
 **/

/**
 *	@brief		Write multiple coils (function code = 0x0F) 
 *	@param[in]  
 *	@param[out] 
 *	@return		
 *	@note		
 **/

