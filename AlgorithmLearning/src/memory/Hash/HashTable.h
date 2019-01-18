#pragma once
/*
散列查找API
Origin: mooc zju 数据结构
@see http://www.icourse163.org/learn/ZJU-93001?tid=1002019005#/learn/announce
C++ STL 哈希表 | unordered_map
*/

#include "pch.h"

namespace HashTableC {
	typedef int Index;                     /* 散列地址类型 */

	// 返回大于N且不超过MAX_INT32的最小素数
	int NextPrime(int N);


	/*③*/
	#define KEYLENGTH 15                   /* 关键词字符串的最大长度 */
	typedef char HashTableLinkedListElementTypeBase[KEYLENGTH + 1]; /* 关键词类型用字符串 */
	typedef HashTableLinkedListElementTypeBase const HashTableLinkedListElementType;
	/******** 以下是单链表的定义 ********/
	typedef struct LNode *PtrToLNode;
	struct LNode {
		HashTableLinkedListElementTypeBase Data;
		PtrToLNode Next;
	};
	typedef PtrToLNode HashTableLinkedListPosition;
	typedef PtrToLNode LinkedList;
	/******** 以上是单链表的定义 ********/

	typedef struct HashTableLiImpl *HashTableLiPtr; /* 散列表类型 */
	struct HashTableLiImpl {   /* 散列表结点定义 */
		int TableSize; /* 表的最大长度 */
		LinkedList Heads;    /* 指向链表头结点的数组 */
	};

	Index Hash(HashTableLinkedListElementType Key, int TableSize);
	HashTableLiPtr CreateLinkedListHashTable(int TableSize);
	HashTableLinkedListPosition Find(HashTableLiPtr H, HashTableLinkedListElementType Key);
	bool Insert(HashTableLiPtr H, HashTableLinkedListElementType Key);
	void DestroyTable(HashTableLiPtr H);

}