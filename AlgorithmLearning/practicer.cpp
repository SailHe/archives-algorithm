#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <random>
#include <time.h>
#include "Standard.h"
using namespace std;

#include "BigInteger.h"

void randomTest(){
	random_device rd;   // non-deterministic generator  
	//÷ɭ��ת���㷨��Mersenne twister����һ��α����������㷨�����ɱ����������ʿ[1]��1997�꿪�����������޶������ֶ��ϵľ������Եݹ� {\displaystyle F_{2}} F_{{2}}�����Կ��ٲ�����������α������������˹ŵ�����������㷨�ĺܶ�ȱ�ݡ�
	mt19937 gen(rd());  // to seed mersenne twister.  
	// replace the call to rd() with a  
	// constant value to get repeatable  
	// results.  

	for (int i = 0; i < 5; ++i) {
		cout << gen() << " "; // print the raw output of the generator.  
	}
	cout << endl;
}

int main_1(){
	rand();
	randomTest();
	freopen("input", "r", stdin);
	vector<int> ans(5);
	int t;
	char origin[1005];
	scanf("%d", &t);
	getchar();
	BigInteger s = BigInteger(9) + BigInteger(1);
	s = 1;
	int n = 10001;
	s.reserve(1005);
	/*
	28462596809170545189064132121198688901480514
	500
	12201368259911100687012387854230469
	*/

	s.fact(10000).print();
	while (t-- > 0){
		BinaryTransition bt = BinaryTransition(100, 4, 3);
		gets(origin);
		char *a = origin, *b = NULL;
		for (b = origin; *b != ' '; ++b);
		*b++ = '\0';
		//bt.transition(a, ans);
		BigInteger A = BigInteger(a);
		printf("A ");
		A.print();
		BigInteger B = BigInteger(b);
		printf("B ");
		B.print();
		BigInteger sum = A + B;
		sum.muity(10000);
		printf("S ");
		sum.print();
		//sum.fact();
		//sum.print();
	}
	return 0;
}

int main_2(){
	/*BigInteger b = BigInteger(35);
	b.print();*/
	printf("����complementCode����\n");
	int onum;
	while (~scanf("%d", &onum)){
		BinaryTransition oBinCode = BinaryTransition(100);
		vector<int> lowTop;
		lowTop.resize(30);
		//ԭ��
		int totalBit = oBinCode.tenToRadix(onum, lowTop.begin(), 2);
		oBinCode.outputWithSymbol(lowTop, 7);

		//����
		oBinCode.reverseCode(lowTop.begin(), totalBit);
		oBinCode.outputWithSymbol(lowTop, 7);

		++onum;

	}
	return 0;
}

typedef __int64 Integer;

/*�ݹ���㷨*/
Integer fibonacciRec(int n){
	return n == 1 || n == 2 ? 1 :
		fibonacciRec(n - 1) + fibonacciRec(n - 2);
}

/*��̬�滮���㷨 ��ͨ��*/
Integer fibonacci(int maxN){
	++maxN;
	Integer *fBuffer = (Integer *)malloc(sizeof(Integer)*maxN);
	memset(fBuffer, 0, sizeof(Integer)*maxN);
	fBuffer[1] = fBuffer[2] = 1;
	for (int n = 3; n < maxN; ++n){
		fBuffer[n] = fBuffer[n - 1] + fBuffer[n - 2];
	}
	Integer temp = fBuffer[maxN - 1];
	free(fBuffer);
	fBuffer = nullptr;
	return temp;
}

/*��̬�滮���㷨 �����ǿ��*/
void fibonacciBuffer(Integer *buffer, int length){
	Integer *f = buffer;
	memset(f, 0, sizeof(Integer)*length);
	f[1] = f[2] = 1;
	for (int n = 3; n < length; ++n){
		f[n] = f[n - 1] + f[n - 2];
	}
}

int main(){
	double temp_ = 0.375;
	printf("%.4f\n", temp_);
	printf("%.2f\n", temp_);
	printf("%d\n", temp_);
	temp_ = 0.30;
	printf("%.4f\n", temp_);
	printf("%.2f\n", temp_);
	printf("%d\n", temp_);
	clock_t start = clock();
	clock_t stop = clock();
	Integer f[101];
	int testN[14] = { 10, 20, 30, 40, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };
	fibonacciBuffer(f, 101);
	puts("n               �ݹ���   ��ʱ(ms)      ��̬�滮���    ��ʱ(ms)");
	for (int i = 0; i < 14; ++i){
		printf("%d ", testN[i]);
		start = clock();
		printf("%20I64d", fibonacciRec(testN[i]));
		stop = clock();
		printf("%10.f", (double)(stop - start));// (double)(stop - start) / CLK_TCK //��

		start = clock();
		//printf("%20I64d", f[testN[i]]);
		printf("%20I64d", fibonacci(testN[i]));
		stop = clock();
		printf("%10.f\n", (double)(stop - start));// (double)(stop - start) / CLK_TCK //��
	}
	return 0;
}
