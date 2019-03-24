#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <random>
#include <time.h>

#include "ExtendSpace.h"
#include "./else/SoftwareTest.h"

void functionalRandomTest() {
	StandardExtend::testAndOut("最大值可比较", MAX_INT32 > 1, true);
	StandardExtend::testAndOut("最小值可比较", MIN_INT32 < -1, true);
	try{
		RandomSpace::RandomUnsigned(0);
	}catch (unsigned e) {
		StandardExtend::testAndOut("异常值", e, 0u);
	}catch (const std::exception &e){
		std::cout << e.what() << std::endl;
	}
	StandardExtend::testAndOut("确定的随机值[0, 1)", RandomSpace::RandomUnsigned(1), 0u);
	StandardExtend::testAndOut("确定的随机值[0, 1)", RandomSpace::Random(1), 0);
	StandardExtend::testAndOut("确定的随机值[0, -1)", RandomSpace::Random(-1), 0);
	StandardExtend::testAndOut("确定的随机值[-1, 0)", RandomSpace::RandomRange(-1, 0), -1);
	StandardExtend::testAndOut("确定的随机值[MAX_INT32 - 1, MAX_INT32)", RandomSpace::RandomUnsignedRange(MAX_INT32 - 1, MAX_INT32), MAX_INT32-1u);
}

int runRandomTest() {
	functionalRandomTest();
	return 0;
}
