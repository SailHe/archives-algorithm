#include "TransitionUtility.h"
#include "../ExtendSpace.h"

namespace TransitionUtility{
	char toUppercaseAscllChar(int num) {
		return num > 9 ? num - 10 + 'A' : num + '0';
	}

	int toIntNum(char alphOrCharNum) {
		int resultInteger = -1;
		// isalnum();
		if ('0' <= alphOrCharNum && alphOrCharNum <= '9') {
			// 0 + alphOrCharNum - '0';
			resultInteger = alphOrCharNum - '0';
		}
		else {
			// isalpha();
			if ('A' <= alphOrCharNum && alphOrCharNum <= 'Z') {
				resultInteger = 10 + alphOrCharNum - 'A';
			}
			else if ('a' <= alphOrCharNum && alphOrCharNum <= 'z') {
				// 数字字符10个{0, 9} 大写字母26个{A, Z} 小写字母26个{a, z}
				resultInteger = 36 + alphOrCharNum - 'a';
			}
			else {
				_ASSERT_EXPR(false, "字符非法! ->合法字符{数字, 大小写字母}");
			}
		}
		return resultInteger;
	}

	int toRadixIntNum(char alphOrCharNum, int radix) {
		int result = toIntNum(alphOrCharNum);
		_ASSERT_EXPR(StandardExtend::inRange(0, result, radix), "不是指定进制的字符串");
		return result;
	}

	char toAbs(int &number) {
		return number < 0 ? (number = -number), '-' : '+';
	}

	void complement(DigitIterator originP, DigitIterator currentP, int n) {
		int residueBit = (currentP - originP) % n;
		if (residueBit != 0) {
			residueBit = n - residueBit;
			while (residueBit-- > 0) {
				currentP[residueBit] = 0;
			}
		}
	}
}