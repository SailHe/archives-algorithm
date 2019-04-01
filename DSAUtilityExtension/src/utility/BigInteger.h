#ifndef _BIGINTEGER_H
#define _BIGINTEGER_H

#include "../pch.h"
#include "TransitionUtility.h"
//#include "Transition.h"
#include "../ExtendSpace.h"

/*
@����:
���bit��ָλ�Ļ� ���������ÿ��Ԫ�ؿɳ���bit;
�������ս������ÿ���ַ�����bit(10����λ), ������һ��Ԫ��Ӧ����Byte(������е���)
���ǻ�һ���Ƕ�ÿ��Ԫ����ʵҲ���Բ��Ƕ���ַ�, ��ôһ�����ΪbitҲ�޿ɺ��(���������)
��������bit, byteʵ�����Ǽ����������, ����bitרָ����λ����Ϣ��λ, byteרָ�ֽ�, ��ʾһ��8λ(bit)Ԫ��

��ô������û�н����, ��ô������ʷ����ԭ��, ����:
���Ⱪ¶�Ľӿ�һ��:10�����ַ�������: length; �����С(size): totalByteNum; �ڲ�ʵ��ʱһ������Ԫ��: byte

*/

/*
��������Ƶ�10���ƴ�����
��֧�ֲ�����λ, ��ŵĸ�������
*/
class BigInteger{
	//����λԪ(��)������(�����ܹ����������������ֵ��˵�ֵ)
	typedef int ByteType;
	typedef JCE::ArrayList<ByteType> BaseImpl;
	const static int ONE = 1;
	const static int ZERO = 0;
public:
	//���ض�Ӧʵ���ַ�������(��������) size = length + 1
	int length() const {
		return calcTotalBitNum() + (symbol() == '\0' ? 0 : 1);
	}
	//  ���ض�Ӧʵ���ַ��������ֵ���λ��(����������)
	int calcTotalBitNum() const {
		int totalByteNum = getTotalByteNum();
		//���ֵ�λ�� + ʣ������ * һ�ֵ�λ��-1
		return
			totalBitOf(digitLowTop[totalByteNum - 1]) +
			(totalByteNum - 1)*(totalBitOf(radix)-1);
	}
	void print() const {
		const char map[17] = "0123456789ABCDEF";
		int totalByteNum = getTotalByteNum();
		putchar(symbol());
		printf("%d", digitLowTop[totalByteNum - 1]);
		for (int i = 1; i < totalByteNum; i++){
			printf("%04d", digitLowTop[totalByteNum - i - 1]);
		}
		puts("");
	}
	void print(char *outBuffer, JCE::SizeType bufferSize) const {
		int totalByteNum = getTotalByteNum();
		//sprintf_s(outBuffer, bufferSize, symbol());
		*outBuffer = symbol();
		if (*outBuffer == '\0') {
			// do nothing
		}
		else {
			++outBuffer;
			--bufferSize;
		}
		sprintf_s(outBuffer, bufferSize, "%d", digitLowTop[totalByteNum - 1]);
		bufferSize -= strnlen_s(outBuffer, bufferSize);
		outBuffer += totalBitOf(digitLowTop[totalByteNum - 1]);
		const int radixTotalbit = totalBitOf(radix);
		const int radixZerobit = radixTotalbit - 1;
		std::string format = "%0" + std::to_string(radixZerobit) + "d";
		for (int i = 1; i < totalByteNum; ++i){
			//"%04d"
			sprintf_s(outBuffer, bufferSize, format.c_str(), digitLowTop[totalByteNum - i - 1]);
			outBuffer += radixZerobit;
			bufferSize -= radixZerobit;
		}
		outBuffer[0] = '\0';
	}
	std::string const &toString() const {
		// std::string��length��size ����ȫһ����
		// �˴�size��lengthȡchar[]�еĺ��� length:'\0'֮ǰ, size: ��������
		int size = length() + 1;
		static std::string result("\0", size);
		result.assign(size, '\0');
		print(&result[0], size);
		return result;
	}
	BigInteger(ByteType originNumber){
		signum = originNumber > 0 ? 1
			: originNumber == 0 ? 0 : -1;
		transitionToLocalRadix(originNumber);
	}
	BigInteger(std::string const &originNumberTopLow){
		std::string temp = TransitionUtility::formatString(originNumberTopLow);
		signum = temp[0] == '-' ? -1
			: temp[0] == '0' ? 0 : 1;
		transitionToLocalRadix(temp);
	}

