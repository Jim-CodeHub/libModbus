/**-----------------------------------------------------------------------------------------------------------------
 * @file	rtud.c
 * @brief	Modbus RTU mode server/slave handler 
 * @note	Bit per Byte : 
 *					1     start  bit
 *					8	  data   bit, least significant bit (LSB) sent first
 *					1 / 0 parity bit (default EVEN)
 *					1 / 2 stop   bit
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <modbcd/comm/server/rtud.h>


#if  ((MBCD_CFG_MS_SEL == 0) && (MBCD_CFG_MOD_RTU_EN > 0)) //Slave and ascii mode enabled

/*
--------------------------------------------------------------------------------------------------------------------
*
*											  VARIABLES DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/
static struct rtud_state stat_s = {.state = RTUD_STATE_IDLE};


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief		Init Modbus Ascii server
 *	@param[in]	buff		-   Recive buffer 
 *	@param[in]	size		-   Size of recive buffer 
 *	@param[in]  recv		-	Method to recive character from RS232/RS485/USART ...
 *	@param[in]  send		-	Method to send character to RS232/RS485/USART ...
 *	@param[in]  exec		-	Method to execute action 
 *	@param[out] None	
 *	@return		None
 *	@note		1. The param 'size' SHOULD BE larger enough
 *				2. The param 'exec' SHOULD BE :
 *					a). Check whether function code and data field is valid or not
 *					b). Execute action
 *					c). Do normal/exception response (rtud_emit() can be called)
 **/
void rtud_init(unsigned char *buff, unsigned short size, pFun_recv recv, pFun_exec exec)
{
	stat_s.pInit = buff;
	stat_s.pInxt = buff;

	stat_s._size = size;

	stat_s.serial_recv = recv;
	stat_s.serial_exec = exec;

	return;
}

/**
 *	@brief	    Recive Modbus client data
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None	
 *	@note		Put the function into the loop or interrupt depends on the implementation of 'serial_recv()'	
 **/
//接收方，要计算15和35，为此要维护一个/两个计时器，该计时器在循环或者定时器中累加，计时大于15停止/大于35停止，
//接收方在收取一个帧中两个字节之间判断时间是否大于15，大于则丢弃，否则判断是否大于35，大于则接受完成，
//为避免计时干扰，应该要用到类似UCOS中的ENTER——CRITICAL和EXIT——CRITCAL
//
//如上所述，如果将RECV放入串口接受中断中，则收到一个BYTE后，是可以进行15判断的，因为收到第二个字节时判断15计数器就行了
//，但是怎么进行35判断呢？串口没有了数据，就不再触发中断，RCVE就停住了，只有客户端发送下一个帧的时候才能进行上一个帧
//的判断。 
//
//所以，RECV应该放入一个定时器等中断或一个轮询中，因为串口是可以通过轮询读取数据的，这就要求定时器的中断速度或者轮询速度
//要足够快，最起码一个LOOP的时间要小于15。
//
//
//
//发送方，不计算15，要尽可能快的循环发送，为此可能要用到类似UCOS中的ENTER_CRITICAL()和EXIT——CRITICAL（），避免一个帧的发送
//被打断，要计算35，35之后再发送下一个帧，为此应该要维护一个计时器（全局变量？），该计时器在循环或者定时器中累加计时（计时大于
//35后就停止计数，发送方发送一个帧之后将其清零，再发送时判断值是否大于35
void rtud_recv(void)
{
	char ch = stat_s.serial_recv();

	if (':' == ch) {memset(stat_s.pInit, 0, stat_s._size); stat_s.pInxt= stat_s.pInit; stat_s.state = RTUD_STATE_0;}

	switch (stat_s.state)
	{
		case RTUD_STATE_0: /**< Check address field */
			{
				/**< Quick address field (the first byte) handler */
				stat_s.state = ((0 != ch) && (MBCD_CFG_ADDRESS != ch))?RTUD_STATE_IDLE:RTUD_STATE_1;
			}
			break;
		case RTUD_STATE_1: /**< Recive data into buff */
			{
				switch (ch)
				{
					case 0X0D: /**< CR */ 
						stat_s.state = RTUD_STATE_2;
						break;
					default:
						{
							*(stat_s.pInxt) = ch; (stat_s.pInxt)++; 
						}
				}
			}
			break;
		case RTUD_STATE_2: /**< End recive and execute action */
			{
				switch (ch)
				{
					case 0X0A: /**< LF */ 
						{
							stat_s.serial_exec(&stat_s);
							stat_s.state = RTUD_STATE_IDLE;
						}
						break;
					default:
						;
				}
			}
			break;
		default: /**< RTU_STATE_IDLE */
			;
	}

	return;
}

#endif //((MBCD_CFG_MS_SEL == 0) && (MBCD_CFG_MOD_RTU_EN > 0))

