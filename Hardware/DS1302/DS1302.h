#ifndef __DS1302_H
#define __DS1302_H

#include "main.h"
#include <time.h>


#define USE_WEEK                          0


#define DS1302_CE(bit)                    GPIO_WriteBit(GPIOB, GPIO_Pin_0, ((bit) ? 1 : 0));
#define DS1302_SCLK(bit)                  GPIO_WriteBit(GPIOC, GPIO_Pin_4, ((bit) ? 1 : 0));
#define DS1302_IO(bit)                    GPIO_WriteBit(GPIOC, GPIO_Pin_5, ((bit) ? 1 : 0));
#define Read_DS1302_IO()                  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)


#define DS1302_IO_OUTMODE_SET()           {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=(u32)3<<20;}
#define DS1302_IO_INPUTMODE_SET()         {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=(u32)8<<20;}


#define DataToBcd(x)                      ((x/10)*16+(x%10))
#define BcdToData(x)                      ((x/16)*10+(x%16))


//DS1302地址定义
#define DS1302_SEC_ADD			    0x80		//秒数据地址
#define DS1302_MIN_ADD			    0x82		//分数据地址
#define DS1302_HOUR_ADD			    0x84		//时数据地址
#define DS1302_DAY_ADD			    0x86		//日数据地址
#define DS1302_MON_ADD		        0x88		//月数据地址
#define DS1302_WEEK_ADD			    0x8A		//星期数据地址
#define DS1302_YEAR_ADD			    0x8C		//年数据地址
#define DS1302_CTRL_ADD		        0x8E		//控制数据地址

typedef struct
{ 
    volatile uint8_t sec;
    volatile uint8_t min;
    volatile uint8_t hour;
    volatile uint8_t day;
    volatile uint8_t mon;
    volatile uint8_t week;
    volatile uint16_t year;
}TimeDate;

extern TimeDate calendar;


void DS1302_Init(void);
uint8_t DS1302_Check(void);
void DS1302_SetTime(TimeDate timeData);
void DS1302_ReadTime(void);
TimeDate GmtimeToLocalTime(time_t gmtime);
void WriteLocalGmtime(time_t gmTime);
time_t ReadLocalGmtime(void);
uint8_t Power_On_Cheak_DS1302(void);


#endif /* __DS1302_H */