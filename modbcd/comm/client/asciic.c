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
 *	@brief	    Recive Modbus server data
 *	@param[in]  ch		-	character which get from client 
 *	@param[out] buff	-   user's data buffer
 *	@param[out] size	-   frame size 
 *	@return	    1(Frame has been recived - CR) / 2(End recive - LF) / 0
 **/
unsigned char asciic_recv(char ch, unsigned char *buff, unsigned short *size)
{
	if (':' == ch) { stat_s.pInxt = buff; stat_s.state = ASCIIC_STATE_0; }

	switch (stat_s.state)
	{
		case ASCIIC_STATE_0: /**< Recive data into buff */
			{
				switch (ch)
				{
					case 0X0D: /**< CR */ 
						{
							stat_s.state = ASCIIC_STATE_1;

							goto _EXIT_CR;
						}
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
							*size = stat_s.pInxt - buff;

							stat_s.state = ASCIIC_STATE_IDLE;

							goto _EXIT_LF;
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
_EXIT_CR:
	return 1; 
_EXIT_LF:
	return 2;
}

/**
 *	@brief	    Send Modbus data to server 
 *	@param[in]  sndc	-	character send function 
 *	@param[in]  data	-	send buffer 
 *	@param[in]  size	-	buffer size 
 *	@param[out] None
 *	@return		None	
 *	@note	    1. The data SHOULD BE setted by ascii frame function		
 *				2. ***Only loop mode is supported
 **/
void asciic_emit(pFun_send sndc, unsigned char *data, unsigned short size)
{
	for (; size != 0; size--) { sndc(*data); data++; }

	return;
}


#endif //((MBCD_CFG_MS_SEL == 1) && (MBCD_CFG_MOD_ASCII_EN > 0))

