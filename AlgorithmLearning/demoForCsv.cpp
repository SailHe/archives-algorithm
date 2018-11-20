#include "stdafx.h"
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "ExtendSpace.h"
#include "Graph\TreeObject.h"
//#include "Graph\Graph.h"
#include "Graph\GraphTemplate.h"
#include "BigInteger.h"
#include "MathLibrary.h"
//using namespace StandardExtend;
using namespace std;


// 地理点数据结构
class PointGeographic {
public:
	//经度
	double lng = -1;
	//纬度
	double lat = -1;
	//热度
	double count = 0;
	string name = "unknown";

	PointGeographic() {}
	//INF构造
	PointGeographic(int count) {
		this->count = count;
	}
	//转换为int
	int operator()(PointGeographic const &rhs)const {
		return rhs.count;
	}
	//与INF比较 基于int构造器
	bool operator<(PointGeographic const &rhs)const {
		return this->count < rhs.count;
	}
	//与INF比较
	/*bool operator<(int const &rhs)const {
		return this->count == rhs;
	}*/
	//INF的赋值 由构造器实现
	/*PointGeographic operator=(int const &rhs)const {
		PointGeographic temp = *this;
		temp.count = rhs;
		return temp;
	}
	bool operator==(PointGeographic const &rhs)const {
		return this->count == rhs.count;
	}*/
};

//用PointGeographic之间的距离dis作为权值, 然后使用一个列表类处理集合
class DisSet {
	using Double = Utility::Double;
	bool isvalid = true;

public:

	JCE::LinkedList<double> distaceList;
	DisSet() {}
	//INF构造
	DisSet(int dis) {
		distaceList.assign(1, dis);
	}
	int toInt() const {
		double sum = MAX_INT32 / 2;
		if (isvalid) {
			sum = StandardExtend::sumValueStatistics(distaceList.begin(), distaceList.end(), 0.0);
		}
		return (int)(sum);
	}
	//转换为int
	int operator()(DisSet const &rhs)const {
		return this->toInt();
	}
	bool operator<(DisSet const &rhs)const {
		return this->toInt() < rhs.toInt();
	}
	//用于最短路
	bool operator==(DisSet const &rhs)const {
		return this->toInt() == rhs.toInt();
	}
	//用于最短路
	bool operator<=(DisSet const &rhs)const {
		return this->toInt() <= rhs.toInt();
	}
	//与INF比较
	/*bool operator<(int const &rhs)const {
		return (int)(*this) < (int)rhs;
	}*/
	//用于最短路
	DisSet operator+(DisSet const &rhs)const {
		DisSet sum = *this;
		sum.distaceList.insert(sum.distaceList.end(), rhs.distaceList.begin(), rhs.distaceList.end());
		return sum;
	}
	//调用之后集合内的点将被定义为INF
	void invalid() {
		isvalid = false;
	}
	void valid() {
		isvalid = true;
	}
};

using WeightT = double;

using Graph = GraphTemplate<WeightT>;
using AdjacentListGraph = AdjacentListGraphTemplate<WeightT>;
using AdjacentMatrixGraph = AdjacentMatrixGraphTemplate<WeightT>;


// 构造出完整的带有size+1个顶点任意两点之间边的图: 最后一个顶点是合成顶点
// @TODO: 性能瓶颈: 如果能保证顶点的唯一性的话 insert时都不用检查存在性
// 也可以在getEdgesList的时候建立顺便map然后可以动态删除无效的边(如果有重复 只保留最后一条)
// 还可以在Graph中维护一个查找结构用于检查边是否存在
// PS: 邻接矩阵insert效率本就为O(1)
Graph *generateGraph(JCE::ArrayList<PointGeographic> const &originList) {
	int size = (int)originList.size();
	Graph *g = new AdjacentListGraph(size + 1);
	//Graph *g = new AdjacentMatrixGraph(size + 1);
	//任意两点(非自己)之间都有一条边(无向边)
	for (int r = 0; r < size; ++r) {
		for (int c = r + 1; c < size; ++c) {
			//权值为两点之间的距离
			Utility::PointDouble lhs = Utility::PointDouble(originList[r].lng, originList[r].lat);
			Utility::PointDouble rhs = Utility::PointDouble(originList[c].lng, originList[c].lat);
			// 此处使用的是距离的平方的1W倍
			double dis2 = Utility::PointDouble::dDistance2(lhs, rhs);
			int weight = (int)(dis2 * 1000000);
			g->insertEdge(Graph::Edge(r, c, weight));
			g->insertEdge(Graph::Edge(c, r, weight));
		}
	}
	// 当返回值为*&&时, 这样子被自动delete了?
	//return std::move(g);
	return g;
}

