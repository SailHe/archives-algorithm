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

// 计算并返回最大子列和 (简易版: T等价于Iter迭代器中的元素类型; MIN_VALUE保证比该段序列的最小值还小)
template<typename Iter, typename T>
T maximumSubsectionSum(Iter left, Iter right, T MIN_VALUE) {
	int fromSub = 1, toSub = 1;
	T previousMaxSum = 0, currentMaxSum = 0, maxSum = MIN_VALUE;
	for (auto it = left; it != right; ++it) {
		if (currentMaxSum < 0) {
			currentMaxSum = *it;
		}
		else {
			currentMaxSum += *it;
		}

		if (currentMaxSum > maxSum) {
			maxSum = currentMaxSum;
		}
	}
	return maxSum;
}

//局部和
struct SubSum{
	// 值
	int value;
	//起始位置 start-stop; begin-end; origin-target
	int startSub;
	//终止位置
	int stopSub;
};
void maximumSubsectionSum(ArrayList<int> const &arrSeq, int &maxSumOut, int &fromOut, int &toOut){
	int ca = 1, t, n;
	//读入目前数据前 总数据的最大和 before-now; previous-current; older-newer
	SubSum before;
	//读入数据的最大和
	SubSum now;
	//整体最大和
	SubSum max;
	auto it = arrSeq.begin();
	if (it == arrSeq.end()) {
		return;
	}
	n = (int)arrSeq.size();
	now.value = *it;
	++it;
	for (now.startSub = now.stopSub = 1, max = before = now; now.stopSub < n; ){
		now.value = *it;
		++it;

		//now的起始位置是一样的
		now.startSub = ++now.stopSub;
		//更新局部最优解
		if (now.value + before.value < now.value) {
			//当前数据局部最优
			before = now;
		}
		else {
			//之前数据局部最优
			before.value += now.value;
		}

		if (max.value < before.value) {
			//更新整体最优解
			max = before;
			//max的末 位置要变化
			max.stopSub = now.stopSub;
		}
	}
	maxSumOut = max.value;
	fromOut = max.startSub;
	toOut = max.stopSub;
}

//	字符统计问题一 O(N)
int mainForSolveD() {
	int n;
	string str;
	while (1 == scanf("%d", &n)) {
		cin >> str;

		int fromSub = 1, toSub = 1, len = 1, maxLen = MIN_INT32;
		for (size_t i = 0; i < str.size(); ++i) {
			len = 1;
			
		}
		printf("From=%d,To=%d\n", fromSub + 1, toSub + 1);
		printf("MaxLen=%d\n", maxLen);
	}
	return 0;
}

// 最大字段和DP O(N)
int mainForSolveA() {
	int nCase, n;
	ArrayList<int> arrSeq;
	scanf("%d", &nCase);
	// while (1 == scanf("%d", &nCase)) 
	{
		for (int iCase = 0; iCase < nCase; ++iCase) {
			scanf("%d", &n);
			arrSeq.clear();
			arrSeq.reserve(n);
			for (int i = 0; i < n; ++i) {
				int temp;
				scanf("%d", &temp);
				arrSeq.push_back(temp);
			}
			int maxSum, fromSub, toSub;
			// maxSum = maximumSubsectionSum(arrSeq.begin(), arrSeq.end(), MIN_INT32);
			maximumSubsectionSum(arrSeq, maxSum, fromSub, toSub);
			// printf("From=%d,To=%d\n", fromSub + 1, toSub + 1);
			// printf("MaxLen=%d\n", maxSum);
			printf("%d\n", maxSum);
		}
	}
	return 0;
}

int main() {
	mainForSolveA();
	return 0;
}
