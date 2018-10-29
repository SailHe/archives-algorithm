#ifndef _BIGINTEGER_H
#define _BIGINTEGER_H

#include "stdafx.h"
#include "ExtendSpace.h"

/*
����: 39h�Ƚ���ÿһλתΪ2����: 3->0011b, 9->1001b   ��0011 1001b Ȼ��תΪĿ����� 111 001b->71o
������ļ��ֽ��Ƶĵ�����ֵ�����ֵ
tBit:   1  2  3     4        5
input:  1  3  7   F(15)    V(31)
output: 1 11 111  1111     11111
*/

//������ת������(֧�� ����2���Ʊ�ʾ��������ƵĴ����ĵ���ת��) ������Ҫ�û��Լ�����
class BinaryTransition{
	//ת��˫���Ĵ���λ: һλ������Ҫ����λ�����Ʊ�ʾ
	int originBitLeast = 1;
	int originRadix = (int)pow((double)2, originBitLeast);
	int targetBitLeast = 1;
	/*
	//ֻ���һ�����Ϳ����滻
	typedef int *Varray;
	typedef Varray VarrayIterator;
	*/
	typedef std::vector<int> Varray;
	typedef Varray::iterator VarrayIterator;
	Varray repository;// = NULL
	VarrayIterator binNumberPointer;// = NULL
	VarrayIterator currentBinNumberPointer;// = NULL
public:
	//֧��[1, 31] ��2 4..��31�ֽ���(��ͷԼ��: ֻ�ܸ߽�����ͽ���ת) 1bit����ʾĿ�������2����
	BinaryTransition(int bitSize, int originBitLeast = 1, int targetBitLeast = 1){
		//repository = (int *)malloc(sizeof(int)*bitSize);
		repository.resize(bitSize);
		reset(originBitLeast, targetBitLeast);
	}
	//�������ת�� ���ܿռ䲻֧������
	void reset(int originBitLeast, int targetBitLeast = 1){
		this->originBitLeast = originBitLeast;
		this->targetBitLeast = targetBitLeast;
		//memset(repository, 0, targetBitLeast*sizeof(int));//Ԥ����һ�α����ʼ��
	}
	//�Զ�����Ϊ�м䴢��Ľ��ƴ���ת�� ans�������յĽ��(������ֶ����0)
	void transition(char *origin, std::vector<int> &ans){
		char oneBit = 0;
		binNumberPointer = repository.begin() + targetBitLeast;
		//binNumberPointer = repository + targetBitLeast;
		currentBinNumberPointer = binNumberPointer;//Ԥ��һ��Ŀ�괢��λ���ڲ���

		//top->lowÿ��Դbit��ת��Ϊ2����(����������Դ�������Ķ����Ʊ�ʾ)

		//while ((oneBit = getchar()) != '\n'){//�������������� ���������ⲿ�����ٶ���һ��char����(���������ܱȽϴ�)
		while ((oneBit = *(origin++)) != '\0'){
			int number = oneBit - '0';
			if (isalpha(oneBit)){
				number = oneBit - 'A' + 10;
			}

			if (number >= originRadix){
				//_DEBUG_ERROR("����ĵ���Bit���ݳ��������Ƶ�ģ.");
				//��4������2λ��ʾ ����4 0010 �޷��������ת�� ��ȻreTopBitʹ�ü�ʹ���Ҳ�����ת��, �������Ǵ��
			}

			int bit = tenToRadix(number, currentBinNumberPointer, 2);
			//number = baseLowTopToTen(currentBinNumberPointer, 2, bit);
			//�����
			complement(currentBinNumberPointer, currentBinNumberPointer + bit, originBitLeast);
			//ע��: ÿoriginBitLeastλ�����򴢴� �޷�ת��ΪÿtargetBitLeastλ�����򴢴� �����������Ǳ����
			int reTopBit = max(bit, originBitLeast);
			for (int i = 0; i < reTopBit / 2; i++){
				std::swap(currentBinNumberPointer[i], currentBinNumberPointer[reTopBit - i - 1]);
			}
			currentBinNumberPointer += reTopBit;
		}

		//int ansSub = 0;
		ans.reserve((currentBinNumberPointer - binNumberPointer) / targetBitLeast);
		//����Ӱ����һ��
		ans.resize(0);

		//��ǰ����(�ڸպõ������ ��ಹ ���������ж���ȥ��ǰ��0)
		binNumberPointer -= targetBitLeast - ((currentBinNumberPointer - binNumberPointer) % targetBitLeast);
		//������˷�0ֵ��Ϊtrue
		bool outputValidValue = false;
		//top->lowÿһ��targetBitLeastתΪ10����(����������Ŀ�����)
		for (int i = 0; i < currentBinNumberPointer - binNumberPointer; i += targetBitLeast){
			//ȥ�����е�ת�����ǰ��0(ת��ǰ��ǰ��0ת������ܲ���)
			int number = baseTopLowToTen(binNumberPointer + i, 2, targetBitLeast);
			outputValidValue = number != 0 || outputValidValue;
			if (outputValidValue){
				ans.push_back(number);
			}
			//printf(outputValidValue ? "%d" : "", number);
		}
		//puts("");
	}

