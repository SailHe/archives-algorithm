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

	// α����޷������� [0, UpperBound); PS: UpperBound != 0
	unsigned RandomUnsigned(unsigned UpperBound);
	// α������� (AbsBound, 0] �� [0, AbsBound)
	int Random(int AbsBound);

	// ��Χ�޷���α��� (LowerBound, UpperBound]
	unsigned RandomUnsignedRange(unsigned LowerBound, unsigned UpperBound);
	int RandomRange(int LowerBound, int UpperBound);
	
	// α����޷������� (0, UpperBound]
	unsigned RandomUnsignedP1(unsigned UpperBound);
	int RandomP1(int UpperBound);
	
	// ���������double����С�� [0, UpperBound)
	double RandomUnsignedDouble(unsigned UpperBound);
	double RandomDouble(int UpperBound);
	
	// �������ֵΪMaxNum��������� (������, ������, Ԫ��ֵ���ȡֵ��Χ, Ŀ���ļ�) PS: �л����дһ��C++���������
	void RandomMatrixUnsignedElement(int Rows, int Cols, int LowerBound, int UpperBound, FILE *DestFp);

	// ����ַ� ([B, E), �ַ�������) PS: B <= E, ���ҽ���B == Eʱֻ����B
	char RandomChar(char B, char E);
	// �� Numerator / Denominator �ĸ��ʽ��м�� ���ɹ��򷵻�true
	bool IsProbability(unsigned Numerator, unsigned Denominator);
	
	// �Ա�lhs��rhs��Ӧ�������ļ���ǰLine������, �ԱȽ�������Judge.out(��ָ���ڼ���������)
	void JudgeByCompare(char const *lhsFileName, char const *rhsFileName, int Line);
	// ����֮ǰ���ܵ�IO��ʼ��
	void InitIO();
}

#endif
