#include "DS1302.h"
#include "systick.h"


TimeDate calendar;



//��ʼ��IIC
void DS1302_Init(void)
{				     
    GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	//ʹ��ʱ��
    
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��ʱ��
    
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
    
    
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5); 	//�����
    
	GPIO_SetBits(GPIOB,GPIO_Pin_0); 	//�����    
}



//��DS1302д��һ���ֽ�����
void DS1302_Write_Byte(uint8_t addr, uint8_t data)
{
    uint8_t i;
    
	DS1302_CE(0);      //ֹͣDS1302����
    
	DS1302_CE(1);      //����DS1302����
    
	addr = addr & 0xFE;  //���λ���㣬д����
    
	DS1302_IO_OUTMODE_SET();
    
	for(i = 0; i < 8; i ++)  //д��ַ
	{
        if(addr & 0x01)
        {
            DS1302_IO(1);        
        }            
		else
        {
            DS1302_IO(0);
        }            

		DS1302_SCLK(1);    //����ʱ��
        
		DS1302_SCLK(0);
        
		addr = addr>>1;
	}
    
	for (i = 0; i < 8; i ++) //д����
	{
        if(data & 0x01)
        {
			DS1302_IO(1);
        }
        else
        {
			DS1302_IO(0);
        }
		
		DS1302_SCLK(1);   //����ʱ��
        
		DS1302_SCLK(0);
        
        data = data>>1;
	}
    
	DS1302_CE(0);      //ֹͣDS1302����
}



//��DS1302����һ���ֽ�����
uint8_t DS1302_Read_Byte(uint8_t addr)
{
    uint8_t i,temp;
    
	DS1302_CE(0);      //ֹͣDS1302����
    
	DS1302_CE(1);      //����DS1302����
    
	addr = addr | 0x01;  //���λ�øߣ�������
    
	DS1302_IO_OUTMODE_SET();
    
	for(i = 0; i < 8; i ++)  //д��ַ
	{
        if(addr & 0x01)
        {
            DS1302_IO(1);
        }
		else
        {
            DS1302_IO(0);
        }
			
		DS1302_SCLK(1);    //����ʱ��
        
		DS1302_SCLK(0);
        
		addr = addr>>1;
	}
    
	DS1302_IO_INPUTMODE_SET();
    
	for (i = 0; i < 8; i ++) //������
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
        
		DS1302_SCLK(1);   //����ʱ��
        
		DS1302_SCLK(0);
	}
    
	DS1302_CE(0);      //ֹͣDS1302����
    
	return temp;
}



//���DS1302�Ƿ�����
//ͨ�������ڼĴ�����ȡУ���ж��Ƿ�����
//����1:���ʧ��
//����0:���ɹ�
uint8_t DS1302_Check(void)
{
    uint8_t temp;
    
    DS1302_Init();
    
 	temp = DS1302_Read_Byte(DS1302_WEEK_ADD);   //�ȶ�ȡһ�Σ�����ȡ���ݲ�����
	
 	temp = DS1302_Read_Byte(DS1302_WEEK_ADD);	//��&0x07����֤������ȷ�ԣ���λ����Ϊ0
    
    if(1<=temp && temp<=7)
    {
        return 0;
    }
    else    //δ��������
    {
        DS1302_Write_Byte(DS1302_CTRL_ADD,0x00);            //�ر�д����
        
        DS1302_Write_Byte(DS1302_WEEK_ADD,DataToBcd(1));    //��
        
        DS1302_Write_Byte(DS1302_CTRL_ADD,0x80);            //��д����
        
        temp = (DS1302_Read_Byte(DS1302_WEEK_ADD)) & 0x07;  //������ռ�õ���λ��ֻ��������λ
        
        if(temp == 1) return 0;
    }
    
    return 1;
}



uint8_t Power_On_Cheak_DS1302(void)
{   
    return (DS1302_Check() ? false : true);  
}
    


//��DS1302д��ʱ������
void DS1302_SetTime(TimeDate timeData)
{
	DS1302_Write_Byte(DS1302_CTRL_ADD,0x00);     	    //�ر�д����
    
	DS1302_Write_Byte(DS1302_SEC_ADD, 0x80);            //��ͣʱ��
    
	DS1302_Write_Byte(DS1302_YEAR_ADD,DataToBcd((timeData.year)));  //�� 
    
#if USE_WEEK
	DS1302_Write_Byte(DS1302_WEEK_ADD,DataToBcd(timeData.week));	//�� 	
#endif
    
	DS1302_Write_Byte(DS1302_MON_ADD, DataToBcd(timeData.mon));	    //�� 
    
	DS1302_Write_Byte(DS1302_DAY_ADD, DataToBcd(timeData.day));		//��
    
	DS1302_Write_Byte(DS1302_HOUR_ADD,DataToBcd(timeData.hour));    //ʱ
    
	DS1302_Write_Byte(DS1302_MIN_ADD, DataToBcd(timeData.min));		//��
    
	DS1302_Write_Byte(DS1302_SEC_ADD, DataToBcd(timeData.sec));		//�룬����ʱ��
    
	DS1302_Write_Byte(DS1302_CTRL_ADD,0x80);		    			//��д���� 
}



//��DS302����ʱ������
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
    time_t gmt = gmTime + 28800;                        //��UTCʱ���ת��Ϊ(U+8����ʱ��)UTC+8ʱ���
    struct tm *localTime;                               //����һ��tm�ṹ��ָ��
    TimeDate calendar;
    
    localTime = localtime(&gmt);                        //ʹ��localtime��UTCʱ���ת��Ϊ����ʱ�䣬����ʹ��gmtime,ת��Ϊ����

    calendar.year = localTime->tm_year + 1900;          //��1900���������
    
    calendar.mon  = localTime->tm_mon + 1;              //��,��Χ�� 0 �� 11 
    
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
    
    localTime.year %= 100;//DS1302 year [0,99] 2024 -��24
    
    DS1302_SetTime(localTime);
}



time_t ReadLocalGmtime(void)
{   
    time_t localGmtime = 0;
    struct  tm localTime;
    
    DS1302_ReadTime();
    
    localTime.tm_year = calendar.year - 1900;           //��1900���������
    
    localTime.tm_mon  = calendar.mon - 1;               //��,��Χ�� 0 �� 11  
    
    localTime.tm_mday = calendar.day;
    
    localTime.tm_hour = calendar.hour;
    
    localTime.tm_min  = calendar.min;
    
    localTime.tm_sec  = calendar.sec;
    
    localGmtime = mktime(&localTime) - 28800;           //��¼���Ǳ���ʱ��+8,����UTCʱ��

    SystemPrintf("localTime:%d", localGmtime);
    
    return localGmtime;
} 