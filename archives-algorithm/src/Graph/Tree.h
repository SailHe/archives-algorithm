#ifndef __TREE_H
#define __TREE_H
using namespace std;
#include <iostream>
#include <cmath>
#include <algorithm>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define MAX 26/*字典树*/
#define ERRORTree -1
#define ERRORQueue NULL
#define ERRORStack NULL
typedef struct AVLNode *Position;/**当前使用的**/
typedef Position BinTree;/* 所有二叉树的抽象地址(共用二叉树基本操作集) */
typedef int ElementTypeTree;
/***********************************************************二叉树(二度树)*********/
struct BTreeNode
{
	ElementTypeTree Data;
	BinTree Left;//NULL表示无数据
	BinTree Right;
	int v;
};
const int LEN_BinTree = sizeof(struct BTreeNode);
/***********************************************************AVL树(二度树)*********/
typedef BinTree AVLTree;/* AVL树类型 针对性的操作集*/
struct AVLNode{
	ElementTypeTree Data; /* 结点数据 */
	BinTree Left;     /* 指向左子树 */
	BinTree Right;    /* 指向右子树 */
	int Height;       /* 树高 */
	int v;//visit 1表示已遍历 0没有
};
const int LENAVL = sizeof(AVLNode);
/***********************************************************静态二叉树(二度树)*********/
typedef char ElementTypeTreeST;
typedef struct BTree_STNode *BinTree_ST;
struct BTree_STNode
{
	ElementTypeTreeST Data;
	int Left;//下标
	int Right;
};// T1[SIZE], T2[SIZE];
const int LEN_TREE_ST = sizeof(struct BTree_STNode);
/***********************************树的底层队列*/
typedef BinTree ElementTypeQueue;
typedef struct Queue *PQueue;
struct QNode
{
	ElementTypeQueue Data;
	QNode *Next;
};
struct Queue
{
	QNode* Front;//头节点
	QNode* Rear;//尾结点
};
/************************************树的底层链栈*/
typedef BinTree ElementTypeStack;
typedef struct Stack *PStack;
struct Stack
{
	ElementTypeStack Data;
	PStack Next;
};
/**************************/
typedef struct TrieNode * Trie;/*字典树类型*/
struct TrieNode
{
	Trie Next[MAX];
	int v;/*经过词径的单词数目*/
};
/**/
int Max(int a, int b);
/*二叉树 通用操作集*/
BinTree CreatBinTree(int n);
BinTree Insert(BinTree BST, ElementTypeTree X);
BinTree Delete(BinTree BST, ElementTypeTree X);
Position Find(BinTree BST, ElementTypeTree X);
Position FindMin(BinTree BST);
Position FindMax(BinTree BST);
/*****************/
AVLTree AVLInsert(AVLTree T, ElementTypeTree X);

#endif