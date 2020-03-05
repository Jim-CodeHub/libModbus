/**-----------------------------------------------------------------------------------------------------------------
 * @file	modbcd.c
 * @brief   modbus client/master side code 
 *
 *  +-----------+---------------+----------------------------+-------------+
 *  | Address   | Function Code |			Data			 |     CRC     |
 *  +-----------+---------------+----------------------------+-------------+
 *  |	1Byte   |	 1Byte      |          0~252 Byte        |    2Byte	   |
 *  +-----------+---------------+----------------------------+-------------+
 *
 * @note	RAM used : MBCD_FRAME_SIZE_MAX + 14 Byte  
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
#include "modbcd.h"
#include "mbcd_box.h"
#include "mbcd_crc.h"


/*
--------------------------------------------------------------------------------------------------------------------
*
*											  VARIABLES DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/
static			tMBCD_Event				pMBMsgTx;								/**< Modbcd Tx message identity  */
static			tMBCD_Event				pMBMsgRx;								/**< Modbcd Rx message identity  */
static			eMBCD_EventType			eMBEvnTx;								/**< Modbcd Tx Event type		 */
static			eMBCD_EventType			eMBEvnRx;								/**< Modbcd Rx Event type		 */	

static volatile eMBCD_RcvState			eRcvState;								/**< Modbcd recv state			  */
static volatile eMBCD_SndState			eSndState = TX_STATE_IDLE;				/**< Modbcd send state			  */

static volatile uint8_t					ucFrameBuff[MBCD_FRAME_SIZE_MAX];		/**< Modbcd frame buffer          */
static volatile uint16_t				usFrameIndx = 0;						/**< Modbcd frame buffer Index	  */
static volatile uint16_t				usFrameLeng = 0;						/**< Mddbcd frame buffer length   */

static volatile uint8_t					ucMBSwitch = false;						/**< Modbcd enable or disable     */

static volatile uint8_t					ucMBSlaveAddr;							/**< Modbcd Address field value   */


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

/**
    @breif      Modbcd init 
    @param[in]  ucPort			-	MAY BE USEFUL for function 'xMBCD_PortSerialInit()'
    @param[in]  ulBaudRate		-	baudrate 
    @param[in]  eParity			-   parity	
    @param[in]  usTimeOut		-   Host timeout setting ( ms )
    @param[out] None
    @return     eMBCD_ErrorCode 
				1. ERR_NOERR	-	if there is no error occur
				2. ERR_PORTERR  -	if serial or timer port init error

	@note		If param 'usTimeOut' set to 0, then Host SHALL NOT timeout
*/
eMBCD_ErrorCode eMBCD_Init( uint8_t ucPort, uint32_t ulBaudRate, eMBCD_Parity eParity, uint16_t usTimerRsp_1Ms ) 
{
    eMBCD_ErrorCode    eStatus = ERR_NOERR;
    uint32_t		   usTimerT35_50us;

    vMBCD_EnterCritical(  );

    /* Modbus RTU uses 8 Databits. */
    if( xMBCD_PortSerialInit( ucPort, ulBaudRate, 8, eParity ) != true )
    {
        eStatus = ERR_PORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        if( ulBaudRate > 19200 )
        {
            usTimerT35_50us = 35;       /* 1800us. */
        }
        else
        {
            /* The timer reload value for a character is given by:
             *
             * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
             *             = 11 * Ticks_per_1s / Baudrate
             *             = 220000 / Baudrate
             * The reload for t3.5 is 1.5 times this value and similary
             * for t3.5.
             */
            usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
        }
        if( xMBCD_PortTimersInit( ( uint16_t ) usTimerT35_50us, usTimerRsp_1Ms ) != true )
        {
            eStatus = ERR_PORTERR;
        }
    }

    vMBCD_Exit_Critical(  );

    return eStatus;
}

