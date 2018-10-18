/****************************************含头节点的**链表基本操作*******************************************/
/*
		不带数据的结点称做 头节点 以下函数皆默认参数链表带有头节点 当然也可以使用头节点创建函数自行创建头节点
*/
/****************************************含头节点的**链表基本操作*******************************************/
//#include <stdio.h>
//#include <string.h>
//#include <malloc.h>
#include "../LinkedList.h"
#define ERROR_ELE_VALUE LinkedList::ERROR_ELE_VALUE
typedef LinkedList::ElementType ElementType;
typedef struct LinkedList::Node LinkedListNode;
typedef LinkedListNode *LinkedListImpl;
//特指其位置 用地址表示
typedef LinkedListNode *Position;
//const int LEN = sizeof(struct LinkedList::Node);
const int LEN = sizeof(LinkedListNode);
/********************************************链表的创建与打印*************/
//为任意链表创建一个初始化值为0的 头节点
LinkedListImpl CreatHead(LinkedListImpl L)
{
	LinkedListImpl Head = (LinkedListImpl)malloc(LEN);
	memset(Head, 0, LEN);
	Head->next = L;
	return Head;
}
//顺序建表
//旧版
LinkedListImpl CreateListOld()
{
	LinkedListImpl plast = NULL, pnow = NULL, head = (LinkedListImpl)malloc(LEN);
	memset(head, 0, LEN);
	while (1)
	{
		pnow = (LinkedListImpl)malloc(LEN);
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
LinkedListImpl CreateListNew()
{
	LinkedListImpl Head = (LinkedListImpl)malloc(LEN), P = Head;
	memset(Head, 0, LEN);
	while (1)
	{
		P->next = (LinkedListImpl)malloc(LEN);
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
LinkedListImpl CreateListRe()
{
	LinkedListImpl plast = NULL, pnow = NULL;
	while (1)
	{
		pnow = (LinkedListImpl)malloc(LEN);
		scanf("%d", &pnow->Data);
		pnow->next = plast;
		plast = pnow;
		if (pnow->Data == -1)
			break;
	}
	return pnow;
}
void ReadList(LinkedListImpl *L) {
	*L = CreateListNew();
}
void Print(LinkedListImpl L)
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
LinkedListImpl Merge(LinkedListImpl L1, LinkedListImpl L2){
	LinkedListImpl head = (LinkedListImpl)malloc(sizeof(LinkedListImpl)), p = head;
	LinkedListImpl p1 = L1->next, p2 = L2->next;
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

LinkedListImpl newNode(ElementType Data){
	LinkedListImpl Head = (LinkedListImpl)malloc(LEN);
	memset(Head, 0, LEN);
	Head->Data = Data;
	return Head;
}
//list1和list2是两个按Data升序链接的链表的头指针；(头指针不是头节点)
//函数Mergelists将两个链表合并成一个按Data升序链接的新链表，并返回结果链表的头指针。
LinkedListNode* Mergelists(LinkedListImpl L1, LinkedListImpl L2){
	LinkedListImpl head = newNode(0), p = head;
	//LinkedListNode *p1 = L1, *p2 = L2;
	LinkedListImpl p1 = L1, p2 = L2;
	while (p1 && p2){
		if (p1->Data <= p2->Data){
			p->next = newNode(p1->Data);
			p1 = p1->next;
		}
		else{
			p->next = newNode(p2->Data);
			p2 = p2->next;
		}
		p = p->next;
	}
	p->next = p1 ? p1 : p2;
	return head->next;
}

//返回L1 与L2的交集的头节点
LinkedListImpl Intersection(LinkedListImpl L1, LinkedListImpl L2)
{
	LinkedListImpl head = (LinkedListImpl)malloc(LEN), p = head;
	LinkedListImpl p1 = L1->next, p2 = L2->next;
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
int Length(LinkedListImpl L){/*此实现基于含头节点的链表*/
	LinkedListImpl p = L;
	int n = 0;
	while (p)
		++n, p = p->next;
	return n;
}
//返回链表第k条数据  不存在返回NULL :: k属于(0, length)
Position FindKth(LinkedListImpl L, int k) {
	int n = 1;
	while (L = L->next) {
		if (n < k)
			++n;
		else if (n == k)
			return L;
		else//k <= 0
			break;
	}
	return NULL;
}
//返回链表倒数第k个元素  不存在返回ERROR_ELE_VALUE
Position FindRKth(LinkedListImpl L, int k) {
	//倒数第k个就是正数第len-k个
	return FindKth(L, Length(L) - k);//若不带头节点length返回值加1
}
//返回线性表中首次出现X的位置 找不到返回NULL
Position FindData(LinkedListImpl L, ElementType X){
	LinkedListImpl p = L;
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
LinkedListImpl InsertData(LinkedListImpl L, ElementType X)
{
	LinkedListImpl pn = NULL, pin = (LinkedListImpl)malloc(LEN), ptemp = NULL;
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
LinkedListImpl InsertInP(LinkedListImpl L, ElementType X, Position P)
{
	LinkedListImpl pn = NULL, pin = (LinkedListImpl)malloc(LEN), head = (LinkedListImpl)malloc(LEN);
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
LinkedListImpl DeleteInP(LinkedListImpl L, Position *DelPosition){
	LinkedListImpl pn = L;
	Position P = *DelPosition;
	pn->Data = 0;
	while (pn->next && P){
		if (pn->next == P){
			P = P->next;//暂用
			free(pn->next);
			pn->next = P;
			*DelPosition = NULL;
			return L;
		}
		else {
			pn = pn->next;
		}
	}
	return NULL;
}
//删除链表 所有特定数据(此处是小于)
LinkedListImpl DeleteAllData(LinkedListImpl head, ElementType Data)
{
	LinkedListImpl plast = NULL, Head = (LinkedListImpl)malloc(LEN), p = Head;
	Head->next = head;
	while (p->next)
	{
		plast = p;
		p = p->next;
		if (p->Data < Data)/*删除条件 此处是小于*/
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
void Destroy(LinkedListImpl P)
{
	LinkedListImpl Ptemp = P->next;
	P->next = NULL;
	while (Ptemp)
	{
		P = Ptemp->next;
		free(Ptemp);
		Ptemp = P;
	}
}
//链表逆置  原链表不变 完全新建 返回新建头节点
LinkedListImpl Reverse(LinkedListImpl Head)
{
	LinkedListImpl plast = NULL, pnow = NULL;
	Head = Head->next;/*传入的不是头节点去掉这句*/
	while (Head)
	{
		pnow = (LinkedListImpl)malloc(LEN);
		pnow->Data = Head->Data;
		pnow->next = plast;
		plast = pnow;
		Head = Head->next;
	}
	pnow = CreatHead(pnow);
	return pnow;
}
LinkedListImpl Reverse_DEBUG(LinkedListImpl L)
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
//仅释放当前结点 并将节点地址改为下一个结点的地址 没有 则NULL
void DestroyNode(LinkedListImpl *P)
{
	LinkedListImpl Ptemp = NULL;
	if ((*P)->next)
		Ptemp = (*P)->next;
	free(*P);
	*P = Ptemp;
}
//返回一个与参数结点相同的节点 del决定是copy还是cut
LinkedListImpl Clone(LinkedListImpl P, bool del)
{
	if (P)
	{
		LinkedListImpl CloneP = (LinkedListImpl)malloc(LEN);
		memcpy(CloneP, P, LEN);
		if (del) {
			DestroyNode(&P);
		}
		return CloneP;
	}
	else {
		return NULL;
	}
		
}

int mainForTest(){
	//freopen("input", "r", stdin);
	LinkedListImpl p1 = NULL, p2 = NULL, p = NULL;
	LinkedListImpl pt = NULL;
	int n;
	puts("输入链表各元素 以-1结尾");
	p1 = CreateListNew();
	p2 = CreateListRe();
	Print(p1); Print(p2);
	puts("begain\n");

	puts("以反转后的p2作为新的链表p\n");
	p = Reverse(p2);

	puts("销毁p2\n");
	Destroy(p2);
	puts("输出p并将p赋给p2\n");
	Print(p2 = p);

	puts("p = 合并p1 p2后的新链表\n");
	p = Merge(p1, p2); Print(p);
	n = Length(p);
	int i = 0;
	while (p->next){
		++i;
		
		Position findPosition = FindKth(p, i);
		if (findPosition != NULL) {
			pt = InsertInP(pt, findPosition->Data, pt);
			printf("find 正数第%d个元素%d\n", i, findPosition->Data);
		}
		else {
			puts("find error");
		}
		
		Position findPositionRe = FindRKth(p, 1);
		if (findPositionRe != NULL){
			printf("find 倒数第%d个元素%d\n", 1, findPositionRe->Data);
		}
		else {
			puts("find error");
		}
		
		ElementType deleteTemp = findPositionRe->Data;
		Position findDataPo = FindData(p, deleteTemp);
		if (DeleteInP(p, &findDataPo) != NULL){
			printf("delete %d\n", deleteTemp);
			Print(p);
		}
		else {
			puts("delete error");
		}
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


//类型接口封装

LinkedList::LinkedList() {
	headNode_ = CreatHead(NULL);
}
LinkedList::LinkedList(LinkedList const &rhs) {
	headNode_ = Clone(rhs.headNode_, false);
}
LinkedList::LinkedList(LinkedList const &&rvalue) {
	headNode_ = Clone(rvalue.headNode_, true);
}
LinkedList::~LinkedList() {
	Destroy(headNode_);
}
LinkedList &LinkedList::input() {
	ReadList(&headNode_);
	return *this;
}
LinkedList &LinkedList::output() {
	Print(headNode_);
	return *this;
}
LinkedList &LinkedList::merge(LinkedList &rhs) {
	Iterator temp = headNode_;
	headNode_ = Merge(headNode_, rhs.headNode_);
	Destroy(temp);
	return *this;
}
LinkedList &LinkedList::mergelists(LinkedList &rhs) {
	Iterator temp = headNode_;
	headNode_ = Mergelists(headNode_, rhs.headNode_);
	Destroy(temp);
	return *this;
}
LinkedList::Iterator LinkedList::intersection(LinkedList &rhs) {
	return Intersection(headNode_, rhs.headNode_);
}
int LinkedList::length() {
	return Length(headNode_);
}
LinkedList::Iterator LinkedList::findKth(int k) {
	return k >= 0 ? FindKth(headNode_, k) : FindRKth(headNode_, -k);
}
LinkedList::Iterator LinkedList::findData(LinkedList::ElementType ele) {
	return FindData(headNode_, ele);
}
bool LinkedList::insertData(ElementType ele) {
	return InsertData(headNode_, ele) != NULL;
}
bool LinkedList::insertInP(ElementType ele, LinkedList::Iterator iter) {
	return InsertInP(headNode_, ele, iter) != NULL;
}
bool LinkedList::deleteInP(LinkedList::Iterator &iter) {
	return DeleteInP(headNode_, &iter) != NULL;
}
int LinkedList::deleteAll(ElementType ele) {
	return DeleteAllData(headNode_, ele) != NULL;
}
LinkedList &LinkedList::reverse() {
	Iterator temp = headNode_;
	headNode_ = Reverse(headNode_);
	Destroy(temp);
	return *this;
}

