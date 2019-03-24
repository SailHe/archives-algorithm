#include "./SoftwareTest.h"
#include "ExtendSpace.h"

// ��Щ�������ų̶Ȼ��д����׵���֤
namespace SoftwareTest {

	FILE * fp = NULL;

	/******���������******/

	unsigned RandomUnsigned(unsigned UpperBound) {
		// �е�ֵ�����������, ��˽���ʹ�ö������б׶˵�(�޷��ָ����� �Լ�ֻ��Debug������)
		// ��˱�Ҫ�ĵط��������쳣����, ���������ܻ���ת��ĳ���̶���ò���޹صĵط�����
		if (UpperBound == 0) {
			throw UpperBound;
		}
		time_t t;
		time(&t);
		t %= UpperBound;
		// rand [0, RAND_MAX] RAND_MAX�ľ���ֵ�ɱ���������������Ϊ32767
		return ((rand()*t) % UpperBound);
	}

	int Random(int AbsBound) {
		bool IsNegative = false;
		if (AbsBound < 0) {
			AbsBound = -AbsBound;
			IsNegative = true;
		}
		unsigned resultUnsigned = 0;
		int result = 0;
		loopUntilNotEq([&]() {
			resultUnsigned = RandomUnsigned(AbsBound);
			return Utility::tryToSignedNum(resultUnsigned, result);
		}, false);
		return IsNegative ? -result : result;
	}

	unsigned RandomUnsignedRange(unsigned LowerBound, unsigned UpperBound) {
		_ASSERT_EXPR(LowerBound <= UpperBound, "��Χ����������Ҫ��");
		// ������(��ֵ): Upper limit, Lower limit
		// ���½�(����EG����): Upper bound, Lower bound
		// ��������������� https://www.zhihu.com/question/21859177
		// ������Ϊ��С�Ҵ�, Ȼ����ν�����߳���, ������->��, ���ʹ涨��Ϊ��Դ(Origin)��ΪĿ��(Target)��Ŀ�ĵ�/ַ(Destnation)
		// Ϊͳһԭ�д��������C++����, ʹ�ô�C���Ա�д�Ĵ���ȫ�Դ��շ�����(�����������, �����ܱ�������ΪC++)
		return(LowerBound + RandomUnsigned(UpperBound - LowerBound));
	}

	int RandomRange(int LowerBound, int UpperBound) {
		_ASSERT_EXPR(LowerBound <= UpperBound, "��Χ����������Ҫ��");
		return(LowerBound + Random(UpperBound - LowerBound));
	}

	unsigned RandomUnsignedP1(unsigned UpperBound) {
		return RandomUnsigned(UpperBound) + 1;
	}

	int RandomP1(int UpperBound){
		return Random(UpperBound) + 1;
	}

	double RandomUnsignedDouble(unsigned UpperBound) {
		/// printf("%d.%d\n", rand(), rand()); // ������С����˼·...
		return (double)RandomUnsigned(UpperBound) / ((double)RandomUnsigned(UpperBound) + 1);
	}

	double RandomDouble(int UpperBound) {
		double result = RandomUnsignedDouble(UpperBound);
		return UpperBound > 0 ? result : -result;
	}

	void RandomMatrixUnsignedElement(int Rows, int Cols, int LowerBound, int UpperBound, FILE *DestFp) {
		//fprintf(DestFp, "%d \n", Rows);
		fprintf(DestFp, "%d\n", Cols);
		while (Rows-- > 0){
			for (int c = 0; c < Cols; c++) {
				fprintf(DestFp, c == 0 ? "" : " ");
				fprintf(DestFp, "%d", RandomUnsignedRange(LowerBound, UpperBound));
			}
		}
		fprintf(DestFp, "\n");
	}

	/******Ӧ�÷�����******/

	char RandomChar(char B, char E) {
		return RandomUnsignedRange(B, E);
	}

	bool IsProbability(unsigned Numerator, unsigned Denominator) {
		// С������ֻ����������, �������ʷ�Χ����, ��Ȼ�����������ֻ�ܱ�ʾ��С��һ������
		_ASSERT_EXPR(Numerator < Denominator, "��ʱû��Ҫʹ�ø���");
		// [0, Denominator)
		int randValue = RandomUnsigned(Denominator);
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
