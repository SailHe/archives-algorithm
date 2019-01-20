#pragma once
#include "../pch.h"
#include "TransitionUtility.h"
#include "../ExtendSpace.h"

class Transition {
public:
	using DigitArray = TransitionUtility::DigitArray;
	using DigitIterator = DigitArray::iterator;
	Transition(int bufferRadix) {
		this->bufferRadix = bufferRadix;
	}
	virtual ~Transition() {}
protected:
	// ���ر�ת������Ӧ�����ֽ��ƻ���ת��ʱ����С����Ԥ��λ(���������Ƶ�λ���ֵ��Ӧ�Ļ�����Ƶ�λ��)
	int calcMinDigitNum() {
		return (std::max)(this->originBitLeast, this->targetBitLeast);
	}
	// ���軺�����ռ������λ, ������СԤ��λ���ı仯��
	int resetBits(int originBitLeast, int targetBitLeast) {
		// �����м�->�����abs; �������->�����ϳ�:max; ����仯��: ����
		int currentMaxBits = calcMinDigitNum();
		this->originBitLeast = originBitLeast;
		this->targetBitLeast = targetBitLeast;
		int nextMaxBits = calcMinDigitNum();
		// ����ܱ���λ���&&������λ��� ��disBits=0 ����disBits = nextMaxBits - currentMaxBits
		int disBits = nextMaxBits - currentMaxBits;
		return disBits;
	}

	void reSizeBuffer(JCE::SizeType nextSize) {
		repositoryBuffer.resize(nextSize);
		// Ԥ����[��λ��]�����ʼ��Ϊ0 �����λʱ�����
		std::for_each(repositoryBuffer.begin(), repositoryBuffer.begin() + targetBitLeast, [](int &bit) {bit = 0; });
		//memset(repositoryBuffer, 0, targetBitLeast*sizeof(int));
	}

	// ת��˫���Ĵ���λ: һλ��������(���Ǹպõ���˼)��Ҫ���ٻ����������λ��ʾ
	int originBitLeast;
	int originRadix;

	int bufferRadix;
	DigitArray repositoryBuffer;// = NULL

	int targetBitLeast;
	int targetRadix;
};

/*
������ת������
֧�ֿ���2���Ʊ�ʾ������[��2����]�Ĵ�����˫��ת��;
�������ָ��int��Χ�ڵ�[��2����](��һ��digitAscllԪ�ر�����int��Χ��) -> bits => [1, 32) ��: 2 4..2^31��31��[��2����];
����2����λ��������
������Ҫ�û��Լ�����

PS:
1. ��2����: ��2Ϊ�����Ľ���
2. 2��������: ʹ�û�2���Ʊ�ʾ����ֵ
3. ��λ��: Դ��Ŀ��[��2����]�ı���λ����������һ������ֵmaxBits; �� <=> [0, maxBits);
4. ����: 39h->71o
   �Ƚ���ÿһλתΪ2����: 3->0011b, 9->1001b
   ��0011 1001b Ȼ��תΪĿ����� 111 001b->71o
5. ������ļ��ֽ��Ƶĵ�����ֵ�����ֵ
   tBit:   1  2  3     4        5
   input:  1  3  7   F(15)    V(31)
   output: 1 11 111  1111     11111
*/
class BinaryTransition : public Transition {
public:
	// �����б�: {��������ж����ƻ����������λ��(��ʵ����������������Զ�����), Դ���Ʊ���λ��, Ŀ����Ʊ���λ��}
	BinaryTransition(JCE::SizeType maxBitSize, int originBitLeast = 1, int targetBitLeast = 1) : Transition(2) {
		this->originBitLeast = 0;
		this->targetBitLeast = 0;
		this->originRadix = 0;
		this->targetRadix = 0;
		//repositoryBuffer = (int *)malloc(sizeof(int)*maxBitSize);
		repositoryBuffer.resize(maxBitSize);
		reset(originBitLeast, targetBitLeast);
	}

	// �������λ��: �Զ����㻺��[������]Ȼ��������С����ռ�(�������������Ƶ�1λ), ͬʱ��ʼ��[��λ��]
	void reset(int originBitLeast, int targetBitLeast) {
		_ASSERT_EXPR(StandardExtend::inRange(1, originBitLeast, 32), "radixBits => [1, 32)");
		_ASSERT_EXPR(StandardExtend::inRange(1, targetBitLeast, 32), "radixBits => [1, 32)");
		reSizeBuffer(repositoryBuffer.size() + resetBits(originBitLeast, targetBitLeast));
		this->originRadix = (int)pow(2.0, originBitLeast);
		this->targetRadix = (int)pow(2.0, targetBitLeast);
	}

