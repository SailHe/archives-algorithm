/* 邻接矩阵存储 - Prim最小生成树算法 */
Vertex FindMinDist(MGraph Graph, WeightType dist[])
{ /* 返回未被收录顶点中dist最小者 */
	Vertex MinV, V;
	WeightType MinDist = INFINITY;

	for (V = 0; V<Graph->Nv; V++) {
		if (dist[V] != 0 && dist[V]<MinDist) {
			/* 若V未被收录，且dist[V]更小 */
			MinDist = dist[V]; /* 更新最小距离 */
			MinV = V; /* 更新对应顶点 */
		}
	}
	if (MinDist < INFINITY) /* 若找到最小dist */
		return MinV; /* 返回对应的顶点下标 */
	else return ERROR;  /* 若这样的顶点不存在，返回-1作为标记 */
}
int Prim(MGraph Graph, LGraph MST)
{ /* 将最小生成树保存为邻接表存储的图MST，返回最小权重和 */
	WeightType dist[MaxVertexNum], TotalWeight;
	Vertex parent[MaxVertexNum], V, W;
	int VCount;
	Edge E;

	/* 初始化。默认初始点下标是0 */
	for (V = 0; V<Graph->Nv; V++) {
		/* 这里假设若V到W没有直接的边，则Graph->G[V][W]定义为INFINITY */
		dist[V] = Graph->G[0][V];
		parent[V] = 0; /* 暂且定义所有顶点的父结点都是初始点0 */
	}
	TotalWeight = 0; /* 初始化权重和     */
	VCount = 0;      /* 初始化收录的顶点数 */
	/* 创建包含所有顶点但没有边的图。注意用邻接表版本 */
	MST = CreateGraph(Graph->Nv);
	E = (Edge)malloc(sizeof(struct ENode)); /* 建立空的边结点 */

	/* 将初始点0收录进MST */
	dist[0] = 0;
	VCount++;
	parent[0] = -1; /* 当前树根是0 */

	while (1) {
		V = FindMinDist(Graph, dist);
		/* V = 未被收录顶点中dist最小者 */
		if (V == ERROR) /* 若这样的V不存在 */
			break;   /* 算法结束 */

		/* 将V及相应的边<parent[V], V>收录进MST */
		E->V1 = parent[V];
		E->V2 = V;
		E->Weight = dist[V];
		InsertEdge(MST, E);
		TotalWeight += dist[V];
		dist[V] = 0;
		VCount++;

		for (W = 0; W<Graph->Nv; W++) /* 对图中的每个顶点W */
		if (dist[W] != 0 && Graph->G[V][W]<INFINITY) {
			/* 若W是V的邻接点并且未被收录 */
			if (Graph->G[V][W] < dist[W]) {
				/* 若收录V使得dist[W]变小 */
				dist[W] = Graph->G[V][W]; /* 更新dist[W] */
				parent[W] = V; /* 更新树 */
			}
		}
	} /* while结束*/
	if (VCount < Graph->Nv) /* MST中收的顶点不到|V|个 */
		TotalWeight = ERROR;
	return TotalWeight;   /* 算法执行完毕，返回最小权重和或错误标记 */
}

/* 邻接表存储 - Kruskal最小生成树算法 */

