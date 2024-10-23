#include "DS1302.h"
#include "systick.h"


TimeDate calendar;



//初始化IIC
void DS1302_Init(void)
{				     
    GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	//使能时钟
    
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能时钟
    
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
    
    
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5); 	//输出高
    
	GPIO_SetBits(GPIOB,GPIO_Pin_0); 	//输出高    
}



//向DS1302写入一个字节数据
void DS1302_Write_Byte(uint8_t addr, uint8_t data)
{
    uint8_t i;
    
	DS1302_CE(0);      //停止DS1302总线
    
	DS1302_CE(1);      //启动DS1302总线
    
	addr = addr & 0xFE;  //最低位置零，写数据
    
	DS1302_IO_OUTMODE_SET();
    
	for(i = 0; i < 8; i ++)  //写地址
	{
        if(addr & 0x01)
        {
            DS1302_IO(1);        
        }            
		else
        {
            DS1302_IO(0);
        }            

		DS1302_SCLK(1);    //产生时钟
        
		DS1302_SCLK(0);
        
		addr = addr>>1;
	}
    
	for (i = 0; i < 8; i ++) //写数据
	{
        if(data & 0x01)
        {
			DS1302_IO(1);
        }
        else
        {
			DS1302_IO(0);
        }
		
		DS1302_SCLK(1);   //产生时钟
        
		DS1302_SCLK(0);
        
        data = data>>1;
	}
    
	DS1302_CE(0);      //停止DS1302总线
}



//从DS1302读出一个字节数据
uint8_t DS1302_Read_Byte(uint8_t addr)
{
    uint8_t i,temp;
    
	DS1302_CE(0);      //停止DS1302总线
    
	DS1302_CE(1);      //启动DS1302总线
    
	addr = addr | 0x01;  //最低位置高，读数据
    
	DS1302_IO_OUTMODE_SET();
    
	for(i = 0; i < 8; i ++)  //写地址
	{
        if(addr & 0x01)
        {
            DS1302_IO(1);
        }
		else
        {
            DS1302_IO(0);
        }
			
		DS1302_SCLK(1);    //产生时钟
        
		DS1302_SCLK(0);
        
		addr = addr>>1;
	}
    
	DS1302_IO_INPUTMODE_SET();
    
	for (i = 0; i < 8; i ++) //读数据
	{
        temp = temp >> 1;
        
        uint8_t state = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
		if(state)
        {
            temp |= 0x80;
        }
		else
        {
            temp &= 0x7F;
        }
        
		DS1302_SCLK(1);   //产生时钟
        
		DS1302_SCLK(0);
	}
    
	DS1302_CE(0);      //停止DS1302总线
    
	return temp;
}



//检查DS1302是否正常
//通过对星期寄存器读取校验判断是否正常
//返回1:检测失败
//返回0:检测成功
uint8_t DS1302_Check(void)
{
    uint8_t temp;
    
    DS1302_Init();
    
 	temp = DS1302_Read_Byte(DS1302_WEEK_ADD);   //先读取一次，不读取数据不正常
	
 	temp = DS1302_Read_Byte(DS1302_WEEK_ADD);	//不&0x07，验证数据正确性，高位正常为0
    
    if(1<=temp && temp<=7)
    {
        return 0;
    }
    else    //未设置星期
    {
        DS1302_Write_Byte(DS1302_CTRL_ADD,0x00);            //关闭写保护
        
        DS1302_Write_Byte(DS1302_WEEK_ADD,DataToBcd(1));    //周
        
        DS1302_Write_Byte(DS1302_CTRL_ADD,0x80);            //打开写保护
        
        temp = (DS1302_Read_Byte(DS1302_WEEK_ADD)) & 0x07;  //星期是占用低三位，只保留低三位
        
        if(temp == 1) return 0;
    }
    
    return 1;
}



uint8_t Power_On_Cheak_DS1302(void)
{   
    return (DS1302_Check() ? false : true);  
}
    


