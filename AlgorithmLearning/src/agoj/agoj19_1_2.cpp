#include<stdio.h>
#include<string.h>
#include<string>
#include<cmath>
#include<vector>
#include<iostream>
#include<algorithm>
#include<queue>
#include<functional>//greater
#include<list>
#include<stack>
#include<map>
#include<iomanip>//cout���ֲ�����

#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)

// #include "ExtendSpace.h"

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

// ����ö��
enum DIRECTION_ENUM {
	UN_KNOEN_DIR, UP_DIR, DOWM_DIR, LEFT_DIR, RIGHT_DIR, LEFT_UP_DIR, RIGHT_UP_DIR
};

// ���ݿ����ö��
enum OPTION_CRUD_ENUM {
	// ����, ����, ����, ɾ��, δ����
	// insert/save, query/find/search, edit/change, remove/erase
	CREATE_OPT, RETRIEVE_OPT, UPDATE_OPT, DELETE_OPT, UN_DEFINDE_OPT
};

// ���ݲ���ö��
enum OPTION_DATA_ENUM {
	// ���ֲ���, �滻, ɾ��, ����, δ���� (���ȼ����Ⱥ����� ��ǰ���Խ��)
	KEEP_OPT, REPLACE_OPT, DELETE_OPT, INSERT_OPT, UN_DEFINDE_OPT
};

