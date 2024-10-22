#include "BSP_USART.h"


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
    NVIC_InitStructure.NVIC_IRQChannel = USART_IRQ;
    
    /* 3��������ռ���ȼ� Ϊ1 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    
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
void USART_Config(void)
{
    /* ����һ��GPIO_InitTypeDef���͵ı��� */
    GPIO_InitTypeDef GPIO_InitStructure;
    /* ����һ��USART_InitTypeDef���͵ı��� */
    USART_InitTypeDef USART_InitStructure;
    
    /* �򿪴��������ʱ�� */
    USART_APBxCLKCmd(USART_CLK,ENABLE);
    
    /* �򿪴���GPIO��ʱ�� */
    USART_GPIO_APBxCLKCmd(USRAT_GPIO_CLK,ENABLE);
    
    /* ��ʼ�� USART Tx ��GPIO */
    GPIO_InitStructure.GPIO_Pin = USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART_TX_GPIO_PORT,&GPIO_InitStructure);
    
    /* ��ʼ�� USART Rx ��GPIO */
    GPIO_InitStructure.GPIO_Pin = USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(USART_RX_GPIO_PORT,&GPIO_InitStructure);
    
    
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
    USART_Init(Serial_Port_Number_USARTx,&USART_InitStructure);
    
    
    /* �����ж����ȼ����� */
    NVIC_Configuration();
    
    /* ʹ�ܴ��� �������ݼĴ����ǿ� �ж� */
    USART_ITConfig(Serial_Port_Number_USARTx, USART_IT_RXNE, ENABLE);
    
    /* ʹ�ܴ��ڡ� �����ܿ��� USART_CR1_UE */
    USART_Cmd(Serial_Port_Number_USARTx, ENABLE);
    
    
}


/**
  * @brief  ��Ƭ�����ⲿ ����һ���ֽ�
  * @param  pUSARTx      ���ں�ѡ��
  * @param  data         Ҫ���͵����ݣ�ֻ����һ���ֽڣ�
  * @retval �޷���ֵ
  */
void USART_SendByte(USART_TypeDef* pUSARTx, uint8_t data)
{
    /* ���ʹ����������� ������*/
    USART_SendData(pUSARTx, data);
    
    /* ��� �������ݼĴ����Ƿ�Ϊ�� USART_SR_TXE    ��Ϊ�����ʾ������ϣ�����û�з�����ϣ������ȴ��䷢��ֱ�����*/
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);   //����ֻ�Ƿ��͵��ֽڣ����Բ���ѡ��USART_FLAG_TXE��
    
}


/**
  * @brief  ��Ƭ�����ⲿ ���������ֽ�
  * @param  pUSARTx      ���ں�ѡ��
  * @param  data         Ҫ���͵����ݣ�ֻ�����������ֽڣ�
  * @retval �޷���ֵ
  */
void USART_SendTwoByte(USART_TypeDef* pUSARTx, uint16_t data)
{
    /* ����һ����Ÿ�8λ�͵�8λ�ı��� */
    uint8_t temp_H, temp_L;
    
    /* ����������� ȡ�� �߰�λ�������temp_H�� */
    temp_H = (data & 0xFF00) >> 8;
    
    /* ����������� ȡ�� �Ͱ�λ�������temp_L�� */
    temp_L = data & 0xFF;
    
    /* �ȷ���temp_H�еĸ�8λ������ ������ */
    USART_SendData(pUSARTx, temp_H);
    
    /* ��� �������ݼĴ����Ƿ�Ϊ�� USART_SR_TXE    ��Ϊ�����ʾ������ϣ�����û�з�����ϣ������ȴ��䷢��ֱ�����*/
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);   //�����Ƿ��������ֽڣ��������������ͣ����Բ���ѡ��USART_FLAG_TXE��
    
    /* �ٷ���temp_L�еĵ�8λ������ ������ */
    USART_SendData(pUSARTx, temp_L);
    
    /* ��� �������ݼĴ����Ƿ�Ϊ�� USART_SR_TXE    ��Ϊ�����ʾ������ϣ�����û�з�����ϣ������ȴ��䷢��ֱ�����*/
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);   //�����Ƿ��������ֽڣ��������������ͣ����Բ���ѡ��USART_FLAG_TXE��
    
    
}



/**
  * @brief  ��Ƭ�����ⲿ ����һ������
  * @param  pUSARTx      ���ں�ѡ��
  * @param  array        Ҫ���͵����飨����޷���8λint���͵����飩
  * @param  num          �����Ԫ�ظ���
  * @retval �޷���ֵ
  */
void USART_SendArray(USART_TypeDef* pUSARTx, uint8_t * array, uint8_t num)
{
    uint8_t i;
    
    for(i = 0; i < num; i++)
    {
        /* ÿ��ѭ�� ����һ���ֽ� �����ڣ�����˵ÿ��ѭ�� ����һ��Ԫ�أ� */
        USART_SendByte(pUSARTx, array[i]);
    }
    
    /* ��� �������ݼĴ����Ƿ�Ϊ�� USART_SR_TC    ��Ϊ�����ʾ������ϣ�����û�з�����ϣ������ȴ��䷢��ֱ�����*/
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);   //�����Ƿ������飬��Ҫ�����������ݣ�ֱ�����ݷ�����ϣ����Բ���ѡ��USART_FLAG_TC��
    
}


/**
  * @brief  ��Ƭ�����ⲿ ����һ���ַ���
  * @param  pUSARTx      ���ں�ѡ��
  * @param  str          Ҫ���͵��ַ���
  * @retval �޷���ֵ
  */
void USART_SendString(USART_TypeDef* pUSARTx, char * str)
{
    uint16_t i = 0;
    
    do
    {
        /* ÿ��ѭ������һ���ַ� ������*/
        USART_SendByte(pUSARTx, *(str + i));
        /* ������1��Ϊ��һ�η�����һ���ַ���׼�� */
        i++;
    }while(*(str + i) != '\0');     //�������ַ���������־ʱ���˳�ѭ��
    
    /* ��� �������ݼĴ����Ƿ�Ϊ�� USART_SR_TC    ��Ϊ�����ʾ������ϣ�����û�з�����ϣ������ȴ��䷢��ֱ�����*/
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);   //�����Ƿ����ַ�������Ҫ�����������ݣ�ֱ�����ݷ�����ϣ����Բ���ѡ��USART_FLAG_TC��
    
}



//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����� ������ */
		USART_SendData(Serial_Port_Number_USARTx, (uint8_t) ch);
		
		/* ��� �������ݼĴ����Ƿ�Ϊ�� USART_SR_TXE    ��Ϊ�����ʾ������ϣ�����û�з�����ϣ������ȴ��䷢��ֱ�����*/
		while (USART_GetFlagStatus(Serial_Port_Number_USARTx, USART_FLAG_TC) == RESET);   //�����Ƿ���һ���ֽڣ��������������ͣ����Բ���ѡ��USART_FLAG_TXE��		
	
		return (ch);
}


///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(Serial_Port_Number_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(Serial_Port_Number_USARTx);
}
