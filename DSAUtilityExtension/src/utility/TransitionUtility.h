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
	6. 不推荐使用ArrayContainer{DigitVarrays}:
	   此类迭代器泛型中使用了数组运算符'[]', 其使[迭代器降级]
	7. 迭代器降级: 降低了迭代器的抽象级别, 使之必须支持任意合理值的迭代器加减运算
	*/
/*
聚合类(Struct)：
	1. 数组
	2. [没有]这些内容的类, 结构和联合: 
		构造函数(使用new操作符时才能构造);
		私有或受保护的成员;
		基类;
		虚函数(如果使用memset初始化的话虚函数表会出问题);
否则称非聚合类
*/
	// 可变整数数字数组(动态数组) 旧称: DigitBitArray(数字位元数组)
	class DSAUTILITYEXTENSION_API DigitVarrays {
		
		// 数字(必须是聚合类)
		typedef int Digit;
		typedef unsigned SizeType;

	public:

		// BitIter -> Biter
		typedef Digit *Iterator;

		DigitVarrays() {
			resize(0);
		}
		DigitVarrays(SizeType size) {
			resize(size);
		}
		~DigitVarrays() {
			free(digitArr_);
			digitArr_ = NULL;
		}
		void resize(SizeType newSize) {
			if (newSize <= __capacity) {
				// DNT
			}
			else {
				reallocProcess(&digitArr_, newSize);
			}
			init(digitArr_, __size, newSize);
			__size = newSize;
		}
		void reserver(SizeType newCapacity) {
			reallocProcess(&digitArr_, newCapacity);
			// init(digitArr_, __capacity, newCapacity);
			__capacity = newCapacity;
		}
		template<typename Iterator1>
		void assign(Iterator1 begin, Iterator1 end) {
			Iterator tBegin = this->begin();
			Iterator tEnd = this->end();
			while (begin != end) {
				if (tBegin != tEnd) {
					*tBegin = *begin;
					++tBegin;
				}
				else {
					push_back(*begin);
				}
				++begin;
			}
		}
		void push_back(Digit ele) {
			if (__size < __capacity) {
				// DNT
			}
			else {
				reserver(2 * __size + 1);
			}
			resize(__size + 1);
			*(end() - 1) = ele;
		}
		SizeType size() {
			return __size;
		}
		void clear() {
			init(digitArr_, 0, __size);
			__size = 0;
		}
		Digit &operator[](int i) {
			assert(0 <= i);
			assert((SizeType)i < __size);
			return digitArr_[i];
		}
		Iterator begin() {
			return digitArr_;
		}
		Iterator end() {
			return digitArr_ + __size;
		}

	private:
		// (重申请内存指针的指针, 新申请大小)
		static void reallocProcess(Digit **result, SizeType newSize) {
			*result = (Digit*)realloc(*result, sizeof(Digit)*newSize);
			assert(*result != NULL);
		}
		// (初始化内存指针, 保留大小, 初始化大小)
		static void init(Digit *result, SizeType keepSize, SizeType initSize) {
			if (initSize >= keepSize) {
				// 初始化(排除原有的__size)
				memset(result + keepSize, 0, sizeof(Digit)*(initSize - keepSize));
			}
		}

		int *digitArr_ = NULL;
		// 已使用的大小(一般与__capacity相等 这个可能没啥用会删除)
		SizeType __size = 0u;
		// 数组大小(已申请的容量)
		SizeType __capacity = 0u;
	};

	//using DigitVariableArray = std::vector<int>;
	//using DigitVariableArrayIterator = DigitVariableArray::iterator;
	using DigitVariableArray = DigitVarrays;
	using DigitVariableArrayIterator = DigitVariableArray::Iterator;

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
	// 返回最终是否有进位(若为true则表示结果的位数多于加数的最大位数, 如果参数是补码的话, 并不一定表示结果产生了溢出错误)
	DSAUTILITYEXTENSION_API bool bigPlush(std::string const &topLowNumA, std::string const &topLowNumB
		, std::string &topLowSum, int radix = 10);
	//去除前导0 若值为0 返回"0"
	DSAUTILITYEXTENSION_API std::string formatString(std::string const &num);

	// 扫描并进位, 返回无法存储的高位进位(如果能存储的话返回0)
	template<class DigitIterator, class Integer>
	Integer carryTopLow(DigitIterator leftIter, DigitIterator rightIter, Integer radix) {
		Integer carry = 0;
		while (leftIter != rightIter) {
			--rightIter;
			*rightIter += carry;
			carry = *rightIter / radix;
			*rightIter %= radix;
		}
		return carry;
	}

	// 短者高位'添数'相加(这个数就是sum的初始化值): 返回最高有效位产生的的进位值
	template<class DigitIterator>
	int topLowTogetherPlush(
		DigitIterator lhsRightIter, int lhsSize
		, DigitIterator rhsRightIter, int rhsSize
		, DigitIterator sumRightIter) {
		auto sumRightIterTmp = sumRightIter;
		while (lhsSize-- > 0) {
			*--sumRightIter = *--lhsRightIter;
		}
		while (rhsSize-- > 0) {
			*--sumRightIterTmp += *--rhsRightIter;
		}
		int lhsTopBit = *lhsRightIter;
		int rhsTopBit = *rhsRightIter;
		int topCarry = (rhsTopBit + lhsTopBit) / 2;
		return topCarry;
	}

	template<class DigitIterator>
	int topLowTogetherPlush(
		DigitIterator lhsLeftIter, DigitIterator lhsRightIter
		, DigitIterator rhsLeftIter, DigitIterator rhsRightIter
		, DigitIterator sumRightIter) {
		auto sumRightIterTmp = sumRightIter;
		while (lhsRightIter != lhsLeftIter) {
			*--sumRightIter = *--lhsRightIter;
		}
		while (rhsRightIter != rhsLeftIter) {
			*--sumRightIterTmp += *--rhsRightIter;
		}
		int lhsTopBit = *lhsRightIter;
		int rhsTopBit = *rhsRightIter;
		int topCarry = (rhsTopBit + lhsTopBit) / 2;
		return topCarry;
	}

	// 计算并返回指定参数对应的补码 (2进制限定); 若带有Unsigned那么表示该方法只是计算补码, 而不管参数是否需要计算补码
	// 无符号位原码字符串
	DSAUTILITYEXTENSION_API std::string calcUnsignedComplementCode(std::string &topLowOriginBinCode);
	// 计算并返回无符号补码 (无符号位原码的数字迭代器范围)
	template<class DigitIterator>
	std::string calcUnsignedComplementCode(DigitIterator topLowOriginCodeLeftIter, DigitIterator topLowOriginCodeRightIter) {
		static DigitVariableArray topLowOriginDigitBufferList;
		static std::string topLowOriginBinCodeBuffer;
		const static std::string one("1");
		topLowOriginBinCodeBuffer.clear();

		topLowOriginDigitBufferList.clear();
		topLowOriginDigitBufferList.assign(topLowOriginCodeLeftIter, topLowOriginCodeRightIter);
		// 反码
		TransitionUtility::inverseCode(topLowOriginDigitBufferList.begin(), topLowOriginDigitBufferList.end());

		topLowOriginBinCodeBuffer.resize(topLowOriginDigitBufferList.size());
		TransitionUtility::digitContainerToCharContainer(
			topLowOriginDigitBufferList.begin(), topLowOriginDigitBufferList.end(), topLowOriginBinCodeBuffer.begin()
		);

		// 补码
		TransitionUtility::bigPlush(topLowOriginBinCodeBuffer, one, topLowOriginBinCodeBuffer, 2);

		return topLowOriginBinCodeBuffer;
	}
	// 计算并将范围内的无符号原码改为补码
	template<class DigitIterator>
	void unsignedComplementCode(DigitIterator topLowOriginCodeLeftIter, DigitIterator topLowOriginCodeRightIter) {
		// 反码
		TransitionUtility::inverseCode(topLowOriginCodeLeftIter, topLowOriginCodeRightIter);
		// 补码
		auto topLowOriginCodeRightIterTemp = --topLowOriginCodeRightIter;
		*topLowOriginCodeRightIter += 1;
		int carray = carryTopLow(topLowOriginCodeLeftIter, topLowOriginCodeRightIterTemp, 2);

	}
	// 10进制真值 返回无符号位的补码
	DSAUTILITYEXTENSION_API std::string calcUnsignedComplementCode(unsigned decNum);

	// 10进制真值(处理了正负值): 返回有符号位的补码
	DSAUTILITYEXTENSION_API std::string calcComplementCode(int decNum);

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
		StandardExtend::iterate(binIterBegin, binIterEnd, [](DigitIterator currentIter) {
			auto &value = *currentIter;
			_ASSERT_EXPR(value == 0 || value == 1, "二值数字!");
			value = value == 0 ? 1 : 0;
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
		// 由于字符串的特殊性必须申请多一个
		// *charIter = '\0';
	}
	template<class DigitContainerIterator>
	std::string digitContainerToString(DigitContainerIterator const digitIterBegin, DigitContainerIterator const digitIterEnd) {
		std::string resultStr;
		resultStr.resize(digitIterEnd - digitIterBegin);
		digitContainerToCharContainer(digitIterBegin, digitIterEnd, resultStr.begin());
		return resultStr;
	}

	// 字符容器{数字, 大小写字母(区分)} -> 指定进制的数字容器
	// 返回digitIterEnd
	template<class CharContainerIterator, class DigitContainerIterator>
	DigitContainerIterator charContainerToDigitContainer(CharContainerIterator charIterBegin, CharContainerIterator charIterEnd, DigitContainerIterator digitIterBegin, int radix) {
		std::for_each(charIterBegin, charIterEnd, [&](char value) {
			*digitIterBegin = toRadixIntNum(value, radix);
			++digitIterBegin;
		});
		return digitIterBegin;
	}
	// 返回digitIterEnd
	template<class DigitIterator>
	DigitIterator stringToDigitArray(char const *str, DigitIterator digitBeginIter, int radix) {
		std::size_t len = std::strlen(str);
		return charContainerToDigitContainer(str, str + len, digitBeginIter, radix);
	}
	// 返回digitIterEnd
	template<class DigitIterator>
	DigitIterator stringToDigitArray(std::string &str, DigitIterator digitBeginIter, int radix) {
		return charContainerToDigitContainer(str.begin(), str.end(), digitBeginIter, radix);
	}

	/** =========== 基础(int)进制(10)转换; 中间数值不能超出int范围 =========== **/
	/*
	范围未知, 但可以预判一个大致范围
	读数字, 写迭代器
	前言: 
	为什么范围是未知的:
		1位10进制数最少需要4位2进制表示(1010), 但10并不是2进制系的
		因此一个n位10进制数实际用的bit位可能远小于理论上的值4n
		EG: 10位10进制数的最大值9999999999, 理论需要4*10=40bit, 但实际只需要36bit
	如果接口中的迭代器为一对, 那么表示有范围验证
	checkBoundaryCondition(边界条件检测): 如果实际转换结果与传入的存储范围不一致 则报错)
	下面讨论的是不定范围的参数:
		由于上述明显的原因(某进制下的1位不一定只代表另一进制下的1位)
		以及进制转换过程中中进位的产生不可避免
		a. 若想提前知晓转换前后的实际准确位数, 唯有先进行一次转换(不可行)
		b. b1-空间与时间矛盾问题: 提前申请足够的数字位(最大使用位数+预留进位), 可避免进位时的内存申请
		b.1 使用LowTop类型的接口
			b2-顺序的问题: 同类接口一起使用能产生忽略效应(但不便于使用和调试)
		    b2-1-位运算补齐的问题: (PS: 基2进制互相转换可以使用位运算)
		         (1位HEX<=>4位BIN)
		         [4]H -> [0,0,1,?] -尾补> [0,0,1,0] -翻转> [0,1,0,0]
				 HEX->DEC:
		         [F]H -> [5,1]D -翻转> [1,5]D
			b4-进位问题有优势: 无论预不预留进位, 进位都在后面, 进位相对方便
		b.2 于是设计了以下新接口: 
		原LowTop接口改良
		rightEndIter radixLowTopToDec(decNum, leftBginIter, radix);

		b2-1. 转换之前4位清零; 或是首补
		   [4]H -> [?,1,0,0] -首补> [0,1,0,0]
		b2-2. HEX->DEC:
		   [F]H -> [1,5]D
		leftBginIter radixTopLowToDec(decNum, rightEndIter, radix);

	PS
		1. 方法命名中的LowTop表示存储的方式, 人类使用习惯是TopLow, 进制运算时LowTop更合理, 各有优势
		2. [leftBginIter, rightEndIter)表示容器中的迭代器
		3. 接口使用
		 3-1. 申请缓存区用于进制转换
		 3-2. 将该缓存区的对应迭代器按照参数名begin, end直接传入
		 3-3. 获取返回迭代器, 根据其意义使用该迭代器进一步调用接口
	ELSE
		由于迭代器理论上(java中)自带hasNext判断功能, C++中虽没有这个功能但会异常, 于是放弃containerMaxSize(最大容量)参数
		之所以没有像原先那样返回digitNum, 一方面是为了体现迭代器的抽象性;
		另一方面是如果需要计算可以在外部自行计算O(N), 而且原转换算法中少了一个变量
		现在这种API既最大限度的使用了迭代器的抽象性, 又没有损失性能, 与原有功能相比只增不减(原来的API不支持list)
		list<int> l(size);
	*/
	 // 10进制数字 -> radix进制的数字数组 (10进制数字, 有足够空间的低位->高位数字迭代器)
	 // 返回位数: totalSizeNum; 结果储存在参数迭代器所处的容器中
	template<class DigitIterator>
	DigitIterator decimalToRadixLowTopBase(unsigned decimalNum, DigitIterator leftBeginIter, int radix) {
		// 要保证即使传入0也能执行一次 左侧
		do {
			*leftBeginIter++ = decimalNum % radix;
			decimalNum /= radix;
		} while (decimalNum != 0);
		return leftBeginIter;
	}
	// 除radix取余, 逆序排列;
	template<class DigitIterator>
	void decimalToRadixTopLow_PreDel(unsigned decimalNum, DigitIterator leftIter, DigitIterator rightIter, int radix) {
		auto realRightIter = decimalToRadixLowTopBase(decimalNum, leftIter, radix);
		_ASSERT_EXPR(realRightIter != rightIter, "数字位越界!");
		std::reverse(leftIter, rightIter);
	}
	
	// ---- Recommend  realBeginIter
	template<class DigitIterator>
	DigitIterator decimalToRadixTopLow(unsigned decimalNum, DigitIterator rightIter, int radix) {
		// 要保证即使传入0也能执行一次
		do {
			*--rightIter = decimalNum % radix;
			decimalNum /= radix;
		} while (decimalNum != 0);
		return rightIter;
	}
	template<class DigitIterator>
	void decimalToRadixTopLow(unsigned decimalNum, DigitIterator leftIter, DigitIterator rightIter, int radix) {
		rightIter = decimalToRadixTopLow(decimalNum, rightIter, radix);
		_ASSERT_EXPR(leftIter == rightIter, "数字位越界!");
	}
	template<class DigitArrayIterator>
	void decimalToRadixTopLow_PreDel(unsigned decimalNum, DigitArrayIterator topPosition, int totalSizeNum, int radix) {
		// 要保证即使传入0也能执行一次
		do {
			topPosition[--totalSizeNum] = decimalNum % radix;
			decimalNum /= radix;
		} while (decimalNum != 0);
	}

	/*
	范围已知
	读写迭代器
	*/

	// ---- Recommend 计算Radix的DEC位权多项式(源低位迭代器, 目标低位迭代器, 两者的共同大小)
	// 返回{realOriginLeftIter, realTargetLeftIter}
	template<class Integer, typename DigitIterator1, typename DigitIterator2>
	std::pair<DigitIterator1, DigitIterator2> radixTopLowToDecimalTopLow(
		DigitIterator1 originRightIter, DigitIterator2 targetRightIter, Integer togetherSize, Integer originRadix) {
		// 位权
		Integer powNum = 1;
		// 从低位(右侧是低位)向高位按权展开 然后以同样的方式存在目标容器里面
		while (togetherSize-- > 0) {
			*--targetRightIter = *--originRightIter * powNum;
			powNum *= originRadix;
		}
		// 用户肯定是已知大小的, 如何传入: 迭代器组 或 togetherSize
		// 对于std::vector不必讨论, 对于list来说相对于计算出togetherSize, 获得某个特定的迭代器更困难一些
		// _ASSERT_EXPR(originRightIter == originLeftIter && targetRightIter == targetLeftIter, "范围大小不同!");
		return { originRightIter, targetRightIter };
	}

	/*
	以下方法的迭代器操作的范围一定是已知的
	读迭代器, 写入数字
	传入两个迭代器的优势在于接口一致, 便于记忆(但是传入一个迭代器如果传错了, 多半也能报错检测出来)
	*/

	// 返回低位到高位按权展开的多项式之和[leftIter, rightIter)底数是radix; 即: 返回任意进制的加权10进制数
	template<class Integer, class DigitIterator>
	Integer radixLowTopToDecimalBase(DigitIterator leftIter, DigitIterator rightIter, Integer radix) {
		static Integer decimalNum;
		Integer powNum = 1;
		decimalNum = 0;
		//从低位(左侧是低位)向高位按权展开 i是幂
		while(leftIter != rightIter) {
			decimalNum += *leftIter * powNum;
			++leftIter;
			powNum *= radix;
		}
		return decimalNum;
	}
	// ---- Recommend
	template<class Integer, class DigitIterator>
	Integer radixTopLowToDecimal(DigitIterator leftIter, DigitIterator rightIter, Integer radix) {
		static Integer decimalNum;
		Integer powNum = 1;
		decimalNum = 0;
		//从低位(右侧是低位)向高位按权展开 i是幂
		while (leftIter != rightIter) {
			decimalNum += *--rightIter * powNum;
			powNum *= radix;
		}
		return decimalNum;
	}
	template<class Integer, class DigitArrayIterator>
	Integer radixTopLowToDecimal_PreDel(DigitArrayIterator topPosition, Integer totalSizeNum, Integer radix) {
		static Integer decimalNum;
		Integer powNum = 1;
		decimalNum = 0;
		//从低位(右侧是低位)向高位按权展开 i是幂
		for (int i = totalSizeNum - 1; i >= 0; --i) {
			decimalNum += topPosition[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return decimalNum;
	}
};
