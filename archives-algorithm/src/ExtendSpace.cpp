#include "ExtendSpace.h" 
namespace StandardExtend{
	/****************Ascll*******************/

	char toLowerAlph(char c){
		if ('0' <= c && c <= '9'){
			return 'a' + c - '0';
		}
		else{
			if ('A' <= c && c <= 'Z'){
				return 'a' + c - 'A';
			}
			else{
				return c;
			}
		}
	}

	int toIntNum(char alphOrCharNum){
		if ('0' <= alphOrCharNum && alphOrCharNum <= '9'){
			return alphOrCharNum - '0';
		}
		else{
			if ('A' <= alphOrCharNum && alphOrCharNum <= 'Z'){
				return 10 + alphOrCharNum - 'A';
			}
			else{
				return 10 + alphOrCharNum - 'a';
			}
		}
	}

	//10ToA(Alph)
	char toUppercaseAscllChar(int num){
		return num > 9 ? num - 10 + 'A' : num + '0';
	}

	//'A' == 'a' == '0' ... 'J' == 'j' == '9' �Դ�����
	bool isAa0Equal(char a, char b){
		return toLowerAlph(a) == toLowerAlph(b);
	}

	void refreshStdin(char end) {
		while (getchar() != end);
	}
	double calcDifftime(time_t startTime) {
		time_t stopTime;
		time(&stopTime);
		double all = difftime(stopTime, startTime);
		return all / 3600;
	}
	double calcDiffClock(clock_t startClock) {
		clock_t stopClock = clock();
		return (double)(stopClock - startClock) / CLK_TCK;
	}
};

namespace MathExtend {
	// ���ַ� ����fun��[L, R]����Сֵ (eps: ����) �����ֵ��ͨ��fun�ⲿʵ��
	double trichotomy(double L, double R, double eps, double(*fun)(double)) {
		double Ll, Rr;
		while (R - L > eps) {
			//����
			Ll = (2 * L + R) / 3;
			Rr = (2 * R + L) / 3;
			if (fun(Ll) > fun(Rr))
				L = Ll;
			else
				R = Rr;
		}
		//������һ������
		return fun(L);
	}

	//�������1-maxDigit(1, 9)��ȫ����  �ֵ���
	void pentration(int maxDigit) {
		char allDigit[11] = "123456789";
		allDigit[maxDigit] = '\0';
		do {
			//����һ�������� #include<algorithm>
			puts(allDigit);
		} while (next_permutation(allDigit, allDigit + maxDigit));
		allDigit[maxDigit] = maxDigit + '0';
	}
	//�������1-maxDigit(1, 9)��ȫ����  ���ֵ���
	void pentration() {
		string temp = "123456789";
		ArrayList<char> allDigit(temp.size());
		//toArrayList<char, std::string::iterator>(temp.begin(), temp.end());
		std::copy(temp.begin(), temp.end(), allDigit.begin());
		penetration(allDigit, 0, 9 - 1, [&](auto container) {
			outPutIterable(container.begin(), container.end());
		});
	}
	//��ŵ���ݹ�ⷨ
	void hannoTowerMove(int n, char a, char b, char c) {
		// hannoTowerMove(n,'A','B','C');
		if (n == 1) {
			//��nֻ��1����ʱ��ֱ�Ӵ�a�ƶ���c
			printf("%c To %c\n", a, c);
		}
		else {
			//��n-1��Ҫ��aͨ��c�ƶ���b
			hannoTowerMove(n - 1, a, c, b);
			printf("%c To %c\n", a, c);
			//n-1���ƶ�����֮��b�俪ʼ�̣�bͨ��a�ƶ���c����ߺ������
			hannoTowerMove(n - 1, b, a, c);
		}
	}
	//Confined(���޵�)

	//��10���Ƶ�numberת��Ϊradix���Ƶ��ַ��� (�ݹ�ʵ��)
	std::string decToBin(int number, std::string &result, int radix) {
		if (number < radix) {
			return result += number;
			//printf("%d", number);
		}
		else {
			decToBin(number / radix, result, radix);
			return result += (number % radix);
			//printf("%d", number % radix);
		}
	}
	//������bigInteger�ܱ���������true
	bool isDivisible(char *bigInteger, int MOD) {
		int len = strlen(bigInteger);
		int ans, i;
		for (ans = i = 0; i < len; i++) {
			ans = (ans * 10 + (bigInteger[i] - '0')) % MOD;
		}
		return ans == 0 ? true : false;
	}
	//ɾ��resStr�е������Ӵ�delSubStr
	void eraseAllSubStr(char *resStr, char *delSubStr) {
		int i, res_len, del_len;
		char *p;
		del_len = strlen(delSubStr);
		res_len = strlen(resStr);

		for (p = strstr(resStr, delSubStr); p != NULL; p = strstr(resStr, delSubStr)) {
			for (i = p - resStr; i < res_len - del_len + 1; i++) {
				resStr[i] = resStr[i + del_len];
			}
			res_len = strlen(resStr);
		}
	}
	//������
	int reverseOrderNumber(int number) {
		int temp = number;
		int sum = 0, product = 1;
		int top = 0;
		int temptop;
		for (top = 0; temp != 0; temp /= 10, top++);
		//for (top = 1; temp /= 10; top++);
		temp = number;
		while (temp != 0) {
			temptop = top-- - 1;
			product = 1;
			while (temptop-- != 0) {
				product *= 10;
			}
			sum += (temp % 10) * product;
			temp /= 10;
		}
		return sum;
	}
	//����radix��������number��digit���ֵĴ���
	int calcDigitCountInNumber(const int number, const int digit, int radix) {
		int temp = number;
		int count = 0;
		do {
			count += ((temp % radix) > 0 ? (temp % radix) : -(temp % radix)) == digit ? 1 : 0;
			temp /= radix;
		} while (temp != 0);
		return count;
	}
	//����radix����������λ��
	int calcDigitTop(int number, int radix) {
		int top = 1;
		while (number /= radix)top++;
		return top;
	}
}

