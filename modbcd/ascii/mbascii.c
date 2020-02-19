/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbascii.c
 * @brief	Modbus ASCII mode client/master and server/slave handler 
 * @note	Bit per Byte : 
 *					1     start  bit
 *					7	  data   bit, least significant bit (LSB) sent first
 *					1 / 0 parity bit (default EVEN)
 *					1 / 2 stop   bit
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include "mbascii.h"


#if  MBCD_CFG_MOD_ASCII_EN > 0 //Ascii mode enabled

/*
--------------------------------------------------------------------------------------------------------------------
*
*											  VARIABLES DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/
	   struct _os_mbox msgbox_asc;
static struct mbascii_state stat_s = {.state_Rx = MBASCII_STATE_NONE, .state_Tx = MBASCII_STATE_NONE, .state_HL = 0};


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief	    Recive Modbus data
 *	@param[in]  getc    -	callback function to get character
 *	@param[out] buff	-   user's data buffer
 *	@param[out] size	-   frame size 
 *	@return	    true (frame has been recived )/ false 
 **/
_BOOL mbascii_recv(SCHAR (*getc)(void), UCHAR *buff, USHRT *size)
{
	assert(MBASCII_STATE_IDLE == stat_s.state_Tx); /**< Tx SHALL BE END FIRST */

	SCHAR ch = getc();

	if (':' == ch) { stat_s.pIndex = buff; stat_s.state_Rx = MBASCII_STATE_0; stat_s.state_HL = 0; }

	switch (stat_s.state_Rx)
	{
		case MBASCII_STATE_0: /**< Recive data into buff */
			{
				switch (ch)
				{
					case ':' : break;
					case 0X0D: /**< CR */ 
						{
							stat_s.state_Rx = MBASCII_STATE_1;
						}
						break;
					default: /**< Save 2 ascii to 1 binary */
						{
							switch (stat_s.state_HL)
							{
								case 0:
									*(stat_s.pIndex)  = AscToBin(ch) << 4;
									break;
								case 1:
									*(stat_s.pIndex) |= AscToBin(ch)     ;  (stat_s.pIndex)++;
									break;
								default:
									;
							}

							stat_s.state_HL++; stat_s.state_HL %= 2;
						}
				}
			}
			break;
		case MBASCII_STATE_1: /**< End recive and execute action */
			{
				switch (ch)
				{
					case 0X0A: /**< LF */ 
						{
							*size = stat_s.pIndex - buff;

							stat_s.state_Rx = MBASCII_STATE_IDLE;

							UCHAR msg = MBCD_EMSG_RECV_OK;
							_OSMboxPost(&msgbox_asc, &msg);

							goto _EXIT_OK;
						}
						break;
					default:
						;
				}
			}
			break;
		default: /**< ASCII_STATE_IDLE & ASCII_STATE_NONE */
			;
	}

	return false;
_EXIT_OK:
	return true;
}

/**
 *	@brief	    Send Modbus data
 *	@param[in]  putc    -	callback function to put character
 *	@param[in]  data	-	send buffer 
 *	@param[in]  size	-	buffer size 
 *	@param[out] None
 *	@return		true (frame has been sent)/ false	
 **/
_BOOL mbascii_emit(void (*putc)(SCHAR ch), UCHAR *data, USHRT size)
{
	assert(MBASCII_STATE_IDLE == stat_s.state_Rx); /**< Rx SHALL BE END FIRST */

	switch (stat_s.state_Tx)
	{
		case MBASCII_STATE_0: /**< Send ch ':' */
			{
				stat_s.pIndex   = data;
				stat_s.state_HL = 0;

				putc(':');

				stat_s.state_Tx = MBASCII_STATE_1;
			}
			break;
		case MBASCII_STATE_1: /**< Send blocks */
			{
				if ((stat_s.pIndex - data) <= size)
				{
					switch (stat_s.state_HL)
					{
						case 0:
							putc(BinToAsc(*(stat_s.pIndex) >> 4  ));
							break;
						case 1:
							putc(BinToAsc(*(stat_s.pIndex) & 0X0F)); (stat_s.pIndex)++;
							break;
						default:
							;
					}

					stat_s.state_HL++; stat_s.state_HL %= 2;
				}
				else
				{
					putc(0X0D); /**< CR */

					stat_s.state_Tx = MBASCII_STATE_2;
				}
			}
			break;
		case MBASCII_STATE_2: /**< Send ch 'LF' */
			{
				putc(0X0A); /**< LF */ 

				stat_s.state_Tx = MBASCII_STATE_IDLE;

				UCHAR msg = MBCD_EMSG_SEND_OK;
				_OSMboxPost(&msgbox_asc, &msg);

				goto _EXIT_OK;
			}
			break;
		default: /**< ASCII_STATE_IDLE & ASCII_STATE_NONE */
			;
	}

	return false;
_EXIT_OK:
	return true;
}

#endif //MBCD_CFG_MOD_ASCII_EN > 0


