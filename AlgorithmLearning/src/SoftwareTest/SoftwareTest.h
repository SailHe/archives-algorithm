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

// 软件测试用命名空间(原ItemTest)
namespace SoftwareTest {
	extern FILE *fp;

	class SampleProgram {
	public:
		// 执行例程 (等价于解题程序的主函数)
		virtual int run() = 0;
	};

	// 循环 直到fun的返回值与给定值不相等 (循环至达成条件就没必要写个模板了, 此处主要是用于极少时候排除某些特定的随机值)
	template<typename LoopFunction>
	void loopUntilNotEq(LoopFunction fun, int result) {
		while (result == fun());
	}

	// 伪随机无符号整数 [0, UpperBound); PS: UpperBound != 0
	unsigned RandomUnsigned(unsigned UpperBound);
	// 伪随机整数 (AbsBound, 0] 或 [0, AbsBound)
	int Random(int AbsBound);

	// 范围无符号伪随机 (LowerBound, UpperBound]
	unsigned RandomUnsignedRange(unsigned LowerBound, unsigned UpperBound);
	int RandomRange(int LowerBound, int UpperBound);
	
	// 伪随机无符号整数 (0, UpperBound]
	unsigned RandomUnsignedP1(unsigned UpperBound);
	int RandomP1(int UpperBound);
	
	// 返回随机的double型正小数 [0, UpperBound)
	double RandomUnsignedDouble(unsigned UpperBound);
	double RandomDouble(int UpperBound);
	
	// 生成最大值为MaxNum的随机矩阵 (总行数, 总列数, 元素值域或取值范围, 目标文件) PS: 有机会加写一个C++流参数版的
	void RandomMatrixUnsignedElement(int Rows, int Cols, int LowerBound, int UpperBound, FILE *DestFp);

	// 随机字符 ([B, E), 字符集容量) PS: B <= E, 当且仅当B == E时只返回B
	char RandomChar(char B, char E);
	// 以 Numerator / Denominator 的概率进行检测 若成功则返回true
	bool IsProbability(unsigned Numerator, unsigned Denominator);
	
	// 对比lhs和rhs对应的两个文件的前Line行内容, 对比结果输出到Judge.out(会指出第几行有问题)
	void JudgeByCompare(char const *lhsFileName, char const *rhsFileName, int Line);
	// 测试之前可能的IO初始化
	void InitIO();
}

#endif
