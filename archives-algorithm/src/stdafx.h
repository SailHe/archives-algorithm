#ifndef __STDAFX_H
#define __STDAFX_H
/*Standard Application Framework Extensions（标准应用程序框架的扩展）*/
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
#include<iomanip>
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
#define DEPRINTF(s){if(DEBUG)printf("---%s\n", s);}


//4字节32位 有符号无穷大0x7FFFFFFF; 双字节无符号整数的最大值是65535
#define infinity 2147483647
using namespace std;
using namespace std::placeholders;

/*与Biology相关的三种类型:人物 怪物 管理员*/
enum TYPE{ PLAYER, PEOPLE, MANAGER, MONSTER };
enum STATUS{ NORMAL, ABNORMAL, CONK, QUIT, BLOCKING, BYPASS };//正常 异常 昏迷(死亡用成员方法判断) 退出 被阻挡 绕行状态
/*视图: up(向上)->背影, down->正面, left->左, right->右  ViewCount:视图个数*/
enum View{ BACK, FRONT, LEFT, RIGHT, ViewCount };
//用于指定动画静止时是播放哪一帧(起始帧, 上一帧)
enum StaticFrameEnum{ ORIGIN_FRAME, PAST_FRAME };
enum STEP{ STEP_X_AXIS, STEP_Y_AXIS };//步进方向
enum MODE{ ON, OFF, UNDEFINED };//模式
//constant.h
const View defaultView = BACK;//默认视图(此值转换为int必须是0)
const double EPSINON = 1e-15;
const double EPS = 1e-9;
const double PI = 3.14159265;//八位π
const double PIA = acos(-1.0);//蓝桥杯可用
/*
const int DIR4x[] = { -1, 0, 1, 0 };
const int DIR4y[] = { 0, 1, 0, -1 };
extern const int Dir8[8][2];
extern const int Dir4[4][2];
*/
const int DIR84R[] = { 1, 0, -1, 0, 1, -1, -1, 1 };
const int DIR84C[] = { 0, 1, 0, -1, 1, 1, -1, -1 };
enum DirCountEnum{ DIR4 = 4, DIR8 = 8 };
typedef pair<int, int> DirectionVector;//在做加法运算时 它的first 与 second与习惯的表示xy rc的意义相同
const DirectionVector DIR84[8] = {//加了extern 会出现重定义问题
	/*r轴正方向->第4123象限 便于调试 顺序不能随意更改*/
	{ 1, 0 }/*下*/,
	{ 0, 1 }/*右*/,
	{ -1, 0 }/*上*/,
	{ 0, -1 }/*左*/,

	{ 1, 1 }/*右下*/,
	{ 1, -1 }/*右上*/,
	{ -1, -1 }/*左上*/,
	{ -1, 1 }/*左下*/,
};
//可变数组
//template<typename T>using Varrays = vector<T>;
////数组表
//template<typename T>using ArrayList = vector<T>;
////邻接表
//template<typename T>using LinkedList = list<T>;
//
//using Package = vector<pair<int, int>>;/*背包类*/
//typedef int Sub;//下标类型
//typedef Sub RowsSub;//行标类型
//typedef Sub ColsSub;//列标类型
//typedef __int64 I64;
//_ASSERTE(_CrtCheckMemory());//内存检查

#endif
