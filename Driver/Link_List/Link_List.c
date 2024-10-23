#include "Link_List.h"
#include <stdlib.h>
#include <string.h>
#include "TLSF.h"
#include <stdio.h>



#define Link_List_Malloc            tlsf_malloc 
#define Link_List_Free              tlsf_free

#define Link_List_Printf            printf



//��ʼ��
SListNode ** SListInit(void)
{
    SListNode **ppFirst;
    
	ppFirst = (SListNode**)Link_List_Malloc(sizeof(SListNode*));
    
    *ppFirst = NULL;
    
    return ppFirst;
}



//�����½ڵ�
static SListNode* CreateNode(DataType stdata)
{
	SListNode*node = (SListNode*)Link_List_Malloc(sizeof(SListNode));
    
	if( node != NULL)
	{   
	    //Ϊ�ṹ���ڵ�pData����ָ�������ڴ�ռ�ͽ������ݴ洢��ʹ�ó��ȸ��ƣ���ʹ���ַ������ȸ���
     	unsigned char *pTempData = Link_List_Malloc(stdata.dataLen);
        
        if(pTempData == NULL)
        {
            printf("########## Link_List_Malloc failed! ##########\r\n");
        }
        
        memcpy((char *)pTempData, (char *)stdata.pData, stdata.dataLen);
        
        node->data.dataLen = stdata.dataLen;
        
        node->data.pData = pTempData;
        
    	node->next = NULL;
	}
    
	return node;
}



// β������ 
void SListPushBack(SListNode** ppFirst, DataType stdata)
{
    SListNode *node = CreateNode(stdata);
	
	if (*ppFirst == NULL)           //������      
	{	
		*ppFirst = node;
        
		return;
	}
	else                            //�ǿ�����Ѱ�������е����һ���ڵ�      
	{  
        SListNode* cur = *ppFirst;
        
        while (cur->next != NULL)
        {
        	cur = cur->next;
        }
        
        cur->next = node;//����������Ľڵ�
    }
}



//ͷ������
void SListPushFront(SListNode **ppFirst, DataType stdata)
{
	SListNode *node = CreateNode(stdata);
    
	node->next = *ppFirst;
    
	*ppFirst = node;
}



// β��ɾ�� 
void SListPopBack(SListNode **ppFirst)
{
	
	if(*ppFirst == NULL)            //�������˳�
	{
        return;  
	}
	else                            //�ǿ���������ɾ������
	{   
	    SListNode*cur = *ppFirst;  
	            
        while (cur->next != NULL)       //Ѱ�������е����һ���ڵ�
        {
            cur = cur->next;
        }
        
        Link_List_Free(cur->data.pData);
        
        Link_List_Free(cur->next);
        
        cur->next = NULL;

        if ((*ppFirst)->next == NULL)   //���һ���ڵ㣬�������
        {
            *ppFirst = NULL;
        }
	}
}



// ͷ��ɾ�� 
void SListPopFront(SListNode **ppFirst)
{
	if(*ppFirst == NULL)            //�������˳�
	{
        return;  
	}
	else                            //�ǿ���������ɾ������
	{ 
    	SListNode *first = *ppFirst;
        
    	*ppFirst = (*ppFirst)->next;
        
        Link_List_Free(first->data.pData);
        
    	Link_List_Free(first);
    }
}



