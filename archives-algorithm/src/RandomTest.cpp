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

void testForMersenneTwister() {
	// non-deterministic generator  
	std::random_device rd;
	//梅森旋转演算法（Mersenne twister）是一个伪随机数发生算法。
	// 由松本真和西村拓士[1]在1997年开发，基于有限二进制字段上的矩阵线性递归。
	// 可以快速产生高质量的伪随机数，修正了古典随机数发生算法的很多缺陷。
	std::mt19937 gen(rd());
	// to seed mersenne twister.  
	// replace the call to rd() with a constant value to get repeatable results.  

	// 普通随机数使用时间作为种子 使用0的话每次产生的随机值是一样的
	srand(clock());
	for (int i = 0; i < 5; ++i) {
		// print the raw output of the generator.  
		std::cout << "梅森随机数: " << gen() << " ";
		std::cout << "普通随机数: " << rand() << std::endl;
	}
	std::cout << std::endl;
}


int runRandomTest() {
	testForMersenneTwister();
	functionalRandomTest();
	return 0;
}
