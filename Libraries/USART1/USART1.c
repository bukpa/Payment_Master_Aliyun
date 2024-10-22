#include "USART1.h"


uint8_t USART1_RX_BUF[USART1_RX_LEN]; 

uint16_t USART1_RX_CNT = 0; //�������ݳ���

volatile uint8_t USART1_time_out_count = 0;



 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
    /* ����һ��NVIC_InitTypeDef�ṹ�����͵� ���� */
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 1������NVIC�����ȼ����� */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
    /* 2������USART���ж�Դ */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    
    /* 3��������ռ���ȼ� Ϊ1 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    
    /* 4�����������ȼ� Ϊ1 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    
    /* 5��ʹ���ж� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    /* 6�������úõ�NVIC��ʼ���ṹ�� д��Ĵ����� */
    NVIC_Init(&NVIC_InitStructure);
    
}



 /**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void USART1_Init(void)
{
    /* ����һ��GPIO_InitTypeDef���͵ı��� */
    GPIO_InitTypeDef GPIO_InitStructure;
    /* ����һ��USART_InitTypeDef���͵ı��� */
    USART_InitTypeDef USART_InitStructure;
    
    /* �򿪴��������ʱ�� */
    RCC_APB2PeriphClockCmd(USART1_CLK,ENABLE);
    
    /* �򿪴���GPIO��ʱ�� */
    RCC_APB2PeriphClockCmd(USART1_GPIO_CLK,ENABLE);
    
    /* ��ʼ�� USART Tx ��GPIO��GPIOA 9�� */
    GPIO_InitStructure.GPIO_Pin = USART1_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_TX_GPIO_PORT,&GPIO_InitStructure);
    
    /* ��ʼ�� USART Rx ��GPIO��GPIOA 10�� */
    GPIO_InitStructure.GPIO_Pin = USART1_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(USART1_RX_GPIO_PORT,&GPIO_InitStructure);
    
    
    /* ���ô��ڵĹ������� */
    
    /* 1�����ò����� */
    USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
    
    /* 2������ ֡���� �ֳ� */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    
    /* 3������ֹͣλ */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    
    /* 4������У��λ */
    USART_InitStructure.USART_Parity = USART_Parity_No;
    
    /* 5������Ӳ�������� */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
    /* 6�����ù���ģʽ�� �������ݺͷ�������һ�����ã��û�����ʵ�֣� */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    /* 7�����������ò���д��Ĵ�������ɴ��ڵĳ�ʼ������ */
    USART_Init(USART1,&USART_InitStructure);
    
    
    /* �����ж����ȼ����� */
    NVIC_Configuration();
    
    /* ʹ�ܴ��� �������ݼĴ����ǿ� �ж� */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    /* ʹ�ܴ��ڡ� �����ܿ��� USART_CR1_UE */
    USART_Cmd(USART1, ENABLE);
    
    
}



void USART1_IRQHandler(void)                	//����1�жϷ������
{  

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//�����жϣ���ȡ�������ݵ�USART1Buff
    {       
        USART1_time_out_count = USART1_RECEIVED_TIME_OUT;
        
        USART1_RX_BUF[USART1_RX_CNT] = USART1->DR;
        
        USART1_RX_CNT++;    
    }  
} 



/*******************************************************************************
* ��������  : USART1_tick
* ����      : �������ݽ��ճ�ʱ����
* ����      : None
* ���      : None
* ����ֵ    : None                                             
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

                BaseType_t xReturn = xSemaphoreGiveFromISR(xUSART1_Recv_Semaphore,&pxHigherPriorityTaskWoken);//�ͷŴ���1�����ź���

                portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
            }
        }	
    }
}


/*******************************************************************************
* ��������  : USART1_SendData
* ����      : ���ڷ��ͺ���
* ����      : �������ݵ�ַ�ͳ���
* ���      : None
* ����ֵ    : None
*******************************************************************************/
void USART1_SendData(u8 *pdata,u16 len)
{
    u16 i;

    if(0 == len)
    {
        return;
    }

    for(i=0;i<len;i++) //ѭ����������
    { 
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
        USART_SendData(USART1, pdata[i]);//��������
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


//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����� ������ */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* ��� �������ݼĴ����Ƿ�Ϊ�� USART_SR_TXE    ��Ϊ�����ʾ������ϣ�����û�з�����ϣ������ȴ��䷢��ֱ�����*/
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   //�����Ƿ���һ���ֽڣ��������������ͣ����Բ���ѡ��USART_FLAG_TXE��		
	
		return (ch);
}


///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
