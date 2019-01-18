#include "HashTable.h"
namespace HashTableC {
	Index Hash(HashTableLinkedListElementType Key, int TableSize) {
		size_t len = strlen(Key);
		size_t sumAssic = 0;
		for (size_t i = 0; i < len; ++i) {
			sumAssic += Key[i];
		}
		return sumAssic % TableSize;
	}

	HashTableLiPtr CreateLinkedListHashTable(int TableSize) {
		HashTableLiPtr H;
		int i;

		H = (HashTableLiPtr)malloc(sizeof(struct HashTableLiImpl));
		/* 保证散列表最大长度是素数，具体见代码5.3 */
		H->TableSize = NextPrime(TableSize);

		/* 以下分配链表头结点数组 */
		H->Heads = (HashTableLinkedListPosition)malloc(H->TableSize * sizeof(struct LNode));
		/* 初始化表头结点 */
		for (i = 0; i < H->TableSize; i++) {
			H->Heads[i].Data[0] = '\0';
			H->Heads[i].Next = NULL;
		}

		return H;
	}

	HashTableLinkedListPosition Find(HashTableLiPtr H, HashTableLinkedListElementType Key) {
		HashTableLinkedListPosition P;
		Index HashValue;

		HashValue = Hash(Key, H->TableSize); /* 初始散列位置 */
		P = H->Heads[HashValue].Next; /* 从该链表的第1个结点开始 */
		/* 当未到表尾，并且Key未找到时 */
		while (P && strcmp(P->Data, Key))
			P = P->Next;

		return P; /* 此时P或者指向找到的结点，或者为NULL */
	}

	bool Insert(HashTableLiPtr H, HashTableLinkedListElementType Key) {
		HashTableLinkedListPosition P, NewCell;
		Index HashValue;

		P = Find(H, Key);
		if (!P) { /* 关键词未找到，可以插入 */
			NewCell = (HashTableLinkedListPosition)malloc(sizeof(struct LNode));
			strcpy(NewCell->Data, Key);
			HashValue = Hash(Key, H->TableSize); /* 初始散列位置 */
			/* 将NewCell插入为H->Heads[HashValue]链表的第1个结点 */
			NewCell->Next = H->Heads[HashValue].Next;
			H->Heads[HashValue].Next = NewCell;
			return true;
		}
		else { /* 关键词已存在 */
			printf("键值已存在");
			return false;
		}
	}

	void DestroyTable(HashTableLiPtr H) {
		int i;
		HashTableLinkedListPosition P, Tmp;

		/* 释放每个链表的结点 */
		for (i = 0; i < H->TableSize; i++) {
			P = H->Heads[i].Next;
			while (P) {
				Tmp = P->Next;
				free(P);
				P = Tmp;
			}
		}
		free(H->Heads); /* 释放头结点数组 */
		free(H);        /* 释放散列表结点 */
	}
}
