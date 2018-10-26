//跟Graph.h没有直接关系

#include "../Queue.h"
#define MaxVertexNum 100/*最大顶点数*/
#define INFINTY 65535/*无穷大<<双字节无符号整数的最大值*/
#define MAP_MAX_SIZE 128/*无穷大<<双字节无符号整数的最大值*/
typedef int Vertex;/*顶点的下标表示顶点*/
typedef int WeightType;/*边的权值*/
typedef char DataType;/*顶点储存类型*/
Vertex Visited[MaxVertexNum];/*初始化为false*/

/*边的定义*/
typedef struct ENode{
	Vertex V1, V2;/*有向边<V1, V2>*/
	WeightType Weight;/*权重*/
}*PtrToENode;
typedef PtrToENode Edge;/*边*/
const int LENEdge = sizeof(struct ENode);/*边结点的大小*/
/*邻接矩阵 图*/
/*图结点的定义*/
typedef struct GNode{
	int Nv;/*顶点数*/
	int Ne;/*边数 N个顶点的无向完全图边数=N(N-1)/2*/
	WeightType G[MaxVertexNum][MaxVertexNum];/*邻接矩阵*/
	DataType Data[MaxVertexNum];/*顶点数据*/
	/*很多时候顶点并没有数据 则可以不出现*/
}*PtrToGNode;
typedef PtrToGNode MGraph;/*以邻接矩阵储存的图类型*/
const int LENMGraph = sizeof(struct GNode);/*图结点的大小*/
MGraph CreateMGraph(int VertexNum)
{/*初始化一个有Vertex个顶点但没有边的图*/
	Vertex V, W;
	MGraph Graph;/*建立图*/
	Graph = (MGraph)malloc(LENMGraph);
	Graph->Nv = VertexNum;
	Graph->Ne = 0;
	/*默认顶点编号从0开始 到Graph-1*/
	for (V = 0; V < Graph->Nv; V++){
		for (W = 0; W < Graph->Nv; W++)
			Graph->G[V][W] = INFINITY;
	}
	puts("已初始化各元素为无穷大");
	return Graph;
}
void InsertEdgeMG(MGraph Graph, Edge E)
{
	/*插入边<V1, V2>*/
	Graph->G[E->V1][E->V2] = E->Weight;
	puts("请确认是无向图--插入函数");
	/*若是无向图 还要插入对称边<V2, V1>*/
	Graph->G[E->V2][E->V1] = E->Weight;
}
MGraph BuilMGraph()
{
	MGraph Graph;
	Edge E;
	Vertex V;
	int Nv, i;
	scanf("%d", &Nv);/*读入顶点个数*/
	Graph = CreateMGraph(Nv);/*初始化有Nv个顶点但没有边的图*/
	scanf("%d", &Graph->Ne);
	if (Graph->Ne != 0){
		E = (Edge)malloc(LENEdge);
		/*读入边 格式：起点 终点 权重*/
		puts("请确认权重为整形--构建函数");
		for (i = 0; i < Graph->Ne; i++){
			scanf("%d %d %d", &E->V1, &E->V2, &E->Weight);
			InsertEdgeMG(Graph, E);
		}
	}
	while (getchar() != 10);
	puts("请确认存在结点数据");
	for (V = 0; V < Graph->Nv; V++)
		scanf("%c", &Graph->Data[V]);
	return Graph;
}

