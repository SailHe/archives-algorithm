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

#include "else/SoftwareTest.h"
#include "ExtendSpace.h"

class SampleProgram_03_20 : public SoftwareTestSpace::SampleProgram {
	/*
	PS: 
	https://www.jianshu.com/p/d686ad9de817
	[语法糖]https://memeda.github.io/%E6%8A%80%E6%9C%AF/2016/09/29/CPP%E8%AF%AD%E6%B3%95%E7%B3%96.html
	https://www.cnblogs.com/Aion/p/3449756.html
	理论上来说lambda内全部使用堆上分配内存的智能指针会比较安全
	因为就算某个对象被销毁后 其函数闭包仍能使用, 此时就会导致问题
	*/
	// 仅用于数据生成
	std::vector<int> integetList;
public:
	// 需要生成1000条测试数据
	int run() override {
		int leftBound, rightLmit;
		while (std::cin >> leftBound >> rightLmit) {
			std::cin.get();
			int currentValue;
			std::vector<int> integetList;
			bool isEndInput = false;
			int maxCount = 1000;
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
				if (StandardExtend::inRange(leftBound, currentValue, rightLmit + 1) && maxCount > 0) {
					integetList.push_back(currentValue);
				}
				// 注意'逻辑与'截断
				--maxCount;
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

	std::function<void(void)> inputDataGenClosure() override {
		// 最大个数为1005, int范围内 每个数字最大值 21370
		const unsigned MAX_SIZE_R_LIMIT = 1001;

		// -调试时就可以看到这些函数内的临时变量会在[捕获后]被释放掉, 即使是引用捕获也一样, 这点与java不同(其GC机制决定的)
		// 用于生成特殊用例: 最大容量生效次数 值全正次数 值全负次数
		unsigned maxSizeEffectiveCount = 100, maxOpEffectiveCount = 1, maxNeEffectiveCount = 1;
		// 全符合条件 全不符合条件
		unsigned maxAllEffectiveCount = 1, maxAllNotEffectiveCount = 1;

		// -因此采用堆上分配内存保持其不被释放, 使用map结构当匿名类的对象好了, lambda也是类似的匿名类, 但貌似不能new的样子
		// -使用std::shared_ptr直接赋值捕获即可, 但auto_ptr, unique_ptr的移动赋值捕获貌似不行
		// -std::shared_ptr<int> a(new int(1));auto b = std::move(a); // 但根据此处实验是都可以的, 多半是编译器没有实现
		std::shared_ptr<std::map<std::string, unsigned>> specialValMap(new std::map<std::string, unsigned>());
		(*specialValMap)["maxSizeEffectiveCount"] = 100;
		(*specialValMap)["maxOpEffectiveCount"] = 1;
		(*specialValMap)["maxNeEffectiveCount"] = 1;
		(*specialValMap)["maxAllEffectiveCount"] = 1;
		(*specialValMap)["maxAllNotEffectiveCount"] = 1;
		return [this, MAX_SIZE_R_LIMIT, specialValMap=std::move(specialValMap)]() {
			int base = RandomSpace::RandomP1(200);
			int lowerBound = base, upperBound = base + RandomSpace::RandomP1(21370 - base);
			printf("%d %d\n", lowerBound, upperBound);
			// 1%的机率出现1次MAX_SIZE_R_LIMIT, 其余属于[10, MAX_SIZE_R_LIMIT)
			unsigned size = RandomSpace::IsProbability(1, 1, (*specialValMap)["maxSizeEffectiveCount"]) ?
				MAX_SIZE_R_LIMIT : RandomSpace::RandomUnsignedRange(10, MAX_SIZE_R_LIMIT);
			bool printfEnd = false;
			integetList.clear();
			bool isAllOp = RandomSpace::IsProbability(1, 2, (*specialValMap)["maxOpEffectiveCount"]);
			bool isAllNe = false;
			if (!isAllOp) {
				isAllNe = RandomSpace::IsProbability(1, 2, (*specialValMap)["maxNeEffectiveCount"]);
			}
			for (unsigned i = 0; i < size; ++i) {
				base = RandomSpace::RandomP1(lowerBound);
				int currentValue = -99999;
				// 2/size 的机率遇见中途的中止标志(size > 10) 当然 遇见后就不会再次出现
				if (RandomSpace::IsProbability(size - 2, size) || printfEnd) {
					// 0.5的概率上界减小至少三倍(为了使数据更随机: 负数可能, 小于下界的正整数的概率更大)
					int realUpperBound = 0;
					if (RandomSpace::IsProbability(1, 2, (*specialValMap)["maxAllEffectiveCount"])) {
						currentValue = RandomSpace::RandomRange(lowerBound, upperBound);
					}
					else {
						if (RandomSpace::IsProbability(1, 2, (*specialValMap)["maxAllNotEffectiveCount"])) {
							currentValue = -RandomSpace::RandomNotRange(lowerBound, upperBound);
						}
						else {
							RandomSpace::loopUntilNotEq([&]() {
								return realUpperBound = upperBound - (base * (RandomSpace::IsProbability(1, 2) ?
									RandomSpace::RandomP1(upperBound / lowerBound) * 3 : 1));
							}, 0);
							currentValue = RandomSpace::RandomP1(realUpperBound);
						}
					}
					// 全正全负 逻辑
					if ((currentValue < 0 && isAllOp) || (currentValue > 0 && isAllNe)) {
						currentValue = -currentValue;
					}
					if (!printfEnd && StandardExtend::inRange(lowerBound, currentValue, upperBound + 1) && i < MAX_SIZE_R_LIMIT - 1) {
						integetList.push_back(currentValue);
					}
				}
				else {
					printfEnd = true;
				}

				printf(i == 0 ? "%d" : " %d", currentValue);
			}
			puts("");
		};
	}

	std::function<void(std::ofstream &)> outputDataGenClosure() override {
		return [this](std::ofstream &output) {
			auto sumValue = StandardExtend::sumValueStatistics(integetList.begin(), integetList.end(), 0);
			double avlValue = 0.0;
			if (integetList.size() != 0) {
				avlValue = StandardExtend::avlValueStatistics(integetList.begin(), integetList.end(), 0.0);
			}
			output << std::fixed << std::setprecision(2) << integetList.size() << " " << sumValue << " " << avlValue << std::endl;
		};
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

1 102
100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 -100 100
这个用例共1001个数, 倒数第二个, 正数第1000个数不符合要求 (题目要求忽略超过1000个的部分)
答案是
999 99900 100.00

还有一个用例: 构造一个以unsigned最大值为size的用例 主要看程序的计数器会不会爆炸(上溢或下溢)
PS: MAX_INT32粗略计算就有100GB 还不算数据生成以及计算的时间
*/
