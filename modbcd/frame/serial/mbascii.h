/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbascii.h
 * @brief	packetize and depacketize Modbus ASCII frame
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
#include <string.h>
#include <modbcd/util/mblrc.h>
#include <modbcd/util/trans.h>


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MBASCII DATABLOCK 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
struct mb_ascii_frame_send{
	char		    colon;
	char	   address[2];
	char	   funCode[2];
	unsigned char *pData ;
	char		   lrc[2];
	char		  crlf[2];

	unsigned short  _size; /**< pData size */
};

struct mb_ascii_frame_recv{
	unsigned char address;
	unsigned char funCode;
	unsigned char *pData ;
	unsigned char lrc    ;

	unsigned char _size  ; /**< pData size, wich has been recived */
};

/*------------------------------------------------------------------------------------------------------------------
 * 
 *											 FUNCTIONS DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
struct mb_ascii_frame_send mbascii_set_frame(unsigned char address, unsigned char funCode, unsigned char *data, unsigned short size);
struct mb_ascii_frame_recv mbascii_get_frame(unsigned char *data, unsigned short size);


#if defined(__cplusplus)
}
#endif


#endif /*__MODBCD_MBASCII_H__*/


