#ifndef __MAIN_H
#define __MAIN_H


#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>


#define SYSTEM_DEBUG                1               //系统打印信息
#define SYSTEM_WARNING              1               //系统警告信息
#define SEND_ATCMD_DEBUG            1               //发送AT指令信息
#define RECV_ATCMD_DEBUG            1               //接收AT指令信息
#define SEND_NETDATA_DEBUG          1               //发送AT指令信息
#define RECV_NETDATA_DEBUG          1               //接收平台数据信息
#define U3RECV_DEBUG                1               //串口屏数据信息


#if  SYSTEM_DEBUG
#define SystemPrintf(fmt,args...)                   printf("|*** System Debug : "#fmt"\r\n",##args)	
#else
#define SystemPrintf(fmt,args...)	                ((void)0)
#endif

#if  SYSTEM_WARNING
#define SystemWaring(fmt,args...)                   printf("/****** File : %s, function : %s, line : %d\r\n/****** System Warning : "#fmt"\r\n",\
                                                            ((char*)__FILE__), ((char*)__FUNCTION__),( __LINE__), ##args)	
#else
#define SystemWaring(fmt,args...)	                ((void)0)
#endif

#if  SEND_ATCMD_DEBUG
#define SendATCmdPrintf(fmt,args...)                printf("|--- Send AT Cmd : "#fmt"\r\n",##args)	
#else
#define SendATCmdPrintf(fmt,args...)	            ((void)0)
#endif

#if     RECV_ATCMD_DEBUG
#define RecvATCmdPrintf(fmt,args...)                printf("|--- Recv AT Data : "#fmt"\r\n",##args)	
#else
#define RecvATCmdPrintf(fmt,args...)                ((void)0)	
#endif

#if     SEND_NETDATA_DEBUG
#define SendNetdataPrintf(fmt,args...)              printf("|--- Send Onenet Data : "#fmt"\r\n",##args)	
#else
#define SendNetdataPrintf(fmt,args...)	            ((void)0)
#endif

#if     RECV_NETDATA_DEBUG
#define RecvNetdataPrintf(fmt,args...)              printf("|--- Recv Onenet Data : "#fmt"\r\n",##args)	
#else
#define RecvNetdataPrintf(fmt,args...)	            ((void)0)
#endif


extern xSemaphoreHandle xUSART1_Recv_Semaphore;
extern xSemaphoreHandle xTLSF_MallocLock_Mutex;

#endif /* __MAIN_H */