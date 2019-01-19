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

int toRadixIntNum(char alphOrCharNum, int radix = 10){
	int result = StandardExtend::toIntNum(alphOrCharNum);
	_ASSERT_EXPR(StandardExtend::inRange(0, result, radix), "����ָ�����Ƶ��ַ���");
	return result;
}

/**
* ������ƴ����ӷ�: ֧�ִ�Сд����, ֻ֧�ִ�д���;
* ֧��10������+26���ַ�(��Сд�ȼ�) ��ʾ��[2, 36]����
* �������������Ʋ������쳣(����10��������A);
* ����std::string ֱ�Ӹ���string�ڴ�ʵ�� sum�����������ͬ
**/
std::string bigPlush(std::string &topLowNumA, std::string &topLowNumB, std::string &topLowSum, int radix = 10) {
	std::size_t lenA = topLowNumA.size(), lenB = topLowNumB.size(), lenAB;
	//��0��
	std::string temp;
	//��λ����, ���߸�λ0����
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
	int ia = -1, carryNum = 0;
	//��ת������ǵ�λ
	int i = -1;
	for (Utility::toSignedNum(lenAB - 1, i); i >= 0; --i) {
		// int sumBit = (topLowNumA[i] - '0') + (topLowNumB[i] - '0') + carryNum;
		int sumBit = toRadixIntNum(topLowNumA[i], radix) + toRadixIntNum(topLowNumB[i], radix) + carryNum;
		// topLowSum[i] = sumBit % 10 + '0';
		topLowSum[i] = StandardExtend::toUppercaseAscllChar(sumBit % radix);
		// carryNum = sumBit / 10;
		carryNum = sumBit / radix;
	}
	return carryNum == 0 ? topLowSum : (topLowSum = "1" + topLowSum);
}

template<class DigitIterator>
std::string digitArrayToString(DigitIterator leftIter, DigitIterator rightIter) {
	auto str = std::string(leftIter, rightIter);
	std::for_each(str.begin(), str.end(), [](char &intNum) {
		intNum = StandardExtend::toUppercaseAscllChar(intNum);
	});
	return str;
}

// ��������Ʋ��� (ʮ��������)
void calcBinaryCode(int decNum, std::string &originCode, std::string &inverseCode, std::string &complementCode) {
	// BinaryTransition oBinCode = BinaryTransition(100);
	vector<int> lowTop;
	vector<int> topLow;
	lowTop.resize(MathExtend::calcDigitTotalSize(decNum, 2));
	topLow.resize(MathExtend::calcDigitTotalSize(decNum, 2));
	puts("ԭ��");
	// solution0
	int totalSize = BinaryTransition::decimalToRadixLowTop(decNum, lowTop.begin(), 2);
	BinaryTransition::outputWithSymbol(lowTop, totalSize);
	// solution1
	BinaryTransition::decimalToRadixTopLow(decNum, topLow.end(), 2);
	originCode = digitArrayToString(topLow.begin(), topLow.end());

	puts("����");
	// solution0
	BinaryTransition::inverseCode(lowTop.begin(), totalSize);
	BinaryTransition::outputWithSymbol(lowTop, totalSize);
	// solution1
	BinaryTransition::inverseCode(topLow.begin(), totalSize);
	inverseCode = digitArrayToString(topLow.begin(), topLow.end());

	// puts("����");
	complementCode = digitArrayToString(lowTop.rbegin(), lowTop.rend());
	bigPlush(complementCode, std::string("1"), complementCode, 2);
	// std::cout << complementCode << std::endl;
}

int main(){
	/*
	BigInteger b = BigInteger(35);
	b.print();
	*/
	std::string originCode;
	std::string inverseCode;
	std::string complementCode;
	calcBinaryCode(2147483647, originCode, inverseCode, complementCode);
	StandardExtend::testAndOut(std::string("ԭ��"), originCode, std::string("1111111111111111111111111111111"));
	StandardExtend::testAndOut(std::string("����"), inverseCode, std::string("0000000000000000000000000000000"));
	StandardExtend::testAndOut(std::string("����"), complementCode, std::string("0000000000000000000000000000001"));
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
	// 1-111-1111 0111-1111 0111-1111 1000-0000ԭ <==> -2139062144
	// 1-000-0000 1000-0000 1000-0000 0111-1111��
	// 1-000-0000 1000-0000 1000-0000 1000-0000�� ->80H
	int arrayS[20];
	memset(arrayS, 0x80, 20*sizeof(int));
	memset(arrayS, 0x7f, 20*sizeof(int));
}

#include "./memory/Hash/HashTable.h"
int mainForHashTable() {
	// ����cpp�ļ����Դ���ͬ������(.h�ǿ϶�����), �����ܴ���ͬ����������޷��������صķ���
	// (�п��ܻ���ֱ���ͨ����DBUG���ͻ���, ������Ҫ��������һ��)
	HashTableC::HashTableLiPtr h = HashTableC::CreateLinkedListHashTable(5);
	srand(clock());
	// ����û�д���ɢ�г�ͻ �޷���Ӷ��ͬHashֵ��Key ��ֻ�����20��Ԫ��
	for (int i = 0; i < 20; ++i) {
		Insert(h, std::to_string(i).c_str());
	}
	printf("%s", Find(h, "3")->Data);
	return 0;
}
