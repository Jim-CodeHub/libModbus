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
 * @note	RAM used : 269Bytes
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
#include "modbcd.h"
#include "mbox.h"
#include "mbcrc.h"


/*
--------------------------------------------------------------------------------------------------------------------
*
*											  VARIABLES DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/
static			_OS_EVENT				sMBMsgID;								/**< Modbcd message box identity  */
static			eMBEventType			eMBEvent;								/**< Modbcd Event type			  */	

static volatile	bool					xSendFlg;								/**< Modbcd data send flag        */
static volatile	bool					xRecvFlg;								/**< Modbcd data load flag		  */

static volatile eMBRcvState				eRcvState;								/**< Modbcd recv state			  */
static volatile eMBSndState				eSndState;								/**< Modbcd send state			  */

static volatile uint8_t					ucFrameBuff[MB_FRAME_SIZE_MAX];			/**< Modbcd frame buffer          */
static volatile uint16_t				usFrameIndx;							/**< Modbcd frame buffer Index	  */
static volatile uint16_t				usFrameLeng;							/**< Mddbcd frame buffer length   */

static volatile uint8_t					ucMBSwitch;								/**< Modbcd enable or disable     */

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
    @param[in]  ucPort			-	MAY BE USEFUL for function 'xMBPortSerialInit()'
    @param[in]  ulBaudRate		-	baudrate 
    @param[in]  eParity			-   parity	
    @param[in]  usTimeOut		-   Host timeout setting ( ms )
    @param[out] None
    @return     eMBErrorCode 
				1. MB_ENOERR	-	if there is no error occur
				2. MB_EPORTERR  -	if serial or timer port init error
*/
eMBErrorCode eMBCDInit( uint8_t ucPort, uint32_t ulBaudRate, eMBParity eParity, uint16_t usTimeOut ) 
{
    eMBErrorCode    eStatus = MB_ENOERR;
    uint32_t        usTimerT35_50us;

    vMBEnterCritical(  );

    /* Modbus RTU uses 8 Databits. */
    if( xMBPortSerialInit( ucPort, ulBaudRate, 8, eParity ) != true )
    {
        eStatus = MB_EPORTERR;
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
        if( xMBPortTimersInit( ( uint16_t ) usTimerT35_50us ) != true )
        {
            eStatus = MB_EPORTERR;
        }
    }

    vMBExit_Critical(  );

	ucMBSwitch = false; /**< Disable modbcd */

    return eStatus;
}

/**
    @breif      Modbcd enabled 
    @param[in]  None 
    @param[out] None
    @return     eMBErrorCode 
				1. MB_ENOERR	-	if there is no error occur
				2. MB_EILLSTATE -	if modbcd has already been enabled 
*/
eMBErrorCode eMBCDEnable( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

	if ( false == ucMBSwitch )
	{
		/**< Active the protocol stack */

		vMBEnterCritical(  );
		/* Initially the receiver is in the state STATE_RX_INIT. we start
		 * the timer and if no character is received within t3.5 we change
		 * to STATE_RX_IDLE. This makes sure that we delay startup of the
		 * modbus protocol stack until the bus is free.
		 */
		eRcvState = RX_STATE_INIT;
		vMBPortSerialEnable( true, false );
		vMBPortTimersEnable(  );

		vMBExit_Critical(  );

		ucMBSwitch = true;	
	}
	else
	{
		eStatus = MB_EILLSTATE;
	}
    return eStatus;
}

/**
    @breif      Modbcd Disable 
    @param[in]  None 
    @param[out] None
    @return     eMBErrorCode 
				1. MB_ENOERR	-	if there is no error occur
				2. MB_EILLSTATE -	if modbcd has already been disabled 
*/
eMBErrorCode eMBCDDisable( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

	if ( true == ucMBSwitch )
	{
		vMBEnterCritical(  );
		vMBPortSerialEnable( false, false );
		vMBPortTimersDisable(  );
		vMBExit_Critical(  );

		ucMBSwitch = false;
	}
	else
	{
		eStatus = MB_EILLSTATE;
	}

    return eStatus;
}

