#pragma once
#include "../pch.h"
#include "../DSAUEdll.h"

namespace TransitionUtility {
	/*
	整数(Integer): 表示支持所有基本运算(含模运算)的类型(EG: int __int64 long long)
	lowTopIter: 低位向高位自增逼近的迭代器 (EG: lowTopList.begain(); topLowList.rbegain();)
	lowTopList: leftIter(低位->高位) rightIter(高位->低位)
	topLowList: leftIter(高位->低位) rightIter(低位->高位)

	LowTop存储利于进位, 不利于编程和调试, 显示时需要反过来输出, 如果翻转的话也需要O(N)
	TopLow存储利于编程和调试, 不利于进位, 进位时需要O(N)的移动(如果提前准备好进位的话就可以避免, 尤其是会发生大量进位的小进制)

	PS:
	0. Radix(基数) Base(基础) Decimal(十进位的; 小数)
	   Radix: 任意进制; Decimal: 十进制
	1. Digit的类型是Integer 其特指将这一个整数作为某进制下的一位(不是比特位bit); 比特位(bit)专指二进制的1位
	2. 迭代器泛型特点: 可迭代的容器都可以(比如数组和std::vector); 仍可以活用std::vector().rbgain();
	3. Iterator 指高度抽象的Container{S, Array, Linked, Map}的迭代器
	4. 使用[leftIter, rightIter)时左右的含义是已经指定好的
	   且隐含需要提前得知范围(转换后的总位数大小); 此时也可以用rbgain(), 但为了避免混淆最好不用
	5. Position 专指低一级的ArrayContainer{S, Array}的迭代器, 不分左右;
	   常与size一同使用, 表示position后面有至少size个元素位(如果API中未出现size, 则需自己把握情况, 一般传begin就行)
	6. 不推荐使用ArrayContainer{DigitArray}:
	   此类迭代器泛型中使用了数组运算符'[]', 其使[迭代器降级]
	7. 迭代器降级: 降低了迭代器的抽象级别, 使之必须支持任意合理值的迭代器加减运算
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

	// UppercaseAscll

	// Integer(D) -> 字符{Alph, 数字}
	DSAUTILITYEXTENSION_API char toAlphOrAscllNum(int num);
	// 字符{字母, 数字} -> Integer(D)
	DSAUTILITYEXTENSION_API int toIntNum(char alphOrAscllNum);
	// 字符{字母, 数字} -> Integer(radix) Base:toIntNum; 加了异常检测
	DSAUTILITYEXTENSION_API int toRadixIntNum(char alphOrAscllNum, int radix = 10);
	// 将数字转换正数(正数仍是正数) 返回该数字的正负符号
	DSAUTILITYEXTENSION_API char toAbs(int &number);

	/**
	* 任意进制大数加法: 支持大小写传入以及输出;
	* 支持 10个数字 + 26个大写字母 + 26个小写字母 表示的[2, 62] 合计61种进制数
	* 若传入参数与进制不符报异常(比如10进制输入A);
	* 基于std::string 直接更改string内存实现 sum可以与加数相同
	**/
	DSAUTILITYEXTENSION_API std::string bigPlush(std::string &topLowNumA, std::string &topLowNumB
		, std::string &topLowSum, int radix = 10);
	//去除前导0 若值为0 返回"0"
	DSAUTILITYEXTENSION_API std::string formatString(std::string const &num);

	// 计算并返回指定参数对应的补码 (2进制限定)
	// 原码字符串
	std::string calcComplementCode(std::string &topLowOriginBinCode);
	// 原码数字列表
	std::string calcComplementCode(DigitArray &topLowOriginCode);
	// 10进制真值
	std::string calcComplementCode(int decNum);

	// 扫描并进位, 返回无法存储的高位进位(如果能存储的话返回0)
	template<class DigitIterator, class Integer>
	Integer carryTopLow(DigitIterator leftIter, DigitIterator rightIter, Integer radix) {
		Integer carry = 0;
		while(leftIter != rightIter) {
			--rightIter;
			*rightIter += carry;
			carry = *rightIter / radix;
			*rightIter %= radix;
		}
		return carry;
	}

