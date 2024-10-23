#ifndef __LINK_LIST_H
#define __LINK_LIST_H


typedef struct  
{
 	unsigned char dataLen;			//数据长度
	unsigned char *pData;		    //数据指针     
}DataType;


typedef struct SListNode 
{
	DataType data; // 值 
	struct SListNode *next; // 指向下一个结点 
}SListNode;


// 初始化 ,构造一条空的链表
SListNode ** SListInit(void);
// 尾部插入 
void SListPushBack(SListNode** ppFirst, DataType data);
// 头部插入 
void SListPushFront(SListNode **ppFirst, DataType data);
// 尾部删除 
void SListPopBack(SListNode **ppFirst);
// 头部删除 
void SListPopFront(SListNode **ppFirst);


#endif /* __LINK_LIST_H */