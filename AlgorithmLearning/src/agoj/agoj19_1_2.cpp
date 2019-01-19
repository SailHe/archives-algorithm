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

// #include "else\DisjointSet.h"
// #include "ExtendSpace.h"
#include "./else/BigInteger.h"

#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using Sub = int;
using I64 = long long;

// 方向枚举
enum DIRECTION_ENUM {
	UN_KNOEN_DIR, UP_DIR, DOWM_DIR, LEFT_DIR, RIGHT_DIR, LEFT_UP_DIR, RIGHT_UP_DIR
};

// 数据库操作枚举
enum OPTION_CRUD_ENUM {
	// 创建, 检索, 更新, 删除, 未定义
	// insert/save, query/find/search, edit/change, remove/erase
	CREATE_OPT, RETRIEVE_OPT, UPDATE_OPT, DELETE_OPT, UN_DEFINDE_OPT
};

// // 数据操作枚举
// enum OPTION_DATA_ENUM {
// 	// 保持不变, 替换, 删除, 插入, 未定义 (优先级按先后排列 以前面的越优)
// 	KEEP_OPT, REPLACE_OPT, DELETE_OPT, INSERT_OPT, UN_DEFINDE_OPT
// };

// 任意进制之间互相转换(使用了二进制转换器类: BinaryTransition) 基于10进制
// @see http://139.196.145.92/contest_show.php?cid=720#problem/B
/*
input
2 9
1100

16 7
4CD

10 36
46

output:
13
3404
1A
*/
int mainForSolveB() {
	int originBase, targetBase;
	string origin, target;
	origin.resize(20);
	target.resize(20);
	vector<int> digitTopLow;
	digitTopLow.resize(20);
	while (cin >> originBase >> targetBase) {
		BinaryTransition bt(20, originBase, targetBase);
		// scanf("%s", &origin[0]);
		cin >> origin;
		// bt.transition(&origin[0], digitTopLow);
		// 这个API也太难用了吧 30min
		int totalBit = TransitionUtility::charContainerToDigitContainer(&origin[0], digitTopLow);
		int tenValue = TransitionUtility::radixTopLowToDecimal(digitTopLow.begin(), originBase, totalBit);
		reverse(digitTopLow.begin(), digitTopLow.end());
		totalBit = TransitionUtility::decimalToRadixLowTopBase(tenValue, digitTopLow.begin(), targetBase);
		TransitionUtility::outputDigitArrayLowTop(digitTopLow, totalBit);
		// for (int i = totalBit - 1; i >= 0; --i) {
		// 	printf("%d", digitTopLow[i]);
		// }
		//cout << endl;
	}
	return 0;
}

// 10 -> radix
int mainForSolveC() {
	int originValue, targetBase;
	string origin, target;
	origin.resize(20);
	target.resize(20);
	vector<int> digitTopLow;
	digitTopLow.resize(20);
	while (cin >> originValue >> targetBase) {
		int tenValue = originValue;
		int totalBit = TransitionUtility::decimalToRadixLowTopBase(tenValue, digitTopLow.begin(), targetBase);
		TransitionUtility::outputDigitArrayLowTop(digitTopLow, totalBit);
	}
	return 0;
}

// 丧心病狂 radix=[2, 62] char={[0,9], [A, Z], [a, z]}
int mainForSolveD() {
	int originBase, targetBase;
	string origin, target;
	origin.resize(20);
	target.resize(20);
	vector<int> digitTopLow, digitLowTop;
	digitTopLow.resize(20);
	digitLowTop.resize(20);
	while (cin >> originBase >> targetBase) {
		BinaryTransition bt(1000+5);// , originBase, targetBase
		scanf("%s", &origin[0]);
		bt.transition(&origin[0], digitTopLow);
		int tv = TransitionUtility::radixTopLowToDecimal(digitTopLow.begin(), 2, digitTopLow.size());
		printf("10->%d\n", tv);
		TransitionUtility::decimalToRadixLowTopBase(tv, digitTopLow.begin(), targetBase);
		reverse(digitLowTop.begin(), digitLowTop.end());
		TransitionUtility::outputDigitArrayLowTop(digitLowTop, digitLowTop.size());
		// // 这个API也太难用了吧 30min
		// int totalBit = BinaryTransition::charContainerToDigitContainer(&origin[0], digitTopLow);
		// int tenValue = BinaryTransition::baseTopLowToTen(digitTopLow.begin(), originBase, totalBit);
		// reverse(digitTopLow.begin(), digitTopLow.end());
		// totalBit = BinaryTransition::tenToRadix(tenValue, digitTopLow.begin(), targetBase);
		// BinaryTransition::outputDigitArrayLowTop(digitTopLow, totalBit);
	}
	return 0;
}

int mainForSolveA() {
	int caseCnt;
	scanf("%d", &caseCnt);
	while (caseCnt-- > 0) {
		string lhs, rhs, equalitySeq;
		cin >> lhs >> rhs;
		lhs = " " + lhs;
		rhs = " " + rhs;
		ArrayList<ArrayList<int>> dis2d(lhs.length(), ArrayList<int>(rhs.length(), 0));
		ArrayList<ArrayList<DIRECTION_ENUM>> path2d(lhs.length(), ArrayList<DIRECTION_ENUM>(rhs.length(), UN_KNOEN_DIR));
		// 第0行和第0列需要初始化为0
		for (SizeType r = 1; r < lhs.length(); ++r) {

		}

		// 行与lhs相关 列与rhs相关
		for (SizeType r = 1; r < lhs.length(); ++r) {
			for (SizeType c = 1; c < rhs.length(); ++c) {
				if (lhs[r] == rhs[c]) {
					//equalitySeq += lhs;
					dis2d[r][c] = dis2d[r - 1][c - 1] + 1;
					path2d[r][c] = LEFT_UP_DIR;
				}
				else {
					if (dis2d[r - 1][c] >= dis2d[r][c - 1]) {
						dis2d[r][c] = dis2d[r - 1][c];
						path2d[r][c] = UP_DIR;
					}
					else {
						dis2d[r][c] = dis2d[r][c - 1];
						path2d[r][c] = LEFT_DIR;
					}
				}
			}
		}
		cout << equalitySeq.length() << endl;
	}
	return 0;
}


// itoa
int main() {
	mainForSolveB();
	//mainForSolveC();
	//mainForSolveD();
	//mainForSolveA();
	return 0;
}