/**
    @breif      Modbcd enabled 
    @param[in]  None 
    @param[out] None
    @return     eMBCD_ErrorCode 
				1. ERR_NOERR	-	if there is no error occur
				2. ERR_ILLSTATE -	if modbcd has already been enabled 
*/
eMBCD_ErrorCode eMBCD_Enable( void )
{
    eMBCD_ErrorCode    eStatus = ERR_ILLSTATE;

	if ( false == ucMBSwitch )
	{
		/**< Active the protocol stack */

		vMBCD_EnterCritical(  );
		/* Initially the receiver is in the state STATE_RX_INIT. we start
		 * the timer and if no character is received within t3.5 we change
		 * to STATE_RX_IDLE. This makes sure that we delay startup of the
		 * modbus protocol stack until the bus is free.
		 */
		eRcvState = RX_STATE_INIT;
		vMBCD_PortSerialEnable( true, false );
		vMBCD_PortTimersEnable( TIMER_T35 );

		ucMBSwitch = true;	

		vMBCD_Exit_Critical(  );

		eStatus = ERR_NOERR;
	}
    return eStatus;
}

/**
    @breif      Modbcd Disable 
    @param[in]  None 
    @param[out] None
    @return     eMBCD_ErrorCode 
				1. ERR_NOERR	-	if there is no error occur
				2. ERR_ILLSTATE -	if modbcd has already been disabled 
*/
eMBCD_ErrorCode eMBCD_Disable( void )
{
    eMBCD_ErrorCode    eStatus = ERR_ILLSTATE;

	if ( true == ucMBSwitch )
	{
		vMBCD_EnterCritical(  );

		vMBCD_PortSerialEnable( false, false );
		vMBCD_PortTimersDisable( TIMER_T35 );
		vMBCD_PortTimersDisable( TIMER_RSP );

		ucMBSwitch = false;

		vMBCD_Exit_Critical(  );

		eStatus = ERR_NOERR;
	}
    return eStatus;
}

/**
    @breif      Modbcd frame PDU load 
    @param[in]  None
    @param[out] pucPDU		-   modbus PDU (FunCode + Data Feild) buffer pointer 
    @param[out] pusLeng		-   modbus PDU length 
    @return     eMBCD_ErrorCode 
				1. ERR_NOERR	-	if there is no error occur
				2. ERR_ILLSTATE -	if modbus frame has not been received
				3. ERR_TIMEDOUT -   if slave response timeout
				4. ERR_NORES	-	if modbus frame error
*/
eMBCD_ErrorCode eMBCD_Load( uint8_t **pucPDU, uint16_t *pusLeng )
{
    eMBCD_ErrorCode    eStatus = ERR_ILLSTATE;
	eMBCD_EventType	  *eEvent;

	if( true == ucMBSwitch ) /**< Check if the protocol stack is ready. */
	{
		/* Check if there is a event available. If not return control to caller.
		 * Otherwise we will handle the event. */
		if( (void *)0 != (eEvent = (eMBCD_EventType *)pvMBCD_BoxAccept( &pMBMsgRx )) )
		{
			switch ( *eEvent )
			{
				case EV_TIME_OUT:
					eStatus = ERR_TIMEDOUT;
					/**< Active modbcd send side */
					vMBCD_BoxPost( &pMBMsgTx, (eMBEvnTx = EV_READY, &eMBEvnTx) );
					break;
				case EV_FRAME_RECEIVED:
					vMBCD_EnterCritical();

					/**< Length , Address and CRC check */
					if ( ( usFrameIndx >= MBCD_FRAME_SIZE_MIN + 1 ) 
							&& ( usMBCD_CRC16( ( uint8_t * ) ucFrameBuff, usFrameIndx ) == 0 ) 
							&& ( ucFrameBuff[MBCD_FRAME_OFFS_ADDR] == ucMBSlaveAddr ) ) 
					{
						*pucPDU  = (uint8_t *)&ucFrameBuff[MBCD_FRAME_OFFS_PDU];
						*pusLeng = usFrameIndx - MBCD_FRAME_OFFS_PDU - MBCD_FRAME_SIZE_CRC;

						eStatus = ERR_NOERR;
					}
					else
					{
						eStatus = ERR_NORES;
					}

					/**< Active modbcd send side */
					vMBCD_BoxPost( &pMBMsgTx, (eMBEvnTx = EV_READY, &eMBEvnTx) );

					vMBCD_Exit_Critical();
					break;
				default: ;
			}
		}
	}

    return eStatus;
}

