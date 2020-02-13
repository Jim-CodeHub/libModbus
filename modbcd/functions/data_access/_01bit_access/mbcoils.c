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

#if 0
/**
 *	@brief		Read coils (function code = 0x01) 
 *	@param[in]  
 *	@param[out] 
 *	@return		
 *	@note		
 **/
void mb_read_coils(struct asciid_state *st, unsigned char *buff, unsigned short _size)
{
	/**--------------------- Check the Validate function code ----------------------*/

	if (MBCD_CFG_COD_RD_COILS_EN <= 0) { goto _EXCEPT_01; }

	struct mb_ascii_frame_send snd;
	struct mb_ascii_frame_recv rcv;

	rcv = mbascii_get_frame(st->pInit, st->_size);

	/**--------------------- Check the Validate data address -----------------------*/

	UINT16 Quantity = (pData[3] << 8) | (pData[4]);

	if (!((Quantity >= 0x0001) && (Quantity <= 0X07D))) { goto _EXCEPT_03; }

	/**--------------------- Check the Validate data value -------------------------*/

	UINT16 StartAdr = (pData[1] << 8) | (pData[2]);

	if (!(StartAdr >= 0x0000) && (StartAdr <= 0XFFFF)) 
	{
		if (
	}

	{goto _EXCEPT_02}

	/**--------------------- Check the Execute MB result ---------------------------*/
	//bool fun_exect()

	unsigned char address;
	unsigned char funCode;
	unsigned char *pData ;
	unsigned char lrc    ;

	unsigned char _size  ; /**< pData size, wich has been recived */




	//pFun_recv serial_recv;
	//pFun_send serial_send;

	/**--------------------- Send Modbus Response ----------------------------------*/

_EXCEPT_01:
	snd = mbascii_set_frame(MBCD_CFG_ADDRESS, 0X01+0X80, buff, 0);
_EXCEPT_02:

_EXCEPT_03:
	snd = mbascii_set_frame(MBCD_CFG_ADDRESS, 0X03+0X80, buff, 0);

	//st->send()
	return;
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

#endif
