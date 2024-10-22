#include "stm32f10x.h"
#include "systick.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BSP_USART.h"


/*
    1、TaskHandle_t - 任务句柄类型, 用于引用和管理任务
    
    2、StartTask_Handler - 任务句柄类型的变量, 它将用于引用某个特定任务。在创建任务时，FreeRTOS 会将任务的句柄赋值给这个变量。
    
    3、定义一个任务句柄变量可以用于存储创建任务时返回的任务句柄；通过任务句柄，可以对任务进行各种操作，如挂起、恢复、删除等。
    
    4、通过定义任务句柄变量 TaskHandle_t StartTask_Handler，你可以在 FreeRTOS 中对任务进行创建、管理和控制。任务句柄是指向任务
       控制块的指针，使你能够在不同的函数或上下文中引用和操作任务。通过使用任务句柄，可以实现对任务的精细控制，包括创建、挂起、恢复和删除任务等操作。
    
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
  * @brief  子任务1 - 阻塞式等待获取信号
  * @param  void * pvParameters     任意类型的指针变量，需外部传入一个变量地址，也可以传入NULL
  * @retval 无返回值
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
  * @brief  子任务2 - 非阻塞式获取信号（不等待直接获取）
  * @param  void * pvParameters     任意类型的指针变量，需外部传入一个变量地址，也可以传入NULL
  * @retval 无返回值
  */
void vTask2(void * pvParameters)
{

    
    while(1)
    {

        vTaskDelay(4000);
    }
}



/**
  * @brief  开始任务 入口执行函数
  * @param  void * pvParameters     任意类型的指针变量，需外部传入一个变量地址，也可以传入NULL
  * @retval 无返回值
  */
void vStartTask(void * pvParameters)
{   
    
    printf("hello task success!!\n");
    
    /* 进入临界区, 确保俩个子任务共处同一起跑线 */
    taskENTER_CRITICAL();  

    /* 创建子任务1 */    
    xTaskCreate(
        (TaskFunction_t)          vTask1,               //参数1：函数指针类型 - TaskFunction_t类型, 需外部传入一个任务函数名（地址）   [补充]：这个函数将作为任务的执行体
        (const char *)            "Task1",              //参数2：字符串指针类型 - const char *类型, 需外部传入一个任务名称（地址）     [补充]：通常是一个描述任务的简短字符串, 用于调试和监视, 最大长度：configMAX_TASK_NAME_LEN
        (configSTACK_DEPTH_TYPE)  128,                  //参数3：uint16_t类型 - configSTACK_DEPTH_TYPE类型, 任务堆栈的深度             [补充]：堆栈深度的单位是半字（2个字节）,这个值定义了任务在运行时所需的堆栈空间大小。128 * 2 = 256字节
        (void * const)            NULL,                 //参数4：可以是任何的指针类型, 需外部传入一个变量地址                          [补充]：传递给任务函数的参数指针。可以传递任何类型的数据，任务函数会将其转换为正确的类型使用。如果不需要传递参数，可以传递 NULL -> (void * const) NULL
        (UBaseType_t)             3,                    //参数5：unsigned long类型 - UBaseType_t类型, 任务的优先级                     [补充]：数值越大，优先级越高
        (TaskHandle_t *)          &xTask1_Handler       //参数6：指向 tskTaskControlBlock 结构体的指针类型 - TaskHandle_t *类型（任务句柄类型的指针）,需外部传入一个任务句柄变量的地址          [补充]：创建任务后，任务句柄会存储在此指针指向的变量中。如果不需要任务句柄，可以传递 NULL -> (TaskHandle_t *) NULL
    );
        
    /* 创建子任务2 */    
    xTaskCreate(
        (TaskFunction_t)          vTask2,               //参数1：函数指针类型 - TaskFunction_t类型, 需外部传入一个任务函数名（地址）   [补充]：这个函数将作为任务的执行体
        (const char *)            "Task2",              //参数2：字符串指针类型 - const char *类型, 需外部传入一个任务名称（地址）     [补充]：通常是一个描述任务的简短字符串, 用于调试和监视, 最大长度：configMAX_TASK_NAME_LEN
        (configSTACK_DEPTH_TYPE)  64,                   //参数3：uint16_t类型 - configSTACK_DEPTH_TYPE类型, 任务堆栈的深度             [补充]：堆栈深度的单位是半字（2个字节）,这个值定义了任务在运行时所需的堆栈空间大小。128 * 2 = 256字节
        (void * const)            NULL,                 //参数4：可以是任何的指针类型, 需外部传入一个变量地址                          [补充]：传递给任务函数的参数指针。可以传递任何类型的数据，任务函数会将其转换为正确的类型使用。如果不需要传递参数，可以传递 NULL -> (void * const) NULL
        (UBaseType_t)             2,                    //参数5：unsigned long类型 - UBaseType_t类型, 任务的优先级                     [补充]：数值越大，优先级越高
        (TaskHandle_t *)          &xTask2_Handler       //参数6：指向 tskTaskControlBlock 结构体的指针类型 - TaskHandle_t *类型（任务句柄类型的指针）,需外部传入一个任务句柄变量的地址          [补充]：创建任务后，任务句柄会存储在此指针指向的变量中。如果不需要任务句柄，可以传递 NULL -> (TaskHandle_t *) NULL
    );    
    
    /* 退出临界区 */
    taskEXIT_CRITICAL();
        
    /* 销毁任务 */
    vTaskDelete(xStartTask_Handler);

}

