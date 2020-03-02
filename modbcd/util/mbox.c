/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbox.c
 * @brief   system message box 
 *
 * @note	RAM used : 0 Bytes
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
#include "mbox.h"


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
    @param[in]  mbox - message box ID pointer,
    @param[in]  msg - message 
    @param[out] None
    @return     None
*/
void _OSMboxPost(struct _os_mbox *mbox, void *msg) 
{
	mbox->msg = msg;

	return;
}

/**
    @brief      Accept message
    @param[in]  mbox - message box ID pointer,
    @param[out] None
    @return     Message 
*/
void *_OSMboxAccept(struct _os_mbox *mbox)
{
	void *msg = (void *)0;

	if ((void *)0 != mbox->msg) /**< To avoid read signal frequently */
	{
		OS_ATOMIC_OPERATION(
				msg = mbox->msg;
				mbox->msg = (void *)0;
				);
	}

	return msg;
}

