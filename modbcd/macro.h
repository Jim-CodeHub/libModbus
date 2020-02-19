/**-----------------------------------------------------------------------------------------------------------------
 * @file	macro.h
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __MODBCD_MACRO_H__
#define __MODBCD_MACRO_H__


#if defined(__cplusplus)
extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *										SHORT ALIAS FOR FUNCTION CODE 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
															/*------------------- Bit data access ----------------*/
#define  MBCD_CODE_RD_DISCRETE_INPUT				0X02	/*													  */
#define  MBCD_CODE_RD_COILS							0X01	/* 										  			  */
#define  MBCD_CODE_WR_SINGLE_COIL					0X05	/* 										  			  */
#define  MBCD_CODE_WR_MULTIPLE_COILS				0x0F	/* 										  			  */

															/*------------------- Word data access ---------------*/
#define  MBCD_CODE_RD_INPUT_REGISTER				0X04	/*              									  */
#define  MBCD_CODE_RD_HOLDING_REGISTERS				0X03	/*              									  */
#define  MBCD_CODE_WR_SINGLE_REGISTER				0X06	/*              									  */
#define  MBCD_CODE_WR_MULTIPLE_REGISTERS			0x10	/*              									  */
#define  MBCD_CODE_RW_MULTIPLE_REGISTERS			0x17	/*              									  */
#define  MBCD_CODE_MASK_WRITE_REGISTER				0x16	/*              									  */
#define  MBCD_CODE_RD_FIFO_QUEUE					0x18	/*              									  */

															/*------------------- File record access -------------*/
#define  MBCD_CODE_RD_FILE_RECORD					0X14	/*													  */
#define  MBCD_CODE_WR_FILE_RECORD					0X15	/* 													  */

															/*------------------- Diagnostics --------------------*/
#define  MBCD_CODE_RD_EXCEPTION_STATUS				0X07	/*													  */
#define  MBCD_CODE_DIAGNOSTIC						0X08	/* 											 		  */
#define  MBCD_CODE_GET_COM_EVENT_COUNTER			0X0B	/* 											 		  */
#define  MBCD_CODE_GET_COM_EVENT_LOG				0X0C	/* 											 		  */
#define  MBCD_CODE_REPORT_SERVER_ID					0X11	/* 											 		  */
#define  MBCD_CODE_RD_DEVICE_IDENTIFICATION			0x2B	/* SUB - 0X0E								 		  */

															/*------------------- Diagnostics(0X08) Sub code -----*/
#define  MBCD_CODE_RET_QUERY_DATA					0x00	/*													  */
#define  MBCD_CODE_RET_RST_COMM_OPT					0X01	/* 													  */
#define  MBCD_CODE_RET_DIAGNOSTIC_REG				0X02	/* 													  */
#define  MBCD_CODE_CH_ASCII_INPUT_DELIMITER			0X03	/* 													  */
#define  MBCD_CODE_FORCE_LISTERN_ONLY_MODE			0X04	/* 													  */
#define  MBCD_CODE_CLR_CNT_DIG_REG					0X0A	/* 													  */
#define  MBCD_CODE_RET_BUS_MSG_CNT					0X0B	/* 													  */
#define  MBCD_CODE_RET_BUS_COMM_ERR_CNT				0X0C	/* 													  */
#define  MBCD_CODE_RET_BUS_EXCEP_ERR_CNT			0x0D	/* 													  */
#define  MBCD_CODE_RET_SERVER_MSG_CNT				0x0E	/* 													  */
#define  MBCD_CODE_RET_SERVER_NO_RSP_CNT			0x0F	/* 													  */
#define  MBCD_CODE_RET_SERVER_NAK_CNT				0x10	/* 													  */
#define  MBCD_CODE_RET_SERVER_BUSY_CNT				0x11	/* 													  */
#define  MBCD_CODE_RET_BUS_CHAR_OVERRUN_CNT			0x12	/* 													  */
#define  MBCD_CODE_CLR_OVERRUN_CNT_FLG				0x14	/* 													  */

															/*------------------- Others -------------------------*/
#define  MBCD_CODE_ENCAP_INTERFACE_TRANSPORT		0X2B	/* SUB - 0X0D and 0X0E		   						  */

															/*------------------- CANOPEN ------------------------*/
#define  MBCD_CODE_CANOPEN_REFERENCE				0X2B	/* SUB - 0XD										  */

															/*------------------- Others And CANOPEN Sub code ----*/
#define  MBCD_CODE_CANOPEN_REFER_PDU				0X0D    /*													  */	
#define  MBCD_CODE_RD_DEVICE_ID						0X0E	/*													  */


/*------------------------------------------------------------------------------------------------------------------
 * 
 *									SHORT ALIAS FOR EXCEPTION RESPONSE CODE 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  MBCD_EXPT_ILLEGAL_FUNCTION					0X01
#define  MBCD_EXPT_ILLEGAL_DATA_ADDRESS				0X02
#define  MBCD_EXPT_ILLEGAL_DATA_VALUE				0X03
#define  MBCD_EXPT_SERVER_DEVICE_FAILURE			0x04
#define	 MBCD_EXPT_ACKNOWLEDGE						0x05
#define  MBCD_EXPT_SERVER_DEVICE_BUSY				0X06
#define	 MBCD_EXPT_MEMORY_PARITY_EEROR				0x08	
#define  MBCD_EXPT_GATEWAY_PATH_UNAVAILABLE			0X0A	
#define	 MBCD_EXPT_GATEWAY_TARGET_DEVICE_RSP_ERR	0X0B


/*------------------------------------------------------------------------------------------------------------------
 * 
 *										 SHORT ALIAS FOR PDU LENGTH
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  MBCD_PDUL_ASCII_MIN						3		/**< Parsed 6/2										  */
#define  MBCD_PDUL_ASCII_MAX						255     /**< Parsed 510/2									  */
#define  MBCD_PDUL_RTU_MIN							4		/**<												  */
#define  MBCD_PDUL_RTU_MAX							256		/**<												  */
#define  MBCD_PDUL_TCP_MIN							8		/**< MBAP + FunCode									  */
#define  MBCD_PDUL_TCP_MAX							260		/**<												  */


/*------------------------------------------------------------------------------------------------------------------
 * 
 *										 SHORT ALIAS MESSAGE EVENT 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  MBCD_EMSG_RECV_OK							1
#define  MBCD_EMSG_SEND_OK							2


#if defined(__cplusplus)
}
#endif


#endif /*__MODBCD_MACRO_H__*/


