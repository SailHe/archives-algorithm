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


// ��������ݽṹ
class PointGeographic {
public:
	//����
	double lng = -1;
	//γ��
	double lat = -1;
	//�ȶ�
	double count = 0;
	string name = "unknown";

	PointGeographic() {}
	//INF����
	PointGeographic(int count) {
		this->count = count;
	}
	//ת��Ϊint
	int operator()(PointGeographic const &rhs)const {
		return rhs.count;
	}
	//��INF�Ƚ� ����int������
	bool operator<(PointGeographic const &rhs)const {
		return this->count < rhs.count;
	}
	//��INF�Ƚ�
	/*bool operator<(int const &rhs)const {
		return this->count == rhs;
	}*/
	//INF�ĸ�ֵ �ɹ�����ʵ��
	/*PointGeographic operator=(int const &rhs)const {
		PointGeographic temp = *this;
		temp.count = rhs;
		return temp;
	}
	bool operator==(PointGeographic const &rhs)const {
		return this->count == rhs.count;
	}*/
};

//��PointGeographic֮��ľ���dis��ΪȨֵ, Ȼ��ʹ��һ���б��ദ����
class DisSet {
	using Double = Utility::Double;
	bool isvalid = true;

public:

	JCE::LinkedList<double> distaceList;
	DisSet() {}
	//INF����
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
	//ת��Ϊint
	int operator()(DisSet const &rhs)const {
		return this->toInt();
	}
	bool operator<(DisSet const &rhs)const {
		return this->toInt() < rhs.toInt();
	}
	//�������·
	bool operator==(DisSet const &rhs)const {
		return this->toInt() == rhs.toInt();
	}
	//�������·
	bool operator<=(DisSet const &rhs)const {
		return this->toInt() <= rhs.toInt();
	}
	//��INF�Ƚ�
	/*bool operator<(int const &rhs)const {
		return (int)(*this) < (int)rhs;
	}*/
	//�������·
	DisSet operator+(DisSet const &rhs)const {
		DisSet sum = *this;
		sum.distaceList.insert(sum.distaceList.end(), rhs.distaceList.begin(), rhs.distaceList.end());
		return sum;
	}
	//����֮�󼯺��ڵĵ㽫������ΪINF
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


// ����������Ĵ���size+1��������������֮��ߵ�ͼ: ���һ�������Ǻϳɶ���
// @TODO: ����ƿ��: ����ܱ�֤�����Ψһ�ԵĻ� insertʱ�����ü�������
// Ҳ������getEdgesList��ʱ����˳��mapȻ����Զ�̬ɾ����Ч�ı�(������ظ� ֻ�������һ��)
// ��������Graph��ά��һ�����ҽṹ���ڼ����Ƿ����
// PS: �ڽӾ���insertЧ�ʱ���ΪO(1)
Graph *generateGraph(JCE::ArrayList<PointGeographic> const &originList) {
	int size = (int)originList.size();
	Graph *g = new AdjacentListGraph(size + 1);
	//Graph *g = new AdjacentMatrixGraph(size + 1);
	//��������(���Լ�)֮�䶼��һ����(�����)
	for (int r = 0; r < size; ++r) {
		for (int c = r + 1; c < size; ++c) {
			//ȨֵΪ����֮��ľ���
			Utility::PointDouble lhs = Utility::PointDouble(originList[r].lng, originList[r].lat);
			Utility::PointDouble rhs = Utility::PointDouble(originList[c].lng, originList[c].lat);
			// �˴�ʹ�õ��Ǿ����ƽ����1W��
			double dis2 = Utility::PointDouble::dDistance2(lhs, rhs);
			int weight = (int)(dis2 * 1000000);
			g->insertEdge(Graph::Edge(r, c, weight));
			g->insertEdge(Graph::Edge(c, r, weight));
		}
	}
	// ������ֵΪ*&&ʱ, �����ӱ��Զ�delete��?
	//return std::move(g);
	return g;
}

//�������stringת��Ϊ�ṹ�����
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

//�����p[index]�뼯��s�����е����������һ����ľ���
double calcMinDis2(int index, JCE::ArrayList<PointGeographic> const &originList, JCE::ArrayList<int> const &pointSetIndexList) {
	double minDis = MAX_INT64;
	FOR_ALL_OBJECT(pointSetIndexList) {
		Utility::PointDouble lhs = Utility::PointDouble(originList[element].lng, originList[element].lat);
		Utility::PointDouble rhs = Utility::PointDouble(originList[index].lng, originList[index].lat);
		minDis = min(minDis, Utility::PointDouble::dDistance2(lhs, rhs));
	}
	return minDis;
}

// ����pointSetIndexList[i]��Ϊ�±�ĵ�ļ�����Ϊһ������������: ���¼���һ����˶����йص�Ȩֵ(��)
// ÿ��ֻ��Ҫ���¼���(ɾ��+��� ���߸���)���Ǹ������йصı�(O(E))
void updateGraph(Graph *g, JCE::ArrayList<PointGeographic> &originList, JCE::ArrayList<int> &pointSetIndexList) {
	//���ܽ���ϳ�һ����, ʵ���Ϸ��͵�Ӧ���Ǳߵ�Ȩֵ ÿ�αȽ�ʱ�Ӽ�����ѡ��һ�������������ĵ�ľ�����Ϊʵ��Ȩֵ
	int size = (int)g->getVertexNum();
	int setPointIndex = size - 1;
	//��������(���Լ�)֮�䶼��һ����(�����)
	for (int index = 0; index < size; ++index) {
		if (index == setPointIndex || find(pointSetIndexList.begin(), pointSetIndexList.end(), index) != pointSetIndexList.end()) {
			continue;
		}
		// ȨֵΪ����֮��ľ���
		// �˴�ʹ�õ��Ǿ����ƽ����1W��
		double dis2 = calcMinDis2(index, originList, pointSetIndexList);
		int weight = (int)(dis2 * 100000);
		g->insertEdge(Graph::Edge(setPointIndex, index, weight));
		g->insertEdge(Graph::Edge(index, setPointIndex, weight));
	}

	// ��������֤
	/*for (int i = 0; i < size; ++i) {
		if (prePath[i] == 0 || find(buffer.begin(), buffer.end(), i) != buffer.end()) {
			buffer.push_back(i);
			cout << originList[i].name << " ǰ��:" << prePath[i] << endl;
		}
	}*/
}

// ��5������Ϊһ������ ;�����㵽�������е�[��̾�������] �ļ���
// O[C(V, 5) * (E + VlogV)] Լ: 75287520*[4950 + 96*log(96)]
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
		// �ڽӾ���: 3.819s; 3.102; 3.202; 0.003(double) 0.677			0.048; 0.129; 0.093
		// �ڽӱ� :  3.205;	3.317; 3.065;								0.022; 0.023; 0.026
		g = generateGraph(originPointList);
	}, "���ߵ�ͼ������");

	int size = (int)originPointList.size(), setSize = 3;
	int resultMinSetDis = MAX_INT32;
	string resultSetName = "";
	clock_t startTime = clock();
	for (int i1 = 0; i1 < size; ++i1) {
		// ��i1��Ϊ��Ч (ԭ����ȫ��Ч��, ��Ч�Ķ����ڼ���ʱ����)
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
			// ��i2��Ϊ��Ч
			g->invalidVertex(i2);
			lngList.push_back(originPointList[i2].lng);
			latList.push_back(originPointList[i2].lat);
			lngSum += originPointList[i2].lng;
			latSum += originPointList[i2].lat;
			for (int i3 = i2 + 1; i3 < size; ++i3) {
				// ��i3��Ϊ��Ч
				g->invalidVertex(i3);
				lngList.push_back(originPointList[i3].lng);
				latList.push_back(originPointList[i3].lat);
				lngSum += originPointList[i3].lng;
				latSum += originPointList[i3].lat;
				for (int i4 = i3+1; i4 < size; ++i4) {
					// ��i4��Ϊ��Ч
					g->invalidVertex(i4);
					lngList.push_back(originPointList[i4].lng);
					latList.push_back(originPointList[i4].lat);
					lngSum += originPointList[i4].lng;
					latSum += originPointList[i4].lat;
					for (int i5 = i4+1; i5 < size; ++i5) {
						// ��i5��Ϊ��Ч
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
						//�������ֱ��ɾ���йصı� �Ѵﵽɾ�������Ŀ��
						updateGraph(g, originPointList, pointSetIndexList);
						// 97 ���ඥ��ʵ��Ϊ100 ��Ч��Ϊ97
						//StandardExtend::testAndOut("���϶����ʵ�ʱ���: ", g->getEdgeNum(), g->getVertexNum()-4);
						JCE::ArrayList<int> prePath;// , dist, buffer;
						JCE::ArrayList<WeightT> minWeightList;
						//StandardExtend::testAndDiffClock([&]() {
							//           DEBUG						RELEASE
							// �ڽӾ���:  
							// �ڽӱ� :  
						g->shortestPath(g->getVertexNum() - 1, minWeightList, prePath);
						//}, "���ϵ��������е�����·��");

						int resultDisBuffer = 0, index = -1;//StandardExtend::sumValueStatistics(dist.begin(), dist.end(), 0);
						FOR_ALL_OBJECT(minWeightList) {
							if (find(pointSetIndexList.begin(), pointSetIndexList.end(), ++index) != pointSetIndexList.end()) {
								continue;
							}
							//dist.push_back(element);
							resultDisBuffer += element;
						}
						if (resultDisBuffer < 0) {
							puts("���!");
							getchar();
						}
						if (resultDisBuffer < resultMinSetDis) {
							resultMinSetDis = resultDisBuffer;
							resultSetName = name;
						}

						// ��ͼ��i5�������Ϊ��Ч
						g->validVertex(i5);
						lngList.pop_back();
						latList.pop_back();
						lngSum -= originPointList[i5].lng;
						latSum -= originPointList[i5].lat;
					}
					// ��ͼ��i4�������Ϊ��Ч
					g->validVertex(i4);
					lngList.pop_back();
					latList.pop_back();
					lngSum -= originPointList[i4].lng;
					latSum -= originPointList[i4].lat;
				}
				// ��ͼ��i3�������Ϊ��Ч
				g->validVertex(i3);
				lngList.pop_back();
				latList.pop_back();
				lngSum -= originPointList[i3].lng;
				latSum -= originPointList[i3].lat;
				cout << "i3=" << i3 << "; ��ʱ: " << StandardExtend::calcDiffClock(startTime)
					<< "; ��ǰ�����" << resultSetName << "; ��ǰ���·����=" << resultMinSetDis << endl;
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
		cout << "i1=" << i1 << "; ��ʱ: " << StandardExtend::calcDiffClock(startTime) << "; ��ǰ�����" << resultSetName << "; ��ǰ���·����=" << resultMinSetDis << endl;
		//cout << "i1=" << i1 << endl;
	}
	cout << "���������: " << resultSetName << ";  ������ϵ���̾���͵�1w����ƽ��: " << resultMinSetDis << endl;
	return 0;
}

int main() {
	mainForSolve_11_17();
	puts("���ⰴ������");
	getchar();
	getchar();
	return 0;
}
