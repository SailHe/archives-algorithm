#include <windows.h> // 创建目录
#include <clocale> // 指定编码(c语言库locale.h)
#include "./else/SoftwareTest.h"
#include "SampleProgram\SampleProgram.h"

// @see https://gist.github.com/1901/5684151
// @see https://www.cnblogs.com/hnrainll/archive/2011/05/07/2039700.html
// Key words: std::wstring 转 std::string
// 把一个wstring转化为string
void toStringFromeWstring(std::wstring const &origin, std::string &dest) {
	std::setlocale(LC_CTYPE, "");

	size_t const mbs_len = wcstombs(NULL, origin.c_str(), 0);
	std::vector<char> tmp(mbs_len + 1);
	wcstombs(&tmp[0], origin.c_str(), tmp.size());

	dest.assign(tmp.begin(), tmp.end() - 1);
}

// 把一个string转化为wstring
void toWstringFromString(std::string const &origin, std::wstring &dest) {
	//   std::setlocale(LC_CTYPE, "");
	std::setlocale(LC_CTYPE, "zh_CN");

	size_t const wcs_len = mbstowcs(NULL, origin.c_str(), 0);
	std::vector<wchar_t> tmp(wcs_len + 1);
	mbstowcs(&tmp[0], origin.c_str(), origin.size());

	dest.assign(tmp.begin(), tmp.end() - 1);
}

// 随机生成 Input Data (用例目录名, 用例文件名, 用例个数 一次数据过多貌似会出问题)
int generateInputData(std::string &sampleFolderName, std::string &caseFileName, int testCaseCnt) {
	std::wstring wtmp;
	toWstringFromString(sampleFolderName, wtmp);
	BOOL flag = CreateDirectory(wtmp.c_str(), NULL);
	_ASSERT_EXPR(flag == 0, "目录创建失败!");
	std::string tmp = sampleFolderName;
	tmp += std::string("/") + caseFileName;
	std::string inputFilePath = tmp + std::string(".in");
	freopen(inputFilePath.c_str(), "w", stdout);

	// 个数为1005, int范围内 每个数字最大值 21370
	int base = 0;
	while (testCaseCnt-- > 0) {
		base = RandomSpace::RandomP1(200);
		int lowerBound = base, upperBound =  RandomSpace::RandomRange(base, 21370);
		printf("%d %d\n", lowerBound, upperBound);
		// (10, 1005]
		unsigned size = 10 + RandomSpace::RandomUnsignedP1(995);
		bool printfEnd = false;
		for (unsigned i = 0; i < size; ++i) {
			base = RandomSpace::RandomP1(lowerBound);
			int currentValue = -99999;
			// 2/size 的机率遇见中途的中止标志(size > 10) 当然 遇见后就不会再次出现
			if (RandomSpace::IsProbability(size - 2, size) || printfEnd) {
				// 0.5的概率上界减小至少三倍(为了使数据更随机: 负数可能, 小于下界的正整数的概率更大)
				int realUpperBound = 0;
				RandomSpace::loopUntilNotEq([&]() {
					return realUpperBound = upperBound - (base * (RandomSpace::IsProbability(1, 2) ?
						RandomSpace::RandomP1(upperBound / lowerBound) * 3 : 1));
				}, 0);
				currentValue = RandomSpace::RandomP1(realUpperBound);
			}
			else {
				printfEnd = true;
			}
			printf(i == 0 ? "%d" : " %d", currentValue);
		}
		puts("");
	}
	return 0;
}

// 通过[解题程序]从[单个输入文件]计算出对应的[单个输出文件] (用例数据目录, 用例文件名, 时限ms)
int calcSingleOutputData(
	std::string &sampleFolderName, 
	std::string &inputCaseFileName, 
	std::string &outputCaseFileName, 
	double timeLimit, 
	SoftwareTestSpace::SampleProgram &sampleSolution
) {
	std::wstring wtmp;
	toWstringFromString(sampleFolderName, wtmp);
	BOOL flag = CreateDirectory(wtmp.c_str(), NULL);
	_ASSERT_EXPR(flag == 0, "目录创建失败!");
	std::string inputFilePath = sampleFolderName + std::string("/") + inputCaseFileName + std::string(".in");
	std::string outputFilePath = sampleFolderName + std::string("/") + outputCaseFileName + std::string(".out");
	freopen(inputFilePath.c_str(), "r", stdin);
	freopen(outputFilePath.c_str(), "w", stdout);
	clock_t Start = clock();

	sampleSolution.run();

	clock_t Stop = clock();
	// (double)(Stop - Start) / CLK_TCK //秒
	_ASSERT_EXPR((double)(Stop - Start) < timeLimit, "超时(可尝试Release)");
	return 0;
}

