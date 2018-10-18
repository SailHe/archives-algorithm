/**********************************************************************树*******************************************/
/*
(01) 每个节点有零个或多个子节点；
(02) 没有父节点的节点称为根节点；
(03) 每一个非根节点有且只有一个父节点；
(04) 除了根节点外，每个子节点可以分为多个不相交的子树。
树的基本术语

若一个结点有子树，那么该结点称为子树根的"双亲"，子树的根是该结点的"孩子"。有相同双亲的结点互为"兄弟"。
一个结点的所有子树上的任何结点都是该结点的后裔。从根结点到某个结点的路径上的所有结点都是该结点的祖先。

结点的度：结点拥有的子树的数目。
叶子：度为零的结点。
分支结点：度不为零的结点。
树的度：树中结点的最大的度。

层次：根结点的层次为1，其余结点的层次等于该结点的双亲结点的层次加1。
树的高度：树中结点的最大层次。
无序树：如果树中结点的各子树之间的次序是不重要的，可以交换位置。
有序树：如果树中结点的各子树之间的次序是重要的, 不可以交换位置。
森林：0个或多个不相交的树组成。对森林加上一个根，森林即成为树；删去根，树即成为森林。
*/
#include "Tree.h"
int Max(int a, int b)
{
	return a > b ? a : b;
}
/***********************************树的底层队列*/
PQueue CreateQ()
{
	PQueue pq = (PQueue)malloc(sizeof(Queue));
	pq->Front = pq->Rear = NULL;
	return pq;
}
bool IsSemptyQu(PQueue PtrQ)
{
	return PtrQ->Front == NULL;
}
void AddQ(PQueue PtrQ, ElementTypeQueue job)
{
	QNode *New = (QNode*)malloc(sizeof(QNode));
	memset(New, 0, sizeof(QNode));
	New->Data = job;
	if (IsSemptyQu(PtrQ))
		PtrQ->Front = New;
	else
		PtrQ->Rear->Next = New;
	PtrQ->Rear = New;
}
ElementTypeQueue DeleteQ(PQueue PtrQ)
{
	QNode *FrontCell = NULL;
	ElementTypeQueue FrontItem = ERRORQueue;
	if (IsSemptyQu(PtrQ))
		puts("队列空");
	else
	{
		FrontCell = PtrQ->Front;
		if (PtrQ->Front == PtrQ->Rear)//只有一个元素
			PtrQ->Front = PtrQ->Rear = NULL;
		else
			PtrQ->Front = PtrQ->Front->Next;
		FrontItem = FrontCell->Data;
		free(FrontCell);
		FrontCell = NULL;
	}
	return FrontItem;
}
/************************************树的底层链栈*/
//创建一个连在PN之前的链栈结点 返回栈头
PStack ConnectP(PStack PN)
{
	PStack Ps = (PStack)malloc(sizeof(Stack));
	Ps->Next = PN;
	return Ps;
}
//创建并返回头式链栈的地址
PStack CreateStack()
{
	return ConnectP(NULL);
}
//压入
void Push(PStack Head, ElementTypeStack Item)
{
	Head->Next = ConnectP(Head->Next);
	Head->Next->Data = Item;
}
//判断链栈是否为空
bool IsSemptySt(PStack Head)
{
	return Head->Next == NULL;
}
//返回栈顶
ElementTypeStack pop(PStack Head)
{
	ElementTypeStack Item;
	PStack SNext = Head->Next;
	if (IsSemptySt(Head))
	{
		puts("堆栈空");
		return ERRORStack;
	}
	else
	{
		Item = Head->Next->Data;
		Head->Next = Head->Next->Next;
		free(SNext);
		SNext = NULL;
		return Item;
	}
}
/***********************************************************二叉树(二度树)*********/
/*
二叉树的性质

二叉树有以下几个性质：TODO(上标和下标)
性质1：二叉树第i层上的结点数目最多为 2^(i-1) (i≥1)。
性质2：深度为k的二叉树至多有2^k-1个结点(k≥1)。
性质3：包含n个结点的二叉树的高度至少为log2 (n+1)。
性质4：在任意一棵二叉树中，若终端结点(树叶,叶子)的个数为n0，度为2的结点数为n2，则n0=n2+1。
*/
//判断二叉树是否为空(基本不用)
bool ISEmptyTree(BinTree BT)
{
	return BT == NULL;
}
/*(队列实现)层序遍历---从上到下->从左到右*/
void LevelOrderTraversal_WithQueue(BinTree BT)
{
	if (!BT)return;
	BinTree T = BT;
	PQueue queue = CreateQ();
	AddQ(queue, T);
	while (!IsSemptyQu(queue))
	{
		T = DeleteQ(queue);
		printf("%d\n", T->Data);
		if (T->Left)AddQ(queue, T->Left);
		if (T->Right)AddQ(queue, T->Right);
	}
}
/*(递归实现)*******************二叉树的遍历*******/
//先序---子树根->左子树->右子树;
void PreOrderTraversal(BinTree BT)
{
	if (BT)
	{
		printf("%d\n", BT->Data);
		PreOrderTraversal(BT->Left);
		PreOrderTraversal(BT->Right);
	}
}
//中序---左子树->子树根->右子树;
void InOrderTraversal(BinTree BT)
{
	if (BT)
	{
		InOrderTraversal(BT->Left);
		printf("%d\n", BT->Data);
		InOrderTraversal(BT->Right);
	}
}
//后序---左子树->右子树->子树根;
void PostOrderTraversal(BinTree BT)
{
	if (BT)
	{
		PostOrderTraversal(BT->Left);
		PostOrderTraversal(BT->Right);
		//if(!(T->Left && T->right))//输出叶节点
		printf("%d\n", BT->Data);
	}
}
//返回树高(深)：(根据公式 Height = max(Hl, Hr) + 1 由后序遍历改编实现)
int Deep(BinTree BT){
	int HL, HR;
	if (BT){
		HL = Deep(BT->Left);
		HR = Deep(BT->Right);
		return (HL > HR ? HL : HR) + 1;
	}
	else
		return 0;//空树深度为零
}
/*(堆栈实现)*******************二叉树的中序遍历********/
void InOrderTraversal_WithStack(BinTree BT)
{
	BinTree T = BT;
	PStack S = CreateStack();//初始化并创建堆栈
	while (T || !IsSemptySt(S))
	{
		while (T)//将沿途遇到的所有结点压栈 并去遍历其左子树
		{
			Push(S, T);//①
			T = T->Left;
		}
		if (!IsSemptySt(S))//当一个左子树遍历完毕后将结点弹出，打印并去遍历其右子树
		{
			T = pop(S);//②将节点弹出并访问结点
			printf("%d\n", T->Data);
			T = T->Right;//转向右子树
		}
	}
}
/*将先序与中序遍历数组 翻译为后序遍历数组*对应遍历数组*当前子树的元素个数*/
void OrderTranslation(int *pre, int *in, int *post, int n)
{
	int Ln;/*左子子树长度*/
	if (n == 0)return;
	post[n - 1] = *pre;/*转化关系:先序遍历数组的首元素就是子树根*/
	for (Ln = 0; Ln < n && in[Ln] != *pre; Ln++);
	OrderTranslation(pre + 1, in, post, Ln);/*先序遍历数组向左子树遍历一个元素 其余数组保持不变*/
	OrderTranslation(pre + Ln + 1, in + Ln + 1, post + Ln, n - Ln - 1);/*向右子树遍历一个元素*/
	/*[1] 2  3  4  5  6
	3  2  4 [1] 6  5
	3  4  2  6  5 [1]*/
}
/******************************************************************************二叉查找树基本操作集************/
/*二叉搜索（查找 排序 判定）树*/
/*
性质：
①非空 左子树的所有键值 小于其根节点的键值
②非空 右子树的所有键值 大于其根结点的键值
③左右子树都是二叉搜索树
④没有键值相等的节点
最大元素：最右分支（一直BST = BST->Right）即可
最小元素：最左分支

n个结点的判定树deep = [log2(n)]+1
判定树上每个结点的查找次数==所在层数
查找成功次数<=deep
4层满二叉树的平均查找次数ASL = (4*8+3*4+2*2+1*1)/15
*/
/*创建元素个数为n的二叉查找树*/
BinTree CreatBinTree(int n)
{
	int i, x;
	BinTree BST = NULL;
	for (i = 0; i<n; i++)
	{
		scanf("%d", &x);
		BST = Insert(BST, x);
	}
	return BST;
}
/*函数Insert将X插入二叉搜索树BST并返回结果树的根结点指针；*/
BinTree Insert(BinTree BST, ElementTypeTree X)
{
	if (!BST)
	{/*子树为空 生成并返回一个结点*/
		BST = (BinTree)malloc(LEN_BinTree);
		memset(BST, 0, LEN_BinTree);
		BST->Data = X;
		//BST->Left = BST->Right = NULL;
	}
	else if (X < BST->Data)
		BST->Left = Insert(BST->Left, X);
	else if (X > BST->Data)
		BST->Right = Insert(BST->Right, X);
	else puts("X已存在 无法插入");
	return BST;
}
/*函数Delete将X从二叉搜索树BST中删除，并返回结果树的根结点指针；如果X不在树中，则打印一行Not Found并返回原树的根结点指针；*/
BinTree Delete(BinTree BST, ElementTypeTree X)
{
	Position Del = BST;
	if (!BST)
		puts("Not Found");
	else if (X < BST->Data)
		BST->Left = Delete(BST->Left, X);/*左子树根节点可能会发生变化*/
	else if (X > BST->Data)
		BST->Right = Delete(BST->Right, X);
	/*找到要删除元素 进行删除*/
	else if (BST->Left && BST->Right)
	{
		/*用右子树中的最小(或左子树最大)元素填充需删除的结点*/
		Del = FindMin(BST->Right);
		BST->Data = Del->Data;
		/*删除右子树的最小元素*/
		BST->Right = Delete(BST->Right, Del->Data);
	}
	else {/*只有一个结点或无结点*/
		if (!BST->Left)
			BST = BST->Right;
		else
			BST = BST->Left;
		free(Del);
		Del = NULL;
	}
	return BST;
}
/*函数Find在二叉搜索树BST中找到X，返回该结点的指针；如果找不到则返回空指针；*/
Position Find(BinTree BST, ElementTypeTree X)
{
	while (BST)
	{
		//if (BST->v == 0 && BST->Data != X)/*全局变量isfind初始化为1,用于判断是否是同一颗二叉树*/
			//isfind = 0;
		if (X > BST->Data)
			BST = BST->Right;/*向右子树移动继续查找*/
		else if (X < BST->Data)
			BST = BST->Left;
		else
			return  BST->v = 1, BST;
	}
	return NULL;
}
/*函数FindMin返回二叉搜索树BST中最小元结点的指针；*/
Position FindMin(BinTree BST)
{
	if (!BST)/*基本定义：树为空返回NULL*/
		return NULL;
	else if (!BST->Left)//else if(!FindMin(BST->Left))
		return BST;/*基本定义：左子叶结点是最小元素*/
	else
		return FindMin(BST->Left);/*递归定义：不是左子叶时进入左子树继续查找*/
}
/*函数FindMax返回二叉搜索树BST中最大元结点的指针。*/
Position FindMax(BinTree BST)
{
	while (BST && BST->Right)
		BST = BST->Right;
	return BST;
}

