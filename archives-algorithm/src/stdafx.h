#ifndef __STDAFX_H
#define __STDAFX_H
/*StandardExtend Application Framework Extensions（标准应用程序框架的扩展）*/
/*所有其它文件需要的头文件(有时间按照字母排序)*/
#include <assert.h>//断言
#include <windows.h>//QueryPerformanceFrequency//
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
#include<functional>//greater<int>
#include<memory>/*智能指针:shared_ptr<>; unique_ptr<>;---p400*/
#include <unordered_set>
#include <unordered_map>

/*
explicit(显式/清楚/明确),只能用在带单个参数的构造函数前，告诉编译器，不能使用该构造函数进行隐式类型转换。
如果确实想转换，则必须采用显式的类型转换方式来进行。从而避免了可能出现的问题和额外的调用开销。

<分离编译模式> 该模式使得 C++ 代码编写得称心而优雅。
:允许在一处翻译单元（Translation Unit）中定义（define）函数、类型、类对象等，在另一处翻译单元引用它们。
编译器（Compiler）处理完所有翻译单元后，
链接器（Linker）接下来处理所有指向 extern 符号的引用，
从而生成单一可执行文件。
extern(外来的) 为了访问其他编译单元（如另一代码文件）中的变量或对象，对普通类型（包括基本数据类、结构和类）
export(导出/出口/输出) 在定义模板类对象和模板函数时的extern功能
*/

//用于安全替换
#define scanf scanf_s
#define gets gets_s

//调试开关 关闭后可减少一些不必要的判断 提高运行效率
#define DEBUG false

//返回一个函数参数  宏参数: 支持迭代器的容器
#define PARAMETER_ALL_OBJECT(container) container.begin(), container.end()

//limit(限度；界线)是一个界限即: > limit时退出
//限界for
#define FOR_LIMIT(i, lowerBound, limit) for (auto i = lowerBound; i < limit; ++i)

//无符号限界for
#define FOR_LIMIT_UNSIGNED(i, lowerBound, limit) for (size_t i = lowerBound; i < limit; ++i)

//it是用户指定的一个名字, 其类型由下界(lowerBound)决定, 迭代范围(bound)内必须保证会出现终止符(terminator: 终结者)
//终止for 宏参数: 指定的名字, 变量1, 量2 (量12需支持!=运算)
#define FOR(it, lowerBound, terminator) for (auto it = lowerBound; it != terminator; ++it)

//for容器内所有元素 宏参数: 支持迭代器的容器  (针对容器作了end()优化的FOR)
#define FOR_ALL_OBJECT(container) for (auto &element : container)

//末尾无空格的打印(cout)
#define DISPLAY(A, lowerBound, terminator) {FOR(i, lowerBound, terminator){printf(i == lowerBound ? "" : " ");cout << A[i] << endl;}}

//调试用打印
#define DE_PRINTF(s){if(DEBUG)printf("---%s\n", s);}
#ifndef  _DEBUG_ERROR
//_STL_VERIFY(false, s);
#define _DEBUG_ERROR(s){_ASSERT_EXPR(false, s); }
#endif //  _DEBUG_ERROR



//4字节32位 有符号无穷大0x7FFFFFFF; 双字节无符号整数的最大值是65535
#define infinity 2147483647
#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)
using namespace std;
using namespace std::placeholders;

//_ASSERTE(_CrtCheckMemory());//内存检查

#endif
