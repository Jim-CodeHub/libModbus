/**-----------------------------------------------------------------------------------------------------------------
 * @file	asciic.h
 * @brief	Modbus ASCII mode client/master handler 
 * @note	Bit per Byte : 
 *					1     start  bit
 *					7	  data   bit, least significant bit (LSB) sent first
 *					1 / 0 parity bit (default EVEN)
 *					1 / 2 stop   bit
 *
 * Copyright (c) 2019-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __MODBCD_ASCIIC_H__
#define __MODBCD_ASCIIC_H__


#if defined(__cplusplus)
extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												ASCIIC INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <string.h>
#include <modbcd/config.h>


#if  ((MBCD_CFG_MS_SEL == 1) && (MBCD_CFG_MOD_ASCII_EN > 0)) //Master and ascii mode enabled

/*------------------------------------------------------------------------------------------------------------------
 * 
 *												ASCIIC SHORT ALIAS 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  ASCIIC_STATE_0							0
#define  ASCIIC_STATE_1							1
#define  ASCIIC_STATE_2							2
#define  ASCIIC_STATE_3							3
#define  ASCIIC_STATE_4							4
#define  ASCIIC_STATE_IDLE					    255	


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												ASCIIC DATABLOCK 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
	
typedef void (*pFun_send)(unsigned char); 

struct asciic_state{
	unsigned char   state;
	unsigned char  *pInxt;
};


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											 FUNCTIONS DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/

unsigned char asciic_recv(char ch, unsigned char *buff, unsigned short size);
void asciic_emit(pFun_send sndc, unsigned char *data, unsigned short size);


#endif //((MBCD_CFG_MS_SEL == 1) && (MBCD_CFG_MOD_ASCII_EN > 0))

#if defined(__cplusplus)
}
#endif


#endif /*__MODBCD_ASCIIC_H__*/


