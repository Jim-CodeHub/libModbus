/**-----------------------------------------------------------------------------------------------------------------
 * @file	asciid.c
 * @brief	Modbus ASCII mode server/slave handler 
 * @note	Bit per Byte : 
 *					1     start  bit
 *					7	  data   bit, least significant bit (LSB) sent first
 *					1 / 0 parity bit (default EVEN)
 *					1 / 2 stop   bit
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <modbcd/comm/server/asciid.h>


#if  ((MBCD_CFG_MS_SEL == 0) && (MBCD_CFG_MOD_ASCII_EN > 0)) //Slave and ascii mode enabled

/*
--------------------------------------------------------------------------------------------------------------------
*
*											  VARIABLES DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/
static struct asciid_state stat_s = {.state = ASCIID_STATE_IDLE};


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief	    Recive Modbus client data
 *	@param[in]  ch		-	character which get from client 
 *	@param[in]  buff	-   user's data buffer	
 *	@param[in]  size	-   buffer size 
 *	@param[out] buff 
 *	@return		Function Code or 0 (invalid)
 **/
unsigned char asciid_recv(char ch, unsigned char *buff, unsigned short size)
{
	if (':' == ch) { memset(buff, 0, _size); stat_s.pInxt = buff; stat_s.state = ASCIID_STATE_0; }

	switch (stat_s.state)
	{
		case ASCIID_STATE_0: /**< Check address field */
			{
				/**< Quick address field (the first byte) handler */
				stat_s.state = ((0 != ch) && (MBCD_CFG_ADDRESS != ch))?ASCIID_STATE_IDLE:ASCIID_STATE_1;
			}
			break;
		case ASCIID_STATE_1: /**< Recive data into buff */
			{
				switch (ch)
				{
					case 0X0D: /**< CR */ 
						stat_s.state = ASCIID_STATE_2;
						break;
					default:
						{
							*(stat_s.pInxt) = ch; (stat_s.pInxt)++; 
						}
				}
			}
			break;
		case ASCIID_STATE_2: /**< End recive and execute action */
			{
				switch (ch)
				{
					case 0X0A: /**< LF */ 
						{
							//*(stat_s.pInxt) = ch; (stat_s.pInxt)++; 

							stat_s.state = ASCIID_STATE_IDLE;
							goto _EXIT;
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

	return 0;
_EXIT:
	return ((buff[2] - 48) << 4) | (buff[3] - 48);
}

/**
 *	@brief	    Response Modbus data to client
 *	@param[in]  sndc	-	character send function 
 *	@param[in]  data	-	send buffer 
 *	@param[in]  size	-	buffer size 
 *	@param[out] None
 *	@return		None	
 *	@note	    1. The data SHOULD BE setted by ascii frame function		
 *				2. ***Only loop mode is supported
 **/
void asciid_emit(pFun_send sndc, unsigned char *data, unsigned short size)
{
	for (; size != 0; size--) { sndc(*data); data++; }

	return;
}


#endif //((MBCD_CFG_MS_SEL == 0) && (MBCD_CFG_MOD_ASCII_EN > 0))

