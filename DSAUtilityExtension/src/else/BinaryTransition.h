#pragma once
#include "../pch.h"
#include "TransitionUtility.h"
#include "../ExtendSpace.h"

/*
������ת������
֧�ֿ���2���Ʊ�ʾ��������ƵĴ�����˫��ת��;
bits => [1, 32) ��: 2 4..2^31��31��[��2����];
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
class BinaryTransition {
public:
	using DigitArray = TransitionUtility::DigitArray;
	using DigitIterator = DigitArray::iterator;

	// �����б�: (��������ж����ƻ����������λ��, Դ���Ʊ���λ��, Ŀ����Ʊ���λ��)
	BinaryTransition(JCE::SizeType bitSize, int originBitLeast = 1, int targetBitLeast = 1) {
		this->originBitLeast = 0;
		this->targetBitLeast = 0;
		this->originRadix = 0;
		this->targetRadix = 0;
		//binaryBuffer = (int *)malloc(sizeof(int)*bitSize);
		binaryBuffer.resize(bitSize);
		reset(originBitLeast, targetBitLeast);
	}

	// �������ת��: �Զ����㻺��[������]Ȼ�����軺��ռ�, ͬʱ��ʼ��[��λ��]
	void reset(int originBitLeast, int targetBitLeast = 1) {
		_ASSERT_EXPR(StandardExtend::inRange(1, originBitLeast, 32), "radix = [1, 32)");
		_ASSERT_EXPR(StandardExtend::inRange(1, targetBitLeast, 32), "radix = [1, 32)");

		// �����м�->�����abs; �������->�����ϳ�:max; ����仯��: ����
		int currentMaxBits = (std::max)(this->originBitLeast, this->targetBitLeast);
		int nextMaxBits = (std::max)(originBitLeast, targetBitLeast);
		// ����ܱ���λ���&&������λ��� ��disBits=0 ����disBits = nextMaxBits - currentMaxBits
		int disBits = nextMaxBits - currentMaxBits;
		binaryBuffer.resize(binaryBuffer.size() + disBits);

		this->originBitLeast = originBitLeast;
		this->targetBitLeast = targetBitLeast;
		this->originRadix = (int)pow(2.0, originBitLeast);
		this->targetRadix = (int)pow(2.0, targetBitLeast);

		// Ԥ����[��λ��]�����ʼ��Ϊ0 �����λʱ�����
		std::for_each(binaryBuffer.begin(), binaryBuffer.begin() + targetBitLeast, [](int &bit) {bit = 0; });
		//memset(binaryBuffer, 0, targetBitLeast*sizeof(int));
	}

	// 2���״���ת��: Դ����->������->Ŀ�����
	// target�������յĽ��(������ֶ����0) PS: �������ָ��int��Χ�ڵĽ���(��һ��digitԪ�ر�����int��Χ��)
	void transition(char const *origin, DigitArray &target) {
		JCE::SizeType minTargetLength =
			std::strlen(origin)*MathExtend::calcDigitTotalSize(originRadix - 1, targetRadix);
		_ASSERT_EXPR(minTargetLength <= binaryBuffer.size(), "����ռ䲻��!");
		char oneBit = 0;
		// top��Ԥ��һ��[��λ��]���ڽ�λ����
		binNumberPointer = binaryBuffer.begin() + targetBitLeast;
		//binNumberPointer = binaryBuffer + targetBitLeast;
		currentBinNumberPointer = binNumberPointer;

		//top->lowÿ��Դbit��ת��Ϊ2����(����������Դ�������Ķ����Ʊ�ʾ)

		//while ((oneBit = getchar()) != '\n'){//�������������� ���������ⲿ�����ٶ���һ��char����(���������ܱȽϴ�)
		while ((oneBit = *(origin++)) != '\0') {
			int number = oneBit - '0';
			if (isalpha(oneBit)) {
				number = oneBit - 'A' + 10;
			}

			//��4������2λ��ʾ ����4 0010 �޷��������ת�� ��ȻreTopBitʹ�ü�ʹ���Ҳ�����ת��, �������Ǵ��
			_ASSERT_EXPR(number < originRadix, "����ĵ���Bit���ݳ��������Ƶ�ģ.");

			int totalBits = TransitionUtility::decimalToRadixLowTopBase(number, currentBinNumberPointer, 2);
			//number = radixLowTopToDecimalBase(currentBinNumberPointer, 2, totalBits);
			//�����
			TransitionUtility::complement(currentBinNumberPointer, currentBinNumberPointer + totalBits, originBitLeast);
			//ע��: ÿoriginBitLeastλ�����򴢴� �޷�ת��ΪÿtargetBitLeastλ�����򴢴� �����������Ǳ����
			int reTopBit = std::max(totalBits, originBitLeast);
			for (int i = 0; i < reTopBit / 2; i++) {
				std::swap(currentBinNumberPointer[i], currentBinNumberPointer[reTopBit - i - 1]);
			}
			currentBinNumberPointer += reTopBit;
		}

		//int ansSub = 0;
		target.reserve((currentBinNumberPointer - binNumberPointer) / targetBitLeast);
		//����Ӱ����һ��
		target.resize(0);

		//��ǰ����(�ڸպõ������ ��ಹ ���������ж���ȥ��ǰ��0)
		binNumberPointer -= targetBitLeast - ((currentBinNumberPointer - binNumberPointer) % targetBitLeast);
		//������˷�0ֵ��Ϊtrue
		bool outputValidValue = false;
		//top->lowÿһ��targetBitLeastתΪ10����(����������Ŀ�����)
		for (int i = 0; i < currentBinNumberPointer - binNumberPointer; i += targetBitLeast) {
			//ȥ�����е�ת�����ǰ��0(ת��ǰ��ǰ��0ת������ܲ���)
			int number = TransitionUtility::radixTopLowToDecimal(binNumberPointer + i, 2, targetBitLeast);
			outputValidValue = number != 0 || outputValidValue;
			if (outputValidValue) {
				target.push_back(number);
			}
			//printf(outputValidValue ? "%d" : "", number);
		}
		//puts("");
	}

	// ת��Ϊ�������[2, (MAX_INT32)/2)����������(������Ҫ��֤�����ʾ�ý��Ƶ����ֵ������)
	//void

private:
	// ת��˫���Ĵ���λ: һλ��������(���Ǹպõ���˼)��Ҫ���ٱ���λ��ʾ
	int originBitLeast;
	int originRadix;
	int targetBitLeast;
	int targetRadix;
	// repository
	DigitArray binaryBuffer;// = NULL
	DigitIterator binNumberPointer;// = NULL
	DigitIterator currentBinNumberPointer;// = NULL
};