	//�˷�ǰ����ȷ���һ��
	void reserve(size_t newSize){
		digitLowTop.reserve(newSize);
	}
	//�ж��Ƿ�ż�� �Ƿ���true
	bool isEvenNumber() const {
		return this->digitLowTop[0] % 2 == 0;
	}

	//<==> +=
	BigInteger &plus(BigInteger const &rhs){
		ByteType subLhs, subRhs, subSum;
		//���ֱ߶�Ӧλ��ֵ ��λ addValue(����) = rhsBit + carryBit
		ByteType rhsBit, carryBit = 0;
		//����
		int wordCntLhs = getTotalByteNum(), wordCntRhs = rhs.getTotalByteNum();

		for (subSum = subLhs = subRhs = 0; subLhs < wordCntLhs || subRhs < wordCntRhs || carryBit != 0; ++subSum){
			//������� ��rhs��ȡ��ϻ����ܴ��ڽ�λ�� ��ʱ������Ϊ0�ͺ�
			rhsBit = subRhs < wordCntRhs ? rhs.digitLowTop[subLhs] : 0;

			if (subLhs < wordCntLhs){
				digitLowTop[subSum] += rhsBit + carryBit;
			}
			else{
				//�����ߵ�����lhs < rhs�Ļ���λ��0����addvalue����0
				digitLowTop.push_back(rhsBit + carryBit);
			}

			carryBit = digitLowTop[subSum] / radix;
			digitLowTop[subSum] %= radix;
			++subLhs, ++subRhs;
		}
		return *this;
	}
	/**
	 * Returns a BigInteger whose value is {@code (this - val)}.
	 *
	 * @param  val value to be subtracted from this BigInteger.
	 * @return {@code this - val}
	 */
	/*BigInteger subtract(BigInteger const &val) {
		if (val.signum == 0)
			return *this;
		if (signum == 0)
			return val.negate();
		if (val.signum != signum)
			return BigInteger(add(mag, val.mag), signum);

		int cmp = compareMagnitude(val);
		if (cmp == 0)
			return ZERO;
		int[] resultMag = (cmp > 0 ? subtract(mag, val.mag)
			: subtract(val.mag, mag));
		resultMag = trustedStripLeadingZeroInts(resultMag);
		return new BigInteger(resultMag, cmp == signum ? 1 : -1);
	}*/

	/**
	 * Returns a BigInteger whose value is {@code (-this)}.
	 *
	 * @return {@code -this}
	 */
	 //���� ����
	void minus();
	BigInteger negate() const {
		BigInteger neg(*this);
		neg.signum = -signum;
		return neg;
	}
	
