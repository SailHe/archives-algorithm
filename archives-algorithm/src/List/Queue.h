#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define ERRORQueue -1
typedef int ElementTypeQueue;
/*****���� ѭ������(��ʹ��Size-1������ռ�)*****/
typedef struct QNodeA
{
	ElementTypeQueue *Data;
	int Front;//��ͷԪ�ص�ǰһ��Ԫ�ص��±�(����ɾ��Ԫ��ʱ ����Ԫ�ز���)
	int Rear;//��βԪ��λ�õ��±�
	int MAX;//Ԫ�ظ���
}*QueueA;
const int LENQA = sizeof(QNodeA);
/********����ʵ��*********/
struct LNodeQ
{
	ElementTypeQueue Data;
	LNodeQ *Next;
};
typedef struct QNodeL
{
	LNodeQ* Front;//ͷ�ڵ�
	LNodeQ* Rear;//β���
}*QueueL;
const int LENQL = sizeof(QNodeL);
QueueA CreateQA(int Size);
bool ISemptyA(QueueA p);
void AddQA(QueueA q, ElementTypeQueue job);
ElementTypeQueue DeleteQA(QueueA q);

QueueL CreateQL();
bool ISemptyL(QueueL PtrQ);
void AddQL(QueueL PtrQ, ElementTypeQueue job);
ElementTypeQueue DeleteQL(QueueL PtrQ);

#endif