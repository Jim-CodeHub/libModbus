/**-----------------------------------------------------------------------------------------------------------------
 * @file	config.h
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __MODBCD_CONFIG_H__
#define __MODBCD_CONFIG_H__


#if defined(__cplusplus)
extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											 CONFIG DATA TYPE 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <stdbool.h>

#define  UCHAR											unsigned char
#define  SCHAR											char	
#define  USHRT											unsigned short
#define  _BOOL											bool


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											 CONFIG CRITICAL 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  MBCD_ENTER_CRITICAL() 
#define  MBCD_EXIT_CRITICAL()


/*------------------------------------------------------------------------------------------------------------------
 * 
 *										   FLASH/ROM SPACE CONFIG
 *
 *				Example : '#define MBCD_CFG_CRC_P3 PROGMEM' (For avr-gcc)
 *
 *				MBCD_CFG_CRC_P1 static unsigned char MBCD_CFG_CRC_P2 auchCRCHi MBCD_CFG_CRC_P3[] = {...}
 *
 *								static unsigned char				 auchCRCHi PROGMEM		  [] = {...}
 *
 *				Note : Set ALL macro empty if data in RAM
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  MBCD_CFG_CRC_P1
#define  MBCD_CFG_CRC_P2
#define  MBCD_CFG_CRC_P3


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											CONFIG CODE SWITCH 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
																/**-------------- Slave address config -----------*/
#define  MBCD_CFG_ADDRESS								0X01	/**< Slave address								  */

																/**-------------- Trans mode config --------------*/
#define  MBCD_CFG_MOD_ASCII_EN							1		/**< ASCII mode	1 for enable & 0 for disable      */
#define  MBCD_CFG_MOD_RTU_EN                          	1		/**< RTU   mode	1 for enable & 0 for disable      */
#define  MBCD_CFG_MOD_TCP_EN                          	1		/**< TCP   mode	1 for enable & 0 for disable      */

																/**-------------- Functions config ---------------*/
																/**< Bit Access ++++++++++++++++++++++++++++++++++*/
#define  MBCD_CFG_COD_RD_DISCRETE_INPUTS_EN				1		/**< Enable 0X02 Read Discrete inputs or not      */
#define  MBCD_CFG_COD_RD_COILS_EN						0		/**< Enable 0x01 Read Coils or not				  */
#define  MBCD_CFG_COD_WR_SINGLE_COIL_EN					1		/**< Enable 0X05 Write Single Coil or not         */
#define  MBCD_CFG_COD_WR_MULTIPLE_COILS_EN				1		/**< Enable 0X0F Write Multiple Coils or not      */
																/**< 16Bit Access ++++++++++++++++++++++++++++++++*/
#define  MBCD_CFG_COD_RD_INPUT_REGISTER_EN				1		/**< Enable 0x04 Read Input Register or not       */
#define  MBCD_CFG_COD_RD_HOLDING_REGISTERSi_EN			1		/**< Enable 0x03 Read Holding Registers or not    */
#define  MBCD_CFG_COD_WR_SINGLE_REGISTER_EN				1		/**< Enable 0x06 Write Single Register or not     */
#define  MBCD_CFG_COD_WR_MULTIPLE_REGISTERS_EN			1		/**< Enable 0x10 Write Multiple Registers or not  */
#define  MBCD_CFG_COD_RW_MULTIPLE_REGISTERS_EN			1		/**< Enable 0x17 Read/Write Multiple Registers y/n*/
#define  MBCD_CFG_COD_WR_MASK_REGISTER_EN				1		/**< Enable 0x16 Mask Write Register or not		  */
#define  MBCD_CFG_COD_RD_FIFO_QUEUE_EN					1		/**< Enable 0x18 Read FIFO queue or not			  */
																/**< File Record Access ++++++++++++++++++++++++++*/
#define  MBCD_CFG_COD_RD_FILE_RECORD_EN					1		/**< Enable 0x14 Read File Record or not		  */
#define  MBCD_CFG_COD_WR_FILE_RECORD_EN					1		/**< Enable 0x15 Write File Record or not		  */
																/**< Diagnostics +++++++++++++++++++++++++++++++++*/
#define  MBCD_CFG_COD_RD_EXCEPTION_STATUS_EN			1		/**< Enable 0X07 Read Exception status or not     */
#define  MBCD_CFG_COD_DIAGNOSTIC_EN						1		/**< Enable 0X08 Diagnostic	or not				  */
#define  MBCD_CFG_COD_GET_COM_EVENT_COUNTER_EN			1		/**< Enable 0X0B Get Com event counter or not	  */
#define  MBCD_CFG_COD_GET_COM_EVENT_LOG_EN				1		/**< Enable 0X0C Get Com event log or not		  */
#define  MBCD_CFG_COD_REPORT_SERVER_ID_EN				1		/**< Enable 0X11 Report server ID or not		  */
#define  MBCD_CFG_COD_RD_DEVICE_ID_EN					1		/**< Enable 0X2B Read device identification or not*/
																/**< Others ++++++++++++++++++++++++++++++++++++++*/
#define  MBCD_CFG_COD_ENCAPSULATED_INTERFACE_TRANS_EN	1		/**< Enable 0X2B-Subcode 13,14 or not			  */
#define  MBCD_CFG_COD_CANOPEN_GENERAL_REFERENCE_EN		1		/**< Enable 0X2B-Subcode 13 or not				  */


#if defined(__cplusplus)
}
#endif


#endif /*__MODBCD_CONFIG_H__*/


