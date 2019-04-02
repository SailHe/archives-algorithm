#pragma once
#include "../pch.h"
#include "../DSAUEdll.h"

namespace TransitionUtility {
	/*
	����(Integer): ��ʾ֧�����л�������(��ģ����)������(EG: int __int64 long long)
	lowTopIter: ��λ���λ�����ƽ��ĵ����� (EG: lowTopList.begain(); topLowList.rbegain();)
	lowTopList: leftIter(��λ->��λ) rightIter(��λ->��λ)
	topLowList: leftIter(��λ->��λ) rightIter(��λ->��λ)

	LowTop�洢���ڽ�λ, �����ڱ�̺͵���, ��ʾʱ��Ҫ���������, �����ת�Ļ�Ҳ��ҪO(N)
	TopLow�洢���ڱ�̺͵���, �����ڽ�λ, ��λʱ��ҪO(N)���ƶ�(�����ǰ׼���ý�λ�Ļ��Ϳ��Ա���, �����ǻᷢ��������λ��С����)

	PS:
	0. Radix(����) Base(����) Decimal(ʮ��λ��; С��)
	   Radix: �������; Decimal: ʮ����
	1. Digit��������Integer ����ָ����һ��������Ϊĳ�����µ�һλ(���Ǳ���λbit); ����λ(bit)רָ�����Ƶ�1λ
	2. �����������ص�: �ɵ���������������(���������std::vector); �Կ��Ի���std::vector().rbgain();
	3. Iterator ָ�߶ȳ����Container{S, Array, Linked, Map}�ĵ�����
	4. ʹ��[leftIter, rightIter)ʱ���ҵĺ������Ѿ�ָ���õ�
	   ��������Ҫ��ǰ��֪��Χ(ת�������λ����С); ��ʱҲ������rbgain(), ��Ϊ�˱��������ò���
	5. Position רָ��һ����ArrayContainer{S, Array}�ĵ�����, ��������;
	   ����sizeһͬʹ��, ��ʾposition����������size��Ԫ��λ(���API��δ����size, �����Լ��������, һ�㴫begin����)
	6. ���Ƽ�ʹ��ArrayContainer{DigitVarrays}:
	   ���������������ʹ�������������'[]', ��ʹ[����������]
	7. ����������: �����˵������ĳ��󼶱�, ʹ֮����֧���������ֵ�ĵ������Ӽ�����
	*/
/*
�ۺ���(Struct)��
	1. ����
	2. [û��]��Щ���ݵ���, �ṹ������: 
		���캯��(ʹ��new������ʱ���ܹ���);
		˽�л��ܱ����ĳ�Ա;
		����;
		�麯��(���ʹ��memset��ʼ���Ļ��麯����������);
����ƷǾۺ���
*/
	// �ɱ�������������(��̬����) �ɳ�: DigitBitArray(����λԪ����)
	class DSAUTILITYEXTENSION_API DigitVarrays {
		
		// ����(�����Ǿۺ���)
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
		// (�������ڴ�ָ���ָ��, �������С)
		static void reallocProcess(Digit **result, SizeType newSize) {
			*result = (Digit*)realloc(*result, sizeof(Digit)*newSize);
			assert(*result != NULL);
		}
		// (��ʼ���ڴ�ָ��, ������С, ��ʼ����С)
		static void init(Digit *result, SizeType keepSize, SizeType initSize) {
			if (initSize >= keepSize) {
				// ��ʼ��(�ų�ԭ�е�__size)
				memset(result + keepSize, 0, sizeof(Digit)*(initSize - keepSize));
			}
		}

		int *digitArr_ = NULL;
		// ��ʹ�õĴ�С(һ����__capacity��� �������ûɶ�û�ɾ��)
		SizeType __size = 0u;
		// �����С(�����������)
		SizeType __capacity = 0u;
	};

	//using DigitVariableArray = std::vector<int>;
	//using DigitVariableArrayIterator = DigitVariableArray::iterator;
	using DigitVariableArray = DigitVarrays;
	using DigitVariableArrayIterator = DigitVariableArray::Iterator;

