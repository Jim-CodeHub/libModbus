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
 *										  MODBUS/CRC CONFIGURE MACRO 
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
 *										MODBUS/FRAME INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <stddef.h>
#include <string.h>


/*------------------------------------------------------------------------------------------------------------------
 * 
 *										MODBUS/FRAME DATA BLOCK
 *
 *------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief CRC structrue with Modbus RTU CRC sequence 
 **/
struct frame_crc{
	unsigned char crcLo;
	unsigned char crcHi;
};

/**
 *	@brief LRC structrue with Modbus RTU ASC sequence 
 **/
struct frame_lrc{
	//TBD
};

struct frame_rtu{
	unsigned char address;
	unsigned char funCode;
	unsigned char data[252];
	struct frame_crc crc; 
};

struct frame_asc{

};

//enum mode{RTU, ASCII, TCP_IP};



#if defined(__cplusplus)
}
#endif


#endif /*__MODEBUS_FRAME_H__*/

