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
	6. ���Ƽ�ʹ��ArrayContainer{DigitArray}:
	   ���������������ʹ�������������'[]', ��ʹ[����������]
	7. ����������: �����˵������ĳ��󼶱�, ʹ֮����֧���������ֵ�ĵ������Ӽ�����
	*/

	/*
	//ֻ���һ�����Ϳ����滻
	typedef int *DigitArray;
	typedef DigitArray DigitIterator;
	*/
	// �ǻ���������������� (�������ַ���) ԭ��: DigitBitArray(����λԪ����)
	typedef std::vector<int> DigitArray;
	// BitIter -> Biter
	typedef DigitArray::iterator DigitIterator;

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
	DSAUTILITYEXTENSION_API std::string bigPlush(std::string const &topLowNumA, std::string const &topLowNumB
		, std::string &topLowSum, int radix = 10);
	//ȥ��ǰ��0 ��ֵΪ0 ����"0"
	DSAUTILITYEXTENSION_API std::string formatString(std::string const &num);

	// ���㲢����ָ��������Ӧ�Ĳ��� (2�����޶�)
	// ԭ���ַ���
	DSAUTILITYEXTENSION_API std::string calcComplementCode(std::string &topLowOriginBinCode);
	// ԭ�����ֵ�������Χ
	template<class DigitIterator>
	std::string calcComplementCode(DigitIterator topLowOriginCodeLeftIter, DigitIterator topLowOriginCodeRightIter) {
		static DigitArray topLowOriginDigitBufferList;
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
	// 10������ֵ(����������ֵ)
	DSAUTILITYEXTENSION_API std::string calcComplementCode(int decNum);

	// ɨ�貢��λ, �����޷��洢�ĸ�λ��λ(����ܴ洢�Ļ�����0)
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
		std::for_each(binIterBegin, binIterEnd, [](int &value) {
			_ASSERT_EXPR(StandardExtend::inRange(0, value, 2), "��������ֵֻ������{0, 1}!");
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
	DSAUTILITYEXTENSION_API DigitIterator stringToDigitArray(char const *str, DigitIterator digitBeginIter, int radix);
	// ����digitIterEnd
	DSAUTILITYEXTENSION_API DigitIterator stringToDigitArray(std::string &str, DigitIterator digitBeginIter, int radix);

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
	template<class Integer, class DigitIterator>
	std::pair<DigitIterator, DigitIterator> radixTopLowToDecimalTopLow(
		DigitIterator originRightIter, DigitIterator targetRightIter, Integer togetherSize, Integer originRadix) {
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
