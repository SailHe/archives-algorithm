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
				// 数字字符10个{0, 9} 大写字母26个{A, Z} 小写字母26个{a, z}
				resultInteger = 36 + alphOrAscllNum - 'a';
			}
			else {
				_ASSERT_EXPR(false, "字符非法! ->合法字符{数字, 大小写字母}");
			}
		}
		return resultInteger;
	}

	int toRadixIntNum(char alphOrAscllNum, int radix) {
		int result = toIntNum(alphOrAscllNum);
		_ASSERT_EXPR(StandardExtend::inRange(0, result, radix), "不是指定进制的字符串");
		return result;
	}

	char toAbs(int &number) {
		return number < 0 ? (number = -number), '-' : '+';
	}


	std::string bigPlush(std::string &topLowNumA, std::string &topLowNumB, std::string &topLowSum, int radix) {
		_ASSERT_EXPR(StandardExtend::inRange(2, radix, 63), "单个字符无法区分表示指定进制!");
		std::size_t lenA = topLowNumA.length(), lenB = topLowNumB.length(), lenAB;
		//补0用
		std::string temp;
		//低位在右, 短者高位0补齐
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
		//反转后左边是低位
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
		// 反码
		TransitionUtility::inverseCode(topLowOriginBinCode.begin(), topLowOriginBinCode.end());

		// 补码
		static std::string one("1");
		TransitionUtility::bigPlush(topLowOriginBinCode, one, topLowOriginBinCode, 2);
		return topLowOriginBinCode;
	}
	std::string calcComplementCode(DigitArray &topLowOriginCode) {
		static std::string topLowOriginBinCode;
		topLowOriginBinCode.resize(topLowOriginCode.size());
		TransitionUtility::digitContainerToCharContainer(topLowOriginCode.begin(), topLowOriginCode.end(), topLowOriginBinCode.begin());
		return calcComplementCode(topLowOriginBinCode);
	}
	std::string calcComplementCode(int decNum) {
		static DigitArray topLow;
		topLow.resize(MathExtend::calcDigitTotalSize(decNum, 2));
		// 原码
		TransitionUtility::decimalToRadixTopLow(decNum, topLow.begin(), topLow.end(), 2);
		return calcComplementCode(topLow);
	}
}
