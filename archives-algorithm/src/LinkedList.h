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
	//��ָ��λ�� �õ�ַ��ʾ
	typedef LinkedListNode *Position;
	//const int LEN = sizeof(struct LinkedList::LinkedListNode);
	const int LEN = sizeof(LinkedListNode);
	/********************************************����Ĵ������ӡ*************/
	//Ϊ����������һ����ʼ��ֵΪ0�� ͷ�ڵ�
	LinkedListImpl CreatHead(LinkedListImpl L)
	{
		LinkedListImpl Head = (LinkedListImpl)malloc(LEN);
		memset(Head, 0, LEN);
		Head->Next_ = L;
		return Head;
	}
	//˳�򽨱�
	//�ɰ�
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
	//��Ⱦɰ� ��5�и���� һ������һ���ж� ����
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
		return Head/*->Next_   **����ͬ**������Ҫͷ�ڵ������伴��*/;
	}
	//���򽨱� �˷�����ͷ�ڵ��ǽ��������������
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
	/***********************************************�����������**************/
	/*
	��㲻�� ԭ��ͷ����
	*/
	/***********************************************�����������************��**/
	//����L1 L2ԭ��ϲ��������ͷ�ڵ�
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
	//list1��list2��������Data�������ӵ������ͷָ�룻(ͷָ�벻��ͷ�ڵ�)
	//����Mergelists����������ϲ���һ����Data�������ӵ������������ؽ�������ͷָ�롣
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

	//����L1 ��L2�Ľ�����ͷ�ڵ�
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
	/*******************************************����Ĳ��������ݲ���********��**/
	/*
	ԭ������ȫ����
	*/
	/*******************************************����Ĳ��������ݲ���*********/
	//�������������ݽ�����+1
	int Length(LinkedListImpl L) {/*��ʵ�ֻ��ں�ͷ�ڵ������*/
		LinkedListImpl p = L;
		int n = 0;
		while (p)
			++n, p = p->Next_;
		return n;
	}
	//���������k������  �����ڷ���NULL :: k����(0, length)
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
	//������������k��Ԫ��  �����ڷ���NULL
	Position FindRKth(LinkedListImpl L, int k) {
		//������k������������len-k��
		return FindKth(L, Length(L) - k);//������ͷ�ڵ�length����ֵ��1
	}
	//�������Ա����״γ���X��λ�� �Ҳ�������NULL
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
	//��X����L�������ָ����е������ԣ����ز���������ͷ
	LinkedListImpl InsertData(LinkedListImpl L, ElementType X)
	{
		LinkedListImpl pn = NULL, pin = (LinkedListImpl)malloc(LEN), ptemp = NULL;
		pin->Data = X; pin->Next_ = NULL;
		pn = L;
		while (pn->Next_)
		{
			if (X < pn->Next_->Data)
				break;/*���ֵ�����*/
			else
				pn = pn->Next_;
		}
		ptemp = pn->Next_;
		pn->Next_ = pin;
		pin->Next_ = ptemp;
		return L;
	}
	//��λ��Pǰ����X  ��������ͷ ������Pλ�÷Ƿ�����NULL   �Դ�����ͷ�ڵ� ��ʹ����ս�� NLLL Ҳ����˳������
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
	//ɾ��λ��P��Ԫ�� ��������ͷ ������Pλ�÷Ƿ�����NULL
	LinkedListImpl DeleteInP(LinkedListImpl L, Position *DelPosition) {
		LinkedListImpl pn = L;
		Position P = *DelPosition;
		pn->Data = 0;
		while (pn->Next_ && P) {
			if (pn->Next_ == P) {
				P = P->Next_;//����
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
	//ɾ������ �����ض�����(�˴���С��)
	LinkedListImpl DeleteAllData(LinkedListImpl head, ElementType Data)
	{
		LinkedListImpl plast = NULL, Head = (LinkedListImpl)malloc(LEN), p = Head;
		Head->Next_ = head;
		while (p->Next_)
		{
			plast = p;
			p = p->Next_;
			if (p->Data < Data)/*ɾ������ �˴���С��*/
			{
				plast->Next_ = p->Next_;//����p����
				p == head ? head = plast->Next_ : 0;//������ɾ���Ľ����headʱheadҪ����
				free(p);//�ͷ�p�����ڴ�
				p = plast;//��ΪҪ��֤p��ָ���ݵ���ȷ��
			}
		}
		return head;
	}
	//���ٵ����� ��ͷP��NULL
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
	//��������  ԭ������ ��ȫ�½� �����½�ͷ�ڵ�
	LinkedListImpl Reverse(LinkedListImpl Head)
	{
		LinkedListImpl plast = NULL, pnow = NULL;
		Head = Head->Next_;/*����Ĳ���ͷ�ڵ�ȥ�����*/
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
	//���ͷŵ�ǰ��� �����ڵ��ַ��Ϊ��һ�����ĵ�ַ û�� ��NULL
	void DestroyNode(LinkedListImpl *P)
	{
		LinkedListImpl Ptemp = NULL;
		if ((*P)->Next_)
			Ptemp = (*P)->Next_;
		free(*P);
		*P = Ptemp;
	}
	//����һ������������ͬ�Ľڵ� del������copy����cut
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
				pt = InsertInP(pt, findPosition->Data, pt);
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
	/*
	1 3 5 7 9 11 -1
	2 4 6 8 10 12 -1
	*/

public:
	LinkedList();
	LinkedList(LinkedList const &rhs);
	LinkedList(LinkedList const &&rvalue);
	~LinkedList();

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



};



//���ͽӿڷ�װ
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
