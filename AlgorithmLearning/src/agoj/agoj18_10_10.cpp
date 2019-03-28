#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#define EPS 1E-5
typedef struct PolyNode *Pnode;
//double版
struct PolyNode
{
	//系数   头必须置0
	double coef;
	//指数  头是项数
	double expon;
	Pnode next;
};
typedef Pnode List;
const int BT_NODE_LEN = sizeof(struct PolyNode);
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
		puts("ERROR");
}
//销毁单链表 表头P置NULL
void Destroy(List P)
{
	List Ptemp = P->next;
	P->next = NULL;
	while (Ptemp)
	{
		P = Ptemp->next;
		free(Ptemp);
		Ptemp = P;
	}
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
//先读指数 再读系数
List ReadPoly(int termsCount)
{
	List Head = NULL, Pleft = NULL, Pright = NULL;
	Pleft = Head = (List)malloc(BT_NODE_LEN);
	memset(Head, 0, BT_NODE_LEN);
	while (termsCount-- > 0)
	{
		Pright = (List)malloc(BT_NODE_LEN);
		scanf("%lf%lf", &Pright->expon, &Pright->coef);
		Pleft->next = Pright;
		Pleft = Pright;
		Pright = Pright->next;
	}
	Pleft->next = NULL;
	return Head;
}
List Add(List L1, List L2)//参数Dest 用于判断是否销毁源链表
{
	List Head = (List)malloc(BT_NODE_LEN), P = Head;
	List p1 = L1->next, p2 = L2->next;
	int ISAdd = 1;
	memset(Head, 0, sizeof(BT_NODE_LEN));
	while (p1 && p2)//指数：不等存大挪大 相等存和都挪
	{
		if (ISAdd)
		{
			P->next = (List)malloc(BT_NODE_LEN);
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
	List SumHead = (List)malloc(BT_NODE_LEN);
	List P1 = L1->next, P2 = L2->next;
	List Head = (List)malloc(BT_NODE_LEN), P = NULL;//中间链表
	memset(SumHead, 0, BT_NODE_LEN);//保证结果链表合法
	memset(Head, 0, BT_NODE_LEN);
	while (P1)//拆L1
	{
		P = Head;
		P2 = L2->next;
		while (P2)//分项乘P2 结果存到SumHead
		{
			if (P1->coef && P2->coef)
			{
				P->next = (List)malloc(BT_NODE_LEN);
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
			//cout << P->coef << " " << P->expon;
			printf("%f %f", P->coef, P->expon);
		}
	}
	else
	{
		printf("0 0");
	}
	puts("");
}

//判断double值是否为0
int IsDoubleZero(double num) {
	return fabs(num) < EPS;
}
//判断double值保留1位的情况下是否为0 若为0返回0
int judgeDoubleTainOne(double num) {
	//return num >= 0.05 ? 1 : 0;
	return num > 0.05 || IsDoubleZero(num - 0.05) == 1 ? 1 : 0;
	//num>=0.05保留一位后为0.1不为0 反之系数视为0->返回0
}
//返回链表储存数据结点个数+1
int Length(List L) {/*此实现基于含头节点的链表*/
	List p = L;
	int n = 0;
	while (p)
		++n, p = p->next;
	return n;
}
//仅释放当前结点 并将节点地址改为下一个结点的地址 没有 则NULL
void DestroyNode(List *P)
{
	List Ptemp = NULL;
	if ((*P)->next)
		Ptemp = (*P)->next;
	free(*P);
	*P = Ptemp;
}
//剔除保留一位小数后为0的项 返回执行后的链表长度
int TainOne(List PHead) {
	//int length = 0;
	List P = PHead;
	while (P && P->next) {
		if (judgeDoubleTainOne(P->next->coef) == 1) {
			//do nothing
			//++length;
		}
		else {
			DestroyNode(&P->next);
		}
		P = P->next;
	}
	return Length(PHead->next);
}

//输出符合条件的项(系数保留一位后不为0的)
void PrintPolyOnereTain(List P){
	int n = 0;
	int length = TainOne(P);
	if (P->next && length != 0){
		printf("%d ", length);
		while (P->next){
			P = P->next;
			if (n++ != 0)
				printf(" ");
			printf("%.f %.1f", P->expon, P->coef);
		}
	}
	else{
		printf("0 0 0.0");
	}
	puts("");
}
int main()
{
	//freopen("input", "r", stdin);
	List P1, P2, PP, PS;
	int termsCount = -1;
	while (1 == scanf("%d", &termsCount)) {
		P1 = ReadPoly(termsCount);
		scanf("%d", &termsCount);
		P2 = ReadPoly(termsCount);
		//PP = Muit(P1, P2);
		//PrintPoly(PP);
		PS = Add(P1, P2);
		PrintPolyOnereTain(PS);

		Destroy(P1);
		Destroy(P2);
		Destroy(PS);
	}
	return 0;
}

/*
input:
2
1 2.4 0 3.2
2
2 1.5 1 0.5

ouput: 3 2 1.5 1 2.9 0 3.2

2
1 2.4 0 3.2
2
2 0.05 1 0.5

1 1 0.03
1 1 0.01

0 0 0.0

1 1 0.03
2 1 0.01 0 0.05
*/