#include<stdio.h>
#include<string.h>
#include<string>
#include<cmath>
#include<vector>
#include<iostream>
#include<algorithm>
#include<queue>
#include<functional>//greater
#include<list>
#include<stack>
#include<map>
#include<iomanip>//cout���ֲ�����

#include "else\DisjointSet.h"
// #include "ExtendSpace.h"

#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

// ����ʵ�Ǳ�....
struct VertexNode {
	int weight;
	int targetID;
	VertexNode() {}
	VertexNode(int targetID, int weight) {
		this->weight = weight;
		this->targetID = targetID;
	}
};

vector<list<VertexNode>> nGraph;
void vOutputGraph(int n) {
	puts("----------------------------");
	for (int i = 0; i < n; ++i) {
		printf("vertex %d\n", i);
		for (auto it = nGraph[i].begin(); it != nGraph[i].end(); ++it) {
			cout << i << " " << it->targetID << endl;
		}
	}
}

//��׼�ļ���������� ����ֵ������ʼ����ͨ�Ķ������;
//PS: ����������������: depth; ͼ���������������������: pastId
int dfsStandard(int pastId, vector<bool> &visited) {
	int cnt = 1;
	visited[pastId] = true;
	for (auto it = nGraph[pastId].begin(); it != nGraph[pastId].end(); ++it) {
		if (!visited[it->targetID]) {
			cnt += dfsStandard(it->targetID, visited);
		}
	}
	//���������򷵻ص��Ǳ�׼dfs���ܵ������� �����ݵĻ���ʵ�ϲ�û�б���ÿһ����
	//visited[pastId] = false;
	return cnt;
}

int bfsStandard(int startID, vector<int> &dist) {
	int cnt = 0;
	queue<int> q;
	q.push(startID);
	dist[startID] = 0;
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		++cnt;
		//����v�����б�
		for (auto it = nGraph[v].begin(); it != nGraph[v].end(); ++it) {
			if (-1 == dist[it->targetID]) {
				dist[it->targetID] = dist[v] + 1;
				q.push(it->targetID);
			}
		}
	}
	return cnt;
}

//�ж�ͼ����ͨ��
int MainJudgeConnectivity() {
	int n, e;
	vector<bool> visited;
	vector<int> dist;
	while (2 == scanf("%d%d", &n, &e)) {
		nGraph.clear();
		nGraph.resize(n);
		int n1, n2;
		for (int i = 0; i < e; ++i) {
			scanf("%d%d", &n1, &n2);
			--n1;
			--n2;
			nGraph[n1].push_back(VertexNode(n2, 1));
			nGraph[n2].push_back(VertexNode(n1, 1));
		}
		visited.assign(n, false);
		dist.assign(n, -1);
		//int bfsCnt = bfsStandard(0, dist);
		int dfsCnt = dfsStandard(0, visited);
		puts(dfsCnt == n ? "Yes" : "No");
	}
	return 0;
}

int main() {
	DisjointSet nDisjointSet(10);
	nDisjointSet.unionElement(0, 1);
	MainJudgeConnectivity();
	return 0;
}
