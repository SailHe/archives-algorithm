#ifndef _BIGINTEGER_H
#define _BIGINTEGER_H

#include"stdafx.h"

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
	static int radixToTen(VarrayIterator lowBitIter, int radix, int totalBit){
		static int number;
		int powNum = 1;
		number = 0;
		//�ӵ�λ���λ��Ȩչ�� i����
		for (int i = 0; i < totalBit; ++i){
			number += lowBitIter[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//����������Ƶļ�Ȩ10������
	//(tenToRadix��������ֵ����ֱ���ô˷���ֱ�ӵĵ�10���Ƶ�ֵ)
	static int baseLowTopToTen(VarrayIterator numLowTop, int radix, int totalBit){
		static int number;
		int powNum = 1;
		number = 0;
		//�ӵ�λ(����ǵ�λ)���λ��Ȩչ�� i����
		for (int i = 0; i < totalBit; ++i){
			number += numLowTop[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//�洢��ʽ��top->low
	template<class BaseT>//֧�ֻ������������(ͨ��ָint __int64)
	static BaseT baseTopLowToTen(VarrayIterator numTopLow, BaseT radix, int totalBit){
		static BaseT number;
		BaseT powNum = 1;
		number = 0;
		//�ӵ�λ(�Ҳ��ǵ�λ)���λ��Ȩչ�� i����
		for (int i = totalBit - 1; i >= 0; --i){
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
	static void reverseCode(VarrayIterator numBin, size_t totalBit){
		for (size_t i = 0; i < totalBit; ++i){
			*numBin = !*numBin;
			++numBin;
		}
	}

	//10�������Ͻ��ƻ��������� 10��������������� 10��ӦA
	static void outputWithSymbol(Varray &numLowTop, int totalBit){
		for (int i = 0; i < totalBit; ++i){
			int pSub = totalBit - i - 1;
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


/*��֧�ֲ�����λ, ��ŵĸ�������*/
class BigInteger{
	//��ʾ����λԪ�Ľ���
	//@Bug 2����ʱ�ᱨ��
	const static int radix = 10000;
	const static int ONE = 1;
	const static int ZERO = 0;
	//����λԪ������(�����ܹ����������������ֵ��˵�ֵ)
	typedef int BitType;
	typedef std::vector<BitType> Base;
	//�����ű�ʶ ���ʱ���Ų����
	char symbol = 0;
	//��������λԪ ������(Ϊ��֧�����ֵ�ֱ�ӹ���������ȽϷ��� ���ַ�������Ӱ�첻��)
	std::vector<BitType> digitLowTop;

	//���������������͵���λ�� 0: 1bit; 10: 2bit
	static int totalBitOf(BitType originNumber){
		int totalBit = 0;
		do{
			originNumber /= 10;
			++totalBit;
		}while (originNumber != 0);
		return totalBit;
	}

	//void setRadix(int r){radix = r;}

	//������������Ĵ���λ�� (grow:ֻ����(advance only) С��ԭλ��ʱ�����´�С)
	void setTotalBit(size_t totalBit, bool grow = true){
		//����֪�����ֵĴ��λ�� ������ǰָ�� �����ڴ����
		if (grow && totalBit <= digitLowTop.size())
			return;
		digitLowTop.resize(totalBit);
	}
	//��Դ����ת��Ϊ���ظ�ʽ����
	void transitionToLocalRadix(int originNumber){
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
	//��Դ�ַ�����ʾ������ת��Ϊ���ظ�ʽ����
	void transitionToLocalRadix(std::string const &originNumberTopLow){
		int numTotalBit = originNumberTopLow.length(), wordBit = totalBitOf(radix) - 1;
		//�ܴ���λ
		int storeTotalBit = numTotalBit / wordBit;
		//���λ
		int overflowingBit = numTotalBit % wordBit;
		storeTotalBit += overflowingBit == 0 ? 0 : 1;
		setTotalBit(storeTotalBit);

		int currentSub = 0;
		if (overflowingBit > 0){
			digitLowTop[--storeTotalBit] = atoi(originNumberTopLow.substr(currentSub, overflowingBit).c_str());
			currentSub += overflowingBit;
		}
		while (storeTotalBit > 0){
			digitLowTop[--storeTotalBit] = atoi(originNumberTopLow.substr(currentSub, wordBit).c_str());
			currentSub += wordBit;
		}
	}
	//С�ڷ���-1 ���ڷ���1 ���ڷ���0
	int compare(const BigInteger& rhs) const{
		size_t topBit1 = digitLowTop.size(), topBit2 = rhs.digitLowTop.size();
		if (topBit1 < topBit2)
			return -1;
		else if (topBit1 > topBit2)
			return 1;
		else{
			for (int i = topBit1 - 1; i >= 0; i--){
				if (digitLowTop[i] < rhs.digitLowTop[i])
					return -1;
				else if (digitLowTop[i] > rhs.digitLowTop[i])
					return 1;
			}
			return 0;
		}
	}
public:
	//����ʵ��λ��
	int getTotalBit(){
		//����*һ�ֵ�λ��(����1?)
		return digitLowTop.size()*totalBitOf(radix);
	}
	void print(){
		const char map[17] = "0123456789ABCDEF";
		int totalBit = digitLowTop.size();
		printf(symbol == '-' ? "-" : "");
		printf("%d", digitLowTop[totalBit - 1]);
		for (int i = 1; i < totalBit; i++){
			printf("%04d", digitLowTop[totalBit - i - 1]);
		}
		puts("");
	}
	void print(char *out){
		int totalBit = digitLowTop.size();
		sprintf(out, symbol == '-' ? "-" : "");
		sprintf(out, "%d", digitLowTop[totalBit - 1]);
		out += totalBitOf(digitLowTop[totalBit - 1]);
		for (int i = 1; i < totalBit; i++){
			sprintf(out, "%04d", digitLowTop[totalBit - i - 1]);
			out += totalBitOf(radix)-1;
		}
		out[0] = '\0';
	}

	BigInteger(BitType originNumber){
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
		for (int i = lenAB - 1; i >= 0; --i) {
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
		BitType subLhs, subRhs, subSum;
		//���ֱ߶�Ӧλ��ֵ ��λ addValue(����) = rhsBit + carryBit
		BitType rhsBit, carryBit = 0;
		//����
		int wordCntLhs = digitLowTop.size(), wordCntRhs = rhs.digitLowTop.size();

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
	//�����˷� �ӷ�ʵ��
	BigInteger &muity(BigInteger const &rhs){
		BigInteger addValue(*this);
		for (BigInteger i = 1; i < rhs; ++i){
			plus(addValue);
		}
		return *this;
	}
	//���ٳ˷� (����λ <= 10000) ����push_back�Ĵ��ڲ����Ż��ض��Ȳ���ֱ��д���㷨 �����Ż�����Ҫ��һ�� �����ڴ��๦����ȫ
	BigInteger &muity(BitType muityBit){
		//int units = 1������λ: ��ʾmuityBit��һ��λ������
		BitType carryBit = 0;
		BitType subProduct;
		int totalBit = digitLowTop.size();
		for (subProduct = 0; subProduct < totalBit || carryBit; subProduct++){
			if (subProduct < totalBit)
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
	BigInteger &fact(int n){
		++n;
		while (--n > 0){
			muity(n);
		}
		return *this;
	}
	//�����׳� (��ֵ<=10000ʱ����ʹ�ÿ��ٳ˷����ⲿ���� ������ʵ������㷨�ڼ�ʹ�����Ż��������1000!�Ѿ���Ҫ��Լ1s��)
	BigInteger &fact(){
		//Ϊ�˲�ʹ�ü��� ֻ����ô���� ʹ�ü����Ļ�����������Բ��ö���
		BigInteger product = 1;
		if (*this > ZERO){
			++*this;
			for (BigInteger i(1); i < *this; ++i){
				product.muity(i);
			}
			*this = product;
		}
		else if(*this == ZERO){
			//0�Ľ׳�Ϊ1
			(*this) = ONE;
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
	BigInteger operator+(BigInteger const &rhs){
		return BigInteger(*this).plus(rhs);
	}
	// ���ڼӷ��ĳ˷�
	BigInteger operator*(BigInteger const &rhs){
		/// �˷�(û��ֱ�� muity Ϊ����һ����ʱ�����ǵ���ʵ�ֵ�)
		BigInteger addValue(*this);
		for (BigInteger i = 1; i < rhs; ++i){
			addValue.plus(*this);
		}
		return addValue;
	}
	// ���ڷǴ������Ҳ������Ŀ��ٳ˷�
	BigInteger operator*(BitType const &muityBit){
		BigInteger result = this->muity(muityBit);
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
	void minus();//���� ����
};

/*

*/

#endif

/*
100
93326215443944152681699238856266700490715968264381621468592963
895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000
500
12201368259911100687012387854230469262535743428031928421924
1023
5291532027401227815504806586605326892579642542517591254377802998714072863352906839
2300
1502[0]4882700144063261010037638111251819477735600118435480095
5��0���һ������5
10000
28462596809170545189[0]64132121198688901480514017027992307941
79994274411340003764443772990786757784775815884062142317528830042339940153518739052421161382716
*/