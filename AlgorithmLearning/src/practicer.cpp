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
	printf("补码complementCode计算\n");
	int onum;
	while (~scanf("%d", &onum)){
		BinaryTransition oBinCode = BinaryTransition(100);
		vector<int> lowTop;
		lowTop.resize(30);
		//原码
		int totalBit = BinaryTransition::decimalToRadixLowTop(onum, lowTop.begin(), 2);
		oBinCode.outputWithSymbol(lowTop, 7);

		//反码
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
	// 按字节对内存块进行初始化(单字节字符型是完全可以的) PS 这可不是MAX_INT32
	// 8bit 7FH: 0111-1111B ==> 4个一样的Byte => 2139062143D
	// 负数是以补码存储的 若想初始化一个负数最小值就至少需要符号位为1
	// -2147483647真
	// 1-111-1111 1111-1111 1111-1111 1111-1111原
	// 1-000-0000 0000-0000 0000-0000 0000-0000反
	// 1-000-0000 0000-0000 0000-0000 0000-0001补 <==> 没有规律, 不能按字节初始化
	// 退一步
	// 1-000-0000 1000-0000 1000-0000 1000-0000补 ->80H
	// 1-000-0000 1000-0000 1000-0000 0111-1111反
	// 1-111-1111 0111-1111 0111-1111 1000-0000原 <==> -2139062144
	int arrayS[20];
	memset(arrayS, 0x80, 20*sizeof(int));
	memset(arrayS, 0x7f, 20*sizeof(int));
}

#include "./memory/Hash/HashTable.h"
int main() {
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
