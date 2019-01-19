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
#include<iomanip>//cout���ֲ�����

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

// ����ö��
enum DIRECTION_ENUM {
	UN_KNOEN_DIR, UP_DIR, DOWM_DIR, LEFT_DIR, RIGHT_DIR, LEFT_UP_DIR, RIGHT_UP_DIR
};

// ���ݿ����ö��
enum OPTION_CRUD_ENUM {
	// ����, ����, ����, ɾ��, δ����
	// insert/save, query/find/search, edit/change, remove/erase
	CREATE_OPT, RETRIEVE_OPT, UPDATE_OPT, DELETE_OPT, UN_DEFINDE_OPT
};

// // ���ݲ���ö��
// enum OPTION_DATA_ENUM {
// 	// ���ֲ���, �滻, ɾ��, ����, δ���� (���ȼ����Ⱥ����� ��ǰ���Խ��)
// 	KEEP_OPT, REPLACE_OPT, DELETE_OPT, INSERT_OPT, UN_DEFINDE_OPT
// };

// �������֮�以��ת��(ʹ���˶�����ת������: BinaryTransition) ����10����
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
		// ���APIҲ̫�����˰� 30min
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

// ɥ�Ĳ��� radix=[2, 62] char={[0,9], [A, Z], [a, z]}
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
		// // ���APIҲ̫�����˰� 30min
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
		// ��0�к͵�0����Ҫ��ʼ��Ϊ0
		for (SizeType r = 1; r < lhs.length(); ++r) {

		}

		// ����lhs��� ����rhs���
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
