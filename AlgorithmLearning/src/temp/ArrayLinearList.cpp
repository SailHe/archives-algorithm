#include "ArrayLinearList.h"
namespace LinearList {
	int mainForArrayLinearList() {
		freopen("./InputForArrayLinearList.txt", "r", stdin);
		List L;
		ElementType X;
		Position P;
		int N;

		L = MakeEmpty();
		scanf("%d", &N);//6
		while (N--) {
			scanf("%d", &X);//1 2 3 4 5 6
			if (Insert(L, X, 0) == false)
				printf(" Insertion Error: %d is not in.\n", X);//6
		}
		scanf("%d", &N);//3
		while (N--) {
			scanf("%d", &X);//6 5 1
			P = Find(L, X);
			if (P == ARRAY_LINEAR_LIST_ERROR_POSITION)
				printf("Finding Error: %d is not in.\n", X);//6
			else
				printf("%d is at position %d.\n", X, P);//5(0) 1(4)
		}
		scanf("%d", &N);//2
		while (N--) {
			scanf("%d", &P);//-1 6
			if (Delete(L, P) == false)
				printf(" Deletion Error.\n");//-1
			if (Insert(L, 0, P) == false)
				printf(" Insertion Error: 0 is not in.\n");
		}
		return 0;
	}
	//����������һ���յ����Ա�
	List MakeEmpty() {
		List L;
		L = (List)malloc(sizeof(struct LNode));
		L->Last = -1;//�ÿ�
		return L;
	}
	//�������Ա���X��λ�á����Ҳ����򷵻�ARRAY_LINEAR_LIST_ERROR_POSITION��
	Position Find(List L, ElementType X) {
		for (int i = 0; i <= L->Last; i++) {
			if (L->Data[i] == X)
				return i;
		}
		return ARRAY_LINEAR_LIST_ERROR_POSITION;
	}
	//��X������λ��P������true��
	//���ռ����������ӡ��FULL��������false��
	//�������Pָ��Ƿ�λ�ã����ӡ��ILLEGAL POSITION��������false��
	bool Insert(List L, ElementType X, Position P) {
		if (L->Last + 1 == ARRAY_LINEAR_LIST_MAX_SIZE) {
			printf("FULL");
			return false;
		}
		if (0 <= P && P <= L->Last + 1) {//�Ϸ�����λ��[0, last+1]
			for (int i = L->Last; i >= P; --i) {
				L->Data[i + 1] = L->Data[i];//�ڵ������λ��ǰ������Ԫ�غ���
			}
			L->Data[P] = X;
			++L->Last;
			return true;
		}
		else {
			printf("ILLEGAL POSITION");
			return false;
		}
	}
	//��λ��P��Ԫ��ɾ��������true��
	//������Pָ��Ƿ�λ�ã����ӡ��POSITION P EMPTY��������P�ǲ���ֵ��������false��
	bool Delete(List L, Position P) {
		if (0 <= P && P < L->Last + 1) { //ɾ��λ�ñ�����[0,Last]֮�� 
			for (int i = P; i < L->Last; ++i) {
				L->Data[i] = L->Data[i + 1];//��ɾ��λ�ú������Ԫ��ǰ��
			}
			--L->Last;
			return true;
		}
		else {
			printf("POSITION %d EMPTY", P);
			return false;
		}
	}

	/*ɾ������ֵ����min����С��max��Ԫ��(min, max)��ɾ�������ʣ��Ԫ�ر���˳��洢���������λ�ò��ܸı䡣*/
	int deleteAll(int A[], int L, int minA, int maxA) {
		int sub = -1;
		for (int i = 0; i < L; ++i) {
			if (minA >= A[i] || A[i] >= maxA) {//ǰ�Ʋ�����ɾ��������Ԫ��
				A[++sub] = A[i];
			}
		}
		return sub + 1;
	}
}
