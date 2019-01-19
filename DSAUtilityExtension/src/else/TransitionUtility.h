#pragma once
#include "../pch.h"
#include "../DSAUEdll.h"

namespace TransitionUtility {
	/*
	����(Integer) ��ʾ֧�����л�������(��ģ����)������(EG: int __int64 long long)
	Digit��������Integer ����ָ����һ��������Ϊĳ�����µ�һλ(���Ǳ���λbit)
	left right�����������ص�: �ɵ���������������(���������std::vector); �Կ��Ի���std::vector().rbgain();
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

	// Integer(D) -> �ַ�{Alph, ����}
	DSAUTILITYEXTENSION_API char toUppercaseAscllChar(int num);
	// �ַ�{��ĸ, ����} -> Integer(D)
	DSAUTILITYEXTENSION_API int toIntNum(char alphOrCharNum);
	// �ַ�{��ĸ, ����} -> Integer(radix) Base:toIntNum; �����쳣���
	DSAUTILITYEXTENSION_API int toRadixIntNum(char alphOrCharNum, int radix = 10);
	// ������ת������(������������) ���ظ����ֵ���������
	DSAUTILITYEXTENSION_API char toAbs(int &number);
	// ����n�ı���λ���0 originP~currentP(n*0)
	DSAUTILITYEXTENSION_API void complement(DigitIterator originP, DigitIterator currentP, int n);

	// �������� -> �ַ�����{����, ��Сд��ĸ(����)}
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

	// �ַ�����{����, ��Сд��ĸ(����)} -> ��������
	template<class CharContainerIterator, class DigitContainerIterator>
	void charContainerToDigitContainer(CharContainerIterator charIterBegin, CharContainerIterator charIterEnd, DigitContainerIterator digitIter) {
		std::for_each(charIterBegin, charIterEnd, [&](char value) {
			*digitIter = toIntNum(value);
			++digitIter;
		});
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
			putchar(toUppercaseAscllChar(currentValue));
		});
		puts("");
	}

	/**
	 * ����(int)����(10)ת��; �м���ֵ���ܳ���int��Χ
	 **/

	 // ���� -> ���� (��Ҫת����10��������, ��λ���λ�洢����ֵ���������)
	 // ����λ��: totalSizeNum; �ı��������
	template<class DigitIterator>
	int decimalToRadixLowTop(unsigned decimaNum, DigitIterator lowTopIter, int radix) {
		static int index;
		index = -1;
		// Ҫ��֤��ʹ����0Ҳ��ִ��һ��
		do {
			*lowTopIter++ = decimaNum % radix;
			++index;
			decimaNum /= radix;
		} while (decimaNum != 0);
		return index + 1;
	}
	// ����decimalToRadixLowTop
	template<class DigitIterator>
	int decimalToRadixTopLow_baseLowTop(unsigned decimaNum, DigitIterator topLowIter, int radix) {
		int totalSizeNum = decimalToRadixLowTop(decimaNum, topLowIter, radix);
		std::reverse(topLowIter, topLowIter + totalSizeNum);
		return totalSizeNum;
	}

	// !!!���Ƽ�ʹ��!!!: ������������ʹ�����������'[]'; ʹ����������(���󼶱�)->����֧���������ֵ�ĵ������Ӽ�����
	// ��Ҫ��ǰ��֪ת�������λ����С(���Ƕ�����bitNum)
	template<class DigitIterator>
	void decimalToRadixTopLow(unsigned decimaNum, DigitIterator topLowLeftIter, int radix, int totalSizeNum) {
		// Ҫ��֤��ʹ����0Ҳ��ִ��һ��
		do {
			topLowLeftIter[--totalSizeNum] = decimaNum % radix;
			decimaNum /= radix;
		} while (decimaNum != 0);
	}
	// ����ĵ��������Ҳ������ ������Ҫ��ǰ��֪ת�������λ����С (���Ƕ�����bitNum) ��ѭ[) ���ﲻ��rbgain()
	template<class DigitIterator>
	void decimalToRadixTopLow(unsigned decimaNum, DigitIterator topLowRightIter, int radix) {
		// Ҫ��֤��ʹ����0Ҳ��ִ��һ��
		do {
			*--topLowRightIter = decimaNum % radix;
			decimaNum /= radix;
		} while (decimaNum != 0);
	}


	// ����������Ƶļ�Ȩ10������ (lowTopIter: ��λ���λ�ƽ��ĵ�����::lowTopList.begain(); topLowList.rbegain();)
	template<class Integer, class DigitIterator>
	Integer radixLowTopToDecimal(DigitIterator lowTopIter, Integer radix, int totalSizeNum) {
		static Integer decimaNum;
		Integer powNum = 1;
		decimaNum = 0;
		//�ӵ�λ(����ǵ�λ)���λ��Ȩչ�� i����
		for (int i = 0; i < totalSizeNum; ++i) {
			decimaNum += lowTopIter[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return decimaNum;
	}
	// �洢��ʽ��top -> low
	template<class Integer, class DigitIterator>
	static Integer radixTopLowToDecimal(DigitIterator topLowIter, Integer radix, int totalSizeNum) {
		static Integer decimaNum;
		Integer powNum = 1;
		decimaNum = 0;
		//�ӵ�λ(�Ҳ��ǵ�λ)���λ��Ȩչ�� i����
		for (int i = totalSizeNum - 1; i >= 0; --i) {
			decimaNum += topLowIter[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return decimaNum;
	}
};
