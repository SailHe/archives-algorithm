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

#include "ExtendSpace.h"

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

template<class ElementType>
//���鼯ģ���� unit join set
class DisjointSet {
public:
	//using ElementType = int;
	//�ü��ϵ�root�±��ʾ����
	using SetRoot = int;
	using SetBaseType = vector<ElementType>;
	//using SetBaseType = ElementType[MAX_N];
	//��class����vector��http://139.196.145.92�ϻ�Runtime Error
	//vector<ElementType> debugTemp1;

	DisjointSet() {}
	// O(N)
	DisjointSet(size_t n) {
		//�µ�Ԫ�س�ʼֵҲ������-1 ÿ��Ԫ�ض�����һ�������ļ���
		disjointSetBase.assign(n, -1);
		//memset(disjointSetBase, -1, n*sizeof(ElementType));
	}

	// �ϲ��������� O(lgN)
	// �������ϲ�����true ���򷵻�false
	bool unionSet(SetRoot root1, SetRoot root2) {
		if (root1 == root2) {
			return false;
		}
		else {
			//��֤С���ϲ���󼯺�
			if (disjointSetBase[root2] < disjointSetBase[root1]) {
				//1����2
				disjointSetBase[root2] += disjointSetBase[root1];
				disjointSetBase[root1] = root2;
			}
			else {
				disjointSetBase[root1] += disjointSetBase[root2];
				disjointSetBase[root2] = root1;
			}
			return true;
		}
	}

	// ����Ԫ�ؽ��кϲ� O(lgN)
	// �������ϲ�����true ���򷵻�false
	bool unionElement(ElementType x1, ElementType x2) {
		return unionSet(findRoot(x1), findRoot(x2));
	}

	// ��ѯָ��Ԫ�صĸ� O(lgN)
	SetRoot findRoot(ElementType x) {
		//����Ĭ�ϳ�ʼֵΪ1
		return disjointSetBase[x] < 0 ? x//�ҵ����ϵĸ�
			: disjointSetBase[x] = findRoot(disjointSetBase[x]);//����Ѱ�Ҳ�����·��ѹ��
	}

	// ����ָ�����ϵ�Ԫ�ظ���(��С) O(1)
	size_t size(SetRoot root) {
		return -disjointSetBase[root];
	}

	// ��ʼ�� O(N)
	void init() {
		disjointSetBase.assign(disjointSetBase.size(), -1);
		//memset(disjointSetBase, -1, sizeof(SetBaseType));
	}

	// ��ԭ���Ļ����������С O(N)
	void reSize(size_t size) {
		disjointSetBase.resize(size, -1);
		//memset(disjointSetBase, -1, sizeof(SetBaseType));
	}

	// ���ؼ����� O(N)
	size_t calcSetCount() {
		size_t cnt = 0;
		for (size_t i = 0; i < disjointSetBase.size(); ++i) {
			cnt += disjointSetBase[i] < 0 ? 1 : 0;
		}
		return cnt;
	}

private:
	SetBaseType disjointSetBase;
};


//�ж�ͼ����ͨ�� --���鼯��
int MainJudgeConnectivity() {
	int n, e;
	vector<bool> visited;
	vector<int> dist;
	while (2 == scanf("%d%d", &n, &e)) {
		DisjointSet<int> nDisjointSet(n);
		int n1, n2;
		for (int i = 0; i < e; ++i) {
			scanf("%d%d", &n1, &n2);
			--n1;
			--n2;
			nDisjointSet.unionElement(n1, n2);
		}
		int disjointSetCnt = nDisjointSet.size(nDisjointSet.findRoot(0));
		puts(disjointSetCnt == n ? "Yes" : "No");
	}
	return 0;
}

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

struct EdgeFullNode {
	int weight = -1;
	int originID = -1;
	int targetID = -1;
	EdgeFullNode() {}
	EdgeFullNode(int originID, int targetID, int weight) {
		this->weight = weight;
		this->originID = originID;
		this->targetID = targetID;
	}
};

// ������С��������Ȩֵ
int kruskal(int n, ArrayList<EdgeFullNode> &edgeList) {
	// �����еı߰���Ȩֵ����
	std::sort(edgeList.begin(), edgeList.end(), [&](EdgeFullNode const &lhs, EdgeFullNode const &rhs) {
		return lhs.weight < rhs.weight;
	});

	DisjointSet<int> nDisjointSet(n);
	int index = 0;
	int lhsVertex = -1, rhsVertex = -1;
	int unitSetCount = n;
	int sumWeight = 0;
	// ��������ĿΪ1, ���߲�����ͨͼʱ�㷨��ֹ
	// ����������Ȼ��һ�� ������geter seter���
	while (unitSetCount != 1 && index < edgeList.size()) {
		lhsVertex = edgeList[index].originID;
		rhsVertex = edgeList[index].targetID;
		// ÿ�μ���һ��Ȩֵ��С�ı߽���ԭ����֮�й�(ͨ���߹���)��������ͬ�ļ�����, ��������һ������
		// �����ϲ���˵�������ڲ�ͬ����
		if (nDisjointSet.unionElement(lhsVertex, rhsVertex)) {
			sumWeight += edgeList[index].weight;
			--unitSetCount;
		}
		else {
			// do nothing
		}
		/*if (nDisjointSet.findRoot(lhsVertex) != nDisjointSet.findRoot(rhsVertex)) {
			sumWeight += edgeList[index].weight;
			nDisjointSet.unionElement(lhsVertex, rhsVertex);
			--unitSetCount;
		}
		else {
			// do nothing
		}*/
		++index;
	}
	return sumWeight;
}

// ��С������Ȩֵ����
/*
5
0 41 67 34 0
41 0 69 24 78
67 69 0 58 62
34 24 58 0 64
0 78 62 64 0

*/
int mainForKruskal() {
	int n;
	// �����б�
	while (cin >> n) {
		// ArrayList<LinkedList<VertexNode>> edgeDada(n);
		ArrayList<EdgeFullNode> edgeList;
		edgeList.reserve(n);
		EdgeFullNode temp;
		for (int r = 0; r < n; ++r) {
			temp.originID = r;
			for (int c = 0; c < n; ++c) {
				temp.targetID = c;
				scanf("%d", &temp.weight);
				if (r <= c) {
					// do nothing
				}
				else {
					edgeList.emplace_back(temp);
					// edgeDada[r].emplace_back(temp);
				}
			}
		}
		// 1w  [0.35, 0.56]s
		// 10w [3.72, 4.08]s
		// 10w [3.138, 3.189]s
		StandardExtend::testAndDiffClock([&]() {
			for (int i = 0; i < 100000; ++i) {
				kruskal(n, edgeList);
			}
		}, "���鼯����");
		
		cout << kruskal(n, edgeList) << endl;
	}
	return 0;
}

int main() {
	//MainJudgeConnectivity();
	mainForKruskal();
	return 0;
}
