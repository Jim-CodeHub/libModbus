/**-----------------------------------------------------------------------------------------------------------------
 * @file	serial.c
 * @brief   FreeModbus serial protable functions	
 *
 * @note	RAM used : 0Bytes
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "port.h"
#include "mb.h"
#include "mbport.h"


/*
--------------------------------------------------------------------------------------------------------------------
*
*											  VARIABLES DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/

/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    if( xRxEnable ) { /**< Enable Rx and Rx IE */ 
        UCSR1B |= _BV( RXEN1 ) | _BV( RXCIE1 ); 
    }
    else { /**< Disable Rx and Rx IE */
        UCSR1B &= ~( _BV( RXEN1 ) | _BV( RXCIE1 ) ); 
    }

    if( xTxEnable ) { /**< Enable Tx and Tx(UDRE) IE */
        UCSR1B |= _BV( TXEN1 ) | _BV( UDRE1 );
    }
    else { /**< Disable Tx and Tx(UDRE) IE */
        UCSR1B &= ~( _BV( TXEN1 ) | _BV( UDRE1 ) );
    }
}

BOOL xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	UCSR1A &= ~(_BV (U2X1)); /**< Switch off U2X1 */

	UBRR1L = (F_CPU/ulBaudRate/16  - 1) % 256; /**< BaudRate L set */
	UBRR1H = (F_CPU/ulBaudRate/16  - 1) / 256; /**< BaudRate H set */

    switch ( ucDataBits ) /**< Data bits set */
    {
        case 8:
			UCSR1B &= ~(_BV (UCSZ12));
			UCSR1C |=   _BV (UCSZ11) | _BV (UCSZ10);
            break;
        case 7:
			UCSR1B &= ~(_BV (UCSZ12));
			UCSR1C |=   _BV (UCSZ11) ;
			UCSR1C &= ~(_BV (UCSZ10));
            break;
		default: ;
    }

    switch ( eParity ) /**< Parity & Stop bit config */
    {
        case MB_PAR_EVEN:
			UCSR1C |=   _BV (UPM11) ; 
			UCSR1C &= ~(_BV (UPM10));

			UCSR1C &= ~(_BV (USBS1)); /**< 1 StopBit */

            break;
        case MB_PAR_ODD:
			UCSR1C |=   _BV (UPM11) | _BV (UPM10); 

            break;
        case MB_PAR_NONE:
			UCSR1C &= ~(_BV (UPM11) | _BV (UPM10)); 
			UCSR1C|=    _BV( USBS1 ); /**< 2 StopBit */  
			
            break;
		default: ;
    }

	UCSR1A &= ~(_BV (MPCM1)); /**< Multi machine comm off */

    vMBPortSerialEnable( FALSE, FALSE );

    return TRUE;
}

BOOL xMBPortSerialPutByte( CHAR ucByte )
{
    UDR1 = ucByte;
    return TRUE;
}

BOOL xMBPortSerialGetByte( CHAR * pucByte )
{
    *pucByte = UDR1;
    return TRUE;
}

ISR(USART1_UDRE_vect) /**< Use UDRE IE instead of TX IE (USART1_TX_vect) to speed up trans */
{
	pxMBFrameCBTransmitterEmpty(  );
}

ISR(USART1_RX_vect)
{
    pxMBFrameCBByteReceived(  );
}

//DEMO undefined 
//void            vMBPortClose( void );
//void            xMBPortSerialClose( void );
