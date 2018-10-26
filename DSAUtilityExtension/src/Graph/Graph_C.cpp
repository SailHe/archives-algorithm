//��Graph.hû��ֱ�ӹ�ϵ

#include "../Queue.h"
#define MaxVertexNum 100/*��󶥵���*/
#define INFINTY 65535/*�����<<˫�ֽ��޷������������ֵ*/
#define MAP_MAX_SIZE 128/*�����<<˫�ֽ��޷������������ֵ*/
typedef int Vertex;/*������±��ʾ����*/
typedef int WeightType;/*�ߵ�Ȩֵ*/
typedef char DataType;/*���㴢������*/
Vertex Visited[MaxVertexNum];/*��ʼ��Ϊfalse*/

/*�ߵĶ���*/
typedef struct ENode{
	Vertex V1, V2;/*�����<V1, V2>*/
	WeightType Weight;/*Ȩ��*/
}*PtrToENode;
typedef PtrToENode Edge;/*��*/
const int LENEdge = sizeof(struct ENode);/*�߽��Ĵ�С*/
/*�ڽӾ��� ͼ*/
/*ͼ���Ķ���*/
typedef struct GNode{
	int Nv;/*������*/
	int Ne;/*���� N�������������ȫͼ����=N(N-1)/2*/
	WeightType G[MaxVertexNum][MaxVertexNum];/*�ڽӾ���*/
	DataType Data[MaxVertexNum];/*��������*/
	/*�ܶ�ʱ�򶥵㲢û������ ����Բ�����*/
}*PtrToGNode;
typedef PtrToGNode MGraph;/*���ڽӾ��󴢴��ͼ����*/
const int LENMGraph = sizeof(struct GNode);/*ͼ���Ĵ�С*/
MGraph CreateMGraph(int VertexNum)
{/*��ʼ��һ����Vertex�����㵫û�бߵ�ͼ*/
	Vertex V, W;
	MGraph Graph;/*����ͼ*/
	Graph = (MGraph)malloc(LENMGraph);
	Graph->Nv = VertexNum;
	Graph->Ne = 0;
	/*Ĭ�϶����Ŵ�0��ʼ ��Graph-1*/
	for (V = 0; V < Graph->Nv; V++){
		for (W = 0; W < Graph->Nv; W++)
			Graph->G[V][W] = INFINITY;
	}
	puts("�ѳ�ʼ����Ԫ��Ϊ�����");
	return Graph;
}
void InsertEdgeMG(MGraph Graph, Edge E)
{
	/*�����<V1, V2>*/
	Graph->G[E->V1][E->V2] = E->Weight;
	puts("��ȷ��������ͼ--���뺯��");
	/*��������ͼ ��Ҫ����ԳƱ�<V2, V1>*/
	Graph->G[E->V2][E->V1] = E->Weight;
}
MGraph BuilMGraph()
{
	MGraph Graph;
	Edge E;
	Vertex V;
	int Nv, i;
	scanf("%d", &Nv);/*���붥�����*/
	Graph = CreateMGraph(Nv);/*��ʼ����Nv�����㵫û�бߵ�ͼ*/
	scanf("%d", &Graph->Ne);
	if (Graph->Ne != 0){
		E = (Edge)malloc(LENEdge);
		/*����� ��ʽ����� �յ� Ȩ��*/
		puts("��ȷ��Ȩ��Ϊ����--��������");
		for (i = 0; i < Graph->Ne; i++){
			scanf("%d %d %d", &E->V1, &E->V2, &E->Weight);
			InsertEdgeMG(Graph, E);
		}
	}
	while (getchar() != 10);
	puts("��ȷ�ϴ��ڽ������");
	for (V = 0; V < Graph->Nv; V++)
		scanf("%c", &Graph->Data[V]);
	return Graph;
}

