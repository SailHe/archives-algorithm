#pragma once
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <time.h>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>

#include "../DSAUEdll.h"

// ��������������ռ�(ԭItemTest)
namespace RandomSpace {
	// extern FILE *fp;

	// ѭ�� ֱ��fun�ķ���ֵ�����ֵ����� (ѭ�������������û��Ҫд��ģ����, �˴���Ҫ�����ڼ���ʱ���ų�ĳЩ�ض������ֵ)
	template<typename LoopFunction>
	void loopUntilNotEq(LoopFunction fun, int result) {
		while (result == fun());
	}

	// α����޷������� [0, UpperBound); PS: UpperBound != 0
	DSAUTILITYEXTENSION_API unsigned RandomUnsigned(unsigned UpperBound);
	// α������� (AbsBound, 0] �� [0, AbsBound)
	DSAUTILITYEXTENSION_API int Random(int AbsBound);

	// ��Χ�޷���α��� (LowerBound, UpperBound]
	DSAUTILITYEXTENSION_API unsigned RandomUnsignedRange(unsigned LowerBound, unsigned UpperBound);
	// ��Χ��
	DSAUTILITYEXTENSION_API unsigned RandomUnsignedNotRange(unsigned LowerBound, unsigned UpperBound);
	DSAUTILITYEXTENSION_API int RandomRange(int LowerBound, int UpperBound);
	DSAUTILITYEXTENSION_API int RandomNotRange(int LowerBound, int UpperBound);
	
	// α����޷������� (0, UpperBound]
	DSAUTILITYEXTENSION_API unsigned RandomUnsignedP1(unsigned UpperBound);
	DSAUTILITYEXTENSION_API int RandomP1(int UpperBound);
	
	// ���������double����С�� [0, UpperBound)
	DSAUTILITYEXTENSION_API double RandomUnsignedDouble(unsigned UpperBound);
	DSAUTILITYEXTENSION_API double RandomDouble(int UpperBound);
	
	// �������ֵΪMaxNum��������� (������, ������, Ԫ��ֵ���ȡֵ��Χ, Ŀ���ļ�) PS: �л����дһ��C++���������
	DSAUTILITYEXTENSION_API void RandomMatrixUnsignedElement(int Rows, int Cols, int LowerBound, int UpperBound, FILE *DestFp);

	// ����ַ� ([B, E), �ַ�������) PS: B <= E, ���ҽ���B == Eʱֻ����B
	DSAUTILITYEXTENSION_API char RandomChar(char B, char E);
	// �� Numerator / Denominator �ĸ��ʽ��м�� ���ɹ��򷵻�true
	DSAUTILITYEXTENSION_API bool IsProbability(unsigned Numerator, unsigned Denominator);
	// ֻ��ЧEffectiveCount��
	DSAUTILITYEXTENSION_API bool IsProbability(unsigned Numerator, unsigned Denominator, unsigned &EffectiveCount);
	
}
