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

// 计算并返回最大子列和 (朴素版: T等价于Iter迭代器中的元素类型; MIN_VALUE保证比该段序列的最小值还小)
// 当遇到0时会计入子列中
template<typename Iter, typename T>
T maximumSubsectionSum(Iter left, Iter right, T MIN_VALUE) {
	int fromSub = 1, toSub = 1;
	// 之前的最大子列和; 当前的最大子列和; 总的最大子列和
	T previousMaxSum = 0, currentMaxSum = 0, maxSum = MIN_VALUE;
	for (auto it = left; it != right; ++it) {
		currentMaxSum = *it;
		// 若之前的最大子列和已经小于0了 那么说明其无法优化后续的子列和->舍弃之前的子列和
		if (previousMaxSum < 0) {
			previousMaxSum = currentMaxSum;
		}
		else {
			previousMaxSum += currentMaxSum;
		}

		if (previousMaxSum > maxSum) {
			maxSum = previousMaxSum;
		}
	}
	return maxSum;
}

/*
约定: 
before-now(时间上的先后);
previous-current(位置上的先后);
older-newer(内容,数据上的先后);

start(ed)-stop-finish(ed)(开始, 停止中止, 完成: 时间上的起始);
origin-target(动身,目标: 空间位置上的起始);
begin-end(首先, 尽头: 内容上的起始);
left-right(左, 右: 迭代器的起始 并不一定在位置上分别位于左和右;  也可以指下标; 或是一个左闭右开的范围保证!=号 left可以通过++使得left==right);
from-to(来自, 到: 下标位置的起始 from <= to 或是用于 已有left和right的地方);
lhs-rhs(左手边, 右手边: 泛指具有两个操作数的方法的左侧和右侧的两个操作数);

ultimately: 最后, 根本
final: 最终的, 决定性的
sub: 替补者, 下标

local: 局部的
global: 全局的

part: 部分(的)
whole: 整体(的)

result: 结果
answer: 回答
*/

//局部和
struct SubSum{
	// 值
	int value;
	//起始位置
	int fromSub;
	//终止位置
	int toSub;
};
void maximumSubsectionSum(ArrayList<int> const &arrSeq, int &maxSumOut, int &fromOut, int &toOut){
	int ca = 1, t, n;
	//读入目前数据前 总数据的最大和
	SubSum previousMaxSubSum;
	//读入数据的最大和
	SubSum currentMaxSubSum;
	//整体最大和
	SubSum resultMaxSubSum;
	auto it = arrSeq.begin();
	if (it == arrSeq.end()) {
		return;
	}
	n = (int)arrSeq.size();
	currentMaxSubSum.value = *it;
	++it;
	for (currentMaxSubSum.fromSub = currentMaxSubSum.toSub = 1, resultMaxSubSum = previousMaxSubSum = currentMaxSubSum; currentMaxSubSum.toSub < n; ){
		currentMaxSubSum.value = *it;
		++it;

		//currentMaxSubSum的起始位置是一样的
		currentMaxSubSum.fromSub = ++currentMaxSubSum.toSub;
		//更新局部最优解
		// currentMaxSubSum.value + previousMaxSubSum.value < currentMaxSubSum.value <==> previousMaxSubSum.value < 0
		// 前者可以理解为 横跨 之前的子列 和 当前的子列的子列 无法使比当前的子列更优
		// 但这tm不是徒增脑活动量嘛
		if (previousMaxSubSum.value < 0) {
			//当前数据局部最优
			previousMaxSubSum = currentMaxSubSum;
		}
		else {
			//之前数据局部最优
			previousMaxSubSum.value += currentMaxSubSum.value;
		}

		if (resultMaxSubSum.value < previousMaxSubSum.value) {
			//更新整体最优解
			resultMaxSubSum = previousMaxSubSum;
			//resultMaxSubSum 的终止位置要变化
			resultMaxSubSum.toSub = currentMaxSubSum.toSub;
		}
	}
	maxSumOut = resultMaxSubSum.value;
	fromOut = resultMaxSubSum.fromSub;
	toOut = resultMaxSubSum.toSub;
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
