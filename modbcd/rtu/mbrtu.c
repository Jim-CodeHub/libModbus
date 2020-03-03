/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbrtu.c
 * @brief   modbcd rtu mode functions 
 *
 * @note	RAM used : 0Bytes
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
#include "mbrtu.h"

/*
--------------------------------------------------------------------------------------------------------------------
*
*											  VARIABLES DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/

static volatile eMBRcvState eRcvState;
static volatile eMBSndState eSndState;


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

void vMBRTUReceiveFSM( void )
{
	assert( eSndState == TX_STATE_IDLE ); /**< To be sure Tx is in IDLE state. */

	UCHAR ucByte = MB_PORT_GET_BYTE(); /**< Always get the character. */ 

	switch ( eRcvState )
	{
		case RX_STATE_INIT:	
			break;
		case RX_STATE_RCVC:	
			break;
		case RX_STATE_ERRS:	
			break;
		case RX_STATE_IDLE:	
			break;
		default: ;
	}
}

/**
    @breif      T3.5 expired FSM 
    @param[in]  None 
    @param[out] None
    @return     None 
*/
void vMBRTUTimerT35Expired( void )
{
	switch ( eRcvState )
	{
		/**< Timer T35 expired. Startup phase is finished. */
		case RX_STATE_INIT:	
			MB_PORT_EVENT_POST( EV_READY );
			MB_PORT_SERIAL_SWITCH( FALSE, TRUE );
			break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
		case RX_STATE_RCVC:	
			MB_PORT_EVENT_POST( EV_FRAME_RECEIVED );
			break;

        /* An error occured while receiving the frame. */
		case RX_STATE_ERRS:	
			break;

        /* Function called in an illegal state. */
		default:
			assert( ( eRcvState == RX_STATE_INIT ) ||
					( eRcvState == RX_STATE_RCVC ) || ( eRcvState == RX_STATE_ERRS ) );
	}

	MB_PORT_TIMERT35_DISABLE();
	eRcvState = RX_STATE_IDLE;
}

