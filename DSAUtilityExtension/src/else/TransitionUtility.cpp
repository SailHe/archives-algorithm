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


	std::string bigPlush(std::string &topLowNumA, std::string &topLowNumB, std::string &topLowSum, int radix) {
		_ASSERT_EXPR(StandardExtend::inRange(2, radix, 63), "�����ַ��޷����ֱ�ʾָ������!");
		std::size_t lenA = topLowNumA.length(), lenB = topLowNumB.length(), lenAB;
		//��0��
		std::string temp;
		//��λ����, ���߸�λ0����
		if (lenA > lenB) {
			temp.resize(lenA - lenB, '0');
			topLowNumB = temp + topLowNumB;
			lenAB = lenA;
		}
		else {
			temp.resize(lenB - lenA, '0');
			topLowNumA = temp + topLowNumA;
			lenAB = lenB;
		}
		if (topLowSum.length() < lenA) {
			topLowSum.resize(lenA, '0');
		}
		int ia = -1, carryNum = 0;
		//��ת������ǵ�λ
		int i = -1;
		for (Utility::toSignedNum(lenAB - 1, i); i >= 0; --i) {
			// int sumBit = (topLowNumA[i] - '0') + (topLowNumB[i] - '0') + carryNum;
			int sumBit = TransitionUtility::toRadixIntNum(topLowNumA[i], radix) + TransitionUtility::toRadixIntNum(topLowNumB[i], radix) + carryNum;
			// topLowSum[i] = sumBit % 10 + '0';
			topLowSum[i] = TransitionUtility::toAlphOrAscllNum(sumBit % radix);
			// carryNum = sumBit / 10;
			carryNum = sumBit / radix;
		}
		return carryNum == 0 ? topLowSum : (topLowSum = "1" + topLowSum);
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

	std::string calcComplementCode(std::string &topLowOriginBinCode) {
		DigitArray topLowOriginCode;
		TransitionUtility::stringToDigitArray(topLowOriginBinCode, topLowOriginCode, 2);
		return calcComplementCode(topLowOriginCode.begin(), topLowOriginCode.end());
	}
	std::string calcComplementCode(int decNum) {
		static DigitArray topLow;
		topLow.resize(MathExtend::calcDigitTotalSize(decNum, 2));
		// ԭ��
		TransitionUtility::decimalToRadixTopLow(decNum, topLow.begin(), topLow.end(), 2);
		return calcComplementCode(topLow.begin(), topLow.end());
	}

	void stringToDigitArray(char const *str, DigitArray &digitArray, int radix) {
		JCE::SizeType len = std::strlen(str);
		digitArray.resize(len);
		charContainerToDigitContainer(str, str + len, digitArray.begin(), digitArray.end(), radix);
	}
	void stringToDigitArray(std::string &str, DigitArray &digitArray, int radix) {
		digitArray.resize(str.length());
		charContainerToDigitContainer(str.begin(), str.end(), digitArray.begin(), digitArray.end(), radix);
	}
}
