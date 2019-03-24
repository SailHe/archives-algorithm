#include "./SoftwareTest.h"

// ���з������кܶ�BUG, ���Ƹ����������, �Ƿ������...
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
		// rand [0, RAND_MAX] RAND_MAX�ľ���ֵ�ɱ���������������Ϊ32767
		return ((rand()*t) % L);
	}

	int RandomRange(int LowerBound, int UpperBound) {
		// ������(��ֵ): Upper limit, Lower limit
		// ���½�(����EG����): Upper bound, Lower bound
		// ��������������� https://www.zhihu.com/question/21859177
		// ������Ϊ��С�Ҵ�, Ȼ����ν�����߳���, ������->��, ���ʹ涨��Ϊ��Դ(Origin)��ΪĿ��(Target)��Ŀ�ĵ�/ַ(Destnation)
		// Ϊͳһԭ�д��������C++����, ʹ�ô�C���Ա�д�Ĵ���ȫ�Դ��շ�����(�����������, �����ܱ�������ΪC++)
		return(LowerBound + Random(UpperBound - LowerBound));
	}

	int Random1(int L) {
		return Random(L) + 1;
	}

	double RandomDouble(int L) {
		/// printf("%d.%d\n", rand(), rand()); // ������С����˼·...
		return (double)Random(L) / ((double)Random1(9));
	}

	char RandomChar(char B, char E) {
		return RandomRange(B, E);
	}

	void RandomMatrix(int Rows, int Cols, int LowerBound, int UpperBound, FILE *DestFp) {
		//fprintf(DestFp, "%d \n", Rows);
		fprintf(DestFp, "%d\n", Cols);
		while (Rows-- > 0){
			for (int c = 0; c < Cols; c++) {
				fprintf(DestFp, c == 0 ? "" : " ");
				fprintf(DestFp, "%d", RandomRange(LowerBound, UpperBound));
			}
		}
		fprintf(DestFp, "\n");
	}

	// @TODO: unsigned (С������ʵ�ֵ����岻��, ���������ܱ�ʾ���õĸ�����)
	bool IsProbability(int Numerator, int Denominator) {
		_ASSERT_EXPR(Numerator < Denominator, "���� < ��ĸ ����û��Ҫʹ�ø���");
		// [0, Denominator)
		int randValue = Random(Denominator);
		return randValue < Numerator;
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
