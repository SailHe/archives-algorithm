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
//���� ѭ������(��ʹ��Size-1������ռ�)
typedef struct QNodeA
{
	ElementTypeQueue *Data;
	int Front;//��ͷԪ�ص�ǰһ��Ԫ�ص��±�(����ɾ��Ԫ��ʱ ����Ԫ�ز���)
	int Rear;//��βԪ��λ�õ��±�
	int MAX;//Ԫ�ظ���
}*QueueA;
const int LENQA = sizeof(QNodeA);
*/

class Queue {
public:
	typedef int LinkedListQueueElementType;
	~Queue() {}
	//���(����)
	void offer(LinkedListQueueElementType &job) {
		AddLinkedListQueue(&baseImpl, job);
	}
	//����(ͶƱ)
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
	//����ʵ��
	struct LinkedListQueue {
		//ͷ�ڵ�ָ��
		struct LinkedListQueueNode* Front;
		//β���ָ��
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
		//ֻ��һ��Ԫ��
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
