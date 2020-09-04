/**-----------------------------------------------------------------------------------------------------------------
 * @file	modbcd.h
 * @brief   modbus client/master side code 
 *
 *  +-----------+---------------+----------------------------+-------------+
 *  | Address   | Function Code |			Data			 |     CRC     |
 *  +-----------+---------------+----------------------------+-------------+
 *  |	1Byte   |	 1Byte      |          0~252 Byte        |    2Byte	   |
 *  +-----------+---------------+----------------------------+-------------+
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#ifndef _MODBCD_H__
#define _MODBCD_H__


#ifdef __cplusplus
	extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MODBCD INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "mbcd_box.h"
#include "port.h"

/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   MODBCD SHORT ALIAS	
 *
 *------------------------------------------------------------------------------------------------------------------
*/
																			/******** Frame defination ************/
#define  MBCD_FRAME_SIZE_MAX						256						/**< Frame maximum size				  */
#define  MBCD_FRAME_SIZE_MIN						4						/**< Frame minimum size				  */	
#define  MBCD_FRAME_SIZE_CRC						2						/**< Frame CRC field size			  */
#define  MBCD_FRAME_OFFS_ADDR						0						/**< Address field offset in frame	  */
#define  MBCD_FRAME_OFFS_CODE						1						/**< FunCode field offset in frame	  */
#define  MBCD_FRAME_OFFS_DATA						2						/**< Data    field offset in frame	  */
#define  MBCD_FRAME_OFFS_PDU						1						/**< Data    field offset in frame	  */


																			/******** Address defination **********/
#define  MBCD_ADDR_BROADCAST						0					    /**< Modbus broadcast address		  */
#define  MBCD_ADDR_SLAVE_MIN						1						/**< Smallest possible slave address  */
#define  MBCD_ADDR_SLAVE_MAX						247						/**< Biggest possible slave address   */


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   MODBCD DATA BLOCKS 
 *
 *------------------------------------------------------------------------------------------------------------------
*/

typedef enum {																/******** Receive state enumeration ***/
	RX_STATE_INIT,															/**< Receive state init				  */	
	RX_STATE_RCVC,															/**< Receive state receiving character*/
	RX_STATE_ERRS,															/**< Receive state errors occur		  */
	RX_STATE_IDLE,															/**< Receive state idle				  */
} eMBCD_RcvState;

typedef enum {																/******** Send state enumeration ******/
	TX_STATE_SNDC,															/**< Transmit state sending character */
	TX_STATE_IDLE,															/**< Transmit state idle			  */
} eMBCD_SndState;

typedef enum {																/******** Internal Event enumeration **/
    EV_READY,															    /**< Startup finished				  */
    EV_FRAME_RECEIVED,      											    /**< Frame received					  */
	EV_TIME_OUT																/**< Response timeout				  */
} eMBCD_EventType;

typedef enum {																/******** Parity enumeration **********/
    PARITY_NONE,															/**< No parity						  */
    PARITY_ODD,																/**< Odd parity						  */
    PARITY_EVEN																/**< Even parity					  */
} eMBCD_Parity;

typedef enum {																/******** Timer select enumeration ****/
	TIMER_T35,																/**< T35 Timer						  */
	TIMER_RSP																/**< Response Timer					  */
} eMBCD_Timer;

typedef enum {																/******** Custom error enumeration ****/
    ERR_NOERR,																/**< no error						  */
    ERR_INVAL,                  											/**< illegal argument				  */
    ERR_PORTERR,                											/**< porting layer error			  */
    ERR_NORES,                  											/**< insufficient resources			  */
    ERR_ILLSTATE,               											/**< protocol stack in illegal state  */
    ERR_TIMEDOUT                											/**< timeout error occurred			  */
} eMBCD_ErrorCode;


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											MODBCD FUN DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
eMBCD_ErrorCode	eMBCD_Init( uint8_t ucPort, uint32_t ulBaudRate, eMBCD_Parity eParity, uint16_t usTimerRsp_1Ms );
eMBCD_ErrorCode	eMBCD_Enable( void );
eMBCD_ErrorCode	eMBCD_Disable( void );
eMBCD_ErrorCode	eMBCD_Load( uint8_t **pucPDU, uint16_t *pusLeng );
eMBCD_ErrorCode	eMBCD_Send( uint8_t ucSlaveAddress, uint8_t ucFunctionCode, const void *pucData, uint16_t usLength );
void			vMBCD_TransmitFSM( void );
void			vMBCD_ReceiveFSM( void );
void			vMBCD_TimerT35Expired( void );
void			vMBCD_TimerRspExpired( void );


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											MBPORT FUN DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
bool            xMBCD_PortSerialInit( uint8_t ucPort, uint32_t ulBaudRate, uint8_t ucDataBits, eMBCD_Parity eParity );

void            vMBCD_PortSerialEnable( bool xRxEnable, bool xTxEnable );
bool            xMBCD_PortSerialGetByte( int8_t * pucByte );
bool            xMBCD_PortSerialPutByte( int8_t ucByte );

bool            xMBCD_PortTimersInit( uint16_t usTimerT35_50us, uint16_t usTimerRsp_1Ms );
void            vMBCD_PortTimersEnable( eMBCD_Timer eTimer );
void            vMBCD_PortTimersDisable( eMBCD_Timer eTimer );

void			vMBCD_EnterCritical( void );
void			vMBCD_Exit_Critical( void );

USHORT			usMBCD_CRC16( UCHAR * pucFrame, USHORT usLen );


#ifdef __cplusplus
	} //extern "C"
#endif


#endif //_MODBCD_H__

