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

int main() {
	mainForTemperatureTranslate();
	//mainForAlphTranslate();

	//mainForSolve1_11_5();
	//mainForSolve2_11_5();
	return 0;
}
