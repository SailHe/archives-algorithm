#include "../ExtendSpace.h"
// 要想导出Dll 这里必须引入对应的头文件
#include "IOSpace.h"

namespace IOSpace {
	// IO初始化
	FILE *InitIO(char const *fileNameForFp, char const*mode) {
		FILE *fp = NULL;
		errno_t flag = fopen_s(&fp, fileNameForFp, mode);
		if (fp == NULL) {
			throw "IO初始化失败!";
		}
		return fp;
	}
	// IO终止
	void ShutupIO(FILE **fp) {
		fclose(*fp);
		*fp = NULL;
	}

	void JudgeByCompare(char const *lhsFileName, char const *rhsFileName, int Line) {
		//rename(); //http://blog.csdn.net/wangshubo1989/article/details/49559195
		// freopen("Judge.out", "w", stdout);
		FILE *stdouter = stdout;
		freopen_s(&stdouter, "Judge.out", "w", stdout);
		FILE *lhsFp = InitIO(lhsFileName, "r");
		FILE *rhsFp = InitIO(rhsFileName, "r");
		char c1 = 0, c2 = 0;
		for (int i = 1; i <= Line; i++) {
			c1 = c2 = 0;
			while (c1 != 10 && c1 == c2) {
				c1 = fgetc(lhsFp);
				c2 = fgetc(rhsFp);
			}
			printf(c1 == c2 ? "" : "Diffrence in line %d\n", i);
		}
		puts("end");
		ShutupIO(&lhsFp);
		ShutupIO(&rhsFp);
	}
}