int main(void)
{
    /* 配置全局优先级分组 FreeRTOS 必须设置为 4 0 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
    USART_Config();
    LED_Init();

    GPIO_SetBits(GPIOC, GPIO_Pin_9);
    
    /*
        1、xTaskCreate 是 FreeRTOS 中用于创建任务的函数
        
        2、它有六个参数，用于指定任务的各个属性，包括任务函数、任务名称、堆栈大小、任务参数、优先级和任务句柄
        
        3、其返回值为 BaseType_t类型（long类型），会返回创建任务的状态 -> pdPASS：任务创建成功
                                                                          pdFAIL：任务创建失败
    */
    BaseType_t Result = xTaskCreate(
        (TaskFunction_t)          vStartTask,           //参数1：函数指针类型 - TaskFunction_t类型, 需外部传入一个任务函数名（地址）   [补充]：这个函数将作为任务的执行体
        (const char *)            "StartTask",          //参数2：字符串指针类型 - const char *类型, 需外部传入一个任务名称（地址）     [补充]：通常是一个描述任务的简短字符串, 用于调试和监视, 最大长度：configMAX_TASK_NAME_LEN
        (configSTACK_DEPTH_TYPE)  128,                  //参数3：uint16_t类型 - configSTACK_DEPTH_TYPE类型, 任务堆栈的深度             [补充]：堆栈深度的单位是半字（2个字节）,这个值定义了任务在运行时所需的堆栈空间大小。128 * 2 = 256字节
        (void * const)            NULL,                 //参数4：可以是任何的指针类型, 需外部传入一个变量地址                          [补充]：传递给任务函数的参数指针。可以传递任何类型的数据，任务函数会将其转换为正确的类型使用。如果不需要传递参数，可以传递 NULL -> (void * const) NULL
        (UBaseType_t)             1,                    //参数5：unsigned long类型 - UBaseType_t类型, 任务的优先级                     [补充]：数值越大，优先级越高
        (TaskHandle_t *)          &xStartTask_Handler   //参数6：指向 tskTaskControlBlock 结构体的指针类型 - TaskHandle_t *类型（任务句柄类型的指针）,需外部传入一个任务句柄变量的地址          [补充]：创建任务后，任务句柄会存储在此指针指向的变量中。如果不需要任务句柄，可以传递 NULL -> (TaskHandle_t *) NULL
    );
    printf(">>>result:%ld\n", Result);
        
    /* 开启任务调度器 */
    vTaskStartScheduler();

    while(1)
    {   
        
    }
}
