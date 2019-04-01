#pragma once
#include "../stdafx.h"
//#include "ExtendSpace.h"
/*
[��������](��ͷ�ڵ�)
[ͷ�ڵ�]: �������ݵĽ��
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
	void operator=(LinkedList const &rhs);

	Iterator begin() const {
		// ��ȥͷ�ڵ�0
		return this->findKth(1);
	}
	Iterator const &end() const {
		static Iterator END = Iterator(nullptr);
		return END;
	}
	//read ����ֵ��������
	LinkedList &input();
	//write
	std::string toString(std::function<std::string(ElementType const&)> toString) const;
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
	//�������������ݵĽ�����
	int length() const ;
	//���������k������  �����ڷ���ERROR_ELE_VALUE :: k����(-length, length)
	Iterator findKth(int k) const ;
	//�������Ա����״γ���ele��λ�� �Ҳ�������NULL
	Iterator findData(ElementType ele) const ;
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
	std::string Print(LinkedListNodePtr L, std::function<std::string(ElementType)> toString) const
	{
		// ����C++Ҳ���򲻵���(��)
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
	//���ؽ��L����֮�������
	int Length(LinkedListNodePtr L) const {
		LinkedListNodePtr p = L;
		int n = 0;
		while (p != NULL)
			++n, p = p->Next_;
		return n;
	}
	//���������k������  �����ڷ���NULL :: k����(0, length)
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
	//������������k��Ԫ��  �����ڷ���NULL
	Position FindRKth(LinkedListNodePtr L, int k) const {
		//������k������������len-k��
		return FindKth(L, Length(L) - k);//������ͷ�ڵ�length����ֵ��1
	}
	//�������Ա����״γ���X��λ�� �Ҳ�������NULL
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
	LinkedListNodePtr DeleteInP(LinkedListNodePtr L, Position *DelPosition) 
	{
		LinkedListNodePtr pn = L;
		Position P = *DelPosition;
		pn->Data = 0;
		while (pn->Next_ && P) {
			if (pn->Next_ == P) {
				//�ݴ漴��ɾ��������һ��
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
