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

//#include "MathLibrary.h"
#include "ExtendSpace.h"
#include "BigInteger.h"
using namespace std;


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
		int totalBit = BinaryTransition::decimalToRadixLowTop(onum, lowTop.begin(), 2);
		oBinCode.outputWithSymbol(lowTop, 7);

		//����
		oBinCode.reverseCode(lowTop.begin(), totalBit);
		oBinCode.outputWithSymbol(lowTop, 7);

		++onum;

	}
	return 0;
}
/*
#include"./memory/ArrayLinearList.h"

int main() {
	LinearList::mainForArrayLinearList();
}
*/


void fun() {
	// ���ֽڶ��ڴ����г�ʼ��(���ֽ��ַ�������ȫ���Ե�) PS ��ɲ���MAX_INT32
	// 8bit 7FH: 0111-1111B ==> 4��һ����Byte => 2139062143D
	// �������Բ���洢�� �����ʼ��һ��������Сֵ��������Ҫ����λΪ1
	// -2147483647��
	// 1-111-1111 1111-1111 1111-1111 1111-1111ԭ
	// 1-000-0000 0000-0000 0000-0000 0000-0000��
	// 1-000-0000 0000-0000 0000-0000 0000-0001�� <==> û�й���, ���ܰ��ֽڳ�ʼ��
	// ��һ��
	// 1-000-0000 1000-0000 1000-0000 1000-0000�� ->80H
	// 1-000-0000 1000-0000 1000-0000 0111-1111��
	// 1-111-1111 0111-1111 0111-1111 1000-0000ԭ <==> -2139062144
	int arrayS[20];
	memset(arrayS, 0x80, 20*sizeof(int));
	memset(arrayS, 0x7f, 20*sizeof(int));
}