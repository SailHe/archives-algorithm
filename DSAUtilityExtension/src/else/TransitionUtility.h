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
	//ȥ��ǰ��0 ��ֵΪ0 ����"0"
	DSAUTILITYEXTENSION_API std::string formatString(std::string const &num);

	// ���㲢����ָ��������Ӧ�Ĳ��� (2�����޶�)
	// ԭ���ַ���
	std::string calcComplementCode(std::string &topLowOriginBinCode);
	// ԭ�������б�
	std::string calcComplementCode(DigitArray &topLowOriginCode);
	// 10������ֵ
	std::string calcComplementCode(int decNum);

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
		StandardExtend::iterate(binIterBegin, binIterEnd, [](DigitIterator currentIt) {
			*currentIt = *currentIt == 0 ? 1 : 0;
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
	}
	template<class DigitContainerIterator>
	std::string digitContainerToString(DigitContainerIterator const digitIterBegin, DigitContainerIterator const digitIterEnd) {
		std::string resultStr;
		resultStr.resize(digitIterEnd - digitIterBegin);
		digitContainerToCharContainer(digitIterBegin, digitIterEnd, resultStr.begin());
		return resultStr;
	}

	// �ַ�����{����, ��Сд��ĸ(����)} -> ��������
	template<class CharContainerIterator, class DigitContainerIterator>
	void charContainerToDigitContainer(CharContainerIterator charIterBegin, CharContainerIterator charIterEnd, DigitContainerIterator digitIterBegin, DigitContainerIterator digitIterEnd) {
		_ASSERT_EXPR(digitIterEnd - digitIterBegin == charIterEnd - charIterBegin, "������С�������!");
		std::for_each(charIterBegin, charIterEnd, [&](char value) {
			*digitIterBegin = toIntNum(value);
			++digitIterBegin;
		});
	}
	DSAUTILITYEXTENSION_API void stringToDigitArray(char const *str, DigitArray &digitArray);
	DSAUTILITYEXTENSION_API void stringToDigitArray(std::string &str, DigitArray &digitArray);
	

	/** =========== ����(int)����(10)ת��; �м���ֵ���ܳ���int��Χ =========== **/

	 // 10�������� -> radix���Ƶ��������� (10��������, ���㹻�ռ�ĵ�λ->��λ���ֵ�����)
	 // ����λ��: totalSizeNum; ��������ڲ���������������������
	template<class DigitIterator>
	int decimalToRadixLowTopBase(unsigned decimaNum, DigitIterator leftIter, int radix) {
		static int index;
		index = -1;
		// Ҫ��֤��ʹ����0Ҳ��ִ��һ�� ���
		do {
			*leftIter++ = decimaNum % radix;
			++index;
			decimaNum /= radix;
		} while (decimaNum != 0);
		return index + 1;
	}
	// ��radixȡ��, ��������;
	template<class DigitIterator>
	int decimalToRadixTopLow_PreDel(unsigned decimaNum, DigitIterator leftIter, DigitIterator rightIter, int radix) {
		int totalSizeNum = decimalToRadixLowTopBase(decimaNum, leftIter, radix);
		_ASSERT_EXPR(totalSizeNum == rightIter - leftIter, "����λԽ��!");
		// ���� decimalToRadixLowTopBase ��ת; radixLowTopToDecimalBase ������д��
		std::reverse(leftIter, rightIter);
		return totalSizeNum;
	}
	// ---- Recommend (checkBoundaryCondition: �߽��������, Ĭ�Ͽ���, ��ʾ����ķ�Χ�Ƿ���ת�����ʵ�ʷ�Χ��С���)
	template<class DigitIterator>
	void decimalToRadixTopLow(unsigned decimaNum, DigitIterator leftIter, DigitIterator rightIter, int radix, bool checkBoundaryCondition = true) {
		// Ҫ��֤��ʹ����0Ҳ��ִ��һ��
		do {
			*--rightIter = decimaNum % radix;
			decimaNum /= radix;
		} while (decimaNum != 0);
		// ����ʱӦ��ǡ�����
		_ASSERT_EXPR(!checkBoundaryCondition || leftIter == rightIter, "����λԽ��!");
	}
	template<class DigitArrayIterator>
	void decimalToRadixTopLow(unsigned decimaNum, DigitArrayIterator position, int radix, int totalSizeNum) {
		// Ҫ��֤��ʹ����0Ҳ��ִ��һ��
		do {
			position[--totalSizeNum] = decimaNum % radix;
			decimaNum /= radix;
		} while (decimaNum != 0);
	}


	// ---- Recommend ����λȨ����ʽ
	template<class Integer, class DigitIterator>
	void radixTopLowToDecimalTopLow(DigitIterator originLeftIter, DigitIterator originRightIter, DigitIterator targetLeftIter, DigitIterator targetRightIter, Integer originRadix) {
		// λȨ
		Integer powNum = 1;
		_ASSERT_EXPR(originRightIter - originLeftIter == targetRightIter - targetLeftIter, "��Χ��С��ͬ!");
		// �ӵ�λ(�Ҳ��ǵ�λ)���λ��Ȩչ�� Ȼ����ͬ���ķ�ʽ����Ŀ����������
		// && targetLeftIter != targetRightIter
		while (originLeftIter != originRightIter) {
			*--targetRightIter = *--originRightIter * powNum;
			powNum *= originRadix;
		}
	}

	// ���ص�λ����λ��Ȩչ���Ķ���ʽ֮��[leftIter, rightIter)������radix; ��: ����������Ƶļ�Ȩ10������
	template<class Integer, class DigitIterator>
	Integer radixLowTopToDecimalBase(DigitIterator leftIter, DigitIterator rightIter, Integer radix) {
		static Integer decimaNum;
		Integer powNum = 1;
		decimaNum = 0;
		//�ӵ�λ(����ǵ�λ)���λ��Ȩչ�� i����
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
		//�ӵ�λ(�Ҳ��ǵ�λ)���λ��Ȩչ�� i����
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
		//�ӵ�λ(�Ҳ��ǵ�λ)���λ��Ȩչ�� i����
		for (int i = totalSizeNum - 1; i >= 0; --i) {
			decimaNum += position[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return decimaNum;
	}
};
