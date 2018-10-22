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

	//'A' == 'a' == '0' ... 'J' == 'j' == '9' 以此类推
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
	// 三分法 求函数fun在[L, R]的最小值 (eps: 精度) 求最大值可通过fun外部实现
	double trichotomy(double L, double R, double eps, double(*fun)(double)) {
		double Ll, Rr;
		while (R - L > eps) {
			//三分
			Ll = (2 * L + R) / 3;
			Rr = (2 * R + L) / 3;
			if (fun(Ll) > fun(Rr))
				L = Ll;
			else
				R = Rr;
		}
		//返回任一个即可
		return fun(L);
	}

	//输出数字1-maxDigit(1, 9)的全排列  字典序
	void pentration(int maxDigit) {
		char allDigit[11] = "123456789";
		allDigit[maxDigit] = '\0';
		do {
			//求下一个排列数 #include<algorithm>
			puts(allDigit);
		} while (std::next_permutation(allDigit, allDigit + maxDigit));
		allDigit[maxDigit] = maxDigit + '0';
	}
	//输出数字1-maxDigit(1, 9)的全排列  非字典序
	void pentration() {
		std::string temp = "123456789";
		StandardExtend::ArrayList<char> allDigit(temp.size());
		//toArrayList<char, std::string::iterator>(temp.begin(), temp.end());
		std::copy(temp.begin(), temp.end(), allDigit.begin());
		MathExtend::penetration(allDigit, 0, 9 - 1, [&](auto container) {
			StandardExtend::outPutIterable(container.begin(), container.end());
		});
	}
	//汉诺塔递归解法
	void hannoTowerMove(int n, char a, char b, char c) {
		// hannoTowerMove(n,'A','B','C');
		if (n == 1) {
			//当n只有1个的时候直接从a移动到c
			printf("%c To %c\n", a, c);
		}
		else {
			//第n-1个要从a通过c移动到b
			hannoTowerMove(n - 1, a, c, b);
			printf("%c To %c\n", a, c);
			//n-1个移动过来之后b变开始盘，b通过a移动到c，这边很难理解
			hannoTowerMove(n - 1, b, a, c);
		}
	}
	//Confined(受限的)

	//将10进制的number转换为radix进制的字符串 (递归实现)
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
	//若大数bigInteger能被整除返回true
	bool isDivisible(char *bigInteger, int MOD) {
		int len = strlen(bigInteger);
		int ans, i;
		for (ans = i = 0; i < len; i++) {
			ans = (ans * 10 + (bigInteger[i] - '0')) % MOD;
		}
		return ans == 0 ? true : false;
	}
	//删除resStr中的所有子串delSubStr
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
	//逆序数
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
	//计算radix进制整数number中digit出现的次数
	int calcDigitCountInNumber(const int number, const int digit, int radix) {
		int temp = number;
		int count = 0;
		do {
			count += ((temp % radix) > 0 ? (temp % radix) : -(temp % radix)) == digit ? 1 : 0;
			temp /= radix;
		} while (temp != 0);
		return count;
	}
	//计算radix进制整数的位数
	int calcDigitTop(int number, int radix) {
		int top = 1;
		while (number /= radix)top++;
		return top;
	}




	I64 Quickfact(I64 a, I64 b, I64 mod) {
		I64 ans = 0;
		while (b) {
			if (b & 1) {
				ans = (ans + a) % mod;
			}
			a = (a << 1) % mod;
			b >>= 1;
		}
		return ans;
	}
	I64 Quickpow(I64 C, I64 R, I64 k) {
		I64 ans = 1;
		while (R > 0) {
			R & 1 ? ans = Quickfact(ans, C, k) : ans;
			C = Quickfact(C, C, k);
			R >>= 1;
		}
		return ans;
	}
	//快速幂简易版 m^n % k（k）
	I64 quickPow(I64 m, I64 n, I64 k) {
		I64 ans = 1;
		while (n > 0) {
			n & 1 ? ans = ((ans%k)*(m%k)) % k : ans;
			m = ((m%k)*(m%k)) % k;
			n >>= 1;
		}
		return ans;
	}
	I64 quickPow(I64 m, I64 n) {
		I64 ans = 1;
		while (n > 0) {
			//n & 1 ? ans = ans*m : ans;
			if (n & 1)
				ans *= m;
			m *= m;
			n >>= 1;
		}
		return ans;
	}
	//任意底数对数
	double logR(double value, double base) {
		return log(value) / log(base);
	}
	//阶 乘
	double fact(int n) {
		double product = n;
		while (--n > 0)product *= n;
		//0,1的阶乘为1(此时正好用p判断)
		return product == 0 ? 1 : product;
	}
	//int 限度内阶乘表 (int 13!爆； double和long long int 18!爆)
	int* factTable(int maxN) {
		int *Fact = (int *)malloc(sizeof(int)* maxN);
		Fact[0] = Fact[1] = 1;
		for (int n = 2; n < maxN; ++n)
			Fact[n] = Fact[n - 1] * n;
		return Fact;
	}
	/*
	n < m && m != 0
		 fact(n)
	 ________________
	fact(n - m) * fact(m)
	*/
	//组合数
	int C(int n, int m) {
		int f = 1, i = 1;
		//n < m时c(n,m) = 0
		f = n - m < 0 ? 0 : f;
		//int 防爆n30 m29时   c(30, 29) = c(30, 1)
		m = n - m < m ? n - m : m;
		while (i <= m) {
			//n * (n - 1) * ...m
			f *= n--;
			//1 *    2    * ...m
			f /= i++;
		}
		return f;
	}
	//排列数
	int A(int n, int m) {
		int d, f = 1;
		for (d = n - m; d < n && d >= 0; --n) {
			f *= n;
		}
		return f;
	}
	//素数(质数)判断
	int isPrime_OLD(int num) {
		double k = sqrt((double)num);
		int i = 0;
		for (i = 2; i <= k && num % i != 0; i++);
		return i > k && num > 1 ? true : false;
	}
	//素数(质数)判断 sqrt的平方优化版(未实测)
	bool isPrime(int num) {
		int i = 0;
		for (i = 2; i*i <= num && num % i != 0; i++);
		return i * i > num && num > 1;
	}
	//真因子和(除本身外的约数）
	int factorSum(int x) {
		//1对应x  只加1不加x
		int sum = 1, i;
		_DEBUG_ERROR(x != 0, "0没有因子");
		//i可能爆
		for (i = 2; i * i <= x; ++i) {
			if (x % i == 0) {
				sum += i;
				//根号x只加一次
				sum += i == x / i ? 0 : x / i;
			}
		}
		//（因子包括1但不包括本身，特别的0没有因子，1的因子为1）；另外因子和为本身的叫完数
		return sum;
	}
	//真因子和表(约数 因数) (10^7一千万2.51s)(400w 10^6 900ms)
	void factorSumTableSieve(const int maxn, int a[]) {
		for (int i = 1; i < maxn; i++) {
			for (int j = i + i; j < maxn; j += i) {
				a[j] += i;
			}
		}
	}
	//素数(质数) 筛选法 埃拉托色尼(Sieve Eratosthenes)(0 1==-1, a[i]==0表示素数) maxN[2, 1e9) PS:maxN = 1e7时超过1s了 1e9似乎无法分配内存
	int* primeSieve(const int maxN) {
		int *prime = (int*)malloc(maxN * sizeof(int));
		memset(prime, 0, maxN * sizeof(int));
		prime[0] = prime[1] = -1;
		for (int i = 2; i < maxN; i++) {
			for (int j = i + i; j < maxN; j += i) {
				prime[j] += i;
			}
		}
		return prime;
	}
	//因子数目
	int factorCount(int x) {
		int i, count = 0;
		//1 对应 x 计2   k对应k 计1
		double k = sqrt((double)x);
		for (i = 1; i <= k; i++) {
			if (x % i == 0) {
				//除因子k外   其余因子成对出现
				count += i * i == x ? 1 : 2;
			}
		}
		return count;
	}
	//开方函数 (eps: 精度)
	double sqrtImpl(double x, double eps) {
		double last, next;
		for (last = 1, next = 2;;) {
			last = next;
			next = (last + (x / last)) / 2;
			if (fabs(next - last) < eps) {
				//????
				return next;
			}
		}
		return -1;
	}
	//求方差
	double variance(float x[], int n) {
		double aver, sum = 0, D = 0;
		int temp, i;
		temp = n;
		while (temp-- != 0) {
			sum += x[temp];
		}
		aver = sum / n;
		for (i = 0; i < n; i++) {
			D += (x[i] - aver) * (x[i] - aver) / n;
		}
		return D;
	}

}

