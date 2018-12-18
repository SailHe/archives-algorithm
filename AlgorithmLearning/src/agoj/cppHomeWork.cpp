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

#include "ExtendSpace.h"

using namespace std;

//�������¶�ת��Ϊ�����¶�
double toFahrenheit(double tCelsius) {
	return (9 / 5.0) * tCelsius + 32;
}

//��һ��
int mainForTemperatureTranslate() {
	double t;
	cout << "�����¶�: " << endl;
	while (cin>>t) {
		cout << toFahrenheit(t) << endl;
		cout << "�����¶�: " << endl;
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

//��һ��
int mainForAlphTranslate() {
	string upperCase;
	cout << "�����ַ���: " << endl;
	while (cin >> upperCase) {
		transform(upperCase.begin(), upperCase.end(), upperCase.begin(), toUpper);
		cout << upperCase << endl;
		cout << "�����ַ���: " << endl;
	}
	return 0;
}


// ����n��������, ͳ�������ж��ٸ���"M"��ĸ��ͷ������, ����������"M"��ͷ�����������������ʾ����
int mainForSolve1_11_5() {
	std::list<string> strList;
	cout << "�������� ��'-1'����: " << endl;
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

//����һԪ���η���ax^2 + bx + c = 0; ��������x1, x2
void calcEquationRoot(double a, double b, double c, double &x1, double &x2) {
	double derta = b*b - 4*a*c;
	_ASSERT_EXPR(derta >= 0, "<0ʱ��ʵ����");
	double tempX1, tempX2;
	tempX1 = (-b + sqrt(derta)) / (2 * a);
	tempX2 = (-b - sqrt(derta)) / (2 * a);
	x1 = tempX1;
	x2 = tempX2;
}

//����һԪһ�η��̵Ľ� (���Է���)
double calcEquationRoot(double a, double b) {
	_ASSERT_EXPR(Utility::Double(a) != 0, "��ʱ�޽�");
	return -b / a;
}

//���һԪ���η��� ax^2 + bx + c = 0; 
//���������������Ϊ��һ�����̵�a��b, a-b��Ϊc
//���������������ΪһԪһ��ax + b = 0���̵�a��b ���������ʹ�ú������� �� ��̬����
int mainForSolve2_11_5() {
	static int a, b, c;
	double x1 = 0, x2 = 0;
	cout << "������a, b, c �Կո���: " << endl;
	cin >> a >> b >> c;
	calcEquationRoot(a, b, c, x1, x2);
	cout << "x1: " << x1 << "; x2: " << x2 << endl;
	calcEquationRoot(x1, x2, x1 - x2, x1, x2);
	cout << "x1: " << x1 << "; x2: " << x2 << endl;
	if (Utility::Double(x1) == 0) {
		cout << "a==0�޽�!" << endl;
	}
	else {
		cout << "x: " << calcEquationRoot(x1, x2) << endl;
	}
	return 0;
}


// ������ݺ���һ��ĵ�һ�������ڼ� �������
// 2005 6��ʾ2005-1-1������6
// Jannuary 2005

class MyDate {
private:
	// ���, ģ���·�, ģ������
	int year, month, dayOfMonth;
	// ģ������[1, 8)
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
	//���������������
	cout << "     " << year << "��" << month << "��" << endl;
	//�������
	cout << ("�� һ �� �� �� �� ��\n");
	int i;
	//ÿ�µ�һ��ǰ�����ÿո����
	for (i = 0; i < dayOfWeek; i++)
		cout << ("   ");

	//ÿ���µ����һ��
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
	//��һ��ѭ��������ĩ
	while (dayOfMonth <= endDayOfMonth) {
		cout << std::setw(2) << dayOfMonth << " ";
		++dayOfMonth;
		//ÿ����������������ں�ͻ���(��Ӧ��������Ǵ�dayOfWeek��ʼ�ģ�����Ҫ�������ϼ���dayOfWeek)
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

	cout<<("����: ����(yyyy) 1-1�ŵ�������[1, 7]: ");
	//���������
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
		cout << "����������[1, 7]!" << endl;
	}
	return 0;
}
//�ж�����
bool isLeapYear(int y){
	if (y % 4 == 0 && y % 100 != 0 || y % 400 == 0)
		return true;
	return false;
}
//��ȡĳ�������ڼ�����ֵ(0-7)
int getWeekDay(int y, int m, int d){
	//�������ڼ��Ĺ̶���ʽ,Y����D����������ڱ��������
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

// ������Ʊ
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

	// ���ص�ǰ�ɼ���ǰһ���������̼۵ı仯�ٷֱ�
	double changePercent() const {
		return ((currentClosingPrice - previousClosingPrice) / previousClosingPrice) * 100;
	}

private:
	// ��Ʊ���
	std::string symbol;
	// ��Ʊ����
	std::string name;
	// ǰһ�յĹ�Ʊ���������̼�
	double previousClosingPrice;
	// ǰһ�յĹ�Ʊ��ǰ�۸�
	double currentClosingPrice;
};

int mainForSolve_12_3() {
	Stock tester("BABA", "AlibabaGroup Inc.");
	tester.setPreviousClosingPrice(90.0);
	tester.setCurrentClosingPrice(80.0);
	std::cout << tester.changePercent() << endl;
	return 0;
}


// ��������� һ���޵ĵ���
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
	// ����ԭ��
	static PointFirstQuadrant O() {
		static PointFirstQuadrant ZERO = { 0.0, 0.0 };
		return ZERO;
	}

	friend std::ostream &operator<<(std::ostream &os, const PointFirstQuadrant &rhs) {
		os << "(" << rhs.x << ", " << rhs.y << ")";
		return os;
	}

	// �������㵽ԭ��ľ���֮��
	double operator+(PointFirstQuadrant const &rhs) const {
		return dDistance(*this, O()) + dDistance(rhs, O());
	}

	// �������㵽ԭ��ľ���֮��
	double operator-(PointFirstQuadrant const &rhs) const {
		return dDistance(*this, O()) - dDistance(rhs, O());
	}

	// ��������֮��ľ���
	double operator*(PointFirstQuadrant const &rhs) const {
		return dDistance(*this, rhs);
	}

	// �ж����
	bool operator==(PointFirstQuadrant const &rhs) const {
		return this->x == rhs.x && this->y == rhs.y;
	}
	bool operator!=(PointFirstQuadrant const &rhs) const {
		return !(*this == rhs);
	}

	// ���������Լ�ԭ����ɵ������ε��ܳ�
	double operator/(PointFirstQuadrant const &rhs) const {
		_ASSERT_EXPR(*this != rhs, "�޷����������!");
		_ASSERT_EXPR(*this != O(), "�޷����������!");
		return *this + rhs + dDistance(*this, rhs);
	}

	// ����ֵ����1
	PointFirstQuadrant operator++() {
		++this->x;
		++this->y;
		judgeIsInFirstQuadrant();
		return *this;
	}

	// ����ֵ����1
	PointFirstQuadrant operator++(int) {
		auto tmp = *this;
		++this->x;
		++this->y;
		judgeIsInFirstQuadrant();
		return tmp;
	}

	// ����ֵ�Լ�1
	PointFirstQuadrant operator--() {
		--this->x;
		--this->y;
		judgeIsInFirstQuadrant();
		return *this;
	}

	// ����ֵ�Լ�1
	PointFirstQuadrant operator--(int) {
		auto tmp = *this;
		--this->x;
		--this->y;
		judgeIsInFirstQuadrant();
		return tmp;
	}

	// �������굽ԭ��ľ���
	void operator()() const {
		cout << "��ԭ��ľ���: " << dDistance(*this, O()) << endl;
	}

	void operator=(PointFirstQuadrant const &rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		judgeIsInFirstQuadrant();
	}

	void judgeIsInFirstQuadrant() const {
		if (this->x < 0 || this->y < 0) {
			cout << "����Խ��: ֻ�����ڵ�һ����!" << endl;
		}
		else {
			// do nothing
		}
	}

	//������������
	static double dDistance(PointFirstQuadrant const &p1, PointFirstQuadrant const &p2) {
		return sqrt(dDistance2(p1, p2));
	}
	//�������������ƽ��
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
	cout << "���Խ��� �����������: ";
	PointFirstQuadrant f(cin);
	// ���ν��Ӧ��һ��
	cout << f;
	f();
	f = f++;
	f = f--;
	f = ++f;
	f = --f;
	cout << f;
	f();

	// fֵû�б�
	f = PointFirstQuadrant(a * b, a / b);
	cout << f;
	f();
	return 0;
}


// ��ɫ
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

// ����
class Geomertric {
public:
	virtual ~Geomertric() {}
	// �������
	virtual double getArea() const = 0;
	// �����ܳ�
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

// ���ڱߵ�������
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

	//���׹�ʽ ������side1, side2, side3��ɵ������ε����
	static double dAreaOfTriangle(SideType const &side1, SideType const &side2, SideType const &side3) {
		double d12 = side1
			, d13 = side2
			, d23 = side3;
		double dL = (d12 + d13 + d23) / 2.0;
		return sqrt(dL * (dL - d12) * (dL - d13) * (dL - d23));
	}
	//���Ϲ��ɶ��� ����true ��ϰ��HDU2899
	static bool isPythagoreanTheorem(int a, int b, int c) {
		return a * a == b * b + c * c || b * b == a * a + c * c || c * c == a * a + b * b;
	}
	//�ж��ɴ���Ĳ����Ƿ������������ ������ ����true ���򷵻�false
	static bool isTrangle(double a, double b, double c) {
		//�����α߳�����ʽ
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
	cout << "���: " << t.getArea()
		<< "�ܳ�: " << t.getPerimetter()
		<< "��ɫ: " << t.getColor().cName
		<< "�Ƿ����: " << t.getIsFill()
		<< endl;
	return 0;
}

int main(int argc, char const *argv[]) {
	string control = "";
	do {
		// ��һ��
		//mainForTemperatureTranslate();
		//mainForAlphTranslate();

		// �ڶ���
		//mainForSolve1_11_5();
		//mainForSolve2_11_5();

		// ������
		// mainForSolve_11_25();

		// ���Ĵ�
		// mainForSolve_12_3();

		// �����
		// mainFor12_17_A();
		mainFor12_17_B();
		cout << "����y/n" << endl;
	} while ((cin >> control) && control != "y");
	return 0;
}
