#ifndef _BIGINTEGER_H
#define _BIGINTEGER_H

#include "../pch.h"
#include "TransitionUtility.h"
//#include "Transition.h"
#include "../ExtendSpace.h"

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
	//返回对应实际字符串长度(包括符号) size = length + 1
	int length() const {
		return calcTotalBitNum() + (symbol() == '\0' ? 0 : 1);
	}
	//  返回对应实际字符串的数字的总位数(不包括符号)
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
		putchar(symbol());
		printf("%d", digitLowTop[totalByteNum - 1]);
		for (int i = 1; i < totalByteNum; i++){
			printf("%04d", digitLowTop[totalByteNum - i - 1]);
		}
		puts("");
	}
	void print(char *outBuffer, JCE::SizeType bufferSize) const {
		int totalByteNum = getTotalByteNum();
		//sprintf_s(outBuffer, bufferSize, symbol());
		*outBuffer = symbol();
		if (*outBuffer == '\0') {
			// do nothing
		}
		else {
			++outBuffer;
			--bufferSize;
		}
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
		int size = length() + 1;
		static std::string result("\0", size);
		result.assign(size, '\0');
		print(&result[0], size);
		return result;
	}
	BigInteger(ByteType originNumber){
		signum = originNumber > 0 ? 1
			: originNumber == 0 ? 0 : -1;
		transitionToLocalRadix(originNumber);
	}
	BigInteger(std::string const &originNumberTopLow){
		std::string temp = TransitionUtility::formatString(originNumberTopLow);
		signum = temp[0] == '-' ? -1
			: temp[0] == '0' ? 0 : 1;
		transitionToLocalRadix(temp);
	}

	//乘法前最好先分配一下
	void reserve(size_t newSize){
		digitLowTop.reserve(newSize);
	}
	//判断是否偶数 是返回true
	bool isEvenNumber() const {
		return this->digitLowTop[0] % 2 == 0;
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
	/**
	 * Returns a BigInteger whose value is {@code (this - val)}.
	 *
	 * @param  val value to be subtracted from this BigInteger.
	 * @return {@code this - val}
	 */
	/*BigInteger subtract(BigInteger const &val) {
		if (val.signum == 0)
			return *this;
		if (signum == 0)
			return val.negate();
		if (val.signum != signum)
			return BigInteger(add(mag, val.mag), signum);

		int cmp = compareMagnitude(val);
		if (cmp == 0)
			return ZERO;
		int[] resultMag = (cmp > 0 ? subtract(mag, val.mag)
			: subtract(val.mag, mag));
		resultMag = trustedStripLeadingZeroInts(resultMag);
		return new BigInteger(resultMag, cmp == signum ? 1 : -1);
	}*/

	/**
	 * Returns a BigInteger whose value is {@code (-this)}.
	 *
	 * @return {@code -this}
	 */
	 //负号 减法
	void minus();
	BigInteger negate() const {
		BigInteger neg(*this);
		neg.signum = -signum;
		return neg;
	}
	
	BigInteger sub(BigInteger const &rhs) {
		// (减, 减去, 负的)是否小减大
		bool minus = *this < rhs;
		ByteType subLhs, subRhs, subSum;
		//subValue(减数) = rhsBit + borrowBit(借位)
		ByteType rhsBit, borrowBit = 0;
		//字数
		int wordCntLhs = getTotalByteNum(), wordCntRhs = rhs.getTotalByteNum();
		//运算数都读取完毕(不用管借位 可能无法再向上借位) -> 中止
		for (subSum = subLhs = subRhs = 0; subLhs < wordCntLhs || subRhs < wordCntRhs; ++subSum) {
			//计算加数 若rhs读取完毕还可能存在进位数 此时加数置为0就好
			rhsBit = subRhs < wordCntRhs ? rhs.digitLowTop[subLhs] : 0;
			rhsBit *= -1;

			if (subLhs < wordCntLhs) {
				digitLowTop[subSum] += rhsBit + borrowBit;
			}
			else {
				//若两者的字数lhs < rhs的话进位是0但是addvalue不是0
				digitLowTop.push_back(rhsBit);
			}

			borrowBit = digitLowTop[subSum] < 0 ? 1 : 0;
			borrowBit *= -1;
			digitLowTop[subSum] %= radix;
			++subLhs, ++subRhs;
		}
		//signum = -signum;
		//plus(rhs.negate());
		if (minus) {
			for (JCE::SizeType i = 0; i < digitLowTop.size(); ++i) {
				digitLowTop[i] = -digitLowTop[i];
			}
			//小减大 自己的符号要变化
			signum = -signum;
		}
		return *this;
	}
	//加数乘法 (基于加法) 返回值和操作数都是自己(返回值是为了便于连续操作 下面的简写操作方法同)
	BigInteger &muity(BigInteger const &rhs) {
		BigInteger addValue(*this);
		// 防止自己乘自己
		BigInteger rhsTemp(rhs);
		for (BigInteger i = 1; i < rhsTemp; ++i){
			plus(addValue);
		}
		return *this;
	}
	//倍加乘法
	BigInteger &muityDouble(ByteType rhs) {
		if (rhs == 0) {
			*this = BigInteger(0);
		}
		else if (rhs == 1) {
			// do nothing
		}
		else {
			//乘一个奇数时最后要多加一个自己
			BigInteger temp = rhs % 2 == 0 ? 0 : *this;
			//乘数rhs大于2时循环: 每次加倍
			while (rhs > 1) {
				muity2();
				//此处可换为减法
				rhs /= 2;
			}
			plus(temp);
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
	//字元乘法 (乘数位 <= 10000)
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
	//快速阶乘 (基于字元乘法 n<=10000)
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
	// 基于字元乘法
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

	/*万进制似乎没法用补码实现减法  可借位减法(唯一可行的应该是模拟减法)*/
	BigInteger operator-(BigInteger const &rhs){
		return BigInteger(*this).sub(rhs);
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

private:
	//表示数字位元的进制 @Bug 2进制时会报错
	const static int radix = 10000;
	//正负号标识 输出时正号不输出
	//char symbol = 0;
	//-1表示小于0; 0表示等于0; 1表示大于0(同Java实现)
	int signum = 0;
	//储存数字位元 的数组(为了支持数字的直接构造这样存比较方便 对字符串构造影响不大)
	std::vector<ByteType> digitLowTop;

	//返回内置数字类型的总10进制位数 0: 1bit; 10: 2bit
	static int totalBitOf(ByteType originNumber) {
		// MathExtend::calcDigitTotalSize(originNumber, 10);
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
		/*symbol = */TransitionUtility::toAbs(originNumber);
		TransitionUtility::decimalToRadixLowTopBase(originNumber, digitLowTop.begin(), radix);
	}
	/*static BigInteger valueOf(int value) {
		return BigInteger(value);
	}*/
	//将源字符串表示的数字转换为本地格式储存
	void transitionToLocalRadix(std::string const &originNumberTopLow) {
		int numTotalBit = 0, wordBit = totalBitOf(radix) - 1;
		Utility::AssertToSignedNum(originNumberTopLow.length(), numTotalBit);
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
	int compare(const BigInteger &rhs) const {
		int topBit1 = -1, topBit2 = -1, result = 0;
		Utility::AssertToSignedNum(digitLowTop.size(), topBit1);
		Utility::AssertToSignedNum(rhs.digitLowTop.size(), topBit2);
		/*if (signum != rhs.signum) {
			result = signum < rhs.signum ? -1 : 1;
		}
		else {*/
			if (topBit1 < topBit2) {
				result = -1;
			}
			else if (topBit1 > topBit2) {
				result = 1;
			}
			else {
				for (int i = topBit1 - 1; i >= 0; --i) {
					if (digitLowTop[i] < rhs.digitLowTop[i]) {
						result = -1;
						break;
					}
					else if (digitLowTop[i] > rhs.digitLowTop[i]) {
						result = 1;
						break;
					}
				}
				//result = 0;
			}
			// 乘其中一个就行
			//result *= signum;
		//}
		return result;
	}
	//返回总的字数
	int getTotalByteNum() const {
		int totalByteNum = -1;
		Utility::AssertToSignedNum(digitLowTop.size(), totalByteNum);
		return totalByteNum;
	}
	//this*=2
	void muity2() {
		this->plus(*this);
	}
	//返回此数的符号 >= 0 没有符号
	char symbol() const {
		return signum == -1 ? '-' : signum == 1 ? '\0' : '\0';
	}
};

#endif

