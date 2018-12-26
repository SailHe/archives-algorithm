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
#include<iomanip>//cout各种操纵器

#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)

// #include "ExtendSpace.h"

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

// 方向枚举
enum DIRECTION_ENUM {
	UN_KNOEN_DIR, UP_DIR, DOWM_DIR, LEFT_DIR, RIGHT_DIR, LEFT_UP_DIR, RIGHT_UP_DIR
};

/*
1
ABDCRHGWDWSDSKJDSKDFHJKFDKJDSAFKJFDAKFDSAJFDKASDJLFLDKF
ERUDSHDFHGFLKGFGFKGFLKSAEWALUTRHGFKIFDGITRMDFLKDSLSDLLEHJFKLEKIREFMFK

RDSDKFKFKSAFKFDKDSSDJFLKF
*/
// 最长公共子序列 (如果有多个按照assic码排序, 小的排前面)
// PS 此题的解法略精妙 如果忘记可能需要配合图解才能看懂
int mainForSolveA() {
	int caseCnt;
	scanf("%d", &caseCnt);
	while (caseCnt-- > 0) {
		string lhs, rhs, equalitySeq;
		cin >> lhs >> rhs;
		lhs = " " + lhs;
		rhs = " " + rhs;
		// if (lhs.length() == 0 || rhs.length() == 0)
		// 第0行和第0列需要初始化为0
		ArrayList<ArrayList<int>> length2d(lhs.length(), ArrayList<int>(rhs.length(), 0));
		ArrayList<ArrayList<DIRECTION_ENUM>> path2d(lhs.length(), ArrayList<DIRECTION_ENUM>(rhs.length(), UN_KNOEN_DIR));
		
		// 行与lhs相关 列与rhs相关
		for (SizeType r = 1; r < lhs.length(); ++r) {
			for (SizeType c = 1; c < rhs.length(); ++c) {
				if (lhs[r] == rhs[c]) {
					//equalitySeq += lhs;
					length2d[r][c] = length2d[r - 1][c - 1] + 1;
					path2d[r][c] = LEFT_UP_DIR;
				}
				else {
					if (length2d[r - 1][c] >= length2d[r][c - 1]) {
						length2d[r][c] = length2d[r - 1][c];
						path2d[r][c] = UP_DIR;
					}
					else {
						length2d[r][c] = length2d[r][c - 1];
						path2d[r][c] = LEFT_DIR;
					}
				}
			}
		}
		SizeType r = lhs.length()-1, c = rhs.length()-1;
		// int nCount = path2d[r][c];  用这个nCount作为while的终止条件也行
		while (r > 0 && c > 0) {
			switch (path2d[r][c])
			{
			case LEFT_UP_DIR: {equalitySeq += lhs[r]; --r; --c; break; }
			case UP_DIR: {--r; break; }
			case LEFT_DIR: {--c; break; }
			default:
				break;
			}
		}
		// cout << length2d[lhs.length() - 1][rhs.length() - 1] << endl;
		cout << equalitySeq.length() << endl;
		reverse(equalitySeq.begin(), equalitySeq.end());
		cout << equalitySeq << endl;
	}
	return 0;
}

int main() {
	mainForSolveA();
	return 0;
}

