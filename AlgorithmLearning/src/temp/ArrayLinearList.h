#pragma once
#include <stdio.h>
#include <stdlib.h>

/*
顺序表操作 ArrayListSequence
线性表是最基本、最简单、也是最常用的一种数据结构。
线性表（linear list）是数据结构的一种，一个线性表是n个具有相同特性的数据元素的有限序列。

顺序表是在计算机内存中以数组的形式保存的线性表，是指用一组地址连续的存储单元依次存储数据元素的线性结构。
*/
namespace LinearList {
#define ARRAY_LINEAR_LIST_MAX_SIZE 7
#define ARRAY_LINEAR_LIST_ERROR_POSITION -1
	typedef int ElementType;
	typedef int Position;
	typedef struct LNode *List;
	struct LNode {
		ElementType Data[ARRAY_LINEAR_LIST_MAX_SIZE];
		// 保存线性表中最后一个元素的位置
		Position Last;
	};

	List MakeEmpty();

	Position Find(List L, ElementType X);

	bool Insert(List L, ElementType X, Position P);

	bool Delete(List L, Position P);

	int mainForArrayLinearList();
}