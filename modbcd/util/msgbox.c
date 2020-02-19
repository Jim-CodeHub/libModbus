/**-----------------------------------------------------------------------------------------------------------------
 * @file	msgbox.c
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
#include "msgbox.h"


/*
--------------------------------------------------------------------------------------------------------------------
*
		                                      FUNCTION PROTOTYPES
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
    @brief      Post message
    @param[in]  mbox - message box ID pointer,
				msg - message	
    @param[out] None
    @return     None
*/
void _OSMboxPost(struct _os_mbox *mbox, void *msg) 
{
	mbox->msg = msg;

	return;
}

/**
    @brief      Post message
    @param[in]  mbox - message box ID pointer,
    @param[out] None
    @return     Message 
*/
void *_OSMboxAccept(struct _os_mbox *mbox)
{
	void *msg = (void *)0;

	if ((void *)0 != mbox->msg) /**< To avoid read signal frequently */
	{
		MBCD_ENTER_CRITICAL();

		msg = mbox->msg;
		mbox->msg = (void *)0;

		MBCD_EXIT_CRITICAL();
	}

	return msg;
}


