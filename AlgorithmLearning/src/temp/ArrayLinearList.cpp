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
	//创建并返回一个空的线性表；
	List MakeEmpty() {
		List L;
		L = (List)malloc(sizeof(struct LNode));
		L->Last = -1;//置空
		return L;
	}
	//返回线性表中X的位置。若找不到则返回ARRAY_LINEAR_LIST_ERROR_POSITION；
	Position Find(List L, ElementType X) {
		for (int i = 0; i <= L->Last; i++) {
			if (L->Data[i] == X)
				return i;
		}
		return ARRAY_LINEAR_LIST_ERROR_POSITION;
	}
	//将X插入在位置P并返回true。
	//若空间已满，则打印“FULL”并返回false；
	//如果参数P指向非法位置，则打印“ILLEGAL POSITION”并返回false；
	bool Insert(List L, ElementType X, Position P) {
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
			return false;
		}
	}
	//将位置P的元素删除并返回true。
	//若参数P指向非法位置，则打印“POSITION P EMPTY”（其中P是参数值）并返回false。
	bool Delete(List L, Position P) {
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

	/*删除所有值大于min而且小于max的元素(min, max)。删除后表中剩余元素保持顺序存储，并且相对位置不能改变。*/
	int deleteAll(int A[], int L, int minA, int maxA) {
		int sub = -1;
		for (int i = 0; i < L; ++i) {
			if (minA >= A[i] || A[i] >= maxA) {//前移不满足删除条件的元素
				A[++sub] = A[i];
			}
		}
		return sub + 1;
	}
}
