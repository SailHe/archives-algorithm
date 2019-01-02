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

#define MAX_IN32 2147483647
#define MIN_IN32 (-MAX_IN32-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>
#define FOR(it, lowerBound, terminator) for (auto it = lowerBound; it != terminator; ++it)

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;
/*
基站安装
@see http://139.196.145.92/contest_show.php?cid=612#problem/A
Sample Input
3 2
1 2
-3 1
2 1
1 2
0 2
1 -2
0 2
3 2
1 2
1 2
1 2
1 3
1 2
0 0
Sample Output
Case 1: 2
Case 2: 1
Case 3: -1
*/
//重载了精度比较符的双精度浮点
class DoubleE {
public:
	DoubleE() {}
	DoubleE(int rhs) {
		value = rhs;
	}
	DoubleE(double rhs) {
		value = rhs;
	}
	inline bool operator==(DoubleE const &rhs) const {
		return fabs(this->value - rhs.value) < EPS;
	}
	inline bool operator!=(DoubleE const &rhs) {
		return !(*this == rhs);
	}


	//cout<<
	friend std::ostream &operator<<(std::ostream &os, const DoubleE &rhs) {
		os << rhs.value;
		return os;
	}

private:
	double value;
};

class PointInt {
public:
	using T = int;
	T x, y;
	PointInt() = default;
	PointInt(T x, T y) {
		x = y, y = y;
	}
	PointInt(std::istream &in) {
		//scanf("%lf%lf", &(this->x), &(this->y));
		in >> this->x >> this->y;
	}
	PointInt O() {
		PointInt ZERO = { 0, 0 };
		return ZERO;
	}

	//能从起始点'跳跃至'终点返回true
	static bool canJumpTo(PointInt const &beganPoint, PointInt const &endPoint, T jumpPower, T raduis) {
		//起点 - 终点 - 跳跃能力 - 活动半径
		return dDistance(beganPoint, endPoint) <= raduis + jumpPower;
	}
	//返回两点间距离
	static T dDistance(PointInt const &p1, PointInt const &p2) {
		return (T)sqrt(dDistance2(p1, p2));
	}
	//返回两点间距离的平方
	static T dDistance2(PointInt const &p1, PointInt const &p2) {
		T dx = p2.x - p1.x;
		T dy = p2.y - p1.y;
		return (dx*dx + dy * dy);
	}
};
//PS c++ 14 才允许将auto用于lambda中
int mainForSolveA_11_7() {
	//n代表小岛数量，d代表基站的覆盖距离
	int n, d, resultCnt, caseCnt = 0;
	while ((cin >> n >> d) && n != 0 && d != 0) {
		bool hasResult = true;
		ArrayList<PointInt> islandList(n);
		//first和second分别表示以岛画圆形成的覆盖范围的左侧和右侧在海岸线上的x坐标
		ArrayList<pair<double,double>> segmentList(n);
		PointInt temp;
		for (int i = 0; i < n; ++i) {
			cin >> temp.x >> temp.y;
			islandList[i] = temp;
			if (hasResult && islandList[i].y > d) {
				hasResult = false;
			}
		}
		if (hasResult) {
			for (int i = 0; i < n; ++i) {
				//d作为斜边y作为直角边计算另一直角边
				double distance = sqrt(1.0*d*d - 1.0*islandList[i].y*islandList[i].y);
				segmentList[i].first = islandList[i].x - distance;
				segmentList[i].second = islandList[i].x + distance;
			}
			sort(segmentList.begin(), segmentList.end(), [&](pair<double, double> const &lhs, pair<double, double> const &rhs) {
				return DoubleE(lhs.first) == DoubleE(rhs.first) ? lhs.second < rhs.second
					: lhs.first < rhs.first;
			});
			//贪心
			resultCnt = 1;
			double currentRight = segmentList[0].second;
			for (int i = 1; i < n; ++i) {
				//相离: 增加基站 且变更右侧
				if (currentRight < segmentList[i].first) {
					++resultCnt;
					currentRight = segmentList[i].second;
				}
				else {
					//当前线段包含在内或是重叠时: 基站数不变 右侧值变为当前线段右侧
					//if (currentRight >= segmentList[i].second) {
					if (currentRight > segmentList[i].second) {
						currentRight = segmentList[i].second;
					}
					else {
						//do nothing
					}
				}
			}
		}
		else {
			resultCnt = -1;
		}
		cout << "Case " << ++caseCnt << ": " << resultCnt << endl;
	}
	return 0;
}

int main() {
	mainForSolveA_11_7();
	return 0;
}


