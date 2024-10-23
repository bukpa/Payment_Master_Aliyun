#include "stm32f10x.h"
#include "systick.h"

#include "FreeRTOS.h"
#include "task.h"

#include "RCC_CLK_Config.h"
#include "USART1.h"
#include "TIM3.h"

#include "DS1302.h"

#include "App_USART1_Recv.h"



uint8_t Cheak_Result = 0;

/*
    1��TaskHandle_t - ����������, �������ú͹�������
    
    2��StartTask_Handler - ���������͵ı���, ������������ĳ���ض������ڴ�������ʱ��FreeRTOS �Ὣ����ľ����ֵ�����������
    
    3������һ�������������������ڴ洢��������ʱ���ص���������ͨ�������������Զ�������и��ֲ���������𡢻ָ���ɾ���ȡ�
    
    4��ͨ���������������� TaskHandle_t StartTask_Handler��������� FreeRTOS �ж�������д���������Ϳ��ơ���������ָ������
       ���ƿ��ָ�룬ʹ���ܹ��ڲ�ͬ�ĺ����������������úͲ�������ͨ��ʹ��������������ʵ�ֶ�����ľ�ϸ���ƣ��������������𡢻ָ���ɾ������Ȳ�����
    
*/
TaskHandle_t xStartTask_Handler;
TaskHandle_t xUSART1_Recv_Task_Handler;
TaskHandle_t xTask2_Handler;



/* 
    1��SemaphoreHandle_t ��һ��������ͣ��������úͲ����ź����򻥳���
    
    2��ʵ���ϣ�SemaphoreHandle_t ��ͨ�����Ͷ����������� QueueHandle_t���� QueueHandle_t ������һ��ָ�� QueueDefinition �ṹ���ָ��
       ����ʵ�ַ�ʽ���ź����Ͷ���ͳһ��һ�����ݽṹ�н��й���ʹ�� FreeRTOS �ڲ������͸�Ч
        2.1��QueueDefinition ����һ���ṹ�壬���ڶ�����е��ڲ����ݽṹ���� FreeRTOS ��ʵ���У��ź����ͻ�����Ҳ��ͨ�����л�����ʵ�ֵģ�������ǹ�����ͬ�Ļ����ṹ
        
        2.2��QueueHandle_t ����һ��ָ�� QueueDefinition �ṹ���ָ�����ͣ����ڱ�ʾ���еľ��
        
        2.3��SemaphoreHandle_t����һ���������� QueueHandle_t�����ڱ�ʾ�ź����򻥳����ľ��
        
    3��ͨ�����ַ�ʽ��FreeRTOS ����ʹ����ͬ�ĵײ������������С��ź����ͻ������������ں˵�ʵ��
*/
xSemaphoreHandle xUSART1_Recv_Semaphore = NULL;


void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}

uint8_t DATA_ARRAY[3]={0x61,0x62,0x63};



/**
  * @brief  ������2 - ������ʽ��ȡ�źţ����ȴ�ֱ�ӻ�ȡ��
  * @param  void * pvParameters     �������͵�ָ����������ⲿ����һ��������ַ��Ҳ���Դ���NULL
  * @retval �޷���ֵ
  */
