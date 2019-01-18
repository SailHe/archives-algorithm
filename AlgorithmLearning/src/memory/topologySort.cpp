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

//topGrapg[v][w]Ϊ1��ʾvӮ��w
int topGrapg[MAX_N][MAX_N];
//indegree[n]��ʾ���n�����
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
		//��ֹ�ظ�����
		if (0 == topGrapg[p1][p2]){
			topGrapg[p1][p2] = 1;
			++indegree[p2];
		}
	}
}

//���ؾ��������С�����Ϊ0�Ķ��� �������ڷ���-1  O(V)
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

//�Ƴ�����v�Լ���v���������б�  O(V)
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
	//���������ÿ���������һ��
	while (-1 != (v = nGetMinIndegreeVertex(n))){
		vRemoveVertexAndEdge(v, n);
		//������
		sprintf(buffer, cnt++ == 0 ? "%d" : " %d", v + 1);
		result += buffer;
	}
	return result;
}

//���� O(V^2*ln(V))  ������ڽӱ�ʵ����O(V + E) Ϊʲô��+������*
void vTopSort(int n, vector<int> &topOrderBuffer){
	priority_queue<int, vector<int>, greater<int>> q;

	topOrderBuffer.resize(n);
	topOrderBuffer.clear();
	//���������Ϊ0�Ķ��������� �����ڴ��ڶ�����������Ķ���ʱ��Ҫ��������� �˴��������ȶ���
	for (int i = 0; i < n; ++i){
		if (0 == indegree[i]){
			q.push(i);
		}
	}
	while (!q.empty()){
		int v = q.top();
		q.pop();
		//�Ƴ��� �����¶���
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

