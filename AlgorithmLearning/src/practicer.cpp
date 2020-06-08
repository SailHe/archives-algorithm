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

// 类中的静态方法->命名空间普通方法(若不考虑java移植的话)


#include"./memory/ArrayLinearList.h"
int mainForLinearList() {
	LinearList::mainForArrayLinearList();
	return 0;
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


template<class T> using ArrayList = std::vector<T>;
template<class T> using LinkedList = std::list<T>;
using SizeType = size_t;
using Sub = int;
using I64 = long long;

// 方向枚举
enum DIRECTION_ENUM {
	UN_KNOEN_DIR, UP_DIR, DOWM_DIR, LEFT_DIR, RIGHT_DIR, LEFT_UP_DIR, RIGHT_UP_DIR
};

// 数据库操作枚举
enum OPTION_CRUD_ENUM {
	// 创建, 检索, 更新, 删除, 未定义
	// insert/save, query/find/search, edit/change, remove/erase
	CREATE_OPT, RETRIEVE_OPT, UPDATE_OPT, DELETE_OPT, UN_DEFINDE_OPT
};

// 数据操作枚举
enum OPTION_DATA_ENUM {
	// 保持不变, 替换, 删除, 插入, 未定义 (优先级按先后排列 以前面的越优)
	KEEP_DATA_OPT, REPLACE_DATA_OPT, DELETE_DATA_OPT, INSERT_DATA_OPT, UN_DEFINDE_DATA_OPT
};

// 任意进制之间互相转换(使用了二进制转换器类: BinaryTransition) 基于10进制
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
