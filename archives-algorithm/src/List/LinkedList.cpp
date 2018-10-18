/****************************************含头节点的**链表基本操作*******************************************/
/*
		不带数据的结点称做 头节点 以下函数皆默认参数链表带有头节点 当然也可以使用头节点创建函数自行创建头节点
*/
/****************************************含头节点的**链表基本操作*******************************************/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define ERROR -1//ERROR应定义为ElementType类型的一个特殊值
#define MIN -2147483647//ERROR应定义为ElementType类型的最小值
#define MAX 2147483647//ERROR应定义为ElementType类型的最小值
typedef int ElementType;
typedef struct Node *Pnode;
struct Node
{
	int Data;
	Pnode next;
};
typedef Pnode List;
typedef Node * Position;//特指其地址 位置之意
const int LEN = sizeof(struct Node);
/********************************************链表的创建与打印*************/
//为任意链表创建一个初始化值为0的 头节点
List CreatHead(List L)
{
	List Head = (List)malloc(LEN);
	memset(Head, 0, LEN);
	Head->next = L;
	return Head;
}
//顺序建表
//旧版
List Read()
{
	List plast = NULL, pnow = NULL, head = (List)malloc(LEN);
	memset(head, 0, LEN);
	while (1)
	{
		pnow = (List)malloc(LEN);
		scanf("%d", &pnow->Data);
		if (pnow->Data == -1)
		{
			free(pnow);
			pnow = NULL;
			break;
		}
		if (plast == NULL)
			head->next = pnow;
		else
			plast->next = pnow;
		plast = pnow;
	}
	if (plast)
		plast->next = NULL;
	return head;
}
//相比旧版 少5行更简洁 一个变量一个判断 更快
List Createlist()
{
	List Head = (List)malloc(LEN), P = Head;
	memset(Head, 0, LEN);
	while (1)
	{
		P->next = (List)malloc(LEN);
		scanf("%d", &P->next->Data);
		if (P->next->Data == -1)
		{
			free(P->next);
			P->next = NULL;
			break;
		}
		P = P->next;
	}
	return Head/*->next   **以下同**若不需要头节点加上这句即可*/;
}
//逆序建表 此法建表头节点是结束标记所带数据
List RCreatelist()
{
	List plast = NULL, pnow = NULL;
	while (1)
	{
		pnow = (List)malloc(LEN);
		scanf("%d", &pnow->Data);
		pnow->next = plast;
		plast = pnow;
		if (pnow->Data == -1)
			break;
	}
	return pnow;
}
void Print(List L)
{
	int n = 0;
	if (L->next)
	{
		while (L = L->next)
		{
			printf(n++ ? " " : "");
			printf("%d", L->Data);
		}
	}
	else
		printf("NULL");
	puts("");
}
/***********************************************递增链表操作**************/
/*
结点不变 原表头置零
*/
/***********************************************递增链表操作************下**/
//返回L1 L2原序合并后的链表头节点
List Merge(List L1, List L2){
	List head = (List)malloc(sizeof(List)), p = head;
	List p1 = L1->next, p2 = L2->next;
	L1->next = L2->next = NULL;
	while (p1 && p2){
		if (p1->Data <= p2->Data){
			p->next = p1;
			p1 = p1->next;
		}
		else{
			p->next = p2;
			p2 = p2->next;
		}
		p = p->next;
	}
	p->next = p1 ? p1 : p2;
	return head;
}

struct ListNode * newNode(int data){
	List Head = (List)malloc(LEN);
	memset(Head, 0, LEN);
	Head->data = data;
	return Head;
}
//list1和list2是两个按data升序链接的链表的头指针；(头指针不是头节点)
//函数mergelists将两个链表合并成一个按data升序链接的新链表，并返回结果链表的头指针。
struct ListNode* mergelists(struct ListNode* L1, struct ListNode* L2){
	struct ListNode* head = newNode(0), *p = head;
	struct ListNode* p1 = L1, *p2 = L2;
	while (p1 && p2){
		if (p1->data <= p2->data){
			p->next = newNode(p1->data);
			p1 = p1->next;
		}
		else{
			p->next = newNode(p2->data);
			p2 = p2->next;
		}
		p = p->next;
	}
	p->next = p1 ? p1 : p2;
	return head->next;
}

