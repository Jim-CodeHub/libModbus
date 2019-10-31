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
 *												MODBUS/FRAME CONFIGURE MACRO 
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
 *												MODBUS/FRAME FUNCTION CODE
 *
 *------------------------------------------------------------------------------------------------------------------
*/
														/**<---------------------- Remarks -----------------------*/
#define  FUNCODE_RD_COILS						0X01	/* Bit access											  */
#define  FUNCODE_RD_DISCRETE_INPUT				0X02	/* Bit access											  */
#define  FUNCODE_RD_HOLDING_REG					0X03	/* Word access											  */
#define  FUNCODE_RD_INPUT_REG					0X04	/* Word access											  */
#define  FUNCODE_WR_SINGLE_COIL					0X05	/* Bit access											  */
#define  FUNCODE_WR_SINGLE_REG					0X06	/* Word access											  */
#define  FUNCODE_RD_EXCEPTION_STATUS			0X07
#define  FUNCODE_DIAGNOSTIC						0X08
#define  FUNCODE_GET_COM_EVENT_COUNTER			0X0B
#define  FUNCODE_GET_COM_EVENT_LOG				0X0C
#define  FUNCODE_WR_MUL_COILS					0x0F	/* Bit access											  */
#define  FUNCODE_WR_MUL_REGS					0x10	/* Word access											  */
#define  FUNCODE_REPORT_SERVER_ID				0X11
#define  FUNCODE_RD_FILE_RECORD					0X14
#define  FUNCODE_WR_FILE_RECORD					0X15
#define  FUNCODE_MASK_WR_REG					0X16
#define  FUNCODE_RD_WR_MUL_REG					0x17
#define  RUNCODE_RD_FIFO_QUEUE					0X18
#define  RUNCODE_RD_DEVICE_IDENTIFICATION		0x2B


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
struct frame_asc_adu frame_asc_set(unsigned char address, unsigned char funCode, const void *data, size_t size);


#if defined(__cplusplus)
}
#endif


#endif /*__MODEBUS_FRAME_H__*/

