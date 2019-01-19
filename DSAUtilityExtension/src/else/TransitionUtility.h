#pragma once
#include "../pch.h"
#include "../DSAUEdll.h"

namespace TransitionUtility {
	/*
	整数(Integer) 表示支持所有基本运算(含模运算)的类型(EG: int __int64 long long)
	Digit的类型是Integer 其特指将这一个整数作为某进制下的一位(不是比特位bit)
	left right迭代器泛型特点: 可迭代的容器都可以(比如数组和std::vector); 仍可以活用std::vector().rbgain();
	*/

	/*
	//只需改一两处就可以替换
	typedef int *DigitArray;
	typedef DigitArray DigitIterator;
	*/
	// 是基于整数数字数组的 (类似于字符串) 原称: DigitBitArray(数字位元数组)
	typedef std::vector<int> DigitArray;
	// BitIter -> Biter
	typedef DigitArray::iterator DigitIterator;

	// Integer(D) -> 字符{Alph, 数字}
	DSAUTILITYEXTENSION_API char toUppercaseAscllChar(int num);
	// 字符{字母, 数字} -> Integer(D)
	DSAUTILITYEXTENSION_API int toIntNum(char alphOrCharNum);
	// 字符{字母, 数字} -> Integer(radix) Base:toIntNum; 加了异常检测
	DSAUTILITYEXTENSION_API int toRadixIntNum(char alphOrCharNum, int radix = 10);
	// 将数字转换正数(正数仍是正数) 返回该数字的正负符号
	DSAUTILITYEXTENSION_API char toAbs(int &number);
	// 不足n的倍数位向后补0 originP~currentP(n*0)
	DSAUTILITYEXTENSION_API void complement(DigitIterator originP, DigitIterator currentP, int n);

	// 数字容器 -> 字符容器{数字, 大小写字母(区分)}
	template<class DigitContainerIterator, class CharContainerIterator>
	void digitContainerToCharContainer(DigitContainerIterator digitIterBegin, DigitContainerIterator digitIterEnd, CharContainerIterator charIter) {
		std::for_each(digitIterBegin, digitIterEnd, [&](char intNum) {
			*charIter = toUppercaseAscllChar(intNum);
			++charIter;
		});
	}
	template<class DigitContainerIterator>
	std::string digitContainerToString(DigitContainerIterator digitIterBegin, DigitContainerIterator digitIterEnd) {
		std::string resultStr;
		resultStr.resize(digitIterEnd - digitIterBegin);
		digitContainerToCharContainer(digitIterBegin, digitIterEnd, resultStr.begin());
		return resultStr;
	}

	// 字符容器{数字, 大小写字母(区分)} -> 数字容器
	template<class CharContainerIterator, class DigitContainerIterator>
	void charContainerToDigitContainer(CharContainerIterator charIterBegin, CharContainerIterator charIterEnd, DigitContainerIterator digitIter) {
		std::for_each(charIterBegin, charIterEnd, [&](char value) {
			*digitIter = toIntNum(value);
			++digitIter;
		});
	}

	// 对每一位二进制取反
	template<class DigitIterator>
	void inverseCode(DigitIterator binIterBegin, DigitIterator binIterEnd) {
		StandardExtend::iterate(binIterBegin, binIterEnd, [](DigitIterator currentIt) {
			*currentIt = *currentIt == 0 ? 1 : 0;
		});
	}

	// 打印数字迭代器中的所有元素; WithSymbol: 10以上数字使用符号输出(10对应A)
	// (隐含条件: 输出的起始<==>已知位数: outputDigitInRange(lowTopList.rbegin(), lowTopList.rbegin() + totalSizeNum);)
	template<class DigitIterator>
	void outputDigitInRange(DigitIterator digitIterBegin, DigitIterator digitIterEnd) {
		std::for_each(digitIterBegin, digitIterEnd, [](int currentValue) {
			putchar(toUppercaseAscllChar(currentValue));
		});
		puts("");
	}

	/**
	 * 基础(int)进制(10)转换; 中间数值不能超出int范围
	 **/

	 // 数字 -> 数组 (需要转换的10进制数字, 低位向高位存储的数值储存迭代器)
	 // 返回位数: totalSizeNum; 改变参数数组
	template<class DigitIterator>
	int decimalToRadixLowTop(unsigned decimaNum, DigitIterator lowTopIter, int radix) {
		static int index;
		index = -1;
		// 要保证即使传入0也能执行一次
		do {
			*lowTopIter++ = decimaNum % radix;
			++index;
			decimaNum /= radix;
		} while (decimaNum != 0);
		return index + 1;
	}
	// 基于decimalToRadixLowTop
	template<class DigitIterator>
	int decimalToRadixTopLow_baseLowTop(unsigned decimaNum, DigitIterator topLowIter, int radix) {
		int totalSizeNum = decimalToRadixLowTop(decimaNum, topLowIter, radix);
		std::reverse(topLowIter, topLowIter + totalSizeNum);
		return totalSizeNum;
	}

	// !!!不推荐使用!!!: 迭代器泛型中使用数组运算符'[]'; 使迭代器降级(抽象级别)->必须支持任意合理值的迭代器加减运算
	// 需要提前得知转换后的总位数大小(不是二进制bitNum)
	template<class DigitIterator>
	void decimalToRadixTopLow(unsigned decimaNum, DigitIterator topLowLeftIter, int radix, int totalSizeNum) {
		// 要保证即使传入0也能执行一次
		do {
			topLowLeftIter[--totalSizeNum] = decimaNum % radix;
			decimaNum /= radix;
		} while (decimaNum != 0);
	}
	// 传入的迭代器是右侧迭代器 隐含需要提前得知转换后的总位数大小 (不是二进制bitNum) 遵循[) 这里不用rbgain()
	template<class DigitIterator>
	void decimalToRadixTopLow(unsigned decimaNum, DigitIterator topLowRightIter, int radix) {
		// 要保证即使传入0也能执行一次
		do {
			*--topLowRightIter = decimaNum % radix;
			decimaNum /= radix;
		} while (decimaNum != 0);
	}


	// 返回任意进制的加权10进制数 (lowTopIter: 低位向高位逼近的迭代器::lowTopList.begain(); topLowList.rbegain();)
	template<class Integer, class DigitIterator>
	Integer radixLowTopToDecimal(DigitIterator lowTopIter, Integer radix, int totalSizeNum) {
		static Integer decimaNum;
		Integer powNum = 1;
		decimaNum = 0;
		//从低位(左侧是低位)向高位按权展开 i是幂
		for (int i = 0; i < totalSizeNum; ++i) {
			decimaNum += lowTopIter[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return decimaNum;
	}
	// 存储方式是top -> low
	template<class Integer, class DigitIterator>
	static Integer radixTopLowToDecimal(DigitIterator topLowIter, Integer radix, int totalSizeNum) {
		static Integer decimaNum;
		Integer powNum = 1;
		decimaNum = 0;
		//从低位(右侧是低位)向高位按权展开 i是幂
		for (int i = totalSizeNum - 1; i >= 0; --i) {
			decimaNum += topLowIter[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return decimaNum;
	}
};
