#include "../../utility/RandomSpace.h"
#include "../../ExtendSpace.h"

// 这些方法可信程度还有待彻底的验证
namespace RandomSpace {

	// FILE * fp = NULL;

	/******随机基础层******/

	unsigned RandomUnsigned(unsigned UpperBound) {
		static bool isInit = []() {
			// 利用静态值作方法的初始化
			srand((unsigned)time(0));
			return true;
		}();
		// 有的值是随机产生的, 因此仅仅使用断言是有弊端的(无法恢复过来 以及只在Debug中运行)
		// 因此必要的地方必须有异常处理, 否则代码可能会跳转至某个固定且貌似无关的地方报错
		if (UpperBound == 0) {
			// throw UpperBound; // 也可以抛出异常值
			throw std::exception("除数为零!"); // 异常类型也可以抛出值 但只能是int
		}
		/*
		time_t t;
		time(&t);
		t %= UpperBound;
		return ((rand()*t) % UpperBound);
		*/
		// rand [0, RAND_MAX] RAND_MAX的具体值由编译器决定，至少为32767
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
		_ASSERT_EXPR(LowerBound <= UpperBound, "范围参数不符合要求");
		// 上下限(数值): Upper limit, Lower limit
		// 上下界(抽象EG境界): Upper bound, Lower bound
		// 关于左和右有争议 https://www.zhihu.com/question/21859177
		// 姑且认为左小右大, 然后所谓人往高出走, 于是左->右, 草率规定左为起源(Origin)右为目标(Target)或目的地/址(Destnation)
		// 为统一原有代码或区分C++代码, 使用纯C语言编写的代码全以大驼峰命名(尽量避免混用, 若不能避免则视为C++)
		return(LowerBound + RandomUnsigned(UpperBound - LowerBound));
	}

	unsigned RandomUnsignedNotRange(unsigned LowerBound, unsigned UpperBound) {
		_ASSERT_EXPR(LowerBound <= UpperBound, "范围参数不符合要求");
		return IsProbability(1, 2) ?
			RandomUnsignedRange(MIN_INT32_UNSIGNED, LowerBound) : RandomUnsignedRange(UpperBound, MAX_INT32_UNSIGNED);
	}

	int RandomRange(int LowerBound, int UpperBound) {
		_ASSERT_EXPR(LowerBound <= UpperBound, "范围参数不符合要求");
		return(LowerBound + Random(UpperBound - LowerBound));
	}

	int RandomNotRange(int LowerBound, int UpperBound) {
		_ASSERT_EXPR(LowerBound <= UpperBound, "范围参数不符合要求");
		return IsProbability(1, 2) ? RandomRange(MIN_INT32, LowerBound) : RandomRange(UpperBound, MAX_INT32);
	}

	unsigned RandomUnsignedP1(unsigned UpperBound) {
		return RandomUnsigned(UpperBound) + 1;
	}

	int RandomP1(int UpperBound){
		return Random(UpperBound) + 1;
	}

	double RandomUnsignedDouble(unsigned UpperBound) {
		/// printf("%d.%d\n", rand(), rand()); // 输出随机小数的思路...
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

	/******应用方法层******/

	char RandomChar(char B, char E) {
		return RandomUnsignedRange(B, E);
	}

	bool IsProbability(unsigned Numerator, unsigned Denominator) {
		// 小数概率只包含有理数, 分数概率范围更广, 虽然计算机本来就只能表示极小的一部分数
		_ASSERT_EXPR(Numerator <= Denominator, "概率不应大于1");
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
