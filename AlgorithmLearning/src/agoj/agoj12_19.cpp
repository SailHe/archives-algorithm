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

#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)

// #include "ExtendSpace.h"

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

// http://139.196.145.92/contest_show.php?cid=701#problem/A
// ����½�(<=)������ Onlogn�㷨
int mainForSolveA() {
	int n, maxLen;
	while (1 == scanf("%d", &n)) {
		ArrayList<int> dataList(n);
		// ��������(��������)
		ArrayList<int> upLimitList(n, 0);
		for (int i = 0; i < n; ++i) {
			scanf("%d", &dataList[i]);
		}
		auto rhsUpLimitIt = upLimitList.begin() + (n - 1);
		auto lhsUpLimitIt = rhsUpLimitIt;
		*lhsUpLimitIt = dataList[n - 1];
		for (int i = n - 2; i > -1; --i) {
			int currentValue = dataList[i];
			// ʹ�ö��ֲ��ҽ���ǰ��������ʵ������б���
			if (currentValue <= *lhsUpLimitIt) {
				*(--lhsUpLimitIt) = currentValue;
			} else if (currentValue > *rhsUpLimitIt) {
				*rhsUpLimitIt = currentValue;
			} else {
				// ���ֲ�ѯ�������б����뵱ǰֵ��ƥ������� (�����ͷ���ֵ�� ��ѭ����ҿ�"[)"ԭ��) eg: 1 2 4 4 5 ��ѯ4 ���ص���2�����4�ĵ�����
				auto midUpLimitIt = lower_bound(lhsUpLimitIt + 1, rhsUpLimitIt, currentValue);
				// ֻ������Ԫ�ص�ʱ��϶�'Խ��' ��Ȼ, �����������ʱ�����Ѿ��ȽϹ��������Ҳ����޵Ļ��Ϳ��Լ��ϴ˶���
				// _ASSERT_EXPR(midUpLimitIt != rhsUpLimitIt, "Խ��!");
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