	// UppercaseAscll

	// Integer(D) -> �ַ�{Alph, ����}
	DSAUTILITYEXTENSION_API char toAlphOrAscllNum(int num);
	// �ַ�{��ĸ, ����} -> Integer(D)
	DSAUTILITYEXTENSION_API int toIntNum(char alphOrAscllNum);
	// �ַ�{��ĸ, ����} -> Integer(radix) Base:toIntNum; �����쳣���
	DSAUTILITYEXTENSION_API int toRadixIntNum(char alphOrAscllNum, int radix = 10);
	// ������ת������(������������) ���ظ����ֵ���������
	DSAUTILITYEXTENSION_API char toAbs(int &number);

	/**
	* ������ƴ����ӷ�: ֧�ִ�Сд�����Լ����;
	* ֧�� 10������ + 26����д��ĸ + 26��Сд��ĸ ��ʾ��[2, 62] �ϼ�61�ֽ�����
	* �������������Ʋ������쳣(����10��������A);
	* ����std::string ֱ�Ӹ���string�ڴ�ʵ�� sum�����������ͬ
	**/
	DSAUTILITYEXTENSION_API std::string bigPlush(std::string &topLowNumA, std::string &topLowNumB
		, std::string &topLowSum, int radix = 10);
	// ���������Ƿ��н�λ(��Ϊtrue���ʾ�����λ�����ڼ��������λ��, ��������ǲ���Ļ�, ����һ����ʾ����������������)
	DSAUTILITYEXTENSION_API bool bigPlush(std::string const &topLowNumA, std::string const &topLowNumB
		, std::string &topLowSum, int radix = 10);
	//ȥ��ǰ��0 ��ֵΪ0 ����"0"
	DSAUTILITYEXTENSION_API std::string formatString(std::string const &num);

	// ɨ�貢��λ, �����޷��洢�ĸ�λ��λ(����ܴ洢�Ļ�����0)
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

	// ���߸�λ'����'���(���������sum�ĳ�ʼ��ֵ): ���������Чλ�����ĵĽ�λֵ
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

	// ���㲢����ָ��������Ӧ�Ĳ��� (2�����޶�); ������Unsigned��ô��ʾ�÷���ֻ�Ǽ��㲹��, �����ܲ����Ƿ���Ҫ���㲹��
	// �޷���λԭ���ַ���
	DSAUTILITYEXTENSION_API std::string calcUnsignedComplementCode(std::string &topLowOriginBinCode);
	// ���㲢�����޷��Ų��� (�޷���λԭ������ֵ�������Χ)
	template<class DigitIterator>
	std::string calcUnsignedComplementCode(DigitIterator topLowOriginCodeLeftIter, DigitIterator topLowOriginCodeRightIter) {
		static DigitVariableArray topLowOriginDigitBufferList;
		static std::string topLowOriginBinCodeBuffer;
		const static std::string one("1");
		topLowOriginBinCodeBuffer.clear();

		topLowOriginDigitBufferList.clear();
		topLowOriginDigitBufferList.assign(topLowOriginCodeLeftIter, topLowOriginCodeRightIter);
		// ����
		TransitionUtility::inverseCode(topLowOriginDigitBufferList.begin(), topLowOriginDigitBufferList.end());

		topLowOriginBinCodeBuffer.resize(topLowOriginDigitBufferList.size());
		TransitionUtility::digitContainerToCharContainer(
			topLowOriginDigitBufferList.begin(), topLowOriginDigitBufferList.end(), topLowOriginBinCodeBuffer.begin()
		);

		// ����
		TransitionUtility::bigPlush(topLowOriginBinCodeBuffer, one, topLowOriginBinCodeBuffer, 2);

		return topLowOriginBinCodeBuffer;
	}
	// ���㲢����Χ�ڵ��޷���ԭ���Ϊ����
	template<class DigitIterator>
	void unsignedComplementCode(DigitIterator topLowOriginCodeLeftIter, DigitIterator topLowOriginCodeRightIter) {
		// ����
		TransitionUtility::inverseCode(topLowOriginCodeLeftIter, topLowOriginCodeRightIter);
		// ����
		auto topLowOriginCodeRightIterTemp = --topLowOriginCodeRightIter;
		*topLowOriginCodeRightIter += 1;
		int carray = carryTopLow(topLowOriginCodeLeftIter, topLowOriginCodeRightIterTemp, 2);

	}
	// 10������ֵ �����޷���λ�Ĳ���
	DSAUTILITYEXTENSION_API std::string calcUnsignedComplementCode(unsigned decNum);

