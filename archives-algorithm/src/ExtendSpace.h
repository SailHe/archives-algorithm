#pragma once

#ifndef _STANDARD_H_
#define _STANDARD_H_
#include "stdafx.h"
/*
std::setfill(char ch)        Fill whitespace with 'ch'
setprecision(int n)     Set floating - point precision to 'n'
std::setw(int w)             Read or write value to 'w' characters
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
const double EPS_DOUBLE = 1e-6;
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
	//using namespace std;
	using namespace JCE;

	//��ΪСд�ַ�
	char toLowerAlph(char c);
	int toIntNum(char alphOrCharNum);
	//10ToA(Alph)
	char toUppercaseAscllChar(int num);
	//'A' == 'a' == '0' ... 'J' == 'j' == '9' �Դ�����
	bool isAa0Equal(char a, char b);

	// =====DATE
	//�����������ʱ����λ��Minite��
	double calcDifftime(time_t startTime);
	//�����������ʱ�Ӳ��λ��s��
	double calcDiffClock(clock_t startClock);
	//���ڲ���Function ���Զ����Function��ִ��ʱ��(��λ��s)
	template<class Function>
	void testAndDiffClock(Function fun, JCE::String const &name = "") {
		clock_t startClock = clock();
		fun();
		std::cout << name + "ִ��ʱ�䣺" << calcDiffClock(startClock) << std::endl;
	}
	//�����ж�
	int isIntercalary(int year);
	//�������(��, ��)�ǲ���(��)�еĵڼ��� ��: �� ��-��-�� ת��Ϊ ����
	//���� (year, month, day ��ǰ��, ��, ��)
	//��������
	int calcNumberOfDays(int year, int month, int day);
	//����t1��t2��ķ���ʱ���min, ����h��min������ڲ���ָ����
	//t��ʽ805 (��ʾ08:05)
	int timeNumDiffrence(int t1, int t2, int *t_h, int *t_min);

	//���stdin������ֱ������end�ַ�
	void refreshStdin(char end);
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
	void outPutIterable(Iterator left, Iterator right, SizeType coutWidth = 0, char coutFillChar = '\0'){
		std::cout << std::setfill(coutFillChar);
		int count = -1;
		iterate(left, right, [&](Iterator left){
			char c = (++count == 0 ? '\0' : ' ');
			std::cout << c << std::setw(coutWidth) << *left;
		});
		puts("");
	}

	template<class Iterator>
	//lineWrap �������л��� С��0 ��ʾ�����������
	void outPutIterable(Iterator left, Iterator right, SizeType coutWidth, char coutFillChar, SizeType lineWrap){
		std::cout << std::setfill(coutFillChar);
		if (left == right) {
			return;
		}
		std::cout << *left;
		++left;
		int c = 1;
		lineWrap = lineWrap < 0 ? right - left : lineWrap;
		iterate(left, right, [&](Iterator left){
			++c;
			if (c % lineWrap == 0){
				//���һ��
				//std::cout << " " << std::setw(coutWidth) << *left << std::endl;
				std::cout << std::setw(coutWidth) << *left << std::endl;
			}
			else if (c % lineWrap == 1){
				//��һ��
				std::cout << *left;
			}
			else{
				//�м��
				//std::cout << " " << std::setw(coutWidth) << *left;
				std::cout << std::setw(coutWidth) << *left;
			}
		});
		//���һ��û�л������ﻻ��
		if (c % lineWrap != 0){
			puts("");
		}
	}

	template<class Iterator, class Fun>
	//lineWrapFun �����Ƿ���  bool(* lineWrapFun)(Iterator left, Iterator right)
	void outPutIterable(Iterator left, Iterator right, char coutFillChar, SizeType coutWidth, Fun lineWrapFun){
		std::cout << std::setfill(coutFillChar);
		int c = 0;
		bool pastWrap = false;
		//lineWrapFun = nullptr == lineWrapFun ? [&](){return right - left; } : lineWrapFun;
		iterate(left, right, [&](Iterator left){
			++c;
			pastWrap = lineWrapFun(left, right);
			if (1 == c){//��һ��
				std::cout << *left;
			}
			else{
				//�м�&���
				std::cout << std::setw(coutWidth) << *left;
			}
			if (pastWrap){//���һ��
				std::cout << std::endl;
				c = 0;
			}
		});
		//���һ��û�л������ﻻ��
		if (!pastWrap){
			puts("");
		}
	}


	template<class T>
	void outPut2DArrayList(ArrayList<ArrayList<T>> const &arrayList2D, char coutFillChar = '0', SizeType coutWidth = 2){
		SizeType rows = arrayList2D.size();
		for (SizeType r = 0; r < rows; ++r){
			outPutIterable(arrayList2D[r].begin(), arrayList2D[r].end(), coutWidth, coutFillChar);
		}
	}

	template<class T>
	void outPut2DArray(T array2D[MAX_R][MAX_C], SizeType n, SizeType coutWidth = 2, char coutFillChar = '0'){
		for (SizeType r = 0; r < n; ++r){
			outPutIterable(array2D[r], array2D[r] + n, coutWidth, coutFillChar);
		}
	}

	template<class T>
	void outPut2DArrayTrangle(T array2D[MAX_R][MAX_C], SizeType n, SizeType coutWidth = 2, char coutFillChar = '0'){
		for (SizeType r = 0; r < n; ++r){
			outPutIterable(array2D[r], array2D[r] + r + 1, coutWidth, coutFillChar);
		}
	}

	template<class Iterator>
	SizeType rankStatistics(Iterator left, Iterator right){
		SizeType rank = 1;
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
	//using namespace StandardExtend;
	/************����*****0*********45*******90*********135*******180********225*******270********315****/
	//int dir[8][2] = { { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 } };
	const int Dir8[8][2] = { { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };
	const int Dir4[4][2] = { { 0, -1 /*��*/ }, { 0, 1 /*��*/ }, { -1, 0 /*��*/ }, { 1, 0 /*��*/ } };

	//�����˾��ȱȽϷ���˫���ȸ���
	class Double {
	public:
		Double() {}
		Double(int rhs) {
			value = rhs;
		}
		Double(double rhs) {
			value = rhs;
		}
		bool operator==(Double const &rhs) const {
			return fabs(this->value - rhs.value) < EPS;
		}

		//cout<<
		friend std::ostream &operator<<(std::ostream &os, const Double &rhs) {
			os << rhs.value;
			return os;
		}

		static void setEPS(double EPS) {
			Double::EPS = EPS;
		}
		static double getEPS() {
			return EPS;
		}
	private:
		double value;
		//1e-6
		static double EPS;
	};

	//�Ǿۺ�  ����
	class PointDouble {
	public:
		double x, y;
		PointDouble() = default;
		PointDouble(double x, double y) {
			x = y, y = y;
		}
		PointDouble(std::istream &in) {
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
	T median(StandardExtend::ArrayList<T> ans) {
		std::sort(ans.begin(), ans.end());
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
	void reverse(StandardExtend::ArrayList<StandardExtend::ArrayList<T>> &a, Sub leftSub, Sub rightSub, StandardExtend::SizeType reverseRows = 1) {
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
	void leftMoveK(StandardExtend::ArrayList<StandardExtend::ArrayList<T>> &a, Sub R, Sub C, StandardExtend::SizeType k) {
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
	void rightMoveK(StandardExtend::ArrayList<StandardExtend::ArrayList<T>> &a, int R, int C, int k) {
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
	StandardExtend::SizeType calcFirstLonConIncSubSeq(StandardExtend::ArrayList<T> const &sequence, int &leftResultSub, int &rightResultSub) {
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
	//using namespace Utility;
	
	//����˷� productMatrix = productMatrix*originMatrix(����������ͬ)
	template<class T>
	void matrixMultiply(const Sub maxRowCol, StandardExtend::ArrayList<StandardExtend::ArrayList<T>> &originMatrix, StandardExtend::ArrayList<StandardExtend::ArrayList<T>> &productMatrix) {
		int r, c, i, tmp;
		StandardExtend::ArrayList<StandardExtend::ArrayList<T>> tempMatrix;
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

	// =======�� ��

	// ���ֲ���origin[n]��key ����key�ĵ�����, ��û�з���origin+size������ ע��:����ж����ͬԪ������������ص�Ԫ�ؿ�������������һ��
	// origin��������������������֧������[]����(����ʽ��������������) ����������[origin, origin+size)��Χ��Ԫ�ر���������
	// std�����ƶ�������ʵ��(std::binary_search) ��ֻ�� ���ȼ��� value ��Ԫ���Ƿ�����ڷ�Χ [first, last) ��
	// ���ƵĻ���:	A. count,find
	//				B.binary_search, lower_bound, upper_bound, equal_range
	template<class Iterator, class ValueType>
	Iterator dichotomy(Iterator origin, JCE::SizeType size, ValueType key) {
		int left = 0, right = size, mid = 0;
		while (left <= right) {
			mid = (right + left) / 2;
			if (origin[mid] < key) {
				left = mid + 1;
			}//��1֮ǰ��ȷ��mid���Ϸ�, ���ǿ��԰��������õ���һ�εļ�����, ������1��һ�ο��ܻ����¼���mid
			else if (origin[mid] > key) {
				//ͬ��
				right = mid - 1;
			}
			else {
				return origin + mid;
			}
		}
		return origin + size;
	}
	// ���ַ� ��һԪ����f(x)������[left, right]�ڵĵ���С��x (eps: ����, �����ֵ���ⲿʵ��)
	template<class TY, class TX, class Fun>
	//TY(*fun)(TX x)
	TX trichotomy(TX left, TX right, Fun fun, TX eps) {
		TX Ll, Rr;
		while (right - left > eps) {
			//����
			Ll = (2 * left + right) / 3;
			Rr = (2 * right + left) / 3;
			if (fun(Ll) > fun(Rr)) {
				left = Ll;
			}
			else {
				right = Rr;
			}
		}
		//����left ,right��һ������(PS: ��Ӧ����f(x)��ֵ ��Ϊfun���ܲ����û���Ҫ��fun �μ�����ע��)
		return left;
	}

	// =======���޵�(Confined)

	//���ش��ű�(malloc) 64 bit ������λ ������26�� T ��Ҫ֧�����������Լ��Լ�
	template<class T>
	StandardExtend::ArrayList<T> calcIllArrangeList(StandardExtend::SizeType MAX_SIZE = 26) {
		StandardExtend::ArrayList<T> M(MAX_SIZE, 0);
		M[0] = 0, M[1] = 1, M[2] = 1;
		for (StandardExtend::SizeType n = 3; n < 26; ++n)
			M[n] = (n - 1) * (M[n - 1] + M[n - 2]);
		return M;
	}
	//���digit��[leftSub, rightSub)��ȫ����  ���ֵ���
	template<class T, class Fun>
	void penetration(StandardExtend::ArrayList<T> container, Sub leftSub, Sub rightSub, Fun visit) {
		if (leftSub == rightSub) {
			visit(container);
		}
		else {
			for (int i = leftSub; i < rightSub; ++i) {
				std::swap(container[leftSub], container[i]);
				penetration(container, leftSub + 1, rightSub, visit);
				std::swap(container[leftSub], container[i]);
			}
		}
	}
	//�������1-maxDigit(1, 9)��ȫ����  �ֵ���
	void pentration(int maxDigit);
	//�������1-maxDigit(1, 9)��ȫ����  ���ֵ���
	void pentration();
	//��ŵ���ݹ�ⷨ
	void hannoTowerMove(int n, char a, char b, char c);
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



	I64 quickFact(I64 a, I64 b, I64 mod);
	//�����ݻ���quickFact�� Ч���Ǽ��װ��1/5
	I64 quickPow_OLD(I64 C, I64 R, I64 k);
	//�����ݼ��װ� m^n % k ������ڼ���powЧ������ͨ���1/20 (��ͨ��Ч�ʸ�)
	I64 quickPow(I64 m, I64 n, I64 k);
	//��������ͨ�� m^n; ��std::powЧ�ʸ��иߵ����� std::powֵ�����
	I64 quickPow(I64 m, I64 n);
	//�����������
	double logR(double value, double base = 5);
	//�� ��
	double fact(int n);
	//int �޶��ڽ׳˱� (int 13!���� double��long long int 18!��)
	int* factTable(int maxN = 13);
	/*
	n < m && m != 0
		 fact(n)
	 ________________
	fact(n - m) * fact(m)
	*/
	//�����
	int C(int n, int m);
	//������
	int A(int n, int m);
	//����(����)�ж�
	int isPrime_OLD(int num);
	//����(����)�ж� sqrt��ƽ���Ż���(δʵ��)
	bool isPrime(int num);
	//�����Ӻ�(���������Լ����
	int factorSum(int x);
	//�����Ӻͱ�(Լ�� ����) (10^7һǧ��2.51s)(400w 10^6 900ms)
	void factorSumTableSieve(const int maxn, int a[]);
	//����(����) ɸѡ�� ������ɫ��(Sieve Eratosthenes)(0 1==-1, a[i]==0��ʾ����) maxN[2, 1e9) PS:maxN = 1e7ʱ����1s�� 1e9�ƺ��޷������ڴ�
	int* primeSieve(const int maxN = 2);
	//������Ŀ
	int factorCount(int x);
	//�������� (eps: ����)
	double sqrtImpl(double x, double eps = 1e-9);
	//�󷽲�
	double variance(float x[], int n);

}

#endif
