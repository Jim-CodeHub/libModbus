/**-----------------------------------------------------------------------------------------------------------------
 * @file	modbcd.c
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <modbcd/modbcd.h>


/*
--------------------------------------------------------------------------------------------------------------------
*
		                                      FUNCTION PROTOTYPES
*
--------------------------------------------------------------------------------------------------------------------
*/
static struct mbcd_server_block stat_s = { .state = MBCD_STATE_0 };


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief	    
 *	@param[in]  
 *	@param[out]
 *	@return		
 *	@note		
 **/
void mbcd_recv_rqst(pFun_recv getc, unsigned char *buff)
{
	unsigned short size = 0;

	switch (stat_s.state)
	{
		case MBCD_STATE_0: /**< Server Com Poll - Recive CR */
			{
				stat_s.state = (1 == asciid_recv(getc(), buff, &size))?MBCD_STATE_1:MBCD_STATE_0;
			}
			break;
		case MBCD_STATE_1: /**< Parse Frame	- Recive LF */
			{
				stat_s.state = MBCD_STATE_0;

				if (2 == asciid_recv(getc(), buff, &size))
				{
					struct mb_ascii_frame_recv frameRcv = mbascii_get_frame(buff, size);

					if ((0 == frameRcv.address) || (MBCD_CFG_ADDRESS == frameRcv.address)) /**< Check Address */
					{
						if (frameRcv.lrc_src == frameRcv.lrc_cal) /**< Check LRC */
						{
							stat_s.state = MBCD_STATE_IDLE;

							switch (frameRcv.funCode)
							{
								case MBCD_FCODE_RD_DISCRETE_INPUT:
									//if (MBCD_CFG_COD_RD_DISCRETE_INPUTS_EN <= 0)
									break;
								case MBCD_FCODE_RD_COILS:
									mb_read_coils(frameRcv.pData);
									break;
								case MBCD_FCODE_WR_SINGLE_COIL:
									//if ((MBCD_CFG_COD_WR_SINGLE_COIL_EN <= 0) && (0 != frameRcv.address))
									break;
								default:
									;
							}
						}
					}
				}
			}
			break;
		default: /**< MBCD_STATE_IDLE and other state */
			;
	}

	return;
}

void mbcd_send_rsps(pFun_send putc, unsigned char *buff )
{
	switch (stat_s.state)
	{
		case MBCD_STATE_6:
			break;
		case MBCD_STATE_9: /**< Exception */
			{}
			break;
		default:
			;
	}


// struct mb_ascii_frame_send mbascii_set_frame(MBCD_CFG_ADDRESS, char funCode, unsigned char *data, unsigned short size)

//	mbascii_set_frame();
//	asciic_emit();
}


#if 0
//Master/Client
mbcd_send_rqst();
mbcd_recv_rsps();

//Slave/Server
mbcd_recv_rqst();
mbcd_send_rsps();


#endif

