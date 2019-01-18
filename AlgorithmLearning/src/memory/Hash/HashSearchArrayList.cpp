#include "HashTable.h"
namespace HashTableC {
	/*
��
���ʵ��û�н��ɢ�г�ͻ ����������
*/
	typedef int ElementType;    /* �ؼ������������� */
	/* ɢ�е�Ԫ״̬���ͣ��ֱ��Ӧ���кϷ�Ԫ�ء��յ�Ԫ������ɾ��Ԫ�� */
	typedef enum { Legitimate, Empty, Deleted } EntryStatusType;

	typedef struct HashEntry Cell; /* ɢ�б�Ԫ���� */
	struct HashEntry {
		ElementType Data; /* ���Ԫ�� */
		EntryStatusType Status;   /* ��Ԫ״̬ */
	};

	typedef struct HashTableArImpl *HashTableArPtr; /* ɢ�б����� */
	// ɢ�б���
	struct HashTableArImpl {
		int TableSize; /* �����󳤶� */
		Cell *Cells;   /* ���ɢ�е�Ԫ���ݵ����� */
	};

	typedef Cell *HashTableArrayListPosition;     /* ��������λ����ָ��ɢ�н���ָ�� */


	Index Hash(ElementType Key, int TableSize) {
		return Key % TableSize;
	}

	HashTableArPtr CreateArrayListHashTable(int TableSize) {
		HashTableArPtr H;
		int i;

		H = (HashTableArPtr)malloc(sizeof(struct HashTableArImpl));
		/* ��֤ɢ�б���󳤶������� */
		H->TableSize = NextPrime(TableSize);
		/* ������Ԫ���� */
		H->Cells = (Cell *)malloc(H->TableSize * sizeof(Cell));
		/* ��ʼ����Ԫ״̬Ϊ���յ�Ԫ�� */
		for (i = 0; i < H->TableSize; i++)
			H->Cells[i].Status = Empty;

		return H;
	}

	/*��*/
	HashTableArrayListPosition Find(HashTableArPtr H, ElementType Key) {
		HashTableArrayListPosition CurrentPos, NewPos;
		// ��¼��ͻ����
		int CNum = 0;

		// ��ʼɢ��λ��
		NewPos = CurrentPos = H->Cells + Hash(Key, H->TableSize);
		/* ����λ�õĵ�Ԫ�ǿգ����Ҳ���Ҫ�ҵ�Ԫ��ʱ��������ͻ */
		while (NewPos->Status != Empty && NewPos->Data != Key) {
			/* �ַ������͵Ĺؼ�����Ҫ strcmp ����!! */
			/* ͳ��1�γ�ͻ�����ж���ż�� */
			if (++CNum % 2) { /* �����γ�ͻ */
				NewPos = CurrentPos + (CNum + 1)*(CNum + 1) / 4; /* ����Ϊ+[(CNum+1)/2]^2 */
				Index NewIndex = NewPos - H->Cells;
				if (NewIndex >= H->TableSize)
					NewPos = H->Cells + NewIndex % H->TableSize; /* ����Ϊ�Ϸ���ַ */
			}
			else { /* ż���γ�ͻ */
				NewPos = CurrentPos - CNum * CNum / 4; /* ����Ϊ-(CNum/2)^2 */
				while (NewPos < 0)
					NewPos += H->TableSize; /* ����Ϊ�Ϸ���ַ */
			}
		}
		return NewPos; /* ��ʱNewPos������Key��λ�ã�������һ���յ�Ԫ��λ�ã���ʾ�Ҳ�����*/
	}

	bool Insert(HashTableArPtr H, ElementType Key) {
		HashTableArrayListPosition Pos = Find(H, Key); /* �ȼ��Key�Ƿ��Ѿ����� */

		if (Pos->Status != Legitimate) { /* ��������Ԫû�б�ռ��˵��Key���Բ����ڴ� */
			Pos->Status = Legitimate;
			Pos->Data = Key;
			/*�ַ������͵Ĺؼ�����Ҫ strcpy ����!! */
			return true;
		}
		else {
			printf("��ֵ�Ѵ���");
			return false;
		}
	}

	int mainForArrayListHashTable() {
		// ����cpp�ļ����Դ���ͬ������(.h�ǿ϶�����), �����ܴ���ͬ����������޷��������صķ���
		// (�п��ܻ���ֱ���ͨ����DBUG���ͻ���, ������Ҫ��������һ��)
		HashTableArPtr h = CreateArrayListHashTable(20);
		srand(clock());
		// ����û�д���ɢ�г�ͻ �޷���Ӷ��ͬHashֵ��Key ��ֻ�����20��Ԫ��
		for (int i = 0; i < 20; ++i) {
			Insert(h, i);
		}
		printf("%d", Find(h, 3)->Data);
		return 0;
	}

}
