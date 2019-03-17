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
#include<iomanip>//cout各种操纵器

#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

//#define MAX_N 1000+5
#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>
using namespace std;

/**/
bool isValidCarIdChar(char c) {
	return c != 'O' && c != 'Q' && c != 'I' && c != 'B';
}

char toAlphOrAscllNum(int num) {
	return num < 10 ? num + '0'
		: num < 36 ? num - 10 + 'A'
		: num - 36 + 'a';
}

int toIntNum(char alphOrAscllNum) {
	int resultInteger = -1;
	// isalnum(); inRange();
	if ('0' <= alphOrAscllNum && alphOrAscllNum <= '9') {
		// 0 + alphOrAscllNum - '0';
		resultInteger = alphOrAscllNum - '0';
	}
	else {
		// isalpha();
		if ('A' <= alphOrAscllNum && alphOrAscllNum <= 'Z') {
			resultInteger = 10 + alphOrAscllNum - 'A';
		}
		else if ('a' <= alphOrAscllNum && alphOrAscllNum <= 'z') {
			// 数字字符10个{0, 9} 大写字母26个{A, Z} 小写字母26个{a, z}
			resultInteger = 36 + alphOrAscllNum - 'a';
		}
		else {
			_ASSERT_EXPR(false, "字符非法! ->合法字符{数字, 大小写字母}");
		}
	}
	return resultInteger;
}

// Left closed right open(左闭右开)
// left <= value < right
// EG: inRange(minEle, element, maxEle+1); <==> element=[minEle, maxEle]
template<class T>
bool inRange(T left, T value, T right) {
	return left <= value && value < right;
}


char mapping(int digit) {
	char cNum = toAlphOrAscllNum(digit);
	// 非法字符就向后映射
	return isValidCarIdChar(cNum) ? cNum : mapping(digit + 1);
}

int toRadixIntNum(char alphOrAscllNum, int radix) {
	int digitNum = toIntNum(alphOrAscllNum);
	// American Standard Code for Information Interchange，美国信息交换标准代码
	//_ASSERT_EXPR(inRange(0, digitNum, radix), "不是指定进制的Asll字符");
	return digitNum;
}

// 字符容器{数字, 大小写字母(区分)} -> 指定进制的数字容器
	// 返回digitIterEnd
template<class CharContainerIterator, class DigitContainerIterator>
DigitContainerIterator charContainerToDigitContainer(CharContainerIterator charIterBegin, CharContainerIterator charIterEnd, DigitContainerIterator digitIterBegin, int radix) {
	std::for_each(charIterBegin, charIterEnd, [&](char value) {
		*digitIterBegin = toRadixIntNum(value, radix);
		++digitIterBegin;
	});
	return digitIterBegin;
}

// ---- Recommend  realBeginIter
template<class DigitIterator>
DigitIterator decimalToRadixTopLow(unsigned decimalNum, DigitIterator rightIter, int radix) {
	// 要保证即使传入0也能执行一次
	do {
		*--rightIter = decimalNum % radix;
		decimalNum /= radix;
	} while (decimalNum != 0);
	return rightIter;
}

// ---- Recommend
template<class Integer, class DigitIterator>
Integer radixTopLowToDecimal(DigitIterator leftIter, DigitIterator rightIter, Integer radix, std::vector<int> const &assicAlphMapNum) {
	static Integer decimalNum;
	Integer powNum = 1;
	decimalNum = 0;
	//从低位(右侧是低位)向高位按权展开 i是幂
	while (leftIter != rightIter) {
		// decimalNum += *--rightIter * powNum;
		decimalNum += assicAlphMapNum[*--rightIter] * powNum;
		powNum *= radix;
	}
	return decimalNum;
}

int main() {

	// 5位32进制的最大值为32^5 - 1
	const int originRadix = 32, targetRadix = 32, limitValue = (int)pow(32, 5);
	int disValue = 0;
	string origin, target;
	origin.resize(20);
	vector<int> digitTopLow;
	digitTopLow.resize(20);
	std::vector<char> digitMapAlphNum(36);
	std::vector<int> alphNumMapDigit(255);
	for (int i = 0, sub = -1; i < 36; ++i) {
		char c = toAlphOrAscllNum(i);
		if (isValidCarIdChar(c)) {
			digitMapAlphNum[++sub] = c;
			alphNumMapDigit[toIntNum(c)] = sub;
		}
	}
	while (cin >> origin >> disValue) {
		digitTopLow.assign(digitTopLow.size(), 0);

		auto digitItEnd = charContainerToDigitContainer(origin.begin(), origin.end(), digitTopLow.begin(), originRadix);
		// 0000M->20 12345->1117317
		int decValue = radixTopLowToDecimal(digitTopLow.begin(), digitItEnd, originRadix, alphNumMapDigit);
		
		disValue %= limitValue;
		decValue += disValue;
		decValue %= limitValue;

		auto topLowLeftIt = decimalToRadixTopLow(decValue, digitTopLow.end(), targetRadix);

		target = "";
		for (; topLowLeftIt != digitTopLow.end(); ++topLowLeftIt) {
			// cout << *topLowLeftIt << " ";
			target += digitMapAlphNum[*topLowLeftIt];
		}
		
		cout << setw(5) << setfill('0') << target << endl;
	}
	return 0;
}