	BigInteger sub(BigInteger const &rhs) {
		// (��, ��ȥ, ����)�Ƿ�С����
		bool minus = *this < rhs;
		ByteType subLhs, subRhs, subSum;
		//subValue(����) = rhsBit + borrowBit(��λ)
		ByteType rhsBit, borrowBit = 0;
		//����
		int wordCntLhs = getTotalByteNum(), wordCntRhs = rhs.getTotalByteNum();
		//����������ȡ���(���ùܽ�λ �����޷������Ͻ�λ) -> ��ֹ
		for (subSum = subLhs = subRhs = 0; subLhs < wordCntLhs || subRhs < wordCntRhs; ++subSum) {
			//������� ��rhs��ȡ��ϻ����ܴ��ڽ�λ�� ��ʱ������Ϊ0�ͺ�
			rhsBit = subRhs < wordCntRhs ? rhs.digitLowTop[subLhs] : 0;
			rhsBit *= -1;

			if (subLhs < wordCntLhs) {
				digitLowTop[subSum] += rhsBit + borrowBit;
			}
			else {
				//�����ߵ�����lhs < rhs�Ļ���λ��0����addvalue����0
				digitLowTop.push_back(rhsBit);
			}

			borrowBit = digitLowTop[subSum] < 0 ? 1 : 0;
			borrowBit *= -1;
			digitLowTop[subSum] %= radix;
			++subLhs, ++subRhs;
		}
		//signum = -signum;
		//plus(rhs.negate());
		if (minus) {
			for (JCE::SizeType i = 0; i < digitLowTop.size(); ++i) {
				digitLowTop[i] = -digitLowTop[i];
			}
			//С���� �Լ��ķ���Ҫ�仯
			signum = -signum;
		}
		return *this;
	}
	//�����˷� (���ڼӷ�) ����ֵ�Ͳ����������Լ�(����ֵ��Ϊ�˱����������� ����ļ�д��������ͬ)
	BigInteger &muity(BigInteger const &rhs) {
		BigInteger addValue(*this);
		// ��ֹ�Լ����Լ�
		BigInteger rhsTemp(rhs);
		for (BigInteger i = 1; i < rhsTemp; ++i){
			plus(addValue);
		}
		return *this;
	}
	//���ӳ˷�
	BigInteger &muityDouble(ByteType rhs) {
		if (rhs == 0) {
			*this = BigInteger(0);
		}
		else if (rhs == 1) {
			// do nothing
		}
		else {
			//��һ������ʱ���Ҫ���һ���Լ�
			BigInteger temp = rhs % 2 == 0 ? 0 : *this;
			//����rhs����2ʱѭ��: ÿ�μӱ�
			while (rhs > 1) {
				muity2();
				//�˴��ɻ�Ϊ����
				rhs /= 2;
			}
			plus(temp);
		}
		return *this;
	}
	//���س˷������ʱ������(ͬjava API)
	BigInteger multiply(BigInteger const &rhs) const {
		///��ֱ�ӵ�muity: ��һ����ʱ����
		BigInteger result(*this);
		for (BigInteger i = 1; i < rhs; ++i) {
			result.plus(*this);
		}
		return result;
	}
	//��Ԫ�˷� (����λ <= 10000)
	BigInteger &muity(ByteType muityBit){
		//����push_back�Ĵ��ڲ����Ż��ض��Ȳ���ֱ��д���㷨 �����Ż�����Ҫ��һ�� �����ڴ��๦����ȫ
		//int units = 1������λ: ��ʾmuityBit��һ��λ������
		ByteType carryBit = 0;
		ByteType subProduct;
		int totalByteNum = getTotalByteNum();
		for (subProduct = 0; subProduct < totalByteNum || carryBit; subProduct++){
			if (subProduct < totalByteNum)
				digitLowTop[subProduct] = digitLowTop[subProduct] * muityBit + carryBit;
			else{
				//λ�˷�ֻ�����ǽ�λ
				digitLowTop.push_back(carryBit);
			}
			carryBit = digitLowTop[subProduct] / radix;
			digitLowTop[subProduct] %= radix;
		}
		return *this;
	}
	BigInteger multiply(ByteType const &rhs) const {
		BigInteger result(*this);
		result.muity(rhs);
		return result;
	}
	//���ٽ׳� (������Ԫ�˷� n<=10000)
	BigInteger &fact(int n){
		++n;
		while (--n > 0){
			muity(n);
		}
		return *this;
	}
	//�����׳� (���ڼ����˷� ���Ż��������1000!Լ1s)
	BigInteger &fact(){
		static BigInteger One(1), Zero(0);
		//Ϊ�˲�ʹ�ü��� ֻ����ô���� ʹ�ü����Ļ�����������Բ��ö���
		BigInteger product = 1;
		if (*this > One){
			++*this;
			for (BigInteger i(1); i < *this; ++i){
				product.muity(i);
			}
			*this = product;
		}
		else if(*this == Zero){
			//0�Ľ׳�Ϊ1
			(*this) = One;
		}
		else{
			//����û�н׳�
		}
		return *this;
	}
	bool operator<(const BigInteger& rhs) const{
		return compare(rhs) == -1;
	}
	bool operator>(const BigInteger& rhs) const{
		return compare(rhs) == 1;
	}
	bool operator==(const BigInteger& rhs) const{
		return compare(rhs) == 0;
	}
	bool operator!=(const BigInteger& rhs) const{
		return compare(rhs) != 0;
	}
	BigInteger operator+(BigInteger const &rhs) const {
		return BigInteger(*this).plus(rhs);
	}
	BigInteger operator+=(BigInteger const &rhs) {
		return plus(rhs);
	}
	// ���ڼ����˷�
	BigInteger operator*(BigInteger const &rhs) const {
		BigInteger result(*this);
		for (BigInteger i = 1; i < rhs; ++i){
			result.plus(*this);
		}
		return result;
	}
	// ������Ԫ�˷�
	BigInteger operator*(ByteType const &muityBit) const {
		BigInteger result = *this;
		result.muity(muityBit);
		return result;
	}
	BigInteger operator*=(BigInteger const &rhs){
		return muity(rhs);
	}
	//ǰ�ð�
	BigInteger& operator++(){
		this->plus(ONE);
		return *this;
	}
	BigInteger operator++(int){
		BigInteger tmp = *this;
		++(*this);
		return tmp;
	}

