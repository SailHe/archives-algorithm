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
#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>
#define FOR(it, lowerBound, terminator) for (auto it = lowerBound; it != terminator; ++it)
//for容器内所有元素 宏参数: 支持迭代器的容器  (针对容器作了end()优化的FOR)
#define FOR_ALL_OBJECT(container) for (auto &element : container)


using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

const int INF = MAX_INT32 / 2;

class Graph {
public:
	
	typedef int VertexKey;//顶点的键类型
	typedef int WeightType;//边的权值类型
	//顶点数据类型
	class VertexValue {
		//VertexKey ID;
		char data = '0';
	};
	//权边(没有出发点信息的带权值的边) 排序时按照权值排序 等权值者按目标点id排序
	struct IndexEdge {
		VertexKey targetID;//边的对象顶点ID
		WeightType weight;//不存在负值称长度 存在负值称权值
		IndexEdge() {
			weight = INF, targetID = -1;
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
	/*有向边(边<=>直接关系)的定义  tips: ownerID targetID有(Relation)关系应该称作ownerID targetID连通*/
	struct Edge :public IndexEdge {
		VertexKey ownerID;//边的拥有者ID
		Edge(VertexKey ownerID, VertexKey targetID, WeightType weight)
			:IndexEdge(targetID, weight), ownerID(ownerID) {}
	};/*有向边<ownerID, targetID>*/

	//存在顶点数据的代码暂未编写vertexData未初始化
	Graph(int vertexNum)
		:vertexNum(vertexNum), visited(vertexNum, false) {
		edgeNum = 0;
	}
	virtual ~Graph() {
	}

	//更新式插入
	virtual void insertEdge(Edge const &e) = 0;//纯虚函数
	virtual void deleteEdge(VertexKey ownerID, VertexKey targetID) = 0;
	//若存在边将边的权值赋予参数 返回true; 否则参数的权值为无穷大 返回false
	virtual bool queryEdge(Edge &e) = 0;
	virtual inline WeightType queryEdgeWieght(VertexKey lhs, VertexKey rhs) = 0;
	virtual inline bool existEdge(VertexKey lhs, VertexKey rhs) = 0;

	//返回边数
	size_t getEdgeNum() const {
		return edgeNum;
	}
	//返回顶点数
	size_t getVertexNum() const {
		return vertexNum;
	}
	//prim 求最小生成树的权值和
	int prim() {
		WeightType minWeightSum = 0;
		ArrayList<bool> isVisitedPrimed(getVertexNum(), false);
		LinkedList<VertexKey> primedList;
		if (getVertexNum() == 0) {
			// do nothing
		}
		else {
			// 加入任意一个作为初始值
			isVisitedPrimed[0] = true;
			primedList.push_back(0);

			while (primedList.size() != getVertexNum()) {
				WeightType minEdgeWeight = MAX_INT32;
				VertexKey exchangeId = -1;
				FOR(itLhsIndex, primedList.begin(), primedList.end()) {
					VertexKey lhsIndex = *itLhsIndex;
					// 在未被加入最小生成树的顶点中，找出权值最小的顶点加入
					FOR_ALL_OBJECT(getEdgeList(lhsIndex)) {
						if (!isVisitedPrimed[element.targetID]) {
							//queryEdgeWieght(lhsIndex, rhsIndex)
							WeightType weight = element.weight;
							if (minEdgeWeight > weight) {
								minEdgeWeight = weight;
								exchangeId = element.targetID;
							}
						}
						else {
							// do nothing
						}
					}
				}
				// 非连通图, 不存在最小生成树
				if (exchangeId == -1) {
					return -1;
				}
				isVisitedPrimed[exchangeId] = true;
				primedList.push_back(exchangeId);
				minWeightSum += minEdgeWeight;
			}
		}
		return minWeightSum;
	}

	// 算法导论版本 origin
	int MST_prim(VertexKey rootVertexId) {
		priority_queue<VertexKey> queueMin;
		// 将所有顶点加入优先队列
		for (VertexKey v = getVertexNum(); v >= 0; --v) {
			queueMin.push(v);
		}
		while (!queueMin.empty()) {
			auto u = queueMin.top();
			queueMin.pop();

		}
	}
protected:

	//在2017之下编译会通过 但实际上这样子是不能通过的(有对其进行修改) -> 旧版本的编译可以通过 高版本不一定(功能更加完善了)
	//typedef list<IndexEdge const> EdgesType;
	typedef LinkedList<IndexEdge> EdgesType;
	//顶点数据 顶点无数据可以不用出现 若顶点数量经常改动 可以直接用map<VertexValue>替代
	ArrayList<VertexValue> vertexData;
	//顶点数
	size_t vertexNum;
	//边数 V个顶点的无向完全图边数=V(V-1)/2
	int edgeNum;
	//负权边个数 (插入一条负权边++ 删除一条-- 当其为0时最短路径会调用dij计算 否则调用spfa)
	int negativeWeightCount = 0;

	ArrayList<bool> visited;
private:
	//获取边listBuffer
	virtual EdgesType const &getEdgeList(size_t originNum) = 0;
	//子类构造的时候应该预留>2倍的空间
	virtual void resizeEdge(size_t newSize) = 0;
};

//邻接矩阵图
class AdjacentMatrixGraph : public Graph {
	//邻接矩阵
	ArrayList<ArrayList<WeightType>> edgeData;
	//若ownerID与targetID之间存在直接关系(边)(即w是否v的邻接点) 返回true
	inline bool existEdge(VertexKey v, VertexKey w) override {
		return edgeData[v][w] < INF;//不存在的边被初始化为INF
	}
	void resizeEdge(size_t newSize)override {
		edgeData.resize(newSize);
		FOR(i, 0, newSize) {
			edgeData[i].resize(newSize, INF);//重分配的内存被置为INF
		}
	}
public:
	AdjacentMatrixGraph(int vertexNum)
		:Graph(vertexNum) {
		edgeData.reserve(2 * vertexNum + 50);
		edgeData.resize(vertexNum);
		FOR(i, 0, vertexNum) {
			edgeData[i].reserve(2 * vertexNum + 50);
			edgeData[i].assign(vertexNum, INF);
		}
	}
	virtual ~AdjacentMatrixGraph() {
	}

