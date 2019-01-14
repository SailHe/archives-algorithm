//#include "VanHe.h"
//#include "Queue.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <queue>
using namespace std;
#define MaxVertexNum 110/*��󶥵���*/
#define INFINITYi 65535/*�����*/
double d;//��Ծ��
double r = 15.0 / 2.0;//�뾶
struct Point
{
	double x;
	double y;
	void GetPoint(Point *p){
		scanf("%lf%lf", &(p->x), &(p->y));
	}/*����һ�����*/
	Point O(){
		Point O = { 0, 0 };
		return O;
	}
};
typedef int Vertex;/*������±��ʾ����*/
typedef double WeightType;/*�ߵ�Ȩֵ*/
typedef Point DataType;/*���㴢������*/
typedef struct ENode{
	Vertex V1, V2;/*�����<V1, V2>*/
	WeightType Weight;/*Ȩ��*/
}*PtrToENode;
typedef PtrToENode Edge;/*��*/
const int LENEdge = sizeof(struct ENode);/*�߽��Ĵ�С*/
typedef struct GNode{
	int Nv;/*������*/
	int Ne;/*���� N�������������ȫͼ����=N(N-1)/2*/
	WeightType G[MaxVertexNum][MaxVertexNum];/*�ڽӾ���*/
	DataType Data[MaxVertexNum];/*��������*/
	void GetEdge(Edge *E)
	{
		scanf("%d %d %d", &(*E)->V1, &(*E)->V2, &(*E)->Weight);
	}/*�Դ��Ĺ�������*/
	void GetData(DataType *Data)
	{
		scanf("%c", Data);
	}
}*PtrToGNode;
typedef PtrToGNode MGraph;/*���ڽӾ��󴢴��ͼ����*/
const int LENMGraph = sizeof(struct GNode);/*ͼ���Ĵ�С*/
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
/*����ͼ����*/
void InsertEdgeMG(MGraph Graph, Edge E)
{/*�ԳƱ�<V2, V1>*/
	Graph->G[E->V1][E->V2] = E->Weight;
	Graph->G[E->V2][E->V1] = E->Weight;
}
MGraph BuilMGraph(unsigned Nv, unsigned Ne, void(*GetData)(DataType*))
{/**/
	MGraph Graph = CreateMGraph();
	/*Ĭ�϶����Ŵ�0��Graph-1*/
	Graph->Nv = Nv;Graph->Ne = Ne;
	for (Vertex V = 1; V < Graph->Nv; V++)
		GetData(&Graph->Data[V]);
	return Graph;
}
/* ���ص�P���� ���α߽�(XL YL�ֱ��ʾ���������߽�ľ��Դ�С)����̾��� */
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
{/*Ȩֵ����ÿ�����������ľ��Ծ���*/
	ENode E;
	Graph->Data[0] = { 0, 0 };/*���&&ԭ��*/
	for (E.V1 = 0; E.V1 < Graph->Nv + 1; E.V1++)
	{
		Graph->G[E.V1][E.V1] = INFINITYi;
		for (E.V2 = E.V1 + 1; E.V2 < Graph->Nv; E.V2++)
		{
			E.Weight = Dis(Graph->Data[E.V1], Graph->Data[E.V2]);
			E.V1 == 0 ? E.Weight -= 7.5 : 0; InsertEdgeMG(Graph, &E);
		}/*�����㵽�߽�Graph->Nv + 1�ľ��Ծ���*/
		E.Weight = DisBorder(Graph, Graph->Data[E.V1], 50.0, 50.0);
		InsertEdgeMG(Graph, &E);
	}
}
void GetData(DataType *Data)
{
	scanf("%lf%lf", &Data->x, &(*Data).y);
}
/*Short Path Fast Agritem Ĭ����ʼ����0(�����Ż���Bellman�CFord algorithm)*/
void SPFA(MGraph Graph)
{
	queue<int> Q;
	Q.push(0);
	int ExistQ[MaxVertexNum] = { 1 };
	int Step[MaxVertexNum] = { 0 };/*��¼��ʼ�㵽���е�����·��*/
	double Dist[MaxVertexNum] = { 0.0 };/*��֪��ʼ�㵽��ʼ����������ٲ���Ϊ0 ���·��Ϊ0*/
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
				{/* �������· || ��ͬ����ȡ�����ٵ� */
					Dist[w] = Graph->G[v][w] + Dist[v];
					Step[w] = Step[v] + 1;/*��������һ��ִ��ʱvһ��Ϊ0 ����û�����·*/
					if (!ExistQ[w])
					{/*�����ڶ����е��п��������µ��Ż���Ԫ�ط����β*/
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
	freopen("C:\\Users\\ħڭ��˼��\\Documents\\Visual Studio 2013\\Projects\\ʵ����\\�������ݱ�д\\test.in", "r", stdin);
	char *Outname = "output1.txt";
	freopen(Outname, "w", stdout);
	/**/
	int N;
	MGraph Graph = NULL;
	while (~scanf("%d%lf", &N, &d)){
		N++;/*����һ��ԭ��*/
		Graph = BuilMGraph(N, N*(N-1), GetData);
		InitializeEdge(Graph); 
		SPFA(Graph);
		FreeGraph(&Graph);
	}
	/**
	freopen("output2.txt", "w", stdout);
	FILE *fp = fopen("C:\\Users\\ħڭ��˼��\\Documents\\Visual Studio 2013\\Projects\\ʵ����\\�������ݱ�д\\test.out", "r");
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
	�ڴ���ac����˽���1���20����ͻ���˸�:
2018/3/7
����˵�Ǵ�����⿪ʼ�ҿ����˵�ָ����ͷ�(ָfree)
��17������ڼҵ�ʱ�����������Ҹо�������ָ���ʹ��
(������delete��free ��ƪ���²�֪�������� �Ժ󿴵��˻Ὣ�����C++Demo��Ŀ����)
Ҳ�ǴӴ��⿪ʼ�ҿ�����ͼ�İ���(��Ҫ��dfs bfs ���·��)
�����������ڼ���ͼ��������ع���, Ҳ���ǽ߾���ѧ��
�������ԼĪһ��ǰд�µĴ������������ܿ��� ���������Լ�һ��ʼ�����������õ�ϰ�ߵ�
�ֿ���HDU�ϴ�12�㵽24����ύ��¼�����ر�п���
��������������Ҳ�������뵱ʱ���°�, ���������ݽṹ�໨���Ҹ����ʱ��, �ɴ˿���������һ����ֵ�õ�
*/
/*
int Path[1];
int Step;//����
//�����汾(Ӧ���������ļ򵥰汾): ��ֻ��Ҫ���·������������Ҫ�������ٵĻ� �����ò������ж�
int save007(void(*visit)(VertexKey)){
	queue<VertexKey> buffer;
	int SubLast = -1;// ָ�����һ����Ԫ��
	int TailThisLayer = -1;// ָ��ǰ������һ��Ԫ��
	int TailNextLayer = -1;// ָ����һ������һ��Ԫ��
	Point pO;
	for (VertexKey v = 0; v < vertexNum; v++){//�������
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
				Path[w] = v;//��¼��w����й�ϵ����һ���ڵ� v
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