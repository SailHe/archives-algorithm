//#include "VanHe.h"
//#include "Queue.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <queue>
using namespace std;
#define MaxVertexNum 110/*最大顶点数*/
#define INFINITYi 65535/*无穷大*/
double d;//跳跃力
double r = 15.0 / 2.0;//半径
struct Point
{
	double x;
	double y;
	void GetPoint(Point *p){
		scanf("%lf%lf", &(p->x), &(p->y));
	}/*返回一个零点*/
	Point O(){
		Point O = { 0, 0 };
		return O;
	}
};
typedef int Vertex;/*顶点的下标表示顶点*/
typedef double WeightType;/*边的权值*/
typedef Point DataType;/*顶点储存类型*/
typedef struct ENode{
	Vertex V1, V2;/*有向边<V1, V2>*/
	WeightType Weight;/*权重*/
}*PtrToENode;
typedef PtrToENode Edge;/*边*/
const int LENEdge = sizeof(struct ENode);/*边结点的大小*/
typedef struct GNode{
	int Nv;/*顶点数*/
	int Ne;/*边数 N个顶点的无向完全图边数=N(N-1)/2*/
	WeightType G[MaxVertexNum][MaxVertexNum];/*邻接矩阵*/
	DataType Data[MaxVertexNum];/*顶点数据*/
	void GetEdge(Edge *E)
	{
		scanf("%d %d %d", &(*E)->V1, &(*E)->V2, &(*E)->Weight);
	}/*自带的构建函数*/
	void GetData(DataType *Data)
	{
		scanf("%c", Data);
	}
}*PtrToGNode;
typedef PtrToGNode MGraph;/*以邻接矩阵储存的图类型*/
const int LENMGraph = sizeof(struct GNode);/*图结点的大小*/
double Dis(Point a, Point b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}
MGraph CreateMGraph()
{
	MGraph Graph = (MGraph)malloc(LENMGraph);
	return Graph;
}
void FreeGraph(MGraph *Graph)
{
	free(*Graph);
	*Graph = NULL;
}
/*无向图插入*/
void InsertEdgeMG(MGraph Graph, Edge E)
{/*对称边<V2, V1>*/
	Graph->G[E->V1][E->V2] = E->Weight;
	Graph->G[E->V2][E->V1] = E->Weight;
}
MGraph BuilMGraph(unsigned Nv, unsigned Ne, void(*GetData)(DataType*))
{/**/
	MGraph Graph = CreateMGraph();
	/*默认顶点编号从0到Graph-1*/
	Graph->Nv = Nv;Graph->Ne = Ne;
	for (Vertex V = 1; V < Graph->Nv; V++)
		GetData(&Graph->Data[V]);
	return Graph;
}
/* 返回点P距离 矩形边界(XL YL分别表示横向和纵向边界的绝对大小)的最短距离 */
double DisBorder(MGraph Graph, Point P, double XL, double YL)
{
	double x = fabs(P.x), y = fabs(P.y);
	if (x > y)
		return XL - x;
	else if (y > x)
		return YL - y;
	else
		return XL > YL ? YL - y : XL - x;
}
void InitializeEdge(MGraph Graph)
{/*权值代表每个两个顶点间的绝对距离*/
	ENode E;
	Graph->Data[0] = { 0, 0 };/*起点&&原点*/
	for (E.V1 = 0; E.V1 < Graph->Nv + 1; E.V1++)
	{
		Graph->G[E.V1][E.V1] = INFINITYi;
		for (E.V2 = E.V1 + 1; E.V2 < Graph->Nv; E.V2++)
		{
			E.Weight = Dis(Graph->Data[E.V1], Graph->Data[E.V2]);
			E.V1 == 0 ? E.Weight -= 7.5 : 0; InsertEdgeMG(Graph, &E);
		}/*各顶点到边界Graph->Nv + 1的绝对距离*/
		E.Weight = DisBorder(Graph, Graph->Data[E.V1], 50.0, 50.0);
		InsertEdgeMG(Graph, &E);
	}
}
void GetData(DataType *Data)
{
	scanf("%lf%lf", &Data->x, &(*Data).y);
}
/*Short Path Fast Agritem 默认起始点是0(队列优化的Bellman–Ford algorithm)*/
void SPFA(MGraph Graph)
{
	queue<int> Q;
	Q.push(0);
	int ExistQ[MaxVertexNum] = { 1 };
	int Step[MaxVertexNum] = { 0 };/*记录起始点到所有点的最短路径*/
	double Dist[MaxVertexNum] = { 0.0 };/*已知起始点到起始点所需的最少步数为0 最短路径为0*/
	for (int i = 1; i <= Graph->Nv; i++)
		Dist[i] = INFINITYi;
	while (!Q.empty())
	{
		int v = Q.front(); Q.pop();
		ExistQ[v] = 0;
		for (int w = 1; w <= Graph->Nv; w++)
		{
			if (Graph->G[v][w] <= d && Graph->G[v][w] + Dist[v] <= Dist[w])
			{
				if (Graph->G[v][w] + Dist[v] < Dist[w] || Step[w] > Step[v] + 1)
				{/* 出现最短路 || 相同距离取步数少的 */
					Dist[w] = Graph->G[v][w] + Dist[v];
					Step[w] = Step[v] + 1;/*这条语句第一次执行时v一定为0 否则没有最短路*/
					if (!ExistQ[w])
					{/*将不在队列中的有可能引起新的优化的元素放入队尾*/
						Q.push(w);
						ExistQ[w] = 1;
					}
				}
			}
		}
	}
	if (Dist[Graph->Nv] == INFINITYi)
		puts("can't be saved");
	else printf("%.2lf %d\n", Dist[Graph->Nv], Step[Graph->Nv]);
}
int main()
{
	//freopen("input1.txt","r",stdin);
	//freopen("input2.txt", "r", stdin);
	/**
	freopen("C:\\Users\\魔诃不思议\\Documents\\Visual Studio 2013\\Projects\\实验室\\测试数据编写\\test.in", "r", stdin);
	char *Outname = "output1.txt";
	freopen(Outname, "w", stdout);
	/**/
	int N;
	MGraph Graph = NULL;
	while (~scanf("%d%lf", &N, &d)){
		N++;/*加上一个原点*/
		Graph = BuilMGraph(N, N*(N-1), GetData);
		InitializeEdge(Graph); 
		SPFA(Graph);
		FreeGraph(&Graph);
	}
	/**
	freopen("output2.txt", "w", stdout);
	FILE *fp = fopen("C:\\Users\\魔诃不思议\\Documents\\Visual Studio 2013\\Projects\\实验室\\测试数据编写\\test.out", "r");
	FILE *fpt = fopen(Outname, "r");
	char c1 = 0, c2 = 0;
	for (int i = 1; i <= 30000; i++)
	{
		c1 = c2 = 0;
		while (c1 != 10 && c1 == c2){
			c1 = fgetc(fp);
			c2 = fgetc(fpt);
		}
		printf(c1 == c2 ? "" : "Error in %d\n", i);
	}puts("end");
	/**/
	return 0;
}
/*
53.27 4

57.83 4

3 18.00
-15 -15
-4 -26
7 -37
*/