	/*��������ת�� �����ֵ����int��Χֱ��ʹ��2���Ƶ��м䴢������������*/

	//������ת������(������������) ���ظ����ֵ���������
	static char toAbs(int &number){
		return number < 0 ? (number = -number), '-' : '+';
	}

	//����->���� (����λ�� �Լ� repositoryLowToTop��λ���λ�洢����ֵ���������)
	static int tenToRadix(unsigned number, VarrayIterator repositoryLowToTop, int radix){
		static int bit;
		bit = -1;
		//Ҫ��֤��ʹ����0Ҳ��ִ��һ��
		do{
			repositoryLowToTop[++bit] = number % radix;
			number /= radix;
		} while (number != 0);
		return bit + 1;
	}

	//�ַ�����->����λԪ���� ������λ��
	static size_t toDigitBitArray(char *originTopLow, Varray &numTopLow){
		size_t len = strlen(originTopLow);
		for (size_t i = 0; i < len; ++i){
			if (isalpha(originTopLow[i])){
				numTopLow[i] = toupper(originTopLow[i]) - 'A' + 10;
			}
			else if (isalnum(originTopLow[i])){
				numTopLow[i] = originTopLow[i] - '0';
			}
			else{
				//_DEBUG_ERROR("�������ݺ��Ƿ��ַ�(�Ϸ��ַ�: ����, ��Сд��ĸ)")
			}
		}
		return len;
	}


