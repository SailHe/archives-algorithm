#ifndef __TREE_H
#define __TREE_H
using namespace std;
#include <iostream>
#include <cmath>
#include <algorithm>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define MAX_ALPHA_COUNT 26/*�ֵ���*/
typedef struct AVLNode *Position;/**��ǰʹ�õ�**/
typedef Position BinTree;/* ���ж������ĳ����ַ(���ö���������������) */
typedef int ElementTypeTree;
/***********************************************************������(������)*********/
struct BTreeNode
{
	ElementTypeTree Data;
	BinTree Left;//NULL��ʾ������
	BinTree Right;
	int v;
};
const int LEN_BinTree = sizeof(struct BTreeNode);
/***********************************************************AVL��(������)*********/
typedef BinTree AVLTree;/* AVL������ ����ԵĲ�����*/
struct AVLNode{
	ElementTypeTree Data; /* ������� */
	BinTree Left;     /* ָ�������� */
	BinTree Right;    /* ָ�������� */
	int Height;       /* ���� */
	int v;//visit 1��ʾ�ѱ��� 0û��
};
const int LENAVL = sizeof(AVLNode);
/***********************************************************��̬������(������)*********/
typedef char ElementTypeTreeST;
typedef struct BTree_STNode *BinTree_ST;
struct BTree_STNode
{
	ElementTypeTreeST Data;
	int Left;//�±�
	int Right;
};// T1[SIZE], T2[SIZE];
const int LEN_TREE_ST = sizeof(struct BTree_STNode);
/***********************************���ĵײ����*/
typedef BinTree ElementTypeQueue;
/************************************���ĵײ���ջ*/
typedef BinTree ElementTypeStack;
typedef struct TrieNode * Trie;/*�ֵ�������*/
struct TrieNode
{
	Trie Next[MAX_ALPHA_COUNT];
	int v;/*�����ʾ��ĵ�����Ŀ*/
};
/**/
int Max(int a, int b);
/*������ ͨ�ò�����*/
BinTree CreatBinTree(int n);
BinTree Insert(BinTree BST, ElementTypeTree X);
BinTree Delete(BinTree BST, ElementTypeTree X);
Position Find(BinTree BST, ElementTypeTree X);
Position FindMin(BinTree BST);
Position FindMax(BinTree BST);
/*****************/
AVLTree AVLInsert(AVLTree T, ElementTypeTree X);

#endif