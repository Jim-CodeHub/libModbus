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
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   MODBCD SHORT ALIAS	
 *
 *------------------------------------------------------------------------------------------------------------------
*/
																			/******** Frame defination ************/
#define  MB_FRAME_SIZE_MAX							256						/**< Frame maximum size				  */
#define  MB_FRAME_SIZE_MIN							4						/**< Frame minimum size				  */	
#define  MB_FRAME_SIZE_CRC							2						/**< Frame CRC field size			  */
#define  MB_FRAME_OFFS_ADDR							0						/**< Address field offset in frame	  */
#define  MB_FRAME_OFFS_CODE							1						/**< FunCode field offset in frame	  */
#define  MB_FRAME_OFFS_DATA							2						/**< Data    field offset in frame	  */


																			/******** Address defination **********/
#define MB_ADDR_BROADCAST							0					    /**< Modbus broadcast address		  */
#define MB_ADDR_SLAVE_MIN							1						/**< Smallest possible slave address  */
#define MB_ADDR_SLAVE_MAX							247						/**< Biggest possible slave address   */

																			/******** FunCode defination **********/
#define MB_FUNC_NONE							    0
#define MB_FUNC_READ_COILS                    	    1
#define MB_FUNC_READ_DISCRETE_INPUTS          	    2
#define MB_FUNC_WRITE_SINGLE_COIL             	    5
#define MB_FUNC_WRITE_MULTIPLE_COILS          	   15
#define MB_FUNC_READ_HOLDING_REGISTER         	    3
#define MB_FUNC_READ_INPUT_REGISTER           	    4
#define MB_FUNC_WRITE_REGISTER                	    6
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      	   16
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS  	   23
#define MB_FUNC_DIAG_READ_EXCEPTION           	    7
#define MB_FUNC_DIAG_DIAGNOSTIC               	    8
#define MB_FUNC_DIAG_GET_COM_EVENT_CNT        	   11
#define MB_FUNC_DIAG_GET_COM_EVENT_LOG        	   12
#define MB_FUNC_DIAG_REPORT_SLAVEID           	   17
#define MB_FUNC_ERROR                         	   128


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
} eMBRcvState;

typedef enum {																/******** Send state enumeration ******/
	TX_STATE_SNDC,															/**< Transmit state sending character */
	TX_STATE_IDLE,															/**< Transmit state idle			  */
} eMBSndState;

typedef enum {																/******** Internal Event enumeration **/
    EV_READY,															    /**< Startup finished				  */
    EV_FRAME_RECEIVED,      											    /**< Frame received					  */
    EV_EXECUTE,             											    /**< Execute function				  */
    EV_FRAME_SENT,          											    /**< Frame has been sent			  */
	EV_FRAME_EMIT,															/**< Frame can be sent				  */
} eMBEventType;

typedef enum {																/******** Parity enumeration **********/
    MB_PAR_NONE,															/**< No parity						  */
    MB_PAR_ODD,																/**< Odd parity						  */
    MB_PAR_EVEN																/**< Even parity					  */
} eMBParity;

typedef enum {																/******** Custom error enumeration ****/
    MB_ENOERR,																/**< no error						  */
    MB_ENOREG,                  											/**< illegal register address		  */
    MB_EINVAL,                  											/**< illegal argument				  */
    MB_EPORTERR,                											/**< porting layer error			  */
    MB_ENORES,                  											/**< insufficient resources			  */
    MB_EIO,                     											/**< I/O error						  */
    MB_EILLSTATE,               											/**< protocol stack in illegal state  */
    MB_ETIMEDOUT                											/**< timeout error occurred			  */
} eMBErrorCode;


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											MODBCD FUN DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
eMBErrorCode	eMBCDInit( uint8_t ucPort, uint32_t ulBaudRate, eMBParity eParity, uint16_t usTimeOut );
eMBErrorCode	eMBCDEnable( void );
eMBErrorCode	eMBCDDisable( void );
eMBErrorCode	eMBCDPoll( void );
eMBErrorCode	eMBCDLoad( uint8_t **pucData, uint16_t *pusLeng );
eMBErrorCode	eMBCDSend( uint8_t ucSlaveAddress, uint8_t ucFunctionCode, const uint8_t *pucData, uint16_t usLength );
void			vMBCDTransmitFSM( void );
void			vMBCDReceiveFSM( void );
void			vMBCDTimerT35Expired( void );


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											MBPORT FUN DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
bool            xMBPortSerialInit( uint8_t ucPort, uint32_t ulBaudRate, uint8_t ucDataBits, eMBParity eParity );

void            vMBPortSerialEnable( bool xRxEnable, bool xTxEnable );
bool            xMBPortSerialGetByte( int8_t * pucByte );
bool            xMBPortSerialPutByte( int8_t ucByte );

bool            xMBPortTimersInit( uint16_t usTimeOut50us );
void            vMBPortTimersEnable( void );
void            vMBPortTimersDisable( void );

void			vMBEnterCritical( void );
void			vMBExit_Critical( void );


#ifdef __cplusplus
	} //extern "C"
#endif


#endif //_MODBCD_H__

