/**-----------------------------------------------------------------------------------------------------------------
 * @file	mbdata.h
 * @brief   modbus official data center, about : Parity, Frame, Exception, ErroCode .etc
 *
 * Copyright (c) 2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#ifndef _MBDATA_H__
#define _MBDATA_H__


#ifdef __cplusplus
	extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												MBDATA INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   MBDATA SHORT ALIAS	
 *
 *------------------------------------------------------------------------------------------------------------------
*/
																			/******** Frame defination ************/
#define  MB_FRAME_SIZE_MAX							256						/**< Frame maximum size				  */
#define  MB_FRAME_SIZE_MIN							4						/**< Frame minimum size				  */	
#define  MB_FRAME_SIZE_CRC							2						/**< Frame CRC field size			  */
#define  MB_FRAME_OFFS_ADDR							0						/**< Address field offset in frame	  */
#define  MB_FRAME_OFFS_CODE							1						/**< FunCode field offset in frame	  */
#define  MB_FRAME_OFFS_DATA							2						/**< Data    field offset in frame	  */


																			/******** Address defination **********/
#define MB_ADDR_BROADCAST							0					    /**< Modbus broadcast address		  */
#define MB_ADDR_SLAVE_MIN							1						/**< Smallest possible slave address  */
#define MB_ADDR_SLAVE_MAX							247						/**< Biggest possible slave address   */

																			/******** FunCode defination **********/
#define MB_FUNC_NONE							    0
#define MB_FUNC_READ_COILS                    	    1
#define MB_FUNC_READ_DISCRETE_INPUTS          	    2
#define MB_FUNC_WRITE_SINGLE_COIL             	    5
#define MB_FUNC_WRITE_MULTIPLE_COILS          	   15
#define MB_FUNC_READ_HOLDING_REGISTER         	    3
#define MB_FUNC_READ_INPUT_REGISTER           	    4
#define MB_FUNC_WRITE_REGISTER                	    6
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      	   16
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS  	   23
#define MB_FUNC_DIAG_READ_EXCEPTION           	    7
#define MB_FUNC_DIAG_DIAGNOSTIC               	    8
#define MB_FUNC_DIAG_GET_COM_EVENT_CNT        	   11
#define MB_FUNC_DIAG_GET_COM_EVENT_LOG        	   12
#define MB_FUNC_DIAG_REPORT_SLAVEID           	   17
#define MB_FUNC_ERROR                         	   128


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   MBDATA DATA BLOCKS 
 *
 *------------------------------------------------------------------------------------------------------------------
*/

typedef enum
{
    MB_PAR_NONE,                /*!< No parity. */
    MB_PAR_ODD,                 /*!< Odd parity. */
    MB_PAR_EVEN                 /*!< Even parity. */
} eMBParity;

typedef enum
{
    MB_ENOERR,                  /*!< no error. */
    MB_ENOREG,                  /*!< illegal register address. */
    MB_EINVAL,                  /*!< illegal argument. */
    MB_EPORTERR,                /*!< porting layer error. */
    MB_ENORES,                  /*!< insufficient resources. */
    MB_EIO,                     /*!< I/O error. */
    MB_EILLSTATE,               /*!< protocol stack in illegal state. */
    MB_ETIMEDOUT                /*!< timeout error occurred. */
} eMBErrorCode;


#ifdef __cplusplus
	} //extern "C"
#endif


#endif //_MBDATA_H__

