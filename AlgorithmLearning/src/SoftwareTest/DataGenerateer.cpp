#include <windows.h> // CreateDirectory
#include "./else/SoftwareTest.h"
#include "./else/StringUtility.h"
#include "SampleProgram\SampleProgram.h"

// 创建一个目录 返回值: 0表示已存在该目录, 1表示新创建
BOOL createDirectory(std::string &dirName) {
	/*
	// 有时间测试一下两者实际的差距
	std::wstring wtmp;
	StringUtility::toWstringFromString(sampleFolderName, wtmp);
	*/
	std::wstring wtmp;
	// 此处是移动语义
	wtmp = StringUtility::toWstring(dirName);
	return CreateDirectory(wtmp.c_str(), NULL);
}

// 拼接路径
std::string joinDirFilePath(std::string const &folderName, std::string const &fileName) {
	return folderName + std::string("/") + fileName;;
}

// 随机生成 Input Data (用例目录名, 用例文件名, 用例个数)
int generateInputData(std::string &sampleFolderName, std::string &caseFileName, int testCaseCnt, std::function<void(void)> genInputDataProcess) {
	createDirectory(sampleFolderName);
	std::string inputFilePath;
	inputFilePath = joinDirFilePath(sampleFolderName, caseFileName + std::string(".in"));
	freopen(inputFilePath.c_str(), "w", stdout);

	while (testCaseCnt-- > 0) {
		genInputDataProcess();
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
	wtmp = StringUtility::toWstring(sampleFolderName);
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
int generateInputOutputData(
	std::string &sampleFolderName, 
	std::string &caseFileName, 
	int testCaseCnt, 
	std::function<void(std::ofstream &)> genInOutPro
) {
	createDirectory(sampleFolderName);
	std::string inputFilePath, outputFilePath;
	inputFilePath = joinDirFilePath(sampleFolderName, caseFileName + std::string(".in"));
	outputFilePath = joinDirFilePath(sampleFolderName, caseFileName + std::string(".out"));
	// 这玩意没写权限('r') 也没啥反应...
	freopen(inputFilePath.c_str(), "w", stdout);
	std::ofstream output = std::ofstream(outputFilePath, std::ios::out | std::ios::trunc);

	while (testCaseCnt-- > 0) {
		genInOutPro(output);
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
	SoftwareTestSpace::SampleProgram &sampleP = SampleProgram_03_20();
	/*
	inputNumberCount();
	return 0;
	sampleP.run();
	return 0;
	*/

	std::function<void(void)> inputDataGen = sampleP.inputDataGenClosure();
	std::function<void(std::ofstream &)> outputDataGen = sampleP.outputDataGenClosure();
	generateInputData(testCaseFolderName, fileName, testCaseCnt, inputDataGen);
	generateInputOutputData(testCaseFolderName, fileName, testCaseCnt, [inputDataGen, outputDataGen](std::ofstream &output) {
		inputDataGen();
		outputDataGen(output);
	});
	calcSingleOutputData(testCaseFolderName, fileName, fileName + "_SP", 1000*30, sampleP);
	IOSpace::JudgeByCompare(
		joinDirFilePath(testCaseFolderName, fileName + ".out").c_str(),
		joinDirFilePath(testCaseFolderName, fileName + "_SP.out").c_str(),
		testCaseCnt
	);
	return 0;
}
