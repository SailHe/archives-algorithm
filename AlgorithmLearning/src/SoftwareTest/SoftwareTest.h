#ifndef __SOFTWARE_TEST_H
#define __SOFTWARE_TEST_H
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

// ��������������ռ�(ԭItemTest)
namespace SoftwareTest {
	extern FILE *fp;

	class SampleProgram {
	public:
		// ִ������ (�ȼ��ڽ�������������)
		virtual int run() = 0;
	};

	// ѭ�� ֱ��fun�ķ���ֵ�����ֵ����� (ѭ�������������û��Ҫд��ģ����, �˴���Ҫ�����ڼ���ʱ���ų�ĳЩ�ض������ֵ)
	template<typename LoopFunction>
	void loopUntilNotEq(LoopFunction fun, int result) {
		while (result == fun());
	}

	// α������� [0, L]
	int Random(int L);
	// ��Χ��� (LowerBound, UpperBound]
	int RandomRange(int LowerBound, int UpperBound);
	// α������� (0, L)
	int Random1(int L);
	// ���������double����С�� [0, L)
	double RandomDouble(int L);
	// ����ַ� ([B, E), �ַ�������) PS: B <= E, ���ҽ���B == Eʱֻ����B
	char RandomChar(char B, char E);

	// �������ֵΪMaxNum��������� (������, ������, Ԫ��ֵ���ȡֵ��Χ, Ŀ���ļ�) PS: �л����дһ��C++���������
	void RandomMatrix(int Rows, int Cols, int LowerBound, int UpperBound, FILE *DestFp);

	// �� Numerator / Denominator �ĸ��ʽ��м�� ���ɹ��򷵻�true
	bool IsProbability(int Numerator, int Denominator);
	
	// �Ա�lhs��rhs��Ӧ�������ļ���ǰLine������, �ԱȽ�������Judge.out(��ָ���ڼ���������)
	void JudgeByCompare(char const *lhsFileName, char const *rhsFileName, int Line);
	// ����֮ǰ���ܵ�IO��ʼ��
	void InitIO();
}

#endif
