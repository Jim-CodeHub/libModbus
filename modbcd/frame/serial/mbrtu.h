/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbrtu.h
 * @brief	packetize and depacketize Modbus RTU frame
 *
 * Copyright (c) 2019-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __MODBCD_MBRTU_H__
#define __MODBCD_MBRTU_H__


#if defined(__cplusplus)
extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MBRTU INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <string.h>
#include <modbcd/util/mbcrc.h>


#if  MBCD_CFG_MOD_RTU_EN  >  0 

/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MBRTU DATABLOCK 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
struct mb_rtu_frame{
	unsigned char	address;
	unsigned char	funCode;
	unsigned char  *pData  ;
	UINT16			crc	   ;

	unsigned short  _size  ; /**< pData size */
};

/*------------------------------------------------------------------------------------------------------------------
 * 
 *											 FUNCTIONS DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
struct mb_rtu_frame mbrtu_set_frame(unsigned char address, unsigned char funCode, unsigned char *data, unsigned short size);
struct mb_rtu_frame mbrtu_get_frame(unsigned char *data, unsigned short size);


#endif//  MBCD_CFG_MOD_RTU_EN  >  0 


#if defined(__cplusplus)
}
#endif


#endif /*__MODBCD_MBRTU_H__*/


