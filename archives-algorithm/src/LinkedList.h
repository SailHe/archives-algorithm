#pragma once
#include "stdafx.h"
//#include "ExtendSpace.h"

template<class ElementType>
class LinkedList {
protected:
	struct LinkedListNode {
		ElementType Data;
		LinkedListNode *Next_;
	};
	//using ElementType = T;
	//LinkedListImpl
	typedef LinkedListNode *LinkedListNodePtr;
	//��ָ��λ�� �õ�ַ��ʾ
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
		Iterator &operator--() {
			//û��ʵ��
			baseIter = nullptr;
			return *this;
		}
		Iterator &operator--(int) {
			//û��ʵ��
			baseIter = nullptr;
			return *this;
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
	//��������
	LinkedList(LinkedList const &rhs);
	LinkedList(LinkedList const &&rvalue);
	//void operator = (LinkedList const &rhs) const = delete;
	void operator = (LinkedList const &rhs);

	Iterator const &end() const {
		static Iterator END = Iterator(nullptr);
		return END;
	}
	//read ����ֵ��������
	LinkedList &input();
	//write
	LinkedList &output();
	/***********************************************�����������**************/
	/*
	��㲻�� ԭ��ͷ����
	*/
	/***********************************************�����������************��**/
	//����L1 L2ԭ��ϲ��������ͷ�ڵ�
	LinkedList &merge(LinkedList &rhs);

	//list1��list2��������Data�������ӵ������ͷָ�룻(ͷָ�벻��ͷ�ڵ�)
	//����Mergelists����������ϲ���һ����Data�������ӵ������������ؽ�������ͷָ�롣
	LinkedList &mergelists(LinkedList &rhs);

	//����L1 ��L2�Ľ�����ͷ�ڵ�
	Iterator intersection(LinkedList &rhs);
	/*******************************************����Ĳ��������ݲ���********��**/
	/*
	ԭ������ȫ����
	*/
	/*******************************************����Ĳ��������ݲ���*********/
	//�������������ݽ�����+1
	int length();
	//���������k������  �����ڷ���ERROR_ELE_VALUE :: k����(-length, length)
	Iterator findKth(int k);
	//�������Ա����״γ���ele��λ�� �Ҳ�������NULL
	Iterator findData(ElementType ele);
	//��ele����L�������ָ����е������ԣ����ز���������ͷ
	bool insertData(ElementType ele);
	//��λ��iterǰ����ele  ��������ͷ ������iterλ�÷Ƿ�����NULL   �Դ�����ͷ�ڵ� ��ʹ����ս�� NLLL Ҳ����˳������
	bool insertInP(ElementType ele, Iterator iter);
	//ɾ��λ��iter��Ԫ�� ��������ͷ ������iterλ�÷Ƿ�����NULL
	bool deleteInP(Iterator &iter);
	//ɾ������ �����ض�����
	int deleteAll(ElementType ele);
	//��������  ԭ������ ��ȫ�½� �����½�ͷ�ڵ�
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
	/********************************************����Ĵ������ӡ*************/
	//Ϊ����������һ����ʼ��ֵΪ0�� ͷ�ڵ�
	LinkedListNodePtr CreatHead(LinkedListNodePtr L)
	{
		LinkedListNodePtr Head = newNode();
		
		Head->Next_ = L;
		return Head;
	}
	//˳�򽨱�
	//�ɰ�
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
	//��Ⱦɰ� ��5�и���� һ������һ���ж� ����
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
		return Head/*->Next_   **����ͬ**������Ҫͷ�ڵ������伴��*/;
	}
	//���򽨱� �˷�����ͷ�ڵ��ǽ��������������
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
	void Print(LinkedListNodePtr L)
	{
		int n = 0;
		if (L->Next_)
		{
			while (L = L->Next_)
			{
				printf(n++ ? " " : "");
				std::cout << L->Data;
			}
		}
		else
			printf("NULL");
		puts("");
	}
	/***********************************************�����������**************/
	/*
	��㲻�� ԭ��ͷ����
	*/
	/***********************************************�����������************��**/
	//����L1 L2ԭ��ϲ��������ͷ�ڵ�
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
	//list1��list2��������Data�������ӵ������ͷָ�룻(ͷָ�벻��ͷ�ڵ�)
	//����Mergelists����������ϲ���һ����Data�������ӵ������������ؽ�������ͷָ�롣
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

