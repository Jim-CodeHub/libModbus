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
#include "mbcd_box.h"
#include "mbcd_crc.h"


/*
--------------------------------------------------------------------------------------------------------------------
*
*											  VARIABLES DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/
static			tMBCD_Event				sMBMsgID;								/**< Modbcd message box identity  */
static			eMBCD_EventType			eMBEvent;								/**< Modbcd Event type			  */	

static volatile	bool					xSendFlg;								/**< Modbcd data send flag        */
static volatile	bool					xRecvFlg;								/**< Modbcd data load flag		  */

static volatile eMBCD_RcvState			eRcvState;								/**< Modbcd recv state			  */
static volatile eMBCD_SndState			eSndState;								/**< Modbcd send state			  */

static volatile uint8_t					ucFrameBuff[MBCD_FRAME_SIZE_MAX];			/**< Modbcd frame buffer          */
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
    @param[in]  ucPort			-	MAY BE USEFUL for function 'xMBCD_PortSerialInit()'
    @param[in]  ulBaudRate		-	baudrate 
    @param[in]  eParity			-   parity	
    @param[in]  usTimeOut		-   Host timeout setting ( ms )
    @param[out] None
    @return     eMBCD_ErrorCode 
				1. ERR_NOERR	-	if there is no error occur
				2. ERR_PORTERR  -	if serial or timer port init error
*/
eMBCD_ErrorCode eMBCD_Init( uint8_t ucPort, uint32_t ulBaudRate, eMBCD_Parity eParity, uint16_t usTimeOut ) 
{
    eMBCD_ErrorCode    eStatus = ERR_NOERR;
    uint32_t        usTimerT35_50us;

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
        if( xMBCD_PortTimersInit( ( uint16_t ) usTimerT35_50us ) != true )
        {
            eStatus = ERR_PORTERR;
        }
    }

    vMBCD_Exit_Critical(  );

	ucMBSwitch = false; /**< Disable modbcd */

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
    eMBCD_ErrorCode    eStatus = ERR_NOERR;

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
		vMBCD_PortTimersEnable(  );

		vMBCD_Exit_Critical(  );

		ucMBSwitch = true;	
	}
	else
	{
		eStatus = ERR_ILLSTATE;
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
    eMBCD_ErrorCode    eStatus = ERR_NOERR;

	if ( true == ucMBSwitch )
	{
		vMBCD_EnterCritical(  );
		vMBCD_PortSerialEnable( false, false );
		vMBCD_PortTimersDisable(  );
		vMBCD_Exit_Critical(  );

		ucMBSwitch = false;
	}
	else
	{
		eStatus = ERR_ILLSTATE;
	}

    return eStatus;
}

/**
    @breif      Modbcd events loop 
    @param[in]  None 
    @param[out] None
    @return     eMBCD_ErrorCode 
				1. ERR_NOERR	-	if there is no error occur
				2. ERR_ILLSTATE -	if modbcd has not been enabled yet
*/
eMBCD_ErrorCode eMBCD_Poll( void )
{
    eMBCD_ErrorCode    eStatus = ERR_NOERR;
    eMBCD_EventType	  *eEvent;

	if( true == ucMBSwitch ) /**< Check if the protocol stack is ready. */
	{
		/* Check if there is a event available. If not return control to caller.
		 * Otherwise we will handle the event. */
		if( (void *)0 != (eEvent = (eMBCD_EventType *)pvMBCD_BoxAccept( &sMBMsgID )) )
		{
			switch ( *eEvent )
			{
				case EV_READY:
					xSendFlg = true; /**< Enable 'eMBSend()' function */
					break;

				case EV_FRAME_RECEIVED:
					vMBCD_EnterCritical();

					/**< Length , Address and CRC check */
					if ( ( usFrameIndx >= MBCD_FRAME_SIZE_MIN + 1 ) 
							&& ( usMBCD_CRC16( ( uint8_t * ) ucFrameBuff, usFrameIndx ) == 0 ) 
							&& ( ucFrameBuff[MBCD_FRAME_OFFS_ADDR] == ucMBSlaveAddr ) ) 
					{
						xRecvFlg = true; /**< Enable receive  for function 'eMBCDLoad()' */
						xSendFlg = true; /**< Enable transmit for function 'eMBCDSend()' */
					}
					else
					{
						eStatus = ERR_NORES;
					}

					vMBCD_Exit_Critical();
					break;

				case EV_EXECUTE:
					break;

				case EV_FRAME_SENT:
					if ( MBCD_ADDR_BROADCAST != ucFrameBuff[MBCD_FRAME_OFFS_ADDR] )
					{
						vMBCD_PortSerialEnable( true, false );
						//+开启超时
					}
					break;
				default: ;
			}
		}
	}
	else
	{
		eStatus = ERR_ILLSTATE;
	}

    return eStatus;
}

