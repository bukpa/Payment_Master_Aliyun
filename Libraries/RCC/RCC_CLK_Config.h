#ifndef __RCC_CLK_CONFIG_H
#define __RCC_CLK_CONFIG_H

#include "stm32f10x.h"

void HSE_SetSysClock(uint32_t RCC_PLLMul_x);
void MCO_GPIO_Config(void);
void HSI_SetSysClock(uint32_t RCC_PLLMul_x);

#endif /* __RCC_CLK_CONFIG_H */