//向DS1302写入时间数据
void DS1302_SetTime(TimeDate timeData)
{
	DS1302_Write_Byte(DS1302_CTRL_ADD,0x00);     	    //关闭写保护
    
	DS1302_Write_Byte(DS1302_SEC_ADD, 0x80);            //暂停时钟
    
	DS1302_Write_Byte(DS1302_YEAR_ADD,DataToBcd((timeData.year)));  //年 
    
#if USE_WEEK
	DS1302_Write_Byte(DS1302_WEEK_ADD,DataToBcd(timeData.week));	//周 	
#endif
    
	DS1302_Write_Byte(DS1302_MON_ADD, DataToBcd(timeData.mon));	    //月 
    
	DS1302_Write_Byte(DS1302_DAY_ADD, DataToBcd(timeData.day));		//日
    
	DS1302_Write_Byte(DS1302_HOUR_ADD,DataToBcd(timeData.hour));    //时
    
	DS1302_Write_Byte(DS1302_MIN_ADD, DataToBcd(timeData.min));		//分
    
	DS1302_Write_Byte(DS1302_SEC_ADD, DataToBcd(timeData.sec));		//秒，开启时钟
    
	DS1302_Write_Byte(DS1302_CTRL_ADD,0x80);		    			//打开写保护 
}



//从DS302读出时钟数据
void DS1302_ReadTime(void)  
{
	uint8_t time_buf[8];
    
	time_buf[1] = DS1302_Read_Byte(DS1302_YEAR_ADD);
    
	time_buf[2] = DS1302_Read_Byte(DS1302_MON_ADD);
    
	time_buf[3] = DS1302_Read_Byte(DS1302_DAY_ADD);
    
	time_buf[4] = DS1302_Read_Byte(DS1302_HOUR_ADD);
    
	time_buf[5] = DS1302_Read_Byte(DS1302_MIN_ADD);
    
	time_buf[6] = DS1302_Read_Byte(DS1302_SEC_ADD);
    
#if USE_WEEK
	time_buf[7] = DS1302_Read_Byte(DS1302_WEEK_ADD);	
#endif
    
	calendar.year = (time_buf[1]>>4)*10+(time_buf[1]&0x0f) + 2000;
    
	calendar.mon  = (time_buf[2]>>4)*10+(time_buf[2]&0x0f);
	
	calendar.day  = (time_buf[3]>>4)*10+(time_buf[3]&0x0f);
	
	calendar.hour = (time_buf[4]>>4)*10+(time_buf[4]&0x0f);
	
	calendar.min  = (time_buf[5]>>4)*10+(time_buf[5]&0x0f);
	
	calendar.sec  = (time_buf[6]>>4)*10+(time_buf[6]&0x0f); 
    
#if USE_WEEK
	calendar.week = time_buf[7]&0x07;
#endif
}



TimeDate GmtimeToLocalTime(time_t gmTime)
{
    time_t gmt = gmTime + 28800;                        //将UTC时间戳转换为(U+8北京时间)UTC+8时间戳
    struct tm *localTime;                               //声明一个tm结构体指针
    TimeDate calendar;
    
    localTime = localtime(&gmt);                        //使用localtime将UTC时间戳转换为本地时间，不能使用gmtime,转换为乱码

    calendar.year = localTime->tm_year + 1900;          //自1900年起的年数
    
    calendar.mon  = localTime->tm_mon + 1;              //月,范围从 0 到 11 
    
    calendar.day  = localTime->tm_mday;
    
    calendar.hour = localTime->tm_hour;
    
    calendar.min  = localTime->tm_min;
    
    calendar.sec  = localTime->tm_sec;
    
    SystemPrintf("System time : %d-%d-%d %d:%d:%d", calendar.year, calendar.mon, calendar.day, \
           calendar.hour, calendar.min, calendar.sec);
    
    return calendar;   
}



void WriteLocalGmtime(time_t gmTime)
{
    TimeDate localTime;
    
    localTime = GmtimeToLocalTime(gmTime);
    
    localTime.year %= 100;//DS1302 year [0,99] 2024 -》24
    
    DS1302_SetTime(localTime);
}



time_t ReadLocalGmtime(void)
{   
    time_t localGmtime = 0;
    struct  tm localTime;
    
    DS1302_ReadTime();
    
    localTime.tm_year = calendar.year - 1900;           //自1900年起的年数
    
    localTime.tm_mon  = calendar.mon - 1;               //月,范围从 0 到 11  
    
    localTime.tm_mday = calendar.day;
    
    localTime.tm_hour = calendar.hour;
    
    localTime.tm_min  = calendar.min;
    
    localTime.tm_sec  = calendar.sec;
    
    localGmtime = mktime(&localTime) - 28800;           //记录的是北京时间+8,计算UTC时间

    SystemPrintf("localTime:%d", localGmtime);
    
    return localGmtime;
} 