/*-------------------- 顶点并查集定义 --------------------*/
typedef int Vertex;
typedef Vertex ElementType; /* 默认元素可以用非负整数表示 */
typedef Vertex SetName;     /* 默认用根结点的下标作为集合名称 */
typedef ElementType SetType[MaxVertexNum]; /* 假设集合元素下标从0开始 */
void InitializeVSet(SetType S, int N)
{ /* 初始化并查集 */
	ElementType X;

	for (X = 0; X<N; X++) S[X] = -1;
}
/* 返回是否执行过合并操作 */
bool Union(SetType S, SetName Root1, SetName Root2){
	if (Root1 == Root2)
		return false;
	/* 保证小集合并入大集合 */
	if (S[Root2] < S[Root1]) { /* 如果集合2比较大 */
		S[Root2] += S[Root1];     /* 集合1并入集合2  */
		S[Root1] = Root2;
	}
	else {                         /* 如果集合1比较大 */
		S[Root1] += S[Root2];     /* 集合2并入集合1  */
		S[Root2] = Root1;
	}
	return true;
}
SetName Find(SetType S, ElementType X)
{ /* 默认集合元素全部初始化为-1 */
	if (S[X] < 0) /* 找到集合的根 */
		return X;
	else
		return S[X] = Find(S, S[X]); /* 路径压缩 */
}
bool CheckCycle(SetType VSet, Vertex V1, Vertex V2)
{ /* 检查连接V1和V2的边是否在现有的最小生成树子集中构成回路 */
	Vertex Root1, Root2;

	Root1 = Find(VSet, V1); /* 得到V1所属的连通集名称 */
	Root2 = Find(VSet, V2); /* 得到V2所属的连通集名称 */

	if (Root1 == Root2) /* 若V1和V2已经连通，则该边不能要 */
		return false;
	else { /* 否则该边可以被收集，同时将V1和V2并入同一连通集 */
		Union(VSet, Root1, Root2);
		return true;
	}
}
/*-------------------- 并查集定义结束 --------------------*/

/*-------------------- 边的最小堆定义 --------------------*/
void PercDown(Edge ESet, int p, int N)
{ /* 改编代码4.24的PercDown( MaxHeap H, int p )    */
	/* 将N个元素的边数组中以ESet[p]为根的子堆调整为关于Weight的最小堆 */
	int Parent, Child;
	struct ENode X;

	X = ESet[p]; /* 取出根结点存放的值 */
	for (Parent = p; (Parent * 2 + 1)<N; Parent = Child) {
		Child = Parent * 2 + 1;
		if ((Child != N - 1) && (ESet[Child].Weight>ESet[Child + 1].Weight))
			Child++;  /* Child指向左右子结点的较小者 */
		if (X.Weight <= ESet[Child].Weight) break; /* 找到了合适位置 */
		else  /* 下滤X */
			ESet[Parent] = ESet[Child];
	}
	ESet[Parent] = X;
}
void InitializeESet(LGraph Graph, Edge ESet)
{ /* 将图的边存入数组ESet，并且初始化为最小堆 */
	Vertex V;
	PtrToAdjVNode W;
	int ECount;

	/* 将图的边存入数组ESet */
	ECount = 0;
	for (V = 0; V<Graph->Nv; V++)
	for (W = Graph->G[V].FirstEdge; W; W = W->Next)
	if (V < W->AdjV) { /* 避免重复录入无向图的边，只收V1<V2的边 */
		ESet[ECount].V1 = V;
		ESet[ECount].V2 = W->AdjV;
		ESet[ECount++].Weight = W->Weight;
	}
	/* 初始化为最小堆 */
	for (ECount = Graph->Ne / 2; ECount >= 0; ECount--)
		PercDown(ESet, ECount, Graph->Ne);
}
int GetEdge(Edge ESet, int CurrentSize)
{ /* 给定当前堆的大小CurrentSize，将当前最小边位置弹出并调整堆 */

	/* 将最小边与当前堆的最后一个位置的边交换 */
	Swap(&ESet[0], &ESet[CurrentSize - 1]);
	/* 将剩下的边继续调整成最小堆 */
	PercDown(ESet, 0, CurrentSize - 1);

	return CurrentSize - 1; /* 返回最小边所在位置 */
}
/*-------------------- 最小堆定义结束 --------------------*/
int Kruskal(LGraph Graph, LGraph MST)
{ /* 将最小生成树保存为邻接表存储的图MST，返回最小权重和 */
	WeightType TotalWeight;
	int ECount, NextEdge;
	SetType VSet; /* 顶点数组 */
	Edge ESet;    /* 边数组 */

	InitializeVSet(VSet, Graph->Nv); /* 初始化顶点并查集 */
	ESet = (Edge)malloc(sizeof(struct ENode)*Graph->Ne);
	InitializeESet(Graph, ESet); /* 初始化边的最小堆 */
	/* 创建包含所有顶点但没有边的图。注意用邻接表版本 */
	MST = CreateGraph(Graph->Nv);
	TotalWeight = 0; /* 初始化权重和     */
	ECount = 0;      /* 初始化收录的边数 */

	NextEdge = Graph->Ne; /* 原始边集的规模 */
	while (ECount < Graph->Nv - 1) {  /* 当收集的边不足以构成树时 */
		NextEdge = GetEdge(ESet, NextEdge); /* 从边集中得到最小边的位置 */
		if (NextEdge < 0) /* 边集已空 */
			break;
		/* 如果该边的加入不构成回路，即两端结点不属于同一连通集 */
		if (CheckCycle(VSet, ESet[NextEdge].V1, ESet[NextEdge].V2) == true) {
			/* 将该边插入MST */
			InsertEdge(MST, ESet + NextEdge);
			TotalWeight += ESet[NextEdge].Weight; /* 累计权重 */
			ECount++; /* 生成树中边数加1 */
		}
	}
	if (ECount < Graph->Nv - 1)
		TotalWeight = -1; /* 设置错误标记，表示生成树不存在 */

	return TotalWeight;
}
/*end*/

