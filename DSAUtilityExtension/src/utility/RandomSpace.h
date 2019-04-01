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

// 软件测试用命名空间(原ItemTest)
namespace RandomSpace {
	// extern FILE *fp;

	// 循环 直到fun的返回值与给定值不相等 (循环至达成条件就没必要写个模板了, 此处主要是用于极少时候排除某些特定的随机值)
	template<typename LoopFunction>
	void loopUntilNotEq(LoopFunction fun, int result) {
		while (result == fun());
	}

	// 伪随机无符号整数 [0, UpperBound); PS: UpperBound != 0
	DSAUTILITYEXTENSION_API unsigned RandomUnsigned(unsigned UpperBound);
	// 伪随机整数 (AbsBound, 0] 或 [0, AbsBound)
	DSAUTILITYEXTENSION_API int Random(int AbsBound);

	// 范围无符号伪随机 (LowerBound, UpperBound]
	DSAUTILITYEXTENSION_API unsigned RandomUnsignedRange(unsigned LowerBound, unsigned UpperBound);
	// 范围外
	DSAUTILITYEXTENSION_API unsigned RandomUnsignedNotRange(unsigned LowerBound, unsigned UpperBound);
	DSAUTILITYEXTENSION_API int RandomRange(int LowerBound, int UpperBound);
	DSAUTILITYEXTENSION_API int RandomNotRange(int LowerBound, int UpperBound);
	
	// 伪随机无符号整数 (0, UpperBound]
	DSAUTILITYEXTENSION_API unsigned RandomUnsignedP1(unsigned UpperBound);
	DSAUTILITYEXTENSION_API int RandomP1(int UpperBound);
	
	// 返回随机的double型正小数 [0, UpperBound)
	DSAUTILITYEXTENSION_API double RandomUnsignedDouble(unsigned UpperBound);
	DSAUTILITYEXTENSION_API double RandomDouble(int UpperBound);
	
	// 生成最大值为MaxNum的随机矩阵 (总行数, 总列数, 元素值域或取值范围, 目标文件) PS: 有机会加写一个C++流参数版的
	DSAUTILITYEXTENSION_API void RandomMatrixUnsignedElement(int Rows, int Cols, int LowerBound, int UpperBound, FILE *DestFp);

	// 随机字符 ([B, E), 字符集容量) PS: B <= E, 当且仅当B == E时只返回B
	DSAUTILITYEXTENSION_API char RandomChar(char B, char E);
	// 以 Numerator / Denominator 的概率进行检测 若成功则返回true
	DSAUTILITYEXTENSION_API bool IsProbability(unsigned Numerator, unsigned Denominator);
	// 只生效EffectiveCount次
	DSAUTILITYEXTENSION_API bool IsProbability(unsigned Numerator, unsigned Denominator, unsigned &EffectiveCount);
	
}