/**
    @breif      Modbcd data field load 
    @param[in]  None
    @param[out] pucData		-   modbus data field buffer pointer 
    @param[out] pusLeng		-   modbus data field buffer length 
    @return     eMBCD_ErrorCode 
				1. ERR_NOERR	-	if there is no error occur
				2. ERR_ILLSTATE -	if modbus frame has not received and processed 
*/
eMBCD_ErrorCode eMBCD_Load( uint8_t **pucData, uint16_t *pusLeng )
{
    eMBCD_ErrorCode    eStatus = ERR_ILLSTATE;

	if ( true == xRecvFlg ) 
	{
		xRecvFlg = false;

		*pucData = (uint8_t *)&ucFrameBuff[MBCD_FRAME_OFFS_DATA];
		*pusLeng = usFrameLeng;

		eStatus = ERR_NOERR;
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
				2. ERR_INVAL	-	if param 'ucSlaveAddress' or 'ucFunctionCode' or 'usLength' error 

	@note		The function DO NOT CHECK user's data field which refer to param 'pucData'
*/
eMBCD_ErrorCode eMBCD_Send( uint8_t ucSlaveAddress, uint8_t ucFunctionCode, const uint8_t *pucData, uint16_t usLength )
{
    eMBCD_ErrorCode    eStatus = ERR_INVAL;
    uint16_t		   usCRC16;

	if ( true == xSendFlg )
	{
		xSendFlg = false;

		/**< Check preconditions and packetize frame */	
		if ( ( ucSlaveAddress >= MBCD_ADDR_BROADCAST ) && ( ucSlaveAddress <= MBCD_ADDR_SLAVE_MAX ) )
		{
			if ( ( 0 <= ucFunctionCode ) && ( ucFunctionCode <= 127 ) )
			{
				if ( ( usLength >= MBCD_FRAME_SIZE_MIN ) && ( usLength <= MBCD_FRAME_SIZE_MAX ) )
				{
					vMBCD_EnterCritical(  );

					ucMBSlaveAddr = ucSlaveAddress;  /**< Save transmit slave address */

					ucFrameBuff[MBCD_FRAME_OFFS_ADDR] = ucSlaveAddress;
					ucFrameBuff[MBCD_FRAME_OFFS_CODE] = ucFunctionCode;

					memmove( (void *)(ucFrameBuff + 2), pucData, usLength );

					/* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
					usCRC16 = usMBCD_CRC16( ( uint8_t * ) ucFrameBuff, usLength + 2);

					ucFrameBuff[usLength + 2] = ( uint8_t )( usCRC16 & 0xFF );
					ucFrameBuff[usLength + 3] = ( uint8_t )( usCRC16 >> 8 );

					usFrameIndx = 0;
					usFrameLeng = usLength + 4;
					eSndState	= TX_STATE_SNDC; /**< Enable transmit FSM */

					vMBCD_Exit_Critical(  );

					vMBCD_PortSerialEnable( false, true );

					eStatus = ERR_NOERR; 
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
			vMBCD_BoxPost(&sMBMsgID, (eMBEvent = EV_FRAME_SENT, &eMBEvent));

            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            vMBCD_PortSerialEnable( false, false );
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
			vMBCD_PortTimersEnable(  );
			break;

			/* In the error state we wait until all characters in the
			 * damaged frame are transmitted.
			 */
		case RX_STATE_ERRS:
			vMBCD_PortTimersEnable(  );
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
			vMBCD_PortTimersEnable(  );
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
			vMBCD_PortTimersEnable(  );
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
			vMBCD_BoxPost(&sMBMsgID, (eMBEvent = EV_READY, &eMBEvent));
			vMBCD_PortSerialEnable(false, true);
			break;

        /* A frame was received and t35 expired. Notify the listener that a new frame was received. */
		case RX_STATE_RCVC:	
			vMBCD_BoxPost(&sMBMsgID, (eMBEvent = EV_FRAME_RECEIVED, &eMBEvent));
			break;

        /* An error occured while receiving the frame. */
		case RX_STATE_ERRS:	
			break;

        /* Function called in an illegal state. */
		default:
			assert( ( eRcvState == RX_STATE_INIT ) ||
					( eRcvState == RX_STATE_RCVC ) || ( eRcvState == RX_STATE_ERRS ) );
	}

	vMBCD_PortTimersDisable( );
	eRcvState = RX_STATE_IDLE;
}

