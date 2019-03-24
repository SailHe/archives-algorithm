#include <windows.h> // ����Ŀ¼
#include <clocale> // ָ������(c���Կ�locale.h)
#include "SoftwareTest.h"
#include "SampleProgram\SampleProgram.h"

// @see https://gist.github.com/1901/5684151
// @see https://www.cnblogs.com/hnrainll/archive/2011/05/07/2039700.html
// Key words: std::wstring ת std::string
// ��һ��wstringת��Ϊstring
void toStringFromeWstring(std::wstring const &origin, std::string &dest) {
	std::setlocale(LC_CTYPE, "");

	size_t const mbs_len = wcstombs(NULL, origin.c_str(), 0);
	std::vector<char> tmp(mbs_len + 1);
	wcstombs(&tmp[0], origin.c_str(), tmp.size());

	dest.assign(tmp.begin(), tmp.end() - 1);
}

// ��һ��stringת��Ϊwstring
void toWstringFromString(std::string const &origin, std::wstring &dest) {
	//   std::setlocale(LC_CTYPE, "");
	std::setlocale(LC_CTYPE, "zh_CN");

	size_t const wcs_len = mbstowcs(NULL, origin.c_str(), 0);
	std::vector<wchar_t> tmp(wcs_len + 1);
	mbstowcs(&tmp[0], origin.c_str(), origin.size());

	dest.assign(tmp.begin(), tmp.end() - 1);
}

// ������� Input Data (����Ŀ¼��, �����ļ���, �������� һ�����ݹ���ò�ƻ������)
int generateInputData(std::string &sampleFolderName, std::string &caseFileName, int testCaseCnt) {
	std::wstring wtmp;
	toWstringFromString(sampleFolderName, wtmp);
	BOOL flag = CreateDirectory(wtmp.c_str(), NULL);
	_ASSERT_EXPR(flag == 0, "Ŀ¼����ʧ��!");
	std::string tmp = sampleFolderName;
	tmp += std::string("/") + caseFileName;
	std::string inputFilePath = tmp + std::string(".in");
	freopen(inputFilePath.c_str(), "w", stdout);

	// ����Ϊ1005, int��Χ�� ÿ���������ֵ 21370
	int base = 0;
	while (testCaseCnt-- > 0) {
		base = SoftwareTest::Random1(200);
		int leftBound = base, rightLimit = base + SoftwareTest::Random1(21370 - base);
		printf("%d %d\n", leftBound, rightLimit);
		int size = 10 + SoftwareTest::Random1(995);
		bool printfEnd = false;
		for (int i = 0; i < size; ++i) {
			base = SoftwareTest::Random1(leftBound);
			int currentValue = -99999;
			// 2/size �Ļ���������;����ֹ��־(size > 10) ��Ȼ ������Ͳ����ٴγ���
			if (SoftwareTest::IsProbability(size - 2, size) || printfEnd) {
				// 0.5�ĸ������޼�С����(Ϊ��ʹ���ݸ����: ��������, С�����޸��ʸ���)
				int limit = 0;
				SoftwareTest::loopUntilNotEq([&]() {
					return limit = rightLimit - (base * (SoftwareTest::IsProbability(1, 2) ? 3 : 1));
				}, 0);
				currentValue = SoftwareTest::Random1(limit);
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

// ͨ��[�������]��[���������ļ�]�������Ӧ��[��������ļ�] (��������Ŀ¼, �����ļ���, ʱ��ms)
int calcSingleOutputData(
	std::string &sampleFolderName, 
	std::string &inputCaseFileName, 
	std::string &outputCaseFileName, 
	double timeLimit, 
	SoftwareTest::SampleProgram &sampleSolution
) {
	std::wstring wtmp;
	toWstringFromString(sampleFolderName, wtmp);
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
int generateInputOutputData(std::string &sampleFolderName, std::string &caseFileName, int testCaseCnt) {
	std::wstring wtmp;
	toWstringFromString(sampleFolderName, wtmp);
	// flag == 0��ʾ�Ѵ��ڸ�Ŀ¼, 1��ʾ�´���
	BOOL flag = CreateDirectory(wtmp.c_str(), NULL);
	// _ASSERT_EXPR(flag == 0, "Ŀ¼����ʧ��!");
	std::string tmp = sampleFolderName;
	tmp += std::string("/") + caseFileName;
	std::string inputFilePath = tmp + std::string(".in");
	std::string outputFilePath = tmp + std::string(".out");
	// ������ûдȨ��('r') Ҳûɶ��Ӧ...
	freopen(inputFilePath.c_str(), "w", stdout);
	std::ofstream output = std::ofstream(outputFilePath, std::ios::out | std::ios::trunc);

	// ����Ϊ1005, int��Χ�� ÿ���������ֵ 21370
	int base = 0;
	while (testCaseCnt-- > 0) {
		base = SoftwareTest::Random1(200);
		int leftBound = base, rightLimit = base + SoftwareTest::Random1(21370 - base);
		printf("%d %d\n", leftBound, rightLimit);
		int size = 10 + SoftwareTest::Random1(995);
		bool printfEnd = false;
		std::vector<int> integetList;
		integetList.clear();
		for (int i = 0; i < size; ++i) {
			base = SoftwareTest::Random1(leftBound);
			int currentValue = -99999;
			// 2/size �Ļ���������;����ֹ��־(size > 10) ��Ȼ ������Ͳ����ٴγ���
			if (SoftwareTest::IsProbability(size - 2, size) || printfEnd) {
				// 0.5�ĸ������޼�С����(Ϊ��ʹ���ݸ����: ��������, С�����޸��ʸ���)
				int limit = 0;
				SoftwareTest::loopUntilNotEq([&]() {
					return limit = rightLimit - (base * (SoftwareTest::IsProbability(1, 2) ? 3 : 1));
				}, 0);
				currentValue = SoftwareTest::Random1(limit);
				if (!printfEnd && StandardExtend::inRange(leftBound, currentValue, rightLimit + 1) && integetList.size() < 1001) {
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
// �������ļ�������ļ� (���е��������ļ�, ������������ ���ɶ�Ӧ�Ķ����������ļ� ���֧��0~9��10��)
// ������ʷ��Զ�ҿ���ûɶ�ô� �Ѿ�ɾ�� ��Ҫ˼����ʹ�����������������ļ���, Ȼ���ļ�������, Ȼ�����´򿪸��ļ�(���ض���), �������͵þ��������

int main() {
	int testCaseCnt = 1000;
	std::string testCaseFolderName("0123"), fileName("0");
	/*int num, cnt = 0;
	while (std::cin >> num) {
		if (num < 89) {
			std::cout << num << " ";
		}
		cnt++;
	}
	std::cout << cnt << std::endl;
	return 0;
	SampleProgram_03_20().run();
	return 0;*/
	generateInputOutputData(testCaseFolderName, fileName, testCaseCnt);
	// generateInputData(testCaseFolderName, fileName, testCaseCnt);
	calcSingleOutputData(testCaseFolderName, fileName, fileName + "_SP", 1000*30, SampleProgram_03_20());
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
