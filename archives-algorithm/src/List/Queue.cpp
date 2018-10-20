/*****************************************************队列*************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SIZEQueue 4
#define ERRORQueue -1
typedef int ElementTypeQueue;
/*****数组 循环队列(仅使用SIZEQueue-1个数组空间) size版不用*****/
typedef struct QNodeA
{
	ElementTypeQueue *Data;// [SIZEQueue];
	int Front;//列头元素的前一个元素的下标(便于删除元素时 返回元素操作)
	int Rear;//列尾元素位置的下标
	int MAX;//元素个数
}*QueueA;
QueueA createQA(int Size)
{
	QueueA pq = (QueueA)malloc(sizeof(struct QNodeA));
	pq->Data = (ElementTypeQueue*)malloc(Size*sizeof(ElementTypeQueue));
	pq->Front = pq->Rear = 0;
	pq->MAX = Size;
	return pq;
}
int emptyDq(QueueA p)
{
	return p->Rear == p->Front;
}
void addQA(QueueA q, ElementTypeQueue job)
{
	if ((q->Rear + 1) % q->MAX == q->Front)
	{
		puts("队列满");
		return;
	}
	else
	{
		q->Rear++;
		q->Rear %= q->MAX;
		q->Data[q->Rear] = job;
	}
}
ElementTypeQueue popQA(QueueA q)
{
	if (q->Front == q->Rear){
		puts("队列空");
		return ERRORQueue;
	}
	else{
		q->Front++;
		q->Front %= q->MAX;
		return q->Data[q->Front];
	}
}

