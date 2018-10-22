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

/*与Biology相关的三种类型:人物 怪物 管理员*/
enum TYPE { PLAYER, PEOPLE, MANAGER, MONSTER };
enum STATUS { NORMAL, ABNORMAL, CONK, QUIT, BLOCKING, BYPASS };//正常 异常 昏迷(死亡用成员方法判断) 退出 被阻挡 绕行状态
/*视图: up(向上)->背影, down->正面, left->左, right->右  ViewCount:视图个数*/
enum View { BACK, FRONT, LEFT, RIGHT, ViewCount };
//用于指定动画静止时是播放哪一帧(起始帧, 上一帧)
enum StaticFrameEnum { ORIGIN_FRAME, PAST_FRAME };
enum STEP { STEP_X_AXIS, STEP_Y_AXIS };//步进方向
enum MODE { ON, OFF, UNDEFINED };//模式
//using Package = vector<pair<int, int>>;/*背包类*/
//constant.h
const View defaultView = BACK;//默认视图(此值转换为int必须是0)

const double EPSINON = 1e-15;
const double EPS = 1e-9;
const double EPS_DOUBLE = 1e-6;
const double PI = 3.14159265;//八位π
const double PIA = acos(-1.0);//蓝桥杯可用

typedef int Sub;//下标类型
typedef Sub RowsSub;//行标类型
typedef Sub ColsSub;//列标类型
typedef __int64 I64;


#define ARRAY_TEMPLATE template<class T, class Iterator>

//标准扩展: 里面的许多标准输入输出方法模板(iterate系列)只建议用作测试 不建议实际使用
namespace StandardExtend{
	const int MAX_R = 1024;
	const int MAX_C = 1024;
	//using namespace std;
	using namespace JCE;

	//变为小写字符
	char toLowerAlph(char c);
	int toIntNum(char alphOrCharNum);
	//10ToA(Alph)
	char toUppercaseAscllChar(int num);
	//'A' == 'a' == '0' ... 'J' == 'j' == '9' 以此类推
	bool isAa0Equal(char a, char b);

	// =====DATE
	//计算与参数的时间差（单位：Minite）
	double calcDifftime(time_t startTime);
	//计算与参数的时钟差（单位：s）
	double calcDiffClock(clock_t startClock);
	//用于测试Function 会自动输出Function的执行时钟(单位：s)
	template<class Function>
	void testAndDiffClock(Function fun, JCE::String const &name = "") {
		clock_t startClock = clock();
		fun();
		std::cout << name + "执行时间：" << calcDiffClock(startClock) << std::endl;
	}
	//闰年判断
	int isIntercalary(int year);
	//计算参数(月, 日)是参数(年)中的第几天 即: 将 年-月-日 转换为 天数
	//传入 (year, month, day 当前年, 月, 日)
	//返回天数
	int calcNumberOfDays(int year, int month, int day);
	//返回t1与t2间的分数时间差min, 并将h，min结果存于参数指针中
	//t格式805 (表示08:05)
	int timeNumDiffrence(int t1, int t2, int *t_h, int *t_min);