/* 邻接表存储 - 拓扑排序算法 */
bool TopSort(LGraph Graph, Vertex TopOrder[])
{ /* 对Graph进行拓扑排序,  TopOrder[]顺序存储排序后的顶点下标 */
	int Indegree[MaxVertexNum], cnt;
	Vertex V;
	PtrToAdjVNode W;
	Queue Q = CreateQueue(Graph->Nv);

	/* 初始化Indegree[] */
	for (V = 0; V<Graph->Nv; V++)
		Indegree[V] = 0;

	/* 遍历图，得到Indegree[] */
	for (V = 0; V<Graph->Nv; V++)
	for (W = Graph->G[V].FirstEdge; W; W = W->Next)
		Indegree[W->AdjV]++; /* 对有向边<V, W->AdjV>累计终点的入度 */

	/* 将所有入度为0的顶点入列 */
	for (V = 0; V<Graph->Nv; V++)
	if (Indegree[V] == 0)
		AddQ(Q, V);

	/* 下面进入拓扑排序 */
	cnt = 0;
	while (!IsEmpty(Q)){
		V = DeleteQ(Q); /* 弹出一个入度为0的顶点 */
		TopOrder[cnt++] = V; /* 将之存为结果序列的下一个元素 */
		/* 对V的每个邻接点W->AdjV */
		for (W = Graph->G[V].FirstEdge; W; W = W->Next)
		if (--Indegree[W->AdjV] == 0)/* 若删除V使得W->AdjV入度为0 */
			AddQ(Q, W->AdjV); /* 则该顶点入列 */
	} /* while结束*/

	if (cnt != Graph->Nv)
		return false; /* 说明图中有回路, 返回不成功标志 */
	else
		return true;
}

/*插入排序*/
void InsertionSort(ElementType A[], int N)
{ /* 插入排序 */
	int P, i;
	ElementType Tmp;

	for (P = 1; P<N; P++) {
		Tmp = A[P]; /* 取出未排序序列中的第一个元素*/
		for (i = P; i>0 && A[i - 1]>Tmp; i--)
			A[i] = A[i - 1]; /*依次与已排序序列中元素比较并右移*/
		A[i] = Tmp; /* 放进合适的位置 */
	}
}
/*希尔排序*/
void ShellSort(ElementType A[], int N)
{ /* 希尔排序 - 用Sedgewick增量序列 */
	int Si, D, P, i;
	ElementType Tmp;
	/* 这里只列出一小部分增量 */
	int Sedgewick[] = { 929, 505, 209, 109, 41, 19, 5, 1, 0 };

	for (Si = 0; Sedgewick[Si] >= N; Si++)
		; /* 初始的增量Sedgewick[Si]不能超过待排序列长度 */

	for (D = Sedgewick[Si]; D>0; D = Sedgewick[++Si])
	for (P = D; P<N; P++) { /* 插入排序*/
		Tmp = A[P];
		for (i = P; i >= D && A[i - D]>Tmp; i -= D)
			A[i] = A[i - D];
		A[i] = Tmp;
	}
}
/*堆排序*/
void Swap(ElementType *a, ElementType *b)
{
	ElementType t = *a; *a = *b; *b = t;
}
void PercDown(ElementType A[], int p, int N)
{ /* 改编代码4.24的PercDown( MaxHeap H, int p )    */
	/* 将N个元素的数组中以A[p]为根的子堆调整为最大堆 */
	int Parent, Child;
	ElementType X;

	X = A[p]; /* 取出根结点存放的值 */
	for (Parent = p; (Parent * 2 + 1)<N; Parent = Child) {
		Child = Parent * 2 + 1;
		if ((Child != N - 1) && (A[Child]<A[Child + 1]))
			Child++;  /* Child指向左右子结点的较大者 */
		if (X >= A[Child]) break; /* 找到了合适位置 */
		else  /* 下滤X */
			A[Parent] = A[Child];
	}
	A[Parent] = X;
}
void HeapSort(ElementType A[], int N)
{ /* 堆排序 */
	int i;

	for (i = N / 2 - 1; i >= 0; i--)/* 建立最大堆 */
		PercDown(A, i, N);

	for (i = N - 1; i>0; i--) {
		/* 删除最大堆顶 */
		Swap(&A[0], &A[i]); /* 见代码7.1 */
		PercDown(A, 0, i);
	}
}

