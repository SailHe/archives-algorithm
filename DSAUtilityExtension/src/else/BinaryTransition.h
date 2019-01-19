#pragma once
#include "../pch.h"
#include "TransitionUtility.h"
#include "../ExtendSpace.h"

/*
二进制转换器类
支持可用2进制表示的任意进制的大数的双向转换;
bits => [1, 32) 即: 2 4..2^31共31种[基2进制];
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
class BinaryTransition {
public:
	using DigitArray = TransitionUtility::DigitArray;
	using DigitIterator = DigitArray::iterator;

	// 参数列表: (计算过程中二进制缓存的最大比特位数, 源进制比特位数, 目标进制比特位数)
	BinaryTransition(JCE::SizeType bitSize, int originBitLeast = 1, int targetBitLeast = 1) {
		this->originBitLeast = 0;
		this->targetBitLeast = 0;
		this->originRadix = 0;
		this->targetRadix = 0;
		//binaryBuffer = (int *)malloc(sizeof(int)*bitSize);
		binaryBuffer.resize(bitSize);
		reset(originBitLeast, targetBitLeast);
	}

	// 重设进制转换: 自动计算缓存[增减量]然后重设缓存空间, 同时初始化[进位段]
	void reset(int originBitLeast, int targetBitLeast = 1) {
		_ASSERT_EXPR(StandardExtend::inRange(1, originBitLeast, 32), "radix = [1, 32)");
		_ASSERT_EXPR(StandardExtend::inRange(1, targetBitLeast, 32), "radix = [1, 32)");

		// 有增有减->最后考虑abs; 多个变量->变量合成:max; 计算变化量: 减法
		int currentMaxBits = (std::max)(this->originBitLeast, this->targetBitLeast);
		int nextMaxBits = (std::max)(originBitLeast, targetBitLeast);
		// 如果总比特位相等&&最大比特位相等 则disBits=0 否则disBits = nextMaxBits - currentMaxBits
		int disBits = nextMaxBits - currentMaxBits;
		binaryBuffer.resize(binaryBuffer.size() + disBits);

		this->originBitLeast = originBitLeast;
		this->targetBitLeast = targetBitLeast;
		this->originRadix = (int)pow(2.0, originBitLeast);
		this->targetRadix = (int)pow(2.0, targetBitLeast);

		// 预留的[进位段]必须初始化为0 否则进位时会出错
		std::for_each(binaryBuffer.begin(), binaryBuffer.begin() + targetBitLeast, [](int &bit) {bit = 0; });
		//memset(binaryBuffer, 0, targetBitLeast*sizeof(int));
	}

	// 2基底大数转换: 源进制->二进制->目标进制
	// target储存最终的结果(不会出现多余的0) PS: 任意进制指的int范围内的进制(任一个digit元素必须在int范围内)
	void transition(char const *origin, DigitArray &target) {
		JCE::SizeType minTargetLength =
			std::strlen(origin)*MathExtend::calcDigitTotalSize(originRadix - 1, targetRadix);
		_ASSERT_EXPR(minTargetLength <= binaryBuffer.size(), "缓存空间不足!");
		char oneBit = 0;
		// top处预留一个[进位段]用于进位补齐
		binNumberPointer = binaryBuffer.begin() + targetBitLeast;
		//binNumberPointer = binaryBuffer + targetBitLeast;
		currentBinNumberPointer = binNumberPointer;

		//top->low每个源bit都转换为2进制(连起来即是源进制数的二进制表示)

		//while ((oneBit = getchar()) != '\n'){//这样的劣势明显 但优势是外部可以少定义一个char数组(这个数组可能比较大)
		while ((oneBit = *(origin++)) != '\0') {
			int number = oneBit - '0';
			if (isalpha(oneBit)) {
				number = oneBit - 'A' + 10;
			}

			//如4进制用2位表示 输入4 0010 无法完成逆序转换 虽然reTopBit使得即使如此也能完成转换, 但这仍是错的
			_ASSERT_EXPR(number < originRadix, "输入的单个Bit数据超出本进制的模.");

			int totalBits = TransitionUtility::decimalToRadixLowTopBase(number, currentBinNumberPointer, 2);
			//number = radixLowTopToDecimalBase(currentBinNumberPointer, 2, totalBits);
			//向后补齐
			TransitionUtility::complement(currentBinNumberPointer, currentBinNumberPointer + totalBits, originBitLeast);
			//注意: 每originBitLeast位的逆序储存 无法转换为每targetBitLeast位的逆序储存 因此这个逆序是必须的
			int reTopBit = std::max(totalBits, originBitLeast);
			for (int i = 0; i < reTopBit / 2; i++) {
				std::swap(currentBinNumberPointer[i], currentBinNumberPointer[reTopBit - i - 1]);
			}
			currentBinNumberPointer += reTopBit;
		}

		//int ansSub = 0;
		target.reserve((currentBinNumberPointer - binNumberPointer) / targetBitLeast);
		//不会影响上一句
		target.resize(0);

		//向前补齐(在刚好的情况下 会多补 但后面有判定会去除前导0)
		binNumberPointer -= targetBitLeast - ((currentBinNumberPointer - binNumberPointer) % targetBitLeast);
		//若输出了非0值变为true
		bool outputValidValue = false;
		//top->low每一个targetBitLeast转为10进制(连起来即是目标进制)
		for (int i = 0; i < currentBinNumberPointer - binNumberPointer; i += targetBitLeast) {
			//去除所有的转换后的前导0(转换前的前导0转换后可能不是)
			int number = TransitionUtility::radixTopLowToDecimal(binNumberPointer + i, 2, targetBitLeast);
			outputValidValue = number != 0 || outputValidValue;
			if (outputValidValue) {
				target.push_back(number);
			}
			//printf(outputValidValue ? "%d" : "", number);
		}
		//puts("");
	}

	// 转换为任意进制[2, (MAX_INT32)/2)的数字数组(最大进制要保证允许表示该进制的最大值的两倍)
	//void

private:
	// 转换双方的储存位: 一位数字至少(就是刚好的意思)需要多少比特位表示
	int originBitLeast;
	int originRadix;
	int targetBitLeast;
	int targetRadix;
	// repository
	DigitArray binaryBuffer;// = NULL
	DigitIterator binNumberPointer;// = NULL
	DigitIterator currentBinNumberPointer;// = NULL
};
