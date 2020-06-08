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
#include "./utility/TransitionUtility.h"
#include "./utility/Transition.h"
// #include "./else/BigInteger.h"
// using namespace std;

// ���еľ�̬����->�����ռ���ͨ����(��������java��ֲ�Ļ�)


#include"./memory/ArrayLinearList.h"
int mainForLinearList() {
	LinearList::mainForArrayLinearList();
	return 0;
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
