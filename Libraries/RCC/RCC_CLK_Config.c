#include "RCC_CLK_Config.h"


/**
  * @brief  ʹ��HSE����ϵͳʱ��
  * @param  RCC_PLLMul_x    x�ķ�Χ��2-16   ����������PLL�ı�Ƶ����
  * @retval �޷���ֵ
  */
void HSE_SetSysClock(uint32_t RCC_PLLMul_x)
{
    //����һ��ErrorStatus���͵ı���,���ڽ��� ��������HSE��״̬���ж����Ƿ������ȶ�
    ErrorStatus HSEStatus;
    
    //1����RCC�����ʼ���ɸ�λ״̬
    RCC_DeInit();
    
    //2��ʹ��HSE�����������ⲿ����ʱ�ӣ�
    RCC_HSEConfig(RCC_HSE_ON);
    
    //3���ȴ�HSE�����ȶ�
    HSEStatus = RCC_WaitForHSEStartUp();
    
    //4���ж�HSE��״̬��ֻ��HSE�ȶ�֮�󣬲ż�������ִ��
    if(HSEStatus == SUCCESS)
    {
        //5��ʹ��FLASH Ԥ��ȡ������
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
        //6������ϵͳʱ�ӣ�SYSYCLK������ �� �������ʱ�� �ı���
        FLASH_SetLatency(FLASH_Latency_2);      //�������õȴ�ʱ��2
                                                //���ó� 2 ��ʱ��SYSCLK ���� 48M Ҳ���Թ���
                                                //������ó� 0 ���� 1 ��ʱ��������õ� SYSCLK �����˷�Χ�Ļ���������Ӳ�����󣬳��������
        
        //7������AHB��APB2��APB1���ߵ�Ԥ��Ƶ����
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        //AHB Ԥ��Ƶ��������Ϊ1��Ƶ��HCLK = SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);         //APB2 Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK2 = HCLK
        RCC_PCLK1Config(RCC_HCLK_Div2);         //APB1 Ԥ��Ƶ��������Ϊ2��Ƶ��PCLK1 = HCLK/2
        
        //8������PLL: PLLʱ��Դ��PLL��Ƶ����
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x);    //����PLLʱ��ԴΪ��HSE1��Ƶ  PLL��Ƶ���ӣ���Ϊ�βδ���
        
        //9��ʹ��PLL
        RCC_PLLCmd(ENABLE);
        
        //10���ȴ�PLL�ȶ�
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        
        //11����PLL�ȶ�֮��ѡ��PLL��Ϊϵͳʱ��
       RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
       
       //12����ȡʱ���л�״̬λ��ȷ�� PLICLK ��ѡΪϵͳʱ��
        while(RCC_GetSYSCLKSource() != 0x08);
    }
    else
    {
        //���HSE ����ʧ�ܣ���ô����ͻ���������û�����������ӳ���Ĵ��봦��
    }
}



/**
  * @brief  ʹ��HSI����ϵͳʱ��
  * @param  RCC_PLLMul_x    x�ķ�Χ��2-16   ����������PLL�ı�Ƶ����
  * @retval �޷���ֵ
  */
void HSI_SetSysClock(uint32_t RCC_PLLMul_x)
{
    //����һ��32λ�ı���,���ùؼ��� volatile ���Σ����ڽ��� ��������HSI��״̬���ж����Ƿ������ȶ�
    __IO uint32_t HSIStatus = 0;
    
    //1����RCC�����ʼ���ɸ�λ״̬
    RCC_DeInit();
    
    //2��ʹ��HSI�����������ⲿ����ʱ�ӣ�
    RCC_HSICmd(ENABLE);
    
    //3���ȴ�HSI�����ȶ������ﲢû�йٷ�����ĺ���������ֱ��ʹ�üĴ������жϣ�
    HSIStatus = RCC->CR & RCC_CR_HSIRDY;
    
    //4���ж�HSI��״̬��ֻ��HSI�ȶ�֮�󣬲ż�������ִ��
    if(HSIStatus == RCC_CR_HSIRDY)
    {
        //5��ʹ��FLASH Ԥ��ȡ������
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
        //6������ϵͳʱ�ӣ�SYSYCLK������ �� �������ʱ�� �ı���
        FLASH_SetLatency(FLASH_Latency_2);      //�������õȴ�ʱ��2
                                                //���ó� 2 ��ʱ��SYSCLK ���� 48M Ҳ���Թ���
                                                //������ó� 0 ���� 1 ��ʱ��������õ� SYSCLK �����˷�Χ�Ļ���������Ӳ�����󣬳��������
        
        //7������AHB��APB2��APB1���ߵ�Ԥ��Ƶ����
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        //AHB Ԥ��Ƶ��������Ϊ1��Ƶ��HCLK = SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);         //APB2 Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK2 = HCLK
        RCC_PCLK1Config(RCC_HCLK_Div2);         //APB1 Ԥ��Ƶ��������Ϊ2��Ƶ��PCLK1 = HCLK/2
        
        //8������PLL: PLLʱ��Դ��PLL��Ƶ����
        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_x);    //����PLLʱ��ԴΪ��HSI 2��Ƶ  PLL��Ƶ���ӣ���Ϊ�βδ���
        
        //9��ʹ��PLL
        RCC_PLLCmd(ENABLE);
        
        //10���ȴ�PLL�ȶ�
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        
        //11����PLL�ȶ�֮��ѡ��PLL��Ϊϵͳʱ��
       RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
       
       //12����ȡʱ���л�״̬λ��ȷ�� PLICLK ��ѡΪϵͳʱ��
        while(RCC_GetSYSCLKSource() != 0x08);
    }
    else
    {
        //���HSI ����ʧ�ܣ���ô����ͻ���������û�����������ӳ���Ĵ��봦��
    }
}



/**
  * @brief  MCOʱ�����IO������
  * @param  �޲���
  * @retval �޷���ֵ
  */
void MCO_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* ����GPIOA ����ʱ�� */    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /* ����GPIO�����š�ģʽ������ٶ� */    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* ���ú��� �����úõĲ���д�뵽�Ĵ��� */    
    GPIO_Init(GPIOA,&GPIO_InitStruct);

}
