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
template<class T>
class Queue {
public:
	typedef T LinkedListQueueElementType;
	Queue() {
		baseImpl = CreateLinkedListQueue();
	}
	~Queue() {
		DestroyLinkedListQueue(baseImpl);
	}
	//入队(提议)
	void offer(LinkedListQueueElementType job) {
		EnqueueLinkedListQueue(baseImpl, job);
	}
	//出队(投票)
	LinkedListQueueElementType poll() {
		return DequeueLinkedListQueue(baseImpl);
	}
	bool isEmpty() {
		return IsEmptyLinkedListQueue(baseImpl);
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
		PtrLinkedListQueue pq = new LinkedListQueue();
		pq->Front = pq->Rear = NULL;
		return pq;
	}
	void DestroyLinkedListQueue(PtrLinkedListQueue q) {
		//assert(false);
		q = nullptr;
	}
	bool IsEmptyLinkedListQueue(PtrLinkedListQueue PtrQ) {
		return PtrQ->Front == NULL;
	}
	void EnqueueLinkedListQueue(PtrLinkedListQueue PtrQ, LinkedListQueueElementType &job) {
		struct LinkedListQueueNode *New = new LinkedListQueueNode();
		New->Data = job;
		if (IsEmptyLinkedListQueue(PtrQ))
			PtrQ->Front = New;
		else
			PtrQ->Rear->Next = New;
		PtrQ->Rear = New;
	}
	LinkedListQueueElementType DequeueLinkedListQueue(PtrLinkedListQueue PtrQ) {
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
		delete(FrontCell);
		FrontCell = NULL;
		return FrontItem;
	}
private:
	PtrLinkedListQueue baseImpl;
};

template<class T>
class Deque {
public:
	typedef T ElementType;
	Deque() {
		baseImpl = CreateDeque();
	}
	
	~Deque() {
		DestroyDeque(baseImpl);
	}
	//入队(提议)
	void offer(ElementType job) {
		Push(baseImpl, job);
	}
	//出队(投票)
	ElementType poll() {
		return Eject(baseImpl);
	}
	bool isEmpty() {
		return IsEmptyDeQueue(baseImpl);
	}
	void shell() {
		ShellForDeQueue();
	}
protected:
	//DeQueue 离队
	typedef enum { OP_PUSH_DEQUEUE, OP_POP_DEQUEUE, OP_INJECT_DEQUEUE, OP_EJECT_DEQUEUE, OP_END_DEQUEUE } OperationDequeue;
	struct DequeLinkedNode {
		ElementType Element;
		DequeLinkedNode *Next, *Last;
	};
	typedef struct DequeLinkedNode *PtrToDqNode;
	struct DequeImpl {
		PtrToDqNode Front, Rear;
	};
	//双端队列
	typedef struct DequeImpl *PtrToDequeImpl;
	/*
	PtrToDequeImpl CreateDeque();
	int Push(ElementType X, PtrToDequeImpl D);
	ElementType Pop(PtrToDequeImpl D);
	int Inject(ElementType X, PtrToDequeImpl D);
	ElementType Eject(PtrToDequeImpl D);
	int IsEmptyDeQueue(PtrToDequeImpl dQ);
	OperationDequeue GetOp();
	void PrintDeque(PtrToDequeImpl D);
	*/

	int ShellForDeQueue(){
		ElementType X;
		PtrToDequeImpl D;
		int done = 0;

		D = CreateDeque();
		puts("输入Push, Pop, Inject, Eject + 值");
		while (done == 0) {
			switch (GetOp()) {
			case OP_PUSH_DEQUEUE:
				std::cin >> X;
				if (Push(D, X) == 0) {
					printf("Memory is Full!\n");
				}
				break;
			case OP_POP_DEQUEUE:
				if (IsEmptyDeQueue(D)) {
					printf("Deque is Empty!\n");
				}
				else {
					X = Pop(D);
					std::cout << X << std::endl;
				}
				break;
			case OP_INJECT_DEQUEUE:
				std::cin >> X;
				if (Inject(D, X) == 0) {
					printf("Memory is Full!\n");
				}
				break;
			case OP_EJECT_DEQUEUE:
				if (IsEmptyDeQueue(D)) {
					printf("Deque is Empty!\n");
				}
				else {
					X = Eject(D);
					std::cout << X << std::endl;
				}
				break;
			case OP_END_DEQUEUE:
				PrintDeque(D);
				done = 1;
				break;
			}
		}
		return 0;
	}

