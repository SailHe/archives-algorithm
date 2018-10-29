#ifndef _BIGINTEGER_H
#define _BIGINTEGER_H

#include "stdafx.h"
#include "ExtendSpace.h"

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
	static int radixToTen(VarrayIterator lowBitIter, int radix, int totalSizeNum){
		static int number;
		int powNum = 1;
		number = 0;
		//从低位向高位按权展开 i是幂
		for (int i = 0; i < totalSizeNum; ++i){
			number += lowBitIter[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//返回任意进制的加权10进制数
	//(tenToRadix参数返回值可以直接用此方法直接的到10进制的值)
	static int baseLowTopToTen(VarrayIterator numLowTop, int radix, int totalSizeNum){
		static int number;
		int powNum = 1;
		number = 0;
		//从低位(左侧是低位)向高位按权展开 i是幂
		for (int i = 0; i < totalSizeNum; ++i){
			number += numLowTop[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//存储方式是top->low
	template<class BaseT>//支持基本运算的类型(通常指int __int64)
	static BaseT baseTopLowToTen(VarrayIterator numTopLow, BaseT radix, int totalSizeNum){
		static BaseT number;
		BaseT powNum = 1;
		number = 0;
		//从低位(右侧是低位)向高位按权展开 i是幂
		for (int i = totalSizeNum - 1; i >= 0; --i){
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
	static void reverseCode(VarrayIterator numBin, size_t totalSizeNum){
		for (size_t i = 0; i < totalSizeNum; ++i){
			*numBin = !*numBin;
			++numBin;
		}
	}

	//10进制以上进制会带符号输出 10进制以下正常输出 10对应A
	static void outputWithSymbol(Varray &numLowTop, int totalSizeNum){
		for (int i = 0; i < totalSizeNum; ++i){
			int pSub = totalSizeNum - i - 1;
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


/*
@除歧:
如果bit是指位的话 于数组而言每个元素可称作bit;
但于最终结果而言每个字符才是bit(10进制位), 数组中一个元素应称作Byte(万进制中的字)
但是换一个角度每个元素其实也可以不是多个字符, 这么一来其称为bit也无可厚非(最初的做法)
问题在于bit, byte实际上是计算机的术语, 其中bit专指二进位制信息单位, byte专指字节, 表示一个8位(bit)元组

这么争论是没有结果的, 那么由于历史遗留原因, 折衷:
对外暴露的接口一律:10进制字符串长度: length; 数组大小(size): totalByteNum; 内部实现时一个数组元素: byte

*/

/*
基于万进制的10进制大数类
不支持产生借位, 变号的负数运算
*/
class BigInteger{
	//数字位元(字)的类型(必须能够存下两个进制最大值相乘的值)
	typedef int ByteType;
	typedef JCE::ArrayList<ByteType> BaseImpl;
	const static int ONE = 1;
	const static int ZERO = 0;
public:
	//返回对应实际字符串的数字的总位数
	int calcTotalBitNum() const {
		int totalByteNum = getTotalByteNum();
		//首字的位数 + 剩余字数 * 一字的位数-1
		return
			totalBitOf(digitLowTop[totalByteNum - 1]) +
			(totalByteNum - 1)*(totalBitOf(radix)-1);
	}
	void print() const {
		const char map[17] = "0123456789ABCDEF";
		int totalByteNum = getTotalByteNum();
		printf(symbol == '-' ? "-" : "");
		printf("%d", digitLowTop[totalByteNum - 1]);
		for (int i = 1; i < totalByteNum; i++){
			printf("%04d", digitLowTop[totalByteNum - i - 1]);
		}
		puts("");
	}
	void print(char *outBuffer, JCE::SizeType bufferSize) const {
		int totalByteNum = getTotalByteNum();
		sprintf_s(outBuffer, bufferSize, symbol == '-' ? "-" : "");
		sprintf_s(outBuffer, bufferSize, "%d", digitLowTop[totalByteNum - 1]);
		bufferSize -= strnlen_s(outBuffer, bufferSize);
		outBuffer += totalBitOf(digitLowTop[totalByteNum - 1]);
		const int radixTotalbit = totalBitOf(radix);
		const int radixZerobit = radixTotalbit - 1;
		std::string format = "%0" + std::to_string(radixZerobit) + "d";
		for (int i = 1; i < totalByteNum; ++i){
			//"%04d"
			sprintf_s(outBuffer, bufferSize, format.c_str(), digitLowTop[totalByteNum - i - 1]);
			outBuffer += radixZerobit;
			bufferSize -= radixZerobit;
		}
		outBuffer[0] = '\0';
	}
	std::string const &toString() const {
		// std::string的length和size 是完全一样的
		// 此处size和length取char[]中的含义 length:'\0'之前, size: 整个数组
		int size = calcTotalBitNum() + 1;
		static std::string result("\0", size);
		result.assign(size, '\0');
		print(&result[0], size);
		return result;
	}
	BigInteger(ByteType originNumber){
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
		int i = -1;
		for (Utility::toSignedNum(lenAB - 1, i); i >= 0; --i) {
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
		ByteType subLhs, subRhs, subSum;
		//右手边对应位的值 进位 addValue(加数) = rhsBit + carryBit
		ByteType rhsBit, carryBit = 0;
		//字数
		int wordCntLhs = getTotalByteNum(), wordCntRhs = rhs.getTotalByteNum();

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
	//加数乘法 (基于加法) 返回值和操作数都是自己(返回值是为了便于连续操作 下面的简写操作方法同)
	BigInteger &muity(BigInteger const &rhs) {
		BigInteger addValue(*this);
		for (BigInteger i = 1; i < rhs; ++i){
			plus(addValue);
		}
		return *this;
	}
	//返回乘法后的临时操作数(同java API)
	BigInteger multiply(BigInteger const &rhs) const {
		///不直接调muity: 少一个临时变量
		BigInteger result(*this);
		for (BigInteger i = 1; i < rhs; ++i) {
			result.plus(*this);
		}
		return result;
	}
	//快速乘法 (乘数位 <= 10000)
	BigInteger &muity(ByteType muityBit){
		//由于push_back的存在不开优化必定比不上直接写得算法 开了优化虽仍要慢一点 但好在此类功能齐全
		//int units = 1计量单位: 表示muityBit的一单位代表多大
		ByteType carryBit = 0;
		ByteType subProduct;
		int totalByteNum = getTotalByteNum();
		for (subProduct = 0; subProduct < totalByteNum || carryBit; subProduct++){
			if (subProduct < totalByteNum)
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
	BigInteger multiply(ByteType const &rhs) const {
		BigInteger result(*this);
		result.muity(rhs);
		return result;
	}
	//快速阶乘 (基于快速乘法 n<=10000)
	BigInteger &fact(int n){
		++n;
		while (--n > 0){
			muity(n);
		}
		return *this;
	}
	//加数阶乘 (基于加数乘法 开优化的情况下1000!约1s)
	BigInteger &fact(){
		static BigInteger One(1), Zero(0);
		//为了不使用减法 只能这么玩了 使用减法的话这个变量可以不用定义
		BigInteger product = 1;
		if (*this > One){
			++*this;
			for (BigInteger i(1); i < *this; ++i){
				product.muity(i);
			}
			*this = product;
		}
		else if(*this == Zero){
			//0的阶乘为1
			(*this) = One;
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
	BigInteger operator+(BigInteger const &rhs) const {
		return BigInteger(*this).plus(rhs);
	}
	BigInteger operator+=(BigInteger const &rhs) {
		return plus(rhs);
	}
	// 基于加数乘法
	BigInteger operator*(BigInteger const &rhs) const {
		BigInteger result(*this);
		for (BigInteger i = 1; i < rhs; ++i){
			result.plus(*this);
		}
		return result;
	}
	// 基于快速乘法
	BigInteger operator*(ByteType const &muityBit) const {
		BigInteger result = *this;
		result.muity(muityBit);
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

	//cin>> (只实现了int范围读入)
	friend std::istream &operator>>(std::istream &is, BigInteger &rhs) {
		ByteType temp;
		is >> temp;
		rhs = BigInteger(temp);
		return is;
	}
	//cout<<
	friend std::ostream &operator<<(std::ostream &os, const BigInteger &rhs) {
		os << rhs.toString();
		return os;
	}
	//只需重载基本类型在左侧的情况 在右边时会隐式调用构造方法
	friend BigInteger operator+(const ByteType lhs, const BigInteger &rhs) {
		return rhs + lhs;
	}
	friend BigInteger operator*(const ByteType lhs, const BigInteger &rhs) {
		return rhs * lhs;
	}
	//向double转换
	/*operator double()const {
		int iValue = std::atof(toString().c_str());
		return iValue;
	}*/


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
	//负号 减法
	void minus();

private:
	//表示数字位元的进制 @Bug 2进制时会报错
	const static int radix = 10000;
	//正负号标识 输出时正号不输出
	char symbol = 0;
	//储存数字位元 的数组(为了支持数字的直接构造这样存比较方便 对字符串构造影响不大)
	std::vector<ByteType> digitLowTop;

	//返回内置数字类型的总10进制位数 0: 1bit; 10: 2bit
	static int totalBitOf(ByteType originNumber) {
		int totalBit = 0;
		do {
			originNumber /= 10;
			++totalBit;
		} while (originNumber != 0);
		return totalBit;
	}

	//void setRadix(int r){radix = r;}

	//设置这个大数的储存位数 (grow:只增长(advance only) 小于原位数时不更新大小)
	void setTotalByteNum(JCE::SizeType totalByteNum, bool grow = true) {
		//若已知该数字的大概位数 可以提前指定 减少内存分配
		if (grow && totalByteNum <= digitLowTop.size())
			return;
		digitLowTop.resize(totalByteNum);
	}
	//将源数字转换为本地格式储存
	void transitionToLocalRadix(int originNumber) {
		int numTotalBit = totalBitOf(originNumber);
		//一个字的位数: 储存位(字位)比进制的总位数少一
		int wordBit = totalBitOf(radix) - 1;
		//当传入数字无法用整字储存时需要多用一个字来储存(即溢出字)
		setTotalByteNum(numTotalBit / wordBit + (numTotalBit % wordBit == 0 ? 0 : 1));
		symbol = BinaryTransition::toAbs(originNumber);
		BinaryTransition::tenToRadix(originNumber, digitLowTop.begin(), radix);
	}
	/*static BigInteger valueOf(int value) {
		return BigInteger(value);
	}*/
	//将源字符串表示的数字转换为本地格式储存
	void transitionToLocalRadix(std::string const &originNumberTopLow) {
		int numTotalBit = 0, wordBit = totalBitOf(radix) - 1;
		Utility::toSignedNum(originNumberTopLow.length(), numTotalBit);
		//总储存位数(字数)
		int storeTotalByteNum = numTotalBit / wordBit;
		//溢出位
		int overflowingBit = numTotalBit % wordBit;
		storeTotalByteNum += overflowingBit == 0 ? 0 : 1;
		setTotalByteNum(storeTotalByteNum);

		int currentSub = 0;
		if (overflowingBit > 0) {
			digitLowTop[--storeTotalByteNum] = atoi(originNumberTopLow.substr(currentSub, overflowingBit).c_str());
			currentSub += overflowingBit;
		}
		while (storeTotalByteNum > 0) {
			digitLowTop[--storeTotalByteNum] = atoi(originNumberTopLow.substr(currentSub, wordBit).c_str());
			currentSub += wordBit;
		}
	}
	//小于返回-1 大于返回1 等于返回0
	int compare(const BigInteger& rhs) const {
		int topBit1 = -1, topBit2 = -1;
		Utility::toSignedNum(digitLowTop.size(), topBit1);
		Utility::toSignedNum(rhs.digitLowTop.size(), topBit2);
		if (topBit1 < topBit2)
			return -1;
		else if (topBit1 > topBit2)
			return 1;
		else {
			for (int i = topBit1 - 1; i >= 0; --i) {
				if (digitLowTop[i] < rhs.digitLowTop[i])
					return -1;
				else if (digitLowTop[i] > rhs.digitLowTop[i])
					return 1;
			}
			return 0;
		}
	}
	//返回总的字数
	int getTotalByteNum() const {
		int totalByteNum = -1;
		Utility::toSignedNum(digitLowTop.size(), totalByteNum);
		return totalByteNum;
	}
};

#endif

