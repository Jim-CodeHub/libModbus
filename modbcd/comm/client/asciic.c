/**-----------------------------------------------------------------------------------------------------------------
 * @file	asciic.c
 * @brief	Modbus ASCII mode client/master handler 
 * @note	Bit per Byte : 
 *					1     start  bit
 *					7	  data   bit, least significant bit (LSB) sent first
 *					1 / 0 parity bit (default EVEN)
 *					1 / 2 stop   bit
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <modbcd/comm/client/asciic.h>


#if  ((MBCD_CFG_MS_SEL == 1) && (MBCD_CFG_MOD_ASCII_EN > 0)) //Master and ascii mode enabled

/*
--------------------------------------------------------------------------------------------------------------------
*
*											  VARIABLES DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/
static struct asciic_state stat_s = {.state = ASCIIC_STATE_IDLE};


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief		Init Modbus Ascii client 
 *	@param[in]	buff		-   Recive buffer 
 *	@param[in]	size		-   Size of recive buffer 
 *	@param[in]  recv		-	Method to recive character from RS232/RS485/USART ...
 *	@param[in]  send		-	Method to send character to RS232/RS485/USART ...
 *	@param[in]  exec		-	Method to execute action 
 *	@param[out] None	
 *	@return		None
 *	@note		1. The param 'size' SHOULD BE larger enough
 *				2. The param 'exec' SHOULD BE :
 *					a). Check whether function code and data field is valid or not
 *					b). Execute action
 *					c). Do normal/exception response (asciic_emit() can be called)
 **/
void asciic_init(unsigned char *buff, unsigned short size, pFun_recv recv, pFun_send send, pFun_exec exec)
{
	stat_s.pInit = buff;
	stat_s.pInxt = buff;

	stat_s._size = size;

	stat_s.serial_recv = recv;
	stat_s.serial_send = send;
	stat_s.serial_exec = exec;

	return;
}

/**
 *	@brief	    Recive Modbus server data
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None	
 *	@note		Put the function into the loop or interrupt depends on the implementation of 'serial_recv()'	
 **/
void asciic_recv(void)
{
	char ch = stat_s.serial_recv();

	if (':' == ch) {memset(stat_s.pInit, 0, stat_s._size); stat_s.pInxt= stat_s.pInit; stat_s.state = ASCIIC_STATE_0;}

	switch (stat_s.state)
	{
		case ASCIIC_STATE_0: /**< Recive data into buff */
			{
				switch (ch)
				{
					case 0X0D: /**< CR */ 
						stat_s.state = ASCIIC_STATE_1;
						break;
					default:
						{
							*(stat_s.pInxt) = ch; (stat_s.pInxt)++; 
						}
				}
			}
			break;
		case ASCIIC_STATE_1: /**< End recive and execute action */
			{
				switch (ch)
				{
					case 0X0A: /**< LF */ 
						{
							stat_s.serial_exec(&stat_s);
							stat_s.state = ASCIIC_STATE_IDLE;
						}
						break;
					default:
						;
				}
			}
			break;
		default: /**< ASCII_STATE_IDLE */
			;
	}

	return;
}

/**
 *	@brief	    Send Modbus data to server 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None	
 *	@note	    1. The data SHOULD HAD BE setted by ascii frame function		
 *				2. ***Only loop mode is supported
 **/
void asciic_emit(unsigned char *data, unsigned short size)
{
	for (; size != 0; size--)
	{
		stat_s.serial_send(*data); data++;
	}

	return;
}

#endif //((MBCD_CFG_MS_SEL == 1) && (MBCD_CFG_MOD_ASCII_EN > 0))

