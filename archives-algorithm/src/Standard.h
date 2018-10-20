#pragma once

#ifndef _STANDARD_H_
#define _STANDARD_H_
//#include "stdafx.h"
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
//#include"E:\Projects\myLib\TreeObject.h"
//#include"E:\Projects\myLib\Graph.h"

#define MAX_R 1001
#define MAX_C 1001
#define ARRAY_TEMPLATE template<class T, class Iterator>

//里面的许多标准输入输出方法模板(iterate系列)只建议用作测试 不建议实际使用
namespace StandardExtend{
	using namespace std;
	/****************Ascll*******************/
	char toLowerAlph(char c);

	int toIntNum(char alphOrCharNum);

	//10ToA(Alph)
	char toUppercaseAscllChar(int num);

	//'A' == 'a' == '0' ... 'J' == 'j' == '9' 以此类推
	bool isAa0Equal(char a, char b);

	template<class T>
	//left <= value < right  EG: inRange(minEle, element, maxEle+1); -> element [minEle, maxEle]
	bool inRange(T left, T value, T right){
		return left <= value && value < right;
	}

	template<class Iterator, class Fun>
	void iterate(Iterator left, Iterator right, Fun visit){
		while (left != right){
			visit(left);
			++left;
		}
	}

	template<class Iterator>
	//coutFillChar表示填充字符 相当于printf的%02d 中的'0' coutWidth表示其中的2
	void outPutIterable(Iterator left, Iterator right, char coutFillChar = 0, size_t coutWidth = 4){
		cout << setfill(coutFillChar);
		cout << setw(coutWidth) << *left;
		++left;
		iterate(left, right, [&](Iterator left){
			cout << " " << setw(coutWidth) << *left;
		});
		puts("");
	}

	template<class Iterator>
	//lineWrap 隔多少行换行 小于0 表示输出结束换行
	void outPutIterable(Iterator left, Iterator right, char coutFillChar, size_t coutWidth, int lineWrap){
		cout << setfill(coutFillChar);
		cout << *left;
		++left;
		int c = 1;
		lineWrap = lineWrap < 0 ? right - left : lineWrap;
		iterate(left, right, [&](Iterator left){
			++c;
			if (c % lineWrap == 0){
				//最后一个
				//cout << " " << setw(coutWidth) << *left << endl;
				cout << setw(coutWidth) << *left << endl;
			}
			else if (c % lineWrap == 1){
				//第一个
				cout << *left;
			}
			else{
				//中间的
				//cout << " " << setw(coutWidth) << *left;
				cout << setw(coutWidth) << *left;
			}
		});
		//最后一次没有换行这里换行
		if (c % lineWrap != 0){
			puts("");
		}
	}

	template<class Iterator, class Fun>
	//lineWrapFun 返回是否换行  bool(* lineWrapFun)(Iterator left, Iterator right)
	void outPutIterable(Iterator left, Iterator right, char coutFillChar, size_t coutWidth, Fun lineWrapFun){
		cout << setfill(coutFillChar);
		int c = 0;
		bool pastWrap = false;
		//lineWrapFun = nullptr == lineWrapFun ? [&](){return right - left; } : lineWrapFun;
		iterate(left, right, [&](Iterator left){
			++c;
			pastWrap = lineWrapFun(left, right);
			if (1 == c){//第一个
				cout << *left;
			}
			else{
				//中间&最后
				cout << setw(coutWidth) << *left;
			}
			if (pastWrap){//最后一个
				cout << endl;
				c = 0;
			}
		});
		//最后一次没有换行这里换行
		if (!pastWrap){
			puts("");
		}
	}


	template<class T>
	void outPut2DArrayList(vector<vector<T>> const &arrayList2D, char coutFillChar = '0', size_t coutWidth = 2){
		size_t rows = arrayList2D.size();
		for (size_t r = 0; r < rows; ++r){
			outPutIterable(arrayList2D[r].begin(), arrayList2D[r].end(), coutFillChar, coutWidth);
		}
	}

	template<class T>
	void outPut2DArray(T array2D[MAX_R][MAX_C], size_t n, char coutFillChar = '0', size_t coutWidth = 2){
		for (size_t r = 0; r < n; ++r){
			outPutIterable(array2D[r], array2D[r] + n, coutFillChar, coutWidth);
		}
	}

	template<class T>
	void outPut2DArrayTrangle(T array2D[MAX_R][MAX_C], size_t n, char coutFillChar = '0', size_t coutWidth = 2){
		for (size_t r = 0; r < n; ++r){
			outPutIterable(array2D[r], array2D[r] + r + 1, coutFillChar, coutWidth);
		}
	}

	template<class Iterator>
	size_t rankStatistics(Iterator left, Iterator right){
		size_t rank = 1;
		iterate(left + 1, right, [&](Iterator left){
			rank += *left == *(left - 1) ? 0 : 1;
		});
		return rank;
	}

	//ARRAY_TEMPLATE
	template<class T, class Iterator>
	T minValueStatistics(Iterator left, Iterator right, T MAX_VALUE){
		T minValue = MAX_VALUE;
		//iterate(left, right, [&](Iterator left){
		//	minValue = min(minValue, *left);
		//});
		while (left != right){
			minValue = min(minValue, *left);
			++left;
		}
		return minValue;
	}

	template<class T, class Iterator>
	T maxValueStatistics(Iterator left, Iterator right, T MIN_VALUE){
		T maxValue = MIN_VALUE;
		while (left != right){
			maxValue = max(maxValue, *left);
			++left;
		}
		return maxValue;
	}

	template<class T, class Iterator>
	T sumValueStatistics(Iterator left, Iterator right, T ZERO_VALUE){
		T sumValue = ZERO_VALUE;
		iterate(left, right, [&](Iterator left){
			sumValue += *left;
		});
		return sumValue;
	}

	template<class T, class Iterator>
	double avlValueStatistics(Iterator left, Iterator right, T ZERO_VALUE){
		return sumValueStatistics(left, right, ZERO_VALUE) / (double)(right - left);
	}

};

#endif
