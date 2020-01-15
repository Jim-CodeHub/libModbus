/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbrtu.c
 * @brief	packetize and depacketize Modbus RTU frame
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <modbcd/frame/serial/mbrtu.h>


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
 *	@brief	    Set Modbus RTU Frame 
 *	@param[in]  address	- slave address 
 *	@param[in]  funCode	- function code
 *	@param[in]  data    - subcode and/or other info data buff
 *	@param[in]  size    - data size 
 *	@param[out] None
 *	@return		RTU Frame 
 *	@note		The function do not have volidity check for param 'data' 
 **/
struct mb_rtu_frame mbrtu_set_frame(unsigned char address, unsigned char funCode, unsigned char *data, unsigned short size)
{
	if (size > 252) { size = 252; } /**< Max size of RTU data = 252Bytes */

	UINT16 crc = mbcrc(data, size);

	struct mb_rtu_frame frame = {.address = address, .funCode = funCode, .pData = data, ._size = size, .crc = crc};

	return frame;
}

