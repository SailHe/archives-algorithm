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

int runRandomTest() {
	functionalRandomTest();
	return 0;
}
