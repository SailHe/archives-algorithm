#include<stdio.h>
#include<string.h>
#include<string>
#include<cmath>
#include<vector>
#include<iostream>
#include<queue>
#include<functional>//greater
#include<list>
#include<stack>
#include<map>
#include<iomanip>//cout���ֲ�����
#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

//#define MAX_N 1000+5
#define MAX_INT_NUM 2147483647
#define MIN_INT_NUM (-MAX_INT_NUM-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>
using namespace std;

template<class T> using ArrayList = vector<T>;
using SizeType = size_t;
using I64 = long long;

int sum, leftSum, rightSum, s1, lefts;
int center;
int maxSum(ArrayList<int> &eleList, int left, int right) {
	sum = 0;
	// ���г���Ϊһʱ -> ֱ�����
	if (left == right) {
		sum = (eleList[left] > 0) ? eleList[left] : 0;
	}
	else {
		//����
		center = (left + right) / 2;
		// 1.���ݹ����
		leftSum = maxSum(eleList, left, center);
		// 2.�Ҳ�ݹ����
		rightSum = maxSum(eleList, center + 1, right);
	}
	s1 = lefts = 0;
	for (int i = center; i >= left; --i) {

	}
}

int main() {
	int n;
	ArrayList<int> eleList;
	while (1 == scanf("%d", &n)) {

	}
	return 0;
}

