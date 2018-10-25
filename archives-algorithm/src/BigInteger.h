#ifndef _BIGINTEGER_H
#define _BIGINTEGER_H

#include"stdafx.h"

/*
例子: 39h先将其每一位转为2进制: 3->0011b, 9->1001b   即0011 1001b 然后转为目标进制 111 001b->71o
可输入的几种进制的单个数值的最大值
tBit:   1  2  3     4        5
input:  1  3  7   F(15)    V(31)
output: 1 11 111  1111     11111
*/

//二进制转换器类(支持 可用2进制表示的任意进制的大数的单向转换) 符号需要用户自己处理
class BinaryTransition{
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
	BinaryTransition(int bitSize, int originBitLeast = 1, int targetBitLeast = 1){
		//repository = (int *)malloc(sizeof(int)*bitSize);
		repository.resize(bitSize);
		reset(originBitLeast, targetBitLeast);
	}
	//重设进制转换 但总空间不支持重设
	void reset(int originBitLeast, int targetBitLeast = 1){
		this->originBitLeast = originBitLeast;
		this->targetBitLeast = targetBitLeast;
		//memset(repository, 0, targetBitLeast*sizeof(int));//预留的一段必须初始化
	}
	//以二进制为中间储存的进制大数转换 ans储存最终的结果(不会出现多余的0)
	void transition(char *origin, std::vector<int> &ans){
		char oneBit = 0;
		binNumberPointer = repository.begin() + targetBitLeast;
		//binNumberPointer = repository + targetBitLeast;
		currentBinNumberPointer = binNumberPointer;//预留一个目标储存位用于补齐

		//top->low每个源bit都转换为2进制(连起来即是源进制数的二进制表示)

		//while ((oneBit = getchar()) != '\n'){//这样的劣势明显 但优势是外部可以少定义一个char数组(这个数组可能比较大)
		while ((oneBit = *(origin++)) != '\0'){
			int number = oneBit - '0';
			if (isalpha(oneBit)){
				number = oneBit - 'A' + 10;
			}

			if (number >= originRadix){
				//_DEBUG_ERROR("输入的单个Bit数据超出本进制的模.");
				//如4进制用2位表示 输入4 0010 无法完成逆序转换 虽然reTopBit使得即使如此也能完成转换, 但这仍是错的
			}

			int bit = tenToRadix(number, currentBinNumberPointer, 2);
			//number = baseLowTopToTen(currentBinNumberPointer, 2, bit);
			//向后补齐
			complement(currentBinNumberPointer, currentBinNumberPointer + bit, originBitLeast);
			//注意: 每originBitLeast位的逆序储存 无法转换为每targetBitLeast位的逆序储存 因此这个逆序是必须的
			int reTopBit = max(bit, originBitLeast);
			for (int i = 0; i < reTopBit / 2; i++){
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
		for (int i = 0; i < currentBinNumberPointer - binNumberPointer; i += targetBitLeast){
			//去除所有的转换后的前导0(转换前的前导0转换后可能不是)
			int number = baseTopLowToTen(binNumberPointer + i, 2, targetBitLeast);
			outputValidValue = number != 0 || outputValidValue;
			if (outputValidValue){
				ans.push_back(number);
			}
			//printf(outputValidValue ? "%d" : "", number);
		}
		//puts("");
	}

	/*基础进制转换 如果数值超出int范围直接使用2进制的中间储存做大数计算*/

	//将数字转换正数(正数仍是正数) 返回该数字的正负符号
	static char toAbs(int &number){
		return number < 0 ? (number = -number), '-' : '+';
	}

	//数字->数组 (返回位数 以及 repositoryLowToTop低位向高位存储的数值储存迭代器)
	static int tenToRadix(unsigned number, VarrayIterator repositoryLowToTop, int radix){
		static int bit;
		bit = -1;
		//要保证即使传入0也能执行一次
		do{
			repositoryLowToTop[++bit] = number % radix;
			number /= radix;
		} while (number != 0);
		return bit + 1;
	}

	//字符数组->数字位元数组 返回总位数
	static size_t toDigitBitArray(char *originTopLow, Varray &numTopLow){
		size_t len = strlen(originTopLow);
		for (size_t i = 0; i < len; ++i){
			if (isalpha(originTopLow[i])){
				numTopLow[i] = toupper(originTopLow[i]) - 'A' + 10;
			}
			else if (isalnum(originTopLow[i])){
				numTopLow[i] = originTopLow[i] - '0';
			}
			else{
				//_DEBUG_ERROR("输入数据含非法字符(合法字符: 数字, 大小写字母)")
			}
		}
		return len;
	}


	/*(lowBitIter: 低位向高位逼近的迭代器 rbegain() )*/
	static int radixToTen(VarrayIterator lowBitIter, int radix, int totalBit){
		static int number;
		int powNum = 1;
		number = 0;
		//从低位向高位按权展开 i是幂
		for (int i = 0; i < totalBit; ++i){
			number += lowBitIter[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//返回任意进制的加权10进制数
	//(tenToRadix参数返回值可以直接用此方法直接的到10进制的值)
	static int baseLowTopToTen(VarrayIterator numLowTop, int radix, int totalBit){
		static int number;
		int powNum = 1;
		number = 0;
		//从低位(左侧是低位)向高位按权展开 i是幂
		for (int i = 0; i < totalBit; ++i){
			number += numLowTop[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//存储方式是top->low
	template<class BaseT>//支持基本运算的类型(通常指int __int64)
	static BaseT baseTopLowToTen(VarrayIterator numTopLow, BaseT radix, int totalBit){
		static BaseT number;
		BaseT powNum = 1;
		number = 0;
		//从低位(右侧是低位)向高位按权展开 i是幂
		for (int i = totalBit - 1; i >= 0; --i){
			number += numTopLow[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}


	//不足n的倍数位向后补0 originP~currentP(n*0)
	static void complement(VarrayIterator originP, VarrayIterator currentP, int n){
		int residueBit = (currentP - originP) % n;
		if (residueBit != 0){
			residueBit = n - residueBit;
			while (residueBit-- > 0){
				currentP[residueBit] = 0;
			}
		}
	}

	//对每一位二进制取反
	static void reverseCode(VarrayIterator numBin, size_t totalBit){
		for (size_t i = 0; i < totalBit; ++i){
			*numBin = !*numBin;
			++numBin;
		}
	}

	//10进制以上进制会带符号输出 10进制以下正常输出 10对应A
	static void outputWithSymbol(Varray &numLowTop, int totalBit){
		for (int i = 0; i < totalBit; ++i){
			int pSub = totalBit - i - 1;
			if (numLowTop[pSub] > 9){
				printf("%c", numLowTop[pSub] - 10 + 'A');
			}
			else{
				printf("%d", numLowTop[pSub]);
			}
		}
		puts("");
	}
};


/*不支持产生借位, 变号的负数运算*/
class BigInteger{
	//表示数字位元的进制
	//@Bug 2进制时会报错
	const static int radix = 10000;
	const static int ONE = 1;
	const static int ZERO = 0;
	//数字位元的类型(必须能够存下两个进制最大值相乘的值)
	typedef int BitType;
	typedef std::vector<BitType> Base;
	//正负号标识 输出时正号不输出
	char symbol = 0;
	//储存数字位元 的数组(为了支持数字的直接构造这样存比较方便 对字符串构造影响不大)
	std::vector<BitType> digitLowTop;

	//返回内置数字类型的总位数 0: 1bit; 10: 2bit
	static int totalBitOf(BitType originNumber){
		int totalBit = 0;
		do{
			originNumber /= 10;
			++totalBit;
		}while (originNumber != 0);
		return totalBit;
	}

	//void setRadix(int r){radix = r;}

	//设置这个大数的储存位数 (grow:只增长(advance only) 小于原位数时不更新大小)
	void setTotalBit(size_t totalBit, bool grow = true){
		//若已知该数字的大概位数 可以提前指定 减少内存分配
		if (grow && totalBit <= digitLowTop.size())
			return;
		digitLowTop.resize(totalBit);
	}
	//将源数字转换为本地格式储存
	void transitionToLocalRadix(int originNumber){
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
	//将源字符串表示的数字转换为本地格式储存
	void transitionToLocalRadix(std::string const &originNumberTopLow){
		int numTotalBit = originNumberTopLow.length(), wordBit = totalBitOf(radix) - 1;
		//总储存位
		int storeTotalBit = numTotalBit / wordBit;
		//溢出位
		int overflowingBit = numTotalBit % wordBit;
		storeTotalBit += overflowingBit == 0 ? 0 : 1;
		setTotalBit(storeTotalBit);

		int currentSub = 0;
		if (overflowingBit > 0){
			digitLowTop[--storeTotalBit] = atoi(originNumberTopLow.substr(currentSub, overflowingBit).c_str());
			currentSub += overflowingBit;
		}
		while (storeTotalBit > 0){
			digitLowTop[--storeTotalBit] = atoi(originNumberTopLow.substr(currentSub, wordBit).c_str());
			currentSub += wordBit;
		}
	}
	//小于返回-1 大于返回1 等于返回0
	int compare(const BigInteger& rhs) const{
		size_t topBit1 = digitLowTop.size(), topBit2 = rhs.digitLowTop.size();
		if (topBit1 < topBit2)
			return -1;
		else if (topBit1 > topBit2)
			return 1;
		else{
			for (int i = topBit1 - 1; i >= 0; i--){
				if (digitLowTop[i] < rhs.digitLowTop[i])
					return -1;
				else if (digitLowTop[i] > rhs.digitLowTop[i])
					return 1;
			}
			return 0;
		}
	}
public:
	//返回实际位数
	int getTotalBit(){
		//字数*一字的位数(不减1?)
		return digitLowTop.size()*totalBitOf(radix);
	}
	void print(){
		const char map[17] = "0123456789ABCDEF";
		int totalBit = digitLowTop.size();
		printf(symbol == '-' ? "-" : "");
		printf("%d", digitLowTop[totalBit - 1]);
		for (int i = 1; i < totalBit; i++){
			printf("%04d", digitLowTop[totalBit - i - 1]);
		}
		puts("");
	}
	void print(char *out){
		int totalBit = digitLowTop.size();
		sprintf(out, symbol == '-' ? "-" : "");
		sprintf(out, "%d", digitLowTop[totalBit - 1]);
		out += totalBitOf(digitLowTop[totalBit - 1]);
		for (int i = 1; i < totalBit; i++){
			sprintf(out, "%04d", digitLowTop[totalBit - i - 1]);
			out += totalBitOf(radix)-1;
		}
		out[0] = '\0';
	}

	BigInteger(BitType originNumber){
		transitionToLocalRadix(originNumber);
	}
	BigInteger(std::string const &originNumberTopLow){
		transitionToLocalRadix(originNumberTopLow);
	}

	//乘法前最好先分配一下
	void reserve(size_t newSize){
		digitLowTop.reserve(newSize);
	}

	// 字符串大数加法
	static std::string bigPlush(std::string &topLowNumA, std::string &topLowNumB, std::string &topLowSum) {
		std::size_t lenA = topLowNumA.size(), lenB = topLowNumB.size(), lenAB;
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
		if (topLowSum.size() < lenA) {
			topLowSum.resize(lenA, '0');
		}
		int ia = -1, c = 0;
		//反转后左边是低位
		for (int i = lenAB - 1; i >= 0; --i) {
			int sumBit = (topLowNumA[i] - '0') + (topLowNumB[i] - '0') + c;
			topLowSum[i] = sumBit % 10 + '0';
			c = sumBit / 10;
		}
		return c == 0 ? topLowSum : (topLowSum = "1" + topLowSum);
	}
	//去除前导0 若值为0 返回"0"
	static std::string formatString(std::string const &num) {
		int sp = -1;
		for (auto it = num.begin(); it != num.end(); ++it) {
			++sp;
			if (*it != '0') {
				break;
			}
		}
		return sp == num.size() ? "0" : num.substr(sp);
	}

	//<==> += 
	BigInteger &plus(BigInteger const &rhs){
		BitType subLhs, subRhs, subSum;
		//右手边对应位的值 进位 addValue(加数) = rhsBit + carryBit
		BitType rhsBit, carryBit = 0;
		//字数
		int wordCntLhs = digitLowTop.size(), wordCntRhs = rhs.digitLowTop.size();

		for (subSum = subLhs = subRhs = 0; subLhs < wordCntLhs || subRhs < wordCntRhs || carryBit != 0; ++subSum){
			//计算加数 若rhs读取完毕还可能存在进位数 此时加数置为0就好
			rhsBit = subRhs < wordCntRhs ? rhs.digitLowTop[subLhs] : 0;

			if (subLhs < wordCntLhs){
				digitLowTop[subSum] += rhsBit + carryBit;
			}
			else{
				//若两者的字数lhs < rhs的话进位是0但是addvalue不是0
				digitLowTop.push_back(rhsBit + carryBit);
			}

			carryBit = digitLowTop[subSum] / radix;
			digitLowTop[subSum] %= radix;
			++subLhs, ++subRhs;
		}
		return *this;
	}
	//大数乘法 加法实现
	BigInteger &muity(BigInteger const &rhs){
		BigInteger addValue(*this);
		for (BigInteger i = 1; i < rhs; ++i){
			plus(addValue);
		}
		return *this;
	}
	//快速乘法 (乘数位 <= 10000) 由于push_back的存在不开优化必定比不上直接写得算法 开了优化虽仍要慢一点 但好在此类功能齐全
	BigInteger &muity(BitType muityBit){
		//int units = 1计量单位: 表示muityBit的一单位代表多大
		BitType carryBit = 0;
		BitType subProduct;
		int totalBit = digitLowTop.size();
		for (subProduct = 0; subProduct < totalBit || carryBit; subProduct++){
			if (subProduct < totalBit)
				digitLowTop[subProduct] = digitLowTop[subProduct] * muityBit + carryBit;
			else{
				//位乘法只可能是进位
				digitLowTop.push_back(carryBit);
			}
			carryBit = digitLowTop[subProduct] / radix;
			digitLowTop[subProduct] %= radix;
		}
		return *this;
	}
	BigInteger &fact(int n){
		++n;
		while (--n > 0){
			muity(n);
		}
		return *this;
	}
	//大数阶乘 (数值<=10000时建议使用快速乘法在外部计算 不过事实上这个算法在即使开了优化的情况下1000!已经需要大约1s了)
	BigInteger &fact(){
		//为了不使用减法 只能这么玩了 使用减法的话这个变量可以不用定义
		BigInteger product = 1;
		if (*this > ZERO){
			++*this;
			for (BigInteger i(1); i < *this; ++i){
				product.muity(i);
			}
			*this = product;
		}
		else if(*this == ZERO){
			//0的阶乘为1
			(*this) = ONE;
		}
		else{
			//负数没有阶乘
		}
		return *this;
	}
	bool operator<(const BigInteger& rhs) const{
		return compare(rhs) == -1;
	}
	bool operator>(const BigInteger& rhs) const{
		return compare(rhs) == 1;
	}
	bool operator==(const BigInteger& rhs) const{
		return compare(rhs) == 0;
	}
	bool operator!=(const BigInteger& rhs) const{
		return compare(rhs) != 0;
	}
	BigInteger operator+(BigInteger const &rhs){
		return BigInteger(*this).plus(rhs);
	}
	// 基于加法的乘法
	BigInteger operator*(BigInteger const &rhs){
		/// 乘法(没有直接 muity 为了少一个临时变量是单独实现的)
		BigInteger addValue(*this);
		for (BigInteger i = 1; i < rhs; ++i){
			addValue.plus(*this);
		}
		return addValue;
	}
	// 基于非大数的右操作数的快速乘法
	BigInteger operator*(BitType const &muityBit){
		BigInteger result = this->muity(muityBit);
		return result;
	}
	BigInteger operator*=(BigInteger const &rhs){
		return muity(rhs);
	}
	//前置版
	BigInteger& operator++(){
		this->plus(ONE);
		return *this;
	}
	BigInteger operator++(int){
		BigInteger tmp = *this;
		++(*this);
		return tmp;
	}

	/*万进制似乎没法用补码实现减法  可借位减法(唯一可行的应该是模拟减法)*/
	BigInteger operator-(BigInteger const &rhs){
		this->plus(rhs);
		return *this;
	}
	BigInteger operator/(BigInteger const &rhs){
		this->plus(rhs);
		return *this;
	}
	BigInteger operator--(){//前置版
		this->plus(BigInteger(-1));
		return *this;
	}
	BigInteger operator--(int){
		BigInteger tmp = *this;
		--(*this);
		return tmp;
	}
	void minus();//负号 减法
};

/*

*/

#endif

/*
100
93326215443944152681699238856266700490715968264381621468592963
895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000
500
12201368259911100687012387854230469262535743428031928421924
1023
5291532027401227815504806586605326892579642542517591254377802998714072863352906839
2300
1502[0]4882700144063261010037638111251819477735600118435480095
5行0最后一个对着5
10000
28462596809170545189[0]64132121198688901480514017027992307941
79994274411340003764443772990786757784775815884062142317528830042339940153518739052421161382716
*/