#pragma once
#include "stdafx.h"

template<class ElementType>
class LinkedList {
public:
	//using ElementType = int;
	//const ElementType ERROR_ELE_VALUE = -1;
	//typedef struct LinkedListNode *PtrToLinkedListNode;
	struct LinkedListNode{
		ElementType Data;
		LinkedListNode *Next_;
	};
	using Iterator = LinkedListNode*;

private:
	Iterator headNode_;

protected:
	//typedef LinkedList::ElementType ElementType;
	typedef struct LinkedList::LinkedListNode LinkedListNode;
	typedef LinkedListNode *LinkedListImpl;
	//特指其位置 用地址表示
	typedef LinkedListNode *Position;
	//const int LEN = sizeof(struct LinkedList::LinkedListNode);
	const int LEN = sizeof(LinkedListNode);
	/********************************************链表的创建与打印*************/
	//为任意链表创建一个初始化值为0的 头节点
	LinkedListImpl CreatHead(LinkedListImpl L)
	{
		LinkedListImpl Head = (LinkedListImpl)malloc(LEN);
		memset(Head, 0, LEN);
		Head->Next_ = L;
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
				head->Next_ = pnow;
			else
				plast->Next_ = pnow;
			plast = pnow;
		}
		if (plast)
			plast->Next_ = NULL;
		return head;
	}
	//相比旧版 少5行更简洁 一个变量一个判断 更快
	LinkedListImpl CreateListNew()
	{
		LinkedListImpl Head = (LinkedListImpl)malloc(LEN), P = Head;
		memset(Head, 0, LEN);
		while (1)
		{
			P->Next_ = (LinkedListImpl)malloc(LEN);
			scanf("%d", &P->Next_->Data);
			if (P->Next_->Data == -1)
			{
				free(P->Next_);
				P->Next_ = NULL;
				break;
			}
			P = P->Next_;
		}
		return Head/*->Next_   **以下同**若不需要头节点加上这句即可*/;
	}
	//逆序建表 此法建表头节点是结束标记所带数据
	LinkedListImpl CreateListRe()
	{
		LinkedListImpl plast = NULL, pnow = NULL;
		while (1)
		{
			pnow = (LinkedListImpl)malloc(LEN);
			scanf("%d", &pnow->Data);
			pnow->Next_ = plast;
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
		if (L->Next_)
		{
			while (L = L->Next_)
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
	LinkedListImpl Merge(LinkedListImpl L1, LinkedListImpl L2) {
		LinkedListImpl head = (LinkedListImpl)malloc(sizeof(LinkedListImpl)), p = head;
		LinkedListImpl p1 = L1->Next_, p2 = L2->Next_;
		L1->Next_ = L2->Next_ = NULL;
		while (p1 && p2) {
			if (p1->Data <= p2->Data) {
				p->Next_ = p1;
				p1 = p1->Next_;
			}
			else {
				p->Next_ = p2;
				p2 = p2->Next_;
			}
			p = p->Next_;
		}
		p->Next_ = p1 ? p1 : p2;
		return head;
	}

	LinkedListImpl newNode(ElementType Data) {
		LinkedListImpl Head = (LinkedListImpl)malloc(LEN);
		memset(Head, 0, LEN);
		Head->Data = Data;
		return Head;
	}
	//list1和list2是两个按Data升序链接的链表的头指针；(头指针不是头节点)
	//函数Mergelists将两个链表合并成一个按Data升序链接的新链表，并返回结果链表的头指针。
	LinkedListNode* Mergelists(LinkedListImpl L1, LinkedListImpl L2) {
		LinkedListImpl head = newNode(0), p = head;
		//LinkedListNode *p1 = L1, *p2 = L2;
		LinkedListImpl p1 = L1, p2 = L2;
		while (p1 && p2) {
			if (p1->Data <= p2->Data) {
				p->Next_ = newNode(p1->Data);
				p1 = p1->Next_;
			}
			else {
				p->Next_ = newNode(p2->Data);
				p2 = p2->Next_;
			}
			p = p->Next_;
		}
		p->Next_ = p1 ? p1 : p2;
		return head->Next_;
	}

	//返回L1 与L2的交集的头节点
	LinkedListImpl Intersection(LinkedListImpl L1, LinkedListImpl L2)
	{
		LinkedListImpl head = (LinkedListImpl)malloc(LEN), p = head;
		LinkedListImpl p1 = L1->Next_, p2 = L2->Next_;
		L1->Next_ = L2->Next_ = NULL;
		memset(head, 0, LEN);
		while (p1 && p2)
		{
			if (p1->Data == p2->Data)
			{
				p->Next_ = p1;
				p1 = p1->Next_;
				p2 = p2->Next_;
				p = p->Next_;
			}
			else if (p1->Data < p2->Data)
			{
				p1 = p1->Next_;
			}
			else
			{
				p2 = p2->Next_;
			}
		}
		p->Next_ = NULL;
		return head;
	}
	/*******************************************链表的查找与数据操作********上**/
	/*
	原链表完全不变
	*/
	/*******************************************链表的查找与数据操作*********/
	//返回链表储存数据结点个数+1
	int Length(LinkedListImpl L) {/*此实现基于含头节点的链表*/
		LinkedListImpl p = L;
		int n = 0;
		while (p)
			++n, p = p->Next_;
		return n;
	}
	//返回链表第k条数据  不存在返回NULL :: k属于(0, length)
	Position FindKth(LinkedListImpl L, int k) {
		int n = 1;
		while (L = L->Next_) {
			if (n < k)
				++n;
			else if (n == k)
				return L;
			else//k <= 0
				break;
		}
		return NULL;
	}
	//返回链表倒数第k个元素  不存在返回NULL
	Position FindRKth(LinkedListImpl L, int k) {
		//倒数第k个就是正数第len-k个
		return FindKth(L, Length(L) - k);//若不带头节点length返回值加1
	}
	//返回线性表中首次出现X的位置 找不到返回NULL
	Position FindData(LinkedListImpl L, ElementType X) {
		LinkedListImpl p = L;
		p->Data = -X;
		while (p)
		{
			if (p->Data == X)
			{
				return p;
			}
			p = p->Next_;
		}
		return NULL;
	}
	//将X插入L，并保持该序列的有序性，返回插入后的链表头
	LinkedListImpl InsertData(LinkedListImpl L, ElementType X)
	{
		LinkedListImpl pn = NULL, pin = (LinkedListImpl)malloc(LEN), ptemp = NULL;
		pin->Data = X; pin->Next_ = NULL;
		pn = L;
		while (pn->Next_)
		{
			if (X < pn->Next_->Data)
				break;/*保持递增序*/
			else
				pn = pn->Next_;
		}
		ptemp = pn->Next_;
		pn->Next_ = pin;
		pin->Next_ = ptemp;
		return L;
	}
	//在位置P前插入X  返回链表头 若参数P位置非法返回NULL   自带虚拟头节点 即使传入空结点 NLLL 也可以顺利插入
	LinkedListImpl InsertInP(LinkedListImpl L, ElementType X, Position P)
	{
		LinkedListImpl pn = NULL, pin = (LinkedListImpl)malloc(LEN), head = (LinkedListImpl)malloc(LEN);
		pn = head;
		pn->Data = -X; pn->Next_ = L;
		pin->Data = X; pin->Next_ = NULL;
		while (pn)
		{
			if (pn->Next_ == P)
			{
				pn->Next_ = pin;
				pin->Next_ = P;
				return head->Next_;
			}
			else
				pn = pn->Next_;
		}
		return NULL;
	}
	//删除位置P的元素 返回链表头 若参数P位置非法返回NULL
	LinkedListImpl DeleteInP(LinkedListImpl L, Position *DelPosition) {
		LinkedListImpl pn = L;
		Position P = *DelPosition;
		pn->Data = 0;
		while (pn->Next_ && P) {
			if (pn->Next_ == P) {
				P = P->Next_;//暂用
				free(pn->Next_);
				pn->Next_ = P;
				*DelPosition = NULL;
				return L;
			}
			else {
				pn = pn->Next_;
			}
		}
		return NULL;
	}
	//删除链表 所有特定数据(此处是小于)
	LinkedListImpl DeleteAllData(LinkedListImpl head, ElementType Data)
	{
		LinkedListImpl plast = NULL, Head = (LinkedListImpl)malloc(LEN), p = Head;
		Head->Next_ = head;
		while (p->Next_)
		{
			plast = p;
			p = p->Next_;
			if (p->Data < Data)/*删除条件 此处是小于*/
			{
				plast->Next_ = p->Next_;//连结p两端
				p == head ? head = plast->Next_ : 0;//当即将删除的结点是head时head要更新
				free(p);//释放p处的内存
				p = plast;//因为要保证p所指内容的正确性
			}
		}
		return head;
	}
	//销毁单链表 表头P置NULL
	void Destroy(LinkedListImpl P)
	{
		LinkedListImpl Ptemp = P->Next_;
		P->Next_ = NULL;
		while (Ptemp)
		{
			P = Ptemp->Next_;
			free(Ptemp);
			Ptemp = P;
		}
	}
	//链表逆置  原链表不变 完全新建 返回新建头节点
	LinkedListImpl Reverse(LinkedListImpl Head)
	{
		LinkedListImpl plast = NULL, pnow = NULL;
		Head = Head->Next_;/*传入的不是头节点去掉这句*/
		while (Head)
		{
			pnow = (LinkedListImpl)malloc(LEN);
			pnow->Data = Head->Data;
			pnow->Next_ = plast;
			plast = pnow;
			Head = Head->Next_;
		}
		pnow = CreatHead(pnow);
		return pnow;
	}
	LinkedListImpl Reverse_DEBUG(LinkedListImpl L)
	{
		Position Old_head, New_head, Temp;
		New_head = NULL;
		Old_head = L->Next_;

		while (Old_head)
		{
			Temp = Old_head->Next_;
			Old_head->Next_ = New_head;
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
		if ((*P)->Next_)
			Ptemp = (*P)->Next_;
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

	int mainForTest() {
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
		while (p->Next_) {
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
			if (findPositionRe != NULL) {
				printf("find 倒数第%d个元素%d\n", 1, findPositionRe->Data);
			}
			else {
				puts("find error");
			}

			ElementType deleteTemp = findPositionRe->Data;
			Position findDataPo = FindData(p, deleteTemp);
			if (DeleteInP(p, &findDataPo) != NULL) {
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

public:
	LinkedList();
	LinkedList(LinkedList const &rhs);
	LinkedList(LinkedList const &&rvalue);
	~LinkedList();

	//read 返回值用于连锁
	LinkedList &input();
	//write
	LinkedList &output();
	/***********************************************递增链表操作**************/
	/*
	结点不变 原表头置零
	*/
	/***********************************************递增链表操作************下**/
	//返回L1 L2原序合并后的链表头节点
	LinkedList &merge(LinkedList &rhs);

	//list1和list2是两个按Data升序链接的链表的头指针；(头指针不是头节点)
	//函数Mergelists将两个链表合并成一个按Data升序链接的新链表，并返回结果链表的头指针。
	LinkedList &mergelists(LinkedList &rhs);

	//返回L1 与L2的交集的头节点
	Iterator intersection(LinkedList &rhs);
	/*******************************************链表的查找与数据操作********上**/
	/*
	原链表完全不变
	*/
	/*******************************************链表的查找与数据操作*********/
	//返回链表储存数据结点个数+1
	int length();
	//返回链表第k条数据  不存在返回ERROR_ELE_VALUE :: k属于(-length, length)
	Iterator findKth(int k);
	//返回线性表中首次出现ele的位置 找不到返回NULL
	Iterator findData(ElementType ele);
	//将ele插入L，并保持该序列的有序性，返回插入后的链表头
	bool insertData(ElementType ele);
	//在位置iter前插入ele  返回链表头 若参数iter位置非法返回NULL   自带虚拟头节点 即使传入空结点 NLLL 也可以顺利插入
	bool insertInP(ElementType ele, Iterator iter);
	//删除位置iter的元素 返回链表头 若参数iter位置非法返回NULL
	bool deleteInP(Iterator &iter);
	//删除链表 所有特定数据
	int deleteAll(ElementType ele);
	//链表逆置  原链表不变 完全新建 返回新建头节点
	LinkedList &reverse();



};



//类型接口封装
template<class ElementType>LinkedList<ElementType>::LinkedList() {
	headNode_ = CreatHead(NULL);
}
template<class ElementType>LinkedList<ElementType>::LinkedList(LinkedList const &rhs) {
	headNode_ = Clone(rhs.headNode_, false);
}
template<class ElementType>LinkedList<ElementType>::LinkedList(LinkedList const &&rvalue) {
	headNode_ = Clone(rvalue.headNode_, true);
}
template<class ElementType>LinkedList<ElementType>::~LinkedList() {
	Destroy(headNode_);
}
template<class ElementType>
LinkedList<ElementType> &LinkedList<ElementType>::input() {
	ReadList(&headNode_);
	return *this;
}
template<class ElementType>
LinkedList<ElementType> &LinkedList<ElementType>::output() {
	Print(headNode_);
	return *this;
}
template<class ElementType>
LinkedList<ElementType> &LinkedList<ElementType>::merge(LinkedList<ElementType> &rhs) {
	Iterator temp = headNode_;
	headNode_ = Merge(headNode_, rhs.headNode_);
	Destroy(temp);
	return *this;
}
template<class ElementType>
LinkedList<ElementType> &LinkedList<ElementType>::mergelists(LinkedList<ElementType> &rhs) {
	Iterator temp = headNode_;
	headNode_ = Mergelists(headNode_, rhs.headNode_);
	Destroy(temp);
	return *this;
}
template<class ElementType >
typename LinkedList<ElementType>::Iterator LinkedList<ElementType>::intersection(LinkedList<ElementType> &rhs) {
	return Intersection(headNode_, rhs.headNode_);
}
template<class ElementType>
int LinkedList<ElementType>::length() {
	return Length(headNode_);
}
template<class ElementType>
typename LinkedList<ElementType>::Iterator LinkedList<ElementType>::findKth(int k) {
	return k >= 0 ? FindKth(headNode_, k) : FindRKth(headNode_, -k);
}
template<class ElementType>
typename LinkedList<ElementType>::Iterator LinkedList<ElementType>::findData(ElementType ele) {
	return FindData(headNode_, ele);
}
template<class ElementType>
bool LinkedList<ElementType>::insertData(ElementType ele) {
	return InsertData(headNode_, ele) != NULL;
}
template<class ElementType>
bool LinkedList<ElementType>::insertInP(ElementType ele, typename LinkedList<ElementType>::Iterator iter) {
	return InsertInP(headNode_, ele, iter) != NULL;
}
template<class ElementType>
bool LinkedList<ElementType>::deleteInP(typename LinkedList<ElementType>::Iterator &iter) {
	return DeleteInP(headNode_, &iter) != NULL;
}
template<class ElementType>
int LinkedList<ElementType>::deleteAll(ElementType ele) {
	return DeleteAllData(headNode_, ele) != NULL;
}
template<class ElementType>
LinkedList<ElementType> &LinkedList<ElementType>::reverse() {
	Iterator temp = headNode_;
	headNode_ = Reverse(headNode_);
	Destroy(temp);
	return *this;
}
