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
#include "./else/BigInteger.h"
// using namespace std;

// ���еľ�̬����->�����ռ���ͨ����(��������java��ֲ�Ļ�)

// ��������Ʋ��� (ʮ��������)
void calcBinaryCode(int decNum, std::string &originCode, std::string &inverseCode, std::string &complementCode) {
	std::vector<int> lowTop;
	std::vector<int> topLow;
	lowTop.resize(MathExtend::calcDigitTotalSize(decNum, 2));
	topLow.resize(MathExtend::calcDigitTotalSize(decNum, 2));
	puts("ԭ��");
	// solution0
	int totalSize = TransitionUtility::decimalToRadixLowTop(decNum, lowTop.begin(), 2);
	TransitionUtility::outputDigitInRange(lowTop.rbegin(), lowTop.rend());
	// solution1
	TransitionUtility::decimalToRadixTopLow(decNum, topLow.end(), 2);
	originCode = TransitionUtility::digitContainerToString(topLow.begin(), topLow.end());

	puts("����");
	// solution0
	TransitionUtility::inverseCode(lowTop.begin(), lowTop.begin() + totalSize);
	TransitionUtility::outputDigitInRange(lowTop.begin(), lowTop.begin() + totalSize);
	// solution1
	TransitionUtility::inverseCode(topLow.begin(), topLow.end());
	inverseCode = TransitionUtility::digitContainerToString(topLow.begin(), topLow.end());

	// puts("����");
	complementCode = TransitionUtility::digitContainerToString(lowTop.rbegin(), lowTop.rend());
	BigInteger::bigPlush(complementCode, std::string("1"), complementCode, 2);
	// std::cout << complementCode << std::endl;
}

int main(){
	/*
	BigInteger b = BigInteger(35);
	b.print();
	*/
	StandardExtend::testAndOut(std::string("����->�ַ�{��ĸ, ����}"), TransitionUtility::toAlphOrAscllNum(0), '0');
	StandardExtend::testAndOut(std::string("����->�ַ�{��ĸ, ����}"), TransitionUtility::toAlphOrAscllNum(9), '9');
	StandardExtend::testAndOut(std::string("����->�ַ�{��ĸ, ����}"), TransitionUtility::toAlphOrAscllNum(10), 'A');
	StandardExtend::testAndOut(std::string("����->�ַ�{��ĸ, ����}"), TransitionUtility::toAlphOrAscllNum(35), 'Z');
	StandardExtend::testAndOut(std::string("����->�ַ�{��ĸ, ����}"), TransitionUtility::toAlphOrAscllNum(36), 'a');
	StandardExtend::testAndOut(std::string("����->�ַ�{��ĸ, ����}"), TransitionUtility::toAlphOrAscllNum(61), 'z');
	StandardExtend::testAndOut(std::string("�ַ�{��ĸ, ����}->����"), TransitionUtility::toIntNum('0'), 0);
	StandardExtend::testAndOut(std::string("�ַ�{��ĸ, ����}->����"), TransitionUtility::toIntNum('9'), 9);
	StandardExtend::testAndOut(std::string("�ַ�{��ĸ, ����}->����"), TransitionUtility::toIntNum('A'), 10);
	StandardExtend::testAndOut(std::string("�ַ�{��ĸ, ����}->����"), TransitionUtility::toIntNum('Z'), 35);
	StandardExtend::testAndOut(std::string("�ַ�{��ĸ, ����}->����"), TransitionUtility::toIntNum('a'), 36);
	StandardExtend::testAndOut(std::string("�ַ�{��ĸ, ����}->����"), TransitionUtility::toIntNum('z'), 61);
	std::string originCode;
	std::string inverseCode;
	std::string complementCode;
	calcBinaryCode(6, originCode, inverseCode, complementCode);
	calcBinaryCode(2147483647, originCode, inverseCode, complementCode);
	StandardExtend::testAndOut(std::string("ԭ��"), originCode, std::string("1111111111111111111111111111111"));
	StandardExtend::testAndOut(std::string("����"), inverseCode, std::string("0000000000000000000000000000000"));
	StandardExtend::testAndOut(std::string("����"), complementCode, std::string("0000000000000000000000000000001"));

	std::cout << "����ת����" << std::endl;
	std::vector<int> targetDigitList;
	std::string targetDigitStr;

	BinaryTransition binRadixTransiter = BinaryTransition(originCode.length(), 1, 1);
	binRadixTransiter.transition(originCode.c_str(), targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("BIN->BIN"), targetDigitStr, originCode);

	binRadixTransiter.reset(2, 2);
	//BinaryTransition binTransOct = BinaryTransition(originCode.length(), 1, 3);
	binRadixTransiter.reset(1, 3);
	binRadixTransiter.transition(originCode.c_str(), targetDigitList);
	//binTransOct.transition(originCode.c_str(), targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("BIN->OCT"), targetDigitStr, std::string("17777777777"));
	//BinaryTransition octTransBin = BinaryTransition(originCode.length(), 3, 1);
	//octTransBin.transition(targetDigitStr.c_str(), targetDigitList);
	binRadixTransiter.reset(3, 1);
	binRadixTransiter.transition(targetDigitStr.c_str(), targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("OCT->BIN"), targetDigitStr, originCode);

	//BinaryTransition binTransHex = BinaryTransition(originCode.length(), 1, 4);
	//binTransHex.transition(originCode.c_str(), targetDigitList);
	binRadixTransiter.reset(1, 4);
	binRadixTransiter.transition(originCode.c_str(), targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("BIN->HEX"), targetDigitStr, std::string("7FFFFFFF"));
	BinaryTransition hexTransBin = BinaryTransition(originCode.length(), 4, 1);
	//hexTransBin.transition(targetDigitStr.c_str(), targetDigitList);
	binRadixTransiter.reset(4, 1);
	binRadixTransiter.transition(targetDigitStr.c_str(), targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("HEX->BIN"), targetDigitStr, originCode);

	// 2^31D
	binRadixTransiter.reset(4, 1);
	binRadixTransiter.transition("80000000", targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("HEX->BIN"), targetDigitStr, std::string("10000000000000000000000000000000"));

	binRadixTransiter.reset(4, 3);
	binRadixTransiter.transition("39", targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("HEX->OCT"), targetDigitStr, std::string("71"));

	binRadixTransiter.reset(1, 1);
	return 0;
}



#include"./memory/ArrayLinearList.h"
int mainForLinearList() {
	LinearList::mainForArrayLinearList();
	return 0;
}

void demoForMemset() {
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
