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

	// 以常用的读模式重定向标准输入[为]指定文件(默认表示重设回标准输入)
	DSAUTILITYEXTENSION_API void RedirectStdinFrom(char const *fileName = NULL);
	// C++版本(默认表示重设回标准输入)
	DSAUTILITYEXTENSION_API void redirectStdinFrom(std::streambuf *newCinBuf = nullptr);
	// 以常用的写模式重定向标准输出[到]指定文件(默认表示重设回标准输出)
	DSAUTILITYEXTENSION_API void RedirectStdoutTo(char const *fileName = NULL);
	// C++版本(默认表示重设回标准输出)
	DSAUTILITYEXTENSION_API void redirectStdoutTo(std::streambuf *newCoutBuf = nullptr);

}
