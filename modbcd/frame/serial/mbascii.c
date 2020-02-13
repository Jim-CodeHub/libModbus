/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbascii.c
 * @brief	packetize and depacketize Modbus ASCII frame
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <modbcd/frame/serial/mbascii.h>


#if  MBCD_CFG_MOD_ASCII_EN  >  0

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
 *	@brief	    Set Modbus ASCII Frame (to send buff)
 *	@param[in]  address	- slave address 
 *	@param[in]  funCode	- function code
 *	@param[in]  data    - subcode and/or other info data buff
 *	@param[in]  size    - data size 
 *	@param[out] None
 *	@return		ASCII Frame 
 *	@note		1. Stack map : colon + address + funCode + data + lrc + CRLF 
 *				2. ***Param 'data' MUST BE double size of origin one 
 *	@code
 *		unsigned char data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0} //ERROR
 *		unsigned char data[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0} //RIGHT
 *	@endcode
 **/
struct mb_ascii_frame_send mbascii_set_frame(unsigned char address, unsigned char funCode, unsigned char *data, unsigned short size)
{
	struct mb_ascii_frame_send frame;

	/*------------------------------- Set Start -------------------------------------*/
	frame.colon = ':';

	if (size > 2*252) { size = 2*252; } /**< Max size of ASCII data = 2*252Bytes */

	hexToAscii(address, frame.address); /**< Set address */
	hexToAscii(funCode, frame.funCode); /**< Set funCode */

	frame._size = size;	 /**< Set data size */
	frame.pData = data;  /**< Set data pointer */

	/*------------------------------- Set LRC --------------------------------------*/

	char buff[size + 4]; 

	buff[0] = frame.address[0];
	buff[1] = frame.address[1];
	buff[2] = frame.funCode[0];
	buff[3] = frame.funCode[1];

	for (char *p = buff + 4; size != 0; size--, data++, p += 2)
	{
		hexToAscii(*data, p);
	}

	unsigned char lrc = mblrc((unsigned char *)buff, sizeof(buff));

	hexToAscii(lrc, frame.lrc);

	/*------------------------------- Set DATA -------------------------------------*/

	memcpy(frame.pData, buff + 4, sizeof(buff) - 4);

	/*------------------------------- Set End --------------------------------------*/

	frame.crlf[0] = 0X0D; /**< CR */
	frame.crlf[1] = 0X0A; /**< LF */

	return frame;
}

/**
 *	@brief	    Get Modbus ASCII Frame (from recive buff)
 *	@param[in]  data - recive buff 
 *	@param[in]  size - data size
 *	@param[out] data 
 *	@return		ASCII Frame 
 *	@note		Stack map : colon + address + funCode + data + lrc + CRLF 
 **/
struct mb_ascii_frame_recv mbascii_get_frame(unsigned char *data, unsigned short size)
{
	struct mb_ascii_frame_recv frame;

	if (size > 2*255) { size = 2*255; } /**< Max size of ASCII frame = 2*255Bytes */

	frame.lrc = mblrc(data, size - 2);

	frame.address = ((data[0] - 48) << 4) | (data[1] - 48);
	frame.funCode = ((data[2] - 48) << 4) | (data[3] - 48);

	frame._size = (size - 6) >> 1;

	size -= 6; /**< Delete address, funCode and LRC size */
	for (unsigned char *p = data + 4; size != 0; size -= 2, data++, p += 2)
	{
		*data = ((*p - 48) >> 4) | (*(p+1) - 48);
	}

	return frame;
}

#endif //MBCD_CFG_MOD_ASCII_EN  >  0

