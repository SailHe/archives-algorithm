#pragma once
// ����Winϵͷ�ļ����C/C++��ͷ
#include <assert.h>//����
#include <conio.h>//getch//
#include <time.h>
#include<cstring>//memset//
#include<cstdio>//scanf//
#include <fstream>
#include <iostream>
#include<algorithm>
#include<cmath>
#include<iomanip> //cout���ֲ�����
#include<set>
#include<string>
#include<stack>
#include<queue>
#include<list>
#include<map>
#include<functional> //greater<int>
#include<memory> //����ָ��:shared_ptr<>; unique_ptr<>;---P400
#include <unordered_set>
#include <unordered_map> //hash


//˫�ֽ��޷�������(16λ)
#define MAX_INT16 65535
#define MIN_INT16 (-MAX_INT16-1)
//4�ֽ�32λ�з������� 2^31-1 = 0x7FFFFFFF; Լ2e9(10bit)
#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)
//8�ֽ�64λ�з������� 2^63-1 std::to_string(pow(2, 63)); Լ9e18(19bit)
#define MAX_INT64 9223372036854775807
#define MIN_INT64 (-MAX_INT64-1)

#define infinity MAX_INT32
