#include<stdio.h>
#include<string.h>
#include<string>
#include<cmath>
#include<vector>
#include<iostream>
#include<algorithm>
#include<queue>
#include<functional>//greater
#include<list>
#include<stack>
#include<map>
#include<iomanip>//cout各种操纵器

#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)

// #include "ExtendSpace.h"

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
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
	KEEP_OPT, REPLACE_OPT, DELETE_OPT, INSERT_OPT, UN_DEFINDE_OPT
};

//二进制转换器类(支持 可用2进制表示的任意进制的大数的单向转换) 符号需要用户自己处理
class BinaryTransition {
	//转换双方的储存位: 一位至少需要多少位二进制表示
	int originBitLeast = 1;
	int originRadix = (int)pow((double)2, originBitLeast);
	int targetBitLeast = 1;
	/*
	//只需改一两处就可以替换
	typedef int *Varray;
	typedef Varray VarrayIterator;
	*/
	typedef std::vector<int> Varray;
	typedef Varray::iterator VarrayIterator;
	Varray repository;// = NULL
	VarrayIterator binNumberPointer;// = NULL
	VarrayIterator currentBinNumberPointer;// = NULL
public:
	//支持[1, 31] 即2 4..共31种进制(口头约定: 只能高进制向低进制转) 1bit即表示目标进制是2进制
	BinaryTransition(int bitSize, int originBitLeast = 1, int targetBitLeast = 1) {
		//repository = (int *)malloc(sizeof(int)*bitSize);
		repository.resize(bitSize);
		reset(originBitLeast, targetBitLeast);
	}
	//重设进制转换 但总空间不支持重设
	void reset(int originBitLeast, int targetBitLeast = 1) {
		this->originBitLeast = originBitLeast;
		this->targetBitLeast = targetBitLeast;
		//memset(repository, 0, targetBitLeast*sizeof(int));//预留的一段必须初始化
	}
	//以二进制为中间储存的进制大数转换 ans储存最终的结果(不会出现多余的0)
	void transition(char *origin, std::vector<int> &ans) {
		char oneBit = 0;
		binNumberPointer = repository.begin() + targetBitLeast;
		//binNumberPointer = repository + targetBitLeast;
		currentBinNumberPointer = binNumberPointer;//预留一个目标储存位用于补齐

		//top->low每个源bit都转换为2进制(连起来即是源进制数的二进制表示)

		//while ((oneBit = getchar()) != '\n'){//这样的劣势明显 但优势是外部可以少定义一个char数组(这个数组可能比较大)
		while ((oneBit = *(origin++)) != '\0') {
			int number = oneBit - '0';
			if (isalpha(oneBit)) {
				number = oneBit - 'A' + 10;
			}

			if (number >= originRadix) {
				//_DEBUG_ERROR("输入的单个Bit数据超出本进制的模.");
				//如4进制用2位表示 输入4 0010 无法完成逆序转换 虽然reTopBit使得即使如此也能完成转换, 但这仍是错的
			}

			int bit = tenToRadix(number, currentBinNumberPointer, 2);
			//number = baseLowTopToTen(currentBinNumberPointer, 2, bit);
			//向后补齐
			complement(currentBinNumberPointer, currentBinNumberPointer + bit, originBitLeast);
			//注意: 每originBitLeast位的逆序储存 无法转换为每targetBitLeast位的逆序储存 因此这个逆序是必须的
			int reTopBit = max(bit, originBitLeast);
			for (int i = 0; i < reTopBit / 2; i++) {
				std::swap(currentBinNumberPointer[i], currentBinNumberPointer[reTopBit - i - 1]);
			}
			currentBinNumberPointer += reTopBit;
		}

		//int ansSub = 0;
		ans.reserve((currentBinNumberPointer - binNumberPointer) / targetBitLeast);
		//不会影响上一句
		ans.resize(0);

		//向前补齐(在刚好的情况下 会多补 但后面有判定会去除前导0)
		binNumberPointer -= targetBitLeast - ((currentBinNumberPointer - binNumberPointer) % targetBitLeast);
		//若输出了非0值变为true
		bool outputValidValue = false;
		//top->low每一个targetBitLeast转为10进制(连起来即是目标进制)
		for (int i = 0; i < currentBinNumberPointer - binNumberPointer; i += targetBitLeast) {
			//去除所有的转换后的前导0(转换前的前导0转换后可能不是)
			int number = baseTopLowToTen(binNumberPointer + i, 2, targetBitLeast);
			outputValidValue = number != 0 || outputValidValue;
			if (outputValidValue) {
				ans.push_back(number);
			}
			//printf(outputValidValue ? "%d" : "", number);
		}
		//puts("");
	}