	// 10������ֵ(����������ֵ): �����з���λ�Ĳ���
	DSAUTILITYEXTENSION_API std::string calcComplementCode(int decNum);

	// λ������n�ı������ڵ�ǰ���������油0; [beginIter, currentIter).size() = kn
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
	// ��ÿһλ������ȡ��
	template<class DigitIterator>
	void inverseCode(DigitIterator binIterBegin, DigitIterator binIterEnd) {
		StandardExtend::iterate(binIterBegin, binIterEnd, [](DigitIterator currentIter) {
			auto &value = *currentIter;
			_ASSERT_EXPR(value == 0 || value == 1, "��ֵ����!");
			value = value == 0 ? 1 : 0;
		});
	}
	// ��ӡ���ֵ������е�����Ԫ��; WithSymbol: 10��������ʹ�÷������(10��ӦA)
	// (��������: �������ʼ<==>��֪λ��: outputDigitInRange(lowTopList.rbegin(), lowTopList.rbegin() + totalSizeNum);)
	template<class DigitIterator>
	void outputDigitInRange(DigitIterator digitIterBegin, DigitIterator digitIterEnd) {
		std::for_each(digitIterBegin, digitIterEnd, [](int currentValue) {
			putchar(toAlphOrAscllNum(currentValue));
		});
		puts("");
	}


	// �������� -> �ַ�����{����, ��Сд��ĸ(����)}
	template<class DigitContainerIterator, class CharContainerIterator>
	void digitContainerToCharContainer(DigitContainerIterator digitIterBegin, DigitContainerIterator digitIterEnd, CharContainerIterator charIter) {
		std::for_each(digitIterBegin, digitIterEnd, [&](char intNum) {
			*charIter = toAlphOrAscllNum(intNum);
			++charIter;
		});
		// �����ַ����������Ա��������һ��
		// *charIter = '\0';
	}
	template<class DigitContainerIterator>
	std::string digitContainerToString(DigitContainerIterator const digitIterBegin, DigitContainerIterator const digitIterEnd) {
		std::string resultStr;
		resultStr.resize(digitIterEnd - digitIterBegin);
		digitContainerToCharContainer(digitIterBegin, digitIterEnd, resultStr.begin());
		return resultStr;
	}

	// �ַ�����{����, ��Сд��ĸ(����)} -> ָ�����Ƶ���������
	// ����digitIterEnd
	template<class CharContainerIterator, class DigitContainerIterator>
	DigitContainerIterator charContainerToDigitContainer(CharContainerIterator charIterBegin, CharContainerIterator charIterEnd, DigitContainerIterator digitIterBegin, int radix) {
		std::for_each(charIterBegin, charIterEnd, [&](char value) {
			*digitIterBegin = toRadixIntNum(value, radix);
			++digitIterBegin;
		});
		return digitIterBegin;
	}
	// ����digitIterEnd
	template<class DigitIterator>
	DigitIterator stringToDigitArray(char const *str, DigitIterator digitBeginIter, int radix) {
		std::size_t len = std::strlen(str);
		return charContainerToDigitContainer(str, str + len, digitBeginIter, radix);
	}
	// ����digitIterEnd
	template<class DigitIterator>
	DigitIterator stringToDigitArray(std::string &str, DigitIterator digitBeginIter, int radix) {
		return charContainerToDigitContainer(str.begin(), str.end(), digitBeginIter, radix);
	}

