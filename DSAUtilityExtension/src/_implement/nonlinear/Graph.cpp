#include "../../nonlinear/Graph.h"


Graph::Graph(int vertexNum)
	:vertexNum(vertexNum), visited(vertexNum, false) {
	edgeNum = 0;
}
Graph::~Graph() {
}

void Graph::resize(size_t newSize) {
	resizeEdge(newSize);
	vertexData.resize(newSize);
	visited.resize(newSize, false);
	//destId = -1;
	if (newSize < vertexNum) {
		//���¼������
		edgeNum = 0;
		negativeWeightCount = 0;
		FOR(i, 0, newSize) {
			auto edges = getEdgeList(i);
			edgeNum += edges.size();
			for (auto &element : edges) {
				if (element.weight < 0) {
					++negativeWeightCount;
				}
			}
		}
	}
	else {
		//Do nothing
	}
	vertexNum = newSize;//resizeVertex
}
void Graph::insertEdgeUndirected(Edge &e) {
	insertEdge(e);
	std::swap(e.ownerID, e.targetID);
	insertEdge(e);
}
void Graph::deleteEdgeUndirected(VertexKey ownerID, VertexKey targetID) {
	deleteEdge(ownerID, targetID);
	deleteEdge(targetID, ownerID);
}

int Graph::bfs(VertexKey origin, JCE::ArrayList<WeightType> &dist, JCE::ArrayList<VertexKey> &predecessor) {
	static std::queue<VertexKey> buffer;
	dist.assign(vertexNum, -1);
	predecessor.assign(vertexNum, -1);
	dist[origin] = 0;
	buffer.push(origin);
	int cnt = 0;

	while (!buffer.empty()) {
		origin = buffer.front();
		buffer.pop();
		customVisit(origin);
		//ÿ���ɵ�������������������һ��
		++cnt;
		if (origin == destId) {
			//���û�������Ŀ�ĵ��㷨�ڼ������Ŀ�ĵ��·��ʱ����� û����ʱӦ��֤��Ϊ��Чֵ-1
			break;
		}
		//visited[origin] = true;//���������ǰdist==-1��ʾδ����, ���滻��visited
		auto &edges = getEdgeList(origin);
		for (auto &element : edges) {
			if (dist[element.targetID] == -1) {//��δ�����ʹ�
				dist[element.targetID] = dist[origin] + 1;//�������
				predecessor[element.targetID] = origin;//·����¼
				buffer.push(element.targetID);
			}
		}
		//visited[origin] = false;//bfs�޷�����
	}
	return cnt;
}

bool Graph::shortestPath(VertexKey startId, JCE::ArrayList<WeightType> &dist, JCE::ArrayList<VertexKey> &path) {
	if (negativeWeightCount == 0) {
		return Dijkstra(startId, dist, path);
	}
	else {
		return SPFA(startId, dist, path);
	}
}
bool Graph::multisourceShortestPath(JCE::ArrayList<JCE::ArrayList<WeightType>> &dists, JCE::ArrayList<JCE::ArrayList<VertexKey>> &paths) {
	return FloydWarshall(dists, paths);
}

void Graph::setDestVertex(VertexKey vertexID) {
	destId = vertexID;
}

size_t Graph::getEdgeNum() const {
	return edgeNum;
}
size_t Graph::getVertexNum() const {
	return vertexNum;
}

