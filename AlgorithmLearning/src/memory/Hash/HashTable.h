#pragma once
/*
ɢ�в���API
Origin: mooc zju ���ݽṹ
@see http://www.icourse163.org/learn/ZJU-93001?tid=1002019005#/learn/announce
C++ STL ��ϣ�� | unordered_map
*/

#include "pch.h"

namespace HashTableC {
	typedef int Index;                     /* ɢ�е�ַ���� */

	// ���ش���N�Ҳ�����MAX_INT32����С����
	int NextPrime(int N);


	/*��*/
	#define KEYLENGTH 15                   /* �ؼ����ַ�������󳤶� */
	typedef char HashTableLinkedListElementTypeBase[KEYLENGTH + 1]; /* �ؼ����������ַ��� */
	typedef HashTableLinkedListElementTypeBase const HashTableLinkedListElementType;
	/******** �����ǵ�����Ķ��� ********/
	typedef struct LNode *PtrToLNode;
	struct LNode {
		HashTableLinkedListElementTypeBase Data;
		PtrToLNode Next;
	};
	typedef PtrToLNode HashTableLinkedListPosition;
	typedef PtrToLNode LinkedList;
	/******** �����ǵ�����Ķ��� ********/

	typedef struct HashTableLiImpl *HashTableLiPtr; /* ɢ�б����� */
	struct HashTableLiImpl {   /* ɢ�б��㶨�� */
		int TableSize; /* �����󳤶� */
		LinkedList Heads;    /* ָ������ͷ�������� */
	};

	Index Hash(HashTableLinkedListElementType Key, int TableSize);
	HashTableLiPtr CreateLinkedListHashTable(int TableSize);
	HashTableLinkedListPosition Find(HashTableLiPtr H, HashTableLinkedListElementType Key);
	bool Insert(HashTableLiPtr H, HashTableLinkedListElementType Key);
	void DestroyTable(HashTableLiPtr H);

}