	//����L1 ��L2�Ľ�����ͷ�ڵ�
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
	/*******************************************����Ĳ��������ݲ���********��**/
	/*
	ԭ������ȫ����
	*/
	/*******************************************����Ĳ��������ݲ���*********/
	//�������������ݽ�����+1
	int Length(LinkedListNodePtr L) {/*��ʵ�ֻ��ں�ͷ�ڵ������*/
		LinkedListNodePtr p = L;
		int n = 0;
		while (p)
			++n, p = p->Next_;
		return n;
	}
	//���������k������  �����ڷ���NULL :: k����(0, length)
	Position FindKth(LinkedListNodePtr L, int k) {
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
	//������������k��Ԫ��  �����ڷ���NULL
	Position FindRKth(LinkedListNodePtr L, int k) {
		//������k������������len-k��
		return FindKth(L, Length(L) - k);//������ͷ�ڵ�length����ֵ��1
	}
	//�������Ա����״γ���X��λ�� �Ҳ�������NULL
	Position FindData(LinkedListNodePtr L, ElementType X) {
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
	//��X����L(���ֵ�����)�������ָ����е������ԣ����ز���������ͷ
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
	//��λ��Pǰ����X  ��������ͷ ������Pλ�÷Ƿ�����NULL   �Դ�����ͷ�ڵ� ��ʹ����ս�� NLLL Ҳ����˳������
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
	//ɾ��λ��P��Ԫ�� ��������ͷ ������Pλ�÷Ƿ�����NULL
	LinkedListNodePtr DeleteInP(LinkedListNodePtr L, Position *DelPosition) {
		LinkedListNodePtr pn = L;
		Position P = *DelPosition;
		pn->Data = 0;
		while (pn->Next_ && P) {
			if (pn->Next_ == P) {
				//����
				P = P->Next_;
				deleteNode(P->Next_);
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
	//ɾ������ �����ض�����(�˴���С��)
	LinkedListNodePtr DeleteAllData(LinkedListNodePtr head, ElementType Data)
	{
		LinkedListNodePtr plast = NULL, Head = newNode(), p = Head;
		Head->Next_ = head;
		while (p->Next_)
		{
			plast = p;
			p = p->Next_;
			if (p->Data < Data)/*ɾ������ �˴���С��*/
			{
				plast->Next_ = p->Next_;//����p����
				p == head ? head = plast->Next_ : 0;//������ɾ���Ľ����headʱheadҪ����
				deleteNode(p);//�ͷ�p�����ڴ�
				p = plast;//��ΪҪ��֤p��ָ���ݵ���ȷ��
			}
		}
		return head;
	}
	//���ٵ����� ��ͷP��NULL
	void Destroy(LinkedListNodePtr &Head)
	{
		LinkedListNodePtr Ptemp = NULL;
		do{
			Ptemp = Head;
			Head = Head->Next_;
			deleteNode(Ptemp);
		} while (Head != NULL);
	}
	//��������  ԭ������ ��ȫ�½� �����½�ͷ�ڵ�
	LinkedListNodePtr Reverse(LinkedListNodePtr Head)
	{
		LinkedListNodePtr plast = NULL, pnow = NULL;
		Head = Head->Next_;/*����Ĳ���ͷ�ڵ�ȥ�����*/
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
	//���ͷŵ�ǰ��� �����ڵ��ַ��Ϊ��һ�����ĵ�ַ û�� ��NULL
	void DestroyNode(LinkedListNodePtr *P)
	{
		LinkedListNodePtr Ptemp = NULL;
		if ((*P)->Next_)
			Ptemp = (*P)->Next_;
		deleteNode(*P);
		*P = Ptemp;
	}
	//����һ������������ͬ�Ľڵ� del������copy����cut
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

	/*
	1 3 5 7 9 11 -1
	2 4 6 8 10 12 -1
	*/
	int mainForLinkedList() {
		//freopen("input", "r", stdin);
		LinkedListNodePtr p1 = NULL, p2 = NULL, p = NULL;
		LinkedListNodePtr pt = NULL;
		int n;
		puts("���������Ԫ�� ��-1��β");
		p1 = CreateListNew();
		p2 = CreateListRe();
		Print(p1); Print(p2);
		puts("begain\n");

		puts("�Է�ת���p2��Ϊ�µ�����p\n");
		p = Reverse(p2);

		puts("����p2\n");
		Destroy(p2);
		puts("���p����p����p2\n");
		Print(p2 = p);

		puts("p = �ϲ�p1 p2���������\n");
		p = Merge(p1, p2); Print(p);
		n = Length(p);
		int i = 0;
		while (p->Next_) {
			++i;

			Position findPosition = FindKth(p, i);
			if (findPosition != NULL) {
				pt = InsertBeforeP(pt, findPosition->Data, pt);
				printf("find ������%d��Ԫ��%d\n", i, findPosition->Data);
			}
			else {
				puts("find error");
			}

			Position findPositionRe = FindRKth(p, 1);
			if (findPositionRe != NULL) {
				printf("find ������%d��Ԫ��%d\n", 1, findPositionRe->Data);
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

private:
	LinkedListNodePtr headNode_;
};



//���ͽӿڷ�װ
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
LinkedList<ElementType> &LinkedList<ElementType>::output() {
	Print(headNode_);
	return *this;
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
