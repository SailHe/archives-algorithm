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
#include<iomanip>//cout各种操纵器
#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

//#define MAX_N 1000+5
#define MAX_INT_NUM 2147483647
#define MIN_INT_NUM (-MAX_INT_NUM-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>
using namespace std;

// 字符串大数加法
string bigPlush(string &topLowNumA, string &topLowNumB, string &topLowSum) {
	size_t lenA = topLowNumA.size(), lenB = topLowNumB.size(), lenAB;
	//补0用
	string temp;
	//低位在右, 短者高位0补齐
	if (lenA > lenB) {
		temp.resize(lenA - lenB, '0');
		topLowNumB = temp + topLowNumB;
		lenAB = lenA;
	}
	else {
		temp.resize(lenB - lenA, '0');
		topLowNumA = temp + topLowNumA;
		lenAB = lenB;
	}
	if (topLowSum.size() < lenA) {
		topLowSum.resize(lenA, '0');
	}
	int ia = -1, c = 0;
	//反转后左边是低位
	for (int i = lenAB - 1; i >= 0; --i) {
		int sumBit = (topLowNumA[i] - '0') + (topLowNumB[i] - '0') + c;
		topLowSum[i] = sumBit % 10+ '0';
		c = sumBit / 10;
	}
	return c == 0 ? topLowSum : (topLowSum = "1" + topLowSum);
}

//去除前导0 若值为0 返回"0"
string formatString(string const &num) {
	int sp = -1;
	for (auto it = num.begin(); it != num.end(); ++it) {
		++sp;
		if (*it != '0') {
			break;
		}
	}
	return sp == num.size() ? "0" : num.substr(sp);
}

int main__() {
	string a, b;
	//freopen("input", "r", stdin);
	while (cin >> a >> b) {
		bigPlush(a, b, a);
		cout << formatString(a) << endl;
	}
	return 0;
}

//将摄氏温度转换为华氏温度
double toFahrenheit(double tCelsius) {
	return (9 / 5.0) * tCelsius + 32;
}

int main__65() {
	double t;
	while (cin>>t) {
		cout << toFahrenheit(t) << endl;
	}
	return 0;
}

bool islower(char c) {
	return 'a' <= c && c <= 'z';
}

char toUpper(char c) {
	//::toupper(c);
	return islower(c) ? (c - 'a') + 'A' : c;
}

int main() {
	string upperCase;
	while (cin >> upperCase) {
		transform(upperCase.begin(), upperCase.end(), upperCase.begin(), toUpper);
		cout << upperCase << endl;
	}
	return 0;
}

