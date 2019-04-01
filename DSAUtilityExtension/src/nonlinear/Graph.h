#pragma once
#include"../stdafx.h"
#include"../ExtendSpace.h"
#include"./GraphUtil.h"
#include "../DSAUEdll.h"

/*
�ڽӱ�ͼ ��ҪV��ͷָ�� + 2E����㣨ÿ���������2���򣩣���eС��V(V-1)/4ʱ��ʡ�ռ��
����V������e���ߵ��ڽӱ�ͼʱ�临�Ӷ�ΪO(V+e)
����һ������n������e���ߵ�����ͼ
�����ڽӱ��ʾ��n���������2e���߱���
����һ������n������e���ߵ�����ͼ
�����ڽӱ��ʾ��n���������e���߱���
���ͼ�бߵ���ĿԶԶС��n2����ϡ��ͼ���������ڽӱ��ʾ�����ڽӾ����ʾ��ʡ�ռ�;
���ͼ�бߵ���Ŀ�ӽ���n2,��������ͼ�ӽ���n*(n-1)��������ͼ,���ǵ��ڽӱ���Ҫ�������򣬲����ڽӾ����ʾ��Ϊ�ˡ�

�������Ƕ���e < V(V-1)/4ʱ����������� ���ߵ�ʵ�����������ߵľ�������(ǰ�߾��߿ռ临�Ӷ� ���߾���ʱ�临�Ӷ�)

����䶯Ƶ��������Ҫ��ʵ��

����(dfs bfs):(��ͼ��һ��Ŀ��)
	�����������:�ڽӾ���ͼ
	�����������:
		�߲���(��ɾ��)
			Ƶ��->�ڽ�mapͼ
			ż��->�ڽӱ�ͼ

tips: ���ĳ�ֹ�ϵ�±��ر�� ���Գ��Դ���û�й�ϵ��
	�������ڽӱ���������о������� ��ʵ�ڽӾ�����Բ��ÿ��ǵ�
	����ֻ�����ڽӱ�
	�߲���Ƶ������������� ֻ������ ���� ֻ�Ĳ�ɾ->mapʵ��
	����֮����������������->listʵ��
	(�����ٵ����Ƕ����������Ը���ֱ�ӵĽ���������4*4��ȫͨ����ͼ�ĵȼ۶���ͼ��������� ���ߵı�������Լ��3ms����)
	�����֪��ʹ�ó�����ôʹ��mapʵ�������Ե�ѡ��(����Ҫע��û���Ż��Ļ��������� �����ڽӾ���ͼ�Ĵ��ڼ�ֵ���ǵ���)
*/
//��������ͼ������ (�ߵĴ���������ʵ�� ��Ĵ����Լ���ķ������ж��ɽӿ���ʵ��)  Adjacent: �ڽӵ�; Adjoin: �ڽ�
//(Graph: ͼ������ͼ)�����Ҫ����ͼ ֻʹ�������ɾ����
class DSAUTILITYEXTENSION_API Graph{
public:
	const static int INF = MAX_INT32 / 2;

	typedef int VertexKey;//����ļ�����
	typedef int WeightType;//�ߵ�Ȩֵ����
	//������������
	class VertexValue{
		//VertexKey ID;
		char data = '0';
	};
	//Ȩ��(û�г�������Ϣ�Ĵ�Ȩֵ�ı�) ����ʱ����Ȩֵ���� ��Ȩֵ�߰�Ŀ���id����
	struct IndexEdge{
		VertexKey targetID;//�ߵĶ��󶥵�ID
		WeightType weight;//�����ڸ�ֵ�Ƴ��� ���ڸ�ֵ��Ȩֵ
		IndexEdge(){
			weight = INF, targetID = -1;
		}
		IndexEdge(VertexKey targetID, WeightType weight) :weight(weight), targetID(targetID){}
		//������: Dijkstra�����ȶ����Ż� �����ʵ��
		bool operator<(IndexEdge const &rhs)const{
			return weight == rhs.weight ? targetID < rhs.targetID
				: weight < rhs.weight;
		}
		//Ϊ�˱���list�����·������ʱ�ĳ�ʼ��
		operator int()const{
			return weight;
		}
	};
	/*�����(��<=>ֱ�ӹ�ϵ)�Ķ���  tips: ownerID targetID��(Relation)��ϵӦ�ó���ownerID targetID��ͨ*/
	struct Edge :public IndexEdge{
		VertexKey ownerID;//�ߵ�ӵ����ID
		Edge(VertexKey ownerID, VertexKey targetID, WeightType weight)
			:IndexEdge(targetID, weight), ownerID(ownerID){}
	};/*�����<ownerID, targetID>*/

