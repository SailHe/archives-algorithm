#pragma once
#include "../ExtendSpace.h"
#include "RandomSpace.h"
#include "IOSpace.h"

namespace SoftwareTestSpace {

	// 生成数据时会调用对应的函数 为了方便实现以及最大限度保留程序的独立性(解耦) 基于重定向输入输出实现
	// PS: 生成数据时stdout被导到输入数据的文件, output是输出文件的流 无法在重定向后再次重定向回屏幕
	class SampleProgram {
	public:
		// 执行例程 (等价于解题程序的主函数)
		virtual int run() = 0;
		// 返回输入数据生成闭包
		virtual std::function<void(void)> inputDataGenClosure() = 0;
		// 返回输出数据生成闭包
		virtual std::function<void(std::ofstream &)> outputDataGenClosure() = 0;
	};
}