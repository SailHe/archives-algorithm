/*****************************************************队列*************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

typedef int ElementTypeQueue;
/*****数组 循环队列(仅使用MAX_QUEUE_SIZE-1个数组空间)*****/
typedef struct QueueArray{
	ElementTypeQueue *Data;
	int Front;//列头元素的前一个元素的下标(便于删除元素时 返回元素操作)
	int Rear;//列尾元素位置的下标
	int MAX;//元素个数
} *PtrToQueueArray;
PtrToQueueArray CreateQA(int Size)
{
	PtrToQueueArray pq = (PtrToQueueArray)malloc(sizeof(struct QueueArray));
	pq->Data = (ElementTypeQueue*)malloc(Size*sizeof(ElementTypeQueue));
	pq->Front = pq->Rear = 0;
	pq->MAX = Size;
	return pq;
}
int EmptyQA(PtrToQueueArray p)
{
	return p->Rear == p->Front;
}
void EnqueueQA(PtrToQueueArray q, ElementTypeQueue job)
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
ElementTypeQueue DequeueQA(PtrToQueueArray q)
{
	assert(!EmptyQA(q));
	q->Front++;
	q->Front %= q->MAX;
	return q->Data[q->Front];
}



/*****************************************************双端队列**********************************************************/
typedef int ElementType;
typedef int DequeArraySub;
typedef struct DequeArray *PtrToDequeArray;
struct DequeArray {
	ElementType *Data;      /* 存储元素的数组   */
	DequeArraySub Front, Rear;   /* 队列的头、尾指针 */
	int MaxSize;            /* 队列最大容量     */
};
typedef PtrToDequeArray Deque;

Deque CreateDeque(int MaxSize)
{   /* 注意：为区分空队列和满队列，需要多开辟一个空间 */
	Deque D = (Deque)malloc(sizeof(struct DequeArray));
	MaxSize++;
	D->Data = (ElementType *)malloc(MaxSize * sizeof(ElementType));
	D->Front = D->Rear = 0;
	D->MaxSize = MaxSize;
	return D;
}

bool Push(ElementType X, Deque D);
ElementType Pop(Deque D);
bool Inject(ElementType X, Deque D);
ElementType Eject(Deque D);

/**********************实现1: Front是实际元素下标   Rear是实际元素下标+1 73-30=43行******************************/
int IsEmptyDeque(Deque p)
{
	return p->Rear == p->Front;
}
int IsFullDeque(Deque d) {
	return (d->Rear + 1) % d->MaxSize == d->Front;
}
//Push(X, D)：将元素X插入到双端队列D的头；
bool Push(ElementType job, Deque q) {
	if (IsFullDeque(q))
		return false;
	else {
		q->Front = q->Front - 1 + q->MaxSize;
		q->Front %= q->MaxSize;
		q->Data[q->Front] = job;
		return true;
	}
}
//Pop(D)：删除双端队列D的头元素，并返回；
ElementType Pop(Deque q) {
	assert(!IsEmptyDeque(q));
	ElementType x = q->Data[q->Front];
	q->Front = (q->Front + 1) % q->MaxSize;
	return x;
}
//Inject(X, D)：将元素X插入到双端队列D的尾部；
bool Inject(ElementType job, Deque q) {
	if (IsFullDeque(q))
		return false;
	else {
		q->Data[q->Rear] = job;
		q->Rear = (q->Rear + 1) % q->MaxSize;
		return true;
	}
}
//Eject(D)：删除双端队列D的尾部元素，并返回。
ElementType Eject(Deque q) {
	assert(!IsEmptyDeque(q));
	q->Rear = q->Rear - 1 + q->MaxSize;
	q->Rear %= q->MaxSize;
	return q->Data[q->Rear];
}

/**********************实现2: Front是实际元素下标-1 Rear是实际元素下标 119-74=45行********************************/
int IsEmptyDeque(Deque p)
{
	return p->Rear == p->Front;
}
int IsFullDeque(Deque d) {
	return (d->Rear + 1) % d->MaxSize == d->Front;
}
//Push(X, D)：将元素X插入到双端队列D的头；
bool Push(ElementType job, Deque q) {
	if (IsFullDeque(q)) {
		return false;
	}
	else {
		q->Data[q->Front] = job;
		q->Front = q->Front - 1 + q->MaxSize;
		q->Front %= q->MaxSize;
		return true;
	}
}
//Pop(D)：删除双端队列D的头元素，并返回；
ElementType Pop(Deque q) {
	assert(!IsEmptyDeque(q));
	q->Front = (q->Front + 1) % q->MaxSize;
	return q->Data[q->Front];
}
//Inject(X, D)：将元素X插入到双端队列D的尾部；
bool Inject(ElementType job, Deque q) {
	if (IsFullDeque(q)) {
		return false;
	}
	else {
		q->Rear = (q->Rear + 1) % q->MaxSize;
		q->Data[q->Rear] = job;
		return true;
	}
}
//Eject(D)：删除双端队列D的尾部元素，并返回。
ElementType Eject(Deque q) {
	assert(!IsEmptyDeque(q));
	int index = q->Rear;
	q->Rear = q->Rear - 1 + q->MaxSize;
	q->Rear %= q->MaxSize;
	return q->Data[index];
}
