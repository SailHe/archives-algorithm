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
	int calcDisBits(int originBitLeast, int targetBitLeast) {
		// �����м�->�����abs; �������->�����ϳ�:max; ����仯��: ����
		int currentMaxBits = (std::max)(this->originBitLeast, this->targetBitLeast);
		int nextMaxBits = (std::max)(originBitLeast, targetBitLeast);
		// ����ܱ���λ���&&������λ��� ��disBits=0 ����disBits = nextMaxBits - currentMaxBits
		int disBits = nextMaxBits - currentMaxBits;
		return disBits;
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
	// �����б�: (��������ж����ƻ����������λ��, Դ���Ʊ���λ��, Ŀ����Ʊ���λ��)
	BinaryTransition(JCE::SizeType bitSize, int originBitLeast = 1, int targetBitLeast = 1) : Transition(2) {
		this->originBitLeast = 0;
		this->targetBitLeast = 0;
		this->originRadix = 0;
		this->targetRadix = 0;
		//repositoryBuffer = (int *)malloc(sizeof(int)*bitSize);
		repositoryBuffer.resize(bitSize);
		reset(originBitLeast, targetBitLeast);
	}

	// �������ת��: �Զ����㻺��[������]Ȼ�����軺��ռ�, ͬʱ��ʼ��[��λ��]
	void reset(int originBitLeast, int targetBitLeast) {
		_ASSERT_EXPR(StandardExtend::inRange(1, originBitLeast, 32), "radixBits => [1, 32)");
		_ASSERT_EXPR(StandardExtend::inRange(1, targetBitLeast, 32), "radixBits => [1, 32)");

		int disBits = calcDisBits(originBitLeast, targetBitLeast);
		repositoryBuffer.resize(repositoryBuffer.size() + disBits);

		this->originBitLeast = originBitLeast;
		this->targetBitLeast = targetBitLeast;
		this->originRadix = (int)pow(2.0, originBitLeast);
		this->targetRadix = (int)pow(2.0, targetBitLeast);

		// Ԥ����[��λ��]�����ʼ��Ϊ0 �����λʱ�����
		std::for_each(repositoryBuffer.begin(), repositoryBuffer.begin() + targetBitLeast, [](int &bit) {bit = 0; });
		//memset(repositoryBuffer, 0, targetBitLeast*sizeof(int));
	}

	// 2���״���ת��: Դ��2����->������->Ŀ���2����; targetTopLow�������յĽ��(������ֶ����0)
	void transition(char const *origin, DigitArray &targetTopLow) {
		JCE::SizeType originToTargetDigitMaxNum =
			std::strlen(origin)*MathExtend::calcDigitTotalSize(originRadix - 1, targetRadix);
		_ASSERT_EXPR(originToTargetDigitMaxNum <= repositoryBuffer.size(), "����ռ䲻��!");
		char digitAscll = 0;
		// top��Ԥ��һ��[��λ��]���ڽ�λ����
		DigitIterator binNumberPointer = repositoryBuffer.begin() + targetBitLeast;
		//binNumberPointer = repositoryBuffer + targetBitLeast;
		DigitIterator currentBinNumberPointer = binNumberPointer;
		
		// Դ��top->lowÿ��bit��ת��Ϊ2����(����������Դ�������Ķ����Ʊ�ʾ)
		while ((digitAscll = *(origin++)) != '\0') {
			// ��4������2λ��ʾ ����4 0010 �޷��������ת�� ��ȻreTopBitʹ�ü�ʹ���Ҳ�����ת��, �������Ǵ��
			int number = TransitionUtility::toRadixIntNum(digitAscll, originRadix);

			int totalBits = TransitionUtility::decimalToRadixLowTopBase(number, currentBinNumberPointer, 2);
			//number = radixLowTopToDecimalBase(currentBinNumberPointer, 2, totalBits);
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

class DecimalTransition : public Transition {
public:
	// �����б�: (��������ж����ƻ�����������λ��, Դ����, Ŀ�����)
	DecimalTransition(JCE::SizeType digitSize, int originRadix, int targetRadix) : Transition(10) {
		this->originBitLeast = 0;
		this->targetBitLeast = 0;
		this->originRadix = 0;
		this->targetRadix = 0;
		//repositoryBuffer = (int *)malloc(sizeof(int)*digitSize);
		repositoryBuffer.resize(digitSize);
		reset(originRadix, targetRadix);
	}

	// ����ת������: �Զ����㻺��[������]Ȼ�����軺��ռ�, ͬʱ��ʼ����Ҫ��Ԫ��
	void reset(int originRadix, int targetRadix) {
		// ������Ҫ��֤�����ʾ�ý��Ƶ����ֵ������
		_ASSERT_EXPR(StandardExtend::inRange(1, originRadix, (MAX_INT32) / 2), "radix = [2, (MAX_INT32)/2)");
		_ASSERT_EXPR(StandardExtend::inRange(1, targetRadix, (MAX_INT32) / 2), "radix = [2, (MAX_INT32)/2)");

		this->originBitLeast = (int)std::ceil(MathExtend::logRadix(originRadix, this->bufferRadix));
		this->targetBitLeast = (int)std::ceil(MathExtend::logRadix(targetRadix, this->bufferRadix));
		this->originRadix = originRadix;
		this->targetRadix = targetRadix;

		int disBits = calcDisBits(this->originBitLeast, this->targetBitLeast);
		repositoryBuffer.resize(repositoryBuffer.size() + disBits);
	}

	// ת��Ϊ������Ƶ���������: Դ����->10����->Ŀ�����; (Դ�������λ������, Դ�������λ�������ĺ�һ��, �������)
	// ���������ĸ�λ������topLeftIterator
	// targetTopLowBuffer���ܺ�origin��ͬ(ͬ���Ͳ�ͬ�����ĵ��������޷��Ƚϵ�, ���Լ��)
	template<class DigitIterator>
	DigitArray::iterator transition(DigitIterator originLeftTop, DigitIterator originRightLow, DigitArray &targetTopLowBuffer) {
		// 1. ����Դ���Ƶ�10����λ��
		// 2. ��Դ����ÿ1λ������0�� תΪ10���� �洢�ڻ�����
		// 3. �������е�10��������תΪĿ�����
		JCE::SizeType originDigitNum = originRightLow - originLeftTop;
		_ASSERT_EXPR(originDigitNum <= repositoryBuffer.size(), "����ռ䲻��!");
		auto beginIt = repositoryBuffer.begin();
		auto endIt = repositoryBuffer.end();

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
