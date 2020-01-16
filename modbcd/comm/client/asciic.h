/**-----------------------------------------------------------------------------------------------------------------
 * @file	asciic.h
 * @brief	Modbus ASCII mode client/master handler 
 * @note	Bit per Byte : 
 *					1     start  bit
 *					7	  data   bit, least significant bit (LSB) sent first
 *					1 / 0 parity bit
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


#if  ((MBCD_CFG_MS_SEL == 0) && (MBCD_CFG_MODE_ASCII_EN > 0)) //Master and ascii mode enabled

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
	
struct asciic_state; /**< Forward declaration */

typedef unsigned char (*pFun_recv)(void); 
typedef void		  (*pFun_exec)(const struct asciic_state *); 

struct asciic_state{
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
void asciic_init(unsigned char *buff, unsigned short size, pFun_recv recv, pFun_exec exec);
void asciic_recv(void);


#endif //((MBCD_CFG_MS_SEL == 1) && (MBCD_CFG_MODE_ASCII_EN > 0))

#if defined(__cplusplus)
}
#endif


#endif /*__MODBCD_ASCIIC_H__*/


