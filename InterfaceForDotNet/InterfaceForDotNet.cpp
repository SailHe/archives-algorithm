// InterfaceForDotNet.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "utility\Biginteger.h"
#include "ExtendSpace.h"
#define _ERC_DLL_API extern  "C" _declspec(dllexport) 

_ERC_DLL_API int SubInDll(int a, int b){
	return a - b;
}

_ERC_DLL_API int A(int n, int m){
	return MathExtend::A(n, m);
}

_ERC_DLL_API int add(int x, int y){
	return x + y;
}

_ERC_DLL_API int sub(int x, int y){
	return x - y;
}

_ERC_DLL_API int testChar(char * src, char * res, int nCount){
	memcpy(res, src, sizeof(char) * nCount);
	return 1;
}

_ERC_DLL_API int testStruct(SystemTime &stSrc, SystemTime &stRes){
	stRes = stSrc;
	return 1;
}
