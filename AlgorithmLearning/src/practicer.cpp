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

// ���еľ�̬����->�����ռ���ͨ����(��������java��ֲ�Ļ�)

// ��������Ʋ��� (ʮ��������)
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
	TransitionUtility::bigPlush(complementCode, std::string("1"), complementCode, 2);
	_ASSERT_EXPR(complementCode == resultBuffer, "rt");
	_ASSERT_EXPR(complementCode == resultBuffer1, "rt");
}

void testBinaryCode(int decValue, std::string const &realOriginCode
	, std::string const &realInverseCode, std::string const &realComplementCode) {
	std::string originCode;
	std::string inverseCode;
	std::string complementCode;
	calcBinaryCode(decValue, originCode, inverseCode, complementCode);
	StandardExtend::testAndOut(std::string("ԭ��"), originCode, realOriginCode);
	StandardExtend::testAndOut(std::string("����"), inverseCode, realInverseCode);
	StandardExtend::testAndOut(std::string("����"), complementCode, realComplementCode);
}

// ����ת������
void binRadixTransUnidirectionTestFun(int originBits, int targetBits, std::vector<int> &originDigitList, std::string const &name, std::string const &realRrsult) {
	static BinaryTransition binRadixTransiter(originDigitList.size(), originBits, targetBits);
	static std::vector<int> targetDigitList;
	static std::string targetDigitStr;
	binRadixTransiter.reset(originBits, targetBits);
	binRadixTransiter.transition(originDigitList.begin(), originDigitList.end(), targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(name, targetDigitStr, realRrsult);
}
// ˫��ת������
void binTransBothwayTestFun(char const *originStr, int originBits_, int targetBits_, char const *targetStr) {
	static std::vector<int> bufferDigitList;
	int originRadix_ = (int)pow(2.0, originBits_), targetRadix_ = (int)pow(2.0, targetBits_);
	size_t len = strlen(originStr);
	bufferDigitList.resize(len);
	// ʹ�õ����������Ǵ����������� ����ʹ�������ж�д����Ȩ�޵�ͬʱ ��ֹ�����ڲ��޸������Ĵ�С
	auto digitIterEnd = TransitionUtility::stringToDigitArray(originStr, bufferDigitList.begin(), originRadix_);
	_ASSERT(digitIterEnd == bufferDigitList.end());
	binRadixTransUnidirectionTestFun(originBits_, targetBits_, bufferDigitList
		, std::to_string(originRadix_) + std::string("->") + std::to_string(targetRadix_), targetStr);

	len = strlen(targetStr);
	bufferDigitList.resize(len);
	digitIterEnd = TransitionUtility::stringToDigitArray(targetStr, bufferDigitList.begin(), targetRadix_);
	_ASSERT(digitIterEnd == bufferDigitList.end());
	// Լ����ȷ���targetStr������, ��������originStr����ǰ��0�Ļ�����Ҫȥ��(��������ֵҲ������)
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

// ˫��ת������
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
	// Լ����ȷ���targetStr������, ��������originStr����ǰ��0�Ļ�����Ҫȥ��(��������ֵҲ������)
	decRadixTransUnidirectionTestFun(targetRadix_, originRadix_, bufferDigitList
		, std::to_string(targetRadix_) + std::string("->") + std::to_string(originRadix_), TransitionUtility::formatString(std::string(originStr)));
}

int main(){
	// obj next(): ���������ص�ǰ�������еĶ���
	// boolean hasNext(): �Ƿ�����һ��
	// void remove(): ɾ����ǰ��
	//std::list<int> li;li.end() -  li.begin();
	/*
	BigInteger b = BigInteger(35);
	b.print();

	std::list<int> tmpL;
	tmpL.push_back(1);
	tmpL.push_back(0);
	std::reverse(tmpL.begin(), tmpL.end());
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

	std::cout << "����ת����" << std::endl;
	std::vector<int> targetDigitList;
	std::string targetDigitStr;
	BinaryTransition binRadixTransiter(0, 4, 1);
	targetDigitList.resize(100);
	auto digitIterEnd = TransitionUtility::stringToDigitArray("80000000", targetDigitList.begin(), 16);
	binRadixTransiter.transition(targetDigitList.begin(), digitIterEnd, targetDigitList);
	targetDigitStr = TransitionUtility::digitContainerToString(targetDigitList.begin(), targetDigitList.end());
	StandardExtend::testAndOut(std::string("HEX->BIN"), targetDigitStr, std::string("10000000000000000000000000000000"));

	// ǰ��0����
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

	// Դ����16��1������λ(���ֵ15H->15D->17O->1111B)
	// ��Ҫ ����2��10��������λ; ����2��8��������λ; ����4��2���Ʊ���λ
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


template<class T> using ArrayList = std::vector<T>;
template<class T> using LinkedList = std::list<T>;
using SizeType = size_t;
using Sub = int;
using I64 = long long;

// ����ö��
enum DIRECTION_ENUM {
	UN_KNOEN_DIR, UP_DIR, DOWM_DIR, LEFT_DIR, RIGHT_DIR, LEFT_UP_DIR, RIGHT_UP_DIR
};

// ���ݿ����ö��
enum OPTION_CRUD_ENUM {
	// ����, ����, ����, ɾ��, δ����
	// insert/save, query/find/search, edit/change, remove/erase
	CREATE_OPT, RETRIEVE_OPT, UPDATE_OPT, DELETE_OPT, UN_DEFINDE_OPT
};

// ���ݲ���ö��
enum OPTION_DATA_ENUM {
	// ���ֲ���, �滻, ɾ��, ����, δ���� (���ȼ����Ⱥ����� ��ǰ���Խ��)
	KEEP_DATA_OPT, REPLACE_DATA_OPT, DELETE_DATA_OPT, INSERT_DATA_OPT, UN_DEFINDE_DATA_OPT
};

// �������֮�以��ת��(ʹ���˶�����ת������: BinaryTransition) ����10����
// itoa
// A not solved
// @see http://139.196.145.92/contest_show.php?cid=720#problem/B
/*
input
2 9
1100

16 7
4CD

10 36
46

output:
13
3404
1A
*/
int mainForAgoj19_1_2SolveB() {
	int originRadix, targetRadix;
	std::string origin;
	origin.resize(20);
	std::vector<int> digitTopLow;
	digitTopLow.resize(20);
	while (std::cin >> originRadix >> targetRadix) {
		std::cin >> origin;
		auto originDigitEnd = TransitionUtility::charContainerToDigitContainer(origin.begin(), origin.end(), digitTopLow.begin(), originRadix);
		// Radix -> DEC
		int decValueNum = TransitionUtility::radixTopLowToDecimal(digitTopLow.begin(), originDigitEnd, originRadix);
		// DEC -> Radix
		auto realLeftIter = TransitionUtility::decimalToRadixTopLow(decValueNum, digitTopLow.end(), targetRadix);
		// output
		TransitionUtility::outputDigitInRange(realLeftIter, digitTopLow.end());
	}
	return 0;
}
