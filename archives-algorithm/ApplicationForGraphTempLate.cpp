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

using Graph = GraphTemplate<DisSet>;
using AdjacentListGraph = AdjacentListGraphTemplate<DisSet>;


// 构造出完整的带有100个顶点任意两点之间边的图
Graph *&&generateGraph(vector<PointGeographic> const &originList) {
	int size = (int)originList.size();
	Graph *g = new AdjacentListGraph(size);
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
	return std::move(g);
}

//将读入的string转换为结构体对象
void toPointGeographicList(vector<PointGeographic> &resultList, string const &originStr) {
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

//计算以第0个点为源的最短路径
int calcRsult_11_17(vector<PointGeographic> const &originList) {
	int size = (int)originList.size();
	Graph *g = new AdjacentListGraph(size);
	//任意两点(非自己)之间都有一条边(无向边)
	for (int r = 0; r < size; ++r) {
		for (int c = r + 1; c < size; ++c) {
			//权值为两点之间的距离
			Utility::PointDouble lhs = Utility::PointDouble(originList[r].lng, originList[r].lat);
			Utility::PointDouble rhs = Utility::PointDouble(originList[c].lng, originList[c].lat);
			// 此处使用的是距离的平方的1W倍
			double dis2 = Utility::PointDouble::dDistance2(lhs, rhs);
			int weight = (int)(dis2 * 100000);
			g->insertEdge(Graph::Edge(r, c, weight));
			g->insertEdge(Graph::Edge(c, r, weight));
		}
	}
	vector<int> dist, prePath, buffer;
	vector<DisSet> weightList;
	g->shortestPath(0, weightList, prePath);
	dist.reserve(weightList.size());
	FOR_ALL_OBJECT(weightList) {
		dist.push_back(element.toInt());
	}

	// 简单人眼验证
	/*for (int i = 0; i < size; ++i) {
		if (prePath[i] == 0 || find(buffer.begin(), buffer.end(), i) != buffer.end()) {
			buffer.push_back(i);
			cout << originList[i].name << " 前驱:" << prePath[i] << endl;
		}
	}*/
	delete g;
	g = nullptr;
	return StandardExtend::sumValueStatistics(dist.begin(), dist.end(), 0);
}

// 把5个点视为一个集合 ;求满足到其余所有点[最短距离和最短] 的集合
// O[C(V, 5) * (E + VlogV)] 约: 75287520*[4950 + 96*log(96)]
int mainForSolve_11_17() {
	//string inputFilePath = "F:/Temper/inputSheet_test3.csv";
	string inputFilePath = "F:/Temper/inputSheet.csv";
	ifstream iFile(inputFilePath);
	string readStr((std::istreambuf_iterator<char>(iFile)), std::istreambuf_iterator<char>());
	//cout << readStr.c_str();
	vector<PointGeographic> originPointList;
	toPointGeographicList(originPointList, readStr);

	auto g = generateGraph(originPointList);

	int size = (int)originPointList.size(), setSize = 3;
	int resultMinSetDis = MAX_INT32;
	string resultSetName = "";
	for (int i1 = 0; i1 < size; ++i1) {
		// 将i1-1(如果有的话) 置为有效
		if (i1 > 0) {
			for (auto ele : g->getEdgeList(i1 - 1)) {
				ele.weight.valid();
			}
		}
		vector<double> lngList;
		lngList.reserve(setSize);
		lngList.push_back(originPointList[i1].lng);
		vector<double> latList;
		latList.reserve(setSize);
		latList.push_back(originPointList[i1].lat);
		double lngSum = originPointList[i1].lng;
		double latSum = originPointList[i1].lat;
		clock_t startTime = clock();
		for (int i2 = i1 + 1; i2 < size; ++i2) {
			// 将i2-1(如果有的话) 置为有效
			if (i2 > i1 + 1) {
				for (auto ele : g->getEdgeList(i2 - 1)) {
					ele.weight.valid();
				}
			}
			lngList.push_back(originPointList[i2].lng);
			latList.push_back(originPointList[i2].lat);
			lngSum += originPointList[i2].lng;
			latSum += originPointList[i2].lat;
			for (int i3 = i2 + 1; i3 < size; ++i3) {
				// 将i2 i3 添加进i1所在的DisSet中

				// 将i3-1(如果有的话) 置为有效
				if (i3 > i2 + 1) {
					for (auto ele : g->getEdgeList(i3 - 1)) {
						ele.weight.valid();
					}
				}
				lngList.push_back(originPointList[i3].lng);
				latList.push_back(originPointList[i3].lat);
				lngSum += originPointList[i3].lng;
				latSum += originPointList[i3].lat;


				double lngAvl = lngSum / setSize;
				double latAvl = latSum / setSize;

				string name = to_string(i1) + " " + to_string(i2) + " " + to_string(i3);
				vector<PointGeographic> resultList;
				resultList.reserve(size - setSize + 1);
				PointGeographic temp;

				temp.lng = lngAvl;
				temp.lat = latAvl;
				temp.name = name;
				resultList.push_back(temp);
				for (JCE::SizeType i = 0; i < originPointList.size(); ++i) {
					if (i == i1 || i == i2 || i == i3) {
						// do nothing
					}
					else {
						resultList.push_back(originPointList[i]);
					}
				}
				int resultDisBuffer = calcRsult_11_17(resultList);
				if (resultDisBuffer < 0) {
					puts("溢出!");
					getchar();
				}
				if (resultDisBuffer < resultMinSetDis) {
					resultMinSetDis = resultDisBuffer;
					resultSetName = name;
				}

				// 将图中i3这个点置为无效 (计算时视为INF)
				for (auto ele : g->getEdgeList(i3)) {
					ele.weight.invalid();
				}
				lngList.pop_back();
				latList.pop_back();
				lngSum -= originPointList[i3].lng;
				latSum -= originPointList[i3].lat;
			}
			for (auto ele : g->getEdgeList(i2)) {
				ele.weight.invalid();
			}
			lngList.pop_back();
			latList.pop_back();
			lngSum -= originPointList[i2].lng;
			latSum -= originPointList[i2].lat;
		}
		for (auto ele : g->getEdgeList(i1)) {
			ele.weight.invalid();
		}
		lngList.pop_back();
		latList.pop_back();
		lngSum -= originPointList[i1].lng;
		latSum -= originPointList[i1].lat;
		cout << "i1=" << i1 << "耗时: " << StandardExtend::calcDiffClock(startTime) << "当前结果集" << resultSetName << "当前resultMinSetDis=" << resultMinSetDis << endl;
		//cout << "i1=" << i1 << endl;
	}
	cout << "结果集合名: " << resultSetName << ";  结果集合的最短距离和: " << resultMinSetDis << endl;
	return 0;
}

int main() {
	mainForSolve_11_17();
	puts("任意按键继续");
	getchar();
	return 0;
}

