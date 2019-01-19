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
		int result = toIntNum(alphOrAscllNum);
		_ASSERT_EXPR(StandardExtend::inRange(0, result, radix), "����ָ�����Ƶ��ַ���");
		return result;
	}

	char toAbs(int &number) {
		return number < 0 ? (number = -number), '-' : '+';
	}
}