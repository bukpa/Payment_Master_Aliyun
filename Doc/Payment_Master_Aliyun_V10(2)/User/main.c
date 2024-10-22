#include "stm32f10x.h"
#include "systick.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BSP_USART.h"


/*
    1��TaskHandle_t - ����������, �������ú͹�������
    
    2��StartTask_Handler - ���������͵ı���, ������������ĳ���ض������ڴ�������ʱ��FreeRTOS �Ὣ����ľ����ֵ�����������
    
    3������һ�������������������ڴ洢��������ʱ���ص���������ͨ�������������Զ�������и��ֲ���������𡢻ָ���ɾ���ȡ�
    
    4��ͨ���������������� TaskHandle_t StartTask_Handler��������� FreeRTOS �ж�������д���������Ϳ��ơ���������ָ������
       ���ƿ��ָ�룬ʹ���ܹ��ڲ�ͬ�ĺ����������������úͲ�������ͨ��ʹ��������������ʵ�ֶ�����ľ�ϸ���ƣ��������������𡢻ָ���ɾ������Ȳ�����
    
*/
TaskHandle_t xStartTask_Handler;
TaskHandle_t xTask1_Handler;
TaskHandle_t xTask2_Handler;


void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}

/**
  * @brief  ������1 - ����ʽ�ȴ���ȡ�ź�
  * @param  void * pvParameters     �������͵�ָ����������ⲿ����һ��������ַ��Ҳ���Դ���NULL
  * @retval �޷���ֵ
  */
void vTask1(void * pvParameters)
{

    while(1)
    {
        USART_SendByte(USART1,0x61);
        
        vTaskDelay(pdMS_TO_TICKS(8000));

    }

}



/**
  * @brief  ������2 - ������ʽ��ȡ�źţ����ȴ�ֱ�ӻ�ȡ��
  * @param  void * pvParameters     �������͵�ָ����������ⲿ����һ��������ַ��Ҳ���Դ���NULL
  * @retval �޷���ֵ
  */
void vTask2(void * pvParameters)
{

    
    while(1)
    {

        vTaskDelay(4000);
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
    
    /* �����ٽ���, ȷ�����������񹲴�ͬһ������ */
    taskENTER_CRITICAL();  

    /* ����������1 */    
    xTaskCreate(
        (TaskFunction_t)          vTask1,               //����1������ָ������ - TaskFunction_t����, ���ⲿ����һ��������������ַ��   [����]�������������Ϊ�����ִ����
        (const char *)            "Task1",              //����2���ַ���ָ������ - const char *����, ���ⲿ����һ���������ƣ���ַ��     [����]��ͨ����һ����������ļ���ַ���, ���ڵ��Ժͼ���, ��󳤶ȣ�configMAX_TASK_NAME_LEN
        (configSTACK_DEPTH_TYPE)  128,                  //����3��uint16_t���� - configSTACK_DEPTH_TYPE����, �����ջ�����             [����]����ջ��ȵĵ�λ�ǰ��֣�2���ֽڣ�,���ֵ����������������ʱ����Ķ�ջ�ռ��С��128 * 2 = 256�ֽ�
        (void * const)            NULL,                 //����4���������κε�ָ������, ���ⲿ����һ��������ַ                          [����]�����ݸ��������Ĳ���ָ�롣���Դ����κ����͵����ݣ��������Ὣ��ת��Ϊ��ȷ������ʹ�á��������Ҫ���ݲ��������Դ��� NULL -> (void * const) NULL
        (UBaseType_t)             3,                    //����5��unsigned long���� - UBaseType_t����, ��������ȼ�                     [����]����ֵԽ�����ȼ�Խ��
        (TaskHandle_t *)          &xTask1_Handler       //����6��ָ�� tskTaskControlBlock �ṹ���ָ������ - TaskHandle_t *���ͣ����������͵�ָ�룩,���ⲿ����һ�������������ĵ�ַ          [����]�������������������洢�ڴ�ָ��ָ��ı����С��������Ҫ�����������Դ��� NULL -> (TaskHandle_t *) NULL
    );
        
    /* ����������2 */    
    xTaskCreate(
        (TaskFunction_t)          vTask2,               //����1������ָ������ - TaskFunction_t����, ���ⲿ����һ��������������ַ��   [����]�������������Ϊ�����ִ����
        (const char *)            "Task2",              //����2���ַ���ָ������ - const char *����, ���ⲿ����һ���������ƣ���ַ��     [����]��ͨ����һ����������ļ���ַ���, ���ڵ��Ժͼ���, ��󳤶ȣ�configMAX_TASK_NAME_LEN
        (configSTACK_DEPTH_TYPE)  64,                   //����3��uint16_t���� - configSTACK_DEPTH_TYPE����, �����ջ�����             [����]����ջ��ȵĵ�λ�ǰ��֣�2���ֽڣ�,���ֵ����������������ʱ����Ķ�ջ�ռ��С��128 * 2 = 256�ֽ�
        (void * const)            NULL,                 //����4���������κε�ָ������, ���ⲿ����һ��������ַ                          [����]�����ݸ��������Ĳ���ָ�롣���Դ����κ����͵����ݣ��������Ὣ��ת��Ϊ��ȷ������ʹ�á��������Ҫ���ݲ��������Դ��� NULL -> (void * const) NULL
        (UBaseType_t)             2,                    //����5��unsigned long���� - UBaseType_t����, ��������ȼ�                     [����]����ֵԽ�����ȼ�Խ��
        (TaskHandle_t *)          &xTask2_Handler       //����6��ָ�� tskTaskControlBlock �ṹ���ָ������ - TaskHandle_t *���ͣ����������͵�ָ�룩,���ⲿ����һ�������������ĵ�ַ          [����]�������������������洢�ڴ�ָ��ָ��ı����С��������Ҫ�����������Դ��� NULL -> (TaskHandle_t *) NULL
    );    
    
    /* �˳��ٽ��� */
    taskEXIT_CRITICAL();
        
    /* �������� */
    vTaskDelete(xStartTask_Handler);

}

int main(void)
{
    /* ����ȫ�����ȼ����� FreeRTOS ��������Ϊ 4 0 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
    USART_Config();
    LED_Init();

    GPIO_SetBits(GPIOC, GPIO_Pin_9);
    
    /*
        1��xTaskCreate �� FreeRTOS �����ڴ�������ĺ���
        
        2��������������������ָ������ĸ������ԣ��������������������ơ���ջ��С��������������ȼ���������
        
        3���䷵��ֵΪ BaseType_t���ͣ�long���ͣ����᷵�ش��������״̬ -> pdPASS�����񴴽��ɹ�
                                                                          pdFAIL�����񴴽�ʧ��
    */
    BaseType_t Result = xTaskCreate(
        (TaskFunction_t)          vStartTask,           //����1������ָ������ - TaskFunction_t����, ���ⲿ����һ��������������ַ��   [����]�������������Ϊ�����ִ����
        (const char *)            "StartTask",          //����2���ַ���ָ������ - const char *����, ���ⲿ����һ���������ƣ���ַ��     [����]��ͨ����һ����������ļ���ַ���, ���ڵ��Ժͼ���, ��󳤶ȣ�configMAX_TASK_NAME_LEN
        (configSTACK_DEPTH_TYPE)  128,                  //����3��uint16_t���� - configSTACK_DEPTH_TYPE����, �����ջ�����             [����]����ջ��ȵĵ�λ�ǰ��֣�2���ֽڣ�,���ֵ����������������ʱ����Ķ�ջ�ռ��С��128 * 2 = 256�ֽ�
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