/**
    @breif      packetize modbcd frame and active send FSM
    @param[in]  ucSlaveAddress		-	slave address 
    @param[in]  ucFunctionCode		-   function code	
    @param[in]  pucData				-   data field buffer pointer 
    @param[in]  usLength			-   data field buffer length 
    @return     eMBCD_ErrorCode  
				1. ERR_NOERR	-	if there is no error occur
				2. ERR_ILLSTATE -	if modbus transmit has not been enabled or modbcd disabled already yet
				3. ERR_INVAL	-	if param 'ucSlaveAddress' or 'ucFunctionCode' or 'usLength' error 

	@note		The function DO NOT CHECK user's data field which refer to param 'pucData'
*/
eMBCD_ErrorCode eMBCD_Send( uint8_t ucSlaveAddress, uint8_t ucFunctionCode, const void *pucData, uint16_t usLength )
{
	eMBCD_ErrorCode    eStatus = ERR_ILLSTATE;
	eMBCD_EventType	  *eEvent;
	uint16_t		   usCRC16;

	if( true == ucMBSwitch ) /**< Check if the protocol stack is ready. */
	{
		/* Check if there is a event available. If not return control to caller.
		 * Otherwise we will handle the event. */
		if( (void *)0 != (eEvent = (eMBCD_EventType *)pvMBCD_BoxAccept( &pMBMsgTx )) )
		{
			eSndState = ERR_INVAL;

			/**< Check preconditions and packetize frame */	
			if ( ( ucSlaveAddress >= MBCD_ADDR_BROADCAST ) && ( ucSlaveAddress <= MBCD_ADDR_SLAVE_MAX ) )
			{
				if ( ( 0 <= ucFunctionCode ) && ( ucFunctionCode <= 127 ) )
				{
					if ( ( usLength >= MBCD_FRAME_SIZE_MIN ) && ( usLength <= MBCD_FRAME_SIZE_MAX ) )
					{
						vMBCD_EnterCritical(  );

						ucMBSlaveAddr = ucSlaveAddress;  /**< Save transmit slave address */

						memset( (void *)ucFrameBuff, 0, sizeof(ucFrameBuff) );

						ucFrameBuff[MBCD_FRAME_OFFS_ADDR] = ucSlaveAddress;
						ucFrameBuff[MBCD_FRAME_OFFS_CODE] = ucFunctionCode;

						memmove( (void *)(ucFrameBuff + 2), pucData, usLength );

						/* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
						usCRC16 = usMBCD_CRC16( ( uint8_t * ) ucFrameBuff, usLength + 2);

						ucFrameBuff[usLength + 2] = ( uint8_t )( usCRC16 & 0xFF );
						ucFrameBuff[usLength + 3] = ( uint8_t )( usCRC16 >> 8 );

						usFrameIndx = 0;
						usFrameLeng = usLength + 4U;
						eSndState	= TX_STATE_SNDC; /**< Enable transmit FSM */

						vMBCD_PortSerialEnable( false, true );

						vMBCD_Exit_Critical(  );

						eStatus = ERR_NOERR; 
					}
				}
			}
		}
	}

	return eStatus;
}

/**
    @breif      Modbcd transmit FSM 
    @param[in]  None 
    @param[out] None
    @return     None 
	@note		Loop / interrupt mode can be used
*/
void vMBCD_TransmitFSM( void )
{
    assert( eRcvState == RX_STATE_IDLE );

    switch ( eSndState )
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case TX_STATE_IDLE:
        break;

	case TX_STATE_SNDC:
        /* check if we are finished. */
        if( usFrameIndx != usFrameLeng )
        {
            xMBCD_PortSerialPutByte( ucFrameBuff[usFrameIndx] ); 
			usFrameIndx++; /* next byte in frame buffer. */
        }
        else
        {
			if ( MBCD_ADDR_BROADCAST != ucFrameBuff[MBCD_FRAME_OFFS_ADDR] )
			{
				/* Disable transmitter and Enable receiver 
				 * & start response Expired. */
				vMBCD_EnterCritical();
				vMBCD_PortSerialEnable( true, false );
				vMBCD_PortTimersEnable( TIMER_RSP );
				vMBCD_Exit_Critical();
			}
			else /**< The Host sent broadcast. */
			{
				vMBCD_PortSerialEnable( false , false );
			}

            eSndState = TX_STATE_IDLE;
        }
        break;
    }
}

