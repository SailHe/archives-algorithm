#include "../../utility/Transition.h"

Transition::Transition(int bufferRadix) {
	this->bufferRadix = bufferRadix;
}
Transition::~Transition() {}
int Transition::calcMinDigitNum() {
	return (std::max)(this->originBitLeast, this->targetBitLeast);
}
int Transition::resetBits(int originBitLeast, int targetBitLeast) {
	// �����м�->�����abs; �������->�����ϳ�:max; ����仯��: ����
	int currentMaxBits = calcMinDigitNum();
	this->originBitLeast = originBitLeast;
	this->targetBitLeast = targetBitLeast;
	int nextMaxBits = calcMinDigitNum();
	// ����ܱ���λ���&&������λ��� ��disBits=0 ����disBits = nextMaxBits - currentMaxBits
	int disBits = nextMaxBits - currentMaxBits;
	return disBits;
}
void Transition::reSizeBuffer(JCE::SizeType nextSize) {
	repositoryBuffer.resize(nextSize);
	// Ԥ����[��λ��]�����ʼ��Ϊ0 �����λʱ�����
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

// �������λ��: �Զ����㻺��[������]Ȼ��������С����ռ�(�������������Ƶ�1λ), ͬʱ��ʼ��[��λ��]
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

// ����ת������: �Զ����㻺��[������]Ȼ��������С����ռ�(�������������Ƶ�1λ), ͬʱ��ʼ����Ҫ��Ԫ��
void DecimalTransition::reset(int originRadix, int targetRadix) {
	// ������Ҫ��֤�����ʾ�ý��Ƶ����ֵ������
	_ASSERT_EXPR(StandardExtend::inRange(2, originRadix, (MAX_INT32) / 2), "radix = [2, (MAX_INT32)/2)");
	_ASSERT_EXPR(StandardExtend::inRange(2, targetRadix, (MAX_INT32) / 2), "radix = [2, (MAX_INT32)/2)");
	int originBitLeast = (int)std::ceil(MathExtend::logRadix(originRadix, this->bufferRadix));
	int targetBitLeast = (int)std::ceil(MathExtend::logRadix(targetRadix, this->bufferRadix));
	reSizeBuffer(repositoryBuffer.size() + resetBits(originBitLeast, targetBitLeast));
	this->originRadix = originRadix;
	this->targetRadix = targetRadix;
}