// 随机生成Input Data 同时计算出 Output Data (没有输入数据 没有解题程序 适用于那些不好写出解题程序的题)
int generateInputOutputData(std::string &sampleFolderName, std::string &caseFileName, int testCaseCnt) {
	std::wstring wtmp;
	toWstringFromString(sampleFolderName, wtmp);
	// flag == 0表示已存在该目录, 1表示新创建
	BOOL flag = CreateDirectory(wtmp.c_str(), NULL);
	// _ASSERT_EXPR(flag == 0, "目录创建失败!");
	std::string tmp = sampleFolderName;
	tmp += std::string("/") + caseFileName;
	std::string inputFilePath = tmp + std::string(".in");
	std::string outputFilePath = tmp + std::string(".out");
	// 这玩意没写权限('r') 也没啥反应...
	freopen(inputFilePath.c_str(), "w", stdout);
	std::ofstream output = std::ofstream(outputFilePath, std::ios::out | std::ios::trunc);

	// 最大个数为1005, int范围内 每个数字最大值 21370
	const int MAX_SIZE_R_LIMIT = 1001;
	int base = 0;
	// 用于生成特殊用例: 最大容量生效次数 值全正次数 值全负次数
	unsigned maxSizeEffectiveCount = 100, maxOpEffectiveCount = 1, maxNeEffectiveCount = 1;
	// 全符合条件 全不符合条件
	unsigned maxAllEffectiveCount = 1, maxAllNotEffectiveCount = 1;
	while (testCaseCnt-- > 0) {
		base = RandomSpace::RandomP1(200);
		int lowerBound = base, upperBound = base + RandomSpace::RandomP1(21370 - base);
		printf("%d %d\n", lowerBound, upperBound);
		// 1%的机率出现1次MAX_SIZE_R_LIMIT, 其余属于[10, MAX_SIZE_R_LIMIT)
		unsigned size = RandomSpace::IsProbability(1, 1, maxSizeEffectiveCount) ?
			MAX_SIZE_R_LIMIT : RandomSpace::RandomUnsignedRange(10, MAX_SIZE_R_LIMIT);
		bool printfEnd = false;
		std::vector<int> integetList;
		bool isAllOp = RandomSpace::IsProbability(1, 2, maxOpEffectiveCount);
		bool isAllNe = false;
		if (!isAllOp) {
			isAllNe = RandomSpace::IsProbability(1, 2, maxNeEffectiveCount);
		}
		for (unsigned i = 0; i < size; ++i) {
			base = RandomSpace::RandomP1(lowerBound);
			int currentValue = -99999;
			// 2/size 的机率遇见中途的中止标志(size > 10) 当然 遇见后就不会再次出现
			if (RandomSpace::IsProbability(size - 2, size) || printfEnd) {
				// 0.5的概率上界减小至少三倍(为了使数据更随机: 负数可能, 小于下界的正整数的概率更大)
				int realUpperBound = 0;
				if (RandomSpace::IsProbability(1, 2, maxAllEffectiveCount)) {
					currentValue = RandomSpace::RandomRange(lowerBound, upperBound);
				}
				else {
					if (RandomSpace::IsProbability(1, 2, maxAllNotEffectiveCount)) {
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
		auto sumValue = StandardExtend::sumValueStatistics(integetList.begin(), integetList.end(), 0);
		double avlValue = 0.0;
		if (integetList.size() != 0) {
			avlValue = StandardExtend::avlValueStatistics(integetList.begin(), integetList.end(), 0.0);
		}
		output << std::fixed << std::setprecision(2) << integetList.size() << " " << sumValue << " " << avlValue << std::endl;
	}
	output.close();
	return 0;
}

// generateMultipleInOutData
// 多输入文件多输出文件 (已有单个输入文件, 已有样例程序 生成对应的多个输入输出文件 最多支持0~9共10组)
// 由于历史久远且可能没啥用处 已经删除 主要思想是使用数字命名的用例文件名, 然后文件名自增, 然后重新打开该文件(或重定向), 接下来就得具体分析了

// 用于测试输入数值数据的个数
int inputNumberCount() {
	int num, cnt = 0;
	while (std::cin >> num) {
		/*if (num == 100) {
			std::cout << num << " ";
		}*/
		++cnt;
	}
	std::cout << cnt << std::endl;
	return cnt;
}

int main() {
	int testCaseCnt = 1000;
	// data/ sample test Judge
	std::string testCaseFolderName("0123"), fileName("0");
	/*
	inputNumberCount();
	return 0;
	SampleProgram_03_20().run();
	return 0;*/
	
	// @TODO: 将下面两个方法的重复内容简化, 使通用化(类似calcSingleOutputData即可)
	//generateInputOutputData(testCaseFolderName, fileName, testCaseCnt);
	// generateInputData(testCaseFolderName, fileName, testCaseCnt);
	calcSingleOutputData(testCaseFolderName, fileName, fileName + "_SP", 1000*30, SampleProgram_03_20());
	IOSpace::JudgeByCompare(
		(testCaseFolderName + "/" + fileName + ".out").c_str(),
		(testCaseFolderName + "/" + fileName + "_SP" + ".out").c_str(),
		testCaseCnt
	);
	// 无法在重定向后再次重定向回屏幕
	// freopen("stdout", "r", stdout);
	// std::cout << "end" << std::endl;
	// printf("end2");
	return 0;
}
