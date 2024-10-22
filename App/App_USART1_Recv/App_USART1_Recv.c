#include "App_USART1_Recv.h"
#include "USART1.h"

uint8_t MyUrast1RecvBuff[U1_RECV_BUFF_SIZE];

uint8_t MyUrast1RecvLen = 0;  //获取的数据长度



//void U1_ATCommand(uint8_t *pBuffer, uint8_t pBufferLen)
//{  
//    uint8_t * pAtBuff = NULL;
//    uint8_t atBuffLen = 0;
//    uint8_t buffMaxLen = U1_RECV_BUFF_SIZE - 5;//去除偏移长度3 + 字符串结束符2
//    
//    if(pBuffer[2] == ':')   //判断是AT数据，进行数据偏移
//    {
//        pAtBuff = &pBuffer[3];
//        atBuffLen = pBufferLen - 3;
//    }
//    else
//    {
//        pAtBuff = pBuffer;
//        atBuffLen = pBufferLen;
//    }

//    if(atBuffLen <= buffMaxLen) 
//    {
//        SendATCmdPrintf("%s" , pAtBuff);
//        pAtBuff[atBuffLen++] = '\r';
//        pAtBuff[atBuffLen++] = '\n';
//        ATCmdSend(pAtBuff, atBuffLen);
//    }
//    else
//    {
//        Usart1_Recv_Printf("AT data buffer is too length, max len : %d\r\n", buffMaxLen);
//    }
//}



////读取用户保存区数据
//void U1_RECommand(uint8_t *pBuffer)
//{
//    uint8_t *pReBuff =  pBuffer;
//    int add, len;

//    if(sscanf((const char *)pReBuff, "RE %d %d", &add, &len) == 2  && len < 100)
//    {
//        char *pReadData = NULL;
//        uint8_t i = 0;
//        pReadData = malloc(len);
//        ReadLocalData(add, (uint8_t *)pReadData, len);     
//        UsartRecv_Printf("System read user data, add : %d, data :", add);
//        for(i=0; i<len; i++)
//        {
//            UsartRecv_Printf(" 0x%02x", pReadData[i]);
//        }
//        UsartRecv_Printf("\r\n");
//        free(pReadData);
//    }
//}



////写入用户保存区数据
//void U1_WECommand(u8 *pBuffer)
//{
//    u8 *pWeBuff =  pBuffer;
//    int add, data;

//    if(sscanf((const char *)pWeBuff, "WE %d %d", &add, &data) == 2 && data <= 0xff)
//    {
//        u8 tempBuff[2] = {0};
//        tempBuff[0] =  (u8)(data);        
//        WriteLocalData(add, tempBuff, 1);
//    }   
//}



//void U1_Q1Command(u8 *pBuffer)
//{
//    u8 byTemp = 0;
//    byTemp=pBuffer[2];

//    switch(byTemp)
//    {
//        case 'A':
//            if(pBuffer[3] == '1')   GPIO_SetBits(GPIOC, GPIO_Pin_7); 
//            else                    GPIO_ResetBits(GPIOC, GPIO_Pin_7); 
//            break;
//        case 'B':
//            {   
//                u8 qrcode[] = "123456789";
//                DWIN_WirteData(QRCODE_WECHAT_ADD, qrcode, strlen((char*)qrcode));
//                DWIN_ChangePage(PayPage);
//                DWIN_ChangeLight(100); 
//            }
//            break;

//        
//        case 'C':
//        case 'c':
//            {
//                switch(pBuffer[3])
//                {
//                    case '1':
//                        {
//                            u8 qrcode[256];
//                            
//                            memset(qrcode, 0, sizeof(qrcode));                                                       
//                            sprintf((char*)qrcode, "%s?type=pay&serialNo=%s&price=0.01", gURL, gSerial);
//                            DWIN_WirteData(QRCODE_WECHAT_ADD, qrcode, strlen((char*)qrcode));
//                            DWIN_ChangePage(PayPage);
//                            DWIN_ChangeLight(100);
//                        }
//                        break;
//                }
//            }
//            break;
//        
//        case 'D':
//        case 'd':
//            {
//                TimeDate time;
//                int year, mon, day, hour, min, sec, week;
//                if( (sscanf((const char *)pBuffer, "Q1D %d-%d-%d %d:%d:%d %d", &year, &mon, &day, \
//                                                                            &hour, &min, &sec, &week) == 7) ||\
//                    (sscanf((const char *)pBuffer, "q1d %d-%d-%d %d:%d:%d %d", &year, &mon, &day, \
//                                                                            &hour, &min, &sec, &week) == 7))
//                {     
//                    time.year = (year);
//                    time.mon  = (mon);
//                    time.day  = (day );
//                    time.hour = (hour);
//                    time.min  = (min );
//                    time.sec  = (sec );
//                    time.week = (week);
//                    DS1302_SetTime(time);
//                    UsartRecv_Printf("Set time: %d-%d-%d %d:%d:%d %d\r\n", time.year, time.mon, time.day, \
//                                                                           time.hour, time.min, time.sec, time.week);
//                }
//            }
//            break;         
//            
//        case 'L':
//            {
//                int lightMode, lightMin, lightMax;
//                u8  minValue[2], maxValue[2];
//                if( (sscanf((const char *)pBuffer, "Q1L %d %d %d", &lightMode, &lightMin, &lightMax) == 3))
//                {
//                    ledWorkMode = lightMode;  
//                    ledMinValue = lightMin;
//                    ledMaxValue = lightMax;   
//                    minValue[0] = ledMinValue / 100;
//                    minValue[1] = ledMinValue % 100;
//                    maxValue[0] = ledMaxValue / 100;
//                    maxValue[1] = ledMaxValue % 100;
//                    WriteLocalData(LED_SET_MODE_ADD, &ledWorkMode, 1);  
//                    WriteLocalData(LED_SET_MIN_ADD, minValue, 2);  
//                    WriteLocalData(LED_SET_MAX_ADD, maxValue, 2);  
//                    printf("Q1L OK\r\n");
//                }
//            }
//            break;
//            