//返回L1 与L2的交集的头节点
List intersection(List L1, List L2)
{
	List head = (List)malloc(LEN), p = head;
	List p1 = L1->next, p2 = L2->next;
	L1->next = L2->next = NULL;
	memset(head, 0, LEN);
	while (p1 && p2)
	{
		if (p1->Data == p2->Data)
		{
			p->next = p1;
			p1 = p1->next;
			p2 = p2->next;
			p = p->next;
		}
		else if (p1->Data < p2->Data)
		{
			p1 = p1->next;
		}
		else
		{
			p2 = p2->next;
		}
	}
	p->next = NULL;
	return head;
}
/*******************************************链表的查找与数据操作********上**/
/*
原链表完全不变
*/
/*******************************************链表的查找与数据操作*********/
//返回链表储存数据结点个数+1
int Length(List L){/*此实现基于含头节点的链表*/
	List p = L;
	int n = 0;
	while (p)
		++n, p = p->next;
	return n;
}
//返回链表第k条数据  不存在返回ERROR :: k属于(0, length)
ElementType FindKth(List L, int k) {
	int n = 1;
	while (L = L->next) {
		if (n < k)
			++n;
		else if (n == k)
			return L->Data;
		else//k <= 0
			break;
	}
	return ERROR;
}
//返回链表倒数第k个元素  不存在返回ERROR
ElementType FindRKth(List L, int k) {
	//倒数第k个就是正数第len-k个
	return FindKth(L, Length(L) - k);//若不带头节点length返回值加1
}
//返回线性表中首次出现X的位置 找不到返回NULL
Position FindData(List L, ElementType X)
{
	List p = L;
	p->Data = -X;
	while (p)
	{
		if (p->Data == X)
		{
			return p;
		}
		p = p->next;
	}
	return NULL;
}
//将X插入L，并保持该序列的有序性，返回插入后的链表头
List InsertData(List L, ElementType X)
{
	List pn = NULL, pin = (List)malloc(LEN), ptemp = NULL;
	pin->Data = X; pin->next = NULL;
	pn = L;
	while (pn->next)
	{
		if (X < pn->next->Data)
			break;/*保持递增序*/
		else
			pn = pn->next;
	}
	ptemp = pn->next;
	pn->next = pin;
	pin->next = ptemp;
	return L;
}
//在位置P前插入X  返回链表头 若参数P位置非法返回NULL   自带虚拟头节点 即使传入空结点 NLLL 也可以顺利插入
List InsertInP(List L, ElementType X, Position P)
{
	List pn = NULL, pin = (List)malloc(LEN), head = (List)malloc(LEN);
	pn = head;
	pn->Data = -X; pn->next = L;
	pin->Data = X; pin->next = NULL;
	while (pn)
	{
		if (pn->next == P)
		{
			pn->next = pin;
			pin->next = P;
			return head->next;
		}
		else
			pn = pn->next;
	}
	return NULL;
}
//删除位置P的元素 返回链表头 若参数P位置非法返回NULL
List DeleteInP(List L, Position P)
{
	List pn = L;
	pn->Data = 0;
	while (pn->next && P)
	{
		if (pn->next == P)
		{
			P = P->next;//暂用
			free(pn->next);
			pn->next = P;
			return L;
		}
		else
			pn = pn->next;
	}
	return NULL;
}
//删除链表 所有特定数据
List Deletelist(List head, int data)
{
	List plast = NULL, Head = (List)malloc(LEN), p = Head;
	Head->next = head;
	while (p->next)
	{
		plast = p;
		p = p->next;
		if (p->Data < data)/*删除条件 此处是小于*/
		{
			plast->next = p->next;//连结p两端
			p == head ? head = plast->next : 0;//当即将删除的结点是head时head要更新
			free(p);//释放p处的内存
			p = plast;//因为要保证p所指内容的正确性
		}
	}
	return head;
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
//链表逆置  原链表不变 完全新建 返回新建头节点
List Reverse(List Head)
{
	List plast = NULL, pnow = NULL;
	Head = Head->next;/*传入的不是头节点去掉这句*/
	while (Head)
	{
		pnow = (List)malloc(LEN);
		pnow->Data = Head->Data;
		pnow->next = plast;
		plast = pnow;
		Head = Head->next;
	}
	pnow = CreatHead(pnow);
	return pnow;
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
//返回一个与参数结点相同的节点 del决定是copy还是cut
List Clone(List P, bool del)
{
	if (P)
	{
		List CloneP = (List)malloc(LEN);
		memcpy(CloneP, P, LEN);
		if (del)
			DestroyNode(&P);
	}
	else
		return NULL;
}
/*******************************************************************************************************************【DEBUG】********/
struct stud_node * Reverse(struct stud_node * L)
{
	Position Old_head, New_head, Temp;
	New_head = NULL;
	Old_head = L->next;

	while (Old_head)
	{
		Temp = Old_head->next;
		Old_head->next = New_head;
		New_head = Old_head;
		Old_head = Temp;
	}
	L = New_head;
	return L;
}/***************/
int main()
{
	List p1 = NULL, p2 = NULL, p = NULL;
	List pt = NULL;
	int n;
	p1 = Createlist();
	p2 = RCreatelist();
	Print(p1); Print(p2);
	puts("begain\n");

	p = Reverse(p2);
	Destroy(p2);
	Print(p2 = p);

	p = Merge(p1, p2); Print(p);
	n = Length(p);
	int i = 1;
	while (p->next)
	{
		ElementType temp;
		temp = FindRKth(p, 1);
		if (temp != -1)
		{
			printf("find 倒数第%d个元素%d\n", 1, temp);
			printf("find 正数第%d个元素%d\n", i++, FindKth(p, i));
			pt = InsertInP(pt, temp, pt);
		}
		else
			puts("find error");
		if (DeleteInP(p, FindData(p, temp)))
		{
			printf("delete %d\n", temp);
			Print(p);
		}
		else
			puts("delete error");
	}
	puts("");
	pt = CreatHead(pt);
	Print(pt);
	p = Reverse(pt);
	Destroy(pt);
	Print(pt = p);

	return 0;
}
/*
1 3 5 7 9 11 -1
2 4 6 8 10 12 -1

*/

/*********************************************多项式处理*************************************************/
/*
										这里的参数P一般是大写
*/
/*********************************************多项式处理*************************************************/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
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
void DestroyNext(List P){
	List Ptemp = NULL;
	if (P->next){
		//if (P->next->next)
		Ptemp = P->next->next;
		free(P->next);
		P->next = Ptemp;
	}
	else
		puts("ERROR");
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
int main()
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