/*	邻接表图 
需要N个头指针 + 2E个结点（每个结点至少2个域），则E小于N(N-1)/4时是省空间的
对于N个顶点E条边的邻接表图时间复杂度为O(N+E)*/
typedef struct AdjVNode *PtrToAdjVNode;//Adjacent:邻接点
/* 邻接点的定义 */
struct AdjVNode{ 
	Vertex AdjV;        /* 邻接点下标 */
	WeightType Weight;  /* 边权重 */
	PtrToAdjVNode Next;    /* 指向下一个邻接点的指针 */
};
/* 顶点表头结点的定义 */
typedef struct Vnode{
	PtrToAdjVNode FirstEdge;/* 边表头指针 */
	DataType Data;            /* 存顶点的数据 */
	/* 注意：很多情况下，顶点无数据，此时Data可以不用出现 */
} AdjList[MaxVertexNum];    /* AdjList是邻接表类型 */
/* 图结点的定义 */
typedef struct LGNode{
	int Nv;     /* 顶点数 */
	int Ne;     /* 边数   */
	AdjList G;  /* 邻接表 */
}*PtrToLGNode;
typedef PtrToLGNode LGraph; /* 以邻接表方式存储的图类型 */
LGraph CreateLGraph(int VertexNum)
{ /* 初始化一个有VertexNum个顶点但没有边的图 */
	Vertex V;
	LGraph Graph;

	Graph = (LGraph)malloc(sizeof(struct GNode)); /* 建立图 */
	Graph->Nv = VertexNum;
	Graph->Ne = 0;
	/* 初始化邻接表头指针 */
	/* 注意：这里默认顶点编号从0开始，到(Graph->Nv - 1) */
	for (V = 0; V<Graph->Nv; V++)
		Graph->G[V].FirstEdge = NULL;

	return Graph;
}
void InsertEdgeLG(LGraph Graph, Edge E)
{
	PtrToAdjVNode NewNode;
	/* 插入边 <V1, V2> */
	/* 为V2建立新的邻接点 */
	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	NewNode->AdjV = E->V2;
	NewNode->Weight = E->Weight;
	/* 将V2插入V1的表头 */
	NewNode->Next = Graph->G[E->V1].FirstEdge;
	Graph->G[E->V1].FirstEdge = NewNode;

	/* 若是无向图，还要插入边 <V2, V1> */
	/* 为V1建立新的邻接点 */
	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	NewNode->AdjV = E->V1;
	NewNode->Weight = E->Weight;
	/* 将V1插入V2的表头 */
	NewNode->Next = Graph->G[E->V2].FirstEdge;
	Graph->G[E->V2].FirstEdge = NewNode;
}
LGraph BuildLGraph()
{
	LGraph Graph;
	Vertex V;
	Edge E;
	int Nv, i;
	scanf("%d", &Nv);   /* 读入顶点个数 */
	Graph = CreateLGraph(Nv); /* 初始化有Nv个顶点但没有边的图 */
	scanf("%d", &(Graph->Ne));   /* 读入边数 */
	if (Graph->Ne != 0) { /* 如果有边 */
		E = (Edge)malloc(sizeof(struct ENode)); /* 建立边结点 */
		for (i = 0; i < Graph->Ne; i++) {
			scanf("%d %d %d", &E->V1, &E->V2, &E->Weight);
			InsertEdgeLG(Graph, E);
		}
	}
	for (V = 0; V < Graph->Nv; V++)
		scanf(" %c", &(Graph->G[V].Data));
	return Graph;
}
int Path[1];
int Step;//步数
/*无向图插入*/
void InsertEdgeMG(MGraph Graph, Edge E)
{/*对称边<V2, V1>*/
	Graph->G[E->V1][E->V2] = E->Weight;
	Graph->G[E->V2][E->V1] = E->Weight;
}
MGraph BuilMGraph(unsigned Nv, unsigned Ne, void(*GetData)(DataType*), void(*GetEdge)(Edge*))
{
	MGraph Graph = CreateMGraph(Nv);
	ENode Ed; Edge E = &Ed;
	for (Vertex V = 1; V <= Graph->Nv; V++)
		GetData(&Graph->Data[V]);
	for (int i = 1; i < Graph->Ne; i++){
		GetEdge(&E);
		InsertEdgeMG(Graph, E);
	}
	return Graph;
}
void Visit(Vertex V)
{
	//printf("访问结点%d:%.lf %.lf\n", V, Graph[W].x, Graph[W].y);
	Visited[V] = true;
}
/* 检查<V, W>是否有权图Graph（不存在的边被初始化为INFINITY）中的一条边，即W是否V的邻接点。*/
/* 此函数根据图的不同类型要做不同的实现，关键取决于对不存在的边的表示方法。*/
int ISEdgaM(MGraph Graph, Vertex V, Vertex W)
{
	return Graph->G[V][W] < INFINITY ? true : false;
}
void DFSL(LGraph Graph, Vertex V, void (*Visit)(Vertex))
{/*以V为出发点对邻接表图深度优先搜索*/
	PtrToAdjVNode W;
	Visit(V);/*访问结点V*/
	Visited[V] = true;/*标记结点V已访问*/
	for (W = Graph->G[V].FirstEdge; W; W = W->Next){/*(遍历)对每个邻接点W->Ad*/
		if (!Visited[V])
			DFSL(Graph, W->AdjV, Visit);
		/*否则转向下一结点*/
	}
}
/* 从图map中匹配字串Mach */
void dfs(int r, int c, char *Mach, int *count, char map[MAP_MAX_SIZE][MAP_MAX_SIZE])
{
	const int Dir4[4][2] = { { 0, 1 /*右*/ }, { 1, 0 /*下*/ }, { -1, 0 /*上*/ }, { 0, -1 /*左*/ } };
	if (map[r][c] == *Mach){
		if (*(Mach + 1) == 0)
			(*count)++;
		else{
			Mach++;
			for (int i = 0; i < 4; i++)
				dfs(r + Dir4[i][0], c + Dir4[i][1], Mach, count, map);
		}
	}
}
int DFSM(MGraph Graph, int V)
{
	int ans = false;
	Visited[V] = true;
	if (IsSafe(Graph, V))
		ans = true;
	else{
		for (int W = 0; W < Graph->Nv; W++)
		{
			if (!Visited[W] && Jump(Graph->Data[V], Graph->Data[W], d, 0))
			{
				if ((ans = DFSM(Graph, W)) == true)
				{
					break;
				}
			}
		}
	}
	return ans;
}
int BFSM(MGraph Graph, QueueA Q, void(*Visit)(Vertex))
{/*以S为出发点对邻接矩阵储存的图Graph进行广度优先搜索*/
	int SubLast = -1;/* 指向最后一步的元素 */
	int TailThisLayer = -1;/* 指向当前层的最后一个元素 */
	int TailNextLayer = -1;/* 指向下一层的最后一个元素 */
	Point O = O.O();
	for (Vertex V = 0; V < Graph->Nv; V++){/*处理起点*/
		if (Jump(O, Graph->Data[V], d, r)){
			Visit(V);
			AddQA(Q, V);
			TailThisLayer = V;
		}
	}
	for (Vertex V; !ISemptyA(Q);){
		V = DeleteQA(Q);
		if (IsSafe(Graph, V)){
			SubLast = V;
			Visit(V);
			break;
		}
		for (Vertex W = 0; W < Graph->Nv; W++){
			if (!Visited[W] && Jump(Graph->Data[V], Graph->Data[W], d, 0)){
				Path[W] = V;/*记录与W结点有关系的上一个节点 V*/
				Visit(W);
				AddQA(Q, W);
				TailNextLayer = W;
			}
		}
		if (V == TailThisLayer){
			Step++;
			TailThisLayer = TailNextLayer;
		}
	}
	return SubLast;
}
void BFSM(MGraph Graph, Vertex S, void (*Visit)(Vertex))
{/*以S为出发点对邻接矩阵储存的图Graph进行广度优先搜索*/
	QueueA Q = CreateQA(MaxVertexNum);
	Vertex V, W;
	Visit(S); Visited[S] = true; AddQA(Q, S);/*处理起点*/
	while (!ISemptyA(Q)){
		V = DeleteQA(Q);
		for (W = 0; W < Graph->Nv; W++){/*处理图中的每个顶点W*/
			/*若W未访问过且与V是邻接点*/
			if (!Visited[W] && ISEdgaM(Graph, W, V)){
				Visit(W);
				Visited[W] = true;
				AddQA(Q, W);
			}
		}
	}
}



