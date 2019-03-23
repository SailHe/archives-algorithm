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

	// [1,L]的伪随机整数
	int Random(int L);
	// 返回double型正小数(0, L]
	double RandomD(int L);
	// 输出随机小数...
	void PDouble();
	// 随机字符 s是起点字符 e是终点字符
	char RandomChar(char s, char e);
	// 以 x / base 的概率进行检测 若成功则返回true
	bool IsProbability(int x, int base);
	// 生成最大值为MaxNum的随机矩阵 (总行数, 总列数, 元素最大值)
	void RandomMatrix(int r, int c, int MaxNum);
	// 对比lhs和rhs对应的两个文件的前Line行内容, 对比结果输出到Judge.out(会指出第几行有问题)
	void JudgeByCompare(char const *lhsFileName, char const *rhsFileName, int Line);
	// 测试之前可能的IO初始化
	void InitIO();
}

#endif