	//���ڶ������ݵĴ�����δ��дvertexDataδ��ʼ��
	Graph(int vertexNum);
	virtual ~Graph();

	void resize(size_t newSize);
	//���� �����
	void insertEdgeUndirected(Edge &e);
	//ɾ�� �����
	void deleteEdgeUndirected(VertexKey ownerID, VertexKey targetID);
	//����ʽ����
	virtual void insertEdge(Edge const &e) = 0;//���麯��
	virtual void deleteEdge(VertexKey ownerID, VertexKey targetID) = 0;
	//ɾ������vID������������б�
	//virtual void deleteVertex(VertexKey vID) = 0;
	//�����ڱ߽��ߵ�Ȩֵ������� ����true; ���������ȨֵΪ����� ����false
	virtual bool queryEdge(Edge &e) = 0;
	
	//(����)����������� ���ش���ʼ����startId�����յ�destId��·����
	virtual int dfs(VertexKey startId) = 0;

	//������: (����)����������� unweighted(��Ȩͼ�ĵ�Դ���·)
	//PS: �㷨�ڲ�����Ȩ��Ϊ1��˵ó��ľ���ʵ�����ǲ���  ��Ȼһ��ʵ�����бߵ�Ȩ����ȵ�ͼҲ����ʹ����Ȩͼ�ķ���
	//    dist������㵽���пɵ�����Ĳ���(origin-1step-2step) ���������Ų���(��Ȩͼ����̾� 2018-7-17)
	//    BFS��һ��äĿ��Ѱ�� ����ʹ�þ��鷨�����㷨(����ʽ����) ����������ʽ���·���㷨����Դ
	//Ч��1: ����destId->���ټ����Ƿ��ִܵ�ĳ������ ͬʱ��������Ų���(���ǵ�Ȩͼ�����µ����·��)
	//		�ŵ�: �����޸���Ȩͼ��ʵ��Ȩֵ
	//		�ж���ʽ: ʹ�� dist����� �� Ŀ������ͨ�Խ����ж�
	//Ч��2:  �ж�����ͼ����ͨ�� ����ֵ����ӵĽ����Ŀ ��δ����destIdʱ��������
	//		�ж���ʽ: ��ͨͼ�ķ���ֵӦ���������
	int bfs(VertexKey origin, JCE::ArrayList<WeightType> &dist, JCE::ArrayList<VertexKey> &predecessor);

	/*
	��Դ���·������: ������ʼ�㵽���е�����·��
	(���·�����Ʒ�������, ·����������, ��ʼԭ��)
	predecessor���ڴ洢·����Ϣ:
	predecessor[v] == infinitTemp �����ڴ�origin->v�����·��
	previousID(ǰ�� tips: ���ó���ǰһ�� ǰһ��ʱ������˼ǿһЩ ǰ���ռ�����˼��ǿ)
	�û�����·���������ⲿ�Զ�����ʶ�������(�ṩ���ʽӿ�? ���Ƕ��û�����?)

	��Ȩ�߲���ͼ:
	gg->insertEdge({ 0, 1, 2 });
	gg->insertEdge({ 0, 2, 3 });
	gg->insertEdge({ 2, 1, -2 });
	gg->insertEdge({ 2, 1, -2 });//��������Ǹ�Ȩ������ͼ
	�������
	�ɳڲ���: ��ָ����ÿ������v��V��������һ������d[v]������������Դ��s��v�����·����Ȩֵ���Ͻ磬
	��Ϊ���·�����ƣ�shortest-path estimate����

	������ɹ�(�޸�Ȩ��)����true
	*/
	bool shortestPath(VertexKey startId, JCE::ArrayList<WeightType> &dist, JCE::ArrayList<VertexKey> &path);
	/*��Դ���·����*/
	bool multisourceShortestPath(JCE::ArrayList<JCE::ArrayList<WeightType>> &dists, JCE::ArrayList<JCE::ArrayList<VertexKey>> &paths);
	