/**
    @breif      Modbcd events loop 
    @param[in]  None 
    @param[out] None
    @return     eMBErrorCode 
				1. MB_ENOERR	-	if there is no error occur
				2. MB_EILLSTATE -	if modbcd has not been enabled yet
*/
eMBErrorCode eMBCDPoll( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    eMBEventType    *eEvent;

	if( true == ucMBSwitch ) /**< Check if the protocol stack is ready. */
	{
		/* Check if there is a event available. If not return control to caller.
		 * Otherwise we will handle the event. */
		if( (void *)0 != (eEvent = (eMBEventType *)_OSMboxAccept( &sMBMsgID )) )
		{
			switch ( *eEvent )
			{
				case EV_READY:
					xSendFlg = true; /**< Enable 'eMBSend()' function */
					break;

				case EV_FRAME_RECEIVED:
					vMBEnterCritical();

					/**< Length , Address and CRC check */
					if ( ( usFrameIndx >= MB_FRAME_SIZE_MIN + 1 ) && ( usMBCRC16( ( uint8_t * ) ucFrameBuff, usFrameIndx ) == 0 )  && ( ucFrameBuff[MB_FRAME_OFFS_ADDR] == ucMBSlaveAddr ) ) 
					{
						xRecvFlg = true; /**< Enable receive  for function 'eMBCDLoad()' */
						xSendFlg = true; /**< Enable transmit for function 'eMBCDSend()' */
					}
					else
					{
						eStatus = MB_ENORES;
					}

					vMBExit_Critical();
					break;

				case EV_EXECUTE:
					break;

				case EV_FRAME_SENT:
					if ( MB_ADDR_BROADCAST != ucFrameBuff[MB_FRAME_OFFS_ADDR] )
					{
						vMBPortSerialEnable( true, false );
						//+开启超时
					}
					break;
				default: ;
			}
		}
	}
	else
	{
		eStatus = MB_EILLSTATE;
	}

    return eStatus;
}

/**
    @breif      Modbcd data field load 
    @param[in]  None
    @param[out] pucData		-   modbus data field buffer pointer 
    @param[out] pusLeng		-   modbus data field buffer length 
    @return     eMBErrorCode 
				1. MB_ENOERR	-	if there is no error occur
				2. MB_EILLSTATE -	if modbus frame has not received and processed 
*/
eMBErrorCode eMBCDLoad( uint8_t **pucData, uint16_t *pusLeng )
{
    eMBErrorCode    eStatus = MB_EILLSTATE;

	if ( true == xRecvFlg ) 
	{
		xRecvFlg = false;

		*pucData = (uint8_t *)&ucFrameBuff[MB_FRAME_OFFS_DATA];
		*pusLeng = usFrameLeng;

		eStatus = MB_ENOERR;
	}

    return eStatus;
}

