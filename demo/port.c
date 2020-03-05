/**-----------------------------------------------------------------------------------------------------------------
 * @file	port.c
 * @brief   modbcd port protable functions	
 *
 * @note	RAM used : 0Bytes
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
#include "modbcd.h"
#include <avr/interrupt.h>


static uint8_t	_SREG;	/**< For Enter & Exit critical */


/*
--------------------------------------------------------------------------------------------------------------------
*											   CRITICAL PORT 
--------------------------------------------------------------------------------------------------------------------
*/
void vMBCD_EnterCritical( void )	{_SREG = SREG; __asm__ volatile("cli");}
void vMBCD_Exit_Critical( void )	{SREG = _SREG;}	


/*
--------------------------------------------------------------------------------------------------------------------
*												TIMER PORT 
--------------------------------------------------------------------------------------------------------------------
*/
static uint16_t   usTimer3TCNTDelta;

bool xMBCD_PortTimersInit( uint16_t usTim1Timerout50us )
{
	usTimer3TCNTDelta = (usTim1Timerout50us * 50) / 16;  /**< Formula : (usTim1Timerout50us * 50) / 1_T_Per_Cnt */

    TCCR3A = 0x00; TCCR3B = 0x00; TCCR3C = 0x00;

    vMBCD_PortTimersDisable(  );

    return true;
}

inline void vMBCD_PortTimersEnable(  )
{
	ETIMSK |= _BV( TOIE3 ); /**< Enable timer3 IE */

	TCNT3 = 65536UL - usTimer3TCNTDelta; /**< Reload timer3 counter */

	TCCR3B |= _BV( CS32 ); /**< Set 256 prescaler (16us per count) */
}

inline void vMBCD_PortTimersDisable(  )
{
	TCCR3B &= ~( _BV( CS32 ) );	/**< Disable timer3 */

	ETIMSK &= ~( _BV( TOIE3 ) ); /**< Disable timer3 IE */
}

ISR(TIMER3_OVF_vect)
{
	(void )vMBCD_TimerT35Expired();
}


/*
--------------------------------------------------------------------------------------------------------------------
*												SERIAL PORT 
--------------------------------------------------------------------------------------------------------------------
*/

void vMBCD_PortSerialEnable( bool xRxEnable, bool xTxEnable )
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

bool xMBCD_PortSerialInit( uint8_t ucPORT, uint32_t ulBaudRate, uint8_t ucDataBits, eMBCD_Parity eParity )
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
        case PARITY_EVEN:
			UCSR1C |=   _BV (UPM11) ; 
			UCSR1C &= ~(_BV (UPM10));

			UCSR1C &= ~(_BV (USBS1)); /**< 1 StopBit */

            break;
        case PARITY_ODD:
			UCSR1C |=   _BV (UPM11) | _BV (UPM10); 

            break;
        case PARITY_NONE:
			UCSR1C &= ~(_BV (UPM11) | _BV (UPM10)); 
			UCSR1C|=    _BV( USBS1 ); /**< 2 StopBit */  
			
            break;
		default: ;
    }

	UCSR1A &= ~(_BV (MPCM1)); /**< Multi machine comm off */

    vMBCD_PortSerialEnable( false, false );

    return true;
}

bool xMBCD_PortSerialPutByte( int8_t ucByte )
{
    UDR1 = ucByte;
    return true;
}

bool xMBCD_PortSerialGetByte( int8_t * pucByte )
{
    *pucByte = UDR1;
    return true;
}

ISR(USART1_UDRE_vect) /**< Use UDRE IE instead of TX IE (USART1_TX_vect) to speed up trans */
{
	( void )vMBCD_TransmitFSM( );
}

ISR(USART1_RX_vect)
{
	( void )vMBCD_ReceiveFSM( );
}

