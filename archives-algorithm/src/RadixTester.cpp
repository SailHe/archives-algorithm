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


// 计算二进制补码 (十进制数字)
void calcBinaryCode(int decNum, std::string &originCode, std::string &inverseCode, std::string &complementCode) {
	std::string resultBuffer = TransitionUtility::calcComplementCode(decNum);

	std::vector<int> topLow;
	topLow.resize(MathExtend::calcDigitTotalSize(decNum, 2));
	TransitionUtility::decimalToRadixTopLow(decNum, topLow.begin(), topLow.end(), 2);
	originCode = TransitionUtility::digitContainerToString(topLow.begin(), topLow.end());

	std::string resultBuffer1 = TransitionUtility::calcComplementCode(topLow.begin(), topLow.end());

	TransitionUtility::inverseCode(topLow.begin(), topLow.end());
	inverseCode = TransitionUtility::digitContainerToString(topLow.begin(), topLow.end());

	complementCode = TransitionUtility::digitContainerToString(topLow.begin(), topLow.end());
	std::string one("1");
	TransitionUtility::bigPlush(complementCode, one, complementCode, 2);
	_ASSERT_EXPR(complementCode == resultBuffer, "rt");
	_ASSERT_EXPR(complementCode == resultBuffer1, "rt");
}

void testBinaryCode(int decValue, std::string const &realOriginCode
	, std::string const &realInverseCode, std::string const &realComplementCode) {
	std::string originCode;
	std::string inverseCode;
	std::string complementCode;
	calcBinaryCode(decValue, originCode, inverseCode, complementCode);
	StandardExtend::testAndOut(std::string("原码"), originCode, realOriginCode);
	StandardExtend::testAndOut(std::string("反码"), inverseCode, realInverseCode);
	StandardExtend::testAndOut(std::string("补码"), complementCode, realComplementCode);
}