bool Graph::Dijkstra(VertexKey origin, JCE::ArrayList<WeightType> &dist, JCE::ArrayList<VertexKey> &predecessor) {
	//��̾������dist���ȶ��� ÿ���������һ�� �ɳ�һ��
	static std::priority_queue<IndexEdge> q;
	static VertexKey v = -1;
	dist.assign(vertexNum, INF);
	predecessor.assign(vertexNum, -1);
	dist[origin] = 0;//��֪ԭ�㵽�������̾���Ϊ0
	//�ʼq=����V(��ʵ����ÿ����Ҫ��ֻ����Сֵ ���ֻ��Ҫ������֪�ķ�������̾�����ƹ��ƵĶ��������)
	q.push({ origin, dist[origin] });//{targetID, weight}

	//�����㷨
	while (!q.empty()) {
		v = q.top().targetID; q.pop();
		auto &edges = getEdgeList(v);
		//��ͼ����v������ÿ����<v, w>�Ķ���w������չ�ɳڲ���
		//FOR_ALL_OBJECT(edges) { // ����cpp������������ʾ�����ҵ�ʵ��...
		for (auto &element : edges) {
			//�ɳڲ���: ���ɳڹ��Ķ��㲻�ᱻ���ɳڵ��Ż� ������ֻ�����Ǹ�Ȩ�ߵ����
			if (dist[v] + element.weight < dist[element.targetID]) {
				if (element.weight < 0)
					return false;//����:�и�Ȩ��
				dist[element.targetID] = dist[v] + element.weight;//����w�����·���������
				predecessor[element.targetID] = v;//����w�����·��ǰ�����
				q.emplace(element.targetID, dist[element.targetID]);
			}
		}
		/*//mi zhi
		if (v == this->destId){
			while (!q.empty()){
				q.pop();
			}
		}*/
	}
	return true;
}

bool Graph::SPFA(VertexKey origin, JCE::ArrayList<WeightType> &dist, JCE::ArrayList<VertexKey> &predecessor) {
	static JCE::ArrayList<int> countEnQ;//��¼������Ӵ����������жϸ�Ȩ��·
	static JCE::ArrayList<bool> existInQ;//��־���飬�ж��Ƿ��ڶ�����
	static JCE::ArrayList<int> stepCount;//��¼����
	static JCE::LinkedList<VertexKey> q;//�������

	countEnQ.assign(vertexNum, 0);
	existInQ.assign(vertexNum, false);
	stepCount.assign(vertexNum, 0);
	dist.assign(vertexNum, INF);//��ʼ������Ϊ�����
	predecessor.assign(vertexNum, -1);//��ʼ��·��Ϊδ֪

	countEnQ[origin] = 1;
	existInQ[origin] = true;
	//��֪��ԭ����̾���Ϊ0 ����ȫΪ�����
	dist[origin] = 0;
	q.push_back(origin);

	while (!q.empty()) {
		auto v = q.front();
		q.pop_front();
		existInQ[v] = false;
		auto &edges = getEdgeList(v);
		for (auto &element : edges) {
			//�÷����Ż��Ķ���v�������еı�<v, w>��Ӧ�Ķ���w�����ɳڲ���
			if (element.weight + dist[v] <= dist[element.targetID]) {
				/* ���ָ��ŵ�Ԫ��w: �����Ż� || �Ⱦ���Ĳ����Ż� */
				if (element.weight + dist[v] < dist[element.targetID] || stepCount[element.targetID] > stepCount[v] + 1) {

					dist[element.targetID] = element.weight + dist[v];
					predecessor[element.targetID] = v;//����ǰ�����
					stepCount[element.targetID] = stepCount[v] + 1;
					if (++countEnQ[element.targetID] == vertexNum) {
						return false;//��Ȩ��·
					}
					/*
					(�п��������µ��Ż� <==> ���·������ֵ��������)��Ԫ��w
					�����ڶ����е�w���
					*/
					if (!existInQ[element.targetID]) {
						//���зǿ������ڶ���(SLF)
						if (!q.empty() && dist[element.targetID] < dist[q.front()])
							q.push_front(element.targetID);//���ڶ���
						else
							q.push_back(element.targetID);//������ڶ�β
						existInQ[element.targetID] = true;
					}
				}
			}
		}
	}
	return true;
}

