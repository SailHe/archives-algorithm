#include "TransitionUtility.h"
#include "../ExtendSpace.h"

namespace TransitionUtility{
	char toAlphOrAscllNum(int num) {
		return num < 10 ? num + '0'
			: num < 36 ? num - 10 + 'A'
			: num - 36 + 'a';
	}

	int toIntNum(char alphOrAscllNum) {
		int resultInteger = -1;
		// isalnum(); inRange();
		if ('0' <= alphOrAscllNum && alphOrAscllNum <= '9') {
			// 0 + alphOrAscllNum - '0';
			resultInteger = alphOrAscllNum - '0';
		}
		else {
			// isalpha();
			if ('A' <= alphOrAscllNum && alphOrAscllNum <= 'Z') {
				resultInteger = 10 + alphOrAscllNum - 'A';
			}
			else if ('a' <= alphOrAscllNum && alphOrAscllNum <= 'z') {
				// �����ַ�10��{0, 9} ��д��ĸ26��{A, Z} Сд��ĸ26��{a, z}
				resultInteger = 36 + alphOrAscllNum - 'a';
			}
			else {
				_ASSERT_EXPR(false, "�ַ��Ƿ�! ->�Ϸ��ַ�{����, ��Сд��ĸ}");
			}
		}
		return resultInteger;
	}

	int toRadixIntNum(char alphOrAscllNum, int radix) {
		int digitNum = toIntNum(alphOrAscllNum);
		// American Standard Code for Information Interchange��������Ϣ������׼����
		_ASSERT_EXPR(StandardExtend::inRange(0, digitNum, radix), "����ָ�����Ƶ�Asll�ַ�");
		return digitNum;
	}

	char toAbs(int &number) {
		return number < 0 ? (number = -number), '-' : '+';
	}

	// �ַ�������(δ��¶API) ���ز����Ĺ�ͬ����
	int zeroPolishing(std::string &topLowNumA, std::string &topLowNumB, std::string &topLowSum) {
		std::size_t lenA = topLowNumA.length(), lenB = topLowNumB.length(), maxLenAB = MIN_INT32;
		//��0��
		std::string temp;
		//��λ����, ���߸�λ0����
		if (lenA > lenB) {
			temp.resize(lenA - lenB, '0');
			topLowNumB = temp + topLowNumB;
			maxLenAB = lenA;
		}
		else {
			temp.resize(lenB - lenA, '0');
			topLowNumA = temp + topLowNumA;
			maxLenAB = lenB;
		}
		topLowSum.resize(maxLenAB, '0');
		return maxLenAB;
	}

	std::string bigPlush(std::string &topLowNumA, std::string &topLowNumB, std::string &topLowSum, int radix) {
		_ASSERT_EXPR(StandardExtend::inRange(2, radix, 63), "�����ַ��޷����ֱ�ʾָ������!");
		size_t togetherLenAB = zeroPolishing(topLowNumA, topLowNumB, topLowSum);
		int carryNum = 0;
		// �ұ��ǵ�λ
		int i = -1;
		for (Utility::toSignedNum(togetherLenAB - 1, i); i >= 0; --i) {
			int sumDigitNum = 
				TransitionUtility::toRadixIntNum(topLowNumA[i], radix)
				+ TransitionUtility::toRadixIntNum(topLowNumB[i], radix)
				+ carryNum;
			topLowSum[i] = TransitionUtility::toAlphOrAscllNum(sumDigitNum % radix);
			carryNum = sumDigitNum / radix;
		}
		return carryNum == 0 ? topLowSum : (topLowSum = "1" + topLowSum);
	}

