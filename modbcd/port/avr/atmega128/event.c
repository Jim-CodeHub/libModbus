/**-----------------------------------------------------------------------------------------------------------------
 * @file	event.c
 * @brief   FreeModbus event protable functions	
 *
 * @note	RAM used : 2Bytes
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include "mb.h"
#include "mbport.h"


/*
--------------------------------------------------------------------------------------------------------------------
*
*											  VARIABLES DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/
static eMBEventType eQueuedEvent;
static BOOL        xEventInQueue;


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/
BOOL xMBPortEventInit( void )
{
    xEventInQueue = FALSE;
    return TRUE;
}

BOOL xMBPortEventPost( eMBEventType eEvent )
{
    xEventInQueue = TRUE;
    eQueuedEvent = eEvent;
    return TRUE;
}

BOOL xMBPortEventGet( eMBEventType * eEvent )
{
    BOOL xEventHappened = FALSE;

    if( xEventInQueue )
    {
        *eEvent = eQueuedEvent;
        xEventInQueue = FALSE;
        xEventHappened = TRUE;
    }
    return xEventHappened;
}

