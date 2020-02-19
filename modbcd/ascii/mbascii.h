/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbascii.h
 * @brief	Modbus ASCII mode client/master and server/slave handler 
 * @note	Bit per Byte : 
 *					1     start  bit
 *					7	  data   bit, least significant bit (LSB) sent first
 *					1 / 0 parity bit (default EVEN)
 *					1 / 2 stop   bit
 *
 * Copyright (c) 2019-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __MODBCD_MBASCII_H__
#define __MODBCD_MBASCII_H__


#if defined(__cplusplus)
extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MBASCII INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include "../config.h"
#include "../macro.h"
#include "../util/trans.h"
#include "../util/msgbox.h"
#include <assert.h>


#if  MBCD_CFG_MOD_ASCII_EN > 0 //Ascii mode enabled

/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MBASCII SHORT ALIAS 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  MBASCII_STATE_NONE							0
#define  MBASCII_STATE_0							1
#define  MBASCII_STATE_1							2
#define  MBASCII_STATE_2							3
#define  MBASCII_STATE_3							4
#define  MBASCII_STATE_4							5
#define  MBASCII_STATE_IDLE					        255	


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MBASCII DATABLOCK 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
struct mbascii_state{
	UCHAR   state_Rx;
	UCHAR   state_HL; /**< High character & Low character state */
	UCHAR   state_Tx;

	UCHAR  *pIndex;
};


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											 FUNCTIONS DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
_BOOL mbascii_emit(void  (*putc)(SCHAR ch), UCHAR *data, USHRT  size);
_BOOL mbascii_recv(SCHAR (*getc)(void)    , UCHAR *buff, USHRT *size);


#endif //MBCD_CFG_MOD_ASCII_EN > 0


#if defined(__cplusplus)
}
#endif


#endif /*__MODBCD_MBASCII_H__*/


