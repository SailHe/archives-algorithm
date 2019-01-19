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
				// �����ַ�10��{0, 9} ��д��ĸ26��{A, Z} Сд��ĸ26��{a, z}
				resultInteger = 36 + alphOrCharNum - 'a';
			}
			else {
				_ASSERT_EXPR(false, "�ַ��Ƿ�! ->�Ϸ��ַ�{����, ��Сд��ĸ}");
			}
		}
		return resultInteger;
	}

	int toRadixIntNum(char alphOrCharNum, int radix) {
		int result = toIntNum(alphOrCharNum);
		_ASSERT_EXPR(StandardExtend::inRange(0, result, radix), "����ָ�����Ƶ��ַ���");
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