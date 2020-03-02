/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbcrc.h
 * @brief   modbucd CRC calculate util 
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef _MBCRC_H__
#define _MBCRC_H__


#ifdef __cplusplus
	extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MODBCD INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include  "port.h"


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											MODBCD FUN DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
USHORT          usMBCRC16( UCHAR * pucFrame, USHORT usLen );


#ifdef __cplusplus
	} //extern "C"
#endif


#endif //_MBCRC_H__

