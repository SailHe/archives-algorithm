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

namespace Standard {
	template<class T>
	//left <= value < right  EG: inRange(minEle, element, maxEle+1); -> element [minEle, maxEle]
	bool inRange(T left, T value, T right) {
		return left <= value && value < right;
	}

	template<class Iterator, class Fun>
	void iterate(Iterator left, Iterator right, Fun visit) {
		while (left != right) {
			visit(left, right);
			++left;
		}
	}

	template<class Iterator>
	//coutFillChar表示填充字符 相当于printf的%02d 中的'0' coutWidth表示其中的2
	void outPutIterable(Iterator left, Iterator right, char coutFillChar = 0, size_t coutWidth = 4) {
		cout << setfill(coutFillChar);
		cout << setw(coutWidth) << *left;
		++left;
		iterate(left, right, [&](Iterator left, Iterator right) {
			cout << " " << setw(coutWidth) << *left;
		});
		puts("");
	}

	template<class Iterator>
	//lineWrap 隔多少行换行 小于0 表示输出结束换行
	void outPutIterable(Iterator left, Iterator right, char coutFillChar, size_t coutWidth, int lineWrap) {
		cout << setfill(coutFillChar);
		cout << *left;
		++left;
		int c = 1;
		lineWrap = lineWrap < 0 ? right - left : lineWrap;
		iterate(left, right, [&](Iterator left, Iterator right) {
			++c;
			if (c % lineWrap == 0) {
				//最后一个
				//cout << " " << setw(coutWidth) << *left << endl;
				cout << setw(coutWidth) << *left << endl;
			}
			else if (c % lineWrap == 1) {
				//第一个
				cout << *left;
			}
			else {
				//中间的
				//cout << " " << setw(coutWidth) << *left;
				cout << setw(coutWidth) << *left;
			}
		});
		//最后一次没有换行这里换行
		if (c % lineWrap != 0) {
			puts("");
		}
	}

	template<class Iterator, class Fun>
	//lineWrapFun 返回是否换行  bool(* lineWrapFun)(Iterator left, Iterator right)
	void outPutIterable(Iterator left, Iterator right, char coutFillChar, size_t coutWidth, Fun lineWrapFun) {
		cout << setfill(coutFillChar);
		int c = 0;
		bool pastWrap = false;
		//lineWrapFun = nullptr == lineWrapFun ? [&](){return right - left; } : lineWrapFun;
		iterate(left, right, [&](Iterator left, Iterator right) {
			++c;
			pastWrap = lineWrapFun(left, right);
			if (1 == c) {//第一个
				cout << *left;
			}
			else {
				//中间&最后
				cout << setw(coutWidth) << *left;
			}
			if (pastWrap) {//最后一个
				cout << endl;
				c = 0;
			}
		});
		//最后一次没有换行这里换行
		if (!pastWrap) {
			puts("");
		}
	}


	template<class T>
	void outPut2DArrayList(vector<vector<T>> const &arrayList2D, char coutFillChar = '0', size_t coutWidth = 2) {
		size_t rows = arrayList2D.size();
		for (size_t r = 0; r < rows; ++r) {
			outPutIterable(arrayList2D[r].begin(), arrayList2D[r].end(), coutFillChar, coutWidth);
		}
	}

	template<class T>
	void outPut2DArray(T array2D[MAX_R][MAX_C], size_t n, char coutFillChar = '0', size_t coutWidth = 2) {
		for (size_t r = 0; r < n; ++r) {
			outPutIterable(array2D[r], array2D[r] + n, coutFillChar, coutWidth);
		}
	}

	template<class T>
	void outPut2DArrayTrangle(T array2D[MAX_R][MAX_C], size_t n, char coutFillChar = '0', size_t coutWidth = 2) {
		for (size_t r = 0; r < n; ++r) {
			outPutIterable(array2D[r], array2D[r] + r + 1, coutFillChar, coutWidth);
		}
	}

	template<class Iterator>
	size_t rankStatistics(Iterator left, Iterator right) {
		size_t rank = 1;
		iterate(left + 1, right, [&](Iterator left, Iterator right) {
			rank += *left == *(left - 1) ? 0 : 1;
		});
		return rank;
	}

	//ARRAY_TEMPLATE
	template<class T, class Iterator>
	T minValueStatistics(Iterator left, Iterator right, T MAX_VALUE) {
		T minValue = MAX_VALUE;
		//iterate(left, right, [&](Iterator left, Iterator right){
		//	minValue = min(minValue, *left);
		//});
		while (left != right) {
			minValue = min(minValue, *left);
			++left;
		}
		return minValue;
	}

	template<class T, class Iterator>
	T maxValueStatistics(Iterator left, Iterator right, T MIN_VALUE) {
		T maxValue = MIN_VALUE;
		while (left != right) {
			maxValue = max(maxValue, *left);
			++left;
		}
		return maxValue;
	}

	template<class T, class Iterator>
	T sumValueStatistics(Iterator left, Iterator right, T ZERO_VALUE) {
		T sumValue = ZERO_VALUE;
		iterate(left, right, [&](Iterator left, Iterator right) {
			sumValue += *left;
		});
		return sumValue;
	}

	template<class T, class Iterator>
	double avlValueStatistics(Iterator left, Iterator right, T ZERO_VALUE) {
		return sumValueStatistics(left, right, ZERO_VALUE) / (double)(right - left);
	}

}

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
		--r-1;
	}
}

//solve-C
int main() {
	int n;
	while (cin >> n) {
		int r = n - 1, c = 0, count = 1,  cn = n * n;
		for (int i = 0; i < n; ++i) {
			calcC(r, c, count++, cn);
			Standard::outPut2DArray(table, n);
			printEndLine(4 * n);
		}
	}
	return 0;
}




