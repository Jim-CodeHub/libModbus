/**-----------------------------------------------------------------------------------------------------------------
 * @file	timer.c
 * @brief   FreeModbus timer protable functions	
 *
 * @note	RAM used : 4Bytes
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
static USHORT   usTimer3TCNTDelta;


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

BOOL xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	usTimer3TCNTDelta = (usTim1Timerout50us * 50) / 16;  /**< Formula : (usTim1Timerout50us * 50) / 1_T_Per_Cnt */

    TCCR3A = 0x00; TCCR3B = 0x00; TCCR3C = 0x00;

    vMBPortTimersDisable(  );

    return TRUE;
}

inline void vMBPortTimersEnable(  )
{
	ETIMSK |= _BV( TOIE3 ); /**< Enable timer3 IE */

	TCNT3 = 65536UL - usTimer3TCNTDelta; /**< Reload timer3 counter */

	TCCR3B |= _BV( CS32 ); /**< Set 256 prescaler (16us per count) */
}

inline void vMBPortTimersDisable(  )
{
	TCCR3B &= ~( _BV( CS32 ) );	/**< Disable timer3 */

	ETIMSK &= ~( _BV( TOIE3 ) ); /**< Disable timer3 IE */
}

ISR(TIMER3_OVF_vect)
{
    ( void )pxMBPortCBTimerExpired(  );
}

//DEMO undefined 
//void            xMBPortTimersClose( void );
//void            vMBPortTimersDelay( USHORT usTimeOutMS );
