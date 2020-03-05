/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbcd_box.c
 * @brief   modbcd system message box 
 *
 * @note	RAM used : 0 Bytes
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
#include "mbcd_box.h"


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
    @param[in]  boxID	- message box ID pointer,
    @param[in]  msg		- message 
    @param[out] None
    @return     None
*/
void	vMBCD_BoxPost( tMBCD_Event *boxID, void *msg )
{
	boxID -> msg = msg;

	return;
}

/**
    @brief      Accept message
    @param[in]  boxID	- message box ID pointer,
    @param[out] None
    @return     Message 
*/
void	*pvMBCD_BoxAccept( tMBCD_Event *boxID )
{
	void *msg = (void *)0;

	if ( (void *)0 != boxID->msg ) /**< To avoid read signal frequently */
	{
		OS_ATOMIC_OPERATION(
				msg = boxID->msg;
				boxID->msg = (void *)0;
				);
	}

	return msg;
}


