#include "../../utility/RandomSpace.h"
#include "../../ExtendSpace.h"

// ��Щ�������ų̶Ȼ��д����׵���֤
namespace RandomSpace {

	// FILE * fp = NULL;

	/******���������******/

	unsigned RandomUnsigned(unsigned UpperBound) {
		static bool isInit = []() {
			// ���þ�ֵ̬�������ĳ�ʼ��
			srand((unsigned)time(0));
			return true;
		}();
		// �е�ֵ�����������, ��˽���ʹ�ö������б׶˵�(�޷��ָ����� �Լ�ֻ��Debug������)
		// ��˱�Ҫ�ĵط��������쳣����, ���������ܻ���ת��ĳ���̶���ò���޹صĵط�����
		if (UpperBound == 0) {
			// throw UpperBound; // Ҳ�����׳��쳣ֵ
			throw std::exception("����Ϊ��!"); // �쳣����Ҳ�����׳�ֵ ��ֻ����int
		}
		/*
		time_t t;
		time(&t);
		t %= UpperBound;
		return ((rand()*t) % UpperBound);
		*/
		// rand [0, RAND_MAX] RAND_MAX�ľ���ֵ�ɱ���������������Ϊ32767
		return rand() % UpperBound;
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
			return Utility::tryToSignedNum(resultUnsigned, result) ? 1 : 0;
		}, 0);
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

	unsigned RandomUnsignedNotRange(unsigned LowerBound, unsigned UpperBound) {
		_ASSERT_EXPR(LowerBound <= UpperBound, "��Χ����������Ҫ��");
		return IsProbability(1, 2) ?
			RandomUnsignedRange(MIN_INT32_UNSIGNED, LowerBound) : RandomUnsignedRange(UpperBound, MAX_INT32_UNSIGNED);
	}

	int RandomRange(int LowerBound, int UpperBound) {
		_ASSERT_EXPR(LowerBound <= UpperBound, "��Χ����������Ҫ��");
		return(LowerBound + Random(UpperBound - LowerBound));
	}

	int RandomNotRange(int LowerBound, int UpperBound) {
		_ASSERT_EXPR(LowerBound <= UpperBound, "��Χ����������Ҫ��");
		return IsProbability(1, 2) ? RandomRange(MIN_INT32, LowerBound) : RandomRange(UpperBound, MAX_INT32);
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
		_ASSERT_EXPR(Numerator <= Denominator, "���ʲ�Ӧ����1");
		// [0, Denominator)
		unsigned randValue = RandomUnsigned(Denominator);
		return randValue < Numerator;
	}

	bool IsProbability(unsigned Numerator, unsigned Denominator, unsigned &EffectiveCount) {
		bool result = false;
		if (EffectiveCount == 0) {
			// DNT
		}
		else {
			if (IsProbability(Numerator, Denominator)) {
				result = true;
				--EffectiveCount;
			}
			else {
				// DNT
			}
		}
		return result;
	}

}
