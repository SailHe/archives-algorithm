#include <stdio.h>
#include <stdlib.h>

#define ElementType int
#define ERROR 1e5
typedef enum { push, pop, inject, eject, end } Operation;

typedef struct Node *PtrToNode;
struct Node {
	ElementType Element;
	PtrToNode Next, Last;
};
typedef struct DequeRecord *Deque;
struct DequeRecord {
	PtrToNode Front, Rear;
};
Deque CreateDeque();
int Push(ElementType X, Deque D);
ElementType Pop(Deque D);
int Inject(ElementType X, Deque D);
ElementType Eject(Deque D);

Operation GetOp();          /* details omitted */
void PrintDeque(Deque D); /* details omitted */

int main()
{
	ElementType X;
	Deque D;
	int done = 0;

	D = CreateDeque();
	while (!done) {
		switch (GetOp()) {
		case push:
			scanf("%d", &X);
			if (!Push(X, D)) printf("Memory is Full!\n");
			break;
		case pop:
			X = Pop(D);
			if (X == ERROR) printf("Deque is Empty!\n");
			break;
		case inject:
			scanf("%d", &X);
			if (!Inject(X, D)) printf("Memory is Full!\n");
			break;
		case eject:
			X = Eject(D);
			if (X == ERROR) printf("Deque is Empty!\n");
			break;
		case end:
			PrintDeque(D);
			done = 1;
			break;
		}
	}
	return 0;
}

/* Your function will be put here */


#include<malloc.h>
#include<string.h>
int empty(Deque dQ){
	return dQ->Front == dQ->Rear;
}
//front始终不变
Deque CreateDeque(){
	Deque pq = (Deque)malloc(sizeof(struct DequeRecord));
	pq->Front = (struct Node*)malloc(sizeof(struct Node));
	memset(pq->Front, 0, sizeof(struct Node));
	pq->Rear = pq->Front;
	return pq;
}
//Push(X,D): Insert item X on the front end of deque D.
int Push(ElementType job, Deque dQ){
	struct Node* front = dQ->Front;
	struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->Element = job;
	if (newNode == NULL)//内存满
		return 0;
	else{
		if (front->Next == NULL)//只有一个元素
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
ElementType Pop(Deque dQ){
	ElementType FrontItem = ERROR;
	if (empty(dQ))
		return ERROR;
	else{
		FrontItem = dQ->Front->Next->Element;
		struct Node* de = dQ->Front->Next;
		if (dQ->Front->Next == dQ->Rear)//只有一个元素
			dQ->Rear = dQ->Front;
		else
			de->Next->Last = dQ->Front;
		dQ->Front->Next = de->Next;
		free(de); de = NULL;
	}
	return FrontItem;
}
//Insert item X on the rear end of deque D.
int Inject(ElementType job, Deque dQ){
	struct Node* front = dQ->Front;
	struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->Element = job;
	if (newNode == NULL)//内存满
		return 0;
	else{
		newNode->Next = NULL;
		newNode->Last = dQ->Rear;
		dQ->Rear->Next = newNode;
		dQ->Rear = newNode;
	}
	return 1;
}
//Remove the rear item from deque D and return it.
ElementType Eject(Deque dQ){
	ElementType FrontItem = ERROR;
	if (!empty(dQ)){
		FrontItem = dQ->Rear->Element;
		struct Node* de = dQ->Rear;
		dQ->Rear = dQ->Rear->Last;
		dQ->Rear->Next = NULL;
		free(de); de = NULL;
	}
	return FrontItem;
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
