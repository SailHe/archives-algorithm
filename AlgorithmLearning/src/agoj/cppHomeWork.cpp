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


// 输入年份和这一年的第一天是星期几 输出日历
// 2005 6表示2005-1-1是星期6
// Jannuary 2005
bool isLeapYear(int);
int getWeekDay(int, int, int);
int calendar(int year, int month, int week){
	//输出日历日历标题
	cout << "     " << year << "年" << month << "月" << endl;
	//输出星期
	cout << ("日 一 二 三 四 五 六\n");
	int i;
	//每月第一周前几天用空格填充
	for (i = 0; i < week; i++)
		cout << ("   ");

	//每个月的最后一天
	int endDayOfMonth;
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
		endDayOfMonth = 31;
	else if (month == 2) {
		if (isLeapYear(year))
			endDayOfMonth = 29;
		else
			endDayOfMonth = 28;
	}
	else
		endDayOfMonth = 30;

	int dayOfMonth = 1;
	//从一号循环到本月末
	while (dayOfMonth <= endDayOfMonth) {
		printf("%2d ", dayOfMonth);
		dayOfMonth++;
		//每输出完星期六的日期后就换行(对应周数起点是从week开始的，所以要在天数上加上week)
		if ((dayOfMonth + week - 1) % 7 == 0)
			cout << endl;
	}
	cout << endl;
	return 0;
}
int mainForSolve_11_25() {
	int year;
	int month;
	int week;

	cout<<("输入: 日期(yyyy) 1-1号的星期数[1, 7]: ");
	//输入年和月
	cin >> year >> week;
	if (StandardExtend::inRange(1, week, 7)) {
		for (month = 1; month < 12; ++month) {
			//calendar(year, month, getWeekDay(year, month, 1));
			calendar(year, month, week);
		}
	}
	else {
		cout << "星期数属于[1, 7]!" << endl;
	}
	return 0;
}
//判断闰年
bool isLeapYear(int y){
	if (y % 4 == 0 && y % 100 != 0 || y % 400 == 0)
		return true;
	return false;
}
//获取某天是星期几返回值(0-7)
int getWeekDay(int y, int m, int d){
	//计算星期几的固定公式,Y是年D是这个日期在本年的天数
	//W = [Y-1] + [(Y-1)/4] - [(Y-1)/100] + [(Y-1)/400] + D 
	int days = d;
	int i;
	for (i = 1; i < m; i++){
		if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
			days += 31;
		else if (i == 2){
			if (isLeapYear(y)) {
				days += 29;
			}
			else {
				days += 28;
			}
		}
		else {
			days += 30;
		}
	}

	int temp = y - 1 + (int)((y - 1) / 4) - (int)((y - 1) / 100) + (int)((y - 1) / 400) + days;
	return temp % 7;
}

int main(int argc, char const *argv[]) {
	string control = "";
	do {
		// 第一次
		//mainForTemperatureTranslate();
		//mainForAlphTranslate();

		// 第二次
		//mainForSolve1_11_5();
		//mainForSolve2_11_5();

		// 第三次
		mainForSolve_11_25();
		cout << "结束y/n" << endl;
	} while ((cin >> control) && control != "y");
	return 0;
}
