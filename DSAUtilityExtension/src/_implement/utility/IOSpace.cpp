#include "../../ExtendSpace.h"
// Ҫ�뵼��Dll ������������Ӧ��ͷ�ļ�
#include "../../utility/IOSpace.h"

namespace IOSpace {
	// IO��ʼ��
	FILE *InitIO(char const *fileNameForFp, char const*mode) {
		FILE *fp = NULL;
		errno_t flag = fopen_s(&fp, fileNameForFp, mode);
		if (fp == NULL) {
			throw "IO��ʼ��ʧ��!";
		}
		return fp;
	}
	// IO��ֹ
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
			// �ض����Win��׼���� Linuxʹ�� "/dev/tty"
			freopen_s(&inFile, "CON", "r", stdin);
		}
		else {
			if (strcmp(FileName, "CON") == 0 || strcmp(FileName, "/dev/tty") == 0) {
				throw "��������е�����, ����ô��, ��������ֲ";
			}
			freopen_s(&inFile, FileName, "r", stdin);
		}
		if (inFile == NULL) {
			throw "�ļ���ʧ��!";
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
		// ������ûдȨ��('r') Ҳûɶ��Ӧ... ���Ծ�������װ��ͦ�õ�, ��������ʹ�ô˺��������µı�ƽ̨����Ŀ�����
		static FILE *outFile = NULL;// FILE *stdouter = stdout;
		// �Զ��ر���һ�ε��ļ�(���ͬ���ļ��ȱ��ض���Ϊstdin��stdout�ͻ������ ��Ҫ����Ϊ��ʧ�� �ļ���ΪNULL)
		ShutupIO(&outFile);
		if (FileName == NULL) {
			// �ض����Win��׼��� Linuxʹ�� "/dev/tty"
			freopen_s(&outFile, "CON", "w", stdout);
		}
		else {
			if (strcmp(FileName, "CON") == 0 || strcmp(FileName, "/dev/tty") == 0) {
				throw "��������е�����, ����ô��, ��������ֲ";
			}
			freopen_s(&outFile, FileName, "w", stdout);
		}
		if (outFile == NULL) {
			throw "�ļ���ʧ��!";
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
		puts(allTheSame ? "end" : "���ڲ�һ���� ����ο�Judge.out�ļ�");
	}
}