/* 归并排序 - 递归实现 */
/* L = 左边起始位置, R = 右边起始位置, RightEnd = 右边终点位置*/
void Merge(ElementType A[], ElementType TmpA[], int L, int R, int RightEnd)
{ /* 将有序的A[L]~A[R-1]和A[R]~A[RightEnd]归并成一个有序序列 */
	int LeftEnd, NumElements, Tmp;
	int i;

	LeftEnd = R - 1; /* 左边终点位置 */
	Tmp = L;         /* 有序序列的起始位置 */
	NumElements = RightEnd - L + 1;

	while (L <= LeftEnd && R <= RightEnd) {
		if (A[L] <= A[R])
			TmpA[Tmp++] = A[L++]; /* 将左边元素复制到TmpA */
		else
			TmpA[Tmp++] = A[R++]; /* 将右边元素复制到TmpA */
	}

	while (L <= LeftEnd)
		TmpA[Tmp++] = A[L++]; /* 直接复制左边剩下的 */
	while (R <= RightEnd)
		TmpA[Tmp++] = A[R++]; /* 直接复制右边剩下的 */

	for (i = 0; i < NumElements; i++, RightEnd--)
		A[RightEnd] = TmpA[RightEnd]; /* 将有序的TmpA[]复制回A[] */
}
void Msort(ElementType A[], ElementType TmpA[], int L, int RightEnd)
{ /* 核心递归排序函数 */
	int Center;

	if (L < RightEnd) {
		Center = (L + RightEnd) / 2;
		Msort(A, TmpA, L, Center);              /* 递归解决左边 */
		Msort(A, TmpA, Center + 1, RightEnd);     /* 递归解决右边 */
		Merge(A, TmpA, L, Center + 1, RightEnd);  /* 合并两段有序序列 */
	}
}
void MergeSort(ElementType A[], int N)
{ /* 归并排序 */
	ElementType *TmpA;
	TmpA = (ElementType *)malloc(N*sizeof(ElementType));

	if (TmpA != NULL) {
		Msort(A, TmpA, 0, N - 1);
		free(TmpA);
	}
	else printf("空间不足");
}

/* 归并排序 - 循环实现 */
/* 这里Merge函数在递归版本中给出 */
/* length = 当前有序子列的长度*/
void Merge_pass(ElementType A[], ElementType TmpA[], int N, int length)
{ /* 两两归并相邻有序子列 */
	int i, j;

	for (i = 0; i <= N - 2 * length; i += 2 * length)
		Merge(A, TmpA, i, i + length, i + 2 * length - 1);
	if (i + length < N) /* 归并最后2个子列*/
		Merge(A, TmpA, i, i + length, N - 1);
	else /* 最后只剩1个子列*/
	for (j = i; j < N; j++) TmpA[j] = A[j];
}
void Merge_Sort(ElementType A[], int N)
{
	int length;
	ElementType *TmpA;

	length = 1; /* 初始化子序列长度*/
	TmpA = malloc(N * sizeof(ElementType));
	if (TmpA != NULL) {
		while (length < N) {
			Merge_pass(A, TmpA, N, length);
			length *= 2;
			Merge_pass(TmpA, A, N, length);
			length *= 2;
		}
		free(TmpA);
	}
	else printf("空间不足");
}