//������ת������(֧�� ����2���Ʊ�ʾ��������ƵĴ����ĵ���ת��) ������Ҫ�û��Լ�����
class BinaryTransition {
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
	BinaryTransition(int bitSize, int originBitLeast = 1, int targetBitLeast = 1) {
		//repository = (int *)malloc(sizeof(int)*bitSize);
		repository.resize(bitSize);
		reset(originBitLeast, targetBitLeast);
	}
	//�������ת�� ���ܿռ䲻֧������
	void reset(int originBitLeast, int targetBitLeast = 1) {
		this->originBitLeast = originBitLeast;
		this->targetBitLeast = targetBitLeast;
		//memset(repository, 0, targetBitLeast*sizeof(int));//Ԥ����һ�α����ʼ��
	}
	//�Զ�����Ϊ�м䴢��Ľ��ƴ���ת�� ans�������յĽ��(������ֶ����0)
	void transition(char *origin, std::vector<int> &ans) {
		char oneBit = 0;
		binNumberPointer = repository.begin() + targetBitLeast;
		//binNumberPointer = repository + targetBitLeast;
		currentBinNumberPointer = binNumberPointer;//Ԥ��һ��Ŀ�괢��λ���ڲ���

		//top->lowÿ��Դbit��ת��Ϊ2����(����������Դ�������Ķ����Ʊ�ʾ)

		//while ((oneBit = getchar()) != '\n'){//�������������� ���������ⲿ�����ٶ���һ��char����(���������ܱȽϴ�)
		while ((oneBit = *(origin++)) != '\0') {
			int number = oneBit - '0';
			if (isalpha(oneBit)) {
				number = oneBit - 'A' + 10;
			}

			if (number >= originRadix) {
				//_DEBUG_ERROR("����ĵ���Bit���ݳ��������Ƶ�ģ.");
				//��4������2λ��ʾ ����4 0010 �޷��������ת�� ��ȻreTopBitʹ�ü�ʹ���Ҳ�����ת��, �������Ǵ��
			}

			int bit = tenToRadix(number, currentBinNumberPointer, 2);
			//number = baseLowTopToTen(currentBinNumberPointer, 2, bit);
			//�����
			complement(currentBinNumberPointer, currentBinNumberPointer + bit, originBitLeast);
			//ע��: ÿoriginBitLeastλ�����򴢴� �޷�ת��ΪÿtargetBitLeastλ�����򴢴� �����������Ǳ����
			int reTopBit = max(bit, originBitLeast);
			for (int i = 0; i < reTopBit / 2; i++) {
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
		for (int i = 0; i < currentBinNumberPointer - binNumberPointer; i += targetBitLeast) {
			//ȥ�����е�ת�����ǰ��0(ת��ǰ��ǰ��0ת������ܲ���)
			int number = baseTopLowToTen(binNumberPointer + i, 2, targetBitLeast);
			outputValidValue = number != 0 || outputValidValue;
			if (outputValidValue) {
				ans.push_back(number);
			}
			//printf(outputValidValue ? "%d" : "", number);
		}
		//puts("");
	}

	/*��������ת�� �����ֵ����int��Χֱ��ʹ��2���Ƶ��м䴢������������*/

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

	//�ַ�����->����λԪ���� ������λ��
	static size_t toDigitBitArray(char *originTopLow, Varray &numTopLow) {
		size_t len = strlen(originTopLow);
		for (size_t i = 0; i < len; ++i) {
			if (isalpha(originTopLow[i])) {
				numTopLow[i] = toupper(originTopLow[i]) - 'A' + 10;
			}
			else if (isalnum(originTopLow[i])) {
				numTopLow[i] = originTopLow[i] - '0';
			}
			else {
				//_DEBUG_ERROR("�������ݺ��Ƿ��ַ�(�Ϸ��ַ�: ����, ��Сд��ĸ)")
			}
		}
		return len;
	}


	/*(lowBitIter: ��λ���λ�ƽ��ĵ����� rbegain() )*/
	static int radixToTen(VarrayIterator lowBitIter, int radix, int totalSizeNum) {
		static int number;
		int powNum = 1;
		number = 0;
		//�ӵ�λ���λ��Ȩչ�� i����
		for (int i = 0; i < totalSizeNum; ++i) {
			number += lowBitIter[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//����������Ƶļ�Ȩ10������
	//(tenToRadix��������ֵ����ֱ���ô˷���ֱ�ӵĵ�10���Ƶ�ֵ)
	static int baseLowTopToTen(VarrayIterator numLowTop, int radix, int totalSizeNum) {
		static int number;
		int powNum = 1;
		number = 0;
		//�ӵ�λ(����ǵ�λ)���λ��Ȩչ�� i����
		for (int i = 0; i < totalSizeNum; ++i) {
			number += numLowTop[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}
	//�洢��ʽ��top->low
	template<class BaseT>//֧�ֻ������������(ͨ��ָint __int64)
	static BaseT baseTopLowToTen(VarrayIterator numTopLow, BaseT radix, int totalSizeNum) {
		static BaseT number;
		BaseT powNum = 1;
		number = 0;
		//�ӵ�λ(�Ҳ��ǵ�λ)���λ��Ȩչ�� i����
		for (int i = totalSizeNum - 1; i >= 0; --i) {
			number += numTopLow[i] * powNum;
			//powNum = radix^i
			powNum *= radix;
		}
		return number;
	}


	//����n�ı���λ���0 originP~currentP(n*0)
	static void complement(VarrayIterator originP, VarrayIterator currentP, int n) {
		int residueBit = (currentP - originP) % n;
		if (residueBit != 0) {
			residueBit = n - residueBit;
			while (residueBit-- > 0) {
				currentP[residueBit] = 0;
			}
		}
	}

	//��ÿһλ������ȡ��
	static void reverseCode(VarrayIterator numBin, size_t totalSizeNum) {
		for (size_t i = 0; i < totalSizeNum; ++i) {
			*numBin = !*numBin;
			++numBin;
		}
	}

	//10�������Ͻ��ƻ��������� 10��������������� 10��ӦA
	static void outputWithSymbol(Varray &numLowTop, int totalSizeNum) {
		for (int i = 0; i < totalSizeNum; ++i) {
			int pSub = totalSizeNum - i - 1;
			if (numLowTop[pSub] > 9) {
				printf("%c", numLowTop[pSub] - 10 + 'A');
			}
			else {
				printf("%d", numLowTop[pSub]);
			}
		}
		puts("");
	}
};

// �������֮�以��ת��(ʹ���˶�����ת������: BinaryTransition) ����10����
// @see http://139.196.145.92/contest_show.php?cid=720#problem/B
/*
input
2 9
1100

16 7
4CD

10 36
46

output:
13
3404
1A
*/
int mainForSolveB() {
	int originBase, targetBase;
	string origin, target;
	origin.resize(20);
	target.resize(20);
	vector<int> digitTopLow;
	digitTopLow.resize(20);
	while (cin >> originBase >> targetBase) {
		BinaryTransition bt(20, originBase, targetBase);
		scanf("%s", &origin[0]);
		// bt.transition(&origin[0], digitTopLow);
		// ���APIҲ̫�����˰� 30min
		int totalBit = BinaryTransition::toDigitBitArray(&origin[0], digitTopLow);
		int tenValue = BinaryTransition::baseTopLowToTen(digitTopLow.begin(), originBase, totalBit);
		reverse(digitTopLow.begin(), digitTopLow.end());
		totalBit = BinaryTransition::tenToRadix(tenValue, digitTopLow.begin(), targetBase);
		BinaryTransition::outputWithSymbol(digitTopLow, totalBit);
		// for (int i = totalBit - 1; i >= 0; --i) {
		// 	printf("%d", digitTopLow[i]);
		// }
		//cout << endl;
	}
	return 0;
}

// 10 -> radix
int mainForSolveC() {
	int originValue, targetBase;
	string origin, target;
	origin.resize(20);
	target.resize(20);
	vector<int> digitTopLow;
	digitTopLow.resize(20);
	while (cin >> originValue >> targetBase) {
		int tenValue = originValue;
		int totalBit = BinaryTransition::tenToRadix(tenValue, digitTopLow.begin(), targetBase);
		BinaryTransition::outputWithSymbol(digitTopLow, totalBit);
	}
	return 0;
}

// ɥ�Ĳ��� radix=[2, 62] char={[0,9], [A, Z], [a, z]}
int mainForSolveD() {
	int originBase, targetBase;
	string origin, target;
	origin.resize(20);
	target.resize(20);
	vector<int> digitTopLow, digitLowTop;
	digitTopLow.resize(20);
	digitLowTop.resize(20);
	while (cin >> originBase >> targetBase) {
		BinaryTransition bt(1000+5);// , originBase, targetBase
		scanf("%s", &origin[0]);
		bt.transition(&origin[0], digitTopLow);
		int tv = BinaryTransition::baseTopLowToTen(digitTopLow.begin(), 2, digitTopLow.size());
		printf("10->%d\n", tv);
		BinaryTransition::tenToRadix(tv, digitLowTop.begin(), targetBase);
		reverse(digitLowTop.begin(), digitLowTop.end());
		BinaryTransition::outputWithSymbol(digitLowTop, digitLowTop.size());
		// // ���APIҲ̫�����˰� 30min
		// int totalBit = BinaryTransition::toDigitBitArray(&origin[0], digitTopLow);
		// int tenValue = BinaryTransition::baseTopLowToTen(digitTopLow.begin(), originBase, totalBit);
		// reverse(digitTopLow.begin(), digitTopLow.end());
		// totalBit = BinaryTransition::tenToRadix(tenValue, digitTopLow.begin(), targetBase);
		// BinaryTransition::outputWithSymbol(digitTopLow, totalBit);
	}
	return 0;
}

int mainForSolveA() {
	int caseCnt;
	scanf("%d", &caseCnt);
	while (caseCnt-- > 0) {
		string lhs, rhs, equalitySeq;
		cin >> lhs >> rhs;
		lhs = " " + lhs;
		rhs = " " + rhs;
		ArrayList<ArrayList<int>> dis2d(lhs.length(), ArrayList<int>(rhs.length(), 0));
		ArrayList<ArrayList<DIRECTION_ENUM>> path2d(lhs.length(), ArrayList<DIRECTION_ENUM>(rhs.length(), UN_KNOEN_DIR));
		// ��0�к͵�0����Ҫ��ʼ��Ϊ0
		for (SizeType r = 1; r < lhs.length(); ++r) {

		}

		// ����lhs��� ����rhs���
		for (SizeType r = 1; r < lhs.length(); ++r) {
			for (SizeType c = 1; c < rhs.length(); ++c) {
				if (lhs[r] == rhs[c]) {
					//equalitySeq += lhs;
					dis2d[r][c] = dis2d[r - 1][c - 1] + 1;
					path2d[r][c] = LEFT_UP_DIR;
				}
				else {
					if (dis2d[r - 1][c] >= dis2d[r][c - 1]) {
						dis2d[r][c] = dis2d[r - 1][c];
						path2d[r][c] = UP_DIR;
					}
					else {
						dis2d[r][c] = dis2d[r][c - 1];
						path2d[r][c] = LEFT_DIR;
					}
				}
			}
		}
		cout << equalitySeq.length() << endl;
	}
	return 0;
}


// itoa
int main() {
	//mainForSolveB();
	//mainForSolveC();
	//mainForSolveD();
	mainForSolveA();
	return 0;
}
