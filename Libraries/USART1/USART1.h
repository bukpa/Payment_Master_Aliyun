#ifndef __USART1_H
#define __USART1_H

#include "main.h"
#include <stdio.h>



/* 串口1— USART1 相关宏定义 */                   
#define USART1_CLK                           RCC_APB2Periph_USART1         
#define USART_BAUDRATE                       115200                      

/* USART1 GPIO 引脚 相关宏定义 */
#define USART1_GPIO_CLK                      (RCC_APB2Periph_GPIOA)
#define USART1_TX_GPIO_PORT                  GPIOA
#define USART1_TX_GPIO_PIN                   GPIO_Pin_9
#define USART1_RX_GPIO_PORT                  GPIOA
#define USART1_RX_GPIO_PIN                   GPIO_Pin_10


#define USART1_RECEIVED_TIME_OUT             5   //接收超时 ms
#define USART1_RX_LEN  	                     128
extern uint8_t USART1_RX_BUF[USART1_RX_LEN];
extern uint16_t USART1_RX_CNT;


#define CHAR_PRINTF(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')


void USART1_Init(void);
void USART1_Tick(void);
void USART1_SendData(u8 *pdata,u16 len);
void HexPrintf(u8 *buf, u32 size, u32 number);





#endif /* __USART1_H */