/* 快速排序 - 直接调用库函数 */
#include <stdlib.h>
/*---------------简单整数排序--------------------*/
int compare(const void *a, const void *b)
{ /* 比较两整数。非降序排列 */
	return (*(int*)a - *(int*)b);
}
/* 调用接口 */
qsort(A, N, sizeof(int), compare);
/*---------------简单整数排序--------------------*/
/*--------------- 一般情况下，对结构体Node中的某键值key排序 ---------------*/
struct Node {
	int key1, key2;
} A[MAXN];
int compare2keys(const void *a, const void *b)
{ /* 比较两种键值：按key1非升序排列；如果key1相等，则按key2非降序排列 */
	int k;
	if (((const struct Node*)a)->key1 < ((const struct Node*)b)->key1)
		k = 1;
	else if (((const struct Node*)a)->key1 >((const struct Node*)b)->key1)
		k = -1;
	else { /* 如果key1相等 */
		if (((const struct Node*)a)->key2 < ((const struct Node*)b)->key2)
			k = -1;
		else
			k = 1;
	}
	return k;
}
/* 调用接口 */
qsort(A, N, sizeof(struct Node), compare2keys);
/*--------------- 一般情况下，对结构体Node中的某键值key排序 ---------------*/

/*②*/
/* 快速排序 */
ElementType Median3(ElementType A[], int Left, int Right)
{
	int Center = (Left + Right) / 2;
	if (A[Left] > A[Center])
		Swap(&A[Left], &A[Center]);
	if (A[Left] > A[Right])
		Swap(&A[Left], &A[Right]);
	if (A[Center] > A[Right])
		Swap(&A[Center], &A[Right]);
	/* 此时A[Left] <= A[Center] <= A[Right] */
	Swap(&A[Center], &A[Right - 1]); /* 将基准Pivot藏到右边*/
	/* 只需要考虑A[Left+1] … A[Right-2] */
	return  A[Right - 1];  /* 返回基准Pivot */
}
void Qsort(ElementType A[], int Left, int Right)
{ /* 核心递归函数 */
	int Pivot, Cutoff, Low, High;

	if (Cutoff <= Right - Left) { /* 如果序列元素充分多，进入快排 */
		Pivot = Median3(A, Left, Right); /* 选基准 */
		Low = Left; High = Right - 1;
		while (1) { /*将序列中比基准小的移到基准左边，大的移到右边*/
			while (A[++Low] < Pivot);
			while (A[--High] > Pivot);
			if (Low < High) Swap(&A[Low], &A[High]);
			else break;
		}
		Swap(&A[Low], &A[Right - 1]);   /* 将基准换到正确的位置 */
		Qsort(A, Left, Low - 1);    /* 递归解决左边 */
		Qsort(A, Low + 1, Right);   /* 递归解决右边 */
	}
	else InsertionSort(A + Left, Right - Left + 1); /* 元素太少，用简单排序 */
}
void QuickSort(ElementType A[], int N)
{ /* 统一接口 */
	Qsort(A, 0, N - 1);
}


/*10.3基数排序*/

