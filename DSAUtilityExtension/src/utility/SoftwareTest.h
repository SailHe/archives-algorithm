#pragma once
#include "../ExtendSpace.h"
#include "RandomSpace.h"
#include "IOSpace.h"
#include "../DSAUEdll.h"

namespace SoftwareTestSpace {

	/*
	生成数据时会调用对应的函数 为了方便实现以及最大限度保留程序的独立性(解耦) 基于重定向输入输出实现
	PS:
	生成数据时stdout被导到输入数据的文件, output是输出文件的流
	在重定向后再次重定向回屏幕:
	@see https://blog.csdn.net/chenyiming_1990/article/details/9473275
	Key Words: freopen_s 重定向回 stdout
	*/
	class SampleProgram {
	public:
		// 执行例程 (等价于解题程序的主函数)
		virtual int run() = 0;
		// 返回单个输入用例数据生成的闭包
		virtual std::function<void(void)> inputDataGenClosure() = 0;
		// 返回单个输出用例数据生成的闭包
		virtual std::function<void(std::ofstream &)> outputDataGenClosure() = 0;
	};

	// 返回该列表的初始化列表字串(指程序中的初始化列表)
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
	
	// 用于测试输入数值数据的个数
	DSAUTILITYEXTENSION_API int inputNumberCount();

	// 一条龙服务: 生成给定样例程序的用例(in和out)->计算样例程序的运行结果->比对样例输出和样例程序的运行结果
	// 样例程序 用例组数 用例存放目录 用例文件名称(*.in, *.out)
	DSAUTILITYEXTENSION_API int trainServices(
		SoftwareTestSpace::SampleProgram &sampleP,
		int testCaseCnt,
		std::string const &testCaseFolderName,
		std::string const &fileName
	);

}
