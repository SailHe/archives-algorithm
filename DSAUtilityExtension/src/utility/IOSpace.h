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

namespace IOSpace {

	// �Ա�lhs��rhs��Ӧ�������ļ���ǰLine������, �ԱȽ�������Judge.out(��ָ���ڼ���������)
	DSAUTILITYEXTENSION_API void JudgeByCompare(char const *lhsFileName, char const *rhsFileName, int Line);

	// �Գ��õĶ�ģʽ�ض����׼����[Ϊ]ָ���ļ�(Ĭ�ϱ�ʾ����ر�׼����)
	DSAUTILITYEXTENSION_API void RedirectStdinFrom(char const *fileName = NULL);
	// C++�汾(Ĭ�ϱ�ʾ����ر�׼����)
	DSAUTILITYEXTENSION_API void redirectStdinFrom(std::streambuf *newCinBuf = nullptr);
	// �Գ��õ�дģʽ�ض����׼���[��]ָ���ļ�(Ĭ�ϱ�ʾ����ر�׼���)
	DSAUTILITYEXTENSION_API void RedirectStdoutTo(char const *fileName = NULL);
	// C++�汾(Ĭ�ϱ�ʾ����ر�׼���)
	DSAUTILITYEXTENSION_API void redirectStdoutTo(std::streambuf *newCoutBuf = nullptr);

}