//将读入的string转换为结构体对象
void toPointGeographicList(JCE::ArrayList<PointGeographic> &resultList, string const &originStr) {
	//@see agoj10_31
	JCE::SizeType lhsPosition = 0, rhsPosition = 0;
	int count = -1;
	char splitC = ',';
	PointGeographic pBuffer;
	do {
		lhsPosition = rhsPosition;
		rhsPosition = originStr.find_first_of(splitC, lhsPosition);

		string buffer = originStr.substr(lhsPosition, rhsPosition - lhsPosition);
		if (++count < 4) {
			//printf((count == 0 ? buffer.c_str() : (string(" ") + buffer.c_str())).c_str());
			if (count == 2) {
				splitC = '\n';
			}
			else if (count == 3) {
				splitC = ',';
				puts("");
			}
			else {
				//do nothing
			}
		}
		else {
			switch (count % 4)
			{
			case 0: {
				pBuffer.name = buffer;
				break;
			}
			case 1: {
				pBuffer.count = atof(buffer.c_str());
				break;
			}
			case 2: {
				splitC = '\n';
				pBuffer.lng = atof(buffer.c_str());
				break;
			}
			case 3: {
				splitC = ',';
				pBuffer.lat = atof(buffer.c_str());
				resultList.push_back(PointGeographic(pBuffer));
				//cout << pBuffer.name <<" "<< pBuffer.count << " " << pBuffer.lng << " " << pBuffer.lat << endl;
				break;
			}
			default:
				break;
			}
		}

	} while (rhsPosition++ != string::npos);
	//buffer.substr();
}

//计算出p[index]离集合s中所有点中最近的那一个点的距离
double calcMinDis2(int index, JCE::ArrayList<PointGeographic> const &originList, JCE::ArrayList<int> const &pointSetIndexList) {
	double minDis = MAX_INT64;
	FOR_ALL_OBJECT(pointSetIndexList) {
		Utility::PointDouble lhs = Utility::PointDouble(originList[element].lng, originList[element].lat);
		Utility::PointDouble rhs = Utility::PointDouble(originList[index].lng, originList[index].lat);
		minDis = min(minDis, Utility::PointDouble::dDistance2(lhs, rhs));
	}
	return minDis;
}

// 将以pointSetIndexList[i]作为下标的点的集合视为一个整体参与计算: 重新计算一遍与此顶点有关的权值(边)
// 每次只需要重新计算(删除+添加 或者更新)与那个顶点有关的边(O(E))
void updateGraph(Graph *g, JCE::ArrayList<PointGeographic> &originList, JCE::ArrayList<int> &pointSetIndexList) {
	//不能将其合成一个点, 实际上泛型的应该是边的权值 每次比较时从集合中选出一个互相离得最近的点的距离作为实际权值
	int size = (int)g->getVertexNum();
	int setPointIndex = size - 1;
	//任意两点(非自己)之间都有一条边(无向边)
	for (int index = 0; index < size; ++index) {
		if (index == setPointIndex || find(pointSetIndexList.begin(), pointSetIndexList.end(), index) != pointSetIndexList.end()) {
			continue;
		}
		// 权值为两点之间的距离
		// 此处使用的是距离的平方的1W倍
		double dis2 = calcMinDis2(index, originList, pointSetIndexList);
		int weight = (int)(dis2 * 100000);
		g->insertEdge(Graph::Edge(setPointIndex, index, weight));
		g->insertEdge(Graph::Edge(index, setPointIndex, weight));
	}

	// 简单人眼验证
	/*for (int i = 0; i < size; ++i) {
		if (prePath[i] == 0 || find(buffer.begin(), buffer.end(), i) != buffer.end()) {
			buffer.push_back(i);
			cout << originList[i].name << " 前驱:" << prePath[i] << endl;
		}
	}*/
}

