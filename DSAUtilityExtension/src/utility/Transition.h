#pragma once
#include "../pch.h"
#include "TransitionUtility.h"
#include "../ExtendSpace.h"
#include "../DSAUEdll.h"

// ����ת����
class DSAUTILITYEXTENSION_API Transition {
public:
	using DigitArray = TransitionUtility::DigitVariableArray;
	using DigitIterator = TransitionUtility::DigitVariableArrayIterator;
	
protected:
	Transition(int bufferRadix);
	virtual ~Transition();
	// ���ر�ת������Ӧ�����ֽ��ƻ���ת��ʱ����С����Ԥ��λ(���������Ƶ�λ���ֵ��Ӧ�Ļ�����Ƶ�λ��)
	int calcMinDigitNum();
	// ���軺�����ռ������λ, ������СԤ��λ���ı仯��
	int resetBits(int originBitLeast, int targetBitLeast);

	void reSizeBuffer(JCE::SizeType nextSize);

	// ת��˫���Ĵ���λ: һλ��������(���Ǹպõ���˼)��Ҫ���ٻ����������λ��ʾ
	int originBitLeast;
	int originRadix;

	int bufferRadix;
	DigitArray repositoryBuffer;

	int targetBitLeast;
	int targetRadix;
};

/*
������ת������: ʹ�ö���������(bit, binNumber)ʵ�ֵĽ���ת����
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
class DSAUTILITYEXTENSION_API BinaryTransition : public Transition {
public:
	// �����б�: {��������ж����ƻ����������λ��(��ʵ����������������Զ�����), Դ���Ʊ���λ��, Ŀ����Ʊ���λ��}
	BinaryTransition(JCE::SizeType maxBitSize, int originBitLeast = 1, int targetBitLeast = 1);

	// �������λ��: �Զ����㻺��[������]Ȼ��������С����ռ�(�������������Ƶ�1λ), ͬʱ��ʼ��[��λ��]
	void reset(int originBitLeast, int targetBitLeast);

	// 2���״���ת��: Դ��2����->������->Ŀ���2����; targetTopLow�������յĽ��(������ֶ����0)
	template<class DigitIterator>
	void transition(DigitIterator originLeftTop, DigitIterator originRightLow, std::vector<int> &targetTopLow) {
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
		auto bitIterator = repositoryBuffer.begin() + targetBitLeast;
		//bitIterator = repositoryBuffer + targetBitLeast;
		auto currentBitIterator = bitIterator;
		
		// Դ��top->lowÿ��bit��ת��Ϊ2����(����������Դ�������Ķ����Ʊ�ʾ)
		while (originLeftTop != originRightLow) {
			auto leftTopIter = TransitionUtility::decimalToRadixTopLow(*originLeftTop++, currentBitIterator + originBitLeast, 2);
			std::fill(currentBitIterator, leftTopIter, 0);
			currentBitIterator += originBitLeast;
		}

		targetTopLow.reserve((currentBitIterator - bitIterator) / targetBitLeast);
		// ����Ӱ����һ��
		targetTopLow.resize(0);

		// ��ǰ����(�ڸպõ������ ��ಹ ���������ж���ȥ��ǰ��0)
		bitIterator -= targetBitLeast - ((currentBitIterator - bitIterator) % targetBitLeast);
		// ������˷�0ֵ��Ϊtrue
		bool hasValidValue = false;
		// top->lowÿһ��targetBitLeastתΪ10����(����������Ŀ�����)
		for (int i = 0; i < currentBitIterator - bitIterator; i += targetBitLeast) {
			// ȥ�����е�ת�����ǰ��0(ת��ǰ��ǰ��0ת������ܲ���)
			int number = TransitionUtility::radixTopLowToDecimal(bitIterator + i, bitIterator + i + targetBitLeast, 2);
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
class DSAUTILITYEXTENSION_API DecimalTransition : public Transition {
public:
	// �����б�: {���������10���ƻ�����������λ��{��λ��������Զ�����}, Դ����, Ŀ�����}
	DecimalTransition(JCE::SizeType maxDigitSize, int originRadix, int targetRadix);

	// ����ת������: �Զ����㻺��[������]Ȼ��������С����ռ�(�������������Ƶ�1λ), ͬʱ��ʼ����Ҫ��Ԫ��
	void reset(int originRadix, int targetRadix);

	// ת��Ϊ������Ƶ���������: Դ����->10����->Ŀ�����; (Դ�������λ������, Դ�������λ�������ĺ�һ��, �������)
	// ���������ĸ�λ������topLeftIterator
	// targetTopLowBuffer���ܺ�origin��ͬ(ͬ���Ͳ�ͬ�����ĵ��������޷��Ƚϵ�, ���Լ��)
	template<class DigitIterator>
	std::vector<int>::iterator transition(DigitIterator originLeftTop, DigitIterator originRightLow, std::vector<int> &targetTopLowBuffer) {
		// 1. ����Դ���Ƶ�10����λ��
		// 2. ��Դ����ÿ1λ�� תΪ10���� �洢�ڻ�����: 1λԴ��������->1��10��������(���ܲ�ֹһλ, ���洢������ֻռ��1λ����)
		// 3. �������е�10��������תΪĿ�����
		JCE::SizeType originDigitNum = originRightLow - originLeftTop;
		int minDigitNum = calcMinDigitNum();
		if (originDigitNum + minDigitNum > repositoryBuffer.size()) {
			// ����ռ䲻��->������ʵĻ����С
			reSizeBuffer(originDigitNum + minDigitNum);
		}
		auto beginDecNumIterator = repositoryBuffer.begin();
		auto endDecNumIterator = repositoryBuffer.begin() + originDigitNum;

		auto realIterPair = 
			TransitionUtility::radixTopLowToDecimalTopLow(originRightLow, endDecNumIterator
				, originDigitNum, (JCE::SizeType)originRadix);
		_ASSERT(originLeftTop == realIterPair.first && beginDecNumIterator == realIterPair.second);

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
		while (beginDecNumIterator != endDecNumIterator) {
			auto realBginIter = TransitionUtility::decimalToRadixTopLow(*beginDecNumIterator, buffer.end(), targetRadix);
			// 0����
			std::fill(buffer.begin(), realBginIter, 0);
			++beginDecNumIterator;
			MathExtend::vectorPlush(
				bufferTopLowLeft, targetTopLowBuffer.end()
				, buffer.begin(), buffer.end()
				, bufferTopLowLeft, targetTopLowBuffer.end()
			);
		}
		int carryTop = TransitionUtility::carryTopLow(targetTopLowBuffer.begin(), targetTopLowBuffer.end(), targetRadix);
		// ������������һ�ν�λ
		_ASSERT_EXPR(carryTop == 0, "�Դ���δ����Ľ�λ!");

		// ����top��0������
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
