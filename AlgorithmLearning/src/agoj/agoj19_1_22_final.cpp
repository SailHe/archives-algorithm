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
#include<iomanip>
#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)
using namespace std;
//template<class T> using ArrayList = vector<T>;
//template<class T> using LinkedList = list<T>;
//using SizeType = size_t; using I64 = long long;

// dec -> radix
template<class DigitIterator>
DigitIterator decToRadixTopLow(unsigned decNum, DigitIterator rightIter, int radix) {
	do {
		*--rightIter = decNum % radix;
		//cout<<*rightIter<<endl;
		decNum /= radix;
	} while (decNum != 0);
	return rightIter;
}
// radix -> dec
template<class DigitIterator>
unsigned radixTopLowToDec(DigitIterator leftIter, DigitIterator rightIter, int radix) {
	unsigned decNum = 0;
	unsigned powNum = 1;
	while (leftIter != rightIter) {
		--rightIter;
		//cout<<"powNum="<<powNum<<endl;
		decNum += *rightIter * powNum;
		powNum *= radix;
	}
	return decNum;
}
// ¾µÏñ
void matrixMirroring(std::vector<vector<int>> &matrixBin, int order) {
	switch (order) {
		// left right
	case 2: {
		std::for_each(matrixBin.begin(), matrixBin.end(), [](vector<int> &binTopLow) {
			std::reverse(binTopLow.begin(), binTopLow.end());
		});
		break; }
			// up down
	case 3: {
		std::reverse(matrixBin.begin(), matrixBin.end());
		break; }
	default: {break; }
	}
}

// fill matrixBin col with vector; PS: vector not in matrixBin
void colFill(std::vector<int> &arrayList, std::vector<vector<int>> &matrixBin, int col) {
	for (int i = 0; i < matrixBin.size(); ++i) {
		if (col >= matrixBin[i].size()) {
			while (true);
		}
		matrixBin[i][col] = arrayList[i];
	}
}

void rowFill(std::vector<int> &arrayList, std::vector<vector<int>> &matrixBin, int row) {
	std::copy(arrayList.begin(), arrayList.end(), matrixBin[row].begin());
}

// Ë³Ê±ÕëÐý×ª|->-
void matrixRotationClockwise(std::vector<vector<int>> &matrixBin, int order) {
	switch (order) {
		// 90
	case 1: {
		auto buffer = matrixBin;
		for (int i = 0; i < buffer.size(); ++i) {
			colFill(buffer[i], matrixBin, matrixBin.size() - i - 1);
		}
		break; }
			// 180
	case 2: {
		matrixRotationClockwise(matrixBin, 1);
		matrixRotationClockwise(matrixBin, 1);
		break; }
			// 270
	case 3: {
		matrixRotationClockwise(matrixBin, 2);
		matrixRotationClockwise(matrixBin, 1);
		break; }
	default: {break; }
	}
}

template<class Iterator>
void outPut(Iterator left, Iterator right) {
	while (left != right) {
		cout << *left++ << " ";
	}
	cout << endl;
}

/*
31 3 3
2147483647 2147483647 2147483647 2147483647 2147483647
2147483647 2147483647 2147483647 2147483647 2147483647
2147483647 2147483647 2147483647 2147483647 2147483647
2147483647 2147483647 2147483647 2147483647 2147483647
2147483647 2147483647 2147483647 2147483647 2147483647
2147483647 2147483647 2147483647 2147483647 2147483647
2147483647
*/
int main() {
	int n, a, b;
	std::vector<int> decList;
	std::vector<int> topLowList;
	std::vector<vector<int>> matrixBin;

	while (cin >> n >> a >> b) {
		bool reee = (1 <= n && n <= 31 && 1 <= a && a <= 3 && 1 <= b && b <= 3);
		while (!reee);
		decList.resize(n);
		matrixBin.resize(n);
		topLowList.resize(n);
		for (int i = 0; i < n; ++i) {
			cin >> decList[i];
			//reee = (0 <= decList[i] && decList[i] <= pow(n-1, 2.0));
			//cout<<!reee<<endl;
			//while(!reee);
			auto x = pow(2.0, n - 1);
			_ASSERT(0 <= decList[i] && decList[i] <= x);
			matrixBin[i].resize(n);
			auto leftIter = decToRadixTopLow(decList[i], topLowList.end(), 2);
			std::fill(topLowList.begin(), leftIter, 0);
			// debug
			//outPut(topLowList.begin(), topLowList.end());
			std::copy(topLowList.begin(), topLowList.end(), matrixBin[i].begin());
			//std::cout<<radixTopLowToDec(matrixBin[i].begin(), matrixBin[i].end(), 2)<<endl;
		}
		matrixMirroring(matrixBin, a);
		matrixRotationClockwise(matrixBin, b);
		// debug
		/*cout<<endl;
		for(int i = 0; i < n; ++i){
			outPut(matrixBin[i].begin(), matrixBin[i].end());
		}*/
		for (size_t i = 0; i < matrixBin.size(); ++i) {
			cout << (i == 0 ? "" : " ");
			unsigned result = radixTopLowToDec(matrixBin[i].begin(), matrixBin[i].end(), 2);
			cout << result;
		}
		cout << endl;
	}
	return 0;
}