	// 位数不足n的倍数则在当前迭代器后面补0; [beginIter, currentIter).size() = kn
	template<class DigitArrayIterator>
	void zeroComplementAfterEnd(DigitArrayIterator beginIter, DigitArrayIterator currentIter, int n) {
		int residueBit = (currentIter - beginIter) % n;
		if (residueBit != 0) {
			residueBit = n - residueBit;
			while (residueBit-- > 0) {
				currentIter[residueBit] = 0;
			}
		}
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
			putchar(toAlphOrAscllNum(currentValue));
		});
		puts("");
	}


	// 数字容器 -> 字符容器{数字, 大小写字母(区分)}
	template<class DigitContainerIterator, class CharContainerIterator>
	void digitContainerToCharContainer(DigitContainerIterator digitIterBegin, DigitContainerIterator digitIterEnd, CharContainerIterator charIter) {
		std::for_each(digitIterBegin, digitIterEnd, [&](char intNum) {
			*charIter = toAlphOrAscllNum(intNum);
			++charIter;
		});
	}
	template<class DigitContainerIterator>
	std::string digitContainerToString(DigitContainerIterator const digitIterBegin, DigitContainerIterator const digitIterEnd) {
		std::string resultStr;
		resultStr.resize(digitIterEnd - digitIterBegin);
		digitContainerToCharContainer(digitIterBegin, digitIterEnd, resultStr.begin());
		return resultStr;
	}

	// 字符容器{数字, 大小写字母(区分)} -> 数字容器
	template<class CharContainerIterator, class DigitContainerIterator>
	void charContainerToDigitContainer(CharContainerIterator charIterBegin, CharContainerIterator charIterEnd, DigitContainerIterator digitIterBegin, DigitContainerIterator digitIterEnd) {
		_ASSERT_EXPR(digitIterEnd - digitIterBegin == charIterEnd - charIterBegin, "容器大小必须相等!");
		std::for_each(charIterBegin, charIterEnd, [&](char value) {
			*digitIterBegin = toIntNum(value);
			++digitIterBegin;
		});
	}
	DSAUTILITYEXTENSION_API void stringToDigitArray(char const *str, DigitArray &digitArray);
	DSAUTILITYEXTENSION_API void stringToDigitArray(std::string &str, DigitArray &digitArray);
	

	/** =========== 基础(int)进制(10)转换; 中间数值不能超出int范围 =========== **/

	 // 10进制数字 -> radix进制的数字数组 (10进制数字, 有足够空间的低位->高位数字迭代器)
	 // 返回位数: totalSizeNum; 结果储存在参数迭代器所处的容器中
	template<class DigitIterator>
	int decimalToRadixLowTopBase(unsigned decimaNum, DigitIterator leftIter, int radix) {
		static int index;
		index = -1;
		// 要保证即使传入0也能执行一次 左侧
		do {
			*leftIter++ = decimaNum % radix;
			++index;
			decimaNum /= radix;
		} while (decimaNum != 0);
		return index + 1;
	}
	// 除radix取余, 逆序排列;
	template<class DigitIterator>
	int decimalToRadixTopLow_PreDel(unsigned decimaNum, DigitIterator leftIter, DigitIterator rightIter, int radix) {
		int totalSizeNum = decimalToRadixLowTopBase(decimaNum, leftIter, radix);
		_ASSERT_EXPR(totalSizeNum == rightIter - leftIter, "数字位越界!");
		// 基于 decimalToRadixLowTopBase 反转; radixLowTopToDecimalBase 不便于写出
		std::reverse(leftIter, rightIter);
		return totalSizeNum;
	}
	// ---- Recommend (checkBoundaryCondition: 边界条件检测, 默认开启, 表示传入的范围是否与转换后的实际范围大小相等)
	template<class DigitIterator>
	void decimalToRadixTopLow(unsigned decimaNum, DigitIterator leftIter, DigitIterator rightIter, int radix, bool checkBoundaryCondition = true) {
		// 要保证即使传入0也能执行一次
		do {
			*--rightIter = decimaNum % radix;
			decimaNum /= radix;
		} while (decimaNum != 0);
		// 结束时应该恰好相等
		_ASSERT_EXPR(!checkBoundaryCondition || leftIter == rightIter, "数字位越界!");
	}
	template<class DigitArrayIterator>
	void decimalToRadixTopLow(unsigned decimaNum, DigitArrayIterator position, int radix, int totalSizeNum) {
		// 要保证即使传入0也能执行一次
		do {
			position[--totalSizeNum] = decimaNum % radix;
			decimaNum /= radix;
		} while (decimaNum != 0);
	}


	// ---- Recommend 计算位权多项式
	template<class Integer, class DigitIterator>
	void radixTopLowToDecimalTopLow(DigitIterator originLeftIter, DigitIterator originRightIter, DigitIterator targetLeftIter, DigitIterator targetRightIter, Integer originRadix) {
		// 位权
		Integer powNum = 1;
		_ASSERT_EXPR(originRightIter - originLeftIter == targetRightIter - targetLeftIter, "范围大小不同!");
		// 从低位(右侧是低位)向高位按权展开 然后以同样的方式存在目标容器里面
		// && targetLeftIter != targetRightIter
		while (originLeftIter != originRightIter) {
			*--targetRightIter = *--originRightIter * powNum;
			powNum *= originRadix;
		}
	}

	// 返回低位到高位按权展开的多项式之和[leftIter, rightIter)底数是radix; 即: 返回任意进制的加权10进制数
	template<class Integer, class DigitIterator>
	Integer radixLowTopToDecimalBase(DigitIterator leftIter, DigitIterator rightIter, Integer radix) {
		static Integer decimaNum;
		Integer powNum = 1;
		decimaNum = 0;
		//从低位(左侧是低位)向高位按权展开 i是幂
		while(leftIter != rightIter) {
			decimaNum += *leftIter * powNum;
			++leftIter;
			powNum *= radix;
		}
		return decimaNum;
	}
	// ---- Recommend
	template<class Integer, class DigitIterator>
	Integer radixTopLowToDecimal(DigitIterator leftIter, DigitIterator rightIter, Integer radix) {
		static Integer decimaNum;
		Integer powNum = 1;
		decimaNum = 0;
		//从低位(右侧是低位)向高位按权展开 i是幂
		while (leftIter != rightIter) {
			decimaNum += *--rightIter * powNum;
			powNum *= radix;
		}
		return decimaNum;
	}
	template<class Integer, class DigitArrayIterator>
	Integer radixTopLowToDecimal_PreDel(DigitArrayIterator position, int totalSizeNum, Integer radix) {
		static Integer decimaNum;
		Integer powNum = 1;
		decimaNum = 0;
		//从低位(右侧是低位)向高位按权展开 i是幂
		for (int i = totalSizeNum - 1; i >= 0; --i) {
			decimaNum += position[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return decimaNum;
	}
};
