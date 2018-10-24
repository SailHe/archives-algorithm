#include<stdio.h>
#include<string.h>
#include<string>
#include<cmath>
#include<vector>
#include<iostream>
#include<queue>
#include<functional>//greater
#include<list>
#include<stack>
#include<map>
#include<iomanip>//cout���ֲ�����
#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

//#define MAX_N 1000+5
#define MAX_INT_NUM 2147483647
#define MIN_INT_NUM (-MAX_INT_NUM-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>
using namespace std;

template<class T> using ArrayList = vector<T>;
using SizeType = size_t;
using I64 = long long;

//������ת������(֧�� ����2���Ʊ�ʾ��������ƵĴ����ĵ���ת��) ������Ҫ�û��Լ�����
class BinaryTransition {
	//ת��˫���Ĵ���λ: һλ������Ҫ����λ�����Ʊ�ʾ
	int originBitLeast = 1;
	int originRadix = (int)pow((double)2, originBitLeast);
	int targetBitLeast = 1;
	typedef vector<int> Varray;
	typedef Varray::iterator VarrayIterator;
	Varray repository;// = NULL
	VarrayIterator binNumberPointer;// = NULL
	VarrayIterator currentBinNumberPointer;// = NULL
public:
	
	//������ת������(������������) ���ظ����ֵ���������
	static char toAbs(int &number) {
		return number < 0 ? (number = -number), '-' : '+';
	}

	//����->���� (����λ�� �Լ� repositoryLowToTop��λ���λ�洢����ֵ���������)
	static int tenToRadix(unsigned number, VarrayIterator repositoryLowToTop, int radix) {
		static int bit;
		bit = -1;
		//Ҫ��֤��ʹ����0Ҳ��ִ��һ��
		do {
			repositoryLowToTop[++bit] = number % radix;
			number /= radix;
		} while (number != 0);
		return bit + 1;
	}

};


/*��֧�ֲ�����λ, ��ŵĸ�������*/
class BigInteger {
	//��ʾ����λԪ�Ľ���
	//@Bug 2����ʱ�ᱨ��
	const static int radix = 10000;
	const static int ONE = 1;
	const static int ZERO = 0;
	//����λԪ������(�����ܹ����������������ֵ��˵�ֵ)
	typedef int BitType;
	typedef vector<BitType> Base;
	//�����ű�ʶ ���ʱ���Ų����
	char symbol = 0;
	//��������λԪ ������(Ϊ��֧�����ֵ�ֱ�ӹ���������ȽϷ��� ���ַ�������Ӱ�첻��)
	vector<BitType> digitLowTop;

	//���������������͵���λ�� 0: 1bit; 10: 2bit
	static int totalBitOf(BitType originNumber) {
		int totalBit = 0;
		do {
			originNumber /= 10;
			++totalBit;
		} while (originNumber != 0);
		return totalBit;
	}

	//������������Ĵ���λ�� (grow:ֻ����(advance only) С��ԭλ��ʱ�����´�С)
	void setTotalBit(size_t totalBit, bool grow = true) {
		//����֪�����ֵĴ��λ�� ������ǰָ�� �����ڴ����
		if (grow && totalBit <= digitLowTop.size())
			return;
		digitLowTop.resize(totalBit);
	}
	//��Դ����ת��Ϊ���ظ�ʽ����
	void transitionToLocalRadix(int originNumber) {
		int numTotalBit = totalBitOf(originNumber);
		//һ���ֵ�λ��: ����λ(��λ)�Ƚ��Ƶ���λ����һ
		int wordBit = totalBitOf(radix) - 1;
		//�����������޷������ִ���ʱ��Ҫ����һ����������(�����λ)
		setTotalBit(numTotalBit / wordBit + (numTotalBit % wordBit == 0 ? 0 : 1));
		symbol = BinaryTransition::toAbs(originNumber);
		BinaryTransition::tenToRadix(originNumber, digitLowTop.begin(), radix);
		/*int i = -1;
		do{
			digitLowTop[++i] = originNumber % radix;
			originNumber /= radix;
		} while (originNumber != 0);*/
	}

public:
	void print() {
		const char map[17] = "0123456789ABCDEF";
		int totalBit = digitLowTop.size();
		printf(symbol == '-' ? "-" : "");
		printf("%d", digitLowTop[totalBit - 1]);
		for (int i = 1; i < totalBit; i++) {
			printf("%04d", digitLowTop[totalBit - i - 1]);
		}
		puts("");
	}