	/*基础进制转换 如果数值超出int范围直接使用2进制的中间储存做大数计算*/

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

	//字符数组->数字位元数组 返回总位数
	static size_t toDigitBitArray(char *originTopLow, Varray &numTopLow) {
		size_t len = strlen(originTopLow);
		for (size_t i = 0; i < len; ++i) {
			if (isalpha(originTopLow[i])) {
				numTopLow[i] = toupper(originTopLow[i]) - 'A' + 10;
			}
			else if (isalnum(originTopLow[i])) {
				numTopLow[i] = originTopLow[i] - '0';
			}
			else {
				//_DEBUG_ERROR("输入数据含非法字符(合法字符: 数字, 大小写字母)")
			}
		}
		return len;
	}


	/*(lowBitIter: 低位向高位逼近的迭代器 rbegain() )*/
	static int radixToTen(VarrayIterator lowBitIter, int radix, int totalSizeNum) {
		static int number;
		int powNum = 1;
		number = 0;
		//从低位向高位按权展开 i是幂
		for (int i = 0; i < totalSizeNum; ++i) {
			number += lowBitIter[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//返回任意进制的加权10进制数
	//(tenToRadix参数返回值可以直接用此方法直接的到10进制的值)
	static int baseLowTopToTen(VarrayIterator numLowTop, int radix, int totalSizeNum) {
		static int number;
		int powNum = 1;
		number = 0;
		//从低位(左侧是低位)向高位按权展开 i是幂
		for (int i = 0; i < totalSizeNum; ++i) {
			number += numLowTop[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//存储方式是top->low
	template<class BaseT>//支持基本运算的类型(通常指int __int64)
	static BaseT baseTopLowToTen(VarrayIterator numTopLow, BaseT radix, int totalSizeNum) {
		static BaseT number;
		BaseT powNum = 1;
		number = 0;
		//从低位(右侧是低位)向高位按权展开 i是幂
		for (int i = totalSizeNum - 1; i >= 0; --i) {
			number += numTopLow[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}


	//不足n的倍数位向后补0 originP~currentP(n*0)
	static void complement(VarrayIterator originP, VarrayIterator currentP, int n) {
		int residueBit = (currentP - originP) % n;
		if (residueBit != 0) {
			residueBit = n - residueBit;
			while (residueBit-- > 0) {
				currentP[residueBit] = 0;
			}
		}
	}

	//对每一位二进制取反
	static void reverseCode(VarrayIterator numBin, size_t totalSizeNum) {
		for (size_t i = 0; i < totalSizeNum; ++i) {
			*numBin = !*numBin;
			++numBin;
		}
	}

	//10进制以上进制会带符号输出 10进制以下正常输出 10对应A
	static void outputWithSymbol(Varray &numLowTop, int totalSizeNum) {
		for (int i = 0; i < totalSizeNum; ++i) {
			int pSub = totalSizeNum - i - 1;
			if (numLowTop[pSub] > 9) {
				printf("%c", numLowTop[pSub] - 10 + 'A');
			}
			else {
				printf("%d", numLowTop[pSub]);
			}
		}
		puts("");
	}
};

// 任意进制之间互相转换(使用了二进制转换器类: BinaryTransition) 基于10进制
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
int mainForSolveB() {
	int originBase, targetBase;
	string origin, target;
	origin.resize(20);
	target.resize(20);
	vector<int> digitTopLow;
	digitTopLow.resize(20);
	while (cin >> originBase >> targetBase) {
		BinaryTransition bt(20, originBase, targetBase);
		scanf("%s", &origin[0]);
		// bt.transition(&origin[0], digitTopLow);
		// 这个API也太难用了吧 30min
		int totalBit = BinaryTransition::toDigitBitArray(&origin[0], digitTopLow);
		int tenValue = BinaryTransition::baseTopLowToTen(digitTopLow.begin(), originBase, totalBit);
		reverse(digitTopLow.begin(), digitTopLow.end());
		totalBit = BinaryTransition::tenToRadix(tenValue, digitTopLow.begin(), targetBase);
		BinaryTransition::outputWithSymbol(digitTopLow, totalBit);
		// for (int i = totalBit - 1; i >= 0; --i) {
		// 	printf("%d", digitTopLow[i]);
		// }
		//cout << endl;
	}
	return 0;
}

// 10 -> radix
int mainForSolveC() {
	int originValue, targetBase;
	string origin, target;
	origin.resize(20);
	target.resize(20);
	vector<int> digitTopLow;
	digitTopLow.resize(20);
	while (cin >> originValue >> targetBase) {
		int tenValue = originValue;
		int totalBit = BinaryTransition::tenToRadix(tenValue, digitTopLow.begin(), targetBase);
		BinaryTransition::outputWithSymbol(digitTopLow, totalBit);
	}
	return 0;
}

// 丧心病狂 radix=[2, 62] char={[0,9], [A, Z], [a, z]}
int mainForSolveD() {
	int originBase, targetBase;
	string origin, target;
	origin.resize(20);
	target.resize(20);
	vector<int> digitTopLow, digitLowTop;
	digitTopLow.resize(20);
	digitLowTop.resize(20);
	while (cin >> originBase >> targetBase) {
		BinaryTransition bt(1000+5);// , originBase, targetBase
		scanf("%s", &origin[0]);
		bt.transition(&origin[0], digitTopLow);
		int tv = BinaryTransition::baseTopLowToTen(digitTopLow.begin(), 2, digitTopLow.size());
		printf("10->%d\n", tv);
		BinaryTransition::tenToRadix(tv, digitLowTop.begin(), targetBase);
		reverse(digitLowTop.begin(), digitLowTop.end());
		BinaryTransition::outputWithSymbol(digitLowTop, digitLowTop.size());
		// // 这个API也太难用了吧 30min
		// int totalBit = BinaryTransition::toDigitBitArray(&origin[0], digitTopLow);
		// int tenValue = BinaryTransition::baseTopLowToTen(digitTopLow.begin(), originBase, totalBit);
		// reverse(digitTopLow.begin(), digitTopLow.end());
		// totalBit = BinaryTransition::tenToRadix(tenValue, digitTopLow.begin(), targetBase);
		// BinaryTransition::outputWithSymbol(digitTopLow, totalBit);
	}
	return 0;
}

int mainForSolveA() {
	int caseCnt;
	scanf("%d", &caseCnt);
	while (caseCnt-- > 0) {
		string lhs, rhs, equalitySeq;
		cin >> lhs >> rhs;
		lhs = " " + lhs;
		rhs = " " + rhs;
		ArrayList<ArrayList<int>> dis2d(lhs.length(), ArrayList<int>(rhs.length(), 0));
		ArrayList<ArrayList<DIRECTION_ENUM>> path2d(lhs.length(), ArrayList<DIRECTION_ENUM>(rhs.length(), UN_KNOEN_DIR));
		// 第0行和第0列需要初始化为0
		for (SizeType r = 1; r < lhs.length(); ++r) {

		}

		// 行与lhs相关 列与rhs相关
		for (SizeType r = 1; r < lhs.length(); ++r) {
			for (SizeType c = 1; c < rhs.length(); ++c) {
				if (lhs[r] == rhs[c]) {
					//equalitySeq += lhs;
					dis2d[r][c] = dis2d[r - 1][c - 1] + 1;
					path2d[r][c] = LEFT_UP_DIR;
				}
				else {
					if (dis2d[r - 1][c] >= dis2d[r][c - 1]) {
						dis2d[r][c] = dis2d[r - 1][c];
						path2d[r][c] = UP_DIR;
					}
					else {
						dis2d[r][c] = dis2d[r][c - 1];
						path2d[r][c] = LEFT_DIR;
					}
				}
			}
		}
		cout << equalitySeq.length() << endl;
	}
	return 0;
}


// itoa
int main() {
	//mainForSolveB();
	//mainForSolveC();
	//mainForSolveD();
	mainForSolveA();
	return 0;
}
