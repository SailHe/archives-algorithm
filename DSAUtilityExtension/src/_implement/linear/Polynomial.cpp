/*********************************************多项式处理*************************************************/
/*
										这里的参数P一般是大写
*/
/*********************************************多项式处理*************************************************/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define scanf scanf_s
typedef struct PolyNode *Pnode;
struct PolyNode
{
	int coef;//系数   头必须置0
	int expon;//指数  头是项数
	Pnode next;
};
typedef Pnode List;
const int LEN = sizeof(struct PolyNode);
//仅释放当前结点 并将节点地址改为下一个结点的地址 没有 返回NULL
void DestroyIt(List *P)
{
	List Ptemp = NULL;
	if ((*P)->next)
		Ptemp = (*P)->next;
	free(*P);
	*P = Ptemp;
}
/*****************应该用这个较安全的函数替换上面一个有BUG的**********************/
//释放结点P-next P->next = P->next->next
void DestroyNext(List P) {
	List Ptemp = NULL;
	if (P->next) {
		//if (P->next->next)
		Ptemp = P->next->next;
		free(P->next);
		P->next = Ptemp;
	}
	else
		puts("ERROR_PTR");
}
//一元多项式求导
void derivation(List p) {
	while (p) {/*因为有头节点 可以直接操作p->next*/
		p->next->coef *= p->next->expon--;
		if (p->next->coef == 0)
			DestroyNext(p);
		p = p->next;
	}
}
// 先读系数 再读指数
List ReadPoly()
{
	List Head = NULL, Pleft = NULL, Pright = NULL;
	Pleft = Head = (List)malloc(LEN);
	memset(Head, 0, LEN);
	scanf("%d", &Head->expon);
	while (Head->expon-- > 0)
	{
		Pright = (List)malloc(LEN);
		scanf("%d%d", &Pright->coef, &Pright->expon);
		Pleft->next = Pright;
		Pleft = Pright;
		Pright = Pright->next;
	}
	Pleft->next = NULL;
	return Head;
}
List Add(List L1, List L2)//参数Dest 用于判断是否销毁源链表
{
	List Head = (List)malloc(LEN), P = Head;
	List p1 = L1->next, p2 = L2->next;
	int ISAdd = 1;
	memset(Head, 0, sizeof(LEN));
	while (p1 && p2)//指数：不等存大挪大 相等存和都挪
	{
		if (ISAdd)
		{
			P->next = (List)malloc(LEN);
			ISAdd = 0;
		}
		if (p1->expon == p2->expon)/*合并同类项 二之一*/
		{
			if (p1->coef + p2->coef)
			{
				P->next->coef = p1->coef + p2->coef;
				P->next->expon = p1->expon;
				ISAdd = 1;
			}
			//p1 = p1->next;
			//p2 = p2->next;
			DestroyIt(&p1);
			DestroyIt(&p2);
		}
		else if (p1->expon > p2->expon)
		{
			if (p1->coef)
			{
				P->next->coef = p1->coef;
				P->next->expon = p1->expon;
				ISAdd = 1;
			}
			//p1 = p1->next;
			DestroyIt(&p1);
		}
		else
		{
			if (p2->coef)
			{
				P->next->coef = p2->coef;
				P->next->expon = p2->expon;
				ISAdd = 1;
			}
			//p2 = p2->next;
			DestroyIt(&p2);
		}
		ISAdd ? P = P->next : 0;
	}
	P->next = p1 ? p1 : p2;//兼连结 置NULL一体
	L1->next = L2->next = NULL;
	return Head;
}
List Muit(List L1, List L2)
{
	List SumHead = (List)malloc(LEN);
	List P1 = L1->next, P2 = L2->next;
	List Head = (List)malloc(LEN), P = NULL;//中间链表
	memset(SumHead, 0, LEN);//保证结果链表合法
	memset(Head, 0, LEN);
	while (P1)//拆L1
	{
		P = Head;
		P2 = L2->next;
		while (P2)//分项乘P2 结果存到SumHead
		{
			if (P1->coef && P2->coef)
			{
				P->next = (List)malloc(LEN);
				P->next->coef = P1->coef * P2->coef;
				P->next->expon = P1->expon + P2->expon;
				P = P->next;
			}
			P2 = P2->next;
		}
		P->next = NULL;
		SumHead = Add(SumHead, Head);
		P1 = P1->next;
	}
	return SumHead;
}
void PrintPoly(List P)
{
	int n = 0;
	if (P->next)
	{
		while (P->next)
		{
			if (n++ != 0)
				printf(" ");
			P = P->next;
			printf("%d %d", P->coef, P->expon);
		}
	}
	else
	{
		printf("0 0");
	}
	puts("");
}
int main___99()
{
	//freopen("input", "r", stdin);
	List P1, P2, PP, PS;
	P1 = ReadPoly();
	P2 = ReadPoly();
	PP = Muit(P1, P2);
	PrintPoly(PP);
	PS = Add(P1, P2);
	PrintPoly(PS);
	return 0;
}
/*
输入样例:

4
3 4  -5 2  6 1  -2 0
3
5 20 -7 4  3 1

输出样例:

15 24 -25 22 30 21 -10 20 -21 8 35 6 -33 5 14 4 -15 3 18 2 -6 1
5 20 -4 4 -5 2 9 1 -2 0
*/

