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
// using namespace std;

// 类中的静态方法->命名空间普通方法(若不考虑java移植的话)

/**
* 任意进制大数加法: 支持大小写传入, 只支持大写输出;
* 支持10个数字+26个字符(大小写等价) 表示的[2, 36]进制
* 若传入参数与进制不符报异常(比如10进制输入A);
* 基于std::string 直接更改string内存实现 sum可以与加数相同
**/
std::string bigPlush(std::string &topLowNumA, std::string &topLowNumB, std::string &topLowSum, int radix = 10) {
	std::size_t lenA = topLowNumA.length(), lenB = topLowNumB.length(), lenAB;
	//补0用
	std::string temp;
	//低位在右, 短者高位0补齐
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
	if (topLowSum.length() < lenA) {
		topLowSum.resize(lenA, '0');
	}
	int ia = -1, carryNum = 0;
	//反转后左边是低位
	int i = -1;
	for (Utility::toSignedNum(lenAB - 1, i); i >= 0; --i) {
		// int sumBit = (topLowNumA[i] - '0') + (topLowNumB[i] - '0') + carryNum;
		int sumBit = StandardExtend::toRadixIntNum(topLowNumA[i], radix) + StandardExtend::toRadixIntNum(topLowNumB[i], radix) + carryNum;
		// topLowSum[i] = sumBit % 10 + '0';
		topLowSum[i] = StandardExtend::toUppercaseAscllChar(sumBit % radix);
		// carryNum = sumBit / 10;
		carryNum = sumBit / radix;
	}
	return carryNum == 0 ? topLowSum : (topLowSum = "1" + topLowSum);
}

