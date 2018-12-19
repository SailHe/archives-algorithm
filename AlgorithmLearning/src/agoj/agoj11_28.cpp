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

// #include "ExtendSpace.h"
#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

// 其实这个集合并非真正意义上的集合 它只储存了关系 没有储存集合的元素
template<class ElementType>
//并查集模板类 unit join set
class DisjointSet {
public:
	//using ElementType = int;
	// 用集合的root下标表示集合
	using SetRoot = int;
	// 元素都表示下标
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
		// 集合默认初始值为-1 (负数表示这是一个集合的根, 绝对值表示集合大小)
		return disjointSetBase[x] < 0 ? x
			// 路径压缩: 将递归找到的集合的根赋值给父结点
			: disjointSetBase[x] = findRoot(disjointSetBase[x]);
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

// 返回最小生成树的权值 若不是连通图顶点数n变为负数 (PS: 不确定此算法是否对负权图有效)
// http://139.196.145.92/contest_show.php?cid=647#problem/B
int kruskal(int &n, ArrayList<EdgeFullNode> &edgeList) {
	// 对所有的边按照权值排序
	std::sort(edgeList.begin(), edgeList.end(), [&](EdgeFullNode const &lhs, EdgeFullNode const &rhs) {
		return lhs.weight < rhs.weight;
	});

	// 最小堆 0号位置始终最小
	/*std::make_heap(edgeList.begin(), edgeList.end(), [&](EdgeFullNode const &lhs, EdgeFullNode const &rhs) {
		return lhs.weight > rhs.weight;
	});*/
	
	DisjointSet<int> nDisjointSet(n);
	int index = 0;
	int lhsVertex = -1, rhsVertex = -1;
	int unitSetCount = n;
	int sumWeight = 0;
	int edgeCnt = edgeList.size();
	// 当集合数目为1, 或者不是连通图时算法中止
	// 这个方法名等会改一下 容易与geter seter误会
	while (unitSetCount != 1 && index < edgeCnt) {
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
		/* 将上面的index改为0即可对应最小堆版本
		std::pop_heap(edgeList.begin(), edgeList.end() - index, [&](EdgeFullNode const &lhs, EdgeFullNode const &rhs) {
			return lhs.weight > rhs.weight;
		});
		*/
		++index;
	}
	n = index < edgeCnt ? n : -1;
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
	while (cin >> n) {
		// 顶点列表
		// ArrayList<LinkedList<VertexNode>> edgeDada(n);
		// 边列表
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
		// 10w [5.437, 5.572]s  使用calcSetCount的版本
		// 10w [3.72, 4.08]s    多2次findRoot调用的版本
		// 10w [5.0, 5.4]s  对上述进行优化的版本 但排序用堆替换
		// 10w [3.138, 3.189]s  对上述进行优化的版本
		/*StandardExtend::testAndDiffClock([&]() {
			for (int i = 0; i < 100000; ++i) {
				kruskal(n, edgeList);
			}
		}, "并查集测试");*/
		
		cout << kruskal(n, edgeList) << endl;
	}
	return 0;
}

// 2018-12-19的客串 使用kruskal判断是否连通图(其实就是使用并查集啦 这里只是为了完善下kruskal函数)
int mainForSolve12_19() {
	// 顶点数 边数
	int vertexCnt, edgeCntIn;
	while (cin >> vertexCnt >> edgeCntIn && (vertexCnt != 0 || edgeCntIn != 0)) {
		// 边列表
		ArrayList<EdgeFullNode> edgeList;
		edgeList.reserve(edgeCntIn);
		EdgeFullNode temp;
		for (int r = 0; r < edgeCntIn; ++r) {
			scanf("%d%d", &temp.originID, &temp.targetID);
			--temp.originID;
			--temp.targetID;
			edgeList.emplace_back(temp);
		}
		
		cout << (kruskal(edgeCntIn, edgeList) == 0 ? "No" : "Yes") << endl;
	}
	return 0;
}


//邻接表图(list实现)
class AdjacentListGraph {
	typedef int VertexKey;//顶点的键类型
	typedef int WeightType;//边的权值类型
	//顶点数据类型
	class VertexValue {
		//VertexKey ID;
		char data = '0';
	};
public:
	//权边(没有出发点信息的带权值的边) 排序时按照权值排序 等权值者按目标点id排序
	struct IndexEdge {
		VertexKey targetID;//边的对象顶点ID
		WeightType weight;//不存在负值称长度 存在负值称权值
		IndexEdge() {
			weight = MAX_INT32 / 2, targetID = -1;
		}
		IndexEdge(VertexKey targetID, WeightType weight) :weight(weight), targetID(targetID) {}
		//可用于: Dijkstra的优先队列优化 跳表的实现
		bool operator<(IndexEdge const &rhs)const {
			return weight == rhs.weight ? targetID < rhs.targetID
				: weight < rhs.weight;
		}
		//为了便于list的最短路径计算时的初始化
		operator int()const {
			return weight;
		}
	};
	typedef LinkedList<IndexEdge> EdgesType;
	//顶点数据 顶点无数据可以不用出现 若顶点数量经常改动 可以直接用map<VertexValue>替代
	ArrayList<VertexValue> vertexData;
	//顶点数
	size_t vertexNum;
	ArrayList<EdgesType> edgeData;
	
