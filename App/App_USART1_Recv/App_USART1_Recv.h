#ifndef __APP_USART1_RECV_H
#define __APP_USART1_RECV_H


#include "main.h"

#define U1_RECV_BUFF_SIZE  USART1_RX_LEN

//用于必要指示信息的打印
#define USARTRECV_PRINTF  1
#if USARTRECV_PRINTF  
#define Usart1_Recv_Printf       printf
#else
#define Usart1_Recv_Printf       //  
#endif


#define ATCmdSend(...)      NET_IO_Send(__VA_ARGS__)

enum
{
    U1_COMMAND_NULL = 0,   // 0
    
    U1_COMMAND_AT,         // 1
    U1_COMMAND_RE,         // 2  
    U1_COMMAND_WE,         // 3 
    U1_COMMAND_RO,         // 4   
    U1_COMMAND_Q1,         // 5
    U1_COMMAND_HELP,       // 6
};



void USART1_Recv_Task(void * pvParameters);

#endif /* __APP_USART1_RECV_H */