/*课件*/
/* 邻接表存储 - 无权图的单源最短路算法 */
/* dist[]和path[]全部初始化为-1 */
void Unweighted(LGraph Graph, int dist[], int path[], Vertex S)
{
	Queue Q;
	Vertex V;
	PtrToAdjVNode W;

	Q = CreateQueue(Graph->Nv); /* 创建空队列, MaxSize为外部定义的常数 */
	dist[S] = 0; /* 初始化源点 */
	AddQ(Q, S);

	while (!IsEmpty(Q)){
		V = DeleteQ(Q);
		for (W = Graph->G[V].FirstEdge; W; W = W->Next) /* 对V的每个邻接点W->AdjV */
		if (dist[W->AdjV] == -1) { /* 若W->AdjV未被访问过 */
			dist[W->AdjV] = dist[V] + 1; /* W->AdjV到S的距离更新 */
			path[W->AdjV] = V; /* 将V记录在S到W->AdjV的路径上 */
			AddQ(Q, W->AdjV);
		}
	} /* while结束*/
}

/* 邻接矩阵存储 - 有权图的单源最短路算法 */
Vertex FindMinDist(MGraph Graph, int dist[], int collected[])
{ /* 返回未被收录顶点中dist最小者 */
	Vertex MinV, V;
	int MinDist = INFINITY;

	for (V = 0; V<Graph->Nv; V++) {
		if (collected[V] == false && dist[V]<MinDist) {
			/* 若V未被收录，且dist[V]更小 */
			MinDist = dist[V]; /* 更新最小距离 */
			MinV = V; /* 更新对应顶点 */
		}
	}
	if (MinDist < INFINITY) /* 若找到最小dist */
		return MinV; /* 返回对应的顶点下标 */
	else return ERROR;  /* 若这样的顶点不存在，返回错误标记 */
}
int Dijkstra(MGraph Graph, int dist[], int path[], Vertex S)
{
	int collected[MaxVertexNum];
	Vertex V, W;

	/* 初始化：此处默认邻接矩阵中不存在的边用INFINITY表示 */
	for (V = 0; V<Graph->Nv; V++) {
		dist[V] = Graph->G[S][V];
		if (dist[V]<INFINITY)
			path[V] = S;
		else
			path[V] = -1;
		collected[V] = false;
	}
	/* 先将起点收入集合 */
	dist[S] = 0;
	collected[S] = true;

	while (1) {
		/* V = 未被收录顶点中dist最小者 */
		V = FindMinDist(Graph, dist, collected);
		if (V == ERROR) /* 若这样的V不存在 */
			break;      /* 算法结束 */
		collected[V] = true;  /* 收录V */
		for (W = 0; W<Graph->Nv; W++) /* 对图中的每个顶点W */
			/* 若W是V的邻接点并且未被收录 */
		if (collected[W] == false && Graph->G[V][W]<INFINITY) {
			///2018/3/9: !!!!!!!!!!!!!!!!!!!!!!!!!这里是错误的 (第一不用判断是否在集合内 第二虽然判断了也不错但加了这句就错了: 可能无法鉴别出负权边)
			if (Graph->G[V][W]<0) /* 若有负边 */	
				return false; /* 不能正确解决，返回错误标记 */
			/* 若收录V使得dist[W]变小 */
			if (dist[V] + Graph->G[V][W] < dist[W]) {
				dist[W] = dist[V] + Graph->G[V][W]; /* 更新dist[W] */
				path[W] = V; /* 更新S到W的路径 */
			}
		}
	} /* while结束*/
	return true; /* 算法执行完毕，返回正确标记 */
}

