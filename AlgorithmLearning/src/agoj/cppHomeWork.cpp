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

class MyDate {
private:
	// 年份, 模年月份, 模月天数
	int year, month, dayOfMonth;
	// 模周天数[1, 8)
	int dayOfWeek;
public:
	void setYear(int year) {
		this->year = year;
	}
	void setMonth(int month) {
		this->month = month;
	}
	void setDay(int day) {
		this->dayOfMonth = day;
	}

	void setDayOfWeek(int week) {
		this->dayOfWeek = week;
	}
	int getYear() const {
		return this->year;
	}
	int getMonth() const {
		return this->month;
	}
	int getDay() const {
		return this->dayOfMonth;
	}
	int getDayOfWeek() const {
		return this->dayOfWeek;
	}
};
bool isLeapYear(int);
int getWeekDay(int, int, int);
int calendar(MyDate *date){
	int year = date->getYear(), month = date->getMonth(), dayOfWeek = date->getDayOfWeek();
	//输出日历日历标题
	cout << "     " << year << "年" << month << "月" << endl;
	//输出星期
	cout << ("日 一 二 三 四 五 六\n");
	int i;
	//每月第一周前几天用空格填充
	for (i = 0; i < dayOfWeek; i++)
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
		cout << std::setw(2) << dayOfMonth << " ";
		++dayOfMonth;
		//每输出完星期六的日期后就换行(对应周数起点是从dayOfWeek开始的，所以要在天数上加上dayOfWeek)
		if ((dayOfMonth + dayOfWeek - 1) % 7 == 0)
			cout << endl;
	}
	date->setDayOfWeek((date->getDayOfWeek() + --dayOfMonth % 7) % 7);
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
	MyDate buffer;
	buffer.setDayOfWeek(week);
	if (StandardExtend::inRange(1, week, 7)) {
		for (month = 1; month < 13; ++month) {
			//calendar(year, month, getWeekDay(year, month, 1));
			buffer.setYear(year);
			buffer.setMonth(month);
			calendar(&buffer);
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

// ===============================================================

// 辣鸡股票
class Stock {
public:
	
	Stock(std::string const &symbol, std::string const &name) {
		this->symbol = symbol;
		this->name = name;
	}

	std::string const &getSymbol() const {
		return symbol;
	}

	/*void setSymbol(std::string const &symbol) {
		this->symbol = symbol;
	}*/

	std::string const &getName() const {
		return name;
	}

	/*void setName(std::string const &name) {
		this->name = name;
	}*/

	double getPreviousClosingPrice() const {
		return previousClosingPrice;
	}

	void setPreviousClosingPrice(double previousClosingPrice) {
		this->previousClosingPrice = previousClosingPrice;
	}

	double getCurrentClosingPrice() const {
		return currentClosingPrice;
	}

	void setCurrentClosingPrice(double currentClosingPrice) {
		this->currentClosingPrice = currentClosingPrice;
	}

	// 返回当前股价与前一交易日收盘价的变化百分比
	double changePercent() const {
		return ((currentClosingPrice - previousClosingPrice) / previousClosingPrice) * 100;
	}

private:
	// 股票编号
	std::string symbol;
	// 股票名字
	std::string name;
	// 前一日的股票交易日收盘价
	double previousClosingPrice;
	// 前一日的股票当前价格
	double currentClosingPrice;
};

int mainForSolve_12_3() {
	Stock tester("BABA", "AlibabaGroup Inc.");
	tester.setPreviousClosingPrice(90.0);
	tester.setCurrentClosingPrice(80.0);
	std::cout << tester.changePercent() << endl;
	return 0;
}


// 运算符重载 一象限的点类
class PointFirstQuadrant {
public:
	double x, y;
	PointFirstQuadrant() = default;
	PointFirstQuadrant(double x, double y) {
		this->x = x;
		this->y = y;
		judgeIsInFirstQuadrant();
	}
	PointFirstQuadrant(std::istream &in) {
		//scanf("%lf%lf", &(this->x), &(this->y));
		in >> this->x >> this->y;
		judgeIsInFirstQuadrant();
	}
	// 返回原点
	static PointFirstQuadrant O() {
		static PointFirstQuadrant ZERO = { 0.0, 0.0 };
		return ZERO;
	}

	friend std::ostream &operator<<(std::ostream &os, const PointFirstQuadrant &rhs) {
		os << "(" << rhs.x << ", " << rhs.y << ")";
		return os;
	}

	// 返回两点到原点的距离之和
	double operator+(PointFirstQuadrant const &rhs) const {
		return dDistance(*this, O()) + dDistance(rhs, O());
	}

	// 返回两点到原点的距离之差
	double operator-(PointFirstQuadrant const &rhs) const {
		return dDistance(*this, O()) - dDistance(rhs, O());
	}

	// 返回两点之间的距离
	double operator*(PointFirstQuadrant const &rhs) const {
		return dDistance(*this, rhs);
	}

	// 判断相等
	bool operator==(PointFirstQuadrant const &rhs) const {
		return this->x == rhs.x && this->y == rhs.y;
	}
	bool operator!=(PointFirstQuadrant const &rhs) const {
		return !(*this == rhs);
	}

	// 返回两点以及原点组成的三角形的周长
	double operator/(PointFirstQuadrant const &rhs) const {
		_ASSERT_EXPR(*this != rhs, "无法组成三角形!");
		_ASSERT_EXPR(*this != O(), "无法组成三角形!");
		return *this + rhs + dDistance(*this, rhs);
	}

	// 坐标值自增1
	PointFirstQuadrant operator++() {
		++this->x;
		++this->y;
		judgeIsInFirstQuadrant();
		return *this;
	}

	// 坐标值自增1
	PointFirstQuadrant operator++(int) {
		auto tmp = *this;
		++this->x;
		++this->y;
		judgeIsInFirstQuadrant();
		return tmp;
	}

	// 坐标值自减1
	PointFirstQuadrant operator--() {
		--this->x;
		--this->y;
		judgeIsInFirstQuadrant();
		return *this;
	}

	// 坐标值自减1
	PointFirstQuadrant operator--(int) {
		auto tmp = *this;
		--this->x;
		--this->y;
		judgeIsInFirstQuadrant();
		return tmp;
	}

	// 返回坐标到原点的距离
	void operator()() const {
		cout << "到原点的距离: " << dDistance(*this, O()) << endl;
	}

	void operator=(PointFirstQuadrant const &rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		judgeIsInFirstQuadrant();
	}

	void judgeIsInFirstQuadrant() const {
		if (this->x < 0 || this->y < 0) {
			cout << "象限越界: 只允许在第一象限!" << endl;
		}
		else {
			// do nothing
		}
	}

	//返回两点间距离
	static double dDistance(PointFirstQuadrant const &p1, PointFirstQuadrant const &p2) {
		return sqrt(dDistance2(p1, p2));
	}
	//返回两点间距离的平方
	static double dDistance2(PointFirstQuadrant const &p1, PointFirstQuadrant const &p2) {
		double dx = p2.x - p1.x;
		double dy = p2.y - p1.y;
		return (dx*dx + dy * dy);
	}
};

int mainFor12_17_A() {
	PointFirstQuadrant a(1, 0), b(0, 1), c = a++, d = --a, e(a + b, a - b);
	cout << a;
	a();
	cout << b;
	b();
	cout << c;
	c();
	cout << d;
	d();
	cout << "({" << a << "+" << b << "}" << ", {" << a << "-" << b << "})" << " = " << e;
	e();
	cout << "测试结束 请输入坐标点: ";
	PointFirstQuadrant f(cin);
	// 两次结果应该一致
	cout << f;
	f();
	f = f++;
	f = f--;
	f = ++f;
	f = --f;
	cout << f;
	f();

	// f值没有变
	f = PointFirstQuadrant(a * b, a / b);
	cout << f;
	f();
	return 0;
}


// 颜色
class Color {
public:
	BYTE r, g, b;
	std::string cName;
	Color() {
		r = g = b = 0;
	}
	Color(std::string const &cName) {
		this->cName = cName;
	}
	Color(BYTE r, BYTE g, BYTE b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	virtual ~Color(){}
};

// 几何
class Geomertric {
public:
	virtual ~Geomertric() {}
	// 返回面积
	virtual double getArea() const = 0;
	// 返回周长
	virtual double getPerimetter() const = 0;

	void setIsFill(bool isFill) {
		this->isFill = isFill;
	}

	bool getIsFill() const {
		return this->isFill;
	}

	void setColor(Color c) {
		this->color = c;
	}

	Color getColor() const {
		return this->color;
	}

private:
	bool isFill = false;
	Color color;
};

// 基于边的三角形
class Triangle : public Geomertric {
	using SideType = double;
public:
	Triangle() {
		side1 = 1.0, side2 = 1.0, side3 = 1.0;
	}
	Triangle(SideType const &side1, SideType const &side2, SideType const &side3) {
		this->side1 = side1;
		this->side2 = side2;
		this->side3 = side3;
	}
	virtual ~Triangle() {}

	//海伦公式 返回由side1, side2, side3组成的三角形的面积
	static double dAreaOfTriangle(SideType const &side1, SideType const &side2, SideType const &side3) {
		double d12 = side1
			, d13 = side2
			, d23 = side3;
		double dL = (d12 + d13 + d23) / 2.0;
		return sqrt(dL * (dL - d12) * (dL - d13) * (dL - d23));
	}
	//符合勾股定理 返回true 练习题HDU2899
	static bool isPythagoreanTheorem(int a, int b, int c) {
		return a * a == b * b + c * c || b * b == a * a + c * c || c * c == a * a + b * b;
	}
	//判断由传入的参数是否能组成三角形 若可以 返回true 否则返回false
	static bool isTrangle(double a, double b, double c) {
		//三角形边长不等式
		return a + b > c && a + c > b && b + c > a
			&& fabs(a - b) < c && fabs(a - c) < b && fabs(b - c) < a;
	}

	void setSide1(SideType s) {
		this->side1 = s;
	}

	SideType getSide1() const {
		return this->side1;
	}

	void setSide2(SideType s) {
		this->side2 = s;
	}

	SideType getSide2() const {
		return this->side2;
	}

	void setSide3(SideType s) {
		this->side3 = s;
	}

	SideType getSide3() const {
		return this->side3;
	}

	double getArea() const override {
		return dAreaOfTriangle(this->getSide1(), this->getSide2(), this->getSide3());
	}

	double getPerimetter() const override {
		return this->getSide1() + this->getSide2() + this->getSide3();
	}

private:
	double side1, side2, side3;
};

int mainFor12_17_B() {
	Triangle t(1.0, 1.5, 1.0);
	// t.setColor(Color(255, 255, 0));
	auto c = Color(255, 255, 0);
	c.cName = "yellow";
	t.setColor(c);
	t.setIsFill(true);
	cout << "面积: " << t.getArea()
		<< "周长: " << t.getPerimetter()
		<< "颜色: " << t.getColor().cName
		<< "是否填充: " << t.getIsFill()
		<< endl;
	return 0;
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
		// mainForSolve_11_25();

		// 第四次
		// mainForSolve_12_3();

		// 第五次
		// mainFor12_17_A();
		mainFor12_17_B();
		cout << "结束y/n" << endl;
	} while ((cin >> control) && control != "y");
	return 0;
}
