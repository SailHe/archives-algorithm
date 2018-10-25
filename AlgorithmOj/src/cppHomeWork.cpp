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

//将摄氏温度转换为华氏温度
double toFahrenheit(double tCelsius) {
	return (9 / 5.0) * tCelsius + 32;
}

int mainForTemperatureTranslate() {
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

int mainForAlphTranslate() {
	string upperCase;
	while (cin >> upperCase) {
		transform(upperCase.begin(), upperCase.end(), upperCase.begin(), toUpper);
		cout << upperCase << endl;
	}
	return 0;
}

int main() {
	mainForTemperatureTranslate();
	mainForAlphTranslate();
	return 0;
}

