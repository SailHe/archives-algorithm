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
#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>
#define FOR(it, lowerBound, terminator) for (auto it = lowerBound; it != terminator; ++it)
//for����������Ԫ�� �����: ֧�ֵ�����������  (�����������end()�Ż���FOR)
#define FOR_ALL_OBJECT(container) for (auto &element : container)


using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

const int INF = MAX_INT32 / 2;

class Graph {
public:
	
	typedef int VertexKey;//����ļ�����
	typedef int WeightType;//�ߵ�Ȩֵ����
	//������������
	class VertexValue {
		//VertexKey ID;
		char data = '0';
	};
	//Ȩ��(û�г�������Ϣ�Ĵ�Ȩֵ�ı�) ����ʱ����Ȩֵ���� ��Ȩֵ�߰�Ŀ���id����
	struct IndexEdge {
		VertexKey targetID;//�ߵĶ��󶥵�ID
		WeightType weight;//�����ڸ�ֵ�Ƴ��� ���ڸ�ֵ��Ȩֵ
		IndexEdge() {
			weight = INF, targetID = -1;
		}
		IndexEdge(VertexKey targetID, WeightType weight) :weight(weight), targetID(targetID) {}
		//������: Dijkstra�����ȶ����Ż� �����ʵ��
		bool operator<(IndexEdge const &rhs)const {
			return weight == rhs.weight ? targetID < rhs.targetID
				: weight < rhs.weight;
		}
		//Ϊ�˱���list�����·������ʱ�ĳ�ʼ��
		operator int()const {
			return weight;
		}
	};
	/*�����(��<=>ֱ�ӹ�ϵ)�Ķ���  tips: ownerID targetID��(Relation)��ϵӦ�ó���ownerID targetID��ͨ*/
	struct Edge :public IndexEdge {
		VertexKey ownerID;//�ߵ�ӵ����ID
		Edge(VertexKey ownerID, VertexKey targetID, WeightType weight)
			:IndexEdge(targetID, weight), ownerID(ownerID) {}
	};/*�����<ownerID, targetID>*/

	//���ڶ������ݵĴ�����δ��дvertexDataδ��ʼ��
	Graph(int vertexNum)
		:vertexNum(vertexNum), visited(vertexNum, false) {
		edgeNum = 0;
	}
	virtual ~Graph() {
	}

	//����ʽ����
	virtual void insertEdge(Edge const &e) = 0;//���麯��
	virtual void deleteEdge(VertexKey ownerID, VertexKey targetID) = 0;
	//�����ڱ߽��ߵ�Ȩֵ������� ����true; ���������ȨֵΪ����� ����false
	virtual bool queryEdge(Edge &e) = 0;
	virtual inline WeightType queryEdgeWieght(VertexKey lhs, VertexKey rhs) = 0;
	virtual inline bool existEdge(VertexKey lhs, VertexKey rhs) = 0;

	//���ر���
	size_t getEdgeNum() const {
		return edgeNum;
	}
	//���ض�����
	size_t getVertexNum() const {
		return vertexNum;
	}
	//prim ����С��������Ȩֵ��
	int prim() {
		WeightType minWeightSum = 0;
		ArrayList<bool> isVisitedPrimed(getVertexNum(), false);
		LinkedList<VertexKey> primedList;
		if (getVertexNum() == 0) {
			// do nothing
		}
		else {
			// ��������һ����Ϊ��ʼֵ
			isVisitedPrimed[0] = true;
			primedList.push_back(0);

			while (primedList.size() != getVertexNum()) {
				WeightType minEdgeWeight = MAX_INT32;
				VertexKey exchangeId = -1;
				FOR(itLhsIndex, primedList.begin(), primedList.end()) {
					VertexKey lhsIndex = *itLhsIndex;
					// ��δ��������С�������Ķ����У��ҳ�Ȩֵ��С�Ķ������
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
				// ����ͨͼ, ��������С������
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

	// �㷨���۰汾 origin
	int MST_prim(VertexKey rootVertexId) {
		priority_queue<VertexKey> queueMin;
		// �����ж���������ȶ���
		for (VertexKey v = getVertexNum(); v >= 0; --v) {
			queueMin.push(v);
		}
		while (!queueMin.empty()) {
			auto u = queueMin.top();
			queueMin.pop();

		}
	}
protected:

	//��2017֮�±����ͨ�� ��ʵ�����������ǲ���ͨ����(�ж�������޸�) -> �ɰ汾�ı������ͨ�� �߰汾��һ��(���ܸ���������)
	//typedef list<IndexEdge const> EdgesType;
	typedef LinkedList<IndexEdge> EdgesType;
	//�������� ���������ݿ��Բ��ó��� ���������������Ķ� ����ֱ����map<VertexValue>���
	ArrayList<VertexValue> vertexData;
	//������
	size_t vertexNum;
	//���� V�������������ȫͼ����=V(V-1)/2
	int edgeNum;
	//��Ȩ�߸��� (����һ����Ȩ��++ ɾ��һ��-- ����Ϊ0ʱ���·�������dij���� �������spfa)
	int negativeWeightCount = 0;

	ArrayList<bool> visited;
private:
	//��ȡ��listBuffer
	virtual EdgesType const &getEdgeList(size_t originNum) = 0;
	//���๹���ʱ��Ӧ��Ԥ��>2���Ŀռ�
	virtual void resizeEdge(size_t newSize) = 0;
};

//�ڽӾ���ͼ
class AdjacentMatrixGraph : public Graph {
	//�ڽӾ���
	ArrayList<ArrayList<WeightType>> edgeData;
	//��ownerID��targetID֮�����ֱ�ӹ�ϵ(��)(��w�Ƿ�v���ڽӵ�) ����true
	inline bool existEdge(VertexKey v, VertexKey w) override {
		return edgeData[v][w] < INF;//�����ڵı߱���ʼ��ΪINF
	}
	void resizeEdge(size_t newSize)override {
		edgeData.resize(newSize);
		FOR(i, 0, newSize) {
			edgeData[i].resize(newSize, INF);//�ط�����ڴ汻��ΪINF
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
			//�Ѵ���: ���������� ֱ�Ӹ��¾���

			//ȷ�ϸ��¶Է�Ȩ��������Ӱ��
			if (edgeData[e.ownerID][e.targetID] < 0) {
				//����
				negativeWeightCount -= e.weight < 0 ? 0 : 1;
			}
			else {
				//����
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
		//ֻҪ�б߾�ɾ��
		if (existEdge(ownerID, targetID)) {
			negativeWeightCount -= edgeData[ownerID][targetID] < 0 ? 1 : 0;
			edgeData[ownerID][targetID] = INF;
			--edgeNum;
		}
		else {
			//������;
		}
	}
	EdgesType const &getEdgeList(size_t originNum)override {
		static EdgesType edgesBuffer;//�߶�ȡ����
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

// �Ƿ���ͨͼ
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

