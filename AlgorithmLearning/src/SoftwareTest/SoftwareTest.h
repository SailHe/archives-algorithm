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

	// [1,L]��α�������
	int Random(int L);
	// ����double����С��(0, L]
	double RandomD(int L);
	// ������С��...
	void PDouble();
	// ����ַ� s������ַ� e���յ��ַ�
	char RandomChar(char s, char e);
	// �� x / base �ĸ��ʽ��м�� ���ɹ��򷵻�true
	bool IsProbability(int x, int base);
	// �������ֵΪMaxNum��������� (������, ������, Ԫ�����ֵ)
	void RandomMatrix(int r, int c, int MaxNum);
	// �Ա�lhs��rhs��Ӧ�������ļ���ǰLine������, �ԱȽ�������Judge.out(��ָ���ڼ���������)
	void JudgeByCompare(char const *lhsFileName, char const *rhsFileName, int Line);
	// ����֮ǰ���ܵ�IO��ʼ��
	void InitIO();
}

#endif
