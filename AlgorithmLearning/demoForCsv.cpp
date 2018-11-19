#include <fstream>
#include <string>
#include <iostream>
#include <streambuf>

#include "ExtendSpace.h"
#include "Graph\Graph.h"
using namespace std;

// ���մ�˼· �����׾�����������һ����Ҫ��excel �ļ�
int mainForCsv() {
	// �����ļ������
	ofstream oFile;

	// ��Ҫ������ļ�
	oFile.open("scoresheet.csv", ios::out | ios::trunc);
	oFile << "����" << "," << "����" << "," << "�༶" << "," << "������" << endl;
	oFile << "����" << "," << "22" << "," << "1" << "," << "JIM" << endl;
	oFile << "����" << "," << "23" << "," << "3" << "," << "TOM" << endl;
	oFile.close();


	// ��Ҫ������ļ� 
	ifstream iFile("scoresheet.csv");
	string readStr((std::istreambuf_iterator<char>(iFile)), std::istreambuf_iterator<char>());
	cout << readStr.c_str();

	return 0;
}

// ���������ݽṹ
struct PointGeographic {
	//����
	double lng = -1;
	//γ��
	double lat = -1;
	//�ȶ�
	double count = 0;
	string name = "unknown";
};

//�������stringת��Ϊ�ṹ�����
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

//�����Ե�0����ΪԴ�����·��
int calcRsult_11_17(vector<PointGeographic> const &originList) {
	int size = (int)originList.size();
	Graph *g = new AdjacentListGraph(size);
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
	vector<int> dist, prePath, buffer;
	g->shortestPath(0, dist, prePath);

	// ��������֤
	/*for (int i = 0; i < size; ++i) {
		if (prePath[i] == 0 || find(buffer.begin(), buffer.end(), i) != buffer.end()) {
			buffer.push_back(i);
			cout << originList[i].name << " ǰ��:" << prePath[i] << endl;
		}
	}*/
	delete g;
	g = nullptr;
	return StandardExtend::sumValueStatistics(dist.begin(), dist.end(), 0);
}

// ��5������Ϊһ������ ;�����㵽�������е�[��̾�������] �ļ���
// O[C(V, 5) * (E + VlogV)] Լ: 75287520*[4950 + 96*log(96)]
int mainForSolve_11_17() {
	//string inputFilePath = "F:/Temper/inputSheet_test.csv";
	string inputFilePath = "F:/Temper/inputSheet.csv";
	ifstream iFile(inputFilePath);
	string readStr((std::istreambuf_iterator<char>(iFile)), std::istreambuf_iterator<char>());
	//cout << readStr.c_str();
	vector<PointGeographic> originPointList;
	toPointGeographicList(originPointList, readStr);
	int size = (int)originPointList.size(), setSize = 5;
	int resultMinSetDis = MAX_INT32;
	string resultSetName = "";
	int resultI1 = 15, resultI2 = 48, resultI3 = 97;
	for (int i1 = 0; i1 < size; ++i1) {
		vector<double> lngList;
		lngList.reserve(setSize);
		lngList.push_back(originPointList[i1].lng);
		vector<double> latList;
		latList.reserve(setSize);
		latList.push_back(originPointList[i1].lat);
		double lngSum = originPointList[i1].lng;
		double latSum = originPointList[i1].lat;
		for (int i2 = i1+1; i2 < size; ++i2) {
			lngList.push_back(originPointList[i2].lng);
			latList.push_back(originPointList[i2].lat);
			lngSum += originPointList[i2].lng;
			latSum += originPointList[i2].lat;
			for (int i3 = resultI1; i3 < resultI1+1; ++i3) {
				lngList.push_back(originPointList[i3].lng);
				latList.push_back(originPointList[i3].lat);
				lngSum += originPointList[i3].lng;
				latSum += originPointList[i3].lat;
				clock_t startTime = clock();
				for (int i4 = resultI2; i4 < resultI2+1; ++i4) {
					lngList.push_back(originPointList[i4].lng);
					latList.push_back(originPointList[i4].lat);
					lngSum += originPointList[i4].lng;
					latSum += originPointList[i4].lat;
					for (int i5 = resultI3; i5 < resultI3+1; ++i5) {
						lngList.push_back(originPointList[i5].lng);
						latList.push_back(originPointList[i5].lat);
						lngSum += originPointList[i5].lng;
						latSum += originPointList[i5].lat;

						//�������ԾͲ���ƽ��ֵ�� ������ƽ��ֵ ���ڹ���setSize��������ĵ�, �˵㽫����������setSize����
						// double lngAvl = StandardExtend::avlValueStatistics(lngList.begin(), lngList.end(), 0.0);
						// double lngAvl_ = lngSum / setSize;
						// StandardExtend::testAndOut("", Utility::Double(lngAvl_), Utility::Double(lngAvl));
						// double latAvl = StandardExtend::avlValueStatistics(latList.begin(), latList.end(), 0.0);
						// double latAvl_ = latSum / setSize;
						// StandardExtend::testAndOut("", Utility::Double(latAvl_), Utility::Double(latAvl));

						double lngAvl = lngSum / setSize;
						double latAvl = latSum / setSize;

						string name = to_string(i1)  + " " + to_string(i2) + " " + to_string(i3) + " " + to_string(i4) + " " + to_string(i5);
						vector<PointGeographic> resultList;
						resultList.reserve(size - setSize + 1);
						PointGeographic temp;
						temp.lng = lngAvl;
						temp.lat = latAvl;
						temp.name = name;
						resultList.push_back(temp);
						for (JCE::SizeType i = 0; i < originPointList.size(); ++i) {
							if (i == i1 || i == i2 || i == i3 || i == i4 || i == i5) {
								// do nothing
							}
							else {
								resultList.push_back(originPointList[i]);
							}
						}
						int resultDisBuffer = calcRsult_11_17(resultList);
						if (resultDisBuffer < resultMinSetDis) {
							resultMinSetDis = resultDisBuffer;
							resultSetName = name;
						}
						lngList.pop_back();
						latList.pop_back();
						lngSum -= originPointList[i5].lng;
						latSum -= originPointList[i5].lat;
					}
					lngList.pop_back();
					latList.pop_back();
					lngSum -= originPointList[i4].lng;
					latSum -= originPointList[i4].lat;
				}
				lngList.pop_back();
				latList.pop_back();
				lngSum -= originPointList[i3].lng;
				latSum -= originPointList[i3].lat;
				//cout << "i3=: " << i3 << "��ʱ: " << StandardExtend::calcDiffClock(startTime) << endl;
			}
			lngList.pop_back();
			latList.pop_back();
			lngSum -= originPointList[i2].lng;
			latSum -= originPointList[i2].lat;
		}
		lngList.pop_back();
		latList.pop_back();
		lngSum -= originPointList[i1].lng;
		latSum -= originPointList[i1].lat;
		cout << "i1=" << i1 << endl;
	}
	cout << "���������: " << resultSetName << ";  ������ϵ���̾����: " << resultMinSetDis << endl;
	return 0;
}

int main() {
	mainForSolve_11_17();
	getchar();
	getchar();
	return 0;
}