	//����Ŀ�궥��(ID) ����һ�������ڵĶ�����<==>δ����Ŀ�Ķ���
	void setDestVertex(VertexKey vertexID);
	template<class Fun>
	void setCustomVisit(Fun const &customVisit){
		this->customVisit = customVisit;
	}
	//���ر���
	size_t getEdgeNum() const;
	//���ض�����
	size_t getVertexNum() const;
protected:

	//��2017֮�±����ͨ�� ��ʵ�����������ǲ���ͨ����(�ж�������޸�) -> �ɰ汾�ı������ͨ�� �߰汾��һ��(���ܸ���������)

	//typedef list<IndexEdge const> EdgesType;
	typedef JCE::LinkedList<IndexEdge> EdgesType;
	//�������� ���������ݿ��Բ��ó��� ���������������Ķ� ����ֱ����map<VertexValue>���
	JCE::ArrayList<VertexValue> vertexData;
	//������
	size_t vertexNum;
	//���� V�������������ȫͼ����=V(V-1)/2
	int edgeNum;
	//��Ȩ�߸��� (����һ����Ȩ��++ ɾ��һ��-- ����Ϊ0ʱ���·�������dij���� �������spfa)
	int negativeWeightCount = 0;

	JCE::ArrayList<bool> visited;
	// bdfsĿ��� ��ʼ������Ŀ���(��ζ��: dfs������� bfs�ó���·���ǵ����ж����)
	VertexKey destId = -1;
	
