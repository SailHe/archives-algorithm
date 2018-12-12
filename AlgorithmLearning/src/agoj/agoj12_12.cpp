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

#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)

// #include "ExtendSpace.h"

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

// http://139.196.145.92/contest_show.php?cid=689#problem/C
// 最长不下降(<=)子序列
int mainForSolveC() {
	int n, maxLen;
	while (1 == scanf("%d", &n)) {
		ArrayList<int> dataList(n);
		ArrayList<int> lenList(n, 0);
		for (int i = 0; i < n; ++i) {
			scanf("%d", &dataList[i]);
		}
		maxLen = lenList[n - 1] = 1;
		for (int i = n - 2; i > -1; --i) {
			// 选择右侧符合条件(<=)的最优局部长度值中最大的那个作为前驱结点
			int maxLocalRhsLen = 0;
			for (int j = i + 1; j < n; ++j) {
				if (dataList[i] <= dataList[j] && maxLocalRhsLen < lenList[j]) {
					maxLocalRhsLen = lenList[j];
				}
			}
			lenList[i] = maxLocalRhsLen + 1;
			if (lenList[i] > maxLen) {
				maxLen = lenList[i];
			}
		}
		printf("%d\n", maxLen);
	}
	return 0;
}

// 传入前驱列表 迭代路径 visit(int index); 没有前驱的结点 前驱值置为负数
template<typename Fun>
void iteratePath(ArrayList<int> const &precursorList, Fun visit, int origin = 0) {
	for (int i = origin; i >= 0; i = precursorList[i]) {
		visit(i);
	}
}

// 完全是C的加强版 并没有删除代码
int mainForSolveD() {
	int n, maxLen, resOrigin;
	while (1 == scanf("%d", &n)) {
		ArrayList<int> dataList(n);
		ArrayList<int> lenList(n, 0);
		ArrayList<int> precursorList(n, -1);
		for (int i = 0; i < n; ++i) {
			scanf("%d", &dataList[i]);
		}
		maxLen = lenList[n - 1] = 1;
		resOrigin = n - 1;
		for (int i = n - 2; i > -1; --i) {
			// 选择右侧符合条件(<=)的最优局部长度值中最大的那个作为前驱结点
			int maxLocalRhsLen = 0;
			for (int j = i + 1; j < n; ++j) {
				if (dataList[i] <= dataList[j] && maxLocalRhsLen < lenList[j]) {
					maxLocalRhsLen = lenList[j];
					precursorList[i] = j;
				}
			}
			lenList[i] = maxLocalRhsLen + 1;
			if (lenList[i] > maxLen) {
				maxLen = lenList[i];
				resOrigin = i;
			}
		}
		printf("%d\n", maxLen);
		int prCnt = -1;
		iteratePath(precursorList, [&](int index) {
			printf((++prCnt == 0 ? "%d" : " %d"), dataList[index]);
		}, resOrigin);
		printf("\n");
	}
	return 0;
}


int main() {
	//mainForSolveC();
	mainForSolveD();
	return 0;
}
