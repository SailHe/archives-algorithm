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

// #include "ExtendSpace.h"
#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

// ��ʵ������ϲ������������ϵļ��� ��ֻ�����˹�ϵ û�д��漯�ϵ�Ԫ��
template<class ElementType>
//���鼯ģ���� unit join set
class DisjointSet {
public:
	//using ElementType = int;
	// �ü��ϵ�root�±��ʾ����
	using SetRoot = int;
	// Ԫ�ض���ʾ�±�
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
		// ����Ĭ�ϳ�ʼֵΪ-1 (������ʾ����һ�����ϵĸ�, ����ֵ��ʾ���ϴ�С)
		return disjointSetBase[x] < 0 ? x
			// ·��ѹ��: ���ݹ��ҵ��ļ��ϵĸ���ֵ�������
			: disjointSetBase[x] = findRoot(disjointSetBase[x]);
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

	// ������geter seter�������
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

// �߱�Դ�� Ŀ��� Ȩֵ ���������Եı���
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

// ��ͨ���ж� �������鼯 (������, �������ıߵ��б�) O(EdgeCnt*lg(VertexCnt))
// ����ͨ����true ���򷵻�false
bool isConnectGraphEdges(int vertexCnt, ArrayList<EdgeFullNode> const &edgeList) {
	// _ASSERT_EXPR((int)edgeList.size() > 0, "ǿ��ת��ʧ��!");
	DisjointSet<int> nDisjointSet(vertexCnt);
	auto endIt = edgeList.end();
	for (auto it = edgeList.begin(); it != endIt; ++it) {
		nDisjointSet.unionElement(it->originID, it->targetID);
	}
	// ��ѯԪ��0(����һ��Ԫ�ؼ���)�����ļ��ϵĴ�С
	int disjointSetCnt = nDisjointSet.size(nDisjointSet.findRoot(0));
	return disjointSetCnt == vertexCnt;
}

// ��·��� �������鼯 (������, �������ıߵ��б�) O(EdgeCnt*lg(VertexCnt))
// �����ڻ�·����true ���򷵻�false
bool existCircleGraphEdges(int vertexCnt, ArrayList<EdgeFullNode> const &edgeList) {
	DisjointSet<int> nDisjointSet(vertexCnt);
	auto endIt = edgeList.end();
	int originRoot = -1, targetRoot = -1;
	bool hasCircle = false;
	for (auto it = edgeList.begin(); it != endIt; ++it) {
		originRoot = nDisjointSet.findRoot(it->originID);
		targetRoot = nDisjointSet.findRoot(it->targetID);
		// ��������ڵ�ӵ����ͬ�����ȣ����ټ�һ���߾ͻ��γɻ���
		if (originRoot == targetRoot) {
			// ��ʱit��Դ���Ŀ���������ͬһ���ϵ�(��ͨ��) �ټ���(�ϲ�Դ��ļ�����Ŀ���ļ���)��itʱ�ͻ����뻷·
			hasCircle = true;
			break;
		}
		nDisjointSet.unionSet(originRoot, targetRoot);
	}
	return hasCircle;
}

// ���ͼ�Ƿ���һ���� (û�л�����ͨͼ) O(EdgeCnt*lg(VertexCnt))
// ��һ��������true ���򷵻�false
bool isGraphTree(int vertexCnt, ArrayList<EdgeFullNode> const &edgeList) {
	// �ٹ���һ�β��鼯 Ҫ��һ����... + ���߼���·
	// return !existCircleGraphEdges(vertexCnt, edgeList) && isConnectGraphEdges(vertexCnt, edgeList);
	DisjointSet<int> nDisjointSet(vertexCnt);
	auto endIt = edgeList.end();
	int originRoot = -1, targetRoot = -1;
	bool hasCircle = false;
	for (auto it = edgeList.begin(); it != endIt; ++it) {
		originRoot = nDisjointSet.findRoot(it->originID);
		targetRoot = nDisjointSet.findRoot(it->targetID);
		if (originRoot == targetRoot) {
			hasCircle = true;
			break;
		}
		nDisjointSet.unionSet(originRoot, targetRoot);
	}
	return !hasCircle && vertexCnt == nDisjointSet.size(nDisjointSet.findRoot(0));
}

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


// ������С��������Ȩֵ (��Ȩͼ��Ч)
// ������С��������Ȩֵ; ��������ͨͼ->����������(vertexCnt)��Ϊ����
// V=vertexCnt; E = edgeCnt; �㷨ƽ��ʱ�临�Ӷ�: max(O(E*lgV), O(E*lgE))
// http://139.196.145.92/contest_show.php?cid=647#problem/B
int kruskal(int &vertexCnt, ArrayList<EdgeFullNode> &edgeList) {
	// �����еı߰���Ȩֵ���� O(E*lgE)
	std::sort(edgeList.begin(), edgeList.end(), [&](EdgeFullNode const &lhs, EdgeFullNode const &rhs) {
		return lhs.weight < rhs.weight;
	});

	// ��С�� 0��λ��ʼ����С
	/*std::make_heap(edgeList.begin(), edgeList.end(), [&](EdgeFullNode const &lhs, EdgeFullNode const &rhs) {
		return lhs.weight > rhs.weight;
	});*/
	
	DisjointSet<int> nDisjointSet(vertexCnt);
	int index = 0;
	int lhsVertex = -1, rhsVertex = -1;
	int unitSetCount = vertexCnt;
	int sumWeight = 0;
	int edgeCnt = edgeList.size();
	// ��������ĿΪ1, ���߲�����ͨͼʱ�㷨��ֹ O(E*lgV)
	while (unitSetCount != 1 && index < edgeCnt) {
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
		/* �������index��Ϊ0���ɶ�Ӧ��С�Ѱ汾
		std::pop_heap(edgeList.begin(), edgeList.end() - index, [&](EdgeFullNode const &lhs, EdgeFullNode const &rhs) {
			return lhs.weight > rhs.weight;
		});
		*/
		++index;
	}
	/*
	0 - 1
	    |
	3 - 2
	*/
	// �������index����3 ��������Ҳֻ��1��(��ͨ) ��˲�����index�ж� �����������ֻ��һ���޶�, ���Ƕ���
	// vertexCnt = index < edgeCnt ? vertexCnt : -1;
	vertexCnt = unitSetCount == 1 ? vertexCnt : -1;
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
	while (cin >> n) {
		// �����б�
		// ArrayList<LinkedList<VertexNode>> edgeDada(n);
		// ���б�
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
		// 10w [5.437, 5.572]s  ʹ��calcSetCount�İ汾
		// 10w [3.72, 4.08]s    ��2��findRoot���õİ汾
		// 10w [5.0, 5.4]s  �����������Ż��İ汾 �������ö��滻
		// 10w [3.138, 3.189]s  �����������Ż��İ汾
		/*StandardExtend::testAndDiffClock([&]() {
			for (int i = 0; i < 100000; ++i) {
				kruskal(n, edgeList);
			}
		}, "���鼯����");*/
		
		cout << kruskal(n, edgeList) << endl;
	}
	return 0;
}

// 2018-12-19�Ŀʹ� ʹ��kruskal�ж��Ƿ���ͨͼ(��ʵ����ʹ�ò��鼯�� ����ֻ��Ϊ��������kruskal����)
int mainForSolve12_19() {
	// ������ ����
	int vertexCnt, edgeCntIn;
	while (cin >> vertexCnt >> edgeCntIn && (vertexCnt != 0 || edgeCntIn != 0)) {
		// ���б�
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


//�ڽӱ�ͼ(listʵ��)
class AdjacentListGraph {
	typedef int VertexKey;//����ļ�����
	typedef int WeightType;//�ߵ�Ȩֵ����
	//������������
	class VertexValue {
		//VertexKey ID;
		char data = '0';
	};
public:
	//Ȩ��(û�г�������Ϣ�Ĵ�Ȩֵ�ı�) ����ʱ����Ȩֵ���� ��Ȩֵ�߰�Ŀ���id����
	struct IndexEdge {
		VertexKey targetID;//�ߵĶ��󶥵�ID
		WeightType weight;//�����ڸ�ֵ�Ƴ��� ���ڸ�ֵ��Ȩֵ
		IndexEdge() {
			weight = MAX_INT32 / 2, targetID = -1;
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
	typedef LinkedList<IndexEdge> EdgesType;
	//�������� ���������ݿ��Բ��ó��� ���������������Ķ� ����ֱ����map<VertexValue>���
	ArrayList<VertexValue> vertexData;
	//������
	size_t vertexNum;
	ArrayList<EdgesType> edgeData;
	
	AdjacentListGraph(int vertexNum) {
		this->vertexNum = vertexNum;
		edgeData.reserve(2 * vertexNum + 50);
		edgeData.resize(vertexNum);
	}
	virtual ~AdjacentListGraph() {
	}

	// �ڽӱ�洢 - ���������㷨
	// ��ͼ�в����ڻ�· (��·) �����ɹ� ����true ���򷵻�false
	// PS: ���ܴ����ظ��ı�
	bool topologySort(ArrayList<VertexKey> &topOrderBuffer) {
		/* ��Graph������������,  topOrderBuffer[]˳��洢�����Ķ����±� */
		topOrderBuffer.resize(vertexNum);
		ArrayList<VertexKey> indegree(vertexNum);
		//queue<VertexKey> q;
		//��֤��ͬ������������������С��
		std::priority_queue<VertexKey, ArrayList<VertexKey>, greater<VertexKey>> q;

		/* ����ͼ���õ�indegree[] edgeData.size()*/
		for (size_t v = 0; v < vertexNum; ++v) {
			//IndexEdge
			for (auto itW = edgeData[v].begin(); itW != edgeData[v].end(); ++itW) {
				++indegree[itW->targetID];/* �������<v, itW->targetID>�ۼ��յ����� */
			}
		}

		/* ���������Ϊ0�Ķ������� */
		for (size_t v = 0; v < vertexNum; ++v) {
			if (indegree[v] == 0) {
				q.push(v);
			}
		}

		/* ��������������� */
		int orderSub = -1;
		while (!q.empty()) {
			/* ����һ�����Ϊ0�Ķ��� ���������� */
			VertexKey v = q.top();//q.front();
			q.pop();
			topOrderBuffer[++orderSub] = v;
			/* ��v��ÿ���ڽӵ�itW->targetID(W->AdjV)*/
			for (auto itW = edgeData[v].begin(); itW != edgeData[v].end(); ++itW) {
				/* ��ɾ��vʹ��itW->targetID���Ϊ0 */
				if (--indegree[itW->targetID] == 0) {
					q.push(itW->targetID);/* ��ö������� */
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

�Ƿ����
*/
// ��ͨͼ + ��·���
// Ϊ�˷�ֹ������m n������ ���ֻҪͬʱ����m n����Ҫ�������������������(����������Ҫʹ�ø����㷨�ӿڵ�ʱ��)
// �ύ: @see http://139.196.145.92/problem_show.php?pid=329#probsubmit
int mainForSolve12_19_B() {
	// ������ ����
	int vertexCnt, edgeCntIn;
	ArrayList<int> topOrderBuffer;
	while (cin >> vertexCnt >> edgeCntIn && (vertexCnt != 0 || edgeCntIn != 0)) {
		// ���б�
		ArrayList<EdgeFullNode> edgeList;
		AdjacentListGraph g(vertexCnt);
		edgeList.reserve(edgeCntIn);
		EdgeFullNode temp;
		DisjointSet<int> nDisjointSet(vertexCnt);
		for (int r = 0; r < edgeCntIn; ++r) {
			scanf("%d%d", &temp.originID, &temp.targetID);
			--temp.originID;
			--temp.targetID;
			edgeList.emplace_back(temp);
			g.edgeData[temp.originID].push_back(AdjacentListGraph::IndexEdge(temp.targetID, temp.weight));
		}
		// �������� �жϻ�·�㷨��bug /memory���и����������cpp
		bool hasCircle = !g.topologySort(topOrderBuffer);
		kruskal(vertexCnt, edgeList);
		bool isConnected = vertexCnt >= 0;

		// �����ͼ���˻���(û�л� �� ��ͨ)��ͼ �ͷ�������
		// cout << ((isGraphTree(vertexCnt, edgeList)) ? "Yes" : "No") << endl;
		// cout << ((isConnected && !existCircleGraphEdges(vertexCnt, edgeList)) ? "Yes" : "No") << endl;
		cout << ((isConnected && !hasCircle) ? "Yes" : "No") << endl;
	}
	return 0;
}

int main() {
	//MainJudgeConnectivity();
	//mainForKruskal();
	mainForSolve12_19_B();
	return 0;
}
