#pragma once
#include <stdio.h>
#include <stdlib.h>

/*
顺序表操作 ArrayListSequence
线性表是最基本、最简单、也是最常用的一种数据结构。
线性表（linear list）是数据结构的一种，一个线性表是n个具有相同特性的数据元素的有限序列。

顺序表是在计算机内存中以数组的形式保存的线性表，是指用一组地址连续的存储单元依次存储数据元素的线性结构。

示例程序: 不支持模板(就算只有int也能做许多事), 不支持自定义大小
*/
namespace LinearList {
	#define ARRAY_LINEAR_LIST_MAX_SIZE 7
	#define ARRAY_LINEAR_LIST_ERROR_POSITION -1
	typedef int ElementType;
	typedef int Position;
	typedef struct ArrayListNode *ArrayLinearList;
	struct ArrayListNode {
		ElementType Data[ARRAY_LINEAR_LIST_MAX_SIZE];
		// 保存线性表中最后一个元素的位置
		Position Last;
	};

	// 创建并返回一个空的线性表;
	ArrayLinearList MakeEmpty();

	// 返回线性表中X的位置。若找不到则返回ARRAY_LINEAR_LIST_ERROR_POSITION;
	Position Find(ArrayLinearList L, ElementType X);

	// 将X插入在位置P并返回true;
	// 若空间已满，则打印“FULL”并返回false;
	// 如果参数P指向非法位置，则打印“ILLEGAL POSITION”并返回false;
	bool Insert(ArrayLinearList L, ElementType X, Position P);

	// 将位置P的元素删除并返回true。
	// 若参数P指向非法位置，则打印“POSITION P EMPTY”（其中P是参数值）并返回false。
	bool Delete(ArrayLinearList L, Position P);

	// 删除所有值大于min而且小于max的元素(min, max)。删除后表中剩余元素保持顺序存储，并且相对位置不能改变。
	int DeleteAll(int A[], int L, int minA, int maxA);

	int mainForArrayLinearList();

}