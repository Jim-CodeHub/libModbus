/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbport.h
 * @brief   modbus portable functions declaration
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#ifndef _MBPORT_H__
#define _MBPORT_H__


#ifdef __cplusplus
	extern "C" {
#endif

/*------------------------------------------------------------------------------------------------------------------
 * 
 *											    MBPORT INCLUDES
 *
 *------------------------------------------------------------------------------------------------------------------
*/


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											MBPORT FUN DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/

BOOL            xMBPortSerialInit( UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity );

void            vMBPortClose( void );

void            xMBPortSerialClose( void );
void            vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable );
BOOL            xMBPortSerialGetByte( CHAR * pucByte );
BOOL            xMBPortSerialPutByte( CHAR ucByte );

BOOL            xMBPortTimersInit( USHORT usTimeOut50us );
void            xMBPortTimersClose( void );
void            vMBPortTimersEnable( void );
void            vMBPortTimersDisable( void );


#ifdef __cplusplus
	} //extern "C"
#endif


#endif //_MBPORT_H__

