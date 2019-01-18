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
		/* ��֤ɢ�б���󳤶������������������5.3 */
		H->TableSize = NextPrime(TableSize);

		/* ���·�������ͷ������� */
		H->Heads = (HashTableLinkedListPosition)malloc(H->TableSize * sizeof(struct LNode));
		/* ��ʼ����ͷ��� */
		for (i = 0; i < H->TableSize; i++) {
			H->Heads[i].Data[0] = '\0';
			H->Heads[i].Next = NULL;
		}

		return H;
	}

	HashTableLinkedListPosition Find(HashTableLiPtr H, HashTableLinkedListElementType Key) {
		HashTableLinkedListPosition P;
		Index HashValue;

		HashValue = Hash(Key, H->TableSize); /* ��ʼɢ��λ�� */
		P = H->Heads[HashValue].Next; /* �Ӹ�����ĵ�1����㿪ʼ */
		/* ��δ����β������Keyδ�ҵ�ʱ */
		while (P && strcmp(P->Data, Key))
			P = P->Next;

		return P; /* ��ʱP����ָ���ҵ��Ľ�㣬����ΪNULL */
	}

	bool Insert(HashTableLiPtr H, HashTableLinkedListElementType Key) {
		HashTableLinkedListPosition P, NewCell;
		Index HashValue;

		P = Find(H, Key);
		if (!P) { /* �ؼ���δ�ҵ������Բ��� */
			NewCell = (HashTableLinkedListPosition)malloc(sizeof(struct LNode));
			strcpy(NewCell->Data, Key);
			HashValue = Hash(Key, H->TableSize); /* ��ʼɢ��λ�� */
			/* ��NewCell����ΪH->Heads[HashValue]����ĵ�1����� */
			NewCell->Next = H->Heads[HashValue].Next;
			H->Heads[HashValue].Next = NewCell;
			return true;
		}
		else { /* �ؼ����Ѵ��� */
			printf("��ֵ�Ѵ���");
			return false;
		}
	}

	void DestroyTable(HashTableLiPtr H) {
		int i;
		HashTableLinkedListPosition P, Tmp;

		/* �ͷ�ÿ������Ľ�� */
		for (i = 0; i < H->TableSize; i++) {
			P = H->Heads[i].Next;
			while (P) {
				Tmp = P->Next;
				free(P);
				P = Tmp;
			}
		}
		free(H->Heads); /* �ͷ�ͷ������� */
		free(H);        /* �ͷ�ɢ�б��� */
	}
}
