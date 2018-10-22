#pragma once

#ifndef _STANDARD_H_
#define _STANDARD_H_
//#include "stdafx.h"
#include<stdio.h>
#include<string.h>
#include<string>
#include<cmath>
#include<vector>
#include<iostream>
#include<queue>
#include<functional>
#include<list>
#include<stack>
#include<map>
#include<iomanip>
/*
setfill(char ch)        Fill whitespace with 'ch'
setprecision(int n)     Set floating - point precision to 'n'
setw(int w)             Read or write value to 'w' characters
setbase(int b)          Output integers in base 'b'(only 'b' is 8 / 10 / 16 could the function work)
*/

/*��Biology��ص���������:���� ���� ����Ա*/
enum TYPE { PLAYER, PEOPLE, MANAGER, MONSTER };
enum STATUS { NORMAL, ABNORMAL, CONK, QUIT, BLOCKING, BYPASS };//���� �쳣 ����(�����ó�Ա�����ж�) �˳� ���赲 ����״̬
/*��ͼ: up(����)->��Ӱ, down->����, left->��, right->��  ViewCount:��ͼ����*/
enum View { BACK, FRONT, LEFT, RIGHT, ViewCount };
//����ָ��������ֹʱ�ǲ�����һ֡(��ʼ֡, ��һ֡)
enum StaticFrameEnum { ORIGIN_FRAME, PAST_FRAME };
enum STEP { STEP_X_AXIS, STEP_Y_AXIS };//��������
enum MODE { ON, OFF, UNDEFINED };//ģʽ
//using Package = vector<pair<int, int>>;/*������*/
//constant.h
const View defaultView = BACK;//Ĭ����ͼ(��ֵת��Ϊint������0)

const double EPSINON = 1e-15;
const double EPS = 1e-9;
const double PI = 3.14159265;//��λ��
const double PIA = acos(-1.0);//���ű�����

typedef int Sub;//�±�����
typedef Sub RowsSub;//�б�����
typedef Sub ColsSub;//�б�����
typedef __int64 I64;


#define ARRAY_TEMPLATE template<class T, class Iterator>

//��׼��չ: ���������׼�����������ģ��(iterateϵ��)ֻ������������ ������ʵ��ʹ��
namespace StandardExtend{
	const int MAX_R = 1024;
	const int MAX_C = 1024;
	using namespace std;
	//�ɱ�����
	template<typename T>using Varrays = vector<T>;
	//�����
	template<typename T>using ArrayList = vector<T>;
	//�ڽӱ�
	template<typename T>using LinkedList = list<T>;

	/****************Ascll*******************/
	char toLowerAlph(char c);

	int toIntNum(char alphOrCharNum);

	//10ToA(Alph)
	char toUppercaseAscllChar(int num);

	//'A' == 'a' == '0' ... 'J' == 'j' == '9' �Դ�����
	bool isAa0Equal(char a, char b);

	//���stdin������ֱ������end�ַ�
	void refreshStdin(char end);
	//�����������ʱ����λ��Minite��
	double calcDifftime(time_t startTime);
	//�����������ʱ�Ӳ��λ��ms��
	double calcDiffClock(clock_t startClock);
	//���ڲ���Function ���Զ����Function��ִ��ʱ��(��λ��ms)
	template<class Function>
	void calcDiffClock(Function fun) {
		clock_t startClock = clock();
		fun();
		printf("ִ��ʱ�䣺%lf\n", calcDiffClock(startClock));
	}

	// �����滻Ϊ: destContainer.resize(origin.end() - origin.begin());
	// std::copy(origin.begin(), origin.end(), destContainer.begin());
	template<class T, class Iterator>
	ArrayList<T> toArrayList(Iterator left, Iterator right) {
		ArrayList<T> result;
		result.reserve(right - left);
		iterate(left, right, [&](Iterator it) {
			result.push_back(*it);
		});
		return result;
	}

	template<class T>
	//left <= value < right  EG: inRange(minEle, element, maxEle+1); -> element [minEle, maxEle]
	bool inRange(T left, T value, T right){
		return left <= value && value < right;
	}

	template<class Iterator, class Fun>
	void iterate(Iterator left, Iterator right, Fun visit){
		while (left != right){
			visit(left);
			++left;
		}
	}

