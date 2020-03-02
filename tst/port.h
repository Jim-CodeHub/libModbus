/**-----------------------------------------------------------------------------------------------------------------
 *
 * @file	port.h
 * @brief   FreeModbus portable header	
 *
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef _PORT_H__
#define _PORT_H__


/*------------------------------------------------------------------------------------------------------------------
 * 
 *												PORT INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   PORT SHORT ALIAS	
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define PR_BEGIN_EXTERN_C						extern "C" {			/**< For c++ programming                  */
#define	PR_END_EXTERN_C							}

#define	INLINE									inline					/**< For 'inline' functions				  */

#define assert( x )														/**< For code check (empty now)			  */

typedef char									BOOL;										
typedef unsigned char							UCHAR;
typedef char									CHAR;
typedef unsigned short							USHORT;
typedef short									SHORT;
typedef unsigned long							ULONG;
typedef long									LONG;

#ifndef TRUE
#define TRUE									1
#endif

#ifndef FALSE
#define FALSE									0
#endif


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   PORT CONFIG 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
																		/**<---------- Trans mode config **********/
#define _MB_ASCII_ENABLED                       (  1 )					/**< Cfg ASCII code enable                */
#define _MB_RTU_ENABLED                         (  1 )					/**< Cfg RTU code enable				  */
#define _MB_TCP_ENABLED                         (  0 )					/**< Cfg TCP code enable				  */

																		/**<---------- TimeOut (ASC) config *******/
#define _MB_ASCII_TIMEOUT_SEC                   (  1 )					/**< Cfg timeout (s) for ascii mode		  */
#define _MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS   (  0 )					/**< Cfg timeout (ms) before trans enable */	

																		/**<---------- Stack config ***************/
#define _MB_FUNC_HANDLERS_MAX                   ( 10 )					/**< Cfg Max size of funCode stack		  */
#define _MB_FUNC_OTHER_REP_SLAVEID_BUF          ( 32 )					/**< Cfg for 'eMBSetSlaveID()'			  */

																		/**<---------- Functions config ***********/
#define _MB_FUNC_OTHER_REP_SLAVEID_ENABLED      (  0 )					/**< Fun 0X11: Report Slave ID			  */
#define _MB_FUNC_READ_INPUT_ENABLED             (  1 )					/**< Fun 0X04: Read Input Registers		  */
#define _MB_FUNC_READ_HOLDING_ENABLED           (  1 )					/**< Fun 0X03: Read Holding Registers	  */
#define _MB_FUNC_WRITE_HOLDING_ENABLED          (  1 )					/**< Fun 0X06: Write Single Registers	  */
#define _MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED (  1 )					/**< Fun 0X10: Write Multiple Registers	  */
#define _MB_FUNC_READ_COILS_ENABLED             (  1 )					/**< Fun 0X01: Read Coils				  */
#define _MB_FUNC_WRITE_COIL_ENABLED             (  1 )					/**< Fun 0X05: Write Coil				  */
#define _MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED   (  1 )					/**< Fun 0X0F: Write Multiple Coils       */
#define _MB_FUNC_READ_DISCRETE_INPUTS_ENABLED   (  1 )					/**< Fun 0X02: Read Discrete Inputs		  */
#define _MB_FUNC_READWRITE_HOLDING_ENABLED      (  1 )					/**< Fun 0X17: Read/Write Multiple Regs	  */


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											   USER'S CONFIG 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define _MB_REG_INPUT_ADDR_START				1000					/**< FunCode 0x04			- Address	  */
#define _MB_REG_INPUT_QUANTITY				    10						/**< FunCode 0X04			- Quantity	  */
#define _MB_COILS_RD_ADDR_START					2000					/**< FunCode 0X01			- Address	  */
#define _MB_COILS_RD_QUANTITY					20						/**< FunCode 0X01			- Quantity	  */
#define _MB_COILS_WR_ADDR_START					3000					/**< FunCode 0X05/0X0F		- Address	  */
#define _MB_COILS_WR_QUANTITY					30						/**< FunCode 0X05/0X0F		- Quantity    */
#define _MB_DISC_RD_ADDR_START					4000					/**< FunCode 0X02			- Address	  */
#define _MB_DISC_RD_QUANTITY					40						/**< FunCode 0X02			- Quantity	  */
#define _MB_HOLD_RD_ADDR_START					5000					/**< FunCode 0x03/0x17		- Address	  */
#define _MB_HOLD_RD_QUANTITY					50						/**< FunCode 0x03/0x17		- Quantity	  */
#define _MB_HOLD_WR_ADDR_START					6000					/**< FunCode 0x06/0X10/0x17 - Address     */
#define _MB_HOLD_WR_QUANTITY					60						/**< FunCode 0x06/0X10/0x17 - Quantity    */ 


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											PORT FUN DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
void ENTER_CRITICAL( void );
void EXIT_CRITICAL( void ) ;


#endif //_PORT_H__