/**
    @breif      Modbcd receive FSM 
    @param[in]  None 
    @param[out] None
    @return     None 
	@note		Loop / interrupt mode can be used
*/
void vMBCD_ReceiveFSM( void )
{
	uint8_t           ucByte;

	assert( eSndState == TX_STATE_IDLE ); /**< To be sure Tx is in IDLE state. */

	/* Always read the character. */
	( void )xMBCD_PortSerialGetByte( ( int8_t * ) & ucByte );

	switch ( eRcvState )
	{
		/* If we have received a character in the init state we have to
		 * wait until the frame is finished.
		 */
		case RX_STATE_INIT:
			vMBCD_PortTimersEnable( TIMER_T35 );
			break;

			/* In the error state we wait until all characters in the
			 * damaged frame are transmitted.
			 */
		case RX_STATE_ERRS:
			vMBCD_PortTimersEnable( TIMER_T35 );
			break;

			/* In the idle state we wait for a new character. If a character
			 * is received the t1.5 and t3.5 timers are started and the
			 * receiver is in the state STATE_RX_RECEIVCE.
			 */
		case RX_STATE_IDLE:
			usFrameIndx = 0;
			ucFrameBuff[usFrameIndx++] = ucByte;
			eRcvState = RX_STATE_RCVC;

			/* Enable t3.5 timers. */
			vMBCD_PortTimersEnable( TIMER_T35 );
			/* Disable response timers. */
			vMBCD_PortTimersDisable( TIMER_RSP );
			break;

			/* We are currently receiving a frame. Reset the timer after
			 * every character received. If more than the maximum possible
			 * number of bytes in a modbus frame is received the frame is
			 * ignored.
			 */
		case RX_STATE_RCVC:
			if ( usFrameIndx < MBCD_FRAME_SIZE_MAX )
			{
				ucFrameBuff[usFrameIndx++] = ucByte;
			}
			else
			{
				eRcvState = RX_STATE_ERRS;
			}
			vMBCD_PortTimersEnable( TIMER_T35 );
			break;
		default: ;
	}
}

/**
    @breif      Timer T3.5 expired FSM 
    @param[in]  None 
    @param[out] None
    @return     None 
	@note	    The function MAY NOT use an individual timer. MAY some flag used
*/
void vMBCD_TimerT35Expired( void )
{
	switch ( eRcvState )
	{
		/**< Timer T35 expired. Startup phase is finished. */
		case RX_STATE_INIT:	
			vMBCD_BoxPost( &pMBMsgTx, (eMBEvnTx = EV_READY, &eMBEvnTx) );
			vMBCD_PortSerialEnable( false, false );
			break;

        /* A frame was received and t35 expired. Notify the listener that a new frame was received. */
		case RX_STATE_RCVC:	
			vMBCD_BoxPost( &pMBMsgRx, (eMBEvnRx = EV_FRAME_RECEIVED, &eMBEvnRx) );
			vMBCD_PortSerialEnable( false, false );
			break;

        /* An error occured while receiving the frame. */
		case RX_STATE_ERRS:	
			break;

        /* Function called in an illegal state. */
		default:
			assert( ( eRcvState == RX_STATE_INIT ) ||
					( eRcvState == RX_STATE_RCVC ) || ( eRcvState == RX_STATE_ERRS ) );
	}

	vMBCD_PortTimersDisable( TIMER_T35 );
	eRcvState = RX_STATE_IDLE;
}

/**
    @breif      Timer Repsone expired FSM 
    @param[in]  None 
    @param[out] None
    @return     None 
	@note	    The function MAY NOT use an individual timer. MAY some flag used
*/
void vMBCD_TimerRspExpired( void )
{
	switch ( eRcvState )
	{
		/**< Rx is listen to the port. */
		case RX_STATE_IDLE:	
			vMBCD_BoxPost(&pMBMsgRx, (eMBEvnRx = EV_TIME_OUT, &eMBEvnRx));
			break;

        /**< Function called in an illegal state. */
		default:
			assert( eRcvState == RX_STATE_IDLE );
	}

	vMBCD_PortTimersDisable( TIMER_RSP );
}