void ClearVisit(BinTree BST)
{
	if (BST && BST->v == 1){
		BST->v = 0;
		ClearVisit(BST->Left);
		ClearVisit(BST->Right);
	}
}
/*(静态链表:动态结构体数组实现)**********************************************************二叉树的同构**********/
BinTree_ST T1, T2;
int Min(int a, int b)
{
	return a < b ? a : b;
}
//创建并初始化二叉树
BinTree_ST CreatBinTree_ST(int MAX_size)
{
	BinTree_ST BT = (BinTree_ST)malloc(LEN_TREE_ST*MAX_size);
	memset(BT, 0, LEN_TREE_ST*MAX_size);//因为数据的使用情况(%c 1byte)与数据的表示情况(int 4byte)不符 若不初始化 读取数据会出问题
	return BT;
}
//获取树的数据 返回根节点下标
int GetData(BinTree_ST T, int MAX_size)
{
	int i, sum = (MAX_size - 1)*MAX_size / 2;
	while ((i = getchar()) == 10 || i == 32);
	ungetc(i, stdin);
	for (i = 0; i < MAX_size; i++)
	{
		scanf("%c %c %c", &T[i].Data, &T[i].Left, &T[i].Right);
		if (T[i].Left == '-')
			T[i].Left = -1;
		else
		{
			T[i].Left -= '0';
			sum -= T[i].Left;
		}

		if (T[i].Right == '-')
			T[i].Right = -1;
		else
		{
			T[i].Right -= '0';
			sum -= T[i].Right;
		}
		getchar();
	}
	return MAX_size == 0 ? -1 : sum;
}
//判断是否T1与T2是否同构 R是根所在下标
bool Isomorphism(int R1, int R2)
{
	if ((R1 == -1 && R2 != -1) || (R1 != -1 && R2 == -1))
		return false;
	else if (R1 == -1 && R2 == -1)
		return true;
	else/*两颗树都不为空*/
	{
		if (T1[R1].Data != T2[R2].Data)
			return false;
		else/*当前数据相等*/
		{	/*按当前顺序尝试匹配左右子树*/
			if (Isomorphism(T1[R1].Left, T2[R2].Left))
				return Isomorphism(T1[R1].Right, T2[R2].Right);
			else/*否则转换左右子树匹配*/
			{
				if (Isomorphism(T1[R1].Left, T2[R2].Right))
					return Isomorphism(T1[R1].Right, T2[R2].Left);
				else
					return 0;
			}
		}
	}
}
/*静态(队列实现 队列的基本类型改为下标类型int即可)层序遍历---从上到下->从左到右*/
void LevelOrderTraversal_ST(int root)
{
	if (root == -1)return;
	BinTree_ST BT = T;
	PQueue queue = CreateQ();
	int sub = root, PR = 0;
	AddQ(queue, sub);
	while (!IsSemptyQu(queue))
	{
		sub = DeleteQ(queue);
		//printf("%d\n", BT[sub].Data);
		if (BT[sub].Left == -1 && BT[sub].Right == -1)
		{
			printf(PR++ ? " " : "");
			printf("%d", sub);
		}
		if (BT[sub].Left != -1)AddQ(queue, BT[sub].Left);
		if (BT[sub].Right != -1)AddQ(queue, BT[sub].Right);
	}
	puts("");
}
/*完全二叉树的层序遍历就是顺序遍历*/
/*返回左子树规模 结点数*/
int GetScaleL(double n/*总节点数*/)
{/*2^h - 1 + x = n*/
	int h = (int)log2(n + 1);/*向下取整*/
	int x = n + 1 - pow(2.0, h);/*最下层单出的结点数*/
	return pow(2.0, h - 1) - 1 + Min(x, pow(2.0, h - 1));
}
/*将有序序列A转化为完全二叉搜索树存进数组T中*/
void CompleteBinTree(int * T, int *ALeft, int *ARight, int TRoot)
{/*solve(A, A+n - 1, 0);*/
	int n = ARight - ALeft + 1;
	if (n == 0)return;
	int nl = GetScaleL(n);
	T[TRoot] = ALeft[nl];/*由0开始数nl个即是子树跟*/
	CompleteBinTree(T, ALeft, ALeft + nl - 1, TRoot * 2 + 1);/*左儿子下标：子树从0开始编号2i+1*/
	CompleteBinTree(T, ALeft + nl + 1, ARight, (TRoot * 2 + 1) + 1);
	/*
	0 1 2 [[3 4 5 [6] 7 8]] 9
	*/
}
/*
int main()
{
	int n1, n2, R1, R2;
	scanf("%d\n", &n1);
	T1 = CreatBinTree_ST(n1);
	R1 = GetData(T1, n1);

	scanf("%d\n", &n2);
	T2 = CreatBinTree_ST(n2);
	R2 = GetData(T2, n2);
	puts(Isomorphism(R1, R2) ? "Yes" : "No");
	return 0;
}
/**/
/****************************************************************************************字典树*****************/
TrieNode root;
void TrieInsert(Trie p, char *str)
{
	int i, id;
	for (i = 0; str[i]; i++)
	{
		id = str[i] - 'a';
		if (p->Next[id] == NULL)
		{
			p->Next[id] = (Trie)malloc(sizeof(TrieNode));
			memset(p->Next[id], 0, sizeof(TrieNode));
		}
		p->Next[id]->v++;/*该词径下有单词str[i]经过*/
		p = p->Next[id];
	}
}
int TrieFind(Trie p, char *str)
{
	int id, i;
	for (i = 0; str[i]; i++)
	{
		id = str[i] - 'a';
		p = p->Next[id];
		if (!p)return 0;
	}
	return p->v;/*子串str遍历完毕 返回v*/
}
/*BUG版*/
void FreeTrie(Trie p)
{
	if (p != NULL)
	{
		for (int i = 0; i < MAX; i++)
		{
			FreeTrie(p->Next[i]);
		}
		free(p);
		p = NULL;
	}
}
/******************************************************************************************AVL树******************/
/*
性质：
n(h) = n(h-1)+n(h-2)+1;
其与斐波那契列类似，而斐波那契列F(i)是一个指数函数
平衡树的高度O log2(n)
平衡因子BF(T) = h(L) - h(R);子树高度差
*/
/**
#include "C:\Users\魔诃不思议\Documents\One more time  One more chance\C\实用函数\库\树.cpp"
typedef BinTree AVLTree;//所有二叉树的抽象地址
/**/
int GetHeight(AVLTree T)
{
	if (!T)
		return 0;
	else if (T->Left && T->Right)
		return Max(T->Left->Height, T->Right->Height) + 1;
	else if (T->Left)
		return T->Left->Height + 1;
	else if (T->Right)
		return T->Right->Height + 1;
	else
		return 1;
	/*用下一层来定义这一层 效率提高特别大*/
	//return T ? Max(GetHeight(T->Left), GetHeight(T->Right)) + 1 : 0;
}
/*左左侧右旋:A与A的左子节点B做单旋 更新树高 返回新根B 指的是不平衡的起源在结点A左侧的左侧*/
AVLTree LeftRotation(AVLTree A)
{
	AVLTree B = A->Left;
	A->Left = B->Right;
	B->Right = A;
	A->Height = Max(GetHeight(A->Left), GetHeight(A->Right)) + 1;
	B->Height = Max(GetHeight(B->Left), A->Height) + 1;
	return B;
}
/*右右侧左旋:A与A的右子节点B做单旋 更新树高 返回新根B*/
AVLTree RightRotation(AVLTree A)
{
	AVLTree B = A->Right;
	A->Right = B->Left;
	B->Left = A;
	A->Height = Max(GetHeight(A->Left), GetHeight(A->Right)) + 1;
	B->Height = Max(GetHeight(B->Right), A->Height) + 1;
	return B;
}
/*左右双旋 先右后左*/
AVLTree LeftRightRotation(AVLTree A)
{
	A->Left = RightRotation(A->Left);/*将B右右单旋返回C与连在A的左侧*/
	return LeftRotation(A);/*将A与C左左单旋返回C*/
}
/*右左双旋 先左后右*/
AVLTree RighLefttRotation(AVLTree A)
{
	A->Right = LeftRotation(A->Right);/*将B左左单旋返回C与连在A的右侧*/
	return RightRotation(A);/*将A与C右右单旋返回C*/
}
/*将x插入平衡树 并返回调整后的平衡树*/
AVLTree AVLInsert(AVLTree T, ElementTypeTree X)
{
	if (!T){/*若插入空树 则新建一个包含结点的树*/
		T = (AVLTree)malloc(LENAVL);
		memset(T, 0, LENAVL);
		T->Data = X;
	}
	else if (X < T->Data){/*往左子树插入*/
		T->Left = AVLInsert(T->Left, X);/*T的平衡因子BF*/
		if (GetHeight(T->Left) - GetHeight(T->Right) == 2){
			if (X < T->Left->Data)
				T = LeftRotation(T);
			else
				T = LeftRightRotation(T);
		}
	}
	else if (X > T->Data){
		T->Right = AVLInsert(T->Right, X);
		if (GetHeight(T->Left) - GetHeight(T->Right) == -2){
			if (X > T->Right->Data)
				T = RightRotation(T);/*右单旋*/
			else
				T = RighLefttRotation(T);
		}
	}
	/*else X==T->Data 无需插入*/
	/*更新树高*/
	T->Height = Max(GetHeight(T->Left), GetHeight(T->Right)) + 1;
	return T;
}
