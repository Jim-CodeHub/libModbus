/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbcoils.c
 * @brief   Read and Write modbus coils (function code -> 0x01, 0x05, 0x0F supported)	
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <modbcd/functions/data_access/_01bit_access/mbcoils.h>


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
 *	@param[in]	data - Frame buffer which hass been parsed
 *	@param[out] None
 *	@return		
 *	@note		
 **/
unsigned char mb_read_coils(const unsigned char *data)
{
	//TBD ADD PDU LENGTH CHECK
	
#if  MBCD_CFG_COD_RD_COILS_EN > 0 

	/**--------------------- Check the Validate data address -----------------------*/

	UINT16 Quantity = (data[3] << 8) | (data[4]);

	if (!((Quantity >= 0x0001) && (Quantity <= 0X07D))) { goto _EXCEPT_03; }

	/**--------------------- Check the Validate data value -------------------------*/

	UINT16 StartAdr = (data[1] << 8) | (data[2]);
	//Note 1 address 0

	if (!(StartAdr >= 0x0000) && (StartAdr <= 0XFFFF)) 
	{
	}

	/**--------------------- Check the Execute MB result ---------------------------*/
//_EXCEPT_01:
	return 0X01; /**< EXCEPTION 0x01 */

//_EXCEPT_02:
	return 0X02; /**< EXCEPTION 0x02 */

_EXCEPT_03:
	return 0X03; /**< EXCEPTION 0x03 */

//_EXCEPT_04:
	return 0X04; /**< EXCEPTION 0x03 */

#else
	return 0X01; /**< EXCEPTION 0x01 */

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