bool Graph::FloydWarshall(JCE::ArrayList<JCE::ArrayList<WeightType>> &dists, JCE::ArrayList<JCE::ArrayList<VertexKey>> &predecessors) {

	dists.resize(vertexNum);
	predecessors.resize(vertexNum);
	FOR(i, 0, vertexNum) {
		dists[i].assign(vertexNum, INF);
		predecessors[i].assign(vertexNum, -1);
		auto &edges = getEdgeList(i);
		for (auto &element : edges) {
			//��Ȩ�س�ʼ��dist û�бߵĳ�ʼ��ΪINF
			dists[i][element.targetID] = element.weight;
			//��ʼ��ǰ�� û�бߵĶ���ǰ��Ϊ-1
			predecessors[i][element.targetID] = i;
		}
		dists[i][i] = 0;//���Լ��ľ������Ϊ0
		predecessors[i][i] = -1;//�Լ�û��ǰ��
	}

	FOR(k, 0, vertexNum) {
		FOR(i, 0, vertexNum) {
			FOR(j, 0, vertexNum) {
				if (dists[i][k] + dists[k][j] < dists[i][j]) {
					if (i == j && dists[i][j] < 0) {
						//���ڸ�ֵȦ: ������ȷ���
						return false;
					}
					else {
						/*
						�и�Ȩʱ: -1 + MAX_INT32 < MAX_INT32
						����������жϼ���dists[i][j] < INF && �ᵼ������������ΪMAX_INT32�ı��޷��ɳ�
						��������ϼ����Ƿ��������
						*/
						if (dists[i][k] != INF && dists[k][j] != INF) {
							dists[i][j] = dists[i][k] + dists[k][j];
							predecessors[i][j] = k;
						}
						else {
							//MAX_INT32 + MAX_INT32 = MAX_INT32;Do nothing
						}

					}
				}
			}
		}
	}

	return true;//�㷨ִ�����
}



// ===============================

inline bool AdjacentMatrixGraph::existEdge(VertexKey v, VertexKey w) {
	return edgeData[v][w] < INF;//�����ڵı߱���ʼ��ΪINF
}
void AdjacentMatrixGraph::resizeEdge(size_t newSize) {
	edgeData.resize(newSize);
	FOR(i, 0, newSize) {
		edgeData[i].resize(newSize, INF);//�ط�����ڴ汻��ΪINF
	}
}

