// InterfaceForDotNet.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
//#include "Biginteger.h"
#include "ExtendSpace.h"
extern  "C" _declspec(dllexport) int A(int n, int m)
{
	return MathExtend::A(n, m);
}

