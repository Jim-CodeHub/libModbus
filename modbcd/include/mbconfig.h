/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbconfig.h
 * @brief   modbus macros configuration 
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#ifndef _MBCONFIGT_H__
#define _MBCONFIGT_H__


#ifdef __cplusplus
	extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MBCONFIGT INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   MBCONFIGT SHORT ALIAS	
 *
 *------------------------------------------------------------------------------------------------------------------
*/

#define  MB_FUNC_DIAG_REP_SLAVEID_BUF				32						/**< For Function 'eMBSetSlaveID()'  */


#define  MB_FUNC_DIAG_REP_SLAVEID_ENABLED			0						/**< FunCode 0x11 code switch		  */
#define  MB_FUNC_READ_INPUT_ENABLED					0						/**< FunCode 0x04 code switch		  */
#define  MB_FUNC_READ_HOLDING_ENABLED				0						/**< FunCode 0x03 code switch		  */
#define  MB_FUNC_WRITE_HOLDING_ENABLED				0						/**< FunCode 0x06 code switch		  */
#define  MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED		0						/**< FunCode 0x10 code switch		  */
#define	 MB_FUNC_READWRITE_HOLDING_ENABLED			0						/**< FunCode 0x17 code switch		  */
#define  MB_FUNC_READ_DISCRETE_INPUTS_ENABLED       0						/**< FunCode 0x02 code switch		  */ 
#define  MB_FUNC_READ_COILS_ENABLED                 1 						/**< FunCode 0x01 code switch		  */ 
#define  MB_FUNC_WRITE_COIL_ENABLED                 1  						/**< FunCode 0x05 code switch		  */ 
#define  MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED       1  						/**< FunCode 0x0F code switch		  */ 

#define  MB_FUNC_HANDLERS_MAX                       11						/**< FunCode Number of Enable above +    
																				 user's define					  */	


#ifdef __cplusplus
	} //extern "C"
#endif


#endif //_MBCONFIGT_H__

