/**-----------------------------------------------------------------------------------------------------------------
 * @file	rtud.h
 * @brief	Modbus RTU mode server/slave handler 
 * @note	Bit per Byte : 
 *					1     start  bit
 *					8	  data   bit, least significant bit (LSB) sent first
 *					1 / 0 parity bit (default EVEN)
 *					1 / 2 stop   bit
 *
 * Copyright (c) 2019-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __MODBCD_RTUD_H__
#define __MODBCD_RTUD_H__


#if defined(__cplusplus)
extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												RTUD INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <string.h>
#include <modbcd/config.h>


#if  ((MBCD_CFG_MS_SEL == 0) && (MBCD_CFG_MOD_RTU_EN > 0)) //Slave and ascii mode enabled

/*------------------------------------------------------------------------------------------------------------------
 * 
 *												RTUD SHORT ALIAS 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  RTUD_STATE_0							0
#define  RTUD_STATE_1							1
#define  RTUD_STATE_2							2
#define  RTUD_STATE_3							3
#define  RTUD_STATE_4							4
#define  RTUD_STATE_IDLE					    255	


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												RTUD DATABLOCK 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
	
struct rtud_state; /**< Forward declaration */

typedef unsigned char (*pFun_recv)(void); 
typedef void		  (*pFun_exec)(const struct rtud_state *); 

struct rtud_state{
	unsigned char   state;
	unsigned char  *pInxt;
	unsigned char  *pInit;

	unsigned short  _size;

	pFun_recv serial_recv;
	pFun_exec serial_exec;
};


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											 FUNCTIONS DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
void rtud_init(unsigned char *buff, unsigned short size, pFun_recv recv, pFun_exec exec);
void rtud_recv(void);


#endif //((MBCD_CFG_MS_SEL == 0) && (MBCD_CFG_MOD_RTU_EN > 0))

#if defined(__cplusplus)
}
#endif


#endif /*__MODBCD_RTUD_H__*/


