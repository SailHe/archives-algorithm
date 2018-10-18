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
/************************************����ʽ����ײ���ջ*/
typedef int ElementTypeStack;
typedef struct Stack *PStack;
struct Stack
{
	ElementTypeStack Data;
	PStack Next;
};
//����һ������PN֮ǰ����ջ��� ����ջͷ
PStack ConnectP(PStack PN)
{
	PStack Ps = (PStack)malloc(sizeof(Stack));
	Ps->Next = PN;
	return Ps;
}
//����������ͷʽ��ջ�ĵ�ַ
PStack CreateStack()
{
	return ConnectP(NULL);
}
//ѹ��
void Push(PStack Head, ElementTypeStack Item)
{
	Head->Next = ConnectP(Head->Next);
	Head->Next->Data = Item;
}
//�ж���ջ�Ƿ�Ϊ��
bool IsSemptySt(PStack Head)
{
	return Head->Next == NULL;
}
//����ջ��
ElementTypeStack pop(PStack Head)
{
	ElementTypeStack Item;
	PStack SNext = Head->Next;
	if (IsSemptySt(Head))
	{
		puts("��ջ��");
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
//��������ʼ��������
List CreatList_ST(int MAX_size)
{
	List BT = (List)malloc(LEN_TREE_ST*MAX_size);
	memset(BT, -1, LEN_TREE_ST*MAX_size);
	return BT;
}
/*����rootΪ�������������ǰk��Ԫ����ת subk���ı��k��Ԫ�ص��±� �����¸��ڵ��±�*/
int K_ReveList(int k, int *subk, int root, PStack Sta/*����Ԫ�����±�int��*/)
{
	int sub = 0;
	if (root == -1)
		return root;
	for (sub = root; k-- >= 0; sub = T[sub].Next){
		*subk = sub;
		if (sub == -1 && k >= 0)/*����k���Ҳ������һ��*/
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
	T[sub].Next = *subk;/*��֤��ǰ����ͨ*/
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
���N,���ʣK-1����ת
00002 6 2
00000 2 00004
00001 4 00005
00002 1 00000
00003 6 -1
00004 3 00001
00005 5 00003

*/