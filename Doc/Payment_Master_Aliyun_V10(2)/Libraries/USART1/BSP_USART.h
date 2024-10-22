#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include <stdio.h>

/* 串口宏定义，要使用哪个串口，直接将下方的宏定义的值改为1即可 */
#define Serial_Port_Number_USART1   1
#define Serial_Port_Number_USART2   0
#define Serial_Port_Number_USART3   0
#define Serial_Port_Number_USART4   0
#define Serial_Port_Number_USART5   0



#if Serial_Port_Number_USART1

/* 串口1— USART1 相关宏定义 */
#define Serial_Port_Number_USARTx           USART1                      
#define USART_CLK                           RCC_APB2Periph_USART1       
#define USART_APBxCLKCmd                    RCC_APB2PeriphClockCmd      
#define USART_BAUDRATE                      115200                      

/* USART1 GPIO 引脚 相关宏定义 */
#define USRAT_GPIO_CLK                      (RCC_APB2Periph_GPIOA)
#define USART_GPIO_APBxCLKCmd               RCC_APB2PeriphClockCmd

#define USART_TX_GPIO_PORT                  GPIOA
#define USART_TX_GPIO_PIN                   GPIO_Pin_9
#define USART_RX_GPIO_PORT                  GPIOA
#define USART_RX_GPIO_PIN                   GPIO_Pin_10

#define USART_IRQ                           USART1_IRQn
#define USART_IRQHandler                    USART1_IRQHandler




#elif Serial_Port_Number_USART2

/* 串口2— USART2 相关宏定义 */
#define Serial_Port_Number_USARTx           USART2                      
#define USART_CLK                           RCC_APB1Periph_USART2       
#define USART_APBxCLKCmd                    RCC_APB1PeriphClockCmd      
#define USART_BAUDRATE                      115200                      

/* USART2 GPIO 引脚 相关宏定义 */
#define USRAT_GPIO_CLK                      (RCC_APB2Periph_GPIOA)
#define USART_GPIO_APBxCLKCmd               RCC_APB2PeriphClockCmd

#define USART_TX_GPIO_PORT                  GPIOA
#define USART_TX_GPIO_PIN                   GPIO_Pin_2
#define USART_RX_GPIO_PORT                  GPIOA
#define USART_RX_GPIO_PIN                   GPIO_Pin_3

#define USART_IRQ                           USART2_IRQn
#define USART_IRQHandler                    USART2_IRQHandler




#elif Serial_Port_Number_USART3

/* 串口3— USART3 相关宏定义 */
#define Serial_Port_Number_USARTx           USART3                      
#define USART_CLK                           RCC_APB1Periph_USART3       
#define USART_APBxCLKCmd                    RCC_APB1PeriphClockCmd      
#define USART_BAUDRATE                      115200                      

/* USART3 GPIO 引脚 相关宏定义 */
#define USRAT_GPIO_CLK                      (RCC_APB2Periph_GPIOB)
#define USART_GPIO_APBxCLKCmd               RCC_APB2PeriphClockCmd

#define USART_TX_GPIO_PORT                  GPIOB
#define USART_TX_GPIO_PIN                   GPIO_Pin_10
#define USART_RX_GPIO_PORT                  GPIOB
#define USART_RX_GPIO_PIN                   GPIO_Pin_11

#define USART_IRQ                           USART3_IRQn
#define USART_IRQHandler                    USART3_IRQHandler




#elif Serial_Port_Number_USART4

/* 串口4— USART4 相关宏定义 */
#define Serial_Port_Number_USARTx           UART4                      
#define USART_CLK                           RCC_APB1Periph_UART4       
#define USART_APBxCLKCmd                    RCC_APB1PeriphClockCmd      
#define USART_BAUDRATE                      115200                      

/* USART4 GPIO 引脚 相关宏定义 */
#define USRAT_GPIO_CLK                      (RCC_APB2Periph_GPIOC)
#define USART_GPIO_APBxCLKCmd               RCC_APB2PeriphClockCmd

#define USART_TX_GPIO_PORT                  GPIOC
#define USART_TX_GPIO_PIN                   GPIO_Pin_10
#define USART_RX_GPIO_PORT                  GPIOC
#define USART_RX_GPIO_PIN                   GPIO_Pin_11

#define USART_IRQ                           UART4_IRQn
#define USART_IRQHandler                    UART4_IRQHandler




#elif Serial_Port_Number_USART5

/* 串口4— USART4 相关宏定义 */
#define Serial_Port_Number_USARTx           UART5                      
#define USART_CLK                           RCC_APB1Periph_UART5       
#define USART_APBxCLKCmd                    RCC_APB1PeriphClockCmd      
#define USART_BAUDRATE                      115200                      

/* USART4 GPIO 引脚 相关宏定义 */
#define USRAT_GPIO_CLK                      (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)
#define USART_GPIO_APBxCLKCmd               RCC_APB2PeriphClockCmd

#define USART_TX_GPIO_PORT                  GPIOC
#define USART_TX_GPIO_PIN                   GPIO_Pin_12
#define USART_RX_GPIO_PORT                  GPIOD
#define USART_RX_GPIO_PIN                   GPIO_Pin_2

#define USART_IRQ                           UART5_IRQn
#define USART_IRQHandler                    UART5_IRQHandler

#endif




void USART_Config(void);
void USART_SendByte(USART_TypeDef* pUSARTx, uint8_t data);
void USART_SendTwoByte(USART_TypeDef* pUSARTx, uint16_t data);
void USART_SendArray(USART_TypeDef* pUSARTx, uint8_t * array, uint8_t num);
void USART_SendString(USART_TypeDef* pUSARTx, char * str);




#endif /* __BSP_USART_H */