void vTask2(void * pvParameters)
{

    
    while(1)
    {
//        USART1_SendData(DATA_ARRAY, 3);
//        delay_1us(1000000);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}



/**
  * @brief  ��ʼ���� ���ִ�к���
  * @param  void * pvParameters     �������͵�ָ����������ⲿ����һ��������ַ��Ҳ���Դ���NULL
  * @retval �޷���ֵ
  */
void vStartTask(void * pvParameters)
{   
    
    printf("hello task success!!\n");

    /* 
        1��xSemaphoreCreateBinary �������ڴ���һ����ʼ״̬Ϊ���ա��Ķ������ź���
    
        2����ν���ա�����ζ���ź����ڴ����������һ��������� xSemaphoreGive ���ͷ�һ�Σ�����ʹ�����ȴ����ź����������ȡ���ź���

        3��ͨ�������������ź���������ʵ��������ͬ�������磬һ�����������ĳ���¼�������ͨ�� xSemaphoreGive ֪ͨ��һ������
    
        4������ֵ������һ�� SemaphoreHandle_t ���͵ľ��������ź��������ɹ�����þ����Ч������ź�������ʧ�ܣ��򷵻� NULL
    */
    xUSART1_Recv_Semaphore = xSemaphoreCreateBinary();
    
    if(xUSART1_Recv_Semaphore == NULL)
    {
        printf("CREAT SIGNAL FAIL!\n");
    }
    else
    {
        printf("CREAT SIGNAL SUCCESS!\n");
    }
    
    /* �����ٽ���, ȷ�����������񹲴�ͬһ������ */
    taskENTER_CRITICAL();  

    /* ����������1 ����1���������߳� */    
    xTaskCreate(
        (TaskFunction_t)          USART1_Recv_Task,             //����1������ָ������ - TaskFunction_t����, ���ⲿ����һ��������������ַ��   [����]�������������Ϊ�����ִ����
        (const char *)            "USART1_Recv_Task",           //����2���ַ���ָ������ - const char *����, ���ⲿ����һ���������ƣ���ַ��     [����]��ͨ����һ����������ļ���ַ���, ���ڵ��Ժͼ���, ��󳤶ȣ�configMAX_TASK_NAME_LEN
        (configSTACK_DEPTH_TYPE)  1024,                         //����3��uint16_t���� - configSTACK_DEPTH_TYPE����, �����ջ�����             [����]����ջ��ȵĵ�λ�ǰ��֣�2���ֽڣ�,���ֵ����������������ʱ����Ķ�ջ�ռ��С��128 * 2 = 256�ֽ�
        (void * const)            NULL,                         //����4���������κε�ָ������, ���ⲿ����һ��������ַ                          [����]�����ݸ��������Ĳ���ָ�롣���Դ����κ����͵����ݣ��������Ὣ��ת��Ϊ��ȷ������ʹ�á��������Ҫ���ݲ��������Դ��� NULL -> (void * const) NULL
        (UBaseType_t)             4,                            //����5��unsigned long���� - UBaseType_t����, ��������ȼ�                     [����]����ֵԽ�����ȼ�Խ��
        (TaskHandle_t *)          &xUSART1_Recv_Task_Handler    //����6��ָ�� tskTaskControlBlock �ṹ���ָ������ - TaskHandle_t *���ͣ����������͵�ָ�룩,���ⲿ����һ�������������ĵ�ַ          [����]�������������������洢�ڴ�ָ��ָ��ı����С��������Ҫ�����������Դ��� NULL -> (TaskHandle_t *) NULL
    );
        
    /* ����������2 */    
    xTaskCreate(
        (TaskFunction_t)          vTask2,               //����1������ָ������ - TaskFunction_t����, ���ⲿ����һ��������������ַ��   [����]�������������Ϊ�����ִ����
        (const char *)            "Task2",              //����2���ַ���ָ������ - const char *����, ���ⲿ����һ���������ƣ���ַ��     [����]��ͨ����һ����������ļ���ַ���, ���ڵ��Ժͼ���, ��󳤶ȣ�configMAX_TASK_NAME_LEN
        (configSTACK_DEPTH_TYPE)  256,                  //����3��uint16_t���� - configSTACK_DEPTH_TYPE����, �����ջ�����             [����]����ջ��ȵĵ�λ�ǰ��֣�2���ֽڣ�,���ֵ����������������ʱ����Ķ�ջ�ռ��С��128 * 2 = 256�ֽ�
        (void * const)            NULL,                 //����4���������κε�ָ������, ���ⲿ����һ��������ַ                          [����]�����ݸ��������Ĳ���ָ�롣���Դ����κ����͵����ݣ��������Ὣ��ת��Ϊ��ȷ������ʹ�á��������Ҫ���ݲ��������Դ��� NULL -> (void * const) NULL
        (UBaseType_t)             4,                    //����5��unsigned long���� - UBaseType_t����, ��������ȼ�                     [����]����ֵԽ�����ȼ�Խ��
        (TaskHandle_t *)          &xTask2_Handler       //����6��ָ�� tskTaskControlBlock �ṹ���ָ������ - TaskHandle_t *���ͣ����������͵�ָ�룩,���ⲿ����һ�������������ĵ�ַ          [����]�������������������洢�ڴ�ָ��ָ��ı����С��������Ҫ�����������Դ��� NULL -> (TaskHandle_t *) NULL
    );    
    
    /* �˳��ٽ��� */
    taskEXIT_CRITICAL();
        
    /* �������� */
    vTaskDelete(xStartTask_Handler);

}
u8 ds1302IsNormal = false;
int main(void)
{
    HSE_SetSysClock(RCC_PLLMul_6);
    
    /* ����ȫ�����ȼ����� FreeRTOS ��������Ϊ 4 0 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
    USART1_Init();   
    
    TIM3_Init();
    
    Cheak_Result = Power_On_Cheak_DS1302();
    
    printf("\r\n[Cheak] >>> %s\r\n", (Cheak_Result ? "DS1302 State Normal!" : "Error:DS1302 State Abnormal!"));

    LED_Init();
//    USART_SendByte(USART1,0x61);
//    GPIO_ResetBits(GPIOC, GPIO_Pin_9);
    
    /*
        1��xTaskCreate �� FreeRTOS �����ڴ�������ĺ���
        
        2��������������������ָ������ĸ������ԣ��������������������ơ���ջ��С��������������ȼ���������
        
        3���䷵��ֵΪ BaseType_t���ͣ�long���ͣ����᷵�ش��������״̬ -> pdPASS�����񴴽��ɹ�
                                                                          pdFAIL�����񴴽�ʧ��
    */
    BaseType_t Result = xTaskCreate(
        (TaskFunction_t)          vStartTask,           //����1������ָ������ - TaskFunction_t����, ���ⲿ����һ��������������ַ��   [����]�������������Ϊ�����ִ����
        (const char *)            "StartTask",          //����2���ַ���ָ������ - const char *����, ���ⲿ����һ���������ƣ���ַ��     [����]��ͨ����һ����������ļ���ַ���, ���ڵ��Ժͼ���, ��󳤶ȣ�configMAX_TASK_NAME_LEN
        (configSTACK_DEPTH_TYPE)  256,                  //����3��uint16_t���� - configSTACK_DEPTH_TYPE����, �����ջ�����             [����]����ջ��ȵĵ�λ�ǰ��֣�2���ֽڣ�,���ֵ����������������ʱ����Ķ�ջ�ռ��С��128 * 2 = 256�ֽ�
        (void * const)            NULL,                 //����4���������κε�ָ������, ���ⲿ����һ��������ַ                          [����]�����ݸ��������Ĳ���ָ�롣���Դ����κ����͵����ݣ��������Ὣ��ת��Ϊ��ȷ������ʹ�á��������Ҫ���ݲ��������Դ��� NULL -> (void * const) NULL
        (UBaseType_t)             1,                    //����5��unsigned long���� - UBaseType_t����, ��������ȼ�                     [����]����ֵԽ�����ȼ�Խ��
        (TaskHandle_t *)          &xStartTask_Handler   //����6��ָ�� tskTaskControlBlock �ṹ���ָ������ - TaskHandle_t *���ͣ����������͵�ָ�룩,���ⲿ����һ�������������ĵ�ַ          [����]�������������������洢�ڴ�ָ��ָ��ı����С��������Ҫ�����������Դ��� NULL -> (TaskHandle_t *) NULL
    );
    printf(">>>result:%ld\n", Result);
        
    /* ������������� */
    vTaskStartScheduler();

    while(1)
    {   
        
    }
}
