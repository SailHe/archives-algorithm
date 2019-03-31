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
/*setfill(char ch)        Fill whitespace with 'ch'
setprecision(int n)     Set floating - point precision to 'n'
setw(int w)             Read or write value to 'w' characters
setbase(int b)          Output integers in base 'b'(only 'b' is 8 / 10 / 16 could the function work)
*/
#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

#define MAX_INT_NUM 2147483647
#define MIN_INT_NUM (-MAX_INT_NUM-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>

using namespace std;

#include "ExtendSpace.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

//m表示n的当前整数划分中最大的值n = m + m1 +... 
int solveD(int n, int m) {
	if (n == 1 || m == 1) {
		return 1;
	}
	else if (n < m) {
		//n不能小于m 当小于时返回等于时的值 
		return solveD(n, n);
	}
	else if (n == m) {
		//n与m相等时有 n=m, n = m + 1 -> m = n-1 
		return 1 + solveD(n, n - 1);
	}
	else {
		//n > m > 1; 情况一: 6 = 5 + 1; 情况二: (6-4 = 2; 6-4 = 1 + 1)
		return solveD(n, m - 1) + solveD(n - m, m);
	}
}

int table[MAX_R][MAX_C];

int main_To_solve_E() {
	int n;
	for (int n = 1; n < MAX_R; ++n) {
		for (int m = 1; m < MAX_C; ++m) {
			int result = -1;
			if (n == 1 || m == 1) {
				result = 1;
			}
			else if (n < m) {
				//n不能小于m 当小于时返回等于时的值 
				result = table[n][n];
			}
			else if (n == m) {
				//n与m相等时有 n=m, n = m + 1 -> m = n-1 
				result = 1 + table[n][n - 1];
			}
			else {
				//n > m > 1; 情况一: 6 = 5 + 1; 情况二: (6-4 = 2; 6-4 = 1 + 1)
				result = table[n][m - 1] + table[n - m][m];
			}
			table[n][m] = result;
		}
	}
	while (cin >> n) {
		//cout<<solveD(n, n)<<endl;
		cout << table[n][n] << endl;
	}
	return 0;
}

//N个“=”
void printEndLine(int n) {
	for (int i = 0; i < n; ++i) {
		printf("=");
	}
	puts("");
}

//当前行, 列, 当前需要向右下遍历多少个元素 
void calcC(int &sr, int &sc, int count, int &currentN) {
	int r = sr, c = sc;
	bool isOdd = count % 2 == 1;
	int dr = isOdd ? 1 : -1;
	int dc = isOdd ? 1 : -1;
	for (int i = 0; i < count; ++i) {
		table[r][c] = currentN--;
		r += dr;
		c += dc;
	}
	if (isOdd) {
		++sc;
	}
	else {
		--r - 1;
	}
}

//solve-C
int main() {
	int n;
	while (cin >> n) {
		int r = n - 1, c = 0, count = 1, cn = n * n;
		for (int i = 0; i < n; ++i) {
			calcC(r, c, count++, cn);
			StandardExtend::outPut2DArray(table, n);
			printEndLine(4 * n);
		}
	}
	return 0;
}




