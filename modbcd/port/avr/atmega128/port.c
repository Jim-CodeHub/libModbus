/**-----------------------------------------------------------------------------------------------------------------
 * @file	port.c
 * @brief   FreeModbus port protable functions	
 *
 * @note	RAM used : 0Bytes
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
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
static UCHAR	_SREG;	/**< For Enter & Exit critical */


/*------------------------------------------------------------------------------------------------------------------
 * 
 *  										  FUNCTIONS DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
eMBErrorCode _eMBRegInputCB     ( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs     );
eMBErrorCode _eMBRegCoilsCB_RD  ( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils    );
eMBErrorCode _eMBRegCoilsCB_WR  ( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils    );
eMBErrorCode _eMBRegDiscreteCB  ( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );
eMBErrorCode _eMBRegHoldingCB_RD( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs	 );
eMBErrorCode _eMBRegHoldingCB_WR( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs	 );


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/
void ENTER_CRITICAL_SECTION( void )	{_SREG = SREG; __asm__ volatile("cli");}
void EXIT_CRITICAL_SECTION( void )	{SREG = _SREG;}	


#if _MB_FUNC_READ_INPUT_ENABLED > 0	

/**
  @breif	   Read-Only & 16bit Access, FunCode = 0X04
  @param[in]   usAddress    - start address of request
  @param[in]   usNRegs      - quantity of registers of request 
  @param[out]  pucRegBuffer - frame data field data buffer 
  @return	   eMBErrorCode MB_ENOREG(ILL address : 0x02)/MB_ENOERR(no error)
 */
eMBErrorCode    eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( ( usAddress >= _MB_REG_INPUT_ADDR_START )
        && ( usAddress + usNRegs <= _MB_REG_INPUT_ADDR_START + _MB_REG_INPUT_QUANTITY ) )
    {
		eStatus = _eMBRegInputCB( pucRegBuffer, usAddress, usNRegs );
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

#endif //_MB_FUNC_READ_INPUT_ENABLED > 0	


#if (_MB_FUNC_READ_HOLDING_ENABLED > 0) || (_MB_FUNC_WRITE_HOLDING_ENABLED > 0) || (_MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0) \
	|| (_MB_FUNC_READWRITE_HOLDING_ENABLED > 0)

/**
  @breif	   1bit Access, For FunCode = 0X03 & 0x06 & 0x10 & 0x17
  @param[in]   usAddress    - start address of request
  @param[in]   usNRegs      - quantity of registers of request 
  @param[in]   eMode		- read or write registers 
  @param[out]  pucRegBuffer - frame data field data buffer 
  @return	   eMBErrorCode MB_ENOREG(ILL address : 0x02)/MB_ENOERR(no error)
  @note		   FreeModbus will call this function two times for write and read
 */
eMBErrorCode    eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;

	switch (eMode)
	{
		case MB_REG_READ:
			if ( (usAddress >= _MB_HOLD_RD_ADDR_START) &&
				 (usAddress + usNRegs <= _MB_HOLD_RD_ADDR_START + _MB_HOLD_RD_QUANTITY) )
			{
				eStatus = _eMBRegHoldingCB_RD( pucRegBuffer, usAddress, usNRegs);
			}
			else
			{
				eStatus = MB_ENOREG;
			}
			break;
		case MB_REG_WRITE:
			if ( (usAddress >= _MB_HOLD_WR_ADDR_START) &&
				 (usAddress + usNRegs <= _MB_HOLD_WR_ADDR_START + _MB_HOLD_WR_QUANTITY) )
			{
				eStatus = _eMBRegHoldingCB_WR( pucRegBuffer, usAddress, usNRegs);
			}
			else
			{
				eStatus = MB_ENOREG;
			}
			break;
		default: ;
	}

	return eStatus;
}

#endif //(_MB_FUNC_READ_HOLDING_ENABLED > 0) || (_MB_FUNC_WRITE_HOLDING_ENABLED > 0) || (_M....


#if (_MB_FUNC_READ_COILS_ENABLED > 0) || (_MB_FUNC_WRITE_COIL_ENABLED > 0) || (_MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0)

/**
  @breif	   1bit Access, For FunCode = 0X01 & 0x05 & 0x0F
  @param[in]   usAddress    - start address of request
  @param[in]   usNRegs      - quantity of coils of request 
  @param[in]   eMode		- read or write(multi & single) coils 
  @param[out]  pucRegBuffer - frame data field data buffer 
  @return	   eMBErrorCode MB_ENOREG(ILL address : 0x02)/MB_ENOERR(no error)
 */
eMBErrorCode    eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;

	switch (eMode)
	{
		case MB_REG_READ:
			if ( (usAddress >= _MB_COILS_RD_ADDR_START) &&
				 (usAddress + usNCoils <= _MB_COILS_RD_ADDR_START + _MB_COILS_RD_QUANTITY) )
			{
				eStatus = _eMBRegCoilsCB_RD( pucRegBuffer, usAddress, usNCoils );
			}
			else
			{
				eStatus = MB_ENOREG;
			}
			break;
		case MB_REG_WRITE:
			if ( (usAddress >= _MB_COILS_WR_ADDR_START) &&
				 (usAddress + usNCoils <= _MB_COILS_WR_ADDR_START + _MB_COILS_WR_QUANTITY) )
			{
				eStatus = _eMBRegCoilsCB_WR( pucRegBuffer, usAddress, usNCoils );
			}
			else
			{
				eStatus = MB_ENOREG;
			}
			break;
		default: ;
	}

	return eStatus;
}

#endif //(_MB_FUNC_READ_COILS_ENABLED > 0) || (_MB_FUNC_WRITE_COIL_ENABLED > 0) || (_MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0)


#if _MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0

/**
  @breif	   1bit Access, For FunCode = 0X02
  @param[in]   usAddress    - start address of request
  @param[in]   usNRegs      - quantity of discretes of request 
  @param[out]  pucRegBuffer - frame data field data buffer 
  @return	   eMBErrorCode MB_ENOREG(ILL address : 0x02)/MB_ENOERR(no error)
 */
eMBErrorCode    eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( ( usAddress >= _MB_DISC_RD_ADDR_START )
        && ( usAddress + usNDiscrete <= _MB_DISC_RD_ADDR_START + _MB_DISC_RD_QUANTITY) )
    {
		eStatus = _eMBRegDiscreteCB( pucRegBuffer, usAddress, usNDiscrete );
    }
    else
    {
        eStatus = MB_ENOREG;
    }

	return eStatus;
}

#endif //_MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0

//DEMO undefined 
//For mbfuncdiag.c callback fun
//For mbfuncother.c callback fun