// 计算二进制补码 (十进制数字)
void calcBinaryCode(int decNum, std::string &originCode, std::string &inverseCode, std::string &complementCode) {
	std::vector<int> lowTop;
	std::vector<int> topLow;
	lowTop.resize(MathExtend::calcDigitTotalSize(decNum, 2));
	topLow.resize(MathExtend::calcDigitTotalSize(decNum, 2));
	puts("原码");
	// solution0
	int totalSize = BinaryTransition::decimalToRadixLowTop(decNum, lowTop.begin(), 2);
	BinaryTransition::outputDigitInRange(lowTop.rbegin(), lowTop.rend());
	// solution1
	BinaryTransition::decimalToRadixTopLow(decNum, topLow.end(), 2);
	originCode = TransitionUtility::digitArrayToString(topLow.begin(), topLow.end());

	puts("反码");
	// solution0
	BinaryTransition::inverseCode(lowTop.begin(), totalSize);
	BinaryTransition::outputDigitArrayLowTop(lowTop, totalSize);
	// solution1
	BinaryTransition::inverseCode(topLow.begin(), totalSize);
	inverseCode = TransitionUtility::digitArrayToString(topLow.begin(), topLow.end());

	// puts("补码");
	complementCode = TransitionUtility::digitArrayToString(lowTop.rbegin(), lowTop.rend());
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
	calcBinaryCode(6, originCode, inverseCode, complementCode);
	calcBinaryCode(2147483647, originCode, inverseCode, complementCode);
	StandardExtend::testAndOut(std::string("原码"), originCode, std::string("1111111111111111111111111111111"));
	StandardExtend::testAndOut(std::string("反码"), inverseCode, std::string("0000000000000000000000000000000"));
	StandardExtend::testAndOut(std::string("补码"), complementCode, std::string("0000000000000000000000000000001"));

	std::cout << "进制转换器" << std::endl;
	std::vector<int> targetDigitList;
	std::string targetDigitStr;

	BinaryTransition binRadixTransiter = BinaryTransition(originCode.length(), 1, 1);
	binRadixTransiter.transition(originCode.c_str(), targetDigitList);
	targetDigitStr = TransitionUtility::digitArrayToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("BIN->BIN"), targetDigitStr, originCode);

	binRadixTransiter.reset(2, 2);
	//BinaryTransition binTransOct = BinaryTransition(originCode.length(), 1, 3);
	binRadixTransiter.reset(1, 3);
	binRadixTransiter.transition(originCode.c_str(), targetDigitList);
	//binTransOct.transition(originCode.c_str(), targetDigitList);
	targetDigitStr = TransitionUtility::digitArrayToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("BIN->OCT"), targetDigitStr, std::string("17777777777"));
	//BinaryTransition octTransBin = BinaryTransition(originCode.length(), 3, 1);
	//octTransBin.transition(targetDigitStr.c_str(), targetDigitList);
	binRadixTransiter.reset(3, 1);
	binRadixTransiter.transition(targetDigitStr.c_str(), targetDigitList);
	targetDigitStr = TransitionUtility::digitArrayToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("OCT->BIN"), targetDigitStr, originCode);

	//BinaryTransition binTransHex = BinaryTransition(originCode.length(), 1, 4);
	//binTransHex.transition(originCode.c_str(), targetDigitList);
	binRadixTransiter.reset(1, 4);
	binRadixTransiter.transition(originCode.c_str(), targetDigitList);
	targetDigitStr = TransitionUtility::digitArrayToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("BIN->HEX"), targetDigitStr, std::string("7FFFFFFF"));
	BinaryTransition hexTransBin = BinaryTransition(originCode.length(), 4, 1);
	//hexTransBin.transition(targetDigitStr.c_str(), targetDigitList);
	binRadixTransiter.reset(4, 1);
	binRadixTransiter.transition(targetDigitStr.c_str(), targetDigitList);
	targetDigitStr = TransitionUtility::digitArrayToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("HEX->BIN"), targetDigitStr, originCode);

	// 2^31D
	binRadixTransiter.reset(4, 1);
	binRadixTransiter.transition("80000000", targetDigitList);
	targetDigitStr = TransitionUtility::digitArrayToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("HEX->BIN"), targetDigitStr, std::string("10000000000000000000000000000000"));

	binRadixTransiter.reset(4, 3);
	binRadixTransiter.transition("39", targetDigitList);
	targetDigitStr = TransitionUtility::digitArrayToString(targetDigitList.begin(), targetDigitList.end());
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
	// 按字节对内存块进行初始化(单字节字符型是完全可以的) PS 这可不是MAX_INT32
	// 8bit 7FH: 0111-1111B ==> 4个一样的Byte => 2139062143D
	// 负数是以补码存储的 若想初始化一个负数最小值就至少需要符号位为1
	// -2147483647真
	// 1-111-1111 1111-1111 1111-1111 1111-1111原
	// 1-000-0000 0000-0000 0000-0000 0000-0000反
	// 1-000-0000 0000-0000 0000-0000 0000-0001补 <==> 没有规律, 不能按字节初始化
	// 退一步
	// 1-111-1111 0111-1111 0111-1111 1000-0000原 <==> -2139062144
	// 1-000-0000 1000-0000 1000-0000 0111-1111反
	// 1-000-0000 1000-0000 1000-0000 1000-0000补 ->80H
	int arrayS[20];
	memset(arrayS, 0x80, 20*sizeof(int));
	memset(arrayS, 0x7f, 20*sizeof(int));
}

#include "./memory/Hash/HashTable.h"
int mainForHashTable() {
	// 两个cpp文件可以存在同名类型(.h是肯定不行), 但不能存在同样定义或是无法区分重载的方法
	// (有可能会出现编译通过但DBUG类型混乱, 可能需要重新生成一下)
	HashTableC::HashTableLiPtr h = HashTableC::CreateLinkedListHashTable(5);
	srand(clock());
	// 由于没有处理散列冲突 无法添加多个同Hash值的Key 故只能添加20个元素
	for (int i = 0; i < 20; ++i) {
		Insert(h, std::to_string(i).c_str());
	}
	printf("%s", Find(h, "3")->Data);
	return 0;
}
