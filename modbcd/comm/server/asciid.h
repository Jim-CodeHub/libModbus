/**-----------------------------------------------------------------------------------------------------------------
 * @file	asciid.h
 * @brief	Modbus ASCII mode server/slave handler 
 * @note	Bit per Byte : 
 *					1     start  bit
 *					7	  data   bit, least significant bit (LSB) sent first
 *					1 / 0 parity bit
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


#if  ((MBCD_CFG_MS_SEL == 0) && (MBCD_CFG_MODE_ASCII_EN > 0)) //Slave and ascii mode enabled

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
	
struct asciid_state; /**< Forward declaration */

typedef unsigned char (*pFun_recv)(void); 
typedef void		  (*pFun_exec)(const struct asciid_state *); 

struct asciid_state{
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
void asciid_init(unsigned char *buff, unsigned short size, pFun_recv recv, pFun_exec exec);
void asciid_recv(void);


#endif //((MBCD_CFG_MS_SEL == 0) && (MBCD_CFG_MODE_ASCII_EN > 0))

#if defined(__cplusplus)
}
#endif


#endif /*__MODBCD_ASCIID_H__*/


