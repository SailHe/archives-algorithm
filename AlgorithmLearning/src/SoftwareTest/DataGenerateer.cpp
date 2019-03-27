#include <windows.h> // CreateDirectory
#include "./else/SoftwareTest.h"
#include "./else/StringUtility.h"
#include "SampleProgram\SampleProgram.h"

// ����һ��Ŀ¼ ����ֵ: 0��ʾ�Ѵ��ڸ�Ŀ¼, 1��ʾ�´���
BOOL createDirectory(std::string &dirName) {
	/*
	// ��ʱ�����һ������ʵ�ʵĲ��
	std::wstring wtmp;
	StringUtility::toWstringFromString(sampleFolderName, wtmp);
	*/
	std::wstring wtmp;
	// �˴����ƶ�����
	wtmp = StringUtility::toWstring(dirName);
	return CreateDirectory(wtmp.c_str(), NULL);
}

// ƴ��·��
std::string joinDirFilePath(std::string const &folderName, std::string const &fileName) {
	return folderName + std::string("/") + fileName;;
}

// ������� Input Data (����Ŀ¼��, �����ļ���, ��������)
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

// ͨ��[�������]��[���������ļ�]�������Ӧ��[��������ļ�] (��������Ŀ¼, �����ļ���, ʱ��ms)
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
	_ASSERT_EXPR(flag == 0, "Ŀ¼����ʧ��!");
	std::string inputFilePath = sampleFolderName + std::string("/") + inputCaseFileName + std::string(".in");
	std::string outputFilePath = sampleFolderName + std::string("/") + outputCaseFileName + std::string(".out");
	freopen(inputFilePath.c_str(), "r", stdin);
	freopen(outputFilePath.c_str(), "w", stdout);
	clock_t Start = clock();

	sampleSolution.run();

	clock_t Stop = clock();
	// (double)(Stop - Start) / CLK_TCK //��
	_ASSERT_EXPR((double)(Stop - Start) < timeLimit, "��ʱ(�ɳ���Release)");
	return 0;
}

// �������Input Data ͬʱ����� Output Data (û���������� û�н������ ��������Щ����д������������)
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
	// ������ûдȨ��('r') Ҳûɶ��Ӧ...
	freopen(inputFilePath.c_str(), "w", stdout);
	std::ofstream output = std::ofstream(outputFilePath, std::ios::out | std::ios::trunc);

	while (testCaseCnt-- > 0) {
		genInOutPro(output);
	}
	output.close();
	return 0;
}

// generateMultipleInOutData
// �������ļ�������ļ� (���е��������ļ�, ������������ ���ɶ�Ӧ�Ķ����������ļ� ���֧��0~9��10��)
// ������ʷ��Զ�ҿ���ûɶ�ô� �Ѿ�ɾ�� ��Ҫ˼����ʹ�����������������ļ���, Ȼ���ļ�������, Ȼ�����´򿪸��ļ�(���ض���), �������͵þ��������

// ���ڲ���������ֵ���ݵĸ���
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
