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
bool isLeapYear(int);
int getWeekDay(int, int, int);
int calendar(int year, int month, int week){
	//���������������
	cout << "     " << year << "��" << month << "��" << endl;
	//�������
	cout << ("�� һ �� �� �� �� ��\n");
	int i;
	//ÿ�µ�һ��ǰ�����ÿո����
	for (i = 0; i < week; i++)
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
		printf("%2d ", dayOfMonth);
		dayOfMonth++;
		//ÿ����������������ں�ͻ���(��Ӧ��������Ǵ�week��ʼ�ģ�����Ҫ�������ϼ���week)
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

	cout<<("����: ����(yyyy) 1-1�ŵ�������[1, 7]: ");
	//���������
	cin >> year >> week;
	if (StandardExtend::inRange(1, week, 7)) {
		for (month = 1; month < 12; ++month) {
			//calendar(year, month, getWeekDay(year, month, 1));
			calendar(year, month, week);
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
		mainForSolve_11_25();
		cout << "����y/n" << endl;
	} while ((cin >> control) && control != "y");
	return 0;
}
