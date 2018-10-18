#pragma once
#include "stdafx.h"

class LinkedList {
public:
	using ElementType = int;
	const ElementType ERROR_ELE_VALUE = -1;
	//typedef struct Node *PtrToLinkedListNode;
	struct Node{
		ElementType Data;
		Node *next;
	};
	using Iterator = Node*;

private:
	Iterator headNode_;
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
