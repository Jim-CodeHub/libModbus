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
#include "port.h"
#include "util/mbox.h"
#include "util/mbcrc.h"


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   MODBCD SHORT ALIAS	
 *
 *------------------------------------------------------------------------------------------------------------------
*/

/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   MODBCD DATA BLOCKS 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
typedef enum
{
    EV_READY,                   /*!< Startup finished. */
    EV_FRAME_RECEIVED,          /*!< Frame received. */
    EV_EXECUTE,                 /*!< Execute function. */
    EV_FRAME_SENT,              /*!< Frame has been sent. */
	EV_FRAME_EMIT,				/**< Frame can be sent */
	EV_FRAME_LOAD				/**< Frame can be load now */
} eMBEventType;

typedef enum {
	RX_STATE_INIT,
	RX_STATE_RCVC,
	RX_STATE_ERRS,
	RX_STATE_IDLE,
} eMBRcvState;

typedef enum {
	TX_STATE_INIT,
	TX_STATE_SNDC,
	TX_STATE_IDLE,
} eMBSndState;

typedef struct {
    UCHAR          ucFunctionCode;
    pxMBFunctionHandler pxHandler;
} xMBFunctionHandler;


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											MODBCD FUN DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
eMBErrorCode	eMBCDInit( UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity, USHORT usTimeOut );
eMBErrorCode	eMBCDEnable( void );
eMBErrorCode	eMBCDDisable( void );
eMBErrorCode	eMBCDPoll( void );
eMBErrorCode	eMBCDLoad( UCHAR **pucData, USHORT *pusLeng );
eMBErrorCode	eMBSend( UCHAR ucSlaveAddress, UCHAR ucFunctionCode, const UCHAR *pucData, USHORT usLength );
void			vMBCDTransmitFSM( void );
void			vMBCDReceiveFSM( void );
void			vMBCDTimerT35Expired( void );


#ifdef __cplusplus
	} //extern "C"
#endif


#endif //_MODBCD_H__

