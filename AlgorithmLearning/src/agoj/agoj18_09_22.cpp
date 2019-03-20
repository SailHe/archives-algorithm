#include<stdio.h>
#include<string.h>
#include<string>
#include<cmath>
#include<vector>
#include<iostream>
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

//#define MAX_N 1000+5
#define MAX_INT_NUM 2147483647
#define MIN_INT_NUM (-MAX_INT_NUM-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>
using namespace std;
namespace StandardExtend{
	template<class T>
	//left <= value < right  EG: inRange(minEle, element, maxEle+1); -> element [minEle, maxEle]
	bool inRange(T left, T value, T right){
		return left <= value && value < right;
	}

	template<class Iterator, class Fun>
	void iterate(Iterator left, Iterator right, Fun visit){
		while (left != right){
			visit(left, right);
			++left;
		}
	}

	template<class Iterator>
	//coutFillChar��ʾ����ַ� �൱��printf��%02d �е�'0' coutWidth��ʾ���е�2
	void outPutIterable(Iterator left, Iterator right, char coutFillChar = 0, size_t coutWidth = 4){
		cout << setfill(coutFillChar);
		cout << setw(coutWidth) << *left;
		++left;
		iterate(left, right, [&](Iterator left, Iterator right){
			cout << " " << setw(coutWidth) << *left;
		});
		puts("");
	}

	template<class Iterator>
	//lineWrap �������л��� С��0 ��ʾ�����������
	void outPutIterable(Iterator left, Iterator right, char coutFillChar, size_t coutWidth, int lineWrap){
		cout << setfill(coutFillChar);
		cout << *left;
		++left;
		int c = 1;
		lineWrap = lineWrap < 0 ? right - left : lineWrap;
		iterate(left, right, [&](Iterator left, Iterator right){
			++c;
			if (c % lineWrap == 0){
				//���һ��
				//cout << " " << setw(coutWidth) << *left << endl;
				cout << setw(coutWidth) << *left << endl;
			}
			else if (c % lineWrap == 1){
				//��һ��
				cout << *left;
			}
			else{
				//�м��
				//cout << " " << setw(coutWidth) << *left;
				cout << setw(coutWidth) << *left;
			}
		});
		//���һ��û�л������ﻻ��
		if (c % lineWrap != 0){
			puts("");
		}
	}

	template<class Iterator, class Fun>
	//lineWrapFun �����Ƿ���  bool(* lineWrapFun)(Iterator left, Iterator right)
	void outPutIterable(Iterator left, Iterator right, char coutFillChar, size_t coutWidth, Fun lineWrapFun){
		cout << setfill(coutFillChar);
		int c = 0;
		bool pastWrap = false;
		//lineWrapFun = nullptr == lineWrapFun ? [&](){return right - left; } : lineWrapFun;
		iterate(left, right, [&](Iterator left, Iterator right){
			++c;
			pastWrap = lineWrapFun(left, right);
			if (1 == c){//��һ��
				cout << *left;
			}
			else{
				//�м�&���
				cout << setw(coutWidth) << *left;
			}
			if (pastWrap){//���һ��
				cout << endl;
				c = 0;
			}
		});
		//���һ��û�л������ﻻ��
		if (!pastWrap){
			puts("");
		}
	}


	template<class T>
	void outPut2DArrayList(vector<vector<T>> const &arrayList2D, char coutFillChar = '0', size_t coutWidth = 2){
		size_t rows = arrayList2D.size();
		for (size_t r = 0; r < rows; ++r){
			outPutIterable(arrayList2D[r].begin(), arrayList2D[r].end(), coutFillChar, coutWidth);
		}
	}

	template<class T>
	void outPut2DArray(T array2D[MAX_R][MAX_C], size_t n, char coutFillChar = '0', size_t coutWidth = 2){
		for (size_t r = 0; r < n; ++r){
			outPutIterable(array2D[r], array2D[r] + n, coutFillChar, coutWidth);
		}
	}

	template<class T>
	void outPut2DArrayTrangle(T array2D[MAX_R][MAX_C], size_t n, char coutFillChar = '0', size_t coutWidth = 2){
		for (size_t r = 0; r < n; ++r){
			outPutIterable(array2D[r], array2D[r] + r + 1, coutFillChar, coutWidth);
		}
	}

	template<class Iterator>
	size_t rankStatistics(Iterator left, Iterator right){
		size_t rank = 1;
		iterate(left + 1, right, [&](Iterator left, Iterator right){
			rank += *left == *(left - 1) ? 0 : 1;
		});
		return rank;
	}

	//ARRAY_TEMPLATE
	template<class T, class Iterator>
	T minValueStatistics(Iterator left, Iterator right, T MAX_VALUE){
		T minValue = MAX_VALUE;
		//iterate(left, right, [&](Iterator left, Iterator right){
		//	minValue = min(minValue, *left);
		//});
		while (left != right){
			minValue = min(minValue, *left);
			++left;
		}
		return minValue;
	}

	template<class T, class Iterator>
	T maxValueStatistics(Iterator left, Iterator right, T MIN_VALUE){
		T maxValue = MIN_VALUE;
		while (left != right){
			maxValue = max(maxValue, *left);
			++left;
		}
		return maxValue;
	}

