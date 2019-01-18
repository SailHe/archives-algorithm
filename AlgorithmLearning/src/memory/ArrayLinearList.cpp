#include "ArrayLinearList.h"
namespace LinearList {
	int mainForArrayLinearList() {
		freopen("./InputForArrayLinearList.txt", "r", stdin);
		ArrayLinearList L;
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
	
	ArrayLinearList MakeEmpty() {
		ArrayLinearList L;
		L = (ArrayLinearList)malloc(sizeof(struct ArrayListNode));
		L->Last = -1;//置空
		return L;
	}

	Position Find(ArrayLinearList L, ElementType X) {
		for (int i = 0; i <= L->Last; i++) {
			if (L->Data[i] == X)
				return i;
		}
		return ARRAY_LINEAR_LIST_ERROR_POSITION;
	}
	
	bool Insert(ArrayLinearList L, ElementType X, Position P) {
		if (L->Last + 1 == ARRAY_LINEAR_LIST_MAX_SIZE) {
			printf("FULL");
			return false;
		}
		if (0 <= P && P <= L->Last + 1) {//合法插入位置[0, last+1]
			for (int i = L->Last; i >= P; --i) {
				L->Data[i + 1] = L->Data[i];//在到达插入位置前将所有元素后移
			}
			L->Data[P] = X;
			++L->Last;
			return true;
		}
		else {
			printf("ILLEGAL POSITION");
			// fprintf(stderr, "ILLEGAL POSITION: %d\n", P);
			return false;
		}
	}
	
	bool Delete(ArrayLinearList L, Position P) {
		if (0 <= P && P < L->Last + 1) { //删除位置必须是[0,Last]之间 
			for (int i = P; i < L->Last; ++i) {
				L->Data[i] = L->Data[i + 1];//将删除位置后的所有元素前移
			}
			--L->Last;
			return true;
		}
		else {
			printf("POSITION %d EMPTY", P);
			return false;
		}
	}

	int DeleteAll(int A[], int L, int minA, int maxA) {
		int sub = -1;
		for (int i = 0; i < L; ++i) {
			if (minA >= A[i] || A[i] >= maxA) {//前移不满足删除条件的元素
				A[++sub] = A[i];
			}
		}
		return sub + 1;
	}
}