	/** =========== ����(int)����(10)ת��; �м���ֵ���ܳ���int��Χ =========== **/
	/*
	��Χδ֪, ������Ԥ��һ�����·�Χ
	������, д������
	ǰ��: 
	Ϊʲô��Χ��δ֪��:
		1λ10������������Ҫ4λ2���Ʊ�ʾ(1010), ��10������2����ϵ��
		���һ��nλ10������ʵ���õ�bitλ����ԶС�������ϵ�ֵ4n
		EG: 10λ10�����������ֵ9999999999, ������Ҫ4*10=40bit, ��ʵ��ֻ��Ҫ36bit
	����ӿ��еĵ�����Ϊһ��, ��ô��ʾ�з�Χ��֤
	checkBoundaryCondition(�߽��������): ���ʵ��ת������봫��Ĵ洢��Χ��һ�� �򱨴�)
	�������۵��ǲ�����Χ�Ĳ���:
		�����������Ե�ԭ��(ĳ�����µ�1λ��һ��ֻ������һ�����µ�1λ)
		�Լ�����ת���������н�λ�Ĳ������ɱ���
		a. ������ǰ֪��ת��ǰ���ʵ��׼ȷλ��, Ψ���Ƚ���һ��ת��(������)
		b. b1-�ռ���ʱ��ì������: ��ǰ�����㹻������λ(���ʹ��λ��+Ԥ����λ), �ɱ����λʱ���ڴ�����
		b.1 ʹ��LowTop���͵Ľӿ�
			b2-˳�������: ͬ��ӿ�һ��ʹ���ܲ�������ЧӦ(��������ʹ�ú͵���)
		    b2-1-λ���㲹�������: (PS: ��2���ƻ���ת������ʹ��λ����)
		         (1λHEX<=>4λBIN)
		         [4]H -> [0,0,1,?] -β��> [0,0,1,0] -��ת> [0,1,0,0]
				 HEX->DEC:
		         [F]H -> [5,1]D -��ת> [1,5]D
			b4-��λ����������: ����Ԥ��Ԥ����λ, ��λ���ں���, ��λ��Է���
		b.2 ��������������½ӿ�: 
		ԭLowTop�ӿڸ���
		rightEndIter radixLowTopToDec(decNum, leftBginIter, radix);

		b2-1. ת��֮ǰ4λ����; �����ײ�
		   [4]H -> [?,1,0,0] -�ײ�> [0,1,0,0]
		b2-2. HEX->DEC:
		   [F]H -> [1,5]D
		leftBginIter radixTopLowToDec(decNum, rightEndIter, radix);

	PS
		1. ���������е�LowTop��ʾ�洢�ķ�ʽ, ����ʹ��ϰ����TopLow, ��������ʱLowTop������, ��������
		2. [leftBginIter, rightEndIter)��ʾ�����еĵ�����
		3. �ӿ�ʹ��
		 3-1. ���뻺�������ڽ���ת��
		 3-2. ���û������Ķ�Ӧ���������ղ�����begin, endֱ�Ӵ���
		 3-3. ��ȡ���ص�����, ����������ʹ�øõ�������һ�����ýӿ�
	ELSE
		���ڵ�����������(java��)�Դ�hasNext�жϹ���, C++����û��������ܵ����쳣, ���Ƿ���containerMaxSize(�������)����
		֮����û����ԭ����������digitNum, һ������Ϊ�����ֵ������ĳ�����;
		��һ�����������Ҫ����������ⲿ���м���O(N), ����ԭת���㷨������һ������
		��������API������޶ȵ�ʹ���˵������ĳ�����, ��û����ʧ����, ��ԭ�й������ֻ������(ԭ����API��֧��list)
		list<int> l(size);
	*/
	 // 10�������� -> radix���Ƶ��������� (10��������, ���㹻�ռ�ĵ�λ->��λ���ֵ�����)
	 // ����λ��: totalSizeNum; ��������ڲ���������������������
	template<class DigitIterator>
	DigitIterator decimalToRadixLowTopBase(unsigned decimalNum, DigitIterator leftBeginIter, int radix) {
		// Ҫ��֤��ʹ����0Ҳ��ִ��һ�� ���
		do {
			*leftBeginIter++ = decimalNum % radix;
			decimalNum /= radix;
		} while (decimalNum != 0);
		return leftBeginIter;
	}
	// ��radixȡ��, ��������;
	template<class DigitIterator>
	void decimalToRadixTopLow_PreDel(unsigned decimalNum, DigitIterator leftIter, DigitIterator rightIter, int radix) {
		auto realRightIter = decimalToRadixLowTopBase(decimalNum, leftIter, radix);
		_ASSERT_EXPR(realRightIter != rightIter, "����λԽ��!");
		std::reverse(leftIter, rightIter);
	}
	