	// 2���״���ת��: Դ��2����->������->Ŀ���2����; targetTopLow�������յĽ��(������ֶ����0)
	void transition(DigitIterator originLeftTop, DigitIterator originRightLow, DigitArray &targetTopLow) {
		// Դ��ֵ��Ŀ����������Ҫ�ö���λ��ʾ
		JCE::SizeType originDigitNum = originRightLow - originLeftTop;
		// ................һ��.............
		JCE::SizeType originToTargetDigitMaxNum =
			originDigitNum*MathExtend::calcDigitTotalSize(originRadix - 1, targetRadix);
		int minDigitNum = calcMinDigitNum();
		if (originToTargetDigitMaxNum + minDigitNum > repositoryBuffer.size()) {
			// ����ռ䲻��->������ʵĻ����С
			reSizeBuffer(originToTargetDigitMaxNum + minDigitNum);
		}
		// top��Ԥ��һ��[��λ��]���ڽ�λ����
		DigitIterator binNumberPointer = repositoryBuffer.begin() + targetBitLeast;
		//binNumberPointer = repositoryBuffer + targetBitLeast;
		DigitIterator currentBinNumberPointer = binNumberPointer;
		
		// Դ��top->lowÿ��bit��ת��Ϊ2����(����������Դ�������Ķ����Ʊ�ʾ)
		while (originLeftTop != originRightLow) {
			// ��4������2λ��ʾ ����4 0010 �޷��������ת�� ��ȻreTopBitʹ�ü�ʹ���Ҳ�����ת��, �������Ǵ��
			int totalBits = TransitionUtility::decimalToRadixLowTopBase(*originLeftTop++, currentBinNumberPointer, 2);
			// [Դ��������]ת��Ϊ2���ƺ���[Դ�ı���λ��]�������
			TransitionUtility::zeroComplementAfterEnd(currentBinNumberPointer, currentBinNumberPointer + totalBits, originBitLeast);
			// ע��: ÿoriginBitLeastλ�����򴢴� �޷�ת��ΪÿtargetBitLeastλ�����򴢴� �����������Ǳ����
			int reTopBit = std::max(totalBits, originBitLeast);
			for (int i = 0; i < reTopBit / 2; i++) {
				std::swap(currentBinNumberPointer[i], currentBinNumberPointer[reTopBit - i - 1]);
			}
			currentBinNumberPointer += reTopBit;
		}

		targetTopLow.reserve((currentBinNumberPointer - binNumberPointer) / targetBitLeast);
		// ����Ӱ����һ��
		targetTopLow.resize(0);

		// ��ǰ����(�ڸպõ������ ��ಹ ���������ж���ȥ��ǰ��0)
		binNumberPointer -= targetBitLeast - ((currentBinNumberPointer - binNumberPointer) % targetBitLeast);
		// ������˷�0ֵ��Ϊtrue
		bool hasValidValue = false;
		// top->lowÿһ��targetBitLeastתΪ10����(����������Ŀ�����)
		for (int i = 0; i < currentBinNumberPointer - binNumberPointer; i += targetBitLeast) {
			// ȥ�����е�ת�����ǰ��0(ת��ǰ��ǰ��0ת������ܲ���)
			int number = TransitionUtility::radixTopLowToDecimal(binNumberPointer + i, binNumberPointer + i + targetBitLeast, 2);
			hasValidValue = number != 0 || hasValidValue;
			if (hasValidValue) {
				targetTopLow.push_back(number);
			}
		}
	}
};

/*
����10���ƵĽ���ת����
PS: ��API�Ͷ�����ת������Щ������
*/
class DecimalTransition : public Transition {
public:
	// �����б�: {���������10���ƻ�����������λ��{��λ��������Զ�����}, Դ����, Ŀ�����}
	DecimalTransition(JCE::SizeType maxDigitSize, int originRadix, int targetRadix) : Transition(10) {
		this->originBitLeast = 0;
		this->targetBitLeast = 0;
		this->originRadix = 0;
		this->targetRadix = 0;
		//repositoryBuffer = (int *)malloc(sizeof(int)*maxDigitSize);
		repositoryBuffer.resize(maxDigitSize);
		reset(originRadix, targetRadix);
	}