/* ①基数排序 - 次位优先 */
/* 假设元素最多有MaxDigit个关键字，基数全是同样的Radix */
#define MaxDigit 4
#define Radix 10
/* 桶元素结点 */
typedef struct Node *PtrToNode;
struct Node {
	int key;
	PtrToNode next;
};
/* 桶头结点 */
struct HeadNode {
	PtrToNode head, tail;
};
typedef struct HeadNode Bucket[Radix];
int GetDigit(int X, int D)
{ /* 默认次位D=1, 主位D<=MaxDigit */
	int d, i;

	for (i = 1; i <= D; i++) {
		d = X % Radix;
		X /= Radix;
	}
	return d;
}
void LSDRadixSort(ElementType A[], int N)
{ /* 基数排序 - 次位优先 */
	int D, Di, i;
	Bucket B;
	PtrToNode tmp, p, List = NULL;

	for (i = 0; i<Radix; i++) /* 初始化每个桶为空链表 */
		B[i].head = B[i].tail = NULL;
	for (i = 0; i<N; i++) { /* 将原始序列逆序存入初始链表List */
		tmp = (PtrToNode)malloc(sizeof(struct Node));
		tmp->key = A[i];
		tmp->next = List;
		List = tmp;
	}
	/* 下面开始排序 */
	for (D = 1; D <= MaxDigit; D++) { /* 对数据的每一位循环处理 */
		/* 下面是分配的过程 */
		p = List;
		while (p) {
			Di = GetDigit(p->key, D); /* 获得当前元素的当前位数字 */
			/* 从List中摘除 */
			tmp = p; p = p->next;
			/* 插入B[Di]号桶尾 */
			tmp->next = NULL;
			if (B[Di].head == NULL)
				B[Di].head = B[Di].tail = tmp;
			else {
				B[Di].tail->next = tmp;
				B[Di].tail = tmp;
			}
		}
		/* 下面是收集的过程 */
		List = NULL;
		for (Di = Radix - 1; Di >= 0; Di--) { /* 将每个桶的元素顺序收集入List */
			if (B[Di].head) { /* 如果桶不为空 */
				/* 整桶插入List表头 */
				B[Di].tail->next = List;
				List = B[Di].head;
				B[Di].head = B[Di].tail = NULL; /* 清空桶 */
			}
		}
	}
	/* 将List倒入A[]并释放空间 */
	for (i = 0; i<N; i++) {
		tmp = List;
		List = List->next;
		A[i] = tmp->key;
		free(tmp);
	}
}

/* ②基数排序 - 主位优先 */
/* 假设元素最多有MaxDigit个关键字，基数全是同样的Radix */
#define MaxDigit 4
#define Radix 10
/* 桶元素结点 */
typedef struct Node *PtrToNode;
struct Node{
	int key;
	PtrToNode next;
};
/* 桶头结点 */
struct HeadNode {
	PtrToNode head, tail;
};
typedef struct HeadNode Bucket[Radix];
int GetDigit(int X, int D)
{ /* 默认次位D=1, 主位D<=MaxDigit */
	int d, i;

	for (i = 1; i <= D; i++) {
		d = X%Radix;
		X /= Radix;
	}
	return d;
}
void MSD(ElementType A[], int L, int R, int D)
{ /* 核心递归函数: 对A[L]...A[R]的第D位数进行排序 */
	int Di, i, j;
	Bucket B;
	PtrToNode tmp, p, List = NULL;
	if (D == 0) return; /* 递归终止条件 */

	for (i = 0; i<Radix; i++) /* 初始化每个桶为空链表 */
		B[i].head = B[i].tail = NULL;
	for (i = L; i <= R; i++) { /* 将原始序列逆序存入初始链表List */
		tmp = (PtrToNode)malloc(sizeof(struct Node));
		tmp->key = A[i];
		tmp->next = List;
		List = tmp;
	}
	/* 下面是分配的过程 */
	p = List;
	while (p) {
		Di = GetDigit(p->key, D); /* 获得当前元素的当前位数字 */
		/* 从List中摘除 */
		tmp = p; p = p->next;
		/* 插入B[Di]号桶 */
		if (B[Di].head == NULL) B[Di].tail = tmp;
		tmp->next = B[Di].head;
		B[Di].head = tmp;
	}
	/* 下面是收集的过程 */
	i = j = L; /* i, j记录当前要处理的A[]的左右端下标 */
	for (Di = 0; Di<Radix; Di++) { /* 对于每个桶 */
		if (B[Di].head) { /* 将非空的桶整桶倒入A[], 递归排序 */
			p = B[Di].head;
			while (p) {
				tmp = p;
				p = p->next;
				A[j++] = tmp->key;
				free(tmp);
			}
			/* 递归对该桶数据排序, 位数减1 */
			MSD(A, i, j - 1, D - 1);
			i = j; /* 为下一个桶对应的A[]左端 */
		}
	}
}
void MSDRadixSort(ElementType A[], int N)
{ /* 统一接口 */
	MSD(A, 0, N - 1, MaxDigit);
}