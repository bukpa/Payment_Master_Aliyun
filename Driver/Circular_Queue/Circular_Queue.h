#ifndef __CIRCULAR_QUEUE_H
#define __CIRCULAR_QUEUE_H

#include "main.h"



typedef struct 
{
	uint8_t *BufferStart;  //��������ʼ��
	uint8_t *BufferEnd;    //������������
	uint8_t *pIn;          //дָ��(��βָ��tail)
	uint8_t *pOut;         //��ָ��(����ָ��head)
	uint16_t mCount;       //���������ݸ���	
}QueueBuffer_Type;


typedef struct 
{
  QueueBuffer_Type *pReceiveDataBuffer;
  QueueBuffer_Type *pSendDataBuffer;

}USARTBuffer_Type;


extern QueueBuffer_Type QueueBuffer;
extern QueueBuffer_Type * Queue_Register( uint16_t mSize );
extern void Queue_Push(QueueBuffer_Type  *pQueueBuffer, uint8_t mData );
extern uint8_t Queue_Pop(QueueBuffer_Type  *pQueueBuffer );
extern uint8_t Queue_Read(QueueBuffer_Type *pQueueBuffer, uint8_t mId );
extern uint16_t Queue_Num(QueueBuffer_Type *pQueueBuffer );
extern void Queue_Clear(QueueBuffer_Type *pQueueBuffer );
extern uint8_t Queue_Full(QueueBuffer_Type *pQueueBuffer, uint16_t MAX_Entry);



#endif /* __CIRCULAR_QUEUE_H */