// 单向转换测试
void binRadixTransUnidirectionTestFun(int originBits, int targetBits, std::vector<int> &originDigitList, std::string const &name, std::string const &realRrsult) {
	static BinaryTransition binRadixTransiter(originDigitList.size(), originBits, targetBits);
	static std::vector<int> targetDigitList;
	static std::string targetDigitStr;
	binRadixTransiter.reset(originBits, targetBits);
	binRadixTransiter.transition(originDigitList.begin(), originDigitList.end(), targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(name, targetDigitStr, realRrsult);
}
// 双向转换测试
void binTransBothwayTestFun(char const *originStr, int originBits_, int targetBits_, char const *targetStr) {
	static std::vector<int> bufferDigitList;
	int originRadix_ = (int)pow(2.0, originBits_), targetRadix_ = (int)pow(2.0, targetBits_);
	size_t len = strlen(originStr);
	bufferDigitList.resize(len);
	// 使用迭代器而不是传入整个容器 可以使方法具有读写内容权限的同时 防止方法内部修改容器的大小
	auto digitIterEnd = TransitionUtility::stringToDigitArray(originStr, bufferDigitList.begin(), originRadix_);
	_ASSERT(digitIterEnd == bufferDigitList.end());
	binRadixTransUnidirectionTestFun(originBits_, targetBits_, bufferDigitList
		, std::to_string(originRadix_) + std::string("->") + std::to_string(targetRadix_), targetStr);

	len = strlen(targetStr);
	bufferDigitList.resize(len);
	digitIterEnd = TransitionUtility::stringToDigitArray(targetStr, bufferDigitList.begin(), targetRadix_);
	_ASSERT(digitIterEnd == bufferDigitList.end());
	// 约定正确结果targetStr是最简的, 如果传入的originStr含有前导0的话就需要去除(方法返回值也是最简的)
	binRadixTransUnidirectionTestFun(targetBits_, originBits_, bufferDigitList
		, std::to_string(targetRadix_) + std::string("->") + std::to_string(originRadix_), TransitionUtility::formatString(std::string(originStr)));
}

void decRadixTransUnidirectionTestFun(int originRadix, int targetRadix, std::vector<int> &originDigitList, std::string const &name, std::string const &realRrsult) {
	static DecimalTransition deRadixTransiter(originDigitList.size(), originRadix, targetRadix);
	static std::vector<int> targetDigitList;
	static std::string targetDigitStr;
	deRadixTransiter.reset(originRadix, targetRadix);
	auto topIter = deRadixTransiter.transition(originDigitList.begin(), originDigitList.end(), targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(topIter, targetDigitList.end());
	StandardExtend::testAndOut(name, targetDigitStr, realRrsult);
}

// 双向转换测试
void decTransBothwayTestFun(char const *originStr, int originRadix_, int targetRadix_, char const *targetStr) {
	static std::vector<int> bufferDigitList;
	size_t len = strlen(originStr);
	bufferDigitList.resize(len);
	auto digitIterEnd = TransitionUtility::stringToDigitArray(originStr, bufferDigitList.begin(), originRadix_);
	_ASSERT(digitIterEnd == bufferDigitList.end());
	decRadixTransUnidirectionTestFun(originRadix_, targetRadix_, bufferDigitList
		, std::to_string(originRadix_) + std::string("->") + std::to_string(targetRadix_), targetStr);

	len = strlen(targetStr);
	bufferDigitList.resize(len);
	digitIterEnd = TransitionUtility::stringToDigitArray(targetStr, bufferDigitList.begin(), targetRadix_);
	_ASSERT(digitIterEnd == bufferDigitList.end());
	// 约定正确结果targetStr是最简的, 如果传入的originStr含有前导0的话就需要去除(方法返回值也是最简的)
	decRadixTransUnidirectionTestFun(targetRadix_, originRadix_, bufferDigitList
		, std::to_string(targetRadix_) + std::string("->") + std::to_string(originRadix_), TransitionUtility::formatString(std::string(originStr)));
}

void functionalTest() {
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
}

void bigIntegerTest() {
	std::string lhs = "7860000053";
	std::string rhs = std::to_string(MAX_INT32);

	std::string sum = "";
	TransitionUtility::bigPlush(lhs, rhs, sum, 10);
	StandardExtend::testAndOut("字符串大数加法: 进最高位", sum, std::string("10007483700"));

	std::string lhsTmp = lhs;
	std::string &sumRefLhs = lhsTmp;
	TransitionUtility::bigPlush(lhsTmp, rhs, sumRefLhs, 10);
	StandardExtend::testAndOut("字符串大数加法: sum引用lhs", sumRefLhs, std::string("10007483700"));

	std::string const lhsTmp1 = lhs;
	TransitionUtility::bigPlush(lhsTmp1, rhs, sum, 10);
	StandardExtend::testAndOut("const字符串大数加法: 进最高位", sum, std::string("10007483700"));

	lhsTmp = lhs;
	std::string const rhsTmp = rhs;
	std::string &sumRefLhs1 = lhsTmp;
	TransitionUtility::bigPlush(lhs, rhsTmp, sumRefLhs1, 10);
	StandardExtend::testAndOut("const字符串大数加法: sum引用rhs", sumRefLhs1, std::string("10007483700"));
}

int runRadixTest() {
	/*
	BigInteger b = BigInteger(35);
	b.print();

	obj next(): 迭代并返回当前迭代器中的对象
	boolean hasNext(): 是否还有下一个
	void remove(): 删除当前的
	std::list<int> li;li.end() -  li.begin();
	std::list<int> tmpL;
	tmpL.push_back(1);
	tmpL.push_back(0);
	std::reverse(tmpL.begin(), tmpL.end());
	*/
	functionalTest();

	testBinaryCode(6
		, std::string("110")
		, std::string("001")
		, std::string("010")
	);
	std::string realOriginCode("1111111111111111111111111111111");
	testBinaryCode(2147483647
		, realOriginCode
		, std::string("0000000000000000000000000000000")
		, std::string("0000000000000000000000000000001")
	);

	std::cout << "进制转换器" << std::endl;
	std::vector<int> targetDigitList;
	std::string targetDigitStr;
	BinaryTransition binRadixTransiter(0, 4, 1);
	targetDigitList.resize(100);
	auto digitIterEnd = TransitionUtility::stringToDigitArray("80000000", targetDigitList.begin(), 16);
	binRadixTransiter.transition(targetDigitList.begin(), digitIterEnd, targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("HEX->BIN"), targetDigitStr, std::string("10000000000000000000000000000000"));

	// 前导0测试
	binTransBothwayTestFun("0010", 1, 1, "10");
	binTransBothwayTestFun("000010", 1, 2, "2");

	std::vector<int> tmp;
	tmp.resize(MathExtend::calcDigitTransiTotalSize(10, 2147483647, 32));
	/*
	auto &a = tmp.begin();
	a = tmp.end();
	a = tmp.begin();
	// unique_ptr
	std::shared_ptr<int> pa(&a);
	auto b = pa;
	*/
	//int realTotalSize = TransitionUtility::decimalToRadixLowTopBase(2147483647, tmp.begin(), 32);
	//realTotalSize = TransitionUtility::decimalToRadixTopLow_PreDel(2147483647, tmp.begin(), tmp.end(), 32);
	//std::string tmpStr = TransitionUtility::digitContainerToString(tmp.rbegin() + tmp.size() - realTotalSize, tmp.rend());
	binTransBothwayTestFun("1VVVVVV", 5, 1, realOriginCode.c_str());
	binTransBothwayTestFun(realOriginCode.c_str(), 1, 5, "1VVVVVV");
	binTransBothwayTestFun(realOriginCode.c_str(), 1, 3, "17777777777");
	binTransBothwayTestFun(realOriginCode.c_str(), 1, 4, "7FFFFFFF");
	// 2^31D
	binTransBothwayTestFun("80000000", 4, 1, "10000000000000000000000000000000");
	binTransBothwayTestFun("39", 4, 3, "71");

	// 源进制16的1个数字位(最大值15H->15D->17O->1111B)
	// 需要 最少2个10进制数字位; 最少2个8进制数字位; 最少4个2进制比特位
	/*
	[3H, 9H]->[48D, 9D]->[60O, 11O]->[53(9), 10(9)]->[120(6), 13(6)]
	   39H        57D        71O           63(9)           133(6)
	*/
	char originStr[10] = "39";
	decTransBothwayTestFun(originStr, 16, 8, "71");
	decTransBothwayTestFun(originStr, 16, 9, "63");
	decTransBothwayTestFun(originStr, 16, 10, "57");
	decTransBothwayTestFun(originStr, 16, 6, "133");
	decTransBothwayTestFun(originStr, 16, 20, "2H");
	decTransBothwayTestFun(originStr, 16, 2, "111001");
	decTransBothwayTestFun("80000000", 16, 2, "10000000000000000000000000000000");
	decTransBothwayTestFun("FFFFFFFF", 16, 2, "11111111111111111111111111111111");
	decTransBothwayTestFun("1", 16, 2, "1");
	decTransBothwayTestFun("2", 16, 2, "10");

	bigIntegerTest();

	return 0;
}
