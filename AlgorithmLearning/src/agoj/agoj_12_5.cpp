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

// ���㲢����������к� (���ذ�: T�ȼ���Iter�������е�Ԫ������; MIN_VALUE��֤�ȸö����е���Сֵ��С)
// ������0ʱ����������� �������б�������������
template<typename Iter, typename T>
T maximumSubsectionSum(Iter left, Iter right, T MIN_VALUE) {
	int fromSub = 1, toSub = 1;
	// ֮ǰ��������к�; ��ǰ��������к�; �ܵ�������к�
	T previousMaxSum = 0, currentMaxSum = 0, maxSum = MIN_VALUE;
	for (auto it = left; it != right; ++it) {
		currentMaxSum = *it;
		// ��֮ǰ��������к��Ѿ�С��0�� ��ô˵�����޷��Ż����������к�->����֮ǰ�����к�
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
Լ��: 
before-now(ʱ���ϵ��Ⱥ�);
previous-current(λ���ϵ��Ⱥ�);
older-newer(����,�����ϵ��Ⱥ�);

start(ed)-stop-finish(ed)(��ʼ, ֹͣ��ֹ, ���: ʱ���ϵ���ʼ);
origin-target(����,Ŀ��: �ռ�λ���ϵ���ʼ);
begin-end(����, ��ͷ: �����ϵ���ʼ);
left-right(��, ��: ����������ʼ ����һ����λ���Ϸֱ�λ�������;  Ҳ����ָ�±�; ����һ������ҿ��ķ�Χ��֤!=�� left����ͨ��++ʹ��left==right);
from-to(����, ��: �±�λ�õ���ʼ from <= to �������� ����left��right�ĵط�);
lhs-rhs(���ֱ�, ���ֱ�: ��ָ���������������ķ����������Ҳ������������);

ultimately: ���, ����
final: ���յ�, �����Ե�
sub: �油��, �±�

local: �ֲ���
global: ȫ�ֵ�

part: ����(��)
whole: ����(��)

result: ���
answer: �ش�
*/

//�ֲ���
template<typename T>
struct SubSum{
	// ֵ
	T value;
	//��ʼλ��
	int fromSub;
	//��ֹλ��
	int toSub;
};

template<typename T>
void maximumSubsectionSum(ArrayList<T> const &arrSeq, T &maxSumOut, int &fromOut, int &toOut){
	//����Ŀǰ����ǰ �����ݵ�����
	SubSum<T> previousMaxSubSum;
	//�������ݵ�����
	SubSum<T> currentMaxSubSum;
	//��������
	SubSum<T> resultMaxSubSum;
	auto left = arrSeq.begin(), right = arrSeq.end();
	currentMaxSubSum.value = 0;
	currentMaxSubSum.fromSub = currentMaxSubSum.toSub = 0;
	for (resultMaxSubSum = previousMaxSubSum = currentMaxSubSum; left != right; ++left){
		currentMaxSubSum.value = *left;

		//currentMaxSubSum����ʼλ����һ����
		currentMaxSubSum.fromSub = ++currentMaxSubSum.toSub;
		//���¾ֲ����Ž�
		// currentMaxSubSum.value + previousMaxSubSum.value < currentMaxSubSum.value <==> previousMaxSubSum.value < 0
		// ǰ�߿������Ϊ ��� ֮ǰ������ �� ��ǰ�����е����� �޷�ʹ�ȵ�ǰ�����и���
		// ����tm����ͽ���Ի����
		// if (previousMaxSubSum.value < 0) {
		if (currentMaxSubSum.value + previousMaxSubSum.value < currentMaxSubSum.value) {
			//��ǰ���ݾֲ�����
			previousMaxSubSum = currentMaxSubSum;
		}
		else {
			//֮ǰ���ݾֲ�����
			previousMaxSubSum.value += currentMaxSubSum.value;
		}

		if (resultMaxSubSum.value < previousMaxSubSum.value) {
			//�����������Ž�
			resultMaxSubSum = previousMaxSubSum;
			//resultMaxSubSum ����ֹλ��Ҫ�仯
			resultMaxSubSum.toSub = currentMaxSubSum.toSub;
		}
	}
	maxSumOut = resultMaxSubSum.value;
	fromOut = resultMaxSubSum.fromSub;
	toOut = resultMaxSubSum.toSub;
}

void maximumSubsectionLength(ArrayList<int> const &arrSeq, int &maxSumOut, int &fromOut, int &toOut) {
	//����Ŀǰ����ǰ �����ݵ�����
	SubSum<int> previousMaxSubSum;
	//�������ݵ�����
	SubSum<int> currentMaxSubSum;
	//��������
	SubSum<int> resultMaxSubSum;
	auto left = arrSeq.begin(), right = arrSeq.end();
	auto previousElement = MIN_INT32;
	currentMaxSubSum.value = 0;

	int currentLen = 0, maxLen = MIN_INT32;

	currentMaxSubSum.fromSub = currentMaxSubSum.toSub = 0;
	for (resultMaxSubSum = previousMaxSubSum = currentMaxSubSum; left != right; ++left) {
		currentMaxSubSum.value = *left;

		currentMaxSubSum.fromSub = ++currentMaxSubSum.toSub;
		if (previousMaxSubSum.value != currentMaxSubSum.value) {
			previousMaxSubSum = currentMaxSubSum;
			currentLen = 1;
		}
		else {
			++currentLen;
		}

		if (currentLen > maxLen) {
			maxLen = currentLen;
			resultMaxSubSum = previousMaxSubSum;
			resultMaxSubSum.toSub = currentMaxSubSum.toSub;
		}
	}
	// maxSumOut = resultMaxSubSum.value;
	maxSumOut = maxLen;
	fromOut = resultMaxSubSum.fromSub;
	toOut = resultMaxSubSum.toSub;
}


//	�ַ�ͳ������һ O(N)
// @see http://139.196.145.92/contest_show.php?cid=667#problem/D
int mainForSolveD() {
	string str;
	while (cin >> str) {
		ArrayList<int> arrSeq(str.begin(), str.end());
		//ArrayList<string> arrSeqS(str.begin(), str.end());
		int maxSum, fromSub, toSub;
		// string maxSumStr;
		maximumSubsectionLength(arrSeq, maxSum, fromSub, toSub);
		// maximumSubsectionSum(arrSeqS, maxSumStr, fromSub, toSub);
		printf("From=%d,To=%d\n", fromSub, toSub);
		printf("MaxLen=%d\n", maxSum);
	}
	return 0;
}

// ����Ӷκ�DP O(N)
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
	// mainForSolveA();
	mainForSolveD();
	return 0;
}
