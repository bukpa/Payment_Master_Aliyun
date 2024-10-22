#include "RCC_CLK_Config.h"


/**
  * @brief  使用HSE配置系统时钟
  * @param  RCC_PLLMul_x    x的范围：2-16   其用来设置PLL的倍频因子
  * @retval 无返回值
  */
void HSE_SetSysClock(uint32_t RCC_PLLMul_x)
{
    //定义一个ErrorStatus类型的变量,用于接收 返回来的HSE的状态，判断其是否启动稳定
    ErrorStatus HSEStatus;
    
    //1、把RCC外设初始化成复位状态
    RCC_DeInit();
    
    //2、使能HSE，开启高速外部晶振（时钟）
    RCC_HSEConfig(RCC_HSE_ON);
    
    //3、等待HSE启动稳定
    HSEStatus = RCC_WaitForHSEStartUp();
    
    //4、判断HSE的状态，只有HSE稳定之后，才继续往下执行
    if(HSEStatus == SUCCESS)
    {
        //5、使能FLASH 预存取缓冲区
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
        //6、设置系统时钟（SYSYCLK）周期 与 闪存访问时间 的比例
        FLASH_SetLatency(FLASH_Latency_2);      //这里设置等待时间2
                                                //设置成 2 的时候，SYSCLK 低于 48M 也可以工作
                                                //如果设置成 0 或者 1 的时候，如果配置的 SYSCLK 超出了范围的话，则会进入硬件错误，程序就死了
        
        //7、设置AHB、APB2、APB1总线的预分频因子
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        //AHB 预分频因子设置为1分频，HCLK = SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);         //APB2 预分频因子设置为1分频，PCLK2 = HCLK
        RCC_PCLK1Config(RCC_HCLK_Div2);         //APB1 预分频因子设置为2分频，PCLK1 = HCLK/2
        
        //8、配置PLL: PLL时钟源、PLL倍频因子
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x);    //设置PLL时钟源为：HSE1分频  PLL倍频因子：作为形参传入
        
        //9、使能PLL
        RCC_PLLCmd(ENABLE);
        
        //10、等待PLL稳定
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        
        //11、当PLL稳定之后，选择PLL作为系统时钟
       RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
       
       //12、读取时钟切换状态位，确保 PLICLK 被选为系统时钟
        while(RCC_GetSYSCLKSource() != 0x08);
    }
    else
    {
        //如果HSE 开启失败，那么程序就会来到这里，用户可在这里添加出错的代码处理
    }
}



/**
  * @brief  使用HSI配置系统时钟
  * @param  RCC_PLLMul_x    x的范围：2-16   其用来设置PLL的倍频因子
  * @retval 无返回值
  */
void HSI_SetSysClock(uint32_t RCC_PLLMul_x)
{
    //定义一个32位的变量,并用关键字 volatile 修饰，用于接收 返回来的HSI的状态，判断其是否启动稳定
    __IO uint32_t HSIStatus = 0;
    
    //1、把RCC外设初始化成复位状态
    RCC_DeInit();
    
    //2、使能HSI，开启高速外部晶振（时钟）
    RCC_HSICmd(ENABLE);
    
    //3、等待HSI启动稳定（这里并没有官方定义的函数，所以直接使用寄存器来判断）
    HSIStatus = RCC->CR & RCC_CR_HSIRDY;
    
    //4、判断HSI的状态，只有HSI稳定之后，才继续往下执行
    if(HSIStatus == RCC_CR_HSIRDY)
    {
        //5、使能FLASH 预存取缓冲区
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
        //6、设置系统时钟（SYSYCLK）周期 与 闪存访问时间 的比例
        FLASH_SetLatency(FLASH_Latency_2);      //这里设置等待时间2
                                                //设置成 2 的时候，SYSCLK 低于 48M 也可以工作
                                                //如果设置成 0 或者 1 的时候，如果配置的 SYSCLK 超出了范围的话，则会进入硬件错误，程序就死了
        
        //7、设置AHB、APB2、APB1总线的预分频因子
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        //AHB 预分频因子设置为1分频，HCLK = SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);         //APB2 预分频因子设置为1分频，PCLK2 = HCLK
        RCC_PCLK1Config(RCC_HCLK_Div2);         //APB1 预分频因子设置为2分频，PCLK1 = HCLK/2
        
        //8、配置PLL: PLL时钟源、PLL倍频因子
        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_x);    //设置PLL时钟源为：HSI 2分频  PLL倍频因子：作为形参传入
        
        //9、使能PLL
        RCC_PLLCmd(ENABLE);
        
        //10、等待PLL稳定
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        
        //11、当PLL稳定之后，选择PLL作为系统时钟
       RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
       
       //12、读取时钟切换状态位，确保 PLICLK 被选为系统时钟
        while(RCC_GetSYSCLKSource() != 0x08);
    }
    else
    {
        //如果HSI 开启失败，那么程序就会来到这里，用户可在这里添加出错的代码处理
    }
}



/**
  * @brief  MCO时钟输出IO口配置
  * @param  无参数
  * @retval 无返回值
  */
void MCO_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* 开启GPIOA 外设时钟 */    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /* 配置GPIO的引脚、模式、输出速度 */    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* 调用函数 将配置好的参数写入到寄存器 */    
    GPIO_Init(GPIOA,&GPIO_InitStruct);

}
