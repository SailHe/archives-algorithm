#include "SoftwareTest.h"

/*
�ļ��������
fprintf(fp, "%s\n", s);
fprintf(fp, "%d\n", n);
fgets(ans, 5, fp);
///fprintf(fp, (i == 0 ? "%d %d\n" : "%d %d\n"), le, ri); fprintf(fp, "\n"); ///

srand(time(0));
//�����������
//��̵��������
//���ñ�̵ĵ����
fp = fopen("sample.in", "w");
freopen("sample.in", "r", stdin);
freopen("sample.out", "w", stdout);
*/

/*
0.StandardPrgram: �õ����->����main������Ϊsolution���������ɵ�main����
1.����ʱע�͵�����������б��(///)�����, ������������д��input�ļ���
2.�ڿ���̨�ó�Ԥ�ڽ����򿪸ո�ע�͵�������, ��ǰ�Ǵ�input�������ݽ�������½����ļ��е�0~9.in(out)�ļ���, ���֧��10������
3.����, �������������ʱ, ����������ĵط�����Ҫ�ֶ�fprintf��fp��, �޷��ֲ��ı���ʹ��ȫ�ֱ���*/

#include <windows.h> // ����Ŀ¼
#include <clocale> // ָ������(c���Կ�locale.h)
using namespace SoftwareTest;
# include "SampleProgram\agoj19_03_20.cpp"
// �˴���������
int solution(){
	SampleProgram_03_20().run();
	return 0;
}

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
		base = Random(200);
		int leftBound = base, rightLimit = base + Random(21370 - base);
		printf("%d %d\n", leftBound, rightLimit);
		int size = 10 + Random(995);
		bool printfEnd = false;
		for (int i = 0; i < size; ++i) {
			base = Random(leftBound);
			int currentValue = -99999;
			// 2/size �Ļ���������;����ֹ��־(size > 10) ��Ȼ ������Ͳ����ٴγ���
			if (IsProbability(size - 2, size) || printfEnd) {
				// 0.5�ĸ������޼�С����(Ϊ��ʹ���ݸ����: ��������, С�����޸��ʸ���)
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

// ͨ��[������������]��[���������ļ�]�������Ӧ��[��������ļ�] (��������Ŀ¼, �����ļ���, ʱ��ms)
int calcSingleOutputData(std::string &sampleFolderName, std::string &inputCaseFileName, std::string &outputCaseFileName, double timeLimit, SoftwareTest::SampleProgram &sampleSolution) {
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
int generateInOut(std::string &sampleFolderName, std::string &caseFileName, int testCaseCnt) {
	std::wstring wtmp;
	toWstringFromString(sampleFolderName, wtmp);
	BOOL flag = CreateDirectory(wtmp.c_str(), NULL);
	_ASSERT_EXPR(flag == 0, "Ŀ¼����ʧ��!");
	std::string tmp = sampleFolderName;
	tmp += std::string("/") + caseFileName;
	std::string inputFilePath = tmp + std::string(".in");
	std::string outputFilePath = tmp + std::string(".out");
	// ������ûдȨ��('r') Ҳûɶ��Ӧ...
	freopen(inputFilePath.c_str(), "w", stdout);
	std::fstream output(outputFilePath);

	// ����Ϊ1005, int��Χ�� ÿ���������ֵ 21370
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
			// 2/size �Ļ���������;����ֹ��־(size > 10) ��Ȼ ������Ͳ����ٴγ���
			if (IsProbability(size - 2, size) || printfEnd) {
				// 0.5�ĸ������޼�С����(Ϊ��ʹ���ݸ����: ��������, С�����޸��ʸ���)
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

// �������ļ�������ļ� (���е������ļ�, ���н������ ���ɶ�Ӧ�Ķ����������ļ� ���֧��0~9��10��)
int generateMultipleInOut(){
	bool flag = CreateDirectory((L"0123"), NULL);//�ļ�����
	char inputFilePath[20] = "0123/0.in";///
	char outputFilePath[20] = "0123/0.out";///
	/**/
	freopen("input", "r", stdin); //��input��ȡ��������
	bool reInput = true;//�ظ�����
	int n, c;
	while (~scanf("%d%d", &n, &c)){

		clock_t Start = clock();///
		freopen(outputFilePath, "w", stdout);///
		fp = fopen(inputFilePath, "w");/// ����������д��in�ļ�
		fprintf(fp, "%d %d\n", n, c); ///
		/**/

		solution();

		if (!reInput)
			break;

		++inputFilePath[5]; /// ���in�ļ�
		++outputFilePath[5]; /// ���out�ļ�
		clock_t Stop = clock();
		if ((double)(Stop - Start) > 1000){// (double)(Stop - Start) / CLK_TCK //��
			puts("��ʱ1000ms");
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
