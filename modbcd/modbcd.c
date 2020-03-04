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
 * @note	RAM used : 0Bytes
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
#include "modbcd.h"


/*
--------------------------------------------------------------------------------------------------------------------
*
*											  VARIABLES DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/
static			_OS_EVENT				sMBMsgID;								/**< Modbcd message box identity  */
static			eMBEventType			eMBEvent;								/**< Modbcd Event type			  */	

static volatile	BOOL					xSendFlg;								/**< Modbcd data send flag        */
static volatile	BOOL					xRecvFlg;								/**< Modbcd data load flag		  */

static volatile eMBRcvState				eRcvState;								/**< Modbcd recv state			  */
static volatile eMBSndState				eSndState;								/**< Modbcd send state			  */

static  volatile UCHAR					ucFrameBuff[MB_SIZE_FRAME_MAX];			/**< Modbcd frame buffer          */
static  volatile USHORT					usFrameIndx;							/**< Modbcd frame buffer Index	  */
static  volatile USHORT					usFrameLeng;							/**< Mddbcd frame buffer length   */

static volatile UCHAR					ucMBSwitch;								/**< Modbcd enable or disable     */

static			xMBFunctionHandler		xFuncHandlers[MB_FUNC_HANDLERS_MAX];  	/**< Modbcd functions handlers    */

static volatile UCHAR					ucMBSlaveAddr;							/**< Modbcd Address field value   */


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
eMBErrorCode eMBCDInit( UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity, USHORT usTimeOut ) 
{
    eMBErrorCode    eStatus = MB_ENOERR;
    ULONG           usTimerT35_50us;

    ENTER_CRITICAL(  );

    /* Modbus RTU uses 8 Databits. */
    if( xMBPortSerialInit( ucPort, ulBaudRate, 8, eParity ) != TRUE )
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
        if( xMBPortTimersInit( ( USHORT ) usTimerT35_50us ) != TRUE )
        {
            eStatus = MB_EPORTERR;
        }
    }

    EXIT_CRITICAL_SECTION(  );

#if MB_FUNC_DIAG_REP_SLAVEID_ENABLED > 0
	( eMBErrorCode ) eMBRegisterCB( MB_FUNC_OTHER_REPORT_SLAVEID , eMBFuncReportSlaveID );
#endif
#if MB_FUNC_READ_INPUT_ENABLED > 0
	( eMBErrorCode ) eMBRegisterCB( MB_FUNC_READ_INPUT_REGISTER, eMBFuncReadInputRegister );
#endif
#if MB_FUNC_READ_HOLDING_ENABLED > 0
	( eMBErrorCode ) eMBRegisterCB( MB_FUNC_READ_HOLDING_REGISTER, eMBFuncReadHoldingRegister );
#endif
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
	( eMBErrorCode ) eMBRegisterCB( MB_FUNC_WRITE_MULTIPLE_REGISTERS, eMBFuncWriteMultipleHoldingRegister );
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
	( eMBErrorCode ) eMBRegisterCB( MB_FUNC_WRITE_REGISTER, eMBFuncWriteHoldingRegister );
#endif
#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0
	( eMBErrorCode ) eMBRegisterCB( MB_FUNC_READWRITE_MULTIPLE_REGISTERS, eMBFuncReadWriteMultipleHoldingRegister );
#endif
#if MB_FUNC_READ_COILS_ENABLED > 0
	( eMBErrorCode ) eMBRegisterCB( MB_FUNC_READ_COILS, eMBFuncReadCoils );
#endif
#if MB_FUNC_WRITE_COIL_ENABLED > 0
	( eMBErrorCode ) eMBRegisterCB( MB_FUNC_WRITE_SINGLE_COIL, eMBFuncWriteCoil );
#endif
#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
	( eMBErrorCode ) eMBRegisterCB( MB_FUNC_WRITE_MULTIPLE_COILS, eMBFuncWriteMultipleCoils );
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
	( eMBErrorCode ) eMBRegisterCB( MB_FUNC_READ_DISCRETE_INPUTS, eMBFuncReadDiscreteInputs );