	/*(lowBitIter: ��λ���λ�ƽ��ĵ����� rbegain() )*/
	static int radixToTen(VarrayIterator lowBitIter, int radix, int totalSizeNum){
		static int number;
		int powNum = 1;
		number = 0;
		//�ӵ�λ���λ��Ȩչ�� i����
		for (int i = 0; i < totalSizeNum; ++i){
			number += lowBitIter[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//����������Ƶļ�Ȩ10������
	//(tenToRadix��������ֵ����ֱ���ô˷���ֱ�ӵĵ�10���Ƶ�ֵ)
	static int baseLowTopToTen(VarrayIterator numLowTop, int radix, int totalSizeNum){
		static int number;
		int powNum = 1;
		number = 0;
		//�ӵ�λ(����ǵ�λ)���λ��Ȩչ�� i����
		for (int i = 0; i < totalSizeNum; ++i){
			number += numLowTop[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//�洢��ʽ��top->low
	template<class BaseT>//֧�ֻ������������(ͨ��ָint __int64)
	static BaseT baseTopLowToTen(VarrayIterator numTopLow, BaseT radix, int totalSizeNum){
		static BaseT number;
		BaseT powNum = 1;
		number = 0;
		//�ӵ�λ(�Ҳ��ǵ�λ)���λ��Ȩչ�� i����
		for (int i = totalSizeNum - 1; i >= 0; --i){
			number += numTopLow[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}


	//����n�ı���λ���0 originP~currentP(n*0)
	static void complement(VarrayIterator originP, VarrayIterator currentP, int n){
		int residueBit = (currentP - originP) % n;
		if (residueBit != 0){
			residueBit = n - residueBit;
			while (residueBit-- > 0){
				currentP[residueBit] = 0;
			}
		}
	}

	//��ÿһλ������ȡ��
	static void reverseCode(VarrayIterator numBin, size_t totalSizeNum){
		for (size_t i = 0; i < totalSizeNum; ++i){
			*numBin = !*numBin;
			++numBin;
		}
	}

	//10�������Ͻ��ƻ��������� 10��������������� 10��ӦA
	static void outputWithSymbol(Varray &numLowTop, int totalSizeNum){
		for (int i = 0; i < totalSizeNum; ++i){
			int pSub = totalSizeNum - i - 1;
			if (numLowTop[pSub] > 9){
				printf("%c", numLowTop[pSub] - 10 + 'A');
			}
			else{
				printf("%d", numLowTop[pSub]);
			}
		}
		puts("");
	}
};


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
	//���ض�Ӧʵ���ַ��������ֵ���λ��
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
		printf(symbol == '-' ? "-" : "");
		printf("%d", digitLowTop[totalByteNum - 1]);
		for (int i = 1; i < totalByteNum; i++){
			printf("%04d", digitLowTop[totalByteNum - i - 1]);
		}
		puts("");
	}
	void print(char *outBuffer, JCE::SizeType bufferSize) const {
		int totalByteNum = getTotalByteNum();
		sprintf_s(outBuffer, bufferSize, symbol == '-' ? "-" : "");
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
		int size = calcTotalBitNum() + 1;
		static std::string result("\0", size);
		result.assign(size, '\0');
		print(&result[0], size);
		return result;
	}
	BigInteger(ByteType originNumber){
		transitionToLocalRadix(originNumber);
	}
	BigInteger(std::string const &originNumberTopLow){
		transitionToLocalRadix(originNumberTopLow);
	}

	//�˷�ǰ����ȷ���һ��
	void reserve(size_t newSize){
		digitLowTop.reserve(newSize);
	}

	// �ַ��������ӷ�
	static std::string bigPlush(std::string &topLowNumA, std::string &topLowNumB, std::string &topLowSum) {
		std::size_t lenA = topLowNumA.size(), lenB = topLowNumB.size(), lenAB;
		//��0��
		std::string temp;
		//��λ����, ���߸�λ0����
		if (lenA > lenB) {
			temp.resize(lenA - lenB, '0');
			topLowNumB = temp + topLowNumB;
			lenAB = lenA;
		}
		else {
			temp.resize(lenB - lenA, '0');
			topLowNumA = temp + topLowNumA;
			lenAB = lenB;
		}
		if (topLowSum.size() < lenA) {
			topLowSum.resize(lenA, '0');
		}
		int ia = -1, c = 0;
		//��ת������ǵ�λ
		int i = -1;
		for (Utility::toSignedNum(lenAB - 1, i); i >= 0; --i) {
			int sumBit = (topLowNumA[i] - '0') + (topLowNumB[i] - '0') + c;
			topLowSum[i] = sumBit % 10 + '0';
			c = sumBit / 10;
		}
		return c == 0 ? topLowSum : (topLowSum = "1" + topLowSum);
	}
	//ȥ��ǰ��0 ��ֵΪ0 ����"0"
	static std::string formatString(std::string const &num) {
		int sp = -1;
		for (auto it = num.begin(); it != num.end(); ++it) {
			++sp;
			if (*it != '0') {
				break;
			}
		}
		return sp == num.size() ? "0" : num.substr(sp);
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
	//�����˷� (���ڼӷ�) ����ֵ�Ͳ����������Լ�(����ֵ��Ϊ�˱����������� ����ļ�д��������ͬ)
	BigInteger &muity(BigInteger const &rhs) {
		BigInteger addValue(*this);
		for (BigInteger i = 1; i < rhs; ++i){
			plus(addValue);
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
	//���ٳ˷� (����λ <= 10000)
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
	//���ٽ׳� (���ڿ��ٳ˷� n<=10000)
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
	// ���ڿ��ٳ˷�
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


	/*������ƺ�û���ò���ʵ�ּ���  �ɽ�λ����(Ψһ���е�Ӧ����ģ�����)*/
	BigInteger operator-(BigInteger const &rhs){
		this->plus(rhs);
		return *this;
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
	//���� ����
	void minus();

private:
	//��ʾ����λԪ�Ľ��� @Bug 2����ʱ�ᱨ��
	const static int radix = 10000;
	//�����ű�ʶ ���ʱ���Ų����
	char symbol = 0;
	//��������λԪ ������(Ϊ��֧�����ֵ�ֱ�ӹ���������ȽϷ��� ���ַ�������Ӱ�첻��)
	std::vector<ByteType> digitLowTop;

	//���������������͵���10����λ�� 0: 1bit; 10: 2bit
	static int totalBitOf(ByteType originNumber) {
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
		symbol = BinaryTransition::toAbs(originNumber);
		BinaryTransition::tenToRadix(originNumber, digitLowTop.begin(), radix);
	}
	/*static BigInteger valueOf(int value) {
		return BigInteger(value);
	}*/
	//��Դ�ַ�����ʾ������ת��Ϊ���ظ�ʽ����
	void transitionToLocalRadix(std::string const &originNumberTopLow) {
		int numTotalBit = 0, wordBit = totalBitOf(radix) - 1;
		Utility::toSignedNum(originNumberTopLow.length(), numTotalBit);
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
	int compare(const BigInteger& rhs) const {
		int topBit1 = -1, topBit2 = -1;
		Utility::toSignedNum(digitLowTop.size(), topBit1);
		Utility::toSignedNum(rhs.digitLowTop.size(), topBit2);
		if (topBit1 < topBit2)
			return -1;
		else if (topBit1 > topBit2)
			return 1;
		else {
			for (int i = topBit1 - 1; i >= 0; --i) {
				if (digitLowTop[i] < rhs.digitLowTop[i])
					return -1;
				else if (digitLowTop[i] > rhs.digitLowTop[i])
					return 1;
			}
			return 0;
		}
	}
	//�����ܵ�����
	int getTotalByteNum() const {
		int totalByteNum = -1;
		Utility::toSignedNum(digitLowTop.size(), totalByteNum);
		return totalByteNum;
	}
};

#endif