	//清空stdin输入流直至遇到end字符
	void refreshStdin(char end);
	// 可以替换为: destContainer.resize(origin.end() - origin.begin());
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
	//coutFillChar表示填充字符 相当于printf的%02d 中的'0' coutWidth表示其中的2
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
	//lineWrap 隔多少行换行 小于0 表示输出结束换行
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
				//最后一个
				//std::cout << " " << std::setw(coutWidth) << *left << std::endl;
				std::cout << std::setw(coutWidth) << *left << std::endl;
			}
			else if (c % lineWrap == 1){
				//第一个
				std::cout << *left;
			}
			else{
				//中间的
				//std::cout << " " << std::setw(coutWidth) << *left;
				std::cout << std::setw(coutWidth) << *left;
			}
		});
		//最后一次没有换行这里换行
		if (c % lineWrap != 0){
			puts("");
		}
	}

	template<class Iterator, class Fun>
	//lineWrapFun 返回是否换行  bool(* lineWrapFun)(Iterator left, Iterator right)
	void outPutIterable(Iterator left, Iterator right, char coutFillChar, SizeType coutWidth, Fun lineWrapFun){
		std::cout << std::setfill(coutFillChar);
		int c = 0;
		bool pastWrap = false;
		//lineWrapFun = nullptr == lineWrapFun ? [&](){return right - left; } : lineWrapFun;
		iterate(left, right, [&](Iterator left){
			++c;
			pastWrap = lineWrapFun(left, right);
			if (1 == c){//第一个
				std::cout << *left;
			}
			else{
				//中间&最后
				std::cout << std::setw(coutWidth) << *left;
			}
			if (pastWrap){//最后一个
				std::cout << std::endl;
				c = 0;
			}
		});
		//最后一次没有换行这里换行
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

//效用扩展
namespace Utility {
	//using namespace StandardExtend;
	/************逆向*****0*********45*******90*********135*******180********225*******270********315****/
	//int dir[8][2] = { { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 } };
	const int Dir8[8][2] = { { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };
	const int Dir4[4][2] = { { 0, -1 /*左*/ }, { 0, 1 /*右*/ }, { -1, 0 /*上*/ }, { 1, 0 /*下*/ } };

	//重载了精度比较符的双精度浮点
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

	//非聚合  点类
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

		//能从起始点'跳跃至'终点返回true
		static bool canJumpTo(PointDouble const &beganPoint, PointDouble const &endPoint, double jumpPower, double raduis) {
			//起点 - 终点 - 跳跃能力 - 活动半径
			return dDistance(beganPoint, endPoint) <= raduis + jumpPower;
		}
		//返回两点间距离
		static double dDistance(PointDouble const &p1, PointDouble const &p2) {
			return sqrt(dDistance2(p1, p2));
		}
		//返回两点间距离的平方
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

		//返回由p1, p2, p3组成的三角形的面积
		static double dAreaOfTriangle(PointDouble const &p1, PointDouble const &p2, PointDouble const &p3) {
			double d12 = PointDouble::dDistance(p1, p2)
				, d13 = PointDouble::dDistance(p1, p3)
				, d23 = PointDouble::dDistance(p2, p3);
			//海伦公式
			double dL = (d12 + d13 + d23) / 2.0;
			return sqrt(dL * (dL - d12) * (dL - d13) * (dL - d23));
		}
		//若点p位于p1, p2, p3组成的三角形内那么返回true 否则返回false
		static bool isInternalTriangle(PointDouble const &p1, PointDouble const &p2, PointDouble const &p3, PointDouble const &p) {
			double a12p = dAreaOfTriangle(p1, p2, p)
				, a13p = dAreaOfTriangle(p1, p3, p)
				, a23p = dAreaOfTriangle(p2, p3, p)
				, a123 = dAreaOfTriangle(p1, p2, p3);
			//被那个点分割出的所有面积块之和与三角形总面积在误差范围内相等时 则在三角形内部 精度过高可能会被坑 一般1E-5即可
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
		//符合勾股定理 返回true 练习题HDU2899
		bool isPythagoreanTheorem(int a, int b, int c) {
			return a * a == b * b + c * c || b * b == a * a + c * c || c * c == a * a + b * b;
		}
	private:
		PointDouble p1, p2, p3;
	};

	//ans为s1+s2(s1与s2的并集)(直接ans.push_back(temp)即可没必要建实际生物s1与s2)
	//返回s1与s2并集的中位数, ans为s1+s2(直接push_back即可) a0,a1,a2...an-1的中位数指a[(n-1)/2] (共n个数)即第(n+1)/2个数(a0为第一个数)
	template<class T>
	T median(StandardExtend::ArrayList<T> ans) {
		std::sort(ans.begin(), ans.end());
		if (ans.size() % 2 == 0)
			return ans[ans.size() / 2 - 1];
		else
			return ans[ans.size() / 2];
	}
	//交换(引用实现 易爆)
	template<class T>
	void swapUnstable(T &a, T &b){
		a += b;
		b = a - b;
		a = a - b;
	}
	//反转reverseRows行的二维数组a 每行位于区间[列leftSub,列rightSub)的元素
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
	/*将R行 C列的二维数组向左移k位*/
	template<class T>
	void leftMoveK(StandardExtend::ArrayList<StandardExtend::ArrayList<T>> &a, Sub R, Sub C, StandardExtend::SizeType k) {
		/*
		8 3
		1 2 3 4 5 6 7 8
		8 7 6 5 4/ 3 2 1
		4 5 6 7 8/ 1 2 3
		*/
		k %= C;
		reverse(a, 0, C, R);/*反转所有元素*/
		reverse(a, 0, C - k, R);/*反转前C-k个元素*/
		reverse(a, C - k, C, R);/*反转后k个元素*/
	}
	/*右移k位<==>左移C-k位 leftMoveK(a, R, C, C-k); <==> rightMoveK(a, R, C, k);*/
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
		reverse(a, 0, k, R);/*反转前k个元素*/
		reverse(a, k, C, R);/*反转后C-k个元素*/
	}
	// 计算第一个最长连续递增子序列(longest continuous increment sub sequence)
	// 返回长度 参数:用于储存最长连续子序列的区间[leftResultSub, rightResultSub)
	template<class T>
	StandardExtend::SizeType calcFirstLonConIncSubSeq(StandardExtend::ArrayList<T> const &sequence, int &leftResultSub, int &rightResultSub) {
		int maxLen = 0, sequenceLen = sequence.size();
		//rightResultSub=-1: 使计算其初是长度时为0
		leftResultSub = 0, rightResultSub = -1;
		for (int l = 0, r = 0, len = 0;; ++r) {
			len = r - l + 1;
			maxLen = rightResultSub - leftResultSub + 1;
			//更新最长递增子列 (第一个子序列:不取等)
			if (len > maxLen) {
				leftResultSub = l, rightResultSub = r;
			}
			else {
				//do nothing
			}
			if (r >= sequenceLen - 1)
				break;
			//r < sequenceLen - 1 保证sequenceLen==1时正确
			if (sequence[r] < sequence[r + 1]) {
				//do nothing
			}
			//当前子列不是递增子列
			else {
				////重置局部递增子列
				l = r + 1;
			}
		}
		++rightResultSub;
		return maxLen;
	}
}

//数学扩展
namespace MathExtend {
	//using namespace Utility;
	
	//矩阵乘法 productMatrix = productMatrix*originMatrix(两矩阵不能相同)
	template<class T>
	void matrixMultiply(const Sub maxRowCol, StandardExtend::ArrayList<StandardExtend::ArrayList<T>> &originMatrix, StandardExtend::ArrayList<StandardExtend::ArrayList<T>> &productMatrix) {
		int r, c, i, tmp;
		StandardExtend::ArrayList<StandardExtend::ArrayList<T>> tempMatrix;
		//memcpy(tempMatrix, productMatrix, maxRowCol * 25 * sizeof(int));
		tempMatrix = originMatrix;
		for (r = 0; r < maxRowCol; r++) {
			for (c = 0; c < maxRowCol; c++) {
				//行列遍历对应相乘 累加给tmp 再赋给 productMatrix
				for (tmp = i = 0; i < maxRowCol; i++)
					tmp += originMatrix[r][i] * tempMatrix[i][c];
				productMatrix[r][c] = tmp;
			}
		}
	}

	// =======查 找

	// 二分查找origin[n]中key 返回key的迭代器, 若没有返回origin+size迭代器 注意:如果有多个相同元素这个方法返回的元素可能是其中任意一个
	// origin迭代器所处的容器必须支持索引[]操作(序列式容器，或者数组) 并且容器内[origin, origin+size)范围内元素必须已排序
	// std有类似二分搜索实现(std::binary_search) 但只是 检查等价于 value 的元素是否出现于范围 [first, last) 中
	// 类似的还有:	A. count,find
	//				B.binary_search, lower_bound, upper_bound, equal_range
	template<class Iterator, class ValueType>
	Iterator dichotomy(Iterator origin, JCE::SizeType size, ValueType key) {
		int left = 0, right = size, mid = 0;
		while (left <= right) {
			mid = (right + left) / 2;
			if (origin[mid] < key) {
				left = mid + 1;
			}//加1之前若确认mid不合法, 于是可以把这个结果用到下一次的计算中, 若不加1下一次可能会重新计算mid
			else if (origin[mid] > key) {
				//同上
				right = mid - 1;
			}
			else {
				return origin + mid;
			}
		}
		return origin + size;
	}
	// 三分法 求一元函数f(x)在区间[left, right]内的的最小解x (eps: 精度, 求最大值可外部实现)
	template<class TY, class TX, class Fun>
	//TY(*fun)(TX x)
	TX trichotomy(TX left, TX right, Fun fun, TX eps) {
		TX Ll, Rr;
		while (right - left > eps) {
			//三分
			Ll = (2 * left + right) / 3;
			Rr = (2 * right + left) / 3;
			if (fun(Ll) > fun(Rr)) {
				left = Ll;
			}
			else {
				right = Rr;
			}
		}
		//返回left ,right任一个即可(PS: 不应返回f(x)的值 因为fun可能不是用户需要的fun 参见顶部注解)
		return left;
	}

	// =======受限的(Confined)

	//返回错排表(malloc) 64 bit 二进制位 最多计算26个 T 需要支持四则运算以及自加
	template<class T>
	StandardExtend::ArrayList<T> calcIllArrangeList(StandardExtend::SizeType MAX_SIZE = 26) {
		StandardExtend::ArrayList<T> M(MAX_SIZE, 0);
		M[0] = 0, M[1] = 1, M[2] = 1;
		for (StandardExtend::SizeType n = 3; n < 26; ++n)
			M[n] = (n - 1) * (M[n - 1] + M[n - 2]);
		return M;
	}
	//输出digit中[leftSub, rightSub)的全排列  非字典序
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
	//输出数字1-maxDigit(1, 9)的全排列  字典序
	void pentration(int maxDigit);
	//输出数字1-maxDigit(1, 9)的全排列  非字典序
	void pentration();
	//汉诺塔递归解法
	void hannoTowerMove(int n, char a, char b, char c);
	//将10进制的number转换为radix进制的字符串 (递归实现)
	std::string decToBin(int number, std::string &result, int radix = 2);
	//若大数bigInteger能被整除返回true
	bool isDivisible(char *bigInteger, int MOD);
	//删除resStr中的所有子串delSubStr
	void eraseAllSubStr(char *resStr, char *delSubStr);
	//逆序数
	int reverseOrderNumber(int number);
	//计算radix进制整数number中digit出现的次数
	int calcDigitCountInNumber(const int number, const int digit, int radix = 10);
	//计算radix进制整数的位数
	int calcDigitTop(int number, int radix = 10);



	I64 quickFact(I64 a, I64 b, I64 mod);
	//快速幂基于quickFact版 效率是简易版的1/5
	I64 quickPow_OLD(I64 C, I64 R, I64 k);
	//快速幂简易版 m^n % k 如果用于计算pow效率是普通版的1/20 (普通版效率高)
	I64 quickPow(I64 m, I64 n, I64 k);
	//快速幂普通版 m^n; 与std::pow效率各有高低相差不大 std::pow值域更大
	I64 quickPow(I64 m, I64 n);
	//任意底数对数
	double logR(double value, double base = 5);
	//阶 乘
	double fact(int n);
	//int 限度内阶乘表 (int 13!爆； double和long long int 18!爆)
	int* factTable(int maxN = 13);
	/*
	n < m && m != 0
		 fact(n)
	 ________________
	fact(n - m) * fact(m)
	*/
	//组合数
	int C(int n, int m);
	//排列数
	int A(int n, int m);
	//素数(质数)判断
	int isPrime_OLD(int num);
	//素数(质数)判断 sqrt的平方优化版(未实测)
	bool isPrime(int num);
	//真因子和(除本身外的约数）
	int factorSum(int x);
	//真因子和表(约数 因数) (10^7一千万2.51s)(400w 10^6 900ms)
	void factorSumTableSieve(const int maxn, int a[]);
	//素数(质数) 筛选法 埃拉托色尼(Sieve Eratosthenes)(0 1==-1, a[i]==0表示素数) maxN[2, 1e9) PS:maxN = 1e7时超过1s了 1e9似乎无法分配内存
	int* primeSieve(const int maxN = 2);
	//因子数目
	int factorCount(int x);
	//开方函数 (eps: 精度)
	double sqrtImpl(double x, double eps = 1e-9);
	//求方差
	double variance(float x[], int n);

}

#endif
