#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
//#include "stdafx.h"
/*
#define ERRORQueue -1
typedef int ElementTypeQueue;
//数组 循环队列(仅使用Size-1个数组空间)
typedef struct QNodeA
{
	ElementTypeQueue *Data;
	int Front;//列头元素的前一个元素的下标(便于删除元素时 返回元素操作)
	int Rear;//列尾元素位置的下标
	int MAX;//元素个数
}*QueueA;
const int LENQA = sizeof(QNodeA);
*/

class Queue {
public:
	typedef int LinkedListQueueElementType;
	~Queue() {}
	//入队(提议)
	void offer(LinkedListQueueElementType &job) {
		AddLinkedListQueue(&baseImpl, job);
	}
	//出队(投票)
	LinkedListQueueElementType poll() {
		return PopLinkedListQueue(&baseImpl);
	}
	bool isEmpty() {
		return IsEmptyLinkedListQueue(&baseImpl);
	}
protected:
	struct LinkedListQueueNode {
		LinkedListQueueElementType Data;
		struct LinkedListQueueNode *Next;
	};
	//链表实现
	struct LinkedListQueue {
		//头节点指针
		struct LinkedListQueueNode* Front;
		//尾结点指针
		struct LinkedListQueueNode* Rear;
	};
	typedef struct LinkedListQueue *PtrLinkedListQueue;
	PtrLinkedListQueue CreateLinkedListQueue() {
		PtrLinkedListQueue pq = (PtrLinkedListQueue)malloc(sizeof(struct LinkedListQueue));
		pq->Front = pq->Rear = NULL;
		return pq;
	}
	bool IsEmptyLinkedListQueue(PtrLinkedListQueue PtrQ) {
		return PtrQ->Front == NULL;
	}
	void AddLinkedListQueue(PtrLinkedListQueue PtrQ, LinkedListQueueElementType job) {
		struct LinkedListQueueNode *New = (struct LinkedListQueueNode*)malloc(sizeof(struct LinkedListQueueNode));
		memset(New, 0, sizeof(struct LinkedListQueueNode));
		New->Data = job;
		if (IsEmptyLinkedListQueue(PtrQ))
			PtrQ->Front = New;
		else
			PtrQ->Rear->Next = New;
		PtrQ->Rear = New;
	}
	LinkedListQueueElementType PopLinkedListQueue(PtrLinkedListQueue PtrQ) {
		struct LinkedListQueueNode *FrontCell = NULL;
		LinkedListQueueElementType FrontItem;
		assert(!IsEmptyLinkedListQueue(PtrQ));
		FrontCell = PtrQ->Front;
		//只有一个元素
		if (PtrQ->Front == PtrQ->Rear)
			PtrQ->Front = PtrQ->Rear = NULL;
		else
			PtrQ->Front = PtrQ->Front->Next;
		FrontItem = FrontCell->Data;
		free(FrontCell);
		FrontCell = NULL;
		return FrontItem;
	}
private:
	LinkedListQueue baseImpl;
};

#endif
