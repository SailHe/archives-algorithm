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

// 传入前驱列表 迭代路径 visit(int index); (PS: 没有前驱的结点 前驱值需置为负数)
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

// 数塔问题 与9-22 的问题类似, 只不过此处是求最小值 结果存储于array2D[0][0]
void proForTrangleMinValue(ArrayList<ArrayList<int>> &array2D, int rows) {
	int r = rows - 2;
	int pastPathSub = -1;
	for (; r >= 0; --r) {
		for (int c = 0; c < r + 1; ++c) {
			int add = 0;
			if (array2D[r + 1][c] > array2D[r + 1][c + 1]) {
				add = array2D[r + 1][c + 1];
				pastPathSub = c + 1;
			}
			else {
				//下一行的第c列和第c+1列相等时 选取序号小的那一列: c
				add = array2D[r + 1][c];
				pastPathSub = c;
			}
			array2D[r][c] += add;
		}
	}
}

int mainForSolveB() {
	int rows, caseCnt;
	scanf("%d", &caseCnt);
	while (caseCnt-- > 0) {
		scanf("%d", &rows);
		ArrayList<ArrayList<int>> array2D(rows);
		for (int r = 0; r < rows; ++r) {
			array2D[r].resize(r + 1);
			for (int c = 0; c < r + 1; ++c) {
				scanf("%d", &array2D[r][c]);
				array2D[r][c] = array2D[r][c];
			}
		}
		proForTrangleMinValue(array2D, rows);
		printf("%d\n", array2D[0][0]);
	}
	return 0;
}

int main() {
	//mainForSolveC();
	//mainForSolveD();
	mainForSolveB();
	return 0;
}
