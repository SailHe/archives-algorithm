#include "HashTable.h"
namespace HashTableC {
	/*
①
这个实现没有解决散列冲突 仅作例子用
*/
	typedef int ElementType;    /* 关键词类型用整型 */
	/* 散列单元状态类型，分别对应：有合法元素、空单元、有已删除元素 */
	typedef enum { Legitimate, Empty, Deleted } EntryStatusType;

	typedef struct HashEntry Cell; /* 散列表单元类型 */
	struct HashEntry {
		ElementType Data; /* 存放元素 */
		EntryStatusType Status;   /* 单元状态 */
	};

	typedef struct HashTableArImpl *HashTableArPtr; /* 散列表类型 */
	// 散列表定义
	struct HashTableArImpl {
		int TableSize; /* 表的最大长度 */
		Cell *Cells;   /* 存放散列单元数据的数组 */
	};

	typedef Cell *HashTableArrayListPosition;     /* 数据所在位置是指向散列结点的指针 */


	Index Hash(ElementType Key, int TableSize) {
		return Key % TableSize;
	}

	HashTableArPtr CreateArrayListHashTable(int TableSize) {
		HashTableArPtr H;
		int i;

		H = (HashTableArPtr)malloc(sizeof(struct HashTableArImpl));
		/* 保证散列表最大长度是素数 */
		H->TableSize = NextPrime(TableSize);
		/* 声明单元数组 */
		H->Cells = (Cell *)malloc(H->TableSize * sizeof(Cell));
		/* 初始化单元状态为“空单元” */
		for (i = 0; i < H->TableSize; i++)
			H->Cells[i].Status = Empty;

		return H;
	}

	/*②*/
	HashTableArrayListPosition Find(HashTableArPtr H, ElementType Key) {
		HashTableArrayListPosition CurrentPos, NewPos;
		// 记录冲突次数
		int CNum = 0;

		// 初始散列位置
		NewPos = CurrentPos = H->Cells + Hash(Key, H->TableSize);
		/* 当该位置的单元非空，并且不是要找的元素时，发生冲突 */
		while (NewPos->Status != Empty && NewPos->Data != Key) {
			/* 字符串类型的关键词需要 strcmp 函数!! */
			/* 统计1次冲突，并判断奇偶次 */
			if (++CNum % 2) { /* 奇数次冲突 */
				NewPos = CurrentPos + (CNum + 1)*(CNum + 1) / 4; /* 增量为+[(CNum+1)/2]^2 */
				Index NewIndex = NewPos - H->Cells;
				if (NewIndex >= H->TableSize)
					NewPos = H->Cells + NewIndex % H->TableSize; /* 调整为合法地址 */
			}
			else { /* 偶数次冲突 */
				NewPos = CurrentPos - CNum * CNum / 4; /* 增量为-(CNum/2)^2 */
				while (NewPos < 0)
					NewPos += H->TableSize; /* 调整为合法地址 */
			}
		}
		return NewPos; /* 此时NewPos或者是Key的位置，或者是一个空单元的位置（表示找不到）*/
	}

	bool Insert(HashTableArPtr H, ElementType Key) {
		HashTableArrayListPosition Pos = Find(H, Key); /* 先检查Key是否已经存在 */

		if (Pos->Status != Legitimate) { /* 如果这个单元没有被占，说明Key可以插入在此 */
			Pos->Status = Legitimate;
			Pos->Data = Key;
			/*字符串类型的关键词需要 strcpy 函数!! */
			return true;
		}
		else {
			printf("键值已存在");
			return false;
		}
	}

	int mainForArrayListHashTable() {
		// 两个cpp文件可以存在同名类型(.h是肯定不行), 但不能存在同样定义或是无法区分重载的方法
		// (有可能会出现编译通过但DBUG类型混乱, 可能需要重新生成一下)
		HashTableArPtr h = CreateArrayListHashTable(20);
		srand(clock());
		// 由于没有处理散列冲突 无法添加多个同Hash值的Key 故只能添加20个元素
		for (int i = 0; i < 20; ++i) {
			Insert(h, i);
		}
		printf("%d", Find(h, 3)->Data);
		return 0;
	}

}