	inline WeightType queryEdgeWieght(VertexKey v, VertexKey w) override {
		return edgeData[v][w];
	}
	bool queryEdge(Edge &e)override {
		if (existEdge(e.ownerID, e.targetID)) {
			e.weight = edgeData[e.ownerID][e.targetID];
			return true;
		}
		else {
			e.weight = INF;
			return false;
		}
	}
	void insertEdge(Edge const &e)override {
		if (existEdge(e.ownerID, e.targetID)) {
			//已存在: 边数不增加 直接更新就行

			//确认更新对否负权边数量的影响
			if (edgeData[e.ownerID][e.targetID] < 0) {
				//减少
				negativeWeightCount -= e.weight < 0 ? 0 : 1;
			}
			else {
				//增加
				negativeWeightCount += e.weight < 0 ? 1 : 0;
			}
		}
		else {
			negativeWeightCount += e.weight < 0 ? 1 : 0;
			++edgeNum;
		}
		edgeData[e.ownerID][e.targetID] = e.weight;
	}
	void deleteEdge(VertexKey ownerID, VertexKey targetID)override {
		//只要有边就删除
		if (existEdge(ownerID, targetID)) {
			negativeWeightCount -= edgeData[ownerID][targetID] < 0 ? 1 : 0;
			edgeData[ownerID][targetID] = INF;
			--edgeNum;
		}
		else {
			//不存在;
		}
	}
	EdgesType const &getEdgeList(size_t originNum)override {
		static EdgesType edgesBuffer;//边读取缓冲
		edgesBuffer.clear();
		FOR(i, 0, vertexNum) {
			if (existEdge(originNum, i)) {
				edgesBuffer.emplace_back(i, edgeData[originNum][i]);
			}
		}
		return edgesBuffer;
	}

};

/*
@see http://139.196.145.92/contest_show.php?cid=635#problem/C
5
0 41 67 34 0
41 0 69 24 78
67 69 0 58 62
34 24 58 0 64
0 78 62 64 0

5
0 41 67 34 0
41 0 69 24 78
67 69 0 58 62
34 24 58 0 64
0 78 62 64 0
*/
int mainForSolveC_11_21() {
	int n;
	while (cin >> n) {
		Graph *g = new AdjacentMatrixGraph(n);
		int weightBuffer;
		for (int r = 0; r < n; ++r) {
			for (int c = 0; c < n; ++c) {
				scanf("%d", &weightBuffer);
				if (r == c) {
					//do nothing
				}
				else {
					if (r == 3 || c == 3) {
						continue;
					}
					g->insertEdge(Graph::Edge(r, c, weightBuffer));
				}
			}
		}
		int result = g->prim();
		cout << result << endl;
		delete g;
		g = nullptr;
	}
	return 0;
}

// 是否连通图
int mainForSolveD_11_21() {
	int nV, nE;
	while (cin >> nV >> nE) {
		Graph *g = new AdjacentMatrixGraph(nV);
		for (int i = 0; i < nE; ++i) {
			int originId, targetId;
			cin >> originId >> targetId;
			g->insertEdge(Graph::Edge(originId-1, targetId-1, 1));
		}
		cout << (g->prim() == -1 ? "No" : "Yes") << endl;
		delete g;
		g = nullptr;
	}
	return 0;
}

