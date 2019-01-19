#pragma once
// 除了Win系头文件外的C/C++标头
#include <assert.h>//断言
#include <conio.h>//getch//
#include <time.h>
#include<cstring>//memset//
#include<cstdio>//scanf//
#include <fstream>
#include <iostream>
#include<algorithm>
#include<cmath>
#include<iomanip> //cout各种操纵器
#include<set>
#include<string>
#include<stack>
#include<queue>
#include<list>
#include<map>
#include<functional> //greater<int>
#include<memory> //智能指针:shared_ptr<>; unique_ptr<>;---P400
#include <unordered_set>
#include <unordered_map> //hash


//双字节无符号整数(16位)
#define MAX_INT16 65535
#define MIN_INT16 (-MAX_INT16-1)
//4字节32位有符号整数 2^31-1 = 0x7FFFFFFF; 约2e9(10bit)
#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)
//8字节64位有符号整数 2^63-1 std::to_string(pow(2, 63)); 约9e18(19bit)
#define MAX_INT64 9223372036854775807
#define MIN_INT64 (-MAX_INT64-1)

#define infinity MAX_INT32
