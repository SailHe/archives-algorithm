#pragma once
#include "../pch.h"
#include "TransitionUtility.h"
#include "../ExtendSpace.h"
#include "../DSAUEdll.h"

// 进制转换器
class DSAUTILITYEXTENSION_API Transition {
public:
	using DigitArray = TransitionUtility::DigitVariableArray;
	using DigitIterator = TransitionUtility::DigitVariableArrayIterator;
	
protected:
	Transition(int bufferRadix);
	virtual ~Transition();
	// 返回本转换器对应的两种进制互相转换时的最小缓存预留位(就是最大进制的位最大值对应的缓存进制的位数)
	int calcMinDigitNum();
	// 重设缓存进制占用数字位, 返回最小预留位数的变化量
	int resetBits(int originBitLeast, int targetBitLeast);

	void reSizeBuffer(JCE::SizeType nextSize);

	// 转换双方的储存位: 一位数字至少(就是刚好的意思)需要多少缓存进制数字位表示
	int originBitLeast;
	int originRadix;

	int bufferRadix;
	DigitArray repositoryBuffer;

	int targetBitLeast;
	int targetRadix;
};

/*
二进制转换器类: 使用二进制数字(bit, binNumber)实现的进制转换器
支持可用2进制表示的任意[基2进制]的大数的双向转换;
任意进制指的int范围内的[基2进制](任一个digitAscll元素必须在int范围内) -> bits => [1, 32) 即: 2 4..2^31共31种[基2进制];
处理2进制位更有优势
符号需要用户自己处理

PS:
1. 基2进制: 以2为底数的进制
2. 2基底数字: 使用基2进制表示的数值
3. 进位段: 源和目标[基2进制]的比特位数中最大的那一个的数值maxBits; 其 <=> [0, maxBits);
4. 例子: 39h->71o
   先将其每一位转为2进制: 3->0011b, 9->1001b
   即0011 1001b 然后转为目标进制 111 001b->71o
5. 可输入的几种进制的单个数值的最大值
   tBit:   1  2  3     4        5
   input:  1  3  7   F(15)    V(31)
   output: 1 11 111  1111     11111
*/
class DSAUTILITYEXTENSION_API BinaryTransition : public Transition {
public:
	// 参数列表: {计算过程中二进制缓存的最大比特位数(若实际情况比这个大则会自动重申), 源进制比特位数, 目标进制比特位数}
	BinaryTransition(JCE::SizeType maxBitSize, int originBitLeast = 1, int targetBitLeast = 1);

	// 重设进制位数: 自动计算缓存[增减量]然后重设最小缓存空间(至少满足最大进制的1位), 同时初始化[进位段]
	void reset(int originBitLeast, int targetBitLeast);

	// 2基底大数转换: 源基2进制->二进制->目标基2进制; targetTopLow储存最终的结果(不会出现多余的0)
	template<class DigitIterator>
	void transition(DigitIterator originLeftTop, DigitIterator originRightLow, std::vector<int> &targetTopLow) {
		// 源数值在目标进制下最多要用多少位表示
		JCE::SizeType originDigitNum = originRightLow - originLeftTop;
		// ................一共.............
		JCE::SizeType originToTargetDigitMaxNum =
			originDigitNum*MathExtend::calcDigitTotalSize(originRadix - 1, targetRadix);
		int minDigitNum = calcMinDigitNum();
		if (originToTargetDigitMaxNum + minDigitNum > repositoryBuffer.size()) {
			// 缓存空间不足->申请合适的缓存大小
			reSizeBuffer(originToTargetDigitMaxNum + minDigitNum);
		}
		// top处预留一个[进位段]用于进位补齐
		auto bitIterator = repositoryBuffer.begin() + targetBitLeast;
		//bitIterator = repositoryBuffer + targetBitLeast;
		auto currentBitIterator = bitIterator;
		
		// 源中top->low每个bit都转换为2进制(连起来即是源进制数的二进制表示)
		while (originLeftTop != originRightLow) {
			auto leftTopIter = TransitionUtility::decimalToRadixTopLow(*originLeftTop++, currentBitIterator + originBitLeast, 2);
			std::fill(currentBitIterator, leftTopIter, 0);
			currentBitIterator += originBitLeast;
		}

		targetTopLow.reserve((currentBitIterator - bitIterator) / targetBitLeast);
		// 不会影响上一句
		targetTopLow.resize(0);

		// 向前补齐(在刚好的情况下 会多补 但后面有判定会去除前导0)
		bitIterator -= targetBitLeast - ((currentBitIterator - bitIterator) % targetBitLeast);
		// 若输出了非0值变为true
		bool hasValidValue = false;
		// top->low每一个targetBitLeast转为10进制(连起来即是目标进制)
		for (int i = 0; i < currentBitIterator - bitIterator; i += targetBitLeast) {
			// 去除所有的转换后的前导0(转换前的前导0转换后可能不是)
			int number = TransitionUtility::radixTopLowToDecimal(bitIterator + i, bitIterator + i + targetBitLeast, 2);
			hasValidValue = number != 0 || hasValidValue;
			if (hasValidValue) {
				targetTopLow.push_back(number);
			}
		}
	}
};