	int IsEmptyDeQueue(PtrToDequeImpl dQ) {
		return dQ->Front == dQ->Rear;
	}
	//Init Dequeue (front始终不变)
	PtrToDequeImpl CreateDeque() {
		PtrToDequeImpl pq = new DequeImpl();
		pq->Front = new DequeLinkedNode();
		pq->Rear = pq->Front;
		return pq;
	}
	void DestroyDeque(PtrToDequeImpl &dQ) {
		//assert(false);
		dQ = nullptr;
	}
	//Push(X,D): Insert item X on the front end of deque D.
	int Push(PtrToDequeImpl dQ, ElementType &job) {
		struct DequeLinkedNode* front = dQ->Front;
		struct DequeLinkedNode *newNode = new DequeLinkedNode();
		newNode->Element = job;
		//内存满
		if (newNode == NULL)
			return 0;
		else {
			//只有一个元素
			if (front->Next == NULL)
				dQ->Rear = newNode;
			else
				front->Next->Last = newNode;
			newNode->Next = front->Next;
			newNode->Last = front;
			front->Next = newNode;
		}
		return 1;
	}
	//Pop(D) : Remove the front item from deque D and return it.
	ElementType Pop(PtrToDequeImpl dQ) {
		ElementType FrontItem;
		assert(!IsEmptyDeQueue(dQ));
		FrontItem = dQ->Front->Next->Element;
		struct DequeLinkedNode* de = dQ->Front->Next;
		if (dQ->Front->Next == dQ->Rear)//只有一个元素
			dQ->Rear = dQ->Front;
		else
			de->Next->Last = dQ->Front;
		dQ->Front->Next = de->Next;
		delete(de); de = NULL;
		return FrontItem;
	}
	//Insert item X on the rear end of deque D.
	int Inject(PtrToDequeImpl dQ, ElementType &job) {
		struct DequeLinkedNode* front = dQ->Front;
		struct DequeLinkedNode *newNode = new DequeLinkedNode();
		newNode->Element = job;
		if (newNode == NULL)//内存满
			return 0;
		else {
			newNode->Next = NULL;
			newNode->Last = dQ->Rear;
			dQ->Rear->Next = newNode;
			dQ->Rear = newNode;
		}
		return 1;
	}
	//Remove the rear item from deque D and return it.
	ElementType Eject(PtrToDequeImpl dQ) {
		ElementType FrontItem;
		assert(!IsEmptyDeQueue(dQ));
		FrontItem = dQ->Rear->Element;
		struct DequeLinkedNode* de = dQ->Rear;
		dQ->Rear = dQ->Rear->Last;
		dQ->Rear->Next = NULL;
		delete(de); de = NULL;
		return FrontItem;
	}
	void PrintDeque(PtrToDequeImpl D) {
		while (!IsEmptyDeQueue(D)) {
			std::cout << Pop(D) << std::endl;
		}
		puts("");
	}
	OperationDequeue GetOp() {
		char s[8];
		scanf("%s", s, 8);
		if (strcmp(s, "Push") == 0) {
			return OP_PUSH_DEQUEUE;
		}
		else if (strcmp(s, "Pop") == 0) {
			return OP_POP_DEQUEUE;
		}
		else if (strcmp(s, "End") == 0) {
			return OP_END_DEQUEUE;
		}
		else if (strcmp(s, "Inject") == 0) {
			return OP_INJECT_DEQUEUE;
		}
		else if (strcmp(s, "Eject") == 0) {
			return OP_EJECT_DEQUEUE;
		}
		else {
			puts("invalid!");
		}
		return OP_END_DEQUEUE;
	}
private:
	PtrToDequeImpl baseImpl;
};

#endif