/**
    @breif      packetize modbcd frame and active send FSM
    @param[in]  ucSlaveAddress		-	slave address 
    @param[in]  ucFunctionCode		-   function code	
    @param[in]  pucData				-   data field buffer pointer 
    @param[in]  usLength			-   data field buffer length 
    @return     eMBErrorCode  
				1. MB_ENOERR	-	if there is no error occur
				2. MB_EINVAL	-	if param 'ucSlaveAddress' or 'ucFunctionCode' or 'usLength' error 

	@note		The function DO NOT CHECK user's data field which refer to param 'pucData'
*/
eMBErrorCode eMBCDSend( uint8_t ucSlaveAddress, uint8_t ucFunctionCode, const uint8_t *pucData, uint16_t usLength )
{
    eMBErrorCode    eStatus = MB_EINVAL;
    uint16_t        usCRC16;

	if ( true == xSendFlg )
	{
		xSendFlg = false;

		/**< Check preconditions and packetize frame */	
		if ( ( ucSlaveAddress >= MB_ADDR_BROADCAST ) && ( ucSlaveAddress <= MB_ADDR_SLAVE_MAX ) )
		{
			if ( ( 0 <= ucFunctionCode ) && ( ucFunctionCode <= 127 ) )
			{
				if ( ( usLength >= MB_FRAME_SIZE_MIN ) && ( usLength <= MB_FRAME_SIZE_MAX ) )
				{
					vMBEnterCritical(  );

					ucMBSlaveAddr = ucSlaveAddress;  /**< Save transmit slave address */

					ucFrameBuff[MB_FRAME_OFFS_ADDR] = ucSlaveAddress;
					ucFrameBuff[MB_FRAME_OFFS_CODE] = ucFunctionCode;

					memmove( (void *)(ucFrameBuff + 2), pucData, usLength );

					/* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
					usCRC16 = usMBCRC16( ( uint8_t * ) ucFrameBuff, usLength + 2);

					ucFrameBuff[usLength + 2] = ( uint8_t )( usCRC16 & 0xFF );
					ucFrameBuff[usLength + 3] = ( uint8_t )( usCRC16 >> 8 );

					usFrameIndx = 0;
					usFrameLeng = usLength + 4;
					eSndState	= TX_STATE_SNDC; /**< Enable transmit FSM */

					vMBExit_Critical(  );

					vMBPortSerialEnable( false, true );

					eStatus = MB_ENOERR; 
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
void vMBCDTransmitFSM( void )
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
            xMBPortSerialPutByte( ucFrameBuff[usFrameIndx] ); 
			usFrameIndx++; /* next byte in frame buffer. */
        }
        else
        {
			_OSMboxPost(&sMBMsgID, (eMBEvent = EV_FRAME_SENT, &eMBEvent));

            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            vMBPortSerialEnable( false, false );
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
void vMBCDReceiveFSM( void )
{
	uint8_t           ucByte;

	assert( eSndState == TX_STATE_IDLE ); /**< To be sure Tx is in IDLE state. */

	/* Always read the character. */
	( void )xMBPortSerialGetByte( ( int8_t * ) & ucByte );

	switch ( eRcvState )
	{
		/* If we have received a character in the init state we have to
		 * wait until the frame is finished.
		 */
		case RX_STATE_INIT:
			vMBPortTimersEnable(  );
			break;

			/* In the error state we wait until all characters in the
			 * damaged frame are transmitted.
			 */
		case RX_STATE_ERRS:
			vMBPortTimersEnable(  );
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
			vMBPortTimersEnable(  );
			break;

			/* We are currently receiving a frame. Reset the timer after
			 * every character received. If more than the maximum possible
			 * number of bytes in a modbus frame is received the frame is
			 * ignored.
			 */
		case RX_STATE_RCVC:
			if ( usFrameIndx < MB_FRAME_SIZE_MAX )
			{
				ucFrameBuff[usFrameIndx++] = ucByte;
			}
			else
			{
				eRcvState = RX_STATE_ERRS;
			}
			vMBPortTimersEnable(  );
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
void vMBCDTimerT35Expired( void )
{
	switch ( eRcvState )
	{
		/**< Timer T35 expired. Startup phase is finished. */
		case RX_STATE_INIT:	
			_OSMboxPost(&sMBMsgID, (eMBEvent = EV_READY, &eMBEvent));
			vMBPortSerialEnable(false, true);
			break;

        /* A frame was received and t35 expired. Notify the listener that a new frame was received. */
		case RX_STATE_RCVC:	
			_OSMboxPost(&sMBMsgID, (eMBEvent = EV_FRAME_RECEIVED, &eMBEvent));
			break;

        /* An error occured while receiving the frame. */
		case RX_STATE_ERRS:	
			break;

        /* Function called in an illegal state. */
		default:
			assert( ( eRcvState == RX_STATE_INIT ) ||
					( eRcvState == RX_STATE_RCVC ) || ( eRcvState == RX_STATE_ERRS ) );
	}

	vMBPortTimersDisable( );
	eRcvState = RX_STATE_IDLE;
}

