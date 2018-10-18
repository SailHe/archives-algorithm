#pragma once
#include "stdafx.h"

class LinkedList {
public:
	using ElementType = int;
	const ElementType ERROR_ELE_VALUE = -1;
	//typedef struct Node *PtrToLinkedListNode;
	struct Node{
		ElementType Data;
		Node *next;
	};
	using Iterator = Node*;

private:
	Iterator headNode_;
public:
	LinkedList();
	LinkedList(LinkedList const &rhs);
	LinkedList(LinkedList const &&rvalue);
	~LinkedList();

	//read 返回值用于连锁
	LinkedList &input();
	//write
	LinkedList &output();
	/***********************************************递增链表操作**************/
	/*
	结点不变 原表头置零
	*/
	/***********************************************递增链表操作************下**/
	//返回L1 L2原序合并后的链表头节点
	LinkedList &merge(LinkedList &rhs);

	//list1和list2是两个按Data升序链接的链表的头指针；(头指针不是头节点)
	//函数Mergelists将两个链表合并成一个按Data升序链接的新链表，并返回结果链表的头指针。
	LinkedList &mergelists(LinkedList &rhs);

	//返回L1 与L2的交集的头节点
	Iterator intersection(LinkedList &rhs);
	/*******************************************链表的查找与数据操作********上**/
	/*
	原链表完全不变
	*/
	/*******************************************链表的查找与数据操作*********/
	//返回链表储存数据结点个数+1
	int length();
	//返回链表第k条数据  不存在返回ERROR_ELE_VALUE :: k属于(-length, length)
	Iterator findKth(int k);
	//返回线性表中首次出现ele的位置 找不到返回NULL
	Iterator findData(ElementType ele);
	//将ele插入L，并保持该序列的有序性，返回插入后的链表头
	bool insertData(ElementType ele);
	//在位置iter前插入ele  返回链表头 若参数iter位置非法返回NULL   自带虚拟头节点 即使传入空结点 NLLL 也可以顺利插入
	bool insertInP(ElementType ele, Iterator iter);
	//删除位置iter的元素 返回链表头 若参数iter位置非法返回NULL
	bool deleteInP(Iterator &iter);
	//删除链表 所有特定数据
	int deleteAll(ElementType ele);
	//链表逆置  原链表不变 完全新建 返回新建头节点
	LinkedList &reverse();
};