//            
//        case 'S':
//            FDPM_1DCodeSend(FDPM_SLAVE_ADD, true);
//            break;
//            
//        case 'E':
//            FDPM_1DCodeSend(FDPM_SLAVE_ADD, false);
//            break;

//               
//        case 'T':
//        case 't':            
//            DS1302_ReadTime();
//            UsartRecv_Printf("System time : %d-%d-%d %d:%d:%d %d\r\n", calendar.year, calendar.mon, calendar.day, \
//                                                                       calendar.hour, calendar.min, calendar.sec, calendar.week);           
//            break;

//        case 'X':
//            break;
//            
//        case 'Y':  
//            break;
//            
//        case 'R':
//            delay_ms(10);
//            __set_FAULTMASK(1); // 关闭所有中断
//            NVIC_SystemReset(); // 复位           
//            break;
//        
//        case 'P':
//            {
//                u8 proClass;
//                if(pBuffer[3] == '0')   
//                {
//                   proClass = 0XFF;
//                }
//                else if(pBuffer[3] == '1')  
//                {
//                   proClass = ONENET_CLASS_FORMAL;
//                }                
//                else if(pBuffer[3] == '2')   
//                {
//                   proClass = ONENET_CLASS_TEST;
//                }
//                WriteLocalData(ONENET_PRO_CLASS_ADD, &proClass, 1);  
//                ReadLocalData(ONENET_PRO_CLASS_ADD, &proClass, 1); 
//                SystemPrintf("product class: %d", proClass);
//            } 
//            break;


//            
//        default:
//            break;
//    }
//}



//void U1_HELPCommand(void)
//{
//    Usart1_Recv_Printf("-------------------- 帮助菜单 --------------------\r\n");
//}



//uint8_t ParseU1Command(uint8_t *pCommad)
//{
//    uint8_t result = U1_COMMAND_NULL;

//    if((pCommad[0] == 'A' || pCommad[0] == 'a')&&\
//       (pCommad[1] == 'T' || pCommad[1] == 't')) 
//    {
//        result = U1_COMMAND_AT;      
//    }
//    else if((pCommad[0] == 'R' || pCommad[0] == 'r')&&\
//            (pCommad[1] == 'E' || pCommad[1] == 'e'))
//    {
//        result = U1_COMMAND_RE;      
//    }
//    else if((pCommad[0] == 'W' || pCommad[0] == 'w')&&\
//            (pCommad[1] == 'E' || pCommad[1] == 'e'))
//    {
//        result = U1_COMMAND_WE;      
//    } 
//    else if((pCommad[0] == 'R' || pCommad[0] == 'r')&&\
//            (pCommad[1] == 'O' || pCommad[1] == 'o'))
//    {
//        result = U1_COMMAND_RO;      
//    }
//    else if((pCommad[0] == 'Q' || pCommad[0] == 'q') && pCommad[1] == '1')
//    {
//        result = U1_COMMAND_Q1;      
//    } 
//    else if((pCommad[0] == 'H' || pCommad[0] == 'h')&&\
//            (pCommad[1] == 'E' || pCommad[1] == 'e')&&\
//            (pCommad[2] == 'L' || pCommad[2] == 'l')&&\
//            (pCommad[3] == 'P' || pCommad[3] == 'p'))
//    {
//        result = U1_COMMAND_HELP;      
//    }
//    else 
//    {
//        result = U1_COMMAND_NULL;
//    }
//    return result;
//}



//void U1CommandFunc(void)
//{

//    uint8_t CommandType = U1_COMMAND_NULL;
//    
//    CommandType = ParseU1Command(MyUrast1RecvBuff);

//    switch(CommandType) 
//    {
//        case U1_COMMAND_AT:
//            U1_ATCommand(MyUrast1RecvBuff, MyUrast1RecvLen);
//            break;  

//        case U1_COMMAND_RE:
//            U1_RECommand(MyUrast1RecvBuff);
//            break;

//        case U1_COMMAND_WE:
//            U1_WECommand(MyUrast1RecvBuff);
//            break;

//        case U1_COMMAND_RO:
//            //U1ReadSTMFlashOrder(MyUrast1RecvBuff);
//            break;

//        case U1_COMMAND_Q1:
//            U1_Q1Command(MyUrast1RecvBuff);
//            break;

//        case U1_COMMAND_HELP:
//            U1_HELPCommand();
//            break;
//            
//        default:
//            break;
//    }   

//}  




void USART1_Recv_Task(void * pvParameters)
{
    uint8_t error;

    while(1)
    {
        BaseType_t xReturn = xSemaphoreTake(xUSART1_Recv_Semaphore, pdMS_TO_TICKS(10));
		if(xReturn == pdTRUE)  //成功接收
		{        
            if(USART1_RX_CNT)
            {
                //复制数据到MyFrameRecvBuff
                memset(MyUrast1RecvBuff, 0, U1_RECV_BUFF_SIZE);
                memcpy(MyUrast1RecvBuff, USART1_RX_BUF, USART1_RX_CNT);
                MyUrast1RecvLen = USART1_RX_CNT;
                memset(USART1_RX_BUF, 0, U1_RECV_BUFF_SIZE);
                USART1_RX_CNT = 0;
                
                //接收数据打印
                printf("\r\nUSART1 Recv Len : %d\r\n", MyUrast1RecvLen);
                printf("USART1 Recv str : %s\r\n",MyUrast1RecvBuff);
                
//                U1CommandFunc();
            }
		}
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}