#pragma once
#include "../stdafx.h"
//#include "ExtendSpace.h"
/*
[单向链表](含头节点)
[头节点]: 不带数据的结点
*/
template<class ElementType>
class LinkedList {
protected:
	struct LinkedListNode {
		ElementType Data;
		LinkedListNode *Next_;
	};

	//LinkedListImpl
	typedef LinkedListNode *LinkedListNodePtr;
	//特指其位置 用地址表示
	typedef LinkedListNode *Position;
public:
	//using Iterator = Iterator<LinkedListNode*>;
	class Iterator {
		using BaseIter = LinkedListNodePtr;
	public:
		Iterator(BaseIter ptr) {
			baseIter = ptr;
		}
		Iterator &operator++() {
			baseIter = baseIter->Next_;
			return *this;
		}
		Iterator operator++(int) {
			BaseIter temp = baseIter;
			baseIter = baseIter->Next_;
			return Iterator(temp);
		}
		bool operator==(Iterator const &rhs) const {
			return baseIter == rhs.baseIter;
		}
		bool operator!=(Iterator const &rhs) const {
			return baseIter != rhs.baseIter;
		}
		operator BaseIter() {
			return baseIter;
		}
		operator bool() {
			return baseIter != nullptr;
		}
		ElementType operator*() {
			return baseIter->Data;
		}
	private:
		BaseIter baseIter;
	};

	LinkedList();
	~LinkedList();
	//拷贝构造
	LinkedList(LinkedList const &rhs);
	LinkedList(LinkedList const &&rvalue);
	//void operator = (LinkedList const &rhs) const = delete;
	void operator=(LinkedList const &rhs);

	Iterator begin() const {
		// 除去头节点0
		return this->findKth(1);
	}
	Iterator const &end() const {
		static Iterator END = Iterator(nullptr);
		return END;
	}
	//read 返回值用于连锁
	LinkedList &input();
	//write
	std::string toString(std::function<std::string(ElementType const&)> toString) const;
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
	//返回链表储存数据的结点个数
	int length() const ;
	//返回链表第k条数据  不存在返回ERROR_ELE_VALUE :: k属于(-length, length)
	Iterator findKth(int k) const ;
	//返回线性表中首次出现ele的位置 找不到返回NULL
	Iterator findData(ElementType ele) const ;
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
protected:
	LinkedListNodePtr newNode() {
		return new LinkedListNode();
	}
	LinkedListNodePtr newNode(ElementType const &Data) {
		LinkedListNodePtr temp = new LinkedListNode();
		temp->Data = Data;
		return temp;
	}

