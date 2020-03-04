/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbreg.c
 * @brief   modbcd function code register 
 *
 * @note	RAM used : 0 Bytes
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
#include "mbreg.h"


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

/**
    @breif      Post message
    @param[in]  mbreg - message box ID pointer,
    @param[in]  msg - message 
    @param[out] None
    @return     None
*/
eMBErrorCode eMBRegisterCB( uint8_t ucFunctionCode, pxMBFunctionHandler pxHandler )
{
    int             i;
    eMBErrorCode    eStatus;

    if( ( 0 < ucFunctionCode ) && ( ucFunctionCode <= 127 ) )
    {
        ENTER_CRITICAL_SECTION(  );
        if( pxHandler != NULL )
        {
            for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )
            {
                if( ( xFuncHandlers[i].pxHandler == NULL ) ||
                    ( xFuncHandlers[i].pxHandler == pxHandler ) )
                {
                    xFuncHandlers[i].ucFunctionCode = ucFunctionCode;
                    xFuncHandlers[i].pxHandler = pxHandler;
                    break;
                }
            }
            eStatus = ( i != MB_FUNC_HANDLERS_MAX ) ? MB_ENOERR : MB_ENORES;
        }
        else
        {
            for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )
            {
                if( xFuncHandlers[i].ucFunctionCode == ucFunctionCode )
                {
                    xFuncHandlers[i].ucFunctionCode = 0;
                    xFuncHandlers[i].pxHandler = NULL;
                    break;
                }
            }
            /* Remove can't fail. */
            eStatus = MB_ENOERR;
        }
        EXIT_CRITICAL_SECTION(  );
    }
    else
    {
        eStatus = MB_EINVAL;
    }
    return eStatus;
}