AdjacentMatrixGraph::AdjacentMatrixGraph(int vertexNum)
	:Graph(vertexNum) {
	edgeData.reserve(2 * vertexNum + 50);
	edgeData.resize(vertexNum);
	FOR(i, 0, vertexNum) {
		edgeData[i].reserve(2 * vertexNum + 50);
		edgeData[i].assign(vertexNum, INF);
	}
}
AdjacentMatrixGraph::~AdjacentMatrixGraph() {
}
bool AdjacentMatrixGraph::queryEdge(Edge &e) {
	if (existEdge(e.ownerID, e.targetID)) {
		e.weight = edgeData[e.ownerID][e.targetID];
		return true;
	}
	else {
		e.weight = INF;
		return false;
	}
}
void AdjacentMatrixGraph::insertEdge(Edge const &e) {
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
void AdjacentMatrixGraph::deleteEdge(VertexKey ownerID, VertexKey targetID) {
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
AdjacentMatrixGraph::EdgesType const &AdjacentMatrixGraph::getEdgeList(size_t originNum) {
	static EdgesType edgesBuffer;//�߶�ȡ����
	edgesBuffer.clear();
	FOR(i, 0, vertexNum) {
		if (existEdge(originNum, i)) {
			edgesBuffer.emplace_back(i, edgeData[originNum][i]);
		}
	}
	return edgesBuffer;
}

int AdjacentMatrixGraph::dfs(VertexKey presentId) {
	customVisit(presentId);

	if (presentId == destId) {
		return 1;
	}
	else {
		visited[presentId] = true;
		int count = 0;
		FOR(nextId, 0, vertexNum) {
			if (edgeData[presentId][nextId] < INF //���ڹ�ϵ
				&& !visited[nextId]) {
				//visited[nextId] = true;//��ͨ����, �Ժ�̽����зÿ�
				count += dfs(nextId);
				//visited[nextId] = false;
			}
		}
		visited[presentId] = false;//dfs�Ļ���
		return count;
	}
}
bool AdjacentMatrixGraph::topologySort(JCE::ArrayList<VertexKey> &topOrderBuffer) {
	_ASSERT_EXPR(false, "not impl");
	/* ��Graph������������,  topOrderBuffer[]˳��洢�����Ķ����±� */
	topOrderBuffer.resize(vertexNum);
	JCE::ArrayList<VertexKey> indegree(vertexNum);
	//queue<VertexKey> q;
	//��֤��ͬ������������������С��
	std::priority_queue<VertexKey, JCE::ArrayList<VertexKey>, JCE::greater<VertexKey>> q;
	return false;
}


// ===============================

AdjacentListGraph::EdgesType::iterator &AdjacentListGraph::listFind(EdgesType &edgssList, VertexKey keyID) {
	static EdgesType::iterator it;
	it = find_if(edgssList.begin(), edgssList.end(), [&](IndexEdge const &i) {
		return keyID == i.targetID;
	});
	return it;
}
void AdjacentListGraph::resizeEdge(size_t newSize) {
	edgeData.resize(newSize);
}
AdjacentListGraph::AdjacentListGraph(int vertexNum) :Graph(vertexNum) {
	edgeData.reserve(2 * vertexNum + 50);
	edgeData.resize(vertexNum);
}
AdjacentListGraph::~AdjacentListGraph() {
}

bool AdjacentListGraph::queryEdge(Edge &e) {
	auto it = listFind(edgeData[e.ownerID], e.targetID);
	if (it == edgeData[e.ownerID].end()) {
		e.weight = INF;
		return false;
	}
	else {
		e.weight = it->weight;
		return true;
	}
}
void AdjacentListGraph::insertEdge(Edge const &e) {
	auto it = listFind(edgeData[e.ownerID], e.targetID);
	if (it == edgeData[e.ownerID].end()) {
		edgeData[e.ownerID].push_back(e);
		++edgeNum;
		negativeWeightCount += e.weight < 0 ? 1 : 0;
	}
	else {
		//ȷ�ϸ��¶Է�Ȩ��������Ӱ��
		if (it->weight < 0) {
			//����
			negativeWeightCount -= e.weight < 0 ? 0 : 1;
		}
		else {
			//����
			negativeWeightCount += e.weight < 0 ? 1 : 0;
		}
		//�Ѵ���: ���������� ֱ�Ӹ��¾���
		it->weight = e.weight;
	}
}
void AdjacentListGraph::deleteEdge(VertexKey ownerID, VertexKey targetID) {
	auto it = listFind(edgeData[ownerID], targetID);
	if (it == edgeData[ownerID].end()) {
		//do nothig
	}
	else {
		negativeWeightCount -= it->weight < 0 ? 1 : 0;
		edgeData[ownerID].erase(it);
		--edgeNum;
	}
}

AdjacentListGraph::EdgesType const &AdjacentListGraph::getEdgeList(size_t originNum) {
	return edgeData[originNum];
}
int AdjacentListGraph::dfs(VertexKey presentId) {
	customVisit(presentId);

	if (presentId == destId) {
		return 1;
	}
	else {
		visited[presentId] = true;
		int count = 0;
		// �ش��ڹ�ϵ
		for (auto &element : edgeData[presentId]) {
			if (!visited[element.targetID]) {
				count += dfs(element.targetID);
			}
		}
		visited[presentId] = false;
		return count;
	}
}

bool AdjacentListGraph::topologySort(JCE::ArrayList<VertexKey> &topOrderBuffer) {
	/* ��Graph������������,  topOrderBuffer[]˳��洢�����Ķ����±� */
	topOrderBuffer.resize(vertexNum);
	JCE::ArrayList<VertexKey> indegree(vertexNum);
	//queue<VertexKey> q;
	//��֤��ͬ������������������С��
	std::priority_queue<VertexKey, JCE::ArrayList<VertexKey>, JCE::greater<VertexKey>> q;

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
	int cnt = -1;
	while (!q.empty()) {
		/* ����һ�����Ϊ0�Ķ��� ���������� */
		VertexKey v = q.top();//q.front();
		q.pop();
		topOrderBuffer[++cnt] = v;
		/* ��v��ÿ���ڽӵ�itW->targetID(W->AdjV)*/
		for (auto itW = edgeData[v].begin(); itW != edgeData[v].end(); ++itW) {
			/* ��ɾ��vʹ��itW->targetID���Ϊ0 */
			if (--indegree[itW->targetID] == 0) {
				q.push(itW->targetID);/* ��ö������� */
			}
		}
	}

	return cnt == edgeData.size();
}


