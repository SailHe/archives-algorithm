#include "../../ExtendSpace.h"
// 要想导出Dll 这里必须引入对应的头文件
#include "../../utility/IOSpace.h"

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
		if (*fp == NULL) {
			// DNT
		}
		else {
			fclose(*fp);
			*fp = NULL;
		}
	}

	void RedirectStdinFrom(char const *FileName) {
		static FILE *inFile = NULL;
		ShutupIO(&inFile);
		if (FileName == NULL) {
			// 重定向回Win标准输入 Linux使用 "/dev/tty"
			freopen_s(&inFile, "CON", "r", stdin);
		}
		else {
			if (strcmp(FileName, "CON") == 0 || strcmp(FileName, "/dev/tty") == 0) {
				throw "这个名称有点特殊, 别这么用, 不利于移植";
			}
			freopen_s(&inFile, FileName, "r", stdin);
		}
		if (inFile == NULL) {
			throw "文件打开失败!";
		}
	}

	void redirectStdinFrom(std::streambuf *newCinBuf) {
		static std::streambuf *cinbuf = std::cin.rdbuf();
		if (newCinBuf == nullptr) {
			std::cin.rdbuf(cinbuf);
		}
		else {
			std::cin.rdbuf(newCinBuf);
		}
	}

	void RedirectStdoutTo(char const *FileName) {
		// 这玩意没写权限('r') 也没啥反应... 所以就这样封装死挺好的, 还避免了使用此函数而导致的被平台捆绑的可能性
		static FILE *outFile = NULL;// FILE *stdouter = stdout;
		// 自动关闭上一次的文件(如果同个文件既被重定向为stdin和stdout就会出问题 主要表现为打开失败 文件流为NULL)
		ShutupIO(&outFile);
		if (FileName == NULL) {
			// 重定向回Win标准输出 Linux使用 "/dev/tty"
			freopen_s(&outFile, "CON", "w", stdout);
		}
		else {
			if (strcmp(FileName, "CON") == 0 || strcmp(FileName, "/dev/tty") == 0) {
				throw "这个名称有点特殊, 别这么用, 不利于移植";
			}
			freopen_s(&outFile, FileName, "w", stdout);
		}
		if (outFile == NULL) {
			throw "文件打开失败!";
		}
	}

	void redirectStdoutTo(std::streambuf *newCoutBuf) {
		static std::streambuf *coutbuf = std::cout.rdbuf();
		if (newCoutBuf == nullptr) {
			std::cout.rdbuf(coutbuf);
		}
		else {
			std::cout.rdbuf(newCoutBuf);
		}
	}

	void JudgeByCompare(char const *lhsFileName, char const *rhsFileName, int Line) {
		//rename(); //http://blog.csdn.net/wangshubo1989/article/details/49559195
		RedirectStdoutTo("Judge.out");
		FILE *lhsFp = InitIO(lhsFileName, "r");
		FILE *rhsFp = InitIO(rhsFileName, "r");
		char c1 = 0, c2 = 0;
		bool allTheSame = true;
		for (int i = 1; i <= Line; i++) {
			c1 = c2 = 0;
			while (c1 != 10 && c1 == c2) {
				c1 = fgetc(lhsFp);
				c2 = fgetc(rhsFp);
			}
			printf(c1 == c2 ? "" : "Diffrence in line %d\n", i);
			if (allTheSame && c1 != c2) {
				allTheSame = false;
			}
		}
		RedirectStdoutTo();
		ShutupIO(&lhsFp);
		ShutupIO(&rhsFp);
		puts(allTheSame ? "end" : "存在不一致行 详情参考Judge.out文件");
	}
}
