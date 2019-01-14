#include<stdio.h>
#include<string.h>
#include<string>
#include<cmath>
#include<vector>
#include<iostream>
#include<queue>
#include<functional>//greater
#include"./Graph/Graph.h"
#include"./Graph/TreeObject.h"
#define EPS 1E-5
#define MAX_N 505
using namespace std;

//topGrapg[v][w]为1表示v赢了w
int topGrapg[MAX_N][MAX_N];
//indegree[n]表示结点n的入度
int indegree[MAX_N];
vector<vector<int>> graphBuffer;

void vInit(int n, int m){
	memset(topGrapg, 0, sizeof(topGrapg));
	memset(indegree, 0, sizeof(indegree));
	int p1, p2;
	for (int i = 0; i < m; ++i){
		scanf("%d%d", &p1, &p2);
		--p1;
		--p2;
		//防止重复输入
		if (0 == topGrapg[p1][p2]){
			topGrapg[p1][p2] = 1;
			++indegree[p2];
		}
	}
}

//返回具有序号最小的入度为0的顶点 若不存在返回-1  O(V)
int nGetMinIndegreeVertex(int n){
	int result = -1;
	for (int i = 0; i < n; ++i){
		if (0 == indegree[i]){
			result = i;
			break;
		}
	}
	return result;
}

//移除顶点v以及由v发出的所有边  O(V)
void vRemoveVertexAndEdge(int v, int n){
	for (int i = 0; i < n; ++i){
		if (1 == topGrapg[v][i]){
			topGrapg[v][i] = 0;
			--indegree[i];
		}
	}
	indegree[v] = -1;
}

//O(V^2)
string sTopSort(int n){
	char buffer[100] = {0};
	string result;
	int cnt = 0, v;
	//正常情况下每个顶点遍历一次
	while (-1 != (v = nGetMinIndegreeVertex(n))){
		vRemoveVertexAndEdge(v, n);
		//输出序号
		sprintf(buffer, cnt++ == 0 ? "%d" : " %d", v + 1);
		result += buffer;
	}
	return result;
}

//队列 O(V^2*ln(V))  如果用邻接表实现是O(V + E) 为什么是+而不是*
void vTopSort(int n, vector<int> &topOrderBuffer){
	priority_queue<int, vector<int>, greater<int>> q;

	topOrderBuffer.resize(n);
	topOrderBuffer.clear();
	//将所有入度为0的顶点加入队列 由于在存在多个满足条件的顶点时需要按序号排列 此处采用优先队列
	for (int i = 0; i < n; ++i){
		if (0 == indegree[i]){
			q.push(i);
		}
	}
	while (!q.empty()){
		int v = q.top();
		q.pop();
		//移除边 并更新队列
		for (int i = 0; i < n; ++i){
			if (1 == topGrapg[v][i]){
				topGrapg[v][i] = 0;
				if (0 == --indegree[i]){
					q.push(i);
				}
			}
		}
		topOrderBuffer.push_back(v);
	}
}

int mainForCurrentFile() {
	//freopen("input", "r", stdin);
	int m, n;
	vector<int> result;
	while (cin >> n >> m){
		vInit(n, m);
		//cout << sTopSort(n) << endl;
		vTopSort(n, result);
		for (auto it = result.begin(); it != result.end(); ++it){
			printf(it == result.begin() ? "%d" : " %d", *it + 1);
		}
		puts("");
	}
	return 0;
}

int mainForLib(){
	freopen("input", "r", stdin);
	int m, n;
	while (cin >> n >> m){
		AdjacentListGraph g = AdjacentListGraph(n);
		int p1, p2;
		for (int i = 0; i < m; ++i){
			scanf("%d%d", &p1, &p2);
			g.insertEdge(Graph::Edge(p1 - 1, p2 - 1, 1));
		}
		vector<Graph::VertexKey> result;
		g.topologySort(result);
		for (auto it = result.begin(); it != result.end(); ++it){
			printf(it == result.begin() ? "%d" : " %d", *it + 1);
		}
		puts("");
	}
	return 0;
};

