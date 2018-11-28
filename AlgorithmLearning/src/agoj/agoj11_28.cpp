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
#include<iomanip>//cout各种操纵器

#include "ExtendSpace.h"

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

template<class ElementType>
//并查集模板类 unit join set
class DisjointSet {
public:
	//using ElementType = int;
	//用集合的root下标表示集合
	using SetRoot = int;
	using SetBaseType = vector<ElementType>;
	//using SetBaseType = ElementType[MAX_N];
	//在class出现vector在http://139.196.145.92上会Runtime Error
	//vector<ElementType> debugTemp1;

	DisjointSet() {}
	// O(N)
	DisjointSet(size_t n) {
		//新的元素初始值也必须是-1 每个元素都代表一个单独的集合
		disjointSetBase.assign(n, -1);
		//memset(disjointSetBase, -1, n*sizeof(ElementType));
	}

	// 合并两个集合 O(lgN)
	// 若发生合并返回true 否则返回false
	bool unionSet(SetRoot root1, SetRoot root2) {
		if (root1 == root2) {
			return false;
		}
		else {
			//保证小集合并入大集合
			if (disjointSetBase[root2] < disjointSetBase[root1]) {
				//1并入2
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

	// 根据元素进行合并 O(lgN)
	// 若发生合并返回true 否则返回false
	bool unionElement(ElementType x1, ElementType x2) {
		return unionSet(findRoot(x1), findRoot(x2));
	}

	// 查询指定元素的根 O(lgN)
	SetRoot findRoot(ElementType x) {
		//集合默认初始值为1
		return disjointSetBase[x] < 0 ? x//找到集合的根
			: disjointSetBase[x] = findRoot(disjointSetBase[x]);//继续寻找并进行路径压缩
	}

	// 返回指定集合的元素个数(大小) O(1)
	size_t size(SetRoot root) {
		return -disjointSetBase[root];
	}

	// 初始化 O(N)
	void init() {
		disjointSetBase.assign(disjointSetBase.size(), -1);
		//memset(disjointSetBase, -1, sizeof(SetBaseType));
	}

	// 在原来的基础上重设大小 O(N)
	void reSize(size_t size) {
		disjointSetBase.resize(size, -1);
		//memset(disjointSetBase, -1, sizeof(SetBaseType));
	}

	// 返回集合数 O(N)
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


//判断图的连通性 --并查集版
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

// 这其实是边....
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

// 返回最小生成树的权值
int kruskal(int n, ArrayList<EdgeFullNode> &edgeList) {
	// 对所有的边按照权值排序
	std::sort(edgeList.begin(), edgeList.end(), [&](EdgeFullNode const &lhs, EdgeFullNode const &rhs) {
		return lhs.weight < rhs.weight;
	});

	DisjointSet<int> nDisjointSet(n);
	int index = 0;
	int lhsVertex = -1, rhsVertex = -1;
	int unitSetCount = n;
	int sumWeight = 0;
	// 当集合数目为1, 或者不是连通图时算法中止
	// 这个方法名等会改一下 容易与geter seter误会
	while (unitSetCount != 1 && index < edgeList.size()) {
		lhsVertex = edgeList[index].originID;
		rhsVertex = edgeList[index].targetID;
		// 每次加入一条权值最小的边进入原本与之有关(通过边关联)的两个不同的集合中, 用以消除一个集合
		// 发生合并即说明其属于不同集合
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

// 最小生成树权值计算
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
	// 顶点列表
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
		}, "并查集测试");
		
		cout << kruskal(n, edgeList) << endl;
	}
	return 0;
}

int main() {
	//MainJudgeConnectivity();
	mainForKruskal();
	return 0;
}
