#include "BSP_USART.h"


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
    NVIC_InitStructure.NVIC_IRQChannel = USART_IRQ;
    
    /* 3、配置抢占优先级 为1 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    
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
void USART_Config(void)
{
    /* 定义一个GPIO_InitTypeDef类型的变量 */
    GPIO_InitTypeDef GPIO_InitStructure;
    /* 定义一个USART_InitTypeDef类型的变量 */
    USART_InitTypeDef USART_InitStructure;
    
    /* 打开串口外设的时钟 */
    USART_APBxCLKCmd(USART_CLK,ENABLE);
    
    /* 打开串口GPIO的时钟 */
    USART_GPIO_APBxCLKCmd(USRAT_GPIO_CLK,ENABLE);
    
    /* 初始化 USART Tx 的GPIO */
    GPIO_InitStructure.GPIO_Pin = USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART_TX_GPIO_PORT,&GPIO_InitStructure);
    
    /* 初始化 USART Rx 的GPIO */
    GPIO_InitStructure.GPIO_Pin = USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(USART_RX_GPIO_PORT,&GPIO_InitStructure);
    
    
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
    USART_Init(Serial_Port_Number_USARTx,&USART_InitStructure);
    
    
    /* 串口中断优先级配置 */
    NVIC_Configuration();
    
    /* 使能串口 接收数据寄存器非空 中断 */
    USART_ITConfig(Serial_Port_Number_USARTx, USART_IT_RXNE, ENABLE);
    
    /* 使能串口— 串口总开关 USART_CR1_UE */
    USART_Cmd(Serial_Port_Number_USARTx, ENABLE);
    
    
}


/**
  * @brief  单片机向外部 发送一个字节
  * @param  pUSARTx      串口号选择
  * @param  data         要发送的数据（只能是一个字节）
  * @retval 无返回值
  */
void USART_SendByte(USART_TypeDef* pUSARTx, uint8_t data)
{
    /* 发送传进来的数据 到串口*/
    USART_SendData(pUSARTx, data);
    
    /* 检测 发送数据寄存器是否为空 USART_SR_TXE    若为空则表示发送完毕，否则没有发送完毕，继续等待其发送直至完毕*/
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);   //这里只是发送单字节，所以参数选择‘USART_FLAG_TXE’
    
}


/**
  * @brief  单片机向外部 发送俩个字节
  * @param  pUSARTx      串口号选择
  * @param  data         要发送的数据（只能是俩个个字节）
  * @retval 无返回值
  */
void USART_SendTwoByte(USART_TypeDef* pUSARTx, uint16_t data)
{
    /* 定义一个存放高8位和低8位的变量 */
    uint8_t temp_H, temp_L;
    
    /* 将传入的数据 取出 高八位放入变量temp_H中 */
    temp_H = (data & 0xFF00) >> 8;
    
    /* 将传入的数据 取出 低八位放入变量temp_L中 */
    temp_L = data & 0xFF;
    
    /* 先发送temp_H中的高8位的数据 到串口 */
    USART_SendData(pUSARTx, temp_H);
    
    /* 检测 发送数据寄存器是否为空 USART_SR_TXE    若为空则表示发送完毕，否则没有发送完毕，继续等待其发送直至完毕*/
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);   //这里是发送两个字节，并不会连续发送，所以参数选择‘USART_FLAG_TXE’
    
    /* 再发送temp_L中的低8位的数据 到串口 */
    USART_SendData(pUSARTx, temp_L);
    
    /* 检测 发送数据寄存器是否为空 USART_SR_TXE    若为空则表示发送完毕，否则没有发送完毕，继续等待其发送直至完毕*/
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);   //这里是发送两个字节，并不会连续发送，所以参数选择‘USART_FLAG_TXE’
    
    
}



/**
  * @brief  单片机向外部 发送一个数组
  * @param  pUSARTx      串口号选择
  * @param  array        要发送的数组（存放无符号8位int类型的数组）
  * @param  num          数组的元素个数
  * @retval 无返回值
  */
void USART_SendArray(USART_TypeDef* pUSARTx, uint8_t * array, uint8_t num)
{
    uint8_t i;
    
    for(i = 0; i < num; i++)
    {
        /* 每次循环 发送一个字节 到串口（或者说每次循环 发送一个元素） */
        USART_SendByte(pUSARTx, array[i]);
    }
    
    /* 检测 发送数据寄存器是否为空 USART_SR_TC    若为空则表示发送完毕，否则没有发送完毕，继续等待其发送直至完毕*/
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);   //这里是发送数组，需要连续发送数据，直至数据发送完毕，所以参数选择‘USART_FLAG_TC’
    
}


/**
  * @brief  单片机向外部 发送一个字符串
  * @param  pUSARTx      串口号选择
  * @param  str          要发送的字符串
  * @retval 无返回值
  */
void USART_SendString(USART_TypeDef* pUSARTx, char * str)
{
    uint16_t i = 0;
    
    do
    {
        /* 每次循环发送一个字符 到串口*/
        USART_SendByte(pUSARTx, *(str + i));
        /* 索引加1，为下一次发送下一个字符做准备 */
        i++;
    }while(*(str + i) != '\0');     //当遇到字符串结束标志时，退出循环
    
    /* 检测 发送数据寄存器是否为空 USART_SR_TC    若为空则表示发送完毕，否则没有发送完毕，继续等待其发送直至完毕*/
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);   //这里是发送字符串，需要连续发送数据，直至数据发送完毕，所以参数选择‘USART_FLAG_TC’
    
}



//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据 到串口 */
		USART_SendData(Serial_Port_Number_USARTx, (uint8_t) ch);
		
		/* 检测 发送数据寄存器是否为空 USART_SR_TXE    若为空则表示发送完毕，否则没有发送完毕，继续等待其发送直至完毕*/
		while (USART_GetFlagStatus(Serial_Port_Number_USARTx, USART_FLAG_TC) == RESET);   //这里是发送一个字节，并不会连续发送，所以参数选择‘USART_FLAG_TXE’		
	
		return (ch);
}


///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(Serial_Port_Number_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(Serial_Port_Number_USARTx);
}
