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

// http://139.196.145.92/contest_show.php?cid=701#problem/A
// 最长不下降(<=)子序列 Onlogn算法
int mainForSolveA() {
	int n, maxLen;
	while (1 == scanf("%d", &n)) {
		ArrayList<int> dataList(n);
		// 上限序列(有序序列)
		ArrayList<int> upLimitList(n, 0);
		for (int i = 0; i < n; ++i) {
			scanf("%d", &dataList[i]);
		}
		auto rhsUpLimitIt = upLimitList.begin() + (n - 1);
		auto lhsUpLimitIt = rhsUpLimitIt;
		*lhsUpLimitIt = dataList[n - 1];
		for (int i = n - 2; i > -1; --i) {
			int currentValue = dataList[i];
			// 使用二分查找将当前结点放入合适的上限列表中
			if (currentValue <= *lhsUpLimitIt) {
				*(--lhsUpLimitIt) = currentValue;
			} else if (currentValue > *rhsUpLimitIt) {
				*rhsUpLimitIt = currentValue;
			} else {
				// 二分查询出上限列表中与当前值相匹配的下限 (参数和返回值都 遵循左闭右开"[)"原则) eg: 1 2 4 4 5 查询4 返回的是2后面的4的迭代器
				auto midUpLimitIt = lower_bound(lhsUpLimitIt + 1, rhsUpLimitIt, currentValue);
				// 只有两个元素的时候肯定'越界' 当然, 如果上述查找时带上已经比较过的左侧和右侧上限的话就可以加上此断言
				// _ASSERT_EXPR(midUpLimitIt != rhsUpLimitIt, "越界!");
				*(midUpLimitIt - 1) = currentValue;
			}
		}
		maxLen = upLimitList.end() - lhsUpLimitIt;
		printf("%d\n", maxLen);
	}
	return 0;
}

int main() {
	mainForSolveA();
	//mainForSolveB();
	//mainForSolveC();
	//mainForSolveD();
	return 0;
}
