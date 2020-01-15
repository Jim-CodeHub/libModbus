/**-----------------------------------------------------------------------------------------------------------------
 * @file	frame.h
 * @brief	packetize and depacketize Modbus message
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __MODBUSCD_FRAME_H__
#define __MODBUSCD_FRAME_H__


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
#define  FUNCODE_RD_DISCRETE_INPUT				0X02	/* Bit  data access										  */
#define  FUNCODE_RD_COILS						0X01	/* Bit  data access										  */
#define  FUNCODE_WR_SINGLE_COIL					0X05	/* Bit  data access										  */
#define  FUNCODE_WR_MULTIPLE_COILS				0x0F	/* Bit  data access										  */

#define  FUNCODE_RD_INPUT_REGISTER				0X04	/* Word data access										  */
#define  FUNCODE_RD_HOLDING_REGISTERS			0X03	/* Word data access										  */
#define  FUNCODE_WR_SINGLE_REGISTER				0X06	/* Word data access										  */
#define  FUNCODE_WR_MULTIPLE_REGISTERS			0x10	/* Word data access										  */
#define  FUNCODE_RW_MULTIPLE_REGISTERS			0x17	/* Word data access										  */
#define  FUNCODE_MASK_WRITE_REGISTER			0x16	/* Word data access										  */
#define  FUNCODE_RD_FIFO_QUEUE					0x18	/* Word data access										  */

#define  FUNCODE_RD_FILE_RECORD					0X14	/* File record access									  */
#define  FUNCODE_WR_FILE_RECORD					0X15	/* File record access									  */

#define  FUNCODE_RD_EXCEPTION_STATUS			0X07	/* Diagnostics											  */
#define  FUNCODE_DIAGNOSTIC						0X08	/* Diagnostics											  */

#define  FUNCODE_S8_RETURN_QUERY_DATA			0x00	/* Diagnostics											  */
#define  FUNCODE_S8_RETURN_RST_COMM_OPT			0X01	/* Diagnostics											  */
#define  FUNCODE_S8_RETURN_DIAGNOSTIC_REG		0X02	/* Diagnostics											  */
#define  FUNCODE_S8_CH_ASCII_INPUT_DELIMITER	0X03	/* Diagnostics											  */
#define  FUNCODE_S8_FORCE_LISTERN_ONLY_MODE		0X04	/* Diagnostics											  */
#define  FUNCODE_S8_CLR_CNT_DIG_REG				0X0A	/* Diagnostics											  */
#define  FUNCODE_S8_RET_BUS_MSG_CNT				0X0B	/* Diagnostics											  */
#define  FUNCODE_S8_RET_BUS_COMM_ERR_CNT		0X0C	/* Diagnostics											  */
#define  FUNCODE_S8_RET_BUS_EXCEP_ERR_CNT		0x0D	/* Diagnostics											  */
#define  FUNCODE_S8_RET_SERVER_MSG_CNT			0x0E	/* Diagnostics											  */
#define  FUNCODE_S8_RET_SERVER_NO_RSP_CNT		0x0F	/* Diagnostics											  */
#define  FUNCODE_S8_RET_SERVER_NAK_CNT			0x10	/* Diagnostics											  */
#define  FUNCODE_S8_RET_SERVER_BUSY_CNT			0x11	/* Diagnostics											  */
#define  FUNCODE_S8_RET_BUS_CHAR_OVERRUN_CNT	0x12	/* Diagnostics											  */
#define  FUNCODE_S8_CLR_OVERRUN_CNT_FLG			0x14	/* Diagnostics											  */

#define  FUNCODE_GET_COM_EVENT_COUNTER			0X0B	/* Diagnostics											  */
#define  FUNCODE_GET_COM_EVENT_LOG				0X0C	/* Diagnostics											  */
#define  FUNCODE_REPORT_SERVER_ID				0X11	/* Diagnostics											  */
#define  FUNCODE_RD_DEVICE_IDENTIFICATION		0x2B	/* Diagnostics(SUB - 0X0E)								  */

#define  FUNCODE_ENCAP_INTERFACE_TRANSPORT		0X2B	/* Other(SUB - 0X0D and 0X0E)							  */

#define  FUNCODE_S2B_CANOPEN_REFER_PDU			0X0D	
#define  FUNCODE_S2B_RD_DEVICE_ID				0X0E	

#define  FUNCODE_CANOPEN_REFERENCE				0X2B	/* CANOPEN(SUB - 0XD)									  */


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


#endif /*__MODBUSCD_FRAME_H__*/