	// ---- Recommend  realBeginIter
	template<class DigitIterator>
	DigitIterator decimalToRadixTopLow(unsigned decimalNum, DigitIterator rightIter, int radix) {
		// Ҫ��֤��ʹ����0Ҳ��ִ��һ��
		do {
			*--rightIter = decimalNum % radix;
			decimalNum /= radix;
		} while (decimalNum != 0);
		return rightIter;
	}
	template<class DigitIterator>
	void decimalToRadixTopLow(unsigned decimalNum, DigitIterator leftIter, DigitIterator rightIter, int radix) {
		rightIter = decimalToRadixTopLow(decimalNum, rightIter, radix);
		_ASSERT_EXPR(leftIter == rightIter, "����λԽ��!");
	}
	template<class DigitArrayIterator>
	void decimalToRadixTopLow_PreDel(unsigned decimalNum, DigitArrayIterator topPosition, int totalSizeNum, int radix) {
		// Ҫ��֤��ʹ����0Ҳ��ִ��һ��
		do {
			topPosition[--totalSizeNum] = decimalNum % radix;
			decimalNum /= radix;
		} while (decimalNum != 0);
	}

	/*
	��Χ��֪
	��д������
	*/

	// ---- Recommend ����Radix��DECλȨ����ʽ(Դ��λ������, Ŀ���λ������, ���ߵĹ�ͬ��С)
	// ����{realOriginLeftIter, realTargetLeftIter}
	template<class Integer, typename DigitIterator1, typename DigitIterator2>
	std::pair<DigitIterator1, DigitIterator2> radixTopLowToDecimalTopLow(
		DigitIterator1 originRightIter, DigitIterator2 targetRightIter, Integer togetherSize, Integer originRadix) {
		// λȨ
		Integer powNum = 1;
		// �ӵ�λ(�Ҳ��ǵ�λ)���λ��Ȩչ�� Ȼ����ͬ���ķ�ʽ����Ŀ����������
		while (togetherSize-- > 0) {
			*--targetRightIter = *--originRightIter * powNum;
			powNum *= originRadix;
		}
		// �û��϶�����֪��С��, ��δ���: �������� �� togetherSize
		// ����std::vector��������, ����list��˵����ڼ����togetherSize, ���ĳ���ض��ĵ�����������һЩ
		// _ASSERT_EXPR(originRightIter == originLeftIter && targetRightIter == targetLeftIter, "��Χ��С��ͬ!");
		return { originRightIter, targetRightIter };
	}

	/*
	���·����ĵ����������ķ�Χһ������֪��
	��������, д������
	�����������������������ڽӿ�һ��, ���ڼ���(���Ǵ���һ�����������������, ���Ҳ�ܱ��������)
	*/

	// ���ص�λ����λ��Ȩչ���Ķ���ʽ֮��[leftIter, rightIter)������radix; ��: ����������Ƶļ�Ȩ10������
	template<class Integer, class DigitIterator>
	Integer radixLowTopToDecimalBase(DigitIterator leftIter, DigitIterator rightIter, Integer radix) {
		static Integer decimalNum;
		Integer powNum = 1;
		decimalNum = 0;
		//�ӵ�λ(����ǵ�λ)���λ��Ȩչ�� i����
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
		//�ӵ�λ(�Ҳ��ǵ�λ)���λ��Ȩչ�� i����
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
		//�ӵ�λ(�Ҳ��ǵ�λ)���λ��Ȩչ�� i����
		for (int i = totalSizeNum - 1; i >= 0; --i) {
			decimalNum += topPosition[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return decimalNum;
	}
};
