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
#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

#define MAX_IN32 2147483647
#define MIN_IN32 (-MAX_IN32-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>
#define FOR(it, lowerBound, terminator) for (auto it = lowerBound; it != terminator; ++it)

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

//#include "ExtendSpace.h"
/*

====== ̰��

7
[1,3]
[1,4]
[2,5]
[3,7]
[4,7]
[6,9]
[7,8]

<<3

7
[1,4]
[1,3]
[2,5]
[4,7]
[3,7]
[6,9]
[7,8]

<<3

7
[1,4]
[1,3]
[2,5]
[4,7]
[3,7]
[6,9]
[1,8]

<<4

PS ����auto
*/
using Item_10_31 = std::pair<int, int>;
int mainForSolve1_10_31() {
	int n;
	while (cin >> n) {
		ArrayList<Item_10_31> dataList(n);
		ArrayList<Item_10_31> machineList(0);
		machineList.reserve(n);
		for (int i = 0; i < n; ++i) {
			scanf(" [%d,%d]", &dataList[i].first, &dataList[i].second);
		}
		//StandardExtend::outPutIterable(dataList.begin(), dataList.end(), 0, 0, [](auto ele) {});
		sort(dataList.begin(), dataList.end(), [](Item_10_31 lhs, Item_10_31 rhs) {
			int result = lhs.first - rhs.first;
			result = result == 0 ? lhs.second - rhs.second : result;
			return result < 0;
		});
		for (int i = 0; i < n; ++i) {
			//���һ����б����Ƿ����һ�����ʵĻ������ŵ�ǰ������
			ArrayList<Item_10_31>::iterator fIt = find_if(machineList.begin(), machineList.end()
				, [&](Item_10_31 ele) {
				//�����Ľ�ֹʱ��λ�ڵ�ǰdata(���)����ʼʱ��֮ǰ����
				return ele.second <= dataList[i].first;
			});
			if (fIt == machineList.end()) {
				machineList.push_back(dataList[i]);
			}
			else {
				fIt->second = dataList[i].second;
			}
		}
		cout << machineList.size() << endl;
	}
	return 0;
}

/*
>>

7 0 10
[4,7]
[2,5]
[1,3]
[7,8]
[3,7]
[1,4]
[6,9]

<<
3
@see http://139.196.145.92/contest_show.php?cid=604#problem/B
����һ��Ļ����� �ڸ�����[tb, te]��Χ�ڵ����������Ч Ȼ�����ڴ˷�Χ������ܼӹ����ٸ����
*/
int mainForSolve2_10_31() {
	int n, tb, te;
	while (cin >> n >> tb >> te) {
		ArrayList<Item_10_31> dataList(0);
		ArrayList<int> countList(0);
		LinkedList<Item_10_31> machineList(0);
		for (int i = 0; i < n; ++i) {
			Item_10_31 temp;
			scanf(" [%d,%d]", &temp.first, &temp.second);
			if (temp.first < tb || temp.second > te) {
				// do nothing
			}
			else {
				dataList.push_back(temp);
			}
		}
		sort(dataList.begin(), dataList.end(), [](Item_10_31 lhs, Item_10_31 rhs) {
			//����ʱ����ʱ��С����ǰ�� ����ʱ�����ʱ��ʼʱ��С����ǰ��
			int result = lhs.second - rhs.second;
			result = result == 0 ? lhs.first - rhs.first : result;
			return result < 0;
		});
		FOR(it, dataList.begin(), dataList.end()) {
			int i = -1;
			//���һ����б����Ƿ����һ�����ʵĻ������ŵ�ǰ������
			LinkedList<Item_10_31>::iterator fIt = find_if(machineList.begin(), machineList.end()
				, [&](Item_10_31 ele) {
				++i;
				//�����Ľ�ֹʱ��λ�ڵ�ǰdata(���)����ʼʱ��֮ǰ����
				return ele.second <= it->first;
			});
			if (fIt == machineList.end()) {
				machineList.push_back(*it);
				countList.push_back(1);
			}
			else {
				++countList[i];
				fIt->second = it->second;
			}
		}
		cout << (countList.empty() ? 0 : countList.front()) << endl;
	}
	return 0;
}

//ͬΪСд��ĸ��ͬΪ���֣��Ұ���ASCII���˳�򣬼����ұߵ��ַ��ϸ������ߵ��ַ�������true
bool isExString(char lhs, char rhs) {
	bool result = (isdigit(lhs) && isdigit(rhs))
		|| (isalpha(lhs) && isalpha(rhs) && islower(lhs) && islower(rhs));
	return result && (rhs - lhs > 0);
}

string calcInnerString(char lhs, char rhs) {
	string result = "";
	while (++lhs < rhs) {
		result += lhs;
	}
	return result;
}

/*
>>
abcs-w-y1234-9s-4zz

<<
abcstuvwxy123456789s-4zz
*/
int main() {
	//mainForSolve1_10_31();
	//mainForSolve2_10_31();
	string originStr;
	char flag = '-';
	while (cin >> originStr) {
		//originStr.insert(originStr.begin(), {'a', 'b'});
		//auto index = originStr.find_first_of("-");
		//auto index = originStr.find("");         
		//find_if(originStr.begin(), originStr.end(), [&](char ele) {});
		SizeType position = 0;
		while ((position = originStr.find_first_of(flag, position)) != string::npos){
			if (position == 0 || position + 1 == originStr.length()) {
				//do nothing
			}
			else {
				if (isExString(originStr[position - 1], originStr[position + 1])) {
					originStr.replace(
						originStr.begin() + position, originStr.begin() + position + 1
						, calcInnerString(originStr[position - 1], originStr[position + 1]));
				}
				else {
					// donothing
				}
			}
			++position;
		}
		cout << originStr << endl;
	}
	return 0;
}
