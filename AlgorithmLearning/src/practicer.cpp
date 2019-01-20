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
#include "./else/TransitionUtility.h"
#include "./else/Transition.h"
// #include "./else/BigInteger.h"
// using namespace std;

// 类中的静态方法->命名空间普通方法(若不考虑java移植的话)

// 计算二进制补码 (十进制数字)
void calcBinaryCode(int decNum, std::string &originCode, std::string &inverseCode, std::string &complementCode) {
	std::vector<int> lowTop;
	std::vector<int> topLow;
	lowTop.resize(MathExtend::calcDigitTotalSize(decNum, 2));
	topLow.resize(MathExtend::calcDigitTotalSize(decNum, 2));
	puts("原码");
	// solution0
	int totalSize = TransitionUtility::decimalToRadixLowTopBase(decNum, lowTop.begin(), 2);
	TransitionUtility::outputDigitInRange(lowTop.rbegin(), lowTop.rend());
	// solution1
	TransitionUtility::decimalToRadixTopLow(decNum, topLow.begin(), topLow.end(), 2);
	originCode = TransitionUtility::digitContainerToString(topLow.begin(), topLow.end());

	puts("反码");
	// solution0
	TransitionUtility::inverseCode(lowTop.begin(), lowTop.begin() + totalSize);
	TransitionUtility::outputDigitInRange(lowTop.begin(), lowTop.begin() + totalSize);
	// solution1
	TransitionUtility::inverseCode(topLow.begin(), topLow.end());
	inverseCode = TransitionUtility::digitContainerToString(topLow.begin(), topLow.end());

	// puts("补码");
	complementCode = TransitionUtility::digitContainerToString(lowTop.rbegin(), lowTop.rend());
	TransitionUtility::bigPlush(complementCode, std::string("1"), complementCode, 2);
	// std::cout << complementCode << std::endl;
}

void radixTransTestFun(int originRadix, int targetRadix, std::vector<int> &originDigitList, std::string const &name, std::string const &realRrsult) {
	static DecimalTransition deRadixTransiter(originDigitList.size(), originRadix, targetRadix);
	static std::vector<int> targetDigitList;
	static std::string targetDigitStr;
	deRadixTransiter.reset(originRadix, targetRadix);
	auto topIter = deRadixTransiter.transition(originDigitList.begin(), originDigitList.end(), targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(topIter, targetDigitList.end());
	StandardExtend::testAndOut(name, targetDigitStr, realRrsult);
}

void radixTransTestFunRe(char const *originStr, int originRadix, int targetRadix, char const *targetStr) {
	static std::vector<int> bufferDigitList;
	TransitionUtility::stringToDigitArray(originStr, bufferDigitList);
	radixTransTestFun(originRadix, targetRadix, bufferDigitList
		, std::to_string(originRadix) + std::string("->") + std::to_string(targetRadix), targetStr);

	TransitionUtility::stringToDigitArray(targetStr, bufferDigitList);
	radixTransTestFun(targetRadix, originRadix, bufferDigitList
		, std::to_string(targetRadix) + std::string("->") + std::to_string(originRadix), std::string(originStr));
}

int main(){
	/*
	BigInteger b = BigInteger(35);
	b.print();
	*/
	StandardExtend::testAndOut(std::string("数字->字符{字母, 数字}"), TransitionUtility::toAlphOrAscllNum(0), '0');
	StandardExtend::testAndOut(std::string("数字->字符{字母, 数字}"), TransitionUtility::toAlphOrAscllNum(9), '9');
	StandardExtend::testAndOut(std::string("数字->字符{字母, 数字}"), TransitionUtility::toAlphOrAscllNum(10), 'A');
	StandardExtend::testAndOut(std::string("数字->字符{字母, 数字}"), TransitionUtility::toAlphOrAscllNum(35), 'Z');
	StandardExtend::testAndOut(std::string("数字->字符{字母, 数字}"), TransitionUtility::toAlphOrAscllNum(36), 'a');
	StandardExtend::testAndOut(std::string("数字->字符{字母, 数字}"), TransitionUtility::toAlphOrAscllNum(61), 'z');
	StandardExtend::testAndOut(std::string("字符{字母, 数字}->数字"), TransitionUtility::toIntNum('0'), 0);
	StandardExtend::testAndOut(std::string("字符{字母, 数字}->数字"), TransitionUtility::toIntNum('9'), 9);
	StandardExtend::testAndOut(std::string("字符{字母, 数字}->数字"), TransitionUtility::toIntNum('A'), 10);
	StandardExtend::testAndOut(std::string("字符{字母, 数字}->数字"), TransitionUtility::toIntNum('Z'), 35);
	StandardExtend::testAndOut(std::string("字符{字母, 数字}->数字"), TransitionUtility::toIntNum('a'), 36);
	StandardExtend::testAndOut(std::string("字符{字母, 数字}->数字"), TransitionUtility::toIntNum('z'), 61);
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

	// 源进制16的1个数字位(最大值15H->15D->17O->1111B)
	// 需要 最少2个10进制数字位; 最少2个8进制数字位; 最少4个2进制比特位
	/*
	[3H, 9H]->[48D, 9D]->[60O, 11O]->[53(9), 10(9)]->[120(6), 13(6)]
	   39H        57D        71O           63(9)           133(6)
	*/
	char originStr[10] = "39";
	std::vector<int> bufferDigitList;
	bufferDigitList.resize(2);
	TransitionUtility::charContainerToDigitContainer(originStr, originStr + 2, bufferDigitList.begin(), bufferDigitList.end());
	radixTransTestFun(16, 2, bufferDigitList, std::string("HEX->BIN"), std::string("111001"));
	radixTransTestFun(16, 8, bufferDigitList, std::string("HEX->OCT"), std::string("71"));
	radixTransTestFun(16, 9, bufferDigitList, std::string("HEX->9"), std::string("63"));
	radixTransTestFun(16, 10, bufferDigitList, std::string("HEX->DEC"), std::string("57"));
	radixTransTestFun(16, 6, bufferDigitList, std::string("HEX->6"), std::string("133"));
	radixTransTestFunRe(originStr, 16, 6, "133");
	radixTransTestFunRe(originStr, 16, 20, "2H");

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
