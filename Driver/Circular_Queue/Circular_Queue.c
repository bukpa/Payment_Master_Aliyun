#include "Circular_Queue.h"


QueueBuffer_Type QueueBuffer;


QueueBuffer_Type * Queue_Register(uint16_t mSize)
{
	QueueBuffer_Type *pQueueBuffer;
    
	pQueueBuffer = (QueueBuffer_Type *)malloc(sizeof(QueueBuffer_Type));    //分配记录队列信息的结构变量
    
	memset((uint8_t *)pQueueBuffer, 0, sizeof(QueueBuffer_Type));                //记录队列信息的结构变量清0
    
	pQueueBuffer->BufferStart = pQueueBuffer->pIn = pQueueBuffer->pOut = malloc( mSize ); //分配队列所需内存
    
	pQueueBuffer->BufferEnd = pQueueBuffer->BufferStart + mSize;    //队列结束指针
    
	return pQueueBuffer;
}



//将数据压入队列
void Queue_Push(QueueBuffer_Type * pQueueBuffer, uint8_t mData)
{
	uint8_t *p = NULL;
    
	p = pQueueBuffer->pIn;
    
	*p++ = mData; //mData 给了pQueueBuffer->pIn并且加1，mcount++
    
	if (p == (pQueueBuffer->BufferEnd + 1))
	{
	   p = pQueueBuffer->BufferStart;
	}
    
	pQueueBuffer->pIn = p;
    
	pQueueBuffer->mCount++;
}



//将数据弹出队列
unsigned char Queue_Pop(QueueBuffer_Type *pQueueBuffer)
{
	unsigned char mData;
	unsigned char *p;
    
	p = pQueueBuffer->pOut;//把pout的送出去，pout加1，并且mcount--
    
	mData = *p;
	
	if(++p == (pQueueBuffer->BufferEnd + 1))
	{
	   p = pQueueBuffer->BufferStart;
	}
	
	pQueueBuffer->pOut = p;
    
	pQueueBuffer->mCount--;
    
	return mData;
}



//读出队列指定序号数据
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



//返回队列数据个数
uint16_t Queue_Num(QueueBuffer_Type *pQueueBuffer )
{
	return pQueueBuffer->mCount;
}



//队列清空
void Queue_Clear(QueueBuffer_Type *pQueueBuffer )
{

	pQueueBuffer->pIn = pQueueBuffer->pOut = pQueueBuffer->BufferStart;
    
	pQueueBuffer->mCount = 0;

}


uint8_t Queue_Full(QueueBuffer_Type *pQueueBuffer, uint16_t MAX_Entry)
{
    return (pQueueBuffer->mCount == MAX_Entry);
}


