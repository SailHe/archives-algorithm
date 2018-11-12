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

#include "ExtendSpace.h"

using namespace std;

//将摄氏温度转换为华氏温度
double toFahrenheit(double tCelsius) {
	return (9 / 5.0) * tCelsius + 32;
}

//第一次
int mainForTemperatureTranslate() {
	double t;
	cout << "输入温度: " << endl;
	while (cin>>t) {
		cout << toFahrenheit(t) << endl;
		cout << "输入温度: " << endl;
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

//第一次
int mainForAlphTranslate() {
	string upperCase;
	cout << "输入字符串: " << endl;
	while (cin >> upperCase) {
		transform(upperCase.begin(), upperCase.end(), upperCase.begin(), toUpper);
		cout << upperCase << endl;
		cout << "输入字符串: " << endl;
	}
	return 0;
}


// 输入n个人姓名, 统计其中有多少个以"M"字母开头的名字, 并把所有以"M"开头的姓名和最长的姓名显示出来
int mainForSolve1_11_5() {
	std::list<string> strList;
	cout << "输入姓名 以'-1'结束: " << endl;
	string inBuffer;
	while ((cin >> inBuffer) && inBuffer != "-1") {
		strList.push_back(inBuffer);
	}
	size_t maxLen = MIN_INT32;
	string maxstr = "";
	for (auto it = strList.begin(); it != strList.end(); ++it) {
		if ((*it)[0] != 'M') {
			// do nothing
		}
		else {
			cout << *it << endl;
		}

		if (it->length() > maxLen) {
			maxLen = it->length();
			maxstr = *it;
		}
	}
	cout << maxstr << endl;
	return 0;
}

//计算一元二次方程ax^2 + bx + c = 0; 的两个根x1, x2
void calcEquationRoot(double a, double b, double c, double &x1, double &x2) {
	double derta = b*b - 4*a*c;
	_ASSERT_EXPR(derta >= 0, "<0时无实数解");
	double tempX1, tempX2;
	tempX1 = (-b + sqrt(derta)) / (2 * a);
	tempX2 = (-b - sqrt(derta)) / (2 * a);
	x1 = tempX1;
	x2 = tempX2;
}

//计算一元一次方程的解 (线性方程)
double calcEquationRoot(double a, double b) {
	_ASSERT_EXPR(Utility::Double(a) != 0, "此时无解");
	return -b / a;
}

//求解一元二次方程 ax^2 + bx + c = 0; 
//将求出的两个解作为下一个方程的a和b, a-b作为c
//将求出的两个解作为一元一次ax + b = 0方程的a和b 求解后输出解使用函数重载 和 静态变量
int mainForSolve2_11_5() {
	static int a, b, c;
	double x1 = 0, x2 = 0;
	cout << "请输入a, b, c 以空格间隔: " << endl;
	cin >> a >> b >> c;
	calcEquationRoot(a, b, c, x1, x2);
	cout << "x1: " << x1 << "; x2: " << x2 << endl;
	calcEquationRoot(x1, x2, x1 - x2, x1, x2);
	cout << "x1: " << x1 << "; x2: " << x2 << endl;
	if (Utility::Double(x1) == 0) {
		cout << "a==0无解!" << endl;
	}
	else {
		cout << "x: " << calcEquationRoot(x1, x2) << endl;
	}
	return 0;
}

int main() {
	mainForTemperatureTranslate();
	//mainForAlphTranslate();

	//mainForSolve1_11_5();
	//mainForSolve2_11_5();
	return 0;
}
