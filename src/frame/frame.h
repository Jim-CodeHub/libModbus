/**-----------------------------------------------------------------------------------------------------------------
 * @file	frame.h
 * @brief	packetize and depacketize Modbus message
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __MODEBUS_FRAME_H__
#define __MODEBUS_FRAME_H__


#if defined(__cplusplus)
extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MODBUS/FRAME INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <stddef.h>
#include <string.h>


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MODBUS/CRC CONFIGURE MACRO 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  CRC_ARR_HI(INDEX)	auchCRCHi[INDEX] 
#define  CRC_ARR_LO(INDEX)	auchCRCLo[INDEX]

#define  CRC_FLS_P1
#define  CRC_FLS_P2
#define  CRC_FLS_P3


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MODBUS/FRAME RTU DATA BLOCK
 *
 *------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief CRC checking part for RTU 
 **/
struct frame_rtu_chk{
	unsigned char address;
	unsigned char funCode;
	unsigned char data[252];
};

/**
 *	@brief CRC structure with Modbus RTU CRC sequence 
 **/
struct frame_rtu_crc{
	unsigned char crcLo;
	unsigned char crcHi;
};

/**
 *	@brief RTU frame (ADU) structure
 **/
struct frame_rtu_adu{
	struct frame_rtu_chk chk;
	struct frame_rtu_crc crc; 
};


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MODBUS/FRAME ASCII DATA BLOCK
 *
 *------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief LRC checking part for ASCII 
 **/
struct frame_asc_chk{
	char address[2];
	char funCode[2];
	char data[2*252];
};

/**
 *	@brief LRC structure with Modbus RTU ASCII sequence 
 **/
struct frame_asc_lrc{
	char lrcHi;
	char lrcLo;
};

/**
 *	@brief ASCII frame (ADU) structure
 **/
struct frame_asc_adu{
	struct frame_asc_chk chk;
	struct frame_asc_lrc lrc; 
};


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												FUNCTIONS DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
struct frame_rtu_adu frame_rtu_set(unsigned char address, unsigned char funCode, const void *data, size_t size);


#if defined(__cplusplus)
}
#endif


#endif /*__MODEBUS_FRAME_H__*/

