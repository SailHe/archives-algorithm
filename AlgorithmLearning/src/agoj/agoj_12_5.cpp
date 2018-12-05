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
// ������0ʱ�����������
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
struct SubSum{
	// ֵ
	int value;
	//��ʼλ��
	int fromSub;
	//��ֹλ��
	int toSub;
};
void maximumSubsectionSum(ArrayList<int> const &arrSeq, int &maxSumOut, int &fromOut, int &toOut){
	int ca = 1, t, n;
	//����Ŀǰ����ǰ �����ݵ�����
	SubSum previousMaxSubSum;
	//�������ݵ�����
	SubSum currentMaxSubSum;
	//��������
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

		//currentMaxSubSum����ʼλ����һ����
		currentMaxSubSum.fromSub = ++currentMaxSubSum.toSub;
		//���¾ֲ����Ž�
		// currentMaxSubSum.value + previousMaxSubSum.value < currentMaxSubSum.value <==> previousMaxSubSum.value < 0
		// ǰ�߿������Ϊ ��� ֮ǰ������ �� ��ǰ�����е����� �޷�ʹ�ȵ�ǰ�����и���
		// ����tm����ͽ���Ի����
		if (previousMaxSubSum.value < 0) {
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

//	�ַ�ͳ������һ O(N)
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

// ����ֶκ�DP O(N)
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
