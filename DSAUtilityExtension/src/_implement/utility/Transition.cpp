#include "../../utility/Transition.h"

Transition::Transition(int bufferRadix) {
	this->bufferRadix = bufferRadix;
}
Transition::~Transition() {}
int Transition::calcMinDigitNum() {
	return (std::max)(this->originBitLeast, this->targetBitLeast);
}
int Transition::resetBits(int originBitLeast, int targetBitLeast) {
	// 有增有减->最后考虑abs; 多个变量->变量合成:max; 计算变化量: 减法
	int currentMaxBits = calcMinDigitNum();
	this->originBitLeast = originBitLeast;
	this->targetBitLeast = targetBitLeast;
	int nextMaxBits = calcMinDigitNum();
	// 如果总比特位相等&&最大比特位相等 则disBits=0 否则disBits = nextMaxBits - currentMaxBits
	int disBits = nextMaxBits - currentMaxBits;
	return disBits;
}
void Transition::reSizeBuffer(JCE::SizeType nextSize) {
	repositoryBuffer.resize(nextSize);
	// 预留的[进位段]必须初始化为0 否则进位时会出错
	std::for_each(repositoryBuffer.begin(), repositoryBuffer.begin() + targetBitLeast, [](int &bit) {bit = 0; });
	//memset(repositoryBuffer, 0, targetBitLeast*sizeof(int));
}

// =============================

BinaryTransition::BinaryTransition(JCE::SizeType maxBitSize, int originBitLeast, int targetBitLeast) : Transition(2) {
	this->originBitLeast = 0;
	this->targetBitLeast = 0;
	this->originRadix = 0;
	this->targetRadix = 0;
	//repositoryBuffer = (int *)malloc(sizeof(int)*maxBitSize);
	repositoryBuffer.resize(maxBitSize);
	reset(originBitLeast, targetBitLeast);
}

// 重设进制位数: 自动计算缓存[增减量]然后重设最小缓存空间(至少满足最大进制的1位), 同时初始化[进位段]
void BinaryTransition::reset(int originBitLeast, int targetBitLeast) {
	_ASSERT_EXPR(StandardExtend::inRange(1, originBitLeast, 32), "radixBits => [1, 32)");
	_ASSERT_EXPR(StandardExtend::inRange(1, targetBitLeast, 32), "radixBits => [1, 32)");
	reSizeBuffer(repositoryBuffer.size() + resetBits(originBitLeast, targetBitLeast));
	this->originRadix = (int)pow(2.0, originBitLeast);
	this->targetRadix = (int)pow(2.0, targetBitLeast);
}

// =============================

DecimalTransition::DecimalTransition(JCE::SizeType maxDigitSize, int originRadix, int targetRadix) : Transition(10) {
	this->originBitLeast = 0;
	this->targetBitLeast = 0;
	this->originRadix = 0;
	this->targetRadix = 0;
	//repositoryBuffer = (int *)malloc(sizeof(int)*maxDigitSize);
	repositoryBuffer.resize(maxDigitSize);
	reset(originRadix, targetRadix);
}

// 重设转换进制: 自动计算缓存[增减量]然后重设最小缓存空间(至少满足最大进制的1位), 同时初始化必要的元素
void DecimalTransition::reset(int originRadix, int targetRadix) {
	// 最大进制要保证允许表示该进制的最大值的两倍
	_ASSERT_EXPR(StandardExtend::inRange(2, originRadix, (MAX_INT32) / 2), "radix = [2, (MAX_INT32)/2)");
	_ASSERT_EXPR(StandardExtend::inRange(2, targetRadix, (MAX_INT32) / 2), "radix = [2, (MAX_INT32)/2)");
	int originBitLeast = (int)std::ceil(MathExtend::logRadix(originRadix, this->bufferRadix));
	int targetBitLeast = (int)std::ceil(MathExtend::logRadix(targetRadix, this->bufferRadix));
	reSizeBuffer(repositoryBuffer.size() + resetBits(originBitLeast, targetBitLeast));
	this->originRadix = originRadix;
	this->targetRadix = targetRadix;
}