/*
http://139.196.145.92/contest_show.php?cid=856#problem/A
题目数据:
12345 -5
12345 8

00005 -5 0000M  1 0000P -1 0000P  1 0000Z  1 ZZZZZ  1

0     6 7     13
A-CDEFG H-JKLMN
 16  20   23  25
-P- RST UVW XYZ

测试数据(test.in):
00005 -5
0000M  1
0000P -1
0000P  1
0000Z  1
ZZZZZ  1

00000 31 00000 32 ZZZZZ 32 00000 1048576 00000 33554432 00000 33554431 ZZZZZ -33554431 ZZZZZ -33554432 ZZZZZ 2147483647 00000 2147483647 ZZZZZ -2147483648
00000 31
00000 32
ZZZZZ 32
00000 1048576
00000 33554432
00000 33554431
ZZZZZ -33554431
ZZZZZ -33554432
ZZZZZ 2147483647
00000 2147483647
ZZZZZ -2147483648

测试数据输出(test.out):
00000
0000N
0000N
0000R
00010
00000

0000Z
00010
0000Z
10000
00000
ZZZZZ
00000
ZZZZZ
ZZZZY
ZZZZZ
ZZZZZ
*/



/**/
// @see https://github.com/tokitsu-kaze/University-Projects/blob/master/%E7%AE%97%E6%B3%95%E5%AF%BC%E8%AE%BA/2018-2016%E7%BA%A7-%E7%9F%AD%E5%AD%A6%E6%9C%9F-%E7%AC%AC%E4%B8%89%E6%AC%A1%E8%80%83%E8%AF%95/E%20-%20%E8%BD%A6%E7%89%8C%E5%8F%B7%E9%97%AE%E9%A2%98.cpp
// #include <bits/stdc++.h>
using namespace std;
// #pragma comment(linker, "/STACK:1024000000,1024000000")
#define mem(a,b) memset((a),(b),sizeof(a))
#define MP make_pair
#define pb push_back
#define fi first
#define se second
#define sz(x) (int)x.size()
#define len(x) (int)x.length()
#define all(x) x.begin(),x.end()
// #define _GLIBCXX_PERMIT_BACKWARD_HASH
// #include <ext/hash_map>
// using namespace __gnu_cxx;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> PII;
typedef pair<ll, ll> PLL;
typedef vector<int> VI;
typedef vector<ll> VL;
// struct str_hash { size_t operator()(const string& str)const { return __stl_hash_string(str.c_str()); } };
const int INF = 0x3f3f3f3f;
const ll LLINF = 0x3f3f3f3f3f3f3f3f;
const double PI = acos(-1.0);
const double eps = 1e-4;
const int MAX = 1e5 + 10;
const ll mod = 1e9 + 7;

int main_()
{
	ll n, stp, pos[333], i, tot, now, lim;
	char re[333];
	string s;
	mem(pos, 0);
	for (i = 0; i <= 9; i++)
		pos[i + '0'] = i, re[i] = '0' + i;
	tot = 10;
	for (i = 'A'; i <= 'Z'; i++)
	{
		if (i == (int)'O' || i == (int)'Q' || i == (int)'I' || i == (int)'B') continue;
		pos[i] = tot++;
		re[tot - 1] = i - 'A' + 'A';
	}
	//    cout<<tot<<endl;
	lim = 0;
	now = 1;
	for (i = 0; i < 5; i++)
	{
		lim += (tot - 1)*now;
		now *= tot;
	}
	lim++;
	while (cin >> s >> stp)
	{
		stp %= lim;
		n = 0;
		reverse(all(s));
		now = 1;
		for (i = 0; i < len(s); i++)
		{
			n += pos[s[i]] * now;
			now *= tot;
		}
		//     cout<<n<<endl;
		n += stp;
		(n += lim) %= lim;
		//     cout<<n<<endl;
		string ans = "";
		function<void(ll)> dfs = [&](ll x)
		{
			if (!x) return;
			dfs(x / tot);
			// printf("%lld", (x % tot)) << " ";
			ans += re[x%tot];
		};
		dfs(n);
		//    cout<<ans<<endl;
		for (i = 0; i < 5 - len(ans); i++) putchar('0');
		cout << ans << "\n";
	}
	return 0;
}
/**/
