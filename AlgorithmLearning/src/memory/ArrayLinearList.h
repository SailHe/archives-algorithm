#pragma once
#include <stdio.h>
#include <stdlib.h>

/*
˳������ ArrayListSequence
���Ա������������򵥡�Ҳ����õ�һ�����ݽṹ��
���Ա�linear list�������ݽṹ��һ�֣�һ�����Ա���n��������ͬ���Ե�����Ԫ�ص��������С�

˳������ڼ�����ڴ������������ʽ��������Ա���ָ��һ���ַ�����Ĵ洢��Ԫ���δ洢����Ԫ�ص����Խṹ��

ʾ������: ��֧��ģ��(����ֻ��intҲ���������), ��֧���Զ����С
*/
namespace LinearList {
	#define ARRAY_LINEAR_LIST_MAX_SIZE 7
	#define ARRAY_LINEAR_LIST_ERROR_POSITION -1
	typedef int ElementType;
	typedef int Position;
	typedef struct ArrayListNode *ArrayLinearList;
	struct ArrayListNode {
		ElementType Data[ARRAY_LINEAR_LIST_MAX_SIZE];
		// �������Ա������һ��Ԫ�ص�λ��
		Position Last;
	};

	// ����������һ���յ����Ա�;
	ArrayLinearList MakeEmpty();

	// �������Ա���X��λ�á����Ҳ����򷵻�ARRAY_LINEAR_LIST_ERROR_POSITION;
	Position Find(ArrayLinearList L, ElementType X);

	// ��X������λ��P������true;
	// ���ռ����������ӡ��FULL��������false;
	// �������Pָ��Ƿ�λ�ã����ӡ��ILLEGAL POSITION��������false;
	bool Insert(ArrayLinearList L, ElementType X, Position P);

	// ��λ��P��Ԫ��ɾ��������true��
	// ������Pָ��Ƿ�λ�ã����ӡ��POSITION P EMPTY��������P�ǲ���ֵ��������false��
	bool Delete(ArrayLinearList L, Position P);

	// ɾ������ֵ����min����С��max��Ԫ��(min, max)��ɾ�������ʣ��Ԫ�ر���˳��洢���������λ�ò��ܸı䡣
	int DeleteAll(int A[], int L, int minA, int maxA);

	int mainForArrayLinearList();

}