	// ����ת������: �Զ����㻺��[������]Ȼ��������С����ռ�(�������������Ƶ�1λ), ͬʱ��ʼ����Ҫ��Ԫ��
	void reset(int originRadix, int targetRadix) {
		// ������Ҫ��֤�����ʾ�ý��Ƶ����ֵ������
		_ASSERT_EXPR(StandardExtend::inRange(2, originRadix, (MAX_INT32) / 2), "radix = [2, (MAX_INT32)/2)");
		_ASSERT_EXPR(StandardExtend::inRange(2, targetRadix, (MAX_INT32) / 2), "radix = [2, (MAX_INT32)/2)");
		int originBitLeast = (int)std::ceil(MathExtend::logRadix(originRadix, this->bufferRadix));
		int targetBitLeast = (int)std::ceil(MathExtend::logRadix(targetRadix, this->bufferRadix));
		reSizeBuffer(repositoryBuffer.size() + resetBits(originBitLeast, targetBitLeast));
		this->originRadix = originRadix;
		this->targetRadix = targetRadix;
	}

	// ת��Ϊ������Ƶ���������: Դ����->10����->Ŀ�����; (Դ�������λ������, Դ�������λ�������ĺ�һ��, �������)
	// ���������ĸ�λ������topLeftIterator
	// targetTopLowBuffer���ܺ�origin��ͬ(ͬ���Ͳ�ͬ�����ĵ��������޷��Ƚϵ�, ���Լ��)
	template<class DigitIterator>
	DigitArray::iterator transition(DigitIterator originLeftTop, DigitIterator originRightLow, DigitArray &targetTopLowBuffer) {
		// 1. ����Դ���Ƶ�10����λ��
		// 2. ��Դ����ÿ1λ�� תΪ10���� �洢�ڻ�����: 1λԴ��������->1��10��������(���ܲ�ֹһλ, ���洢������ֻռ��1λ����)
		// 3. �������е�10��������תΪĿ�����
		JCE::SizeType originDigitNum = originRightLow - originLeftTop;
		int minDigitNum = calcMinDigitNum();
		if (originDigitNum + minDigitNum > repositoryBuffer.size()) {
			// ����ռ䲻��->������ʵĻ����С
			reSizeBuffer(originDigitNum + minDigitNum);
		}
		auto beginIt = repositoryBuffer.begin();
		auto endIt = repositoryBuffer.begin() + originDigitNum;

		TransitionUtility::radixTopLowToDecimalTopLow(originLeftTop, originRightLow, beginIt, endIt, originRadix);

		// Դ���Ƶ�1����λ��Ӧ��Ŀ����Ƶ�����λ��
		JCE::SizeType originToTargetDigitNum = MathExtend::calcDigitTotalSize(originRadix - 1, targetRadix);
		// ���һ����Ҫ��������λ
		JCE::SizeType originToTargetDigitMaxNum = originDigitNum*originToTargetDigitNum;
		// �����ô�С ��ʼֵӦ��Ϊ0
		targetTopLowBuffer.clear();
		// Ԥ��һλ��λ(���ֻ�����1λ)
		targetTopLowBuffer.resize(originToTargetDigitMaxNum + 1);
		auto bufferTopLowLeft = targetTopLowBuffer.begin() + 1;
		DigitArray buffer;
		buffer.resize(originToTargetDigitMaxNum);
		while (beginIt != endIt) {
			// �˴����Զ�����0��, ������
			TransitionUtility::decimalToRadixTopLow(*beginIt, buffer.begin(), buffer.end(), targetRadix, false);
			++beginIt;
			MathExtend::vectorPlush(
				bufferTopLowLeft, targetTopLowBuffer.end()
				, buffer.begin(), buffer.end()
				, bufferTopLowLeft, targetTopLowBuffer.end()
			);
			std::fill(buffer.begin(), buffer.end(), 0);
		}
		int carryTop = TransitionUtility::carryTopLow(targetTopLowBuffer.begin(), targetTopLowBuffer.end(), targetRadix);
		// ������������һ�ν�λ
		_ASSERT_EXPR(carryTop == 0, "�Դ���δ����Ľ�λ!");
		auto topIter = targetTopLowBuffer.begin();
		while(topIter != targetTopLowBuffer.end()) {
			if (*topIter != 0) {
				break;
			}
			++topIter;
		}
		// ���ȫ��0�Ļ����ص����ڶ�λ������
		return topIter == targetTopLowBuffer.end() ? --topIter : topIter;
	}
};
