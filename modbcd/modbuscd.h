/**-----------------------------------------------------------------------------------------------------------------
 * @file	modbuscd.h
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __MBCD_MODBUSCD_H__
#define __MBCD_MODBUSCD_H__


#if defined(__cplusplus)
extern "C" {
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											 MODBUSCD - INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/

/*------------------------------------------------------------------------------------------------------------------
 * 
 *											 MODBUSCD - DATA BLOCK 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
enum MBcdMode{RTU, ASCII, TCP};

enum MBcdErrCode{
    NOERR,                  /*!< no error. */
    NOREG,                  /*!< illegal register address. */
    INVAL,                  /*!< illegal argument. */
    PORTERR,                /*!< porting layer error. */
    NORES,                  /*!< insufficient resources. */
    IO,                     /*!< I/O error. */
    ILLSTATE,               /*!< protocol stack in illegal state. */
    TIMEDOUT                /*!< timeout error occurred. */
};



typedef enum
{
    MB_PAR_NONE,                /*!< No parity. */
    MB_PAR_ODD,                 /*!< Odd parity. */
    MB_PAR_EVEN                 /*!< Even parity. */
} eMBParity;

#if defined(__cplusplus)
}
#endif


#endif /*__MBCD_MODBUSCD_H__*/
