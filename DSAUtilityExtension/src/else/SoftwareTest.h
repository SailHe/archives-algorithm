#pragma once
#include "../ExtendSpace.h"
#include "RandomSpace.h"
#include "IOSpace.h"
#include "../DSAUEdll.h"

namespace SoftwareTestSpace {

	/*
	��������ʱ����ö�Ӧ�ĺ��� Ϊ�˷���ʵ���Լ�����޶ȱ�������Ķ�����(����) �����ض����������ʵ��
	PS:
	��������ʱstdout�������������ݵ��ļ�, output������ļ�����
	���ض�����ٴ��ض������Ļ:
	@see https://blog.csdn.net/chenyiming_1990/article/details/9473275
	Key Words: freopen_s �ض���� stdout
	*/
	class SampleProgram {
	public:
		// ִ������ (�ȼ��ڽ�������������)
		virtual int run() = 0;
		// ���ص������������������ɵıհ�
		virtual std::function<void(void)> inputDataGenClosure() = 0;
		// ���ص�����������������ɵıհ�
		virtual std::function<void(std::ofstream &)> outputDataGenClosure() = 0;
	};

	// ���ظ��б�ĳ�ʼ���б��ִ�(ָ�����еĳ�ʼ���б�)
	template<typename Itorator, typename T>
	std::string getInitString(Itorator begin, Itorator end, std::function<std::string(T)> toString) {
		std::string initListStr;
		std::for_each(begin, end, [&initListStr, &toString](T curVa) {
			StandardExtend::formatStrAppend(initListStr, toString(curVa), ",");
		});
		return "{" + initListStr + "}";
	}

}

namespace DataGenerater {
	
	// ���ڲ���������ֵ���ݵĸ���
	DSAUTILITYEXTENSION_API int inputNumberCount();

	// һ��������: ���ɸ����������������(in��out)->����������������н��->�ȶ����������������������н��
	// �������� �������� �������Ŀ¼ �����ļ�����(*.in, *.out)
	DSAUTILITYEXTENSION_API int trainServices(
		SoftwareTestSpace::SampleProgram &sampleP,
		int testCaseCnt,
		std::string const &testCaseFolderName,
		std::string const &fileName
	);

}