/*
朴素纪念版
*/
bool Dijkstra_(VertexKey origin, vector<WeightType> &dist, vector<VertexKey> &predecessor){
	//q[v]为true属于优先队列(原collected集合) 
	//顺便: 在数组实现的基础上也没法改为stl的heap实现(排序权dist是变动的但是又不能损失效率去强行copy) 除非用指针但那样复杂度也高于优先队列优化
	static vector<bool> q;
	static VertexKey v = -1, w = -1;
	dist.assign(vertexNum, INF);
	predecessor.assign(vertexNum, -1);
	dist[origin] = 0;
	q.assign(vertexNum, true);

	/*核心算法*/
	while (true){
		//优先队列的pop实现
		static int MinDist;
		for (MinDist = INF, w = 0; w < vertexNum; ++w) {
			if (q[w] && dist[w] < MinDist){//查找属于该"优先队列"中的一条最小权
				MinDist = dist[w];
				v = w;
			}
		}
		if (MinDist < INF){
			q[v] = false;//将v从原集合删除 并 [加入其补集]
		}
		else{//队列为空 算法结束
			break;
		}


		/* 对图中由v出发的每条边<v, w>的顶点w进行拓展操作*/
		for (w = 0; w < vertexNum; ++w){
			if (edgeData[v][w] < INF){
				/*拓展操作*/
				if (dist[v] + edgeData[v][w] < dist[w]){
					if (edgeData[v][w] < 0)
						return false;//错误:有负权边
					dist[w] = dist[v] + edgeData[v][w];//更新最短路径距离dist[w]
					predecessor[w] = v;//更新w的最短路径前驱结点
				}
			}
		}
	}
	return true;
}

/* 邻接矩阵存储 - 多源最短路算法 */
int Floyd(MGraph Graph, WeightType D[][MaxVertexNum], Vertex path[][MaxVertexNum])
{
	Vertex i, j, k;

	/* 初始化 */
	for (i = 0; i<Graph->Nv; i++)
	for (j = 0; j<Graph->Nv; j++) {
		D[i][j] = Graph->G[i][j];
		path[i][j] = -1;//2018/3/8此处是错误的
	}

	for (k = 0; k<Graph->Nv; k++)
	for (i = 0; i<Graph->Nv; i++)
	for (j = 0; j<Graph->Nv; j++)
		//维基上的算法描述还是很准的: 不用判断dists[i][j] < INF && 但有负权时: -1 + infinity < infinity 会导致出现infinity-1的距离
	if (D[i][k] + D[k][j] < D[i][j]) {
		D[i][j] = D[i][k] + D[k][j];
		if (i == j && D[i][j]<0) /* 若发现负值圈 */
			return false; /* 不能正确解决，返回错误标记 */
		path[i][j] = k;
	}
	return true; /* 算法执行完毕，返回正确标记 */
}
