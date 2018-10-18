#include <stdio.h>
#include <stdlib.h>

#define ERROR -1
typedef int ElementType;
typedef enum { push, pop, inject, eject, end } Operation;
typedef int Position;
typedef struct QNode *PtrToQNode;
struct QNode {
	ElementType *Data;      /* 存储元素的数组   */
	Position Front, Rear;   /* 队列的头、尾指针 */
	int MaxSize;            /* 队列最大容量     */
};
typedef PtrToQNode Deque;

Deque CreateDeque(int MaxSize)
{   /* 注意：为区分空队列和满队列，需要多开辟一个空间 */
	Deque D = (Deque)malloc(sizeof(struct QNode));
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

Operation GetOp();          /* 裁判实现，细节不表 */
void PrintDeque(Deque D); /* 裁判实现，细节不表 */
/**********************Front是实际元素下标   Rear是实际元素下标+1******************************/
int emptyDq(Deque p)
{
	return p->Rear == p->Front;
}
int fullDq(Deque d){
	return (d->Rear + 1) % d->MaxSize == d->Front;
}
//Push(X, D)：将元素X插入到双端队列D的头；
bool Push(ElementType job, Deque q){
	if (fullDq(q))
		return false;
	else{
		q->Front = q->Front - 1 + q->MaxSize;
		q->Front %= q->MaxSize;
		q->Data[q->Front] = job;
		return true;
	}
}
//Pop(D)：删除双端队列D的头元素，并返回；
ElementType Pop(Deque q){
	if (emptyDq(q))
		return ERROR;
	else{
		ElementType x = q->Data[q->Front];
		q->Front = (q->Front + 1) % q->MaxSize;
		return x;
	}
}
//Inject(X, D)：将元素X插入到双端队列D的尾部；
bool Inject(ElementType job, Deque q){
	if (fullDq(q))
		return false;
	else{
		q->Data[q->Rear] = job;
		q->Rear = (q->Rear + 1) % q->MaxSize;
		return true;
	}
}
//Eject(D)：删除双端队列D的尾部元素，并返回。
ElementType Eject(Deque q){
	if (emptyDq(q))
		return ERROR;
	else{
		q->Rear = q->Rear - 1 + q->MaxSize;
		q->Rear %= q->MaxSize;
		return q->Data[q->Rear];
	}
}
/**********************Front是实际元素下标-1 Rear是实际元素下标********************************/
int emptyDq(Deque p)
{
	return p->Rear == p->Front;
}
int fullDq(Deque d){
	return (d->Rear + 1) % d->MaxSize == d->Front;
}
//Push(X, D)：将元素X插入到双端队列D的头；
bool Push(ElementType job, Deque q){
	if (fullDq(q)){
		//puts("Deque is Full!");
		return false;
	}
	else{
		q->Data[q->Front] = job;
		q->Front = q->Front - 1 + q->MaxSize;
		q->Front %= q->MaxSize;
		return true;
	}
}
//Pop(D)：删除双端队列D的头元素，并返回；
ElementType Pop(Deque q){
	if (emptyDq(q)){
		//puts("Deque is Empty!");
		return ERROR;
	}
	else{
		q->Front = (q->Front + 1) % q->MaxSize;
		return q->Data[q->Front];
	}
}
//Inject(X, D)：将元素X插入到双端队列D的尾部；
bool Inject(ElementType job, Deque q){
	if (fullDq(q)){
		//puts("Deque is Full!");
		return false;
	}
	else{
		q->Rear = (q->Rear + 1) % q->MaxSize;
		q->Data[q->Rear] = job;
		return true;
	}
}
//Eject(D)：删除双端队列D的尾部元素，并返回。
ElementType Eject(Deque q){
	if (emptyDq(q)){
		//puts("Deque is Empty!");
		return ERROR;
	}
	else{
		int index = q->Rear;
		q->Rear = q->Rear - 1 + q->MaxSize;
		q->Rear %= q->MaxSize;
		return q->Data[index];
	}
}

#include<string.h>
void PrintDeque(Deque D){
	while (D->Front != D->Rear){
		printf("%d ", Pop(D));
	}
	puts("");
}
Operation GetOp(){
	char s[8];
	scanf("%s", s);
	if (strcmp(s, "Push") == 0){
		return push;
	}
	else if (strcmp(s, "Pop") == 0){
		return pop;
	}
	else if (strcmp(s, "End") == 0){
		return end;
	}
	if (strcmp(s, "Inject") == 0){
		return inject;
	}
	if (strcmp(s, "Eject") == 0){
		return eject;
	}
	return end;
}
//freopen("input", "r", stdin);

int main()
{
	freopen("input", "r", stdin);
	ElementType X;
	Deque D;
	int N, done = 0;

	scanf("%d", &N);
	D = CreateDeque(N);
	for (X = 1; X <= 2 * N; ++X){
		//if (!Push(X, D))printf("Deque is Full!\n");printf("%d\n", Eject(D));
	}
	while (!done) {
		switch (GetOp()) {
		case push:
			scanf("%d", &X);
			if (!Push(X, D)) printf("Deque is Full!\n");
			break;
		case pop:
			X = Pop(D);
			if (X == ERROR) printf("Deque is Empty!\n");
			else printf("%d is out\n", X);
			break;
		case inject:
			scanf("%d", &X);
			if (!Inject(X, D)) printf("Deque is Full!\n");
			break;
		case eject:
			X = Eject(D);
			if (X == ERROR) printf("Deque is Empty!\n");
			else printf("%d is out\n", X);
			break;
		case end:
			PrintDeque(D);
			done = 1;
			break;
		}
	}
	return 0;
}