#endif

	ucMBSwitch = FALSE; /**< Disable modbcd */

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

	if ( FALSE == ucMBSwitch )
	{
		/**< Active the protocol stack */

		ENTER_CRITICAL(  );
		/* Initially the receiver is in the state STATE_RX_INIT. we start
		 * the timer and if no character is received within t3.5 we change
		 * to STATE_RX_IDLE. This makes sure that we delay startup of the
		 * modbus protocol stack until the bus is free.
		 */
		eRcvState = STATE_RX_INIT;
		vMBPortSerialEnable( TRUE, FALSE );
		vMBPortTimersEnable(  );

		EXIT_CRITICAL(  );

		ucMBSwitch = TRUE;	
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

	if ( TRUE == ucMBSwitch )
	{
		ENTER_CRITICAL_SECTION(  );
		vMBPortSerialEnable( FALSE, FALSE );
		vMBPortTimersDisable(  );
		EXIT_CRITICAL_SECTION(  );

		ucMBSwitch = FALSE;
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
				3. MB_ENORES	-	if received frame error
*/
eMBErrorCode eMBPoll( void )
{
    int             i;
    eMBErrorCode    eStatus = MB_ENOERR;
    eMBEventType    *eEvent;

	if( TRUE == ucMBSwitch ) /**< Check if the protocol stack is ready. */
	{
		/* Check if there is a event available. If not return control to caller.
		 * Otherwise we will handle the event. */
		if( (void *)0 != (eEvent = (eMBEventType *)OSMboxAccept( &sMBMsgPoll)) )
		{
			switch ( *eEvent )
			{
				case EV_READY:
					xSendFlg = TRUE; /**< Enable 'eMBSend()' function */
					break;

				case EV_FRAME_RECEIVED:
					ENTER_CRITICAL();
					/**< Length , Address and CRC check */
					if ( ( usFrameIndx >= MB_FRAME_SIZE_MIN ) && ( usMBCRC16( ( UCHAR * ) ucFrameBuff, usFrameIndx ) == 0 )  && ( ucFrameBuff[MB_FRAME_OFFS_ADDR] == ucMBSlaveAddr ) ) 
					{
						EXIT_CRITICAL();

						for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )
						{
							/* No more function handlers registered. Abort. */
							if( xFuncHandlers[i].ucFunctionCode == 0 )
							{
								break;
							}
							else if( xFuncHandlers[i].ucFunctionCode == ucFunctionCode )
							{
								if ( MB_ENOERR == (eStatus = xFuncHandlers[i].pxHandler( (UCHAR *)&ucFrameBuff[MB_FRAME_OFFS_DATA], &usFrameLeng)) )
								{
									xRecvFlg = TRUE; /**< Enable receive  for function 'eMBCDLoad()' */
									xSendFlg = TRUE; /**< Enable transmit for function 'eMBCDSend()' */
								}
								break;
							}
						}
					}
					else
					{
						EXIT_CRITICAL();
						eStatus = MB_ENORES;
					}
					break;

				case EV_EXECUTE:
					break;

				case EV_FRAME_SENT:
					if ( MB_ADDR_BROADCAST != ucFrameBuff[MB_FRAME_OFFS_ADDR] )
					{
						vMBPortSerialEnable( TRUE, FALSE );
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
eMBErrorCode eMBCDLoad( UCHAR **pucData, USHORT *pusLeng )
{
    eMBErrorCode    eStatus = MB_EILLSTATE;

	if ( TRUE == xRecvFlg ) 
	{
		xRecvFlg = FALSE;

		*pucData = ucFrameBuff[MB_FRAME_OFFS_DATA];
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
eMBErrorCode eMBCDSend( UCHAR ucSlaveAddress, UCHAR ucFunctionCode, const UCHAR *pucData, USHORT usLength )
{
    eMBErrorCode    eStatus = MB_EINVAL;
    USHORT          usCRC16;

	if ( TRUE == xSendFlg )
	{
		xSendFlg = FALSE;

		/**< Check preconditions and packetize frame */	
		if ( ( ucSlaveAddress >= MB_ADDR_BROADCAST ) && ( ucSlaveAddress <= MB_ADDR_SLAVE_MAX ) )
		{
			if ( ( 0 <= ucFunctionCode ) && ( ucFunctionCode <= 127 ) )
			{
				if ( ( usLength >= MB_FRAME_SIZE_MIN ) && ( usLength <= MB_FRAME_SIZE_MAX ) )
				{
					ENTER_CRITICAL(  );

					ucMBSlaveAddr = ucSlaveAddress;  /**< Save transmit slave address */

					ucFrameBuff[MB_FRAME_OFFS_ADDR] = ucSlaveAddress;
					ucFrameBuff[MB_FRAME_OFFS_CODE] = ucFunctionCode;

					memmove( ucFrameBuff + 2, pucData, usLength );

					/* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
					usCRC16 = usMBCRC16( ( UCHAR * ) ucFrameBuff, usLength + 2);

					ucFrameBuff[usLength + 2] = ( UCHAR )( usCRC16 & 0xFF );
					ucFrameBuff[usLength + 3] = ( UCHAR )( usCRC16 >> 8 );

					usFrameIndx = 0;
					usFrameLeng = usLength + 4;
					eSndState	= TX_STATE_SNDC; /**< Enable transmit FSM */

					EXIT_CRITICAL(  );

					vMBPortSerialEnable( FLASE, TRUE );

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
            vMBPortSerialEnable( FALSE, FALSE );
            eSndState = STATE_TX_IDLE;
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
	UCHAR           ucByte;

	assert( eSndState == TX_STATE_IDLE ); /**< To be sure Tx is in IDLE state. */

	/* Always read the character. */
	( void )xMBPortSerialGetByte( ( CHAR * ) & ucByte );

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
			vMBPortSerialEnable(FALSE, TRUE);
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