	/*������ƺ�û���ò���ʵ�ּ���  �ɽ�λ����(Ψһ���е�Ӧ����ģ�����)*/
	BigInteger operator-(BigInteger const &rhs){
		return BigInteger(*this).sub(rhs);
	}
	BigInteger operator/(BigInteger const &rhs){
		this->plus(rhs);
		return *this;
	}
	BigInteger operator--(){//ǰ�ð�
		this->plus(BigInteger(-1));
		return *this;
	}
	BigInteger operator--(int){
		BigInteger tmp = *this;
		--(*this);
		return tmp;
	}

	//cin>> (ֻʵ����int��Χ����)
	friend std::istream &operator>>(std::istream &is, BigInteger &rhs) {
		ByteType temp;
		is >> temp;
		rhs = BigInteger(temp);
		return is;
	}
	//cout<<
	friend std::ostream &operator<<(std::ostream &os, const BigInteger &rhs) {
		os << rhs.toString();
		return os;
	}
	//ֻ�����ػ���������������� ���ұ�ʱ����ʽ���ù��췽��
	friend BigInteger operator+(const ByteType lhs, const BigInteger &rhs) {
		return rhs + lhs;
	}
	friend BigInteger operator*(const ByteType lhs, const BigInteger &rhs) {
		return rhs * lhs;
	}
	//��doubleת��
	/*operator double()const {
		int iValue = std::atof(toString().c_str());
		return iValue;
	}*/

private:
	//��ʾ����λԪ�Ľ��� @Bug 2����ʱ�ᱨ��
	const static int radix = 10000;
	//�����ű�ʶ ���ʱ���Ų����
	//char symbol = 0;
	//-1��ʾС��0; 0��ʾ����0; 1��ʾ����0(ͬJavaʵ��)
	int signum = 0;
	//��������λԪ ������(Ϊ��֧�����ֵ�ֱ�ӹ���������ȽϷ��� ���ַ�������Ӱ�첻��)
	std::vector<ByteType> digitLowTop;

	//���������������͵���10����λ�� 0: 1bit; 10: 2bit
	static int totalBitOf(ByteType originNumber) {
		// MathExtend::calcDigitTotalSize(originNumber, 10);
		int totalBit = 0;
		do {
			originNumber /= 10;
			++totalBit;
		} while (originNumber != 0);
		return totalBit;
	}

