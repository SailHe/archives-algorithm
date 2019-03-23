#include "./SoftwareTest.h"

namespace SoftwareTest {

	FILE * fp = NULL;

	/******���ϵ��******/

	int Random(int L) {
		// �е�ֵ�����������, ��˽���ʹ�ö������б׶˵�(�޷��ָ����� �Լ�ֻ��Debug������)
		// ��˱�Ҫ�ĵط��������쳣����, ���������ܻ���ת��ĳ���̶���ò���޹صĵط�����
		if (L == 0) {
			throw L;
		}
		time_t t;
		time(&t);
		t %= L;
		return ((rand()*t) % L) + 1;
	}

	double RandomD(int L) {
		return (double)Random(L) / ((double)Random(9) + 1);
	}

	void PDouble() {
		printf("%d.%d\n", rand(), rand());
	}

	char RandomChar(char s, char e) {
		return(rand() % (e - s + 1) + s);
	}

	bool IsProbability(int x, int base) {
		int range = Random(base);
		return range <= x;
	}

	void RandomMatrix(int r, int c, int MaxNum) {
		//fprintf(fp, "%d \n", r);
		fprintf(fp, "%d\n", c);
		while (r-- > 0)
		{
			for (int i = 0; i < c; i++)
			{
				fprintf(fp, i ? " " : "");
				fprintf(fp, "%d", Random(MaxNum) - 1);
			}
		}
		fprintf(fp, "\n");
	}

	/*******ELSE*******/

	void JudgeByCompare(char const *lhsFileName, char const *rhsFileName, int Line) {
		//rename(); //http://blog.csdn.net/wangshubo1989/article/details/49559195
		freopen("Judge.out", "w", stdout);
		FILE *fp = fopen(lhsFileName, "r");
		FILE *fpi = fopen(rhsFileName, "r");
		char c1 = 0, c2 = 0;
		for (int i = 1; i <= Line; i++){
			c1 = c2 = 0;
			while (c1 != 10 && c1 == c2) {
				c1 = fgetc(fp);
				c2 = fgetc(fpi);
			}
			printf(c1 == c2 ? "" : "Diffrence in line %d\n", i);
		}
		puts("end");
	}

	void InitIO() {
		/**/
		const char *InName = "test.in";
		const char *OutName = "test.out";
		/**
		char *InName = "sample.in";
		char *OutName = "sample.out";
		/**/
		fp = fopen(InName, "w");//InName.in��д��
		freopen("sample.in", "r", stdin);
		//freopen("test.out", "w", stdout);
		freopen(OutName, "w", stdout);//����������������OutName
	}

}