/*
基于10进制的进制转换器
PS: 其API和二进制转换器有些许区别
*/
class DSAUTILITYEXTENSION_API DecimalTransition : public Transition {
public:
	// 参数列表: {计算过程中10进制缓存的最大数字位数{若位数不足会自动重申}, 源进制, 目标进制}
	DecimalTransition(JCE::SizeType maxDigitSize, int originRadix, int targetRadix);

	// 重设转换进制: 自动计算缓存[增减量]然后重设最小缓存空间(至少满足最大进制的1位), 同时初始化必要的元素
	void reset(int originRadix, int targetRadix);

	// 转换为任意进制的数字数组: 源进制->10进制->目标进制; (源进制最高位迭代器, 源进制最低位迭代器的后一个, 结果缓存)
	// 返回最结果的高位迭代器topLeftIterator
	// targetTopLowBuffer不能和origin相同(同类型不同容器的迭代器是无法比较的, 难以检测)
	template<class DigitIterator>
	std::vector<int>::iterator transition(DigitIterator originLeftTop, DigitIterator originRightLow, std::vector<int> &targetTopLowBuffer) {
		// 1. 计算源进制的10进制位数
		// 2. 将源进制每1位数 转为10进制 存储在缓存中: 1位源进制数字->1个10进制数字(可能不止一位, 但存储意义上只占用1位数字)
		// 3. 将缓存中的10进制数字转为目标进制
		JCE::SizeType originDigitNum = originRightLow - originLeftTop;
		int minDigitNum = calcMinDigitNum();
		if (originDigitNum + minDigitNum > repositoryBuffer.size()) {
			// 缓存空间不足->申请合适的缓存大小
			reSizeBuffer(originDigitNum + minDigitNum);
		}
		auto beginDecNumIterator = repositoryBuffer.begin();
		auto endDecNumIterator = repositoryBuffer.begin() + originDigitNum;

		auto realIterPair = 
			TransitionUtility::radixTopLowToDecimalTopLow(originRightLow, endDecNumIterator
				, originDigitNum, (JCE::SizeType)originRadix);
		_ASSERT(originLeftTop == realIterPair.first && beginDecNumIterator == realIterPair.second);

		// 源进制的1数字位对应的目标进制的数字位数
		JCE::SizeType originToTargetDigitNum = MathExtend::calcDigitTotalSize(originRadix - 1, targetRadix);
		// 最多一共需要多少数字位
		JCE::SizeType originToTargetDigitMaxNum = originDigitNum*originToTargetDigitNum;
		// 会重置大小 初始值应当为0
		targetTopLowBuffer.clear();
		// 预留一位进位(最多只会产生1位)
		targetTopLowBuffer.resize(originToTargetDigitMaxNum + 1);
		auto bufferTopLowLeft = targetTopLowBuffer.begin() + 1;
		DigitArray buffer;
		buffer.resize(originToTargetDigitMaxNum);
		while (beginDecNumIterator != endDecNumIterator) {
			auto realBginIter = TransitionUtility::decimalToRadixTopLow(*beginDecNumIterator, buffer.end(), targetRadix);
			// 0补齐
			std::fill(buffer.begin(), realBginIter, 0);
			++beginDecNumIterator;
			MathExtend::vectorPlush(
				bufferTopLowLeft, targetTopLowBuffer.end()
				, buffer.begin(), buffer.end()
				, bufferTopLowLeft, targetTopLowBuffer.end()
			);
		}
		int carryTop = TransitionUtility::carryTopLow(targetTopLowBuffer.begin(), targetTopLowBuffer.end(), targetRadix);
		// 理论上最多产生一次进位
		_ASSERT_EXPR(carryTop == 0, "仍存在未处理的进位!");

		// 计算top非0迭代器
		auto topIter = targetTopLowBuffer.begin();
		while(topIter != targetTopLowBuffer.end()) {
			if (*topIter != 0) {
				break;
			}
			++topIter;
		}
		// 如果全是0的话返回倒数第二位迭代器
		return topIter == targetTopLowBuffer.end() ? --topIter : topIter;
	}
};
