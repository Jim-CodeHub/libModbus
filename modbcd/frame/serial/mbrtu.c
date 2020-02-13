/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbrtu.c
 * @brief	packetize and depacketize Modbus RTU frame
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <modbcd/frame/serial/mbrtu.h>


#if  MBCD_CFG_MOD_RTU_EN  >  0 

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
 *	@brief	    Set Modbus RTU Frame (to send buff)
 *	@param[in]  address	- slave address 
 *	@param[in]  funCode	- function code
 *	@param[in]  data    - subcode and/or other info data buff
 *	@param[in]  size    - data size 
 *	@param[out] None
 *	@return		RTU Frame 
 *	@note		Stack map : address + funCode + data + crc 
 **/
struct mb_rtu_frame mbrtu_set_frame(unsigned char address, unsigned char funCode, unsigned char *data, unsigned short size)
{
	if (size > 252) { size = 252; } /**< Max size of RTU data = 252Bytes */

	unsigned char buff[size + 2]; 

	buff[0] = address;
	buff[1] = funCode;

	memcpy(buff + 2, data, size);

	UINT16 crc = mbcrc(buff, size + 2);

	crc = ((crc >> 8) | (crc << 8)); /**< Switch Hi - Low Byte */

	struct mb_rtu_frame frame = {.address = address, .funCode = funCode, .pData = data, .crc = crc, ._size = size};

	return frame;
}

/**
 *	@brief	    Get Modbus RTU Frame (from recive buff)
 *	@param[in]  data - recive buff 
 *	@param[in]  size - data size
 *	@param[out] None
 *	@return		RTU Frame 
 *	@note		Stack map : address + funCode + data + crc 
 **/
struct mb_rtu_frame mbrtu_get_frame(unsigned char *data, unsigned short size)
{
	if (size > 256) { size = 256; } /**< Max size of RTU frame = 256Bytes */

	UINT16 crc = mbcrc(data, size - 2);

	crc = ((crc >> 8) | (crc << 8)); /**< Switch Hi - Low Byte */

	struct mb_rtu_frame frame = {.address = data[0], .funCode = data[1], .pData = data + 2, .crc = crc, ._size = size - 4};

	return frame;
}

#endif //MBCD_CFG_MOD_RTU_EN  >  0 