	//void setRadix(int r){radix = r;}

	//������������Ĵ���λ�� (grow:ֻ����(advance only) С��ԭλ��ʱ�����´�С)
	void setTotalByteNum(JCE::SizeType totalByteNum, bool grow = true) {
		//����֪�����ֵĴ��λ�� ������ǰָ�� �����ڴ����
		if (grow && totalByteNum <= digitLowTop.size())
			return;
		digitLowTop.resize(totalByteNum);
	}
	//��Դ����ת��Ϊ���ظ�ʽ����
	void transitionToLocalRadix(int originNumber) {
		int numTotalBit = totalBitOf(originNumber);
		//һ���ֵ�λ��: ����λ(��λ)�Ƚ��Ƶ���λ����һ
		int wordBit = totalBitOf(radix) - 1;
		//�����������޷������ִ���ʱ��Ҫ����һ����������(�������)
		setTotalByteNum(numTotalBit / wordBit + (numTotalBit % wordBit == 0 ? 0 : 1));
		/*symbol = */TransitionUtility::toAbs(originNumber);
		TransitionUtility::decimalToRadixLowTopBase(originNumber, digitLowTop.begin(), radix);
	}
	/*static BigInteger valueOf(int value) {
		return BigInteger(value);
	}*/
	//��Դ�ַ�����ʾ������ת��Ϊ���ظ�ʽ����
	void transitionToLocalRadix(std::string const &originNumberTopLow) {
		int numTotalBit = 0, wordBit = totalBitOf(radix) - 1;
		Utility::AssertToSignedNum(originNumberTopLow.length(), numTotalBit);
		//�ܴ���λ��(����)
		int storeTotalByteNum = numTotalBit / wordBit;
		//���λ
		int overflowingBit = numTotalBit % wordBit;
		storeTotalByteNum += overflowingBit == 0 ? 0 : 1;
		setTotalByteNum(storeTotalByteNum);

		int currentSub = 0;
		if (overflowingBit > 0) {
			digitLowTop[--storeTotalByteNum] = atoi(originNumberTopLow.substr(currentSub, overflowingBit).c_str());
			currentSub += overflowingBit;
		}
		while (storeTotalByteNum > 0) {
			digitLowTop[--storeTotalByteNum] = atoi(originNumberTopLow.substr(currentSub, wordBit).c_str());
			currentSub += wordBit;
		}
	}
	//С�ڷ���-1 ���ڷ���1 ���ڷ���0
	int compare(const BigInteger &rhs) const {
		int topBit1 = -1, topBit2 = -1, result = 0;
		Utility::AssertToSignedNum(digitLowTop.size(), topBit1);
		Utility::AssertToSignedNum(rhs.digitLowTop.size(), topBit2);
		/*if (signum != rhs.signum) {
			result = signum < rhs.signum ? -1 : 1;
		}
		else {*/
			if (topBit1 < topBit2) {
				result = -1;
			}
			else if (topBit1 > topBit2) {
				result = 1;
			}
			else {
				for (int i = topBit1 - 1; i >= 0; --i) {
					if (digitLowTop[i] < rhs.digitLowTop[i]) {
						result = -1;
						break;
					}
					else if (digitLowTop[i] > rhs.digitLowTop[i]) {
						result = 1;
						break;
					}
				}
				//result = 0;
			}
			// ������һ������
			//result *= signum;
		//}
		return result;
	}
	//�����ܵ�����
	int getTotalByteNum() const {
		int totalByteNum = -1;
		Utility::AssertToSignedNum(digitLowTop.size(), totalByteNum);
		return totalByteNum;
	}
	//this*=2
	void muity2() {
		this->plus(*this);
	}
	//���ش����ķ��� >= 0 û�з���
	char symbol() const {
		return signum == -1 ? '-' : signum == 1 ? '\0' : '\0';
	}
};

#endif

