/**-----------------------------------------------------------------------------------------------------------------
 * @file	port.c
 * @brief   modbcd port protable functions ( style2 for timer port )
 *
 * @note	RAM used : 5Bytes
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
static uint16_t   usTimerT35Cnt = 0;
static uint16_t   usTimerRspCnt = 0;

static uint16_t   usTimerT35Val = 0;
static uint16_t   usTimerRspVal = 0;

static bool		  bTimerT35Switch = false;
static bool		  bTimerRspSwitch = false;

bool xMBCD_PortTimersInit( uint16_t usTimerT35_50us, uint16_t usTimerRsp_1Ms )
{
    vMBCD_PortTimersDisable( TIMER_T35 );
    vMBCD_PortTimersDisable( TIMER_RSP );

	ETIMSK |= _BV( TOIE3 ); /**< Enable timer3 IE */

	TCCR3B |= _BV( CS32 ); /**< Set 256 prescaler (16us per count) */

	TCNT3 = 65536UL - 16*3; /**< Reload timer, 16*3 about 50us  */ 

	usTimerT35Val = usTimerT35_50us * 50 / 16*3;
	usTimerRspVal = usTimerRsp_1Ms * 1000UL / 16*3;

    return true;
}

inline void vMBCD_PortTimersEnable( eMBCD_Timer eTimer )
{
	if ( TIMER_T35 == eTimer )
	{
		usTimerT35Cnt = 0;
		bTimerT35Switch = true;
	}
	else
	{
		usTimerRspCnt = 0;
		bTimerRspSwitch = true;
	}
}

inline void vMBCD_PortTimersDisable( eMBCD_Timer eTimer )
{
	if ( TIMER_T35 == eTimer )
	{
		bTimerT35Switch = false;
	}
	else
	{
		bTimerRspSwitch = false;
	}
}

ISR(TIMER3_OVF_vect) //Common timer
{
	TCNT3 = 65536UL - 16*3; /**< Reload timer, 16*3 about 50us per IE */ 

	if (( true == bTimerT35Switch ) && ( usTimerT35Cnt++ == usTimerT35Val ))
	{
		(void )vMBCD_TimerT35Expired();
	}
	
	if (( true == bTimerRspSwitch ) && ( usTimerRspCnt++ == usTimerRspVal )) 
	{
		(void )vMBCD_TimerRspExpired();
	}
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

