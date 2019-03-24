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

	// 对比lhs和rhs对应的两个文件的前Line行内容, 对比结果输出到Judge.out(会指出第几行有问题)
	DSAUTILITYEXTENSION_API void JudgeByCompare(char const *lhsFileName, char const *rhsFileName, int Line);

}
