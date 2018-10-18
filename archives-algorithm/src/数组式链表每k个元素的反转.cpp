//#include "Tree.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#define ERRORTree -1
#define ERRORQueue NULL
#define ERRORStack NULL
int Max(int a, int b)
{
	return a > b ? a : b;
}
typedef int ElementTypeList;
typedef struct ListNode *List;
struct ListNode
{
	ElementTypeList Data;
	int Next;
};
const int LEN_TREE_ST = sizeof(struct ListNode);
List CreatList_ST(int MAX_size);
List T = CreatList_ST(100005);
/************************************数组式链表底层链栈*/
typedef int ElementTypeStack;
typedef struct Stack *PStack;
struct Stack
{
	ElementTypeStack Data;
	PStack Next;
};
//创建一个连在PN之前的链栈结点 返回栈头
PStack ConnectP(PStack PN)
{
	PStack Ps = (PStack)malloc(sizeof(Stack));
	Ps->Next = PN;
	return Ps;
}
//创建并返回头式链栈的地址
PStack CreateStack()
{
	return ConnectP(NULL);
}
//压入
void Push(PStack Head, ElementTypeStack Item)
{
	Head->Next = ConnectP(Head->Next);
	Head->Next->Data = Item;
}
//判断链栈是否为空
bool IsSemptySt(PStack Head)
{
	return Head->Next == NULL;
}
//返回栈顶
ElementTypeStack pop(PStack Head)
{
	ElementTypeStack Item;
	PStack SNext = Head->Next;
	if (IsSemptySt(Head))
	{
		puts("堆栈空");
		return ERRORStack;
	}
	else
	{
		Item = Head->Next->Data;
		Head->Next = Head->Next->Next;
		free(SNext);
		SNext = NULL;
		return Item;
	}
}
//创建并初始化二叉树
List CreatList_ST(int MAX_size)
{
	List BT = (List)malloc(LEN_TREE_ST*MAX_size);
	memset(BT, -1, LEN_TREE_ST*MAX_size);
	return BT;
}
/*将以root为根的数组链表的前k个元素逆转 subk被改变第k个元素的下标 返回新根节点下标*/
int K_ReveList(int k, int *subk, int root, PStack Sta/*基本元素是下标int型*/)
{
	int sub = 0;
	if (root == -1)
		return root;
	for (sub = root; k-- >= 0; sub = T[sub].Next){
		*subk = sub;
		if (sub == -1 && k >= 0)/*不足k个且不是最后一个*/
			return root;
	}
	for (sub = root; sub != *subk; sub = T[sub].Next){
		Push(Sta, sub);
		root = sub;
	}
	for (sub = root; !IsSemptySt(Sta); sub = T[sub].Next)
	{
		T[sub].Next = pop(Sta);
	}
	T[sub].Next = *subk;/*保证当前的连通*/
	*subk = sub;
	return root;
}
int main()
{
	int n, k;
	int root, sub, subk;
	PStack sta = CreateStack();
	while (~scanf("%d%d%d\n", &root, &n, &k)){
		while (n-- > 0){
			scanf("%d", &sub);
			scanf("%d%d", &T[sub].Data, &T[sub].Next);
		}
		root = K_ReveList(k, &subk, root, sta);
		while (subk != -1 && T[subk].Next != -1){
			sub = subk;
			T[sub].Next = K_ReveList(k, &subk, T[subk].Next, sta);
		}
		for (sub = root; sub != -1; sub = T[sub].Next){
			if (T[sub].Next == -1)
				break;
			printf("%05d %d %05d\n", sub, T[sub].Data, T[sub].Next);
		}printf("%05d %d %d\n", sub, T[sub].Data, T[sub].Next);
	}
	return 0;
}
/*
最大N,最后剩K-1不反转
00002 6 2
00000 2 00004
00001 4 00005
00002 1 00000
00003 6 -1
00004 3 00001
00005 5 00003

*/