	// �Զ������
	void(*customVisit)(VertexKey) = [](VertexKey v){
		//printf("���ʽ��%d\n", v);//������
	};
	/*
		0.ÿ�δ�Q��delete_min��O(V)��Ԫ��v�ᱻɾ��
		ÿ����չ������v�бߵĶ������̾���O(V) ����Ϊ�ڽӱ����O(E)
		�㷨����QΪ��ʱֹͣ �������V����չ����
		ʱ�临�Ӷ�O(V^2)
		1.����q���黻Ϊ����set
		v = popMin��O(1)
		˼·1 =����ʣ�µ�Ԫ��w O(V-delCount) ƽ��(V/2)
		+��ѯ�Ƿ��б�<v, w>O(1)
		˼·2 =����v�����б�e O(E)(EΪͼ�����ж���ıߵ�ƽ������)
		+��ѯ�Ƿ��ڼ�����O(1)

		ÿ��popһ��ֱ������Ϊ�չ�V��
		ǰ���ܵ�ʱ����ȷ���� V*(V/2)
		���ǵ��ߵ�ƽ������E < (V/2)ʱ��һ�����Ը���һЩ
		˼·1ʵ��: �ڽӾ���� set�ǿ�ɾ����СԪ�ص��˻�BST(ɾ�����õ���), ���ı���O(V)������>1 ���ʵ��Ч����O(V^2)���
		˼·2ʵ��: �ڽӱ��   set����С��
	*/
	/*
		���ȶ���(��)�Ż���(A Star��������������)
		https://zh.wikipedia.org/wiki/%E6%88%B4%E5%85%8B%E6%96%AF%E7%89%B9%E6%8B%89%E7%AE%97%E6%B3%95
		Ŀǰ��֪������ �Ǹ�Ȩ����ͼ��Դ���·���㷨: �Ͽ�˹���㷨
		dijkstra�㷨��ȷ��ǰ����: ����·�����ɶ̵�·�����������ġ�(��˲��ܼ��㸺Ȩ)
	*/
	bool Dijkstra(VertexKey origin, JCE::ArrayList<WeightType> &dist, JCE::ArrayList<VertexKey> &predecessor);
	/*
		https://zh.wikipedia.org/wiki/%E8%B4%9D%E5%B0%94%E6%9B%BC-%E7%A6%8F%E7%89%B9%E7%AE%97%E6%B3%95#%E9%98%9F%E5%88%97%E4%BC%98%E5%8C%96
		�������ŵ�Short Path Fast Agritem
		(�����Ż���Bellman�CFord algorithm)
		SPFA�㷨�ڸ���Ȩͼ�Ͽ�����ȫȡ��Bellman-ford�㷨��������ϡ��ͼ��Ҳ�������á�
		*/
	bool SPFA(VertexKey origin, JCE::ArrayList<WeightType> &dist, JCE::ArrayList<VertexKey> &predecessor);
	/*
		https://zh.wikipedia.org/zh-hans/Floyd-Warshall%E7%AE%97%E6%B3%95
		������ȷ��������ͼ��Ȩ�������ɴ��ڸ�Ȩ��·���������������·�����⣬ͬʱҲ�����ڼ�������ͼ�Ĵ��ݱհ�.
		ʱ�临�Ӷ�O(V^3)�ռ临�Ӷ�ΪO(V^2)
		*/
	bool FloydWarshall(JCE::ArrayList<JCE::ArrayList<WeightType>> &dists, JCE::ArrayList<JCE::ArrayList<VertexKey>> &predecessors);
private:
	//��ȡ��listBuffer
	virtual EdgesType const &getEdgeList(size_t originNum) = 0;
	//���๹���ʱ��Ӧ��Ԥ��>2���Ŀռ�
	virtual void resizeEdge(size_t newSize) = 0;
};

//�ڽӾ���ͼ
class DSAUTILITYEXTENSION_API AdjacentMatrixGraph : public Graph{
	//�ڽӾ���
	JCE::ArrayList<JCE::ArrayList<WeightType>> edgeData;
	//��ownerID��targetID֮�����ֱ�ӹ�ϵ(��)(��w�Ƿ�v���ڽӵ�) ����true
	inline bool existEdge(VertexKey v, VertexKey w);
	void resizeEdge(size_t newSize)override;
public:
	AdjacentMatrixGraph(int vertexNum);
	virtual ~AdjacentMatrixGraph();
	bool queryEdge(Edge &e)override;
	void insertEdge(Edge const &e)override;
	void deleteEdge(VertexKey ownerID, VertexKey targetID)override;
	EdgesType const &getEdgeList(size_t originNum)override;