	bool bigPlush(std::string const &topLowNumA, std::string const &topLowNumB, std::string &topLowSum, int radix) {
		_ASSERT_EXPR(StandardExtend::inRange(2, radix, 63), "�����ַ��޷����ֱ�ʾָ������!");
		int lenA, lenB, togetherLenAB = -1;
		Utility::toSignedNum(topLowNumA.length(), lenA);
		Utility::toSignedNum(topLowNumB.length(), lenB);
		if (lenA > lenB) {
			topLowSum.resize(lenA, '0');
			togetherLenAB = lenB;
		}
		else {
			topLowSum.resize(lenB, '0');
			togetherLenAB = lenA;
		}
		int carryNum = 0;
		// �ұ��ǵ�λ len��i���������з�����; һֱѭ��ֱ��lhs��rhs���������(�ڼ��κ�һ���ȵ�����϶�Ҫ���� ��ֵ��0)
		for (int i = 1; (lenA - i) >= 0 || (lenB - i) >= 0; ++i) {
			int sumDigitNum = 
				(lenA - i >= 0 ? TransitionUtility::toRadixIntNum(topLowNumA[lenA - i], radix) : 0)
				+ (lenB - i >= 0 ? TransitionUtility::toRadixIntNum(topLowNumB[lenB - i], radix) : 0)
				+ carryNum;
			topLowSum[topLowSum.length() - i] = TransitionUtility::toAlphOrAscllNum(sumDigitNum % radix);
			carryNum = sumDigitNum / radix;
		}
		return (carryNum == 0 ? false : (topLowSum = "1" + topLowSum, true));
	}

	std::string formatString(std::string const &num) {
		int sp = -1;
		for (auto it = num.begin(); it != num.end(); ++it) {
			++sp;
			if (*it != '0') {
				break;
			}
		}
		return sp == num.size() ? "0" : num.substr(sp);
	}

	std::string calcUnsignedComplementCode(std::string &topLowOriginBinCode) {
		DigitArray topLowOriginCode(topLowOriginBinCode.length());
		auto digitIterEnd = TransitionUtility::stringToDigitArray(topLowOriginBinCode, topLowOriginCode.begin(), 2);
		_ASSERT_EXPR(digitIterEnd == topLowOriginCode.end(), "rt");
		return calcUnsignedComplementCode(topLowOriginCode.begin(), digitIterEnd);
	}
	std::string calcUnsignedComplementCode(unsigned decNum) {
		static DigitArray topLowOriginDigitListBuffer;
		std::string complementCode;
		if (decNum == 0) {
			complementCode = "0";
		}
		else {
			topLowOriginDigitListBuffer.clear();
			topLowOriginDigitListBuffer.resize(MathExtend::calcDigitTotalSize(decNum, 2));
			// ԭ��
			TransitionUtility::decimalToRadixTopLow(
				decNum, topLowOriginDigitListBuffer.begin(), topLowOriginDigitListBuffer.end(), 2
			);
			complementCode = TransitionUtility::digitContainerToString(
				topLowOriginDigitListBuffer.begin(), topLowOriginDigitListBuffer.end()
			);
			complementCode = calcUnsignedComplementCode(topLowOriginDigitListBuffer.begin(), topLowOriginDigitListBuffer.end());
		}

		return complementCode;
	}
	std::string calcComplementCode(int decNum) {
		static DigitArray topLowOriginDigitListBuffer;
		std::string complementCode;
		if (decNum == 0) {
			complementCode = "00";
		}
		else {
			topLowOriginDigitListBuffer.clear();
			topLowOriginDigitListBuffer.resize(MathExtend::calcDigitTotalSize(decNum, 2));
			// ԭ��
			TransitionUtility::decimalToRadixTopLow(
				std::abs(decNum), topLowOriginDigitListBuffer.begin(), topLowOriginDigitListBuffer.end(), 2
			);
			if (decNum > 0) {
				complementCode = TransitionUtility::digitContainerToString(
					topLowOriginDigitListBuffer.begin(), topLowOriginDigitListBuffer.end()
				);
				complementCode = "0" + complementCode;
			}
			else {
				complementCode = calcUnsignedComplementCode(topLowOriginDigitListBuffer.begin(), topLowOriginDigitListBuffer.end());
				complementCode = "1" + complementCode;
			}
		}
		
		return complementCode;
	}

	DigitIterator stringToDigitArray(char const *str, DigitIterator digitBeginIter, int radix) {
		std::size_t len = std::strlen(str);
		return charContainerToDigitContainer(str, str + len, digitBeginIter, radix);
	}
	DigitIterator stringToDigitArray(std::string &str, DigitIterator digitBeginIter, int radix) {
		return charContainerToDigitContainer(str.begin(), str.end(), digitBeginIter, radix);
	}
}
