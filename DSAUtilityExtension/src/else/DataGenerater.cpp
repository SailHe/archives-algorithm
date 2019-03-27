#include <windows.h> // CreateDirectory
#include "./SoftwareTest.h"
#include "./StringUtility.h"
#include "IOSpace.h"

namespace DataGenerater {
	// ����һ��Ŀ¼ ����ֵ: 0��ʾ�Ѵ��ڸ�Ŀ¼, 1��ʾ�´���
	BOOL createDirectory(std::string const &dirName) {
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

	// ʹ�ø����������������� �����������ض�����ָ���ļ� (����Ŀ¼��, �����ļ���, ��������)
	int generateInputData(
		std::string &sampleFolderName,
		std::string &caseFileName,
		int testCaseCnt,
		std::function<void(void)> genInputDataProcess
	) {
		createDirectory(sampleFolderName);
		std::string inputFilePath;
		inputFilePath = joinDirFilePath(sampleFolderName, caseFileName + std::string(".in"));
		IOSpace::RedirectStdoutTo(inputFilePath.c_str());

		while (testCaseCnt-- > 0) {
			genInputDataProcess();
		}
		IOSpace::RedirectStdoutTo();
		return 0;
	}

	// ͨ��[�������]��[���������ļ�]�������Ӧ��[��������ļ�] (��������Ŀ¼, �����ļ���, ʱ��ms)
	int calcSingleOutputData(
		std::string const &sampleFolderName,
		std::string const &inputCaseFileName,
		std::string const &outputCaseFileName,
		double timeLimit,
		SoftwareTestSpace::SampleProgram &sampleSolution
	) {
		std::wstring wtmp;
		wtmp = StringUtility::toWstring(sampleFolderName);
		BOOL flag = CreateDirectory(wtmp.c_str(), NULL);
		_ASSERT_EXPR(flag == 0, "Ŀ¼����ʧ��!");
		std::string inputFilePath = sampleFolderName + std::string("/") + inputCaseFileName + std::string(".in");
		std::string outputFilePath = sampleFolderName + std::string("/") + outputCaseFileName + std::string(".out");
		IOSpace::RedirectStdinFrom(inputFilePath.c_str());
		IOSpace::RedirectStdoutTo(outputFilePath.c_str());

		clock_t Start = clock();
		sampleSolution.run();
		clock_t Stop = clock();

		IOSpace::RedirectStdoutTo();
		IOSpace::RedirectStdinFrom();
		// (double)(Stop - Start) / CLK_TCK //��
		_ASSERT_EXPR((double)(Stop - Start) < timeLimit, "��ʱ(�ɳ���Release)");
		return 0;
	}

	// ʹ�ø����������������� �������ض��������ָ���ļ� (û���������� û�н������ ��������Щ����д������������)
	int generateInputOutputData(
		std::string const &sampleFolderName,
		std::string const &caseFileName,
		int testCaseCnt,
		std::function<void(std::ofstream &)> genInOutPro
	) {
		createDirectory(sampleFolderName);
		std::string inputFilePath, outputFilePath;
		inputFilePath = joinDirFilePath(sampleFolderName, caseFileName + std::string(".in"));
		outputFilePath = joinDirFilePath(sampleFolderName, caseFileName + std::string(".out"));
		// ����׼����ض���Ϊָ���ļ�
		IOSpace::RedirectStdoutTo(inputFilePath.c_str());

		std::ofstream output = std::ofstream(outputFilePath, std::ios::out | std::ios::trunc);

		while (testCaseCnt-- > 0) {
			genInOutPro(output);
		}

		output.close();
		IOSpace::RedirectStdoutTo();
		return 0;
	}

	// generateMultipleInOutData
	// �������ļ�������ļ� (���е��������ļ�, ������������ ���ɶ�Ӧ�Ķ����������ļ� ���֧��0~9��10��)
	// ������ʷ��Զ�ҿ���ûɶ�ô� �Ѿ�ɾ�� ��Ҫ˼����ʹ�����������������ļ���, Ȼ���ļ�������, Ȼ�����´򿪸��ļ�(���ض���), �������͵þ��������

	int trainServices(
		SoftwareTestSpace::SampleProgram &sampleP, 
		int testCaseCnt, 
		std::string const &testCaseFolderName, 
		std::string const &fileName
	) {
		std::function<void(void)> inputDataGen = sampleP.inputDataGenClosure();
		std::function<void(std::ofstream &)> outputDataGen = sampleP.outputDataGenClosure();
		// generateInputData(testCaseFolderName, fileName, testCaseCnt, inputDataGen); // ûɶ�õĸо�
		generateInputOutputData(testCaseFolderName, fileName, testCaseCnt, [inputDataGen, outputDataGen](std::ofstream &output) {
			inputDataGen();
			outputDataGen(output);
		});
		std::string samplParCalcOutputData = fileName + "_sample_parogram";
		calcSingleOutputData(testCaseFolderName, fileName, samplParCalcOutputData, 1000 * 30, sampleP);
		IOSpace::JudgeByCompare(
			joinDirFilePath(testCaseFolderName, fileName + ".out").c_str(),
			joinDirFilePath(testCaseFolderName, fileName + "_sample_parogram.out").c_str(),
			testCaseCnt
		);
		return 0;
	}
}