#pragma once
#include "../ExtendSpace.h"
#include "RandomSpace.h"
#include "IOSpace.h"

namespace SoftwareTestSpace {

	class SampleProgram {
	public:
		// 执行例程 (等价于解题程序的主函数)
		virtual int run() = 0;
	};

}
