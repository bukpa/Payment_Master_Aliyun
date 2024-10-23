#include "Circular_Queue.h"


QueueBuffer_Type QueueBuffer;


QueueBuffer_Type * Queue_Register(uint16_t mSize)
{
	QueueBuffer_Type *pQueueBuffer;
    
	pQueueBuffer = (QueueBuffer_Type *)malloc(sizeof(QueueBuffer_Type));    //�����¼������Ϣ�Ľṹ����
    
	memset((uint8_t *)pQueueBuffer, 0, sizeof(QueueBuffer_Type));                //��¼������Ϣ�Ľṹ������0
    
	pQueueBuffer->BufferStart = pQueueBuffer->pIn = pQueueBuffer->pOut = malloc( mSize ); //������������ڴ�
    
	pQueueBuffer->BufferEnd = pQueueBuffer->BufferStart + mSize;    //���н���ָ��
    
	return pQueueBuffer;
}



//������ѹ�����
void Queue_Push(QueueBuffer_Type * pQueueBuffer, uint8_t mData)
{
	uint8_t *p = NULL;
    
	p = pQueueBuffer->pIn;
    
	*p++ = mData; //mData ����pQueueBuffer->pIn���Ҽ�1��mcount++
    
	if (p == (pQueueBuffer->BufferEnd + 1))
	{
	   p = pQueueBuffer->BufferStart;
	}
    
	pQueueBuffer->pIn = p;
    
	pQueueBuffer->mCount++;
}



//�����ݵ�������
unsigned char Queue_Pop(QueueBuffer_Type *pQueueBuffer)
{
	unsigned char mData;
	unsigned char *p;
    
	p = pQueueBuffer->pOut;//��pout���ͳ�ȥ��pout��1������mcount--
    
	mData = *p;
	
	if(++p == (pQueueBuffer->BufferEnd + 1))
	{
	   p = pQueueBuffer->BufferStart;
	}
	
	pQueueBuffer->pOut = p;
    
	pQueueBuffer->mCount--;
    
	return mData;
}



//��������ָ���������
unsigned char Queue_Read(QueueBuffer_Type *pQueueBuffer, unsigned char mId )
{
	unsigned char *pTemp;
    
	pTemp = pQueueBuffer->pOut + mId;
    
	if( pTemp < pQueueBuffer->BufferEnd )
	{
       return(*pTemp);
	}
	else
	{
       return(*(pTemp - pQueueBuffer->BufferEnd + pQueueBuffer->BufferStart));
	} 
}



//���ض������ݸ���
uint16_t Queue_Num(QueueBuffer_Type *pQueueBuffer )
{
	return pQueueBuffer->mCount;
}



//�������
void Queue_Clear(QueueBuffer_Type *pQueueBuffer )
{

	pQueueBuffer->pIn = pQueueBuffer->pOut = pQueueBuffer->BufferStart;
    
	pQueueBuffer->mCount = 0;

}


uint8_t Queue_Full(QueueBuffer_Type *pQueueBuffer, uint16_t MAX_Entry)
{
    return (pQueueBuffer->mCount == MAX_Entry);
}