	LinkedListNodePtr newNode_c(ElementType Data) {
		LinkedListNodePtr Head = (LinkedListNodePtr)malloc(sizeof(LinkedListNode));
		memset(Head, 0, sizeof(LinkedListNode));
		Head->Data = Data;
		return Head;
	}
	void deleteNode(LinkedListNodePtr &node) {
		delete node;
		node = nullptr;
	}
	/********************************************链表的创建与打印*************/
	//为任意链表创建一个初始化值为0的 头节点
	LinkedListNodePtr CreatHead(LinkedListNodePtr L)
	{
		LinkedListNodePtr Head = newNode();
		
		Head->Next_ = L;
		return Head;
	}
	//顺序建表
	//旧版
	LinkedListNodePtr CreateListOld()
	{
		LinkedListNodePtr plast = NULL, pnow = NULL, head = newNode();
		while (1)
		{
			pnow = newNode();
			scanf("%d", &pnow->Data);
			if (pnow->Data == -1)
			{
				deleteNode(pnow);
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
	LinkedListNodePtr CreateListNew()
	{
		LinkedListNodePtr Head = newNode(), P = Head;
		while (1)
		{
			P->Next_ = newNode();
			//scanf("%d", &P->Next_->Data);
			std::cin >> P->Next_->Data;
			if (P->Next_->Data == -1)
			{
				deleteNode(P->Next_);
				P->Next_ = NULL;
				break;
			}
			P = P->Next_;
		}
		return Head/*->Next_   **以下同**若不需要头节点加上这句即可*/;
	}
	//逆序建表 此法建表头节点是结束标记所带数据
	LinkedListNodePtr CreateListRe()
	{
		LinkedListNodePtr plast = NULL, pnow = NULL;
		while (1)
		{
			pnow = newNode();
			scanf("%d", &pnow->Data);
			pnow->Next_ = plast;
			plast = pnow;
			if (pnow->Data == -1)
				break;
		}
		return pnow;
	}
	void ReadList(LinkedListNodePtr *L) {
		*L = CreateListNew();
	}
	std::string Print(LinkedListNodePtr L, std::function<std::string(ElementType)> toString) const
	{
		// 混用C++也是万不得已(懒)
		std::string result;
		int n = 0;
		if (L->Next_)
		{
			while (L = L->Next_)
			{
				result += n++ ? " " : "";
				result += toString(L->Data);
			}
		}
		else {
			result += "NULL";
		}
		return result;
	}
	/***********************************************递增链表操作**************/
	/*
	结点不变 原表头置零
	*/
	/***********************************************递增链表操作************下**/
	//返回L1 L2原序合并后的链表头节点
	LinkedListNodePtr Merge(LinkedListNodePtr L1, LinkedListNodePtr L2) {
		LinkedListNodePtr head = newNode(), p = head;
		LinkedListNodePtr p1 = L1->Next_, p2 = L2->Next_;
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
	//list1和list2是两个按Data升序链接的链表的头指针；(头指针不是头节点)
	//函数Mergelists将两个链表合并成一个按Data升序链接的新链表，并返回结果链表的头指针。
	LinkedListNode* Mergelists(LinkedListNodePtr L1, LinkedListNodePtr L2) {
		LinkedListNodePtr head = newNode(0), p = head;
		//LinkedListNode *p1 = L1, *p2 = L2;
		LinkedListNodePtr p1 = L1, p2 = L2;
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
	LinkedListNodePtr Intersection(LinkedListNodePtr L1, LinkedListNodePtr L2)
	{
		LinkedListNodePtr head = newNode(), p = head;
		LinkedListNodePtr p1 = L1->Next_, p2 = L2->Next_;
		L1->Next_ = L2->Next_ = NULL;
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
	//返回结点L及其之后结点个数
	int Length(LinkedListNodePtr L) const {
		LinkedListNodePtr p = L;
		int n = 0;
		while (p != NULL)
			++n, p = p->Next_;
		return n;
	}
	//返回链表第k条数据  不存在返回NULL :: k属于(0, length)
	Position FindKth(LinkedListNodePtr L, int k) const {
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
	Position FindRKth(LinkedListNodePtr L, int k) const {
		//倒数第k个就是正数第len-k个
		return FindKth(L, Length(L) - k);//若不带头节点length返回值加1
	}
	//返回线性表中首次出现X的位置 找不到返回NULL
	Position FindData(LinkedListNodePtr L, ElementType X) const {
		LinkedListNodePtr p = L;
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
	//将X插入L(保持递增序)，并保持该序列的有序性，返回插入后的链表头
	LinkedListNodePtr InsertIncreaseData(LinkedListNodePtr L, ElementType X)
	{
		LinkedListNodePtr pn = NULL, pin = newNode(), ptemp = NULL;
		pin->Data = X; pin->Next_ = NULL;
		pn = L;
		while (pn->Next_)
		{
			if (X < pn->Next_->Data)
				break;
			else
				pn = pn->Next_;
		}
		ptemp = pn->Next_;
		pn->Next_ = pin;
		pin->Next_ = ptemp;
		return L;
	}
	//在位置P前插入X  返回链表头 若参数P位置非法返回NULL   自带虚拟头节点 即使传入空结点 NLLL 也可以顺利插入
	LinkedListNodePtr InsertBeforeP(LinkedListNodePtr L, ElementType X, Position P)
	{
		LinkedListNodePtr pn = NULL, pin = newNode(), head = newNode();
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
	LinkedListNodePtr DeleteInP(LinkedListNodePtr L, Position *DelPosition) 
	{
		LinkedListNodePtr pn = L;
		Position P = *DelPosition;
		pn->Data = 0;
		while (pn->Next_ && P) {
			if (pn->Next_ == P) {
				//暂存即将删除结点的下一个
				P = P->Next_;
				deleteNode(pn->Next_);
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
	LinkedListNodePtr DeleteAllData(LinkedListNodePtr head, ElementType Data)
	{
		LinkedListNodePtr plast = NULL, Head = newNode(), p = Head;
		Head->Next_ = head;
		while (p->Next_)
		{
			plast = p;
			p = p->Next_;
			if (p->Data < Data)/*删除条件 此处是小于*/
			{
				plast->Next_ = p->Next_;//连结p两端
				p == head ? head = plast->Next_ : 0;//当即将删除的结点是head时head要更新
				deleteNode(p);//释放p处的内存
				p = plast;//因为要保证p所指内容的正确性
			}
		}
		return head;
	}
	//销毁单链表 表头P置NULL
	void Destroy(LinkedListNodePtr &Head)
	{
		LinkedListNodePtr Ptemp = NULL;
		do{
			Ptemp = Head;
			Head = Head->Next_;
			deleteNode(Ptemp);
		} while (Head != NULL);
	}
	//链表逆置  原链表不变 完全新建 返回新建头节点
	LinkedListNodePtr Reverse(LinkedListNodePtr Head)
	{
		LinkedListNodePtr plast = NULL, pnow = NULL;
		Head = Head->Next_;/*传入的不是头节点去掉这句*/
		while (Head)
		{
			pnow = newNode();
			pnow->Data = Head->Data;
			pnow->Next_ = plast;
			plast = pnow;
			Head = Head->Next_;
		}
		pnow = CreatHead(pnow);
		return pnow;
	}
	LinkedListNodePtr Reverse_DEBUG(LinkedListNodePtr L)
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
	void DestroyNode(LinkedListNodePtr *P)
	{
		LinkedListNodePtr Ptemp = NULL;
		if ((*P)->Next_)
			Ptemp = (*P)->Next_;
		deleteNode(*P);
		*P = Ptemp;
	}
	//返回一个与参数结点相同的节点 del决定是copy还是cut
	LinkedListNodePtr CloneNode(LinkedListNodePtr P, bool del)
	{
		if (P)
		{
			LinkedListNodePtr CloneP = newNode();
			if (del) {
				DestroyNode(&P);
			}
			return CloneP;
		}
		else {
			return NULL;
		}
	}
	LinkedListNodePtr CloneLinkedList(LinkedListNodePtr RhsHead) {
		assert(RhsHead != NULL);
		LinkedListNodePtr Head = newNode(RhsHead->Data), P = Head;
		RhsHead = RhsHead->Next_;

		while (RhsHead != NULL){
			P->Next_ = newNode(RhsHead->Data);
			RhsHead = RhsHead->Next_;
			P = P->Next_;
		}
		return Head;
	}

private:
	LinkedListNodePtr headNode_;
};



//类型接口封装
template<class ElementType>
LinkedList<ElementType>::LinkedList() {
	headNode_ = CreatHead(NULL);
}
template<class ElementType>
LinkedList<ElementType>::LinkedList(LinkedList const &rhs) {
	headNode_ = CloneLinkedList(rhs.headNode_);
}
template<class ElementType>
LinkedList<ElementType>::LinkedList(LinkedList const &&rvalue) {
	headNode_ = CloneLinkedList(rvalue.headNode_);
}
template<class ElementType>
void LinkedList<ElementType>::operator = (LinkedList const &rhs) {
	headNode_ = CloneLinkedList(rhs.headNode_);
}
template<class ElementType>
LinkedList<ElementType>::~LinkedList() {
	Destroy(headNode_);
}
template<class ElementType>
LinkedList<ElementType> &LinkedList<ElementType>::input() {
	ReadList(&headNode_);
	return *this;
}
template<class ElementType>
std::string LinkedList<ElementType>::toString(std::function<std::string(ElementType const&)> toString) const {
	return Print(headNode_, toString);
}
template<class ElementType>
LinkedList<ElementType> &LinkedList<ElementType>::merge(LinkedList<ElementType> &rhs) {
	LinkedListNodePtr temp = headNode_;
	headNode_ = Merge(headNode_, rhs.headNode_);
	Destroy(temp);
	return *this;
}
template<class ElementType>
LinkedList<ElementType> &LinkedList<ElementType>::mergelists(LinkedList<ElementType> &rhs) {
	LinkedListNodePtr temp = headNode_;
	headNode_ = Mergelists(headNode_, rhs.headNode_);
	Destroy(temp);
	return *this;
}
template<class ElementType >
typename LinkedList<ElementType>::Iterator LinkedList<ElementType>::intersection(LinkedList<ElementType> &rhs) {
	return Intersection(headNode_, rhs.headNode_);
}
template<class ElementType>
int LinkedList<ElementType>::length() const {
	return Length(headNode_->Next_);
}
template<class ElementType>
typename LinkedList<ElementType>::Iterator LinkedList<ElementType>::findKth(int k) const {
	return k >= 0 ? FindKth(headNode_, k) : FindRKth(headNode_, -k);
}
template<class ElementType>
typename LinkedList<ElementType>::Iterator LinkedList<ElementType>::findData(ElementType ele) const {
	return FindData(headNode_, ele);
}
template<class ElementType>
bool LinkedList<ElementType>::insertData(ElementType ele) {
	return InsertIncreaseData(headNode_, ele) != NULL;
}
template<class ElementType>
bool LinkedList<ElementType>::insertInP(ElementType ele, typename LinkedList<ElementType>::Iterator iter) {
	return InsertBeforeP(headNode_, ele, iter) != NULL;
}
template<class ElementType>
bool LinkedList<ElementType>::deleteInP(typename LinkedList<ElementType>::Iterator &iter) {
	LinkedListNodePtr temp = iter;
	return DeleteInP(headNode_, &temp) != NULL;
}
template<class ElementType>
int LinkedList<ElementType>::deleteAll(ElementType ele) {
	return DeleteAllData(headNode_, ele) != NULL;
}
template<class ElementType>
LinkedList<ElementType> &LinkedList<ElementType>::reverse() {
	LinkedListNodePtr temp = headNode_;
	headNode_ = Reverse(headNode_);
	Destroy(temp);
	return *this;
}
