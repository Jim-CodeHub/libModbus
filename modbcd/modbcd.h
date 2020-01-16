/**-----------------------------------------------------------------------------------------------------------------
 * @file	modbcd.h
 *
 * Copyright (c) 2019-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __MODBCD_H__
#define __MODBCD_H__


#if defined(__cplusplus)
extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MODBCD INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <modbcd/util/trans.h>
#include <modbcd/util/mblrc.h>
#include <modbcd/util/mbcrc.h>
#include <modbcd/frame/serial/mbrtu.h>
#include <modbcd/frame/serial/mbascii.h>
#include <modbcd/frame/frame.h>


/*------------------------------------------------------------------------------------------------------------------
 * 
 *									MODBCD SHORT ALIAS FOR EXCEPTION RESPONSE CODE 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  MBCD_ILLEGAL_FUNCTION							0X01
#define  MBCD_ILLEGAL_DATA_ADDRESS						0X02
#define  MBCD_ILLEGAL_DATA_VALUE						0X03
#define  MBCD_SERVER_DEVICE_FAILURE						0x04
#define	 MBCD_ACKNOWLEDGE								0x05
#define  MBCD_SERVER_DEVICE_BUSY						0X06
#define	 MBCD_MEMORY_PARITY_EEROR						0x08	
#define  MBCD_GATEWAY_PATH_UNAVAILABLE					0X0A	
#define	 MBCD_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPONSE	0X0B


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												FUNCTIONS DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/


#if defined(__cplusplus)
}
#endif


#endif /*__MODBCD_H__*/


