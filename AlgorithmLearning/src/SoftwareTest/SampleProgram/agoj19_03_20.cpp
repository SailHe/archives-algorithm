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

// 放到宏定义后有bug
#include "ExtendSpace.h"
#include "../SoftwareTest.h"

#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

//#define MAX_N 1000+5
#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>


class SampleProgram_03_20 : public SoftwareTest::SampleProgram {
public:
	// 需要生成1000条测试数据
	int run() override {
		int leftBound, rightLmit;
		while (std::cin >> leftBound >> rightLmit) {
			std::cin.get();
			int currentValue;
			std::vector<int> integetList;
			bool isEndInput = false;
			while (true) {
				char nextC = std::cin.peek();
				if (nextC == '\n' || std::cin.eof()) {
					break;
				}
				std::cin >> currentValue;
				// nextC = std::cin.get();
				if (currentValue == -99999 || isEndInput) {
					isEndInput = true;
					continue;
				}
				if (StandardExtend::inRange(leftBound, currentValue, rightLmit + 1) && integetList.size() < 1001) {
					integetList.push_back(currentValue);
				}
			}
			auto sumValue = StandardExtend::sumValueStatistics(integetList.begin(), integetList.end(), 0);
			double avlValue = 0.0;
			if (integetList.size() != 0) {
				avlValue = StandardExtend::avlValueStatistics(integetList.begin(), integetList.end(), 0.0);
			}
			std::cout << std::fixed << std::setprecision(2) << integetList.size() << " " << sumValue << " " << avlValue << std::endl;
		}
		return 0;
	}

};

/*

【第三次作业】
【问题描述】
输入若干个整数，计算在给定范围内（含边界值）值的个数、总和、平均值。
【输入说明】
本问题有多组测试数据，对于每组测试数据，输入有两行，第一行为两个整数，分别表示给
定范围的两个值；第二行为若干个整数，整数的个数不超过 1000，超过部分的数据被忽略，
结束标志为-99999（可能存在没有结束标志就终止输入的情况）。
【输出说明】
对于每组输入，输出为三个数。前两个是整数，分别表示个数和总和，最后一个平均值为实
数，要求平均值保留二位小数，输出数之间有一个空格。如果没有符合要求的输入，那么输
出 0 0 0.00。
【Sample Input】

2 7
1 2 3 4 5 6 7 8 -99999
3 6
-1 1 -99999

【Sample Output】
6 27 4.50
0 0 0.00
【说明】
在开始尝试解决问题时，可以考虑把个数限制为不超过 10 个，最后再在把限制放宽到满足题目所要求的。
对于限制为 10 时，以下几组测试数据是最起码要通过的：
3 9
0 1 2 3 4 5 6 7 8 9 10 11 -99999
3 5
0 1 2 3 4 5 6 -99999
3 5
0 1 2 3 4 5 6
3 5
0 1 2 3 4 5 6 7 8 9 10 11 12
3 5
0 1 2 3 4 5 6 7 8 9 -99999
3 5
0 1 2 3 4 5 6 7 -99999 1 2 3 4 5
3 5
0 1 2 3 4 5 6 7 8 9 10 11 12 110 -99999
【其他说明】
这次题目需要不止一次课，因此：
一、先设计测试用例
二、修改测试用例
三、根据测试用例设计代码，起码你的代码要通过你自己的测试用例。
*/
