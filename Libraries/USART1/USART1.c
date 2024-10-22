#include "USART1.h"


uint8_t USART1_RX_BUF[USART1_RX_LEN]; 

uint16_t USART1_RX_CNT = 0; //接收数据长度

volatile uint8_t USART1_time_out_count = 0;



 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
    /* 定义一个NVIC_InitTypeDef结构体类型的 变量 */
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 1、配置NVIC的优先级分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
    /* 2、配置USART的中断源 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    
    /* 3、配置抢占优先级 为1 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    
    /* 4、配置子优先级 为1 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    
    /* 5、使能中断 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    /* 6、将配置好的NVIC初始化结构体 写入寄存器里 */
    NVIC_Init(&NVIC_InitStructure);
    
}



 /**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void USART1_Init(void)
{
    /* 定义一个GPIO_InitTypeDef类型的变量 */
    GPIO_InitTypeDef GPIO_InitStructure;
    /* 定义一个USART_InitTypeDef类型的变量 */
    USART_InitTypeDef USART_InitStructure;
    
    /* 打开串口外设的时钟 */
    RCC_APB2PeriphClockCmd(USART1_CLK,ENABLE);
    
    /* 打开串口GPIO的时钟 */
    RCC_APB2PeriphClockCmd(USART1_GPIO_CLK,ENABLE);
    
    /* 初始化 USART Tx 的GPIO（GPIOA 9） */
    GPIO_InitStructure.GPIO_Pin = USART1_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_TX_GPIO_PORT,&GPIO_InitStructure);
    
    /* 初始化 USART Rx 的GPIO（GPIOA 10） */
    GPIO_InitStructure.GPIO_Pin = USART1_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(USART1_RX_GPIO_PORT,&GPIO_InitStructure);
    
    
    /* 配置串口的工作参数 */
    
    /* 1、配置波特率 */
    USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
    
    /* 2、配置 帧数据 字长 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    
    /* 3、配置停止位 */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    
    /* 4、配置校验位 */
    USART_InitStructure.USART_Parity = USART_Parity_No;
    
    /* 5、配置硬件流控制 */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
    /* 6、配置工作模式— 接收数据和发送数据一起配置（用或运算实现） */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    /* 7、将上述配置参数写入寄存器，完成串口的初始化配置 */
    USART_Init(USART1,&USART_InitStructure);
    
    
    /* 串口中断优先级配置 */
    NVIC_Configuration();
    
    /* 使能串口 接收数据寄存器非空 中断 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    /* 使能串口— 串口总开关 USART_CR1_UE */
    USART_Cmd(USART1, ENABLE);
    
    
}



void USART1_IRQHandler(void)                	//串口1中断服务程序
{  

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收中断，读取接收数据到USART1Buff
    {       
        USART1_time_out_count = USART1_RECEIVED_TIME_OUT;
        
        USART1_RX_BUF[USART1_RX_CNT] = USART1->DR;
        
        USART1_RX_CNT++;    
    }  
} 



/*******************************************************************************
* 函数名称  : USART1_tick
* 描述      : 串口数据接收超时函数
* 输入      : None
* 输出      : None
* 返回值    : None                                             
*******************************************************************************/
void USART1_Tick(void)
{   
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    if(USART1_time_out_count > 0)
    {
        if(USART1_RX_CNT)
        {
            
            USART1_time_out_count--;

            if(0 == USART1_time_out_count)
            {   

                BaseType_t xReturn = xSemaphoreGiveFromISR(xUSART1_Recv_Semaphore,&pxHigherPriorityTaskWoken);//释放串口1接收信号量

                portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
            }
        }	
    }
}


/*******************************************************************************
* 函数名称  : USART1_SendData
* 描述      : 串口发送函数
* 输入      : 发送数据地址和长度
* 输出      : None
* 返回值    : None
*******************************************************************************/
void USART1_SendData(u8 *pdata,u16 len)
{
    u16 i;

    if(0 == len)
    {
        return;
    }

    for(i=0;i<len;i++) //循环发送数据
    { 
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
        USART_SendData(USART1, pdata[i]);//发送数据
    } 
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
}


/**
 * @param buf: User buffer
 * @param size: Dump data size
 * @param number: The number of outputs per line
*/
void HexPrintf(u8 *buf, u32 size, u32 number)
{
    int i, j;

    for (i = 0; i < size; i += number)
    {
        printf("%08X: ", i);

        for (j = 0; j < number; j++)
        {
            if (j % 8 == 0)
            {
                printf(" ");
            }
            if (i + j < size)
                printf("%02X ", buf[i + j]);
            else
                printf("   ");
        }
        printf(" ");

        for (j = 0; j < number; j++)
        {
            if (i + j < size)
            {
                printf("%c", CHAR_PRINTF(buf[i + j]) ? buf[i + j] : '.');
            }
        }
        printf("\r\n");
    }
}


//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据 到串口 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 检测 发送数据寄存器是否为空 USART_SR_TXE    若为空则表示发送完毕，否则没有发送完毕，继续等待其发送直至完毕*/
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   //这里是发送一个字节，并不会连续发送，所以参数选择‘USART_FLAG_TXE’		
	
		return (ch);
}


///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
