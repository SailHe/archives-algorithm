#pragma once
#include <stdio.h>
#include <stdlib.h>

/*
˳������ ArrayListSequence
���Ա������������򵥡�Ҳ����õ�һ�����ݽṹ��
���Ա�linear list�������ݽṹ��һ�֣�һ�����Ա���n��������ͬ���Ե�����Ԫ�ص��������С�

˳������ڼ�����ڴ������������ʽ��������Ա���ָ��һ���ַ�����Ĵ洢��Ԫ���δ洢����Ԫ�ص����Խṹ��
*/
namespace LinearList {
#define ARRAY_LINEAR_LIST_MAX_SIZE 7
#define ARRAY_LINEAR_LIST_ERROR_POSITION -1
	typedef int ElementType;
	typedef int Position;
	typedef struct LNode *List;
	struct LNode {
		ElementType Data[ARRAY_LINEAR_LIST_MAX_SIZE];
		// �������Ա������һ��Ԫ�ص�λ��
		Position Last;
	};

	List MakeEmpty();

	Position Find(List L, ElementType X);

	bool Insert(List L, ElementType X, Position P);

	bool Delete(List L, Position P);

	int mainForArrayLinearList();
}