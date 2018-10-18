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
/*****数组 循环队列(仅使用Size-1个数组空间)*****/
typedef struct QNodeA
{
	ElementTypeQueue *Data;
	int Front;//列头元素的前一个元素的下标(便于删除元素时 返回元素操作)
	int Rear;//列尾元素位置的下标
	int MAX;//元素个数
}*QueueA;
const int LENQA = sizeof(QNodeA);
/********链表实现*********/
struct LNodeQ
{
	ElementTypeQueue Data;
	LNodeQ *Next;
};
typedef struct QNodeL
{
	LNodeQ* Front;//头节点
	LNodeQ* Rear;//尾结点
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