	template<class Iterator>
	//coutFillChar��ʾ����ַ� �൱��printf��%02d �е�'0' coutWidth��ʾ���е�2
	void outPutIterable(Iterator left, Iterator right, size_t coutWidth = 0, char coutFillChar = '\0'){
		cout << setfill(coutFillChar);
		int count = -1;
		iterate(left, right, [&](Iterator left){
			char c = (++count == 0 ? '\0' : ' ');
			cout << c << setw(coutWidth) << *left;
		});
		puts("");
	}

	template<class Iterator>
	//lineWrap �������л��� С��0 ��ʾ�����������
	void outPutIterable(Iterator left, Iterator right, size_t coutWidth, char coutFillChar, size_t lineWrap){
		cout << setfill(coutFillChar);
		if (left == right) {
			return;
		}
		cout << *left;
		++left;
		int c = 1;
		lineWrap = lineWrap < 0 ? right - left : lineWrap;
		iterate(left, right, [&](Iterator left){
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
		iterate(left, right, [&](Iterator left){
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
			outPutIterable(arrayList2D[r].begin(), arrayList2D[r].end(), coutWidth, coutFillChar);
		}
	}

	template<class T>
	void outPut2DArray(T array2D[MAX_R][MAX_C], size_t n, size_t coutWidth = 2, char coutFillChar = '0'){
		for (size_t r = 0; r < n; ++r){
			outPutIterable(array2D[r], array2D[r] + n, coutWidth, coutFillChar);
		}
	}

	template<class T>
	void outPut2DArrayTrangle(T array2D[MAX_R][MAX_C], size_t n, size_t coutWidth = 2, char coutFillChar = '0'){
		for (size_t r = 0; r < n; ++r){
			outPutIterable(array2D[r], array2D[r] + r + 1, coutWidth, coutFillChar);
		}
	}

	template<class Iterator>
	size_t rankStatistics(Iterator left, Iterator right){
		size_t rank = 1;
		iterate(left + 1, right, [&](Iterator left){
			rank += *left == *(left - 1) ? 0 : 1;
		});
		return rank;
	}

	//ARRAY_TEMPLATE
	template<class T, class Iterator>
	T minValueStatistics(Iterator left, Iterator right, T MAX_VALUE){
		T minValue = MAX_VALUE;
		//iterate(left, right, [&](Iterator left){
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
		iterate(left, right, [&](Iterator left){
			sumValue += *left;
		});
		return sumValue;
	}

	template<class T, class Iterator>
	double avlValueStatistics(Iterator left, Iterator right, T ZERO_VALUE){
		return sumValueStatistics(left, right, ZERO_VALUE) / (double)(right - left);
	}

};

//Ч����չ
namespace Utility {
	using namespace StandardExtend;
	/************����*****0*********45*******90*********135*******180********225*******270********315****/
	//int dir[8][2] = { { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 } };
	const int Dir8[8][2] = { { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };
	const int Dir4[4][2] = { { 0, -1 /*��*/ }, { 0, 1 /*��*/ }, { -1, 0 /*��*/ }, { 1, 0 /*��*/ } };

	//�Ǿۺ�  ����
	class PointDouble {
	public:
		double x, y;
		PointDouble() = default;
		PointDouble(double x, double y) {
			x = y, y = y;
		}
		PointDouble(istream &in) {
			//scanf("%lf%lf", &(this->x), &(this->y));
			in >> this->x >> this->y;
		}
		PointDouble O() {
			PointDouble ZERO = { 0.0, 0.0 };
			return ZERO;
		}

		//�ܴ���ʼ��'��Ծ��'�յ㷵��true
		static bool canJumpTo(PointDouble const &beganPoint, PointDouble const &endPoint, double jumpPower, double raduis) {
			//��� - �յ� - ��Ծ���� - ��뾶
			return dDistance(beganPoint, endPoint) <= raduis + jumpPower;
		}
		//������������
		static double dDistance(PointDouble const &p1, PointDouble const &p2) {
			return sqrt(dDistance2(p1, p2));
		}
		//�������������ƽ��
		static double dDistance2(PointDouble const &p1, PointDouble const &p2) {
			double dx = p2.x - p1.x;
			double dy = p2.y - p1.y;
			return (dx*dx + dy * dy);
		}
	};

	class Triangle {
	public:
		Triangle(){}
		Triangle(PointDouble const &p1, PointDouble const &p2, PointDouble const &p3){
			this->p1 = p1;
			this->p2 = p2;
			this->p3 = p3;
		}
		~Triangle(){}

		//������p1, p2, p3��ɵ������ε����
		static double dAreaOfTriangle(PointDouble const &p1, PointDouble const &p2, PointDouble const &p3) {
			double d12 = PointDouble::dDistance(p1, p2)
				, d13 = PointDouble::dDistance(p1, p3)
				, d23 = PointDouble::dDistance(p2, p3);
			//���׹�ʽ
			double dL = (d12 + d13 + d23) / 2.0;
			return sqrt(dL * (dL - d12) * (dL - d13) * (dL - d23));
		}
		//����pλ��p1, p2, p3��ɵ�����������ô����true ���򷵻�false
		static bool isInternalTriangle(PointDouble const &p1, PointDouble const &p2, PointDouble const &p3, PointDouble const &p) {
			double a12p = dAreaOfTriangle(p1, p2, p)
				, a13p = dAreaOfTriangle(p1, p3, p)
				, a23p = dAreaOfTriangle(p2, p3, p)
				, a123 = dAreaOfTriangle(p1, p2, p3);
			//���Ǹ���ָ�������������֮�������������������Χ�����ʱ �����������ڲ� ���ȹ��߿��ܻᱻ�� һ��1E-5����
			return (fabs(a123 - (a12p + a13p + a23p)) < EPS);
		}
		void mainForTriangle() {
			//freopen("input", "r", stdin);
			PointDouble p1, p2, p3, p;
			while (8 == scanf_s("%lf%lf%lf%lf%lf%lf%lf%lf"
				, &p1.x, &p1.y
				, &p2.x, &p2.y
				, &p3.x, &p3.y
				, &p.x, &p.y)) {
				puts(isInternalTriangle(p1, p2, p3, p) ? "Yes" : "No");
			}
		}
		//���Ϲ��ɶ��� ����true ��ϰ��HDU2899
		bool isPythagoreanTheorem(int a, int b, int c) {
			return a * a == b * b + c * c || b * b == a * a + c * c || c * c == a * a + b * b;
		}
	private:
		PointDouble p1, p2, p3;
	};

	//ansΪs1+s2(s1��s2�Ĳ���)(ֱ��ans.push_back(temp)����û��Ҫ��ʵ������s1��s2)
	//����s1��s2��������λ��, ansΪs1+s2(ֱ��push_back����) a0,a1,a2...an-1����λ��ָa[(n-1)/2] (��n����)����(n+1)/2����(a0Ϊ��һ����)
	template<class T>
	T median(ArrayList<T> ans) {
		sort(ans.begin(), ans.end());
		if (ans.size() % 2 == 0)
			return ans[ans.size() / 2 - 1];
		else
			return ans[ans.size() / 2];
	}
	//����(����ʵ�� �ױ�)
	template<class T>
	void swapUnstable(T &a, T &b){
		a += b;
		b = a - b;
		a = a - b;
	}
	//��תreverseRows�еĶ�ά����a ÿ��λ������[��leftSub,��rightSub)��Ԫ��
	template<class T>
	void reverse(ArrayList<ArrayList<T>> &a, Sub leftSub, Sub rightSub, size_t reverseRows = 1) {
		for (int r = 0; r < reverseRows; r++) {
			for (int c = leftSub, i = 0; i < (rightSub - leftSub) / 2; i++, c++) {
				int temp = a[r][c];
				a[r][c] = a[r][rightSub - i - 1];
				a[r][rightSub - i - 1] = temp;
			}
		}
	}
	/*��R�� C�еĶ�ά����������kλ*/
	template<class T>
	void leftMoveK(ArrayList<ArrayList<T>> &a, Sub R, Sub C, size_t k) {
		/*
		8 3
		1 2 3 4 5 6 7 8
		8 7 6 5 4/ 3 2 1
		4 5 6 7 8/ 1 2 3
		*/
		k %= C;
		reverse(a, 0, C, R);/*��ת����Ԫ��*/
		reverse(a, 0, C - k, R);/*��תǰC-k��Ԫ��*/
		reverse(a, C - k, C, R);/*��ת��k��Ԫ��*/
	}
	/*����kλ<==>����C-kλ leftMoveK(a, R, C, C-k); <==> rightMoveK(a, R, C, k);*/
	template<class T>
	void rightMoveK(ArrayList<ArrayList<T>> &a, int R, int C, int k) {
		/*
		8 3
		1 2 3 4 5 6 7 8
		8 7 6/ 5 4 3 2 1
		6 7 8/ 1 2 3 4 5
		*/
		k %= C;
		reverse(a, 0, C, R);
		reverse(a, 0, k, R);/*��תǰk��Ԫ��*/
		reverse(a, k, C, R);/*��ת��C-k��Ԫ��*/
	}
	// �����һ�����������������(longest continuous increment sub sequence)
	// ���س��� ����:���ڴ�������������е�����[leftResultSub, rightResultSub)
	template<class T>
	size_t calcFirstLonConIncSubSeq(vector<T> const &sequence, int &leftResultSub, int &rightResultSub) {
		int maxLen = 0, sequenceLen = sequence.size();
		//rightResultSub=-1: ʹ��������ǳ���ʱΪ0
		leftResultSub = 0, rightResultSub = -1;
		for (int l = 0, r = 0, len = 0;; ++r) {
			len = r - l + 1;
			maxLen = rightResultSub - leftResultSub + 1;
			//������������� (��һ��������:��ȡ��)
			if (len > maxLen) {
				leftResultSub = l, rightResultSub = r;
			}
			else {
				//do nothing
			}
			if (r >= sequenceLen - 1)
				break;
			//r < sequenceLen - 1 ��֤sequenceLen==1ʱ��ȷ
			if (sequence[r] < sequence[r + 1]) {
				//do nothing
			}
			//��ǰ���в��ǵ�������
			else {
				////���þֲ���������
				l = r + 1;
			}
		}
		++rightResultSub;
		return maxLen;
	}
}

//��ѧ��չ
namespace MathExtend {
	using namespace Utility;
	
	//����˷� productMatrix = productMatrix*originMatrix(����������ͬ)
	template<class T>
	void matrixMultiply(const Sub maxRowCol, ArrayList<ArrayList<T>> &originMatrix, ArrayList<ArrayList<T>> &productMatrix) {
		int r, c, i, tmp;
		ArrayList<ArrayList<T>> tempMatrix;
		//memcpy(tempMatrix, productMatrix, maxRowCol * 25 * sizeof(int));
		tempMatrix = originMatrix;
		for (r = 0; r < maxRowCol; r++) {
			for (c = 0; c < maxRowCol; c++) {
				//���б�����Ӧ��� �ۼӸ�tmp �ٸ��� productMatrix
				for (tmp = i = 0; i < maxRowCol; i++)
					tmp += originMatrix[r][i] * tempMatrix[i][c];
				productMatrix[r][c] = tmp;
			}
		}
	}
	// ���ַ� ����fun��[L, R]����Сֵ (eps: ����) �����ֵ��ͨ��fun�ⲿʵ��
	double trichotomy(double L, double R, double eps, double(*fun)(double));


	//���ش��ű�(malloc) 64 bit ������λ ������26�� T ��Ҫ֧�����������Լ��Լ�
	template<class T>
	ArrayList<T> calcIllArrangeList(size_t MAX_SIZE = 26) {
		ArrayList<T> M(MAX_SIZE, 0);
		M[0] = 0, M[1] = 1, M[2] = 1;
		for (size_t n = 3; n < 26; ++n)
			M[n] = (n - 1) * (M[n - 1] + M[n - 2]);
		return M;
	}
	//���digit��[leftSub, rightSub)��ȫ����  ���ֵ���
	template<class T, class Fun>
	void penetration(ArrayList<T> container, Sub leftSub, Sub rightSub, Fun visit) {
		if (leftSub == rightSub) {
			visit(container);
		}
		else {
			for (int i = leftSub; i < rightSub; ++i) {
				swap(container[leftSub], container[i]);
				penetration(container, leftSub + 1, rightSub, visit);
				swap(container[leftSub], container[i]);
			}
		}
	}
	//�������1-maxDigit(1, 9)��ȫ����  �ֵ���
	void pentration(int maxDigit);
	//�������1-maxDigit(1, 9)��ȫ����  ���ֵ���
	void pentration();
	//��ŵ���ݹ�ⷨ
	void hannoTowerMove(int n, char a, char b, char c);
	//Confined(���޵�)

	//��10���Ƶ�numberת��Ϊradix���Ƶ��ַ��� (�ݹ�ʵ��)
	std::string decToBin(int number, std::string &result, int radix = 2);
	//������bigInteger�ܱ���������true
	bool isDivisible(char *bigInteger, int MOD);
	//ɾ��resStr�е������Ӵ�delSubStr
	void eraseAllSubStr(char *resStr, char *delSubStr);
	//������
	int reverseOrderNumber(int number);
	//����radix��������number��digit���ֵĴ���
	int calcDigitCountInNumber(const int number, const int digit, int radix = 10);
	//����radix����������λ��
	int calcDigitTop(int number, int radix = 10);
}

#endif
