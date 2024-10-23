#include "Link_List.h"
#include <stdlib.h>
#include <string.h>
#include "TLSF.h"
#include <stdio.h>



#define Link_List_Malloc            tlsf_malloc 
#define Link_List_Free              tlsf_free

#define Link_List_Printf            printf



//初始化
SListNode ** SListInit(void)
{
    SListNode **ppFirst;
    
	ppFirst = (SListNode**)Link_List_Malloc(sizeof(SListNode*));
    
    *ppFirst = NULL;
    
    return ppFirst;
}



//申请新节点
static SListNode* CreateNode(DataType stdata)
{
	SListNode*node = (SListNode*)Link_List_Malloc(sizeof(SListNode));
    
	if( node != NULL)
	{   
	    //为结构体内的pData数据指针申请内存空间和进行数据存储，使用长度复制，不使用字符串长度复制
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



// 尾部插入 
void SListPushBack(SListNode** ppFirst, DataType stdata)
{
    SListNode *node = CreateNode(stdata);
	
	if (*ppFirst == NULL)           //空链表      
	{	
		*ppFirst = node;
        
		return;
	}
	else                            //非空链表，寻找链表中的最后一个节点      
	{  
        SListNode* cur = *ppFirst;
        
        while (cur->next != NULL)
        {
        	cur = cur->next;
        }
        
        cur->next = node;//插入新申请的节点
    }
}



//头部插入
void SListPushFront(SListNode **ppFirst, DataType stdata)
{
	SListNode *node = CreateNode(stdata);
    
	node->next = *ppFirst;
    
	*ppFirst = node;
}



// 尾部删除 
void SListPopBack(SListNode **ppFirst)
{
	
	if(*ppFirst == NULL)            //空链表退出
	{
        return;  
	}
	else                            //非空链表，进行删除操作
	{   
	    SListNode*cur = *ppFirst;  
	            
        while (cur->next != NULL)       //寻找链表中的最后一个节点
        {
            cur = cur->next;
        }
        
        Link_List_Free(cur->data.pData);
        
        Link_List_Free(cur->next);
        
        cur->next = NULL;

        if ((*ppFirst)->next == NULL)   //最后一个节点，清空链表
        {
            *ppFirst = NULL;
        }
	}
}



// 头部删除 
void SListPopFront(SListNode **ppFirst)
{
	if(*ppFirst == NULL)            //空链表退出
	{
        return;  
	}
	else                            //非空链表，进行删除操作
	{ 
    	SListNode *first = *ppFirst;
        
    	*ppFirst = (*ppFirst)->next;
        
        Link_List_Free(first->data.pData);
        
    	Link_List_Free(first);
    }
}



