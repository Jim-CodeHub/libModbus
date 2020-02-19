/**-----------------------------------------------------------------------------------------------------------------
 * @file	modbcd.c
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <modbcd/modbcd.h>


/*
--------------------------------------------------------------------------------------------------------------------
*
												   VARIABLES 
*
--------------------------------------------------------------------------------------------------------------------
*/
extern struct _os_mbox msgbox_asc;


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief	    
 *	@param[in]  
 *	@param[out]
 *	@return		
 *	@note		The param 'buff' and 'size' SHOULD BE the same as mbxxx_recv one
 **/
#if 0
	server: recv - func[handler] - emit
	client: emit - recv - [handler]


	服务器端
		mbascii_recv() //可以在中断 也可以在轮询中
					   //封装到mbcd_recv();

		mbcd_recv_rqst() //buff 和 size 同上，同样可以在中断/轮询中
						//该函数判断Frame/调用functions/并调用执行函数
						//然后返回状态码
						//根据返回的状态码，来重置buff和size
						//然后激活emit函数（可以通过msgbox）
						//封装到mbcd_func()

		mbascii_emit() //可以在中断中 可以在轮询中，该函数通过rqst激活
						//封装到mbcd_emit()

		//由上述灵感，服务器端可以设定三个函数
		//1。 recv 负责接收，接收的同时将Frame搞定（如ascii模式下，需要两两合并字符，tcp和rtu模式下就不同了）
		//		   RECV完成之后使用MSGBOX 通知 2
		//
		//2。 func 功能和执行函数（在func中使用callback回调搞定执行即可）
		//		   runc统一对帧进行判定，然后回调执行函数，然后根据不同的状态码来设定返回帧
		//		   【状态码判定（帧判定 + 回调执行） + 设定响应BUFF】
		//		   注：当不需要响应时不激活emit，否则MSGBOX通知 3
		//
		//3。 emit 负责将func设定的buff发送
		//			EIMT完成后使用MSGBOX通知 1
		//
		//PS1: modbcd.c 该文件负责将1 2 3 分别封装，因为1 2 3，都有三个模式
		//PS2: 这里需要加入超时处理 TBD
		//
		//其实，主机端， 1 2 3的优先级颠倒了，1是eimt，2是recv 3是func
		//		3上面会有很多省略，3上面会有很多省略，
		//		
		//需要设定主从机，因为优先级颠倒了，MSGBOX通知顺序会有不同
		//	或者，可以直接在服务器端函数上增加(宏或变量)判断，来决定通知哪些，这样就不用重新写主机端了
		//	从机端收到响应之后 不需要跟服务器一样调用func，收到之后是异常还是正常（数据）都由用户自己
		//	决定下一步动作了
		//
#endif
void mbcd_recv_rqst(enum eMbcd_mode mode, UCHAR *buff, USHRT *size)
{
	if (MBCD_EMSG_RECV_OK == *((UCHAR *)_OSMboxAccept(&msgbox_asc)))
	{
		/**<----------------------------------- Check frame ------------------------------------------*/

		if ((size >= MBCD_PDUL_ASCII_MIN) && (size <= MBCD_PDUL_ASCII_MAX)) /**< Check PUD length */
		{
			if ((0 == buff[0]) || (MBCD_CFG_ADDRESS == buff[0])) /**< Check Address */
			{
				if (buff[size-1] == mblrc(buff, size - 1)) /**< Check LRC */
				{
					switch (buff[1]) /**< Select funCode */
					{
						case MBCD_CODE_RD_DISCRETE_INPUT:
							//if (MBCD_CFG_COD_RD_DISCRETE_INPUTS_EN <= 0)
							break;
						case MBCD_CODE_RD_COILS:
							//mb_read_coils(buff, &size);
							break;
						case MBCD_CODE_WR_SINGLE_COIL:
							//if ((MBCD_CFG_COD_WR_SINGLE_COIL_EN <= 0) && (0 != frameRcv.address))
							break;
						default:
							;
					}
				}
			}
		}
	}

	return;

#if 0
	switch (mode)
	{
#if MBCD_CFG_MOD_ASCII_EN > 0
		case ascii:

			break;
#endif
#if MBCD_CFG_MOD_RTU_EN > 0
		case rtu  :
			break;
#endif
#if MBCD_CFG_MOD_TCP_EN > 0
		case tcp  :  
			break;
#endif
		defualt:
			;
	} /**< switch(mode) */
#endif
}


#if 0
//Master/Client
mbcd_send_rqst();
mbcd_recv_rsps();

//Slave/Server
mbcd_recv_rqst();
mbcd_send_rsps();

#endif

