#include<stdio.h>
#include<string.h>
#include<string>
#include<cmath>
#include<vector>
#include<iostream>
#include<queue>
#include<functional>//greater
#include<list>
#include<stack>
#include<map>
#include<iomanip>//cout各种操纵器
#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

//#define MAX_N 1000+5
#define MAX_INT_NUM 2147483647
#define MIN_INT_NUM (-MAX_INT_NUM-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>
using namespace std;

template<class T> using ArrayList = vector<T>;
using SizeType = size_t;
using I64 = long long;

//二进制转换器类(支持 可用2进制表示的任意进制的大数的单向转换) 符号需要用户自己处理
class BinaryTransition {
	//转换双方的储存位: 一位至少需要多少位二进制表示
	int originBitLeast = 1;
	int originRadix = (int)pow((double)2, originBitLeast);
	int targetBitLeast = 1;
	typedef vector<int> Varray;
	typedef Varray::iterator VarrayIterator;
	Varray repository;// = NULL
	VarrayIterator binNumberPointer;// = NULL
	VarrayIterator currentBinNumberPointer;// = NULL
public:
	
	//将数字转换正数(正数仍是正数) 返回该数字的正负符号
	static char toAbs(int &number) {
		return number < 0 ? (number = -number), '-' : '+';
	}

	//数字->数组 (返回位数 以及 repositoryLowToTop低位向高位存储的数值储存迭代器)
	static int tenToRadix(unsigned number, VarrayIterator repositoryLowToTop, int radix) {
		static int bit;
		bit = -1;
		//要保证即使传入0也能执行一次
		do {
			repositoryLowToTop[++bit] = number % radix;
			number /= radix;
		} while (number != 0);
		return bit + 1;
	}

};


/*不支持产生借位, 变号的负数运算*/
class BigInteger {
	//表示数字位元的进制
	//@Bug 2进制时会报错
	const static int radix = 10000;
	const static int ONE = 1;
	const static int ZERO = 0;
	//数字位元的类型(必须能够存下两个进制最大值相乘的值)
	typedef int BitType;
	typedef vector<BitType> Base;
	//正负号标识 输出时正号不输出
	char symbol = 0;
	//储存数字位元 的数组(为了支持数字的直接构造这样存比较方便 对字符串构造影响不大)
	vector<BitType> digitLowTop;

	//返回内置数字类型的总位数 0: 1bit; 10: 2bit
	static int totalBitOf(BitType originNumber) {
		int totalBit = 0;
		do {
			originNumber /= 10;
			++totalBit;
		} while (originNumber != 0);
		return totalBit;
	}

	//设置这个大数的储存位数 (grow:只增长(advance only) 小于原位数时不更新大小)
	void setTotalBit(size_t totalBit, bool grow = true) {
		//若已知该数字的大概位数 可以提前指定 减少内存分配
		if (grow && totalBit <= digitLowTop.size())
			return;
		digitLowTop.resize(totalBit);
	}
	//将源数字转换为本地格式储存
	void transitionToLocalRadix(int originNumber) {
		int numTotalBit = totalBitOf(originNumber);
		//一个字的位数: 储存位(字位)比进制的总位数少一
		int wordBit = totalBitOf(radix) - 1;
		//当传入数字无法用整字储存时需要多用一个字来储存(即溢出位)
		setTotalBit(numTotalBit / wordBit + (numTotalBit % wordBit == 0 ? 0 : 1));
		symbol = BinaryTransition::toAbs(originNumber);
		BinaryTransition::tenToRadix(originNumber, digitLowTop.begin(), radix);
		/*int i = -1;
		do{
			digitLowTop[++i] = originNumber % radix;
			originNumber /= radix;
		} while (originNumber != 0);*/
	}

public:
	void print() {
		const char map[17] = "0123456789ABCDEF";
		int totalBit = digitLowTop.size();
		printf(symbol == '-' ? "-" : "");
		printf("%d", digitLowTop[totalBit - 1]);
		for (int i = 1; i < totalBit; i++) {
			printf("%04d", digitLowTop[totalBit - i - 1]);
		}
		puts("");
	}

	BigInteger(BitType originNumber) {
		transitionToLocalRadix(originNumber);
	}

	//乘法前最好先分配一下
	void reserve(size_t newSize) {
		digitLowTop.reserve(newSize);
	}

	//<==> += 
	BigInteger &plus(BigInteger const &rhs) {
		BitType subLhs, subRhs, subSum;
		//右手边对应位的值 进位 addValue(加数) = rhsBit + carryBit
		BitType rhsBit, carryBit = 0;
		//字数
		int wordCntLhs = digitLowTop.size(), wordCntRhs = rhs.digitLowTop.size();

		for (subSum = subLhs = subRhs = 0; subLhs < wordCntLhs || subRhs < wordCntRhs || carryBit != 0; ++subSum) {
			//计算加数 若rhs读取完毕还可能存在进位数 此时加数置为0就好
			rhsBit = subRhs < wordCntRhs ? rhs.digitLowTop[subLhs] : 0;

			if (subLhs < wordCntLhs) {
				digitLowTop[subSum] += rhsBit + carryBit;
			}
			else {
				//若两者的字数lhs < rhs的话进位是0但是addvalue不是0
				digitLowTop.push_back(rhsBit + carryBit);
			}

			carryBit = digitLowTop[subSum] / radix;
			digitLowTop[subSum] %= radix;
			++subLhs, ++subRhs;
		}
		return *this;
	}
	//快速乘法 (乘数位 <= 10000) 由于push_back的存在不开优化必定比不上直接写得算法 开了优化虽仍要慢一点 但好在此类功能齐全
	BigInteger &muity(BitType muityBit) {
		//int units = 1计量单位: 表示muityBit的一单位代表多大
		BitType carryBit = 0;
		BitType subProduct;
		int totalBit = digitLowTop.size();
		for (subProduct = 0; subProduct < totalBit || carryBit; subProduct++) {
			if (subProduct < totalBit)
				digitLowTop[subProduct] = digitLowTop[subProduct] * muityBit + carryBit;
			else {
				//位乘法只可能是进位
				digitLowTop.push_back(carryBit);
			}
			carryBit = digitLowTop[subProduct] / radix;
			digitLowTop[subProduct] %= radix;
		}
		return *this;
	}
	
	BigInteger operator+(BigInteger const &rhs) {
		return BigInteger(*this).plus(rhs);
	}
};

//最小的几个错排数是：D1 = 0，D2 = 1，D3=2，D4 = 9，D5 = 44，D6 = 265，D7 = 1854
ArrayList<BigInteger> calcIllArrangeList(SizeType MAX_SIZE = 26) {
	ArrayList<BigInteger> M(MAX_SIZE, 0);
	M[0] = 0, M[1] = 0, M[2] = 1;
	for (SizeType n = 3; n < MAX_SIZE; ++n)
		M[n] = (M[n - 1] + M[n - 2]).muity(n - 1);
	return M;
}

int mainForIllArrange() {
	auto table = calcIllArrangeList(1001);
	int n;
	//table[100].print();
	while (1 == scanf("%d", &n)) {
		table[n].print();
	}
	return 0;
}

int main_0() {
	mainForIllArrange();
	return 0;
}


// ps: ACM OJ应该是开了编译器优化的