// 把5个点视为一个集合 ;求满足到其余所有点[最短距离和最短] 的集合
// O[C(V, 5) * (E + VlogV)] 约: 75287520*[4950 + 96*log(96)]
int mainForSolve_11_17() {
	// 0 1 3
	//string inputFilePath = "F:/Temper/inputSheet_test3_1.csv";
	// 1 2 4
	//string inputFilePath = "F:/Temper/inputSheet_test3.csv";
	// 0 1 2 3 4 
	//string inputFilePath = "F:/Temper/inputSheet_test.csv";
	string inputFilePath = "F:/Temper/inputSheet.csv";
	ifstream iFile(inputFilePath);
	string readStr((std::istreambuf_iterator<char>(iFile)), std::istreambuf_iterator<char>());
	//cout << readStr.c_str();
	JCE::ArrayList<PointGeographic> originPointList;
	toPointGeographicList(originPointList, readStr);
	Graph *g = nullptr;
	StandardExtend::testAndDiffClock([&]() {
		//           DEBUG(DisSet)											RELEASE
		// 邻接矩阵: 3.819s; 3.102; 3.202; 0.003(double) 0.677			0.048; 0.129; 0.093
		// 邻接表 :  3.205;	3.317; 3.065;								0.022; 0.023; 0.026
		g = generateGraph(originPointList);
	}, "带边的图的生成");

	int size = (int)originPointList.size(), setSize = 3;
	int resultMinSetDis = MAX_INT32;
	string resultSetName = "";
	clock_t startTime = clock();
	for (int i1 = 0; i1 < size; ++i1) {
		// 将i1置为无效 (原本是全有效的, 无效的顶点在计算时跳过)
		g->invalidVertex(i1);
		JCE::ArrayList<double> lngList;
		lngList.reserve(setSize);
		lngList.push_back(originPointList[i1].lng);
		JCE::ArrayList<double> latList;
		latList.reserve(setSize);
		latList.push_back(originPointList[i1].lat);
		double lngSum = originPointList[i1].lng;
		double latSum = originPointList[i1].lat;
		for (int i2 = i1 + 1; i2 < size; ++i2) {
			// 将i2置为无效
			g->invalidVertex(i2);
			lngList.push_back(originPointList[i2].lng);
			latList.push_back(originPointList[i2].lat);
			lngSum += originPointList[i2].lng;
			latSum += originPointList[i2].lat;
			for (int i3 = i2 + 1; i3 < size; ++i3) {
				// 将i3置为无效
				g->invalidVertex(i3);
				lngList.push_back(originPointList[i3].lng);
				latList.push_back(originPointList[i3].lat);
				lngSum += originPointList[i3].lng;
				latSum += originPointList[i3].lat;
				for (int i4 = i3+1; i4 < size; ++i4) {
					// 将i4置为无效
					g->invalidVertex(i4);
					lngList.push_back(originPointList[i4].lng);
					latList.push_back(originPointList[i4].lat);
					lngSum += originPointList[i4].lng;
					latSum += originPointList[i4].lat;
					for (int i5 = i4+1; i5 < size; ++i5) {
						// 将i5置为无效
						g->invalidVertex(i5);
						lngList.push_back(originPointList[i5].lng);
						latList.push_back(originPointList[i5].lat);
						lngSum += originPointList[i5].lng;
						latSum += originPointList[i5].lat;


						double lngAvl = lngSum / setSize;
						double latAvl = latSum / setSize;

						string name = to_string(i1) + ": " + originPointList[i1].name
							+ " " + to_string(i2) + ": " + originPointList[i2].name
							+ " " + to_string(i3) + ": " + originPointList[i3].name
							+ " " + to_string(i4) + ": " + originPointList[i4].name
							+ " " + to_string(i5) + ": " + originPointList[i5].name;

						JCE::ArrayList<int> pointSetIndexList;
						pointSetIndexList.reserve(size - setSize + 1);
						pointSetIndexList.push_back(i1);
						pointSetIndexList.push_back(i2);
						pointSetIndexList.push_back(i3);
						pointSetIndexList.push_back(i4);
						pointSetIndexList.push_back(i5);
						//或许可以直接删除有关的边 已达到删除顶点的目的
						updateGraph(g, originPointList, pointSetIndexList);
						// 97 其余顶点实际为100 有效的为97
						//StandardExtend::testAndOut("集合顶点的实际边数: ", g->getEdgeNum(), g->getVertexNum()-4);
						JCE::ArrayList<int> prePath;// , dist, buffer;
						JCE::ArrayList<WeightT> minWeightList;
						//StandardExtend::testAndDiffClock([&]() {
							//           DEBUG						RELEASE
							// 邻接矩阵:  
							// 邻接表 :  
						g->shortestPath(g->getVertexNum() - 1, minWeightList, prePath);
						//}, "集合到其余所有点的最短路径");

						int resultDisBuffer = 0, index = -1;//StandardExtend::sumValueStatistics(dist.begin(), dist.end(), 0);
						FOR_ALL_OBJECT(minWeightList) {
							if (find(pointSetIndexList.begin(), pointSetIndexList.end(), ++index) != pointSetIndexList.end()) {
								continue;
							}
							//dist.push_back(element);
							resultDisBuffer += element;
						}
						if (resultDisBuffer < 0) {
							puts("溢出!");
							getchar();
						}
						if (resultDisBuffer < resultMinSetDis) {
							resultMinSetDis = resultDisBuffer;
							resultSetName = name;
						}

						// 将图中i5这个点置为有效
						g->validVertex(i5);
						lngList.pop_back();
						latList.pop_back();
						lngSum -= originPointList[i5].lng;
						latSum -= originPointList[i5].lat;
					}
					// 将图中i4这个点置为有效
					g->validVertex(i4);
					lngList.pop_back();
					latList.pop_back();
					lngSum -= originPointList[i4].lng;
					latSum -= originPointList[i4].lat;
				}
				// 将图中i3这个点置为有效
				g->validVertex(i3);
				lngList.pop_back();
				latList.pop_back();
				lngSum -= originPointList[i3].lng;
				latSum -= originPointList[i3].lat;
				cout << "i3=" << i3 << "; 耗时: " << StandardExtend::calcDiffClock(startTime)
					<< "; 当前结果集" << resultSetName << "; 当前最短路径和=" << resultMinSetDis << endl;
			}
			g->validVertex(i2);
			lngList.pop_back();
			latList.pop_back();
			lngSum -= originPointList[i2].lng;
			latSum -= originPointList[i2].lat;
		}
		g->validVertex(i1);
		lngList.pop_back();
		latList.pop_back();
		lngSum -= originPointList[i1].lng;
		latSum -= originPointList[i1].lat;
		cout << "i1=" << i1 << "; 耗时: " << StandardExtend::calcDiffClock(startTime) << "; 当前结果集" << resultSetName << "; 当前最短路径和=" << resultMinSetDis << endl;
		//cout << "i1=" << i1 << endl;
	}
	cout << "结果集合名: " << resultSetName << ";  结果集合的最短距离和的1w倍的平方: " << resultMinSetDis << endl;
	return 0;
}

int main() {
	mainForSolve_11_17();
	puts("任意按键继续");
	getchar();
	getchar();
	return 0;
}