	/*
	���֧�ݹ��㷨: past -> present -> next;
	present���� -> ���� -> ���� -> �ݹ�
	����ͼ��present�붥��Ĵ���˳���й�)����ͼ��present��λ���й��Ǳ�����ʵ� ��֧�������ٴγ��ֵ����Ծ���
	�����ʵ�������������һ��ʵ����Щ���� ������������visit��ʼ���� �����ڼ�������չ
	(����Ҫʹ�ÿ��Կ�����DIR84�м�һ��{0, 0}����ʹ��do{}while();) ��ͨ��DIR84"����"��һ�η��ʾ�û��Ҫ��ԯ������

	ͼ��dfs���ȼ��ڻ�������:
		1.ͼ�ķ����������޶���(��ϵ: ������Ҫ��������������) ��˴���Ĳ���ֻ����{ǰ��, ��ʱ, ���}�а�����ʱ������
		 ǰ������ʱ��������޹�, ���ǲ����ĺ���Ҳ��depth�޹�
		 �����ڳ�ʼ��������û��ⲿ�������Ҫ���о���, ���ֻ��ѡ����ʱ���������
		2.�ڹ�ϵ�����۶������������Ҫ��ȷ����: �ڽ��a�������ǰa�����ٱ�a�ĺ�̷���, ��˶���ͼ��Ҫ�����ʿ���
		 �ڲ����Ƿ��ʿ���ʵ�ֵ�ǰ����(һ����visited[])����ȷ: ��a�����й�ϵ�������ʱӦ���ܱ�����ƽ�н�����
		 ���Ƿ��ʿ��Ƶ�˳������� ��Ϊ����->����->��Ϊδ����
		 (˳��һ��: ��������ֻ��Ҫ�ڴﵽҪ��ʱ�������Ž��������, depth�����һ��ķ��ʿ���, ����û�н�����Ҳ����Ҫ)
		3.������ǰ���� ���ʿ���������ʵ��: ������ʱ���; �����̽��
		 ��ѡ����� ��ô���Դ�����ʼ����״̬(�⽫���𼫴�Ĳ��������)
	���ǽ�����: ���Ƕ���ͼ ����dfs���������ʵ��
	*/
	int dfs(VertexKey presentId)override;
	template<class Fun>//��������ʵ��
	int dfsTest(VertexKey presentId, Fun visit){
		customVisit(presentId);

		if (presentId == destId){
			return 1;
		}
		else{
			visited[presentId] = true;
			int count = 0;
			FOR(nextId, 0, vertexNum){
				if (edgeData[presentId][nextId] < INF //���ڹ�ϵ
					&& !visited[nextId]){
					//visited[nextId] = true;//�Ժ�̽����зÿ�
					count += dfsTest(nextId, visit);
					//visited[nextId] = false;
				}
			}
			visit(presentId);
			visited[presentId] = false;//dfs�Ļ���
			return count;
		}
	}
	/* �ڽӱ�洢 - ���������㷨: ��ͼ�в����� ��· (��·) �����ɹ� ����true*/
	bool topologySort(JCE::ArrayList<VertexKey> &topOrderBuffer);
};

//�ڽӱ�ͼ(listʵ��)
class DSAUTILITYEXTENSION_API AdjacentListGraph : public Graph{
	/*
	�ڽӱ�ͼ(mapʵ��)
	// [��һ�ߵ�id][�ߵ�Ȩֵ]
	typedef map<VertexKey, WeightType> EdgesType;
	//(map�����ͻ��Hash�� map��list�Ļ����ϸ����˱߲��� ����������)
	EdgesType edgeData[MaxVertexNum];
	һ����4*4��dfs�������е�·������û��ʲôʵ��Ӧ�ó���
	��������ʵ����Ϊ����Գ���ͼ��list��ɾ���ѵ����
	��ʵ���ϴ�ʵ�ֱ��ھطѿռ�(����4����) �����ھ�ʵ�ֵĳ���ͼ������Ч������(�������)
	���Ǿ�ֻ��ϡ��ͼ���е��� ������ɾ��ʡ�µ��ǵ�ʱ�������������ʱ�����������
	����һ������ʱ��ߵ���������ͳ��
	*/
	//list��������map
	//�ڽӱ�(��������ͻ��Hash��)
	JCE::ArrayList<EdgesType> edgeData;
	EdgesType::iterator &listFind(EdgesType &edgssList, VertexKey keyID);
	void resizeEdge(size_t newSize);
public:
	AdjacentListGraph(int vertexNum);
	virtual ~AdjacentListGraph();

	bool queryEdge(Edge &e)override;
	void insertEdge(Edge const &e)override;
	void deleteEdge(VertexKey ownerID, VertexKey targetID)override;
	//void deleteVertex(VertexKey vID) override {}

	EdgesType const &getEdgeList(size_t originNum)override;
	int dfs(VertexKey presentId)override;

	/* �ڽӱ�洢 - ���������㷨: ��ͼ�в����ڻ�· (��·) �����ɹ� ����true*/
	bool topologySort(JCE::ArrayList<VertexKey> &topOrderBuffer);
};

