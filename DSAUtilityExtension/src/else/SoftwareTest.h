#pragma once
#include "../ExtendSpace.h"
#include "RandomSpace.h"
#include "IOSpace.h"

namespace SoftwareTestSpace {

	// ��������ʱ����ö�Ӧ�ĺ��� Ϊ�˷���ʵ���Լ�����޶ȱ�������Ķ�����(����) �����ض����������ʵ��
	// PS: ��������ʱstdout�������������ݵ��ļ�, output������ļ����� �޷����ض�����ٴ��ض������Ļ
	class SampleProgram {
	public:
		// ִ������ (�ȼ��ڽ�������������)
		virtual int run() = 0;
		// ���������������ɱհ�
		virtual std::function<void(void)> inputDataGenClosure() = 0;
		// ��������������ɱհ�
		virtual std::function<void(std::ofstream &)> outputDataGenClosure() = 0;
	};
}