	template<class T, class Iterator>
	T sumValueStatistics(Iterator left, Iterator right, T ZERO_VALUE){
		T sumValue = ZERO_VALUE;
		iterate(left, right, [&](Iterator left, Iterator right){
			sumValue += *left;
		});
		return sumValue;
	}

	template<class T, class Iterator>
	double avlValueStatistics(Iterator left, Iterator right, T ZERO_VALUE){
		return sumValueStatistics(left, right, ZERO_VALUE) / (double)(right - left);
	}
};

int rows, group;
int array2D[MAX_R][MAX_C];
int arrayAAO[MAX_R][MAX_C];

void vTrangleMaxValueAndPathOutput(vector<int> &path){
	auto it = path.begin();
	/*if (it != path.end()){
		cout << *it++ << endl;
	}
	bool outPutFlag = false;
	if (it != path.end()){
		cout << *it++;
		outPutFlag = true;
	}
	while (it != path.end()){
		cout << "-->" << *it++;
	}
	if (outPutFlag){
		puts("");
	}*/
	int sum = StandardExtend::sumValueStatistics(path.begin() + 1, path.end(), 0);
	printf("%d\n", sum);
}

int pathColAA[MAX_R][MAX_C];

void TrangleMaxValueAndPath(){
	vector<int> resultPath;
	int r = rows - 2;
	int pastPathSub = -1;
	for (int c = 0; c < rows; ++c){
		pathColAA[rows - 1][c] = c;
	}
	for (; r >= 0; --r){
		for (int c = 0; c < r + 1; ++c){
			int add = 0;
			if (array2D[r + 1][c] < array2D[r + 1][c + 1]){
				add = array2D[r + 1][c + 1];
				pastPathSub = c + 1;
			}
			else{
				//��һ�еĵ�c�к͵�c+1�����ʱ ѡȡ���С����һ��: c
				add = array2D[r + 1][c];
				pastPathSub = c;
			}
			array2D[r][c] += add;
			pathColAA[r][c] = pastPathSub;
		}
	}
	resultPath.push_back(array2D[0][0]);
	int nextcolSub = 0;
	for (int r = 0; r < rows; ++r){
		resultPath.push_back(arrayAAO[r][nextcolSub]);
		nextcolSub = pathColAA[r][nextcolSub];
	}
	vTrangleMaxValueAndPathOutput(resultPath);
}

//�������� http://139.196.145.92/contest_show.php?cid=556#problem/B

/*
5
13
11 8
12 7 26
6 14 15 8
12 7 13 24 11
*/
void mainForBaseVersion()
{
	auto &input = cin;
	auto &output = cout;

	int n;
	int s[101][101] = { 0 };
	input >> n;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= i; j++)
		{
			input >> s[i][j];
		}
	}

	int max[101][101] = { 0 };
	for (int i = 1; i <= n; i++)
	{
		//��ʼ��,������һ���max�������������������һ���ֵ
		max[n][i] = s[n][i];
	}
	for (int i = n - 1; i >= 1; i--)
	{//�ӵ����ڶ������ϼ���
		for (int j = 1; j <= i; j++)
		{//����� i ���ÿһ����� max[][]
			int one = max[i + 1][j];//Ҫô����һ���Ϻ�������������
			int two = max[i + 1][j + 1];//Ҫô����һ���Ϻ���������ұ���
			if (one > two)
			{
				max[i][j] = s[i][j] + one;
			}
			else
			{
				max[i][j] = s[i][j] + two;
			}
		}
	}
	output << max[1][1];
}
int main_TrangleMaxValueAndPath(){
	while (1 == scanf("%d", &rows)){
		for (int r = 0; r < rows; ++r){
			for (int c = 0; c < r + 1; ++c){
				scanf("%d", &array2D[r][c]);
				arrayAAO[r][c] = array2D[r][c];
			}
		}
		//outPut2DArray(array2D, rows);
		TrangleMaxValueAndPath();
	}
	return 0;
}

int gcd(long long a, long long b)
{
	return b == 0 ? a : gcd(b, a % b);
}

//��� 3/2+2/3+5/12+��+(m+2)/[m*(m+1)]�ĺͲ�����n�����m���� http://139.196.145.92/contest_show.php?cid=556#problem/A
int main(){
	int n;
	double sum, pastSum;
	int ansTable[21] = {-1
		, 0
		, 1
		, 4
		, 11
		, 31
		, 83
		, 227
		, 616
		, 1674
		, 4550
		, 12367
		, 33617
		, 91380
		, 248397
		, 675214
		, 1835421
		, 4989191
		, 13562027
		, 36865412
		, 100210581
	};
	//freopen("input", "r", stdin);
	while (1 == scanf("%d", &n)){
		printf("%d\n", ansTable[n]);
		continue;
		long long m = 0;
		pastSum = sum = 0.0;
		do{
			++m;
			//Լ�ֻ���
			long long upValue = (m + 2);
			long long downValue = (m*(m + 1));
			long long g = gcd(upValue, downValue);
			upValue /= g;
			downValue /= g;
			pastSum = sum;
			//��ĸΪ0ʱû����
			sum += (double)upValue / downValue;
			//printf("%d/%d ", upValue, downValue);
		}while (!(sum > n));
		//printf("%lld %f\n", m-1, pastSum);
		printf("%lld\n", m - 1);
	}
	return 0;
}

