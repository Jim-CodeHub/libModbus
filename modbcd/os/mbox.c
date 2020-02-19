/*
*********************************************************************************************************
*                                                 OS LIB 
*
*                          (c) Copyright 2018, Jim, All Rights Reserved
*
*                           Resource statistics:
*                                - RAM used    : 1+OS_MBOX_MAX Byte
*                                - Flash used  : 0Byte
*                                - EEPROM used : 0KByte
*
* @file   : mbox.c
* @brief  : message box
* @author : Jim zhang 
*********************************************************************************************************
*/
#include "mbox.h"


/*
*********************************************************************************************************
*
*                                       VARIABLES DECLARATION
*
*********************************************************************************************************
*/
static _OS_EVENT s_ucOsMbox_value[OS_MBOX_MAX]; 
static int8_t s_cOsMbox_index = -1;


/*
*********************************************************************************************************
*
*                                       FUNCTIONS IMPLEMENT
*
*********************************************************************************************************
*/
/**
    @fn         Create message box 
    @param[in]  None
    @param[out] None
    @return     Message box ID pointer 
*/
struct _os_mbox *_OSMboxCreate(void)
{
	struct _os_mbox *mbox = (void *)0;

	s_cOsMbox_index++;
	if (OS_MBOX_MAX != s_cOsMbox_index)
	{
		mbox = &s_ucOsMbox_value[s_cOsMbox_index];	
	}

	return mbox;
}

/**
    @fn         Post message
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
    @fn         Post message
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


