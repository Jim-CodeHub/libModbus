/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbcd_box.h
 * @brief   modbcd system message box 
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#ifndef _MBCD_BOX_H__
#define _MBCD_BOX_H__


#ifdef __cplusplus
	extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MODBCD BOX INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include "modbcd.h"


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   MBCD_BOX SHORT ALIAS	
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  MBCD_EVENT_MAX						 2


#define  MBCD_ATOMIC_OPERATION(__INS__)	 {   \
	vMBCD_EnterCritical();					 \
                                             \
	__INS__							  	     \
                                             \
	vMBCD_Exit_Critical();					 \
}


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   MBCD_BOX DATA BLOCKS 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
typedef struct { void *msg; } tMBCD_Event;


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											MBCD_BOX FUN DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
void		 vMBCD_BoxPost(tMBCD_Event *boxID, void *msg);
void		 *pvMBCD_BoxAccept(tMBCD_Event *boxID);


#ifdef __cplusplus
	} //extern "C"
#endif


#endif //_MBCD_BOX_H__