	AdjacentListGraph(int vertexNum) {
		this->vertexNum = vertexNum;
		edgeData.reserve(2 * vertexNum + 50);
		edgeData.resize(vertexNum);
	}
	virtual ~AdjacentListGraph() {
	}

	/* 邻接表存储 - 拓扑排序算法: 若图中不存在回路 (环路) 则计算成功 返回true*/
	bool topologySort(ArrayList<VertexKey> &topOrderBuffer) {
		/* 对Graph进行拓扑排序,  topOrderBuffer[]顺序存储排序后的顶点下标 */
		topOrderBuffer.resize(vertexNum);
		ArrayList<VertexKey> indegree(vertexNum);
		//queue<VertexKey> q;
		//保证在同等排名下优先输出序号小的
		std::priority_queue<VertexKey, ArrayList<VertexKey>, greater<VertexKey>> q;

		/* 遍历图，得到indegree[] edgeData.size()*/
		for (size_t v = 0; v < vertexNum; ++v) {
			//IndexEdge
			for (auto itW = edgeData[v].begin(); itW != edgeData[v].end(); ++itW) {
				++indegree[itW->targetID];/* 对有向边<v, itW->targetID>累计终点的入度 */
			}
		}

		/* 将所有入度为0的顶点入列 */
		for (size_t v = 0; v < vertexNum; ++v) {
			if (indegree[v] == 0) {
				q.push(v);
			}
		}

		/* 下面进入拓扑排序 */
		int orderSub = -1;
		while (!q.empty()) {
			/* 弹出一个入度为0的顶点 加入结果序列 */
			VertexKey v = q.top();//q.front();
			q.pop();
			topOrderBuffer[++orderSub] = v;
			/* 对v的每个邻接点itW->targetID(W->AdjV)*/
			for (auto itW = edgeData[v].begin(); itW != edgeData[v].end(); ++itW) {
				/* 若删除v使得itW->targetID入度为0 */
				if (--indegree[itW->targetID] == 0) {
					q.push(itW->targetID);/* 则该顶点入列 */
				}
			}
		}

		return orderSub + 1 == edgeData.size();
	}
};

/*
4 3
1 2
2 3
3 4

3 1
2 3

4 4
1 2
2 3
3 4
4 1

4 4
1 2
2 3
3 4
3 1

4 2
1 2
2 3

0 0
*/
// 连通图 + 环路检测
// @see http://139.196.145.92/problem_show.php?pid=329#probsubmit
int mainForSolve12_19_B() {
	// 顶点数 边数
	int vertexCnt, edgeCntIn;
	ArrayList<int> topOrderBuffer;
	while (cin >> vertexCnt >> edgeCntIn && (vertexCnt != 0 || edgeCntIn != 0)) {
		// 边列表
		ArrayList<EdgeFullNode> edgeList;
		AdjacentListGraph g(vertexCnt);
		edgeList.reserve(edgeCntIn);
		EdgeFullNode temp;
		DisjointSet<int> nDisjointSet(vertexCnt);
		bool hasCircle = false;
		for (int r = 0; r < edgeCntIn; ++r) {
			scanf("%d%d", &temp.originID, &temp.targetID);
			--temp.originID;
			--temp.targetID;
			edgeList.emplace_back(temp);
			g.edgeData[temp.originID].push_back(AdjacentListGraph::IndexEdge(temp.targetID, temp.weight));
			int root1 = nDisjointSet.findRoot(temp.originID);
			int root2 = nDisjointSet.findRoot(temp.targetID);
			// 如果两个节点拥有相同的祖先，则再加一条边就会形成环。
			if (root1 == root2) {
				hasCircle = true;
			}
			nDisjointSet.unionElement(temp.originID, temp.targetID);
		}

		// hasCircle = !g.topologySort(topOrderBuffer);
		int disjointSetCnt = nDisjointSet.size(nDisjointSet.findRoot(0));
		bool isConnected = disjointSetCnt == vertexCnt;
		// kruskal(vertexCnt, edgeList);
		// isConnected = vertexCnt < 0;

		// 如果是(连通图 并且 没有环的话) 一棵树 就符合题意 该死的m n
		cout << ((!hasCircle && isConnected) ? "Yes" : "No") << endl;
	}
	return 0;
}

int main() {
	//MainJudgeConnectivity();
	//mainForKruskal();
	mainForSolve12_19_B();
	return 0;
}
