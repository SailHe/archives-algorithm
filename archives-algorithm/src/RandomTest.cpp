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
	StandardExtend::testAndOut("���ֵ�ɱȽ�", MAX_INT32 > 1, true);
	StandardExtend::testAndOut("��Сֵ�ɱȽ�", MIN_INT32 < -1, true);
	try{
		RandomSpace::RandomUnsigned(0);
	}catch (unsigned e) {
		StandardExtend::testAndOut("�쳣ֵ", e, 0u);
	}catch (const std::exception &e){
		std::cout << e.what() << std::endl;
	}
	StandardExtend::testAndOut("ȷ�������ֵ[0, 1)", RandomSpace::RandomUnsigned(1), 0u);
	StandardExtend::testAndOut("ȷ�������ֵ[0, 1)", RandomSpace::Random(1), 0);
	StandardExtend::testAndOut("ȷ�������ֵ[0, -1)", RandomSpace::Random(-1), 0);
	StandardExtend::testAndOut("ȷ�������ֵ[-1, 0)", RandomSpace::RandomRange(-1, 0), -1);
	StandardExtend::testAndOut("ȷ�������ֵ[MAX_INT32 - 1, MAX_INT32)", RandomSpace::RandomUnsignedRange(MAX_INT32 - 1, MAX_INT32), MAX_INT32-1u);
}

void testForMersenneTwister() {
	// non-deterministic generator  
	std::random_device rd;
	//÷ɭ��ת���㷨��Mersenne twister����һ��α����������㷨��
	// ���ɱ����������ʿ[1]��1997�꿪�����������޶������ֶ��ϵľ������Եݹ顣
	// ���Կ��ٲ�����������α������������˹ŵ�����������㷨�ĺܶ�ȱ�ݡ�
	std::mt19937 gen(rd());
	// to seed mersenne twister.  
	// replace the call to rd() with a constant value to get repeatable results.  

	// ��ͨ�����ʹ��ʱ����Ϊ���� ʹ��0�Ļ�ÿ�β��������ֵ��һ����
	srand(clock());
	for (int i = 0; i < 5; ++i) {
		// print the raw output of the generator.  
		std::cout << "÷ɭ�����: " << gen() << " ";
		std::cout << "��ͨ�����: " << rand() << std::endl;
	}
	std::cout << std::endl;
}


int runRandomTest() {
	testForMersenneTwister();
	functionalRandomTest();
	return 0;
}
