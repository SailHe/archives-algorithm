#include "SoftwareTest.h"

/*
文件编程例子
fprintf(fp, "%s\n", s);
fprintf(fp, "%d\n", n);
fgets(ans, 5, fp);
///fprintf(fp, (i == 0 ? "%d %d\n" : "%d %d\n"), le, ri); fprintf(fp, "\n"); ///

srand(time(0));
//随机输入生成
//标程的输入输出
//调用标程的到输出
fp = fopen("sample.in", "w");
freopen("sample.in", "r", stdin);
freopen("sample.out", "w", stdout);
*/

/*
0.StandardPrgram: 得到标程->将其main函数改为solution供数据生成的main调用
1.测试时注释掉带有三个反斜线(///)的语句, 所有输入数据写在input文件内
2.在控制台得出预期结果后打开刚刚注释掉的内容, 当前是从input读入数据结果存入新建的文件夹的0~9.in(out)文件中, 最多支持10组数据
3.另外, 在输出测试数据时, 凡是有输入的地方都需要手动fprintf到fp中, 无法局部的变量使用全局变量*/

#include <windows.h> // 创建目录
#include <clocale> // 指定编码(c语言库locale.h)
using namespace SoftwareTest;
# include "SampleProgram\agoj19_03_20.cpp"
// 此处调用例程
int solution(){
	SampleProgram_03_20().run();
	return 0;
}

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
		base = Random(200);
		int leftBound = base, rightLimit = base + Random(21370 - base);
		printf("%d %d\n", leftBound, rightLimit);
		int size = 10 + Random(995);
		bool printfEnd = false;
		for (int i = 0; i < size; ++i) {
			base = Random(leftBound);
			int currentValue = -99999;
			// 2/size 的机率遇见中途的中止标志(size > 10) 当然 遇见后就不会再次出现
			if (IsProbability(size - 2, size) || printfEnd) {
				// 0.5的概率上限减小三倍(为了使数据更随机: 负数可能, 小于下限概率更大)
				int limit = 0;
				SoftwareTest::loopUntilNotEq([&]() {
					return limit = rightLimit - (base * (IsProbability(1, 2) ? 3 : 1));
				}, 0);
				currentValue = Random(limit);
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

// 通过[解题样例程序]从[单个输入文件]计算出对应的[单个输出文件] (用例数据目录, 用例文件名, 时限ms)
int calcSingleOutputData(std::string &sampleFolderName, std::string &inputCaseFileName, std::string &outputCaseFileName, double timeLimit, SoftwareTest::SampleProgram &sampleSolution) {
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
int generateInOut(std::string &sampleFolderName, std::string &caseFileName, int testCaseCnt) {
	std::wstring wtmp;
	toWstringFromString(sampleFolderName, wtmp);
	BOOL flag = CreateDirectory(wtmp.c_str(), NULL);
	_ASSERT_EXPR(flag == 0, "目录创建失败!");
	std::string tmp = sampleFolderName;
	tmp += std::string("/") + caseFileName;
	std::string inputFilePath = tmp + std::string(".in");
	std::string outputFilePath = tmp + std::string(".out");
	// 这玩意没写权限('r') 也没啥反应...
	freopen(inputFilePath.c_str(), "w", stdout);
	std::fstream output(outputFilePath);

	// 个数为1005, int范围内 每个数字最大值 21370
	int base = 0;
	while (testCaseCnt-- > 0) {
		base = Random(200);
		int leftBound = base, rightLimit = base + Random(21370 - base);
		printf("%d %d\n", leftBound, rightLimit);
		int size = 10 + Random(995);
		bool printfEnd = false;
		std::vector<int> integetList;
		integetList.clear();
		for (int i = 0; i < size; ++i) {
			base = Random(leftBound);
			int currentValue = -99999;
			// 2/size 的机率遇见中途的中止标志(size > 10) 当然 遇见后就不会再次出现
			if (IsProbability(size - 2, size) || printfEnd) {
				// 0.5的概率上限减小三倍(为了使数据更随机: 负数可能, 小于下限概率更大)
				int limit = 0;
				SoftwareTest::loopUntilNotEq([&]() {
					return limit = rightLimit - (base * (IsProbability(1, 2) ? 3 : 1));
				}, 0);
				currentValue = Random(limit);
				if (!printfEnd) {
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

// 多输入文件多输出文件 (已有单输入文件, 已有解题程序 生成对应的多个输入输出文件 最多支持0~9共10组)
int generateMultipleInOut(){
	bool flag = CreateDirectory((L"0123"), NULL);//文件夹名
	char inputFilePath[20] = "0123/0.in";///
	char outputFilePath[20] = "0123/0.out";///
	/**/
	freopen("input", "r", stdin); //从input读取输入数据
	bool reInput = true;//重复输入
	int n, c;
	while (~scanf("%d%d", &n, &c)){

		clock_t Start = clock();///
		freopen(outputFilePath, "w", stdout);///
		fp = fopen(inputFilePath, "w");/// 将输入数据写入in文件
		fprintf(fp, "%d %d\n", n, c); ///
		/**/

		solution();

		if (!reInput)
			break;

		++inputFilePath[5]; /// 多个in文件
		++outputFilePath[5]; /// 多个out文件
		clock_t Stop = clock();
		if ((double)(Stop - Start) > 1000){// (double)(Stop - Start) / CLK_TCK //秒
			puts("超时1000ms");
		}
		/**/
	}
	fclose(fp); ///
	fp = NULL;
	return 0;
}

int main() {
	int testCaseCnt = 2+1;
	std::string testCaseFolderName("0123"), fileName("0");
	/*int num, cnt = 0;
	while (std::cin >> num) {
		cnt++;
	}
	return 0;*/
	// SampleProgram_03_20().run();
	generateInOut(testCaseFolderName, fileName, testCaseCnt);
	// generateInputData(testCaseFolderName, fileName, testCaseCnt);
	calcSingleOutputData(testCaseFolderName, fileName, fileName + "_SP", 1000*3, SampleProgram_03_20());
	SoftwareTest::JudgeByCompare(
		(testCaseFolderName + "/" + fileName + ".out").c_str(),
		(testCaseFolderName + "/" + fileName + "_SP" + ".out").c_str(),
		testCaseCnt
	);
	// freopen("stdout", "r", stdout);
	// std::cout << "end" << std::endl;
	// printf("end2");
	return 0;
}
