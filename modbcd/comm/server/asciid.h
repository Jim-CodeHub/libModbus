/**-----------------------------------------------------------------------------------------------------------------
 * @file	asciid.h
 * @brief	Modbus ASCII mode server/slave handler 
 * @note	Bit per Byte : 
 *					1     start  bit
 *					7	  data   bit, least significant bit (LSB) sent first
 *					1 / 0 parity bit (default EVEN)
 *					1 / 2 stop   bit
 *
 * Copyright (c) 2019-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __MODBCD_ASCIID_H__
#define __MODBCD_ASCIID_H__


#if defined(__cplusplus)
extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												ASCIID INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <string.h>
#include <modbcd/config.h>


#if  ((MBCD_CFG_MS_SEL == 0) && (MBCD_CFG_MOD_ASCII_EN > 0)) //Slave and ascii mode enabled

/*------------------------------------------------------------------------------------------------------------------
 * 
 *												ASCIID SHORT ALIAS 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  ASCIID_STATE_0							0
#define  ASCIID_STATE_1							1
#define  ASCIID_STATE_2							2
#define  ASCIID_STATE_3							3
#define  ASCIID_STATE_4							4
#define  ASCIID_STATE_IDLE					    255	


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												ASCIID DATABLOCK 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
	
typedef void (*pFun_send)(unsigned char); 
typedef char (*pFun_recv)(void); 

struct asciid_state{
	unsigned char   state;
	unsigned char  *pInxt;
};


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											 FUNCTIONS DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
unsigned char asciid_recv(char ch, unsigned char *buff, unsigned short *size);
void asciid_emit(pFun_send sndc, unsigned char *data, unsigned short size);


#endif //((MBCD_CFG_MS_SEL == 0) && (MBCD_CFG_MOD_ASCII_EN > 0))

#if defined(__cplusplus)
}
#endif


#endif /*__MODBCD_ASCIID_H__*/


