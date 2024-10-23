#ifndef __LINK_LIST_H
#define __LINK_LIST_H


typedef struct  
{
 	unsigned char dataLen;			//���ݳ���
	unsigned char *pData;		    //����ָ��     
}DataType;


typedef struct SListNode 
{
	DataType data; // ֵ 
	struct SListNode *next; // ָ����һ����� 
}SListNode;


// ��ʼ�� ,����һ���յ�����
SListNode ** SListInit(void);
// β������ 
void SListPushBack(SListNode** ppFirst, DataType data);
// ͷ������ 
void SListPushFront(SListNode **ppFirst, DataType data);
// β��ɾ�� 
void SListPopBack(SListNode **ppFirst);
// ͷ��ɾ�� 
void SListPopFront(SListNode **ppFirst);


#endif /* __LINK_LIST_H */