	BigInteger(BitType originNumber) {
		transitionToLocalRadix(originNumber);
	}

	//�˷�ǰ����ȷ���һ��
	void reserve(size_t newSize) {
		digitLowTop.reserve(newSize);
	}

	//<==> += 
	BigInteger &plus(BigInteger const &rhs) {
		BitType subLhs, subRhs, subSum;
		//���ֱ߶�Ӧλ��ֵ ��λ addValue(����) = rhsBit + carryBit
		BitType rhsBit, carryBit = 0;
		//����
		int wordCntLhs = digitLowTop.size(), wordCntRhs = rhs.digitLowTop.size();

		for (subSum = subLhs = subRhs = 0; subLhs < wordCntLhs || subRhs < wordCntRhs || carryBit != 0; ++subSum) {
			//������� ��rhs��ȡ��ϻ����ܴ��ڽ�λ�� ��ʱ������Ϊ0�ͺ�
			rhsBit = subRhs < wordCntRhs ? rhs.digitLowTop[subLhs] : 0;

			if (subLhs < wordCntLhs) {
				digitLowTop[subSum] += rhsBit + carryBit;
			}
			else {
				//�����ߵ�����lhs < rhs�Ļ���λ��0����addvalue����0
				digitLowTop.push_back(rhsBit + carryBit);
			}

			carryBit = digitLowTop[subSum] / radix;
			digitLowTop[subSum] %= radix;
			++subLhs, ++subRhs;
		}
		return *this;
	}
	//���ٳ˷� (����λ <= 10000) ����push_back�Ĵ��ڲ����Ż��ض��Ȳ���ֱ��д���㷨 �����Ż�����Ҫ��һ�� �����ڴ��๦����ȫ
	BigInteger &muity(BitType muityBit) {
		//int units = 1������λ: ��ʾmuityBit��һ��λ������
		BitType carryBit = 0;
		BitType subProduct;
		int totalBit = digitLowTop.size();
		for (subProduct = 0; subProduct < totalBit || carryBit; subProduct++) {
			if (subProduct < totalBit)
				digitLowTop[subProduct] = digitLowTop[subProduct] * muityBit + carryBit;
			else {
				//λ�˷�ֻ�����ǽ�λ
				digitLowTop.push_back(carryBit);
			}
			carryBit = digitLowTop[subProduct] / radix;
			digitLowTop[subProduct] %= radix;
		}
		return *this;
	}
	
	BigInteger operator+(BigInteger const &rhs) {
		return BigInteger(*this).plus(rhs);
	}
};

//��С�ļ����������ǣ�D1 = 0��D2 = 1��D3=2��D4 = 9��D5 = 44��D6 = 265��D7 = 1854
ArrayList<BigInteger> calcIllArrangeList(SizeType MAX_SIZE = 26) {
	ArrayList<BigInteger> M(MAX_SIZE, 0);
	M[0] = 0, M[1] = 0, M[2] = 1;
	for (SizeType n = 3; n < MAX_SIZE; ++n)
		M[n] = (M[n - 1] + M[n - 2]).muity(n - 1);
	return M;
}

int mainForIllArrange() {
	auto table = calcIllArrangeList(1001);
	int n;
	//table[100].print();
	while (1 == scanf("%d", &n)) {
		table[n].print();
	}
	return 0;
}

int main_0() {
	mainForIllArrange();
	return 0;
}


// ps: ACM OJӦ���ǿ��˱������Ż���
