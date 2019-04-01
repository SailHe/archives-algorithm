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
		//重新计算边数
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
		//每个可到达结点理论上入队且入队一次
		++cnt;
		if (origin == destId) {
			//若用户设置了目的点算法在计算出到目的点的路径时便结束 没设置时应保证其为无效值-1
			break;
		}
		//visited[origin] = true;//在运算结束前dist==-1表示未访问, 其替换了visited
		auto &edges = getEdgeList(origin);
		for (auto &element : edges) {
			if (dist[element.targetID] == -1) {//若未被访问过
				dist[element.targetID] = dist[origin] + 1;//距离更新
				predecessor[element.targetID] = origin;//路径记录
				buffer.push(element.targetID);
			}
		}
		//visited[origin] = false;//bfs无法回溯
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
	//最短距离估计dist优先队列 每个顶点入队一次 松弛一次
	static std::priority_queue<IndexEdge> q;
	static VertexKey v = -1;
	dist.assign(vertexNum, INF);
	predecessor.assign(vertexNum, -1);
	dist[origin] = 0;//已知原点到本身的最短距离为0
	//最开始q=所有V(但实际上每次需要的只是最小值 因此只需要加入已知的非无穷最短距离估计估计的顶点就行了)
	q.push({ origin, dist[origin] });//{targetID, weight}

	//核心算法
	while (!q.empty()) {
		v = q.top().targetID; q.pop();
		auto &edges = getEdgeList(v);
		//对图中由v出发的每条边<v, w>的顶点w进行拓展松弛操作
		//FOR_ALL_OBJECT(edges) { // 分离cpp后用这个宏就显示不能找到实现...
		for (auto &element : edges) {
			//松弛操作: 先松弛过的顶点不会被后松弛的优化 若还能只可能是负权边的情况
			if (dist[v] + element.weight < dist[element.targetID]) {
				if (element.weight < 0)
					return false;//错误:有负权边
				dist[element.targetID] = dist[v] + element.weight;//更新w的最短路径距离估计
				predecessor[element.targetID] = v;//更新w的最短路径前驱结点
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
	static JCE::ArrayList<int> countEnQ;//记录各点入队次数，用于判断负权回路
	static JCE::ArrayList<bool> existInQ;//标志数组，判断是否在队列中
	static JCE::ArrayList<int> stepCount;//记录步数
	static JCE::LinkedList<VertexKey> q;//处理队列

	countEnQ.assign(vertexNum, 0);
	existInQ.assign(vertexNum, false);
	stepCount.assign(vertexNum, 0);
	dist.assign(vertexNum, INF);//初始化距离为无穷大
	predecessor.assign(vertexNum, -1);//初始化路径为未知

	countEnQ[origin] = 1;
	existInQ[origin] = true;
	//已知到原点最短距离为0 其余全为无穷大
	dist[origin] = 0;
	q.push_back(origin);

	while (!q.empty()) {
		auto v = q.front();
		q.pop_front();
		existInQ[v] = false;
		auto &edges = getEdgeList(v);
		for (auto &element : edges) {
			//用发生优化的顶点v对其所有的边<v, w>对应的顶点w进行松弛操作
			if (element.weight + dist[v] <= dist[element.targetID]) {
				/* 出现更优的元素w: 距离优化 || 等距离的步数优化 */
				if (element.weight + dist[v] < dist[element.targetID] || stepCount[element.targetID] > stepCount[v] + 1) {

					dist[element.targetID] = element.weight + dist[v];
					predecessor[element.targetID] = v;//更新前驱结点
					stepCount[element.targetID] = stepCount[v] + 1;
					if (++countEnQ[element.targetID] == vertexNum) {
						return false;//负权回路
					}
					/*
					(有可能引起新的优化 <==> 最短路径估计值有所调整)的元素w
					将不在队列中的w入队
					*/
					if (!existInQ[element.targetID]) {
						//队列非空且优于队首(SLF)
						if (!q.empty() && dist[element.targetID] < dist[q.front()])
							q.push_front(element.targetID);//放在队首
						else
							q.push_back(element.targetID);//否则放在队尾
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
			//用权重初始化dist 没有边的初始化为INF
			dists[i][element.targetID] = element.weight;
			//初始化前驱 没有边的顶点前驱为-1
			predecessors[i][element.targetID] = i;
		}
		dists[i][i] = 0;//到自己的距离估计为0
		predecessors[i][i] = -1;//自己没有前驱
	}

	FOR(k, 0, vertexNum) {
		FOR(i, 0, vertexNum) {
			FOR(j, 0, vertexNum) {
				if (dists[i][k] + dists[k][j] < dists[i][j]) {
					if (i == j && dists[i][j] < 0) {
						//存在负值圈: 不能正确解决
						return false;
					}
					else {
						/*
						有负权时: -1 + MAX_INT32 < MAX_INT32
						若在最外层判断加上dists[i][j] < INF && 会导致最初距离估计为MAX_INT32的边无法松弛
						在这里加上加数是否存在无穷
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

	return true;//算法执行完毕
}



// ===============================

inline bool AdjacentMatrixGraph::existEdge(VertexKey v, VertexKey w) {
	return edgeData[v][w] < INF;//不存在的边被初始化为INF
}
void AdjacentMatrixGraph::resizeEdge(size_t newSize) {
	edgeData.resize(newSize);
	FOR(i, 0, newSize) {
		edgeData[i].resize(newSize, INF);//重分配的内存被置为INF
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
void AdjacentMatrixGraph::deleteEdge(VertexKey ownerID, VertexKey targetID) {
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
AdjacentMatrixGraph::EdgesType const &AdjacentMatrixGraph::getEdgeList(size_t originNum) {
	static EdgesType edgesBuffer;//边读取缓冲
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
			if (edgeData[presentId][nextId] < INF //存在关系
				&& !visited[nextId]) {
				//visited[nextId] = true;//普通回溯, 对后继结点进行访控
				count += dfs(nextId);
				//visited[nextId] = false;
			}
		}
		visited[presentId] = false;//dfs的回溯
		return count;
	}
}
bool AdjacentMatrixGraph::topologySort(JCE::ArrayList<VertexKey> &topOrderBuffer) {
	_ASSERT_EXPR(false, "not impl");
	/* 对Graph进行拓扑排序,  topOrderBuffer[]顺序存储排序后的顶点下标 */
	topOrderBuffer.resize(vertexNum);
	JCE::ArrayList<VertexKey> indegree(vertexNum);
	//queue<VertexKey> q;
	//保证在同等排名下优先输出序号小的
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
		//确认更新对否负权边数量的影响
		if (it->weight < 0) {
			//减少
			negativeWeightCount -= e.weight < 0 ? 0 : 1;
		}
		else {
			//增加
			negativeWeightCount += e.weight < 0 ? 1 : 0;
		}
		//已存在: 边数不增加 直接更新就行
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
		// 必存在关系
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
	/* 对Graph进行拓扑排序,  topOrderBuffer[]顺序存储排序后的顶点下标 */
	topOrderBuffer.resize(vertexNum);
	JCE::ArrayList<VertexKey> indegree(vertexNum);
	//queue<VertexKey> q;
	//保证在同等排名下优先输出序号小的
	std::priority_queue<VertexKey, JCE::ArrayList<VertexKey>, JCE::greater<VertexKey>> q;

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
	int cnt = -1;
	while (!q.empty()) {
		/* 弹出一个入度为0的顶点 加入结果序列 */
		VertexKey v = q.top();//q.front();
		q.pop();
		topOrderBuffer[++cnt] = v;
		/* 对v的每个邻接点itW->targetID(W->AdjV)*/
		for (auto itW = edgeData[v].begin(); itW != edgeData[v].end(); ++itW) {
			/* 若删除v使得itW->targetID入度为0 */
			if (--indegree[itW->targetID] == 0) {
				q.push(itW->targetID);/* 则该顶点入列 */
			}
		}
	}

	return cnt == edgeData.size();
}