/*
	在此题ac后过了将近1年刚20的我突发此感:
2018/3/7
可以说是从这道题开始我窥见了的指针的释放(指free)
在17年年底在家的时候我终于自我感觉领悟了指针的使用
(区别了delete与free 有篇文章不知道放哪了 以后看到了会将其放在C++Demo项目里面)
也是从此题开始我窥见了图的奥妙(主要是dfs bfs 最短路径)
而今天正是在集成图类而对其重构的, 也算是竭尽所学了
看到这份约莫一年前写下的代码至今我仍能看懂 我是庆幸自己一开始就养成了良好的习惯的
又看到HDU上从12点到24点的提交记录我是特别感慨的
我想我现在做的也是类似与当时的事吧, 但集成数据结构类花了我更多的时间, 由此看来我想这一定是值得的
*/
/*
int Path[1];
int Step;//步数
//保留版本(应该是这道题的简单版本): 若只需要最短路径步数而不需要步数最少的话 可以用层数来判断
int save007(void(*visit)(VertexKey)){
	queue<VertexKey> buffer;
	int SubLast = -1;// 指向最后一步的元素
	int TailThisLayer = -1;// 指向当前层的最后一个元素
	int TailNextLayer = -1;// 指向下一层的最后一个元素
	Point pO;
	for (VertexKey v = 0; v < vertexNum; v++){//处理起点
		if (jump(pO, vertexData[v], d, r)){
			visit(v);
			buffer.push(v);
			TailThisLayer = v;
		}
	}
	for (VertexKey v; !buffer.empty();){
		v = buffer.front(); buffer.pop();
		if (isSafe(v)){
			SubLast = v;
			visit(v);
			break;
		}
		for (VertexKey w = 0; w < vertexNum; w++){
			if (!visited[w] && jump(vertexData[v], vertexData[w], d, 0)){
				Path[w] = v;//记录与w结点有关系的上一个节点 v
				visit(w);
				buffer.push(w);
				TailNextLayer = w;
			}
		}
		if (v == TailThisLayer){
			Step++;
			TailThisLayer = TailNextLayer;
		}
	}
	return SubLast;
}
*/