/*	�ڽӱ�ͼ 
��ҪN��ͷָ�� + 2E����㣨ÿ���������2���򣩣���EС��N(N-1)/4ʱ��ʡ�ռ��
����N������E���ߵ��ڽӱ�ͼʱ�临�Ӷ�ΪO(N+E)*/
typedef struct AdjVNode *PtrToAdjVNode;//Adjacent:�ڽӵ�
/* �ڽӵ�Ķ��� */
struct AdjVNode{ 
	Vertex AdjV;        /* �ڽӵ��±� */
	WeightType Weight;  /* ��Ȩ�� */
	PtrToAdjVNode Next;    /* ָ����һ���ڽӵ��ָ�� */
};
/* �����ͷ���Ķ��� */
typedef struct Vnode{
	PtrToAdjVNode FirstEdge;/* �߱�ͷָ�� */
	DataType Data;            /* �涥������� */
	/* ע�⣺�ܶ�����£����������ݣ���ʱData���Բ��ó��� */
} AdjList[MaxVertexNum];    /* AdjList���ڽӱ����� */
/* ͼ���Ķ��� */
typedef struct LGNode{
	int Nv;     /* ������ */
	int Ne;     /* ����   */
	AdjList G;  /* �ڽӱ� */
}*PtrToLGNode;
typedef PtrToLGNode LGraph; /* ���ڽӱ�ʽ�洢��ͼ���� */
LGraph CreateLGraph(int VertexNum)
{ /* ��ʼ��һ����VertexNum�����㵫û�бߵ�ͼ */
	Vertex V;
	LGraph Graph;

	Graph = (LGraph)malloc(sizeof(struct GNode)); /* ����ͼ */
	Graph->Nv = VertexNum;
	Graph->Ne = 0;
	/* ��ʼ���ڽӱ�ͷָ�� */
	/* ע�⣺����Ĭ�϶����Ŵ�0��ʼ����(Graph->Nv - 1) */
	for (V = 0; V<Graph->Nv; V++)
		Graph->G[V].FirstEdge = NULL;

	return Graph;
}
void InsertEdgeLG(LGraph Graph, Edge E)
{
	PtrToAdjVNode NewNode;
	/* ����� <V1, V2> */
	/* ΪV2�����µ��ڽӵ� */
	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	NewNode->AdjV = E->V2;
	NewNode->Weight = E->Weight;
	/* ��V2����V1�ı�ͷ */
	NewNode->Next = Graph->G[E->V1].FirstEdge;
	Graph->G[E->V1].FirstEdge = NewNode;

	/* ��������ͼ����Ҫ����� <V2, V1> */
	/* ΪV1�����µ��ڽӵ� */
	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	NewNode->AdjV = E->V1;
	NewNode->Weight = E->Weight;
	/* ��V1����V2�ı�ͷ */
	NewNode->Next = Graph->G[E->V2].FirstEdge;
	Graph->G[E->V2].FirstEdge = NewNode;
}
LGraph BuildLGraph()
{
	LGraph Graph;
	Vertex V;
	Edge E;
	int Nv, i;
	scanf("%d", &Nv);   /* ���붥����� */
	Graph = CreateLGraph(Nv); /* ��ʼ����Nv�����㵫û�бߵ�ͼ */
	scanf("%d", &(Graph->Ne));   /* ������� */
	if (Graph->Ne != 0) { /* ����б� */
		E = (Edge)malloc(sizeof(struct ENode)); /* �����߽�� */
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
int Step;//����
/*����ͼ����*/
void InsertEdgeMG(MGraph Graph, Edge E)
{/*�ԳƱ�<V2, V1>*/
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
	//printf("���ʽ��%d:%.lf %.lf\n", V, Graph[W].x, Graph[W].y);
	Visited[V] = true;
}
/* ���<V, W>�Ƿ���ȨͼGraph�������ڵı߱���ʼ��ΪINFINITY���е�һ���ߣ���W�Ƿ�V���ڽӵ㡣*/
/* �˺�������ͼ�Ĳ�ͬ����Ҫ����ͬ��ʵ�֣��ؼ�ȡ���ڶԲ����ڵıߵı�ʾ������*/
int ISEdgaM(MGraph Graph, Vertex V, Vertex W)
{
	return Graph->G[V][W] < INFINITY ? true : false;
}
void DFSL(LGraph Graph, Vertex V, void (*Visit)(Vertex))
{/*��VΪ��������ڽӱ�ͼ�����������*/
	PtrToAdjVNode W;
	Visit(V);/*���ʽ��V*/
	Visited[V] = true;/*��ǽ��V�ѷ���*/
	for (W = Graph->G[V].FirstEdge; W; W = W->Next){/*(����)��ÿ���ڽӵ�W->Ad*/
		if (!Visited[V])
			DFSL(Graph, W->AdjV, Visit);
		/*����ת����һ���*/
	}
}
/* ��ͼmap��ƥ���ִ�Mach */
void dfs(int r, int c, char *Mach, int *count, char map[MAP_MAX_SIZE][MAP_MAX_SIZE])
{
	const int Dir4[4][2] = { { 0, 1 /*��*/ }, { 1, 0 /*��*/ }, { -1, 0 /*��*/ }, { 0, -1 /*��*/ } };
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
{/*��SΪ��������ڽӾ��󴢴��ͼGraph���й����������*/
	int SubLast = -1;/* ָ�����һ����Ԫ�� */
	int TailThisLayer = -1;/* ָ��ǰ������һ��Ԫ�� */
	int TailNextLayer = -1;/* ָ����һ������һ��Ԫ�� */
	Point O = O.O();
	for (Vertex V = 0; V < Graph->Nv; V++){/*�������*/
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
				Path[W] = V;/*��¼��W����й�ϵ����һ���ڵ� V*/
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
{/*��SΪ��������ڽӾ��󴢴��ͼGraph���й����������*/
	QueueA Q = CreateQA(MaxVertexNum);
	Vertex V, W;
	Visit(S); Visited[S] = true; AddQA(Q, S);/*�������*/
	while (!ISemptyA(Q)){
		V = DeleteQA(Q);
		for (W = 0; W < Graph->Nv; W++){/*����ͼ�е�ÿ������W*/
			/*��Wδ���ʹ�����V���ڽӵ�*/
			if (!Visited[W] && ISEdgaM(Graph, W, V)){
				Visit(W);
				Visited[W] = true;
				AddQA(Q, W);
			}
		}
	}
}



/*�μ�*/
/* �ڽӱ�洢 - ��Ȩͼ�ĵ�Դ���·�㷨 */
/* dist[]��path[]ȫ����ʼ��Ϊ-1 */
void Unweighted(LGraph Graph, int dist[], int path[], Vertex S)
{
	Queue Q;
	Vertex V;
	PtrToAdjVNode W;

	Q = CreateQueue(Graph->Nv); /* �����ն���, MaxSizeΪ�ⲿ����ĳ��� */
	dist[S] = 0; /* ��ʼ��Դ�� */
	AddQ(Q, S);

	while (!IsEmpty(Q)){
		V = DeleteQ(Q);
		for (W = Graph->G[V].FirstEdge; W; W = W->Next) /* ��V��ÿ���ڽӵ�W->AdjV */
		if (dist[W->AdjV] == -1) { /* ��W->AdjVδ�����ʹ� */
			dist[W->AdjV] = dist[V] + 1; /* W->AdjV��S�ľ������ */
			path[W->AdjV] = V; /* ��V��¼��S��W->AdjV��·���� */
			AddQ(Q, W->AdjV);
		}
	} /* while����*/
}

/* �ڽӾ���洢 - ��Ȩͼ�ĵ�Դ���·�㷨 */
Vertex FindMinDist(MGraph Graph, int dist[], int collected[])
{ /* ����δ����¼������dist��С�� */
	Vertex MinV, V;
	int MinDist = INFINITY;

	for (V = 0; V<Graph->Nv; V++) {
		if (collected[V] == false && dist[V]<MinDist) {
			/* ��Vδ����¼����dist[V]��С */
			MinDist = dist[V]; /* ������С���� */
			MinV = V; /* ���¶�Ӧ���� */
		}
	}
	if (MinDist < INFINITY) /* ���ҵ���Сdist */
		return MinV; /* ���ض�Ӧ�Ķ����±� */
	else return ERROR;  /* �������Ķ��㲻���ڣ����ش����� */
}
int Dijkstra(MGraph Graph, int dist[], int path[], Vertex S)
{
	int collected[MaxVertexNum];
	Vertex V, W;

	/* ��ʼ�����˴�Ĭ���ڽӾ����в����ڵı���INFINITY��ʾ */
	for (V = 0; V<Graph->Nv; V++) {
		dist[V] = Graph->G[S][V];
		if (dist[V]<INFINITY)
			path[V] = S;
		else
			path[V] = -1;
		collected[V] = false;
	}
	/* �Ƚ�������뼯�� */
	dist[S] = 0;
	collected[S] = true;

	while (1) {
		/* V = δ����¼������dist��С�� */
		V = FindMinDist(Graph, dist, collected);
		if (V == ERROR) /* ��������V������ */
			break;      /* �㷨���� */
		collected[V] = true;  /* ��¼V */
		for (W = 0; W<Graph->Nv; W++) /* ��ͼ�е�ÿ������W */
			/* ��W��V���ڽӵ㲢��δ����¼ */
		if (collected[W] == false && Graph->G[V][W]<INFINITY) {
			///2018/3/9: !!!!!!!!!!!!!!!!!!!!!!!!!�����Ǵ���� (��һ�����ж��Ƿ��ڼ����� �ڶ���Ȼ�ж���Ҳ�����������ʹ���: �����޷��������Ȩ��)
			if (Graph->G[V][W]<0) /* ���и��� */	
				return false; /* ������ȷ��������ش����� */
			/* ����¼Vʹ��dist[W]��С */
			if (dist[V] + Graph->G[V][W] < dist[W]) {
				dist[W] = dist[V] + Graph->G[V][W]; /* ����dist[W] */
				path[W] = V; /* ����S��W��·�� */
			}
		}
	} /* while����*/
	return true; /* �㷨ִ����ϣ�������ȷ��� */
}

/*
���ؼ����
*/
bool Dijkstra_(VertexKey origin, vector<WeightType> &dist, vector<VertexKey> &predecessor){
	//q[v]Ϊtrue�������ȶ���(ԭcollected����) 
	//˳��: ������ʵ�ֵĻ�����Ҳû����Ϊstl��heapʵ��(����Ȩdist�Ǳ䶯�ĵ����ֲ�����ʧЧ��ȥǿ��copy) ������ָ�뵫�������Ӷ�Ҳ�������ȶ����Ż�
	static vector<bool> q;
	static VertexKey v = -1, w = -1;
	dist.assign(vertexNum, INF);
	predecessor.assign(vertexNum, -1);
	dist[origin] = 0;
	q.assign(vertexNum, true);

	/*�����㷨*/
	while (true){
		//���ȶ��е�popʵ��
		static int MinDist;
		for (MinDist = INF, w = 0; w < vertexNum; ++w) {
			if (q[w] && dist[w] < MinDist){//�������ڸ�"���ȶ���"�е�һ����СȨ
				MinDist = dist[w];
				v = w;
			}
		}
		if (MinDist < INF){
			q[v] = false;//��v��ԭ����ɾ�� �� [�����䲹��]
		}
		else{//����Ϊ�� �㷨����
			break;
		}


		/* ��ͼ����v������ÿ����<v, w>�Ķ���w������չ����*/
		for (w = 0; w < vertexNum; ++w){
			if (edgeData[v][w] < INF){
				/*��չ����*/
				if (dist[v] + edgeData[v][w] < dist[w]){
					if (edgeData[v][w] < 0)
						return false;//����:�и�Ȩ��
					dist[w] = dist[v] + edgeData[v][w];//�������·������dist[w]
					predecessor[w] = v;//����w�����·��ǰ�����
				}
			}
		}
	}
	return true;
}

/* �ڽӾ���洢 - ��Դ���·�㷨 */
int Floyd(MGraph Graph, WeightType D[][MaxVertexNum], Vertex path[][MaxVertexNum])
{
	Vertex i, j, k;

	/* ��ʼ�� */
	for (i = 0; i<Graph->Nv; i++)
	for (j = 0; j<Graph->Nv; j++) {
		D[i][j] = Graph->G[i][j];
		path[i][j] = -1;//2018/3/8�˴��Ǵ����
	}

	for (k = 0; k<Graph->Nv; k++)
	for (i = 0; i<Graph->Nv; i++)
	for (j = 0; j<Graph->Nv; j++)
		//ά���ϵ��㷨�������Ǻ�׼��: �����ж�dists[i][j] < INF && ���и�Ȩʱ: -1 + infinity < infinity �ᵼ�³���infinity-1�ľ���
	if (D[i][k] + D[k][j] < D[i][j]) {
		D[i][j] = D[i][k] + D[k][j];
		if (i == j && D[i][j]<0) /* �����ָ�ֵȦ */
			return false; /* ������ȷ��������ش����� */
		path[i][j] = k;
	}
	return true; /* �㷨ִ����ϣ�������ȷ��� */
}
