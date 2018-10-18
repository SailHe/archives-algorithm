#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 63
#define ERRORHeap NULL
typedef struct HuffmanTreeNode *HuffmanTree;/*哈夫曼树*/
struct HuffmanTreeNode{
	int Weight;
	char c;//编码字符
	HuffmanTree Left, Right;
};
enum HeapType{MinHeap = -1,MAXHeap = 1};
typedef HuffmanTree ElementTypeHeap;
typedef	struct HeapStruct *Heap;/*最大或最小堆*/
struct HeapStruct{
	ElementTypeHeap *Data;/*储存堆元素的数组*/
	int Size;/*当前堆的大小*/
	int Capacity;/*堆容量*/
	HeapType Type;
};
int HuffmanCmp(const void* Tmin, const void* Tmax)
{
	return ((HuffmanTree)Tmax)->Weight - ((HuffmanTree)Tmin)->Weight;
}
int MaxHeapCmp(const void *min, const void *max)
{
	return (ElementTypeHeap*)min - (ElementTypeHeap*)max;
}
int MinHeapCmp(const void *min, const void *max)
{
	return (ElementTypeHeap*)max - (ElementTypeHeap*)min;
}
/*构建堆H的数组 返回数组元素个数*/
int GetHeapData(Heap H, int NUM0, int *Freq)
{/*从下标1开始存放数据 0存放哨兵*/
	int MaxSize = *Freq;
	int Size = sizeof(ElementTypeHeap)*(MaxSize + 1);/*申请元素数组*/
	H->Data = (ElementTypeHeap*)malloc(Size);
	for (int i = 0; i <= MaxSize; i++)/*申请数组的每个元素*/{
		H->Data[i] = (ElementTypeHeap)malloc(sizeof(HuffmanTreeNode));
		memset(H->Data[i], 0, sizeof(HuffmanTreeNode));
		H->Data[i]->Weight = Freq[i];
	}
	H->Data[0]->Weight = NUM0;/*哨兵*/
	return MaxSize;
}
/*创建0号元素值为NUM0容量为MaxSize的堆*/
Heap CreateHeap(int NUM0, int *Freq)
{
	Heap H = (Heap)malloc(sizeof(struct HeapStruct));
	H->Size = GetHeapData(H, NUM0, Freq);
	H->Capacity = Freq[0];
	return H;
}
bool ISFullHeap(Heap H)
{
	return H->Size == H->Capacity;
}
bool IsEmptyHeap(Heap H)
{
	return (H->Size == 0);
}
void InsertHeap(Heap H, ElementTypeHeap Item, int Cmp(const void *min, const void *max))
{
	int i;
	if (ISFullHeap(H)){
		puts(H->Type == MAXHeap ? "最大堆已满" : "最小堆已满");
		return;
	}
	/*i指向插入后堆中最后一个元素的位置*/
	i = ++H->Size;
	for (; Cmp(H->Data[i / 2], Item) < 0; i /= 2)
	//for (; H->Data[i / 2]->Weight > Item->Weight; i /= 2)
		H->Data[i] = H->Data[i / 2];/*向上过滤结点*/
	H->Data[i] = Item;/*插入*/
}
/*下滤函数 将H中以H->Data[p]为根的子堆调整为最大或最小或最小堆*/
void PercoDown(Heap H, int Start, int Cmp(const void *min, const void *max), char *Order)
{
	int Parent, Child;
	ElementTypeHeap X;
	/*下滤：取出自己          从自己开始找到一个合适的位置*/
	/*删除：取出尾元素 Size-- 从堆根开始*/
	int p = strcmp(Order, "删除") == 0 ? H->Size-- : Start;
	X = H->Data[p];/*取出需要下滤的值*/
	for (Parent = Start; 2 * Parent <= H->Size; Parent = Child){
		Child = 2*Parent;/*若左儿子==H->Size; 则右儿子不存在*/
		if (Child != H->Size && Cmp(H->Data[Child], H->Data[Child + 1]) < 0)/*注意三级指针*/
		//if (Child != H->Size && H->Data[Child]->Weight > H->Data[Child + 1]->Weight)
			Child++;/*选取左右儿子中大或小的一个*/
		if (Cmp(X, H->Data[Child]) >= 0)
		//if (X->Weight <= H->Data[Child]->Weight)
			break;
		else/*将孩子上移<==>将temp下移*/
			H->Data[Parent] = H->Data[Child];
	}
	H->Data[Parent] = X;
}
ElementTypeHeap DeleteHeap(Heap H, int Cmp(const void *min, const void *max))
{
	ElementTypeHeap Item;
	if (IsEmptyHeap(H)){
		puts(H->Type == MAXHeap ? "最大堆已空 无法删除" : "最小堆已空 无法删除");
		return ERRORHeap;
	}
	Item = H->Data[1];/*取出即将返回的值*/
	/*矛盾在于:Size--删除的是尾元素 但是根据堆的定义我们应该删除1号元素
	将尾元素替换下滤时的p元素下滤 这样便于操作 且实际上删除的是1号元素*/
	PercoDown(H, 1, Cmp, "删除");
	return Item;
}
/*将堆H构建为最大堆或最小堆 复杂度O(N) */
/* 堆排序对N个记录进行堆排序，最坏的情况下时间复杂度是O(NlogN) */
/*
找十个最小的，那你最简单的方法只要一个个比过去，用10N就好。
1万个数里面找最大的10个数，也就说只需要一个大小为10的最小堆就行了。
从第11个数开始，每次输入一个数，比堆顶大的话，就替换掉堆顶元素。
这样1w个数下来，最小堆中就保存了最大的10个数字。
复杂度是 1W * log（10）， 或者说 N log（k） 其中 k是一个远比N小的常数
而按照你最简单的方法，需要 1W * 10。也就是N*k
这里看常数部分复杂度就差了 2倍。
如果N=10w， k=1000， 复杂度就差了100倍
*/
void BuildHeadp(Heap H, int Cmp(const void *min, const void *max))
{/*对于一个以读入数据但需要调整的最大或最小或最小堆 从最后一个子节点的父节点开始调整到根节点1(自下而上建堆)*/
	for (int i = H->Size / 2; i > 0; i--){
		PercoDown(H, i, Cmp, "下滤");
	}
}


/*哈夫曼树的构建 最小堆实现*/
/*
没有度为1的结点
n个叶子结点的huffman树的节点数=2n-1
*/
/*频值向量Freq[0]代表权值个数 0<=Freq[i])是权值 i[1,*Freq O(N log(N))*/
HuffmanTree Huffman(int *Freq)
{
	int LEN = sizeof(struct HuffmanTreeNode);
	HuffmanTree T;
	Heap H = CreateHeap(-1, Freq);H->Type = MinHeap;
	BuildHeadp(H, HuffmanCmp);/*按权值构建为最小堆*/
	while ((*Freq)-- > 1){/*做H->Size-1次合并 每次将权值最小的两颗树合并 但是运算途中Size会变化*/
		T = (HuffmanTree)malloc(LEN);
		T->Left = DeleteHeap(H, HuffmanCmp);
		T->Right = DeleteHeap(H, HuffmanCmp);
		T->Weight = T->Left->Weight + T->Right->Weight;/*计算新权值*/
		InsertHeap(H, T, HuffmanCmp);
	}
	return DeleteHeap(H, HuffmanCmp);
}
//带权路径长度
int WPL(HuffmanTree T, int Depth)
{
	if (!T->Left && !T->Right)
		return Depth*T->Weight;
	else/*否则一定有两个孩子*/
		return WPL(T->Left, Depth + 1) + WPL(T->Right, Depth + 1);
}
bool JudgeCodeLen(int n, char s[/*n*/][2*N], int *Freq, int STDWPL)
{
	int CodeLen, i, len;
	for (CodeLen = i = 0; i < n; i++){
		len = strlen(s[i]);
		if ((len = strlen(s[i])) > n - 1)/*n结点最长编码长度为n-1*/
			return 0;
		CodeLen += len*Freq[i+1];
	}
	return CodeLen == STDWPL ? 1 : 0;
}
bool JudgeCodePre(int n, char s[/*n*/][2 * N])
{
	int i, j, len;
	int LEN = sizeof(struct HuffmanTreeNode);
	HuffmanTree BT = (HuffmanTree)malloc(LEN), T = BT;
	memset(T, 0, LEN);
	for (i = 0; i < n; i++){
		len = strlen(s[i]);
		T = BT;
		for (j = 0; j < len; j++){
			if (s[i][j] == '1'){
				if (!T->Left){
					T->Left = (HuffmanTree)malloc(LEN);
					memset(T->Left, 0, LEN);
				}
				else if (T->Weight)
						return 0;
				T = T->Left;
			}
			else{
				if (!T->Right){
					T->Right = (HuffmanTree)malloc(LEN);
					memset(T->Right, 0, LEN);
				}
				else if (T->Weight)
					return 0;
				T = T->Right;
			}
		}
		if (T->Left || T->Right)
			return 0;
		T->Weight = 1;
	}
	return 1;
}

//返回一个仅含大写英文字母和下划线的句子的哈夫曼编码所占内存,普通编码memory = sentence.length()*8
int huffmanMemory(string sentence){
	int freq[MAXN];
	memset(freq, 0, MAXN * 4);

	for (unsigned i = 0; i < sentence.length(); ++i){
		if (sentence[i] == '_')
			freq[26]++;
		else{
			freq[sentence[i] - 'A']++;
		}
	}
	priority_queue<int, vector<int>, greater<int> > q;

	for (int i = 0; i < MAXN; ++i){
		if (freq[i] != 0)
			q.push(freq[i]);
	}
	int tmp = 0, mem = 0;
	while (q.size() > 1){
		tmp = q.top(); q.pop();
		tmp += q.top();
		q.pop();
		q.push(tmp);
		mem += tmp;//其实就是把越小的频率反复多加几次，越大的频率少加几次
	}
	return mem == 0 ? sentence.length() : mem;
}
/**
int main()
{
	int CodeWPL;//标准
	int i, j, n, m, Freq[N + 1] = { 0 };
	HuffmanTree T;
	char ch[N+1], s[N][2*N];
	while (~scanf("%d\n", &n)){
		Freq[0] = n; memset(s, 0, N*N);
		memset(ch, 0, N + 1); ch[0] = 32;//初始化
		for(i = 1;i <= n;i++){
			ch[i] = getchar();
			scanf("%d", Freq + i);
			getchar();
		}
		T = Huffman(Freq);
		CodeWPL = WPL(T, 0);
		scanf("%d\n", &m);//学生数
		for (i = 0; i < m; i++){
			for (j = 0; j < n; j++){
				ch[0] = getchar();
				scanf("%s", s[j]);
				getchar();
			}
			puts(JudgeCodeLen(n, s, Freq, CodeWPL) && JudgeCodePre(n, s)? "Yes" : "No");
		}
	}return 0;
}
/*
小写字母，01反、且2点对换；有2点重合
7
A 1 B 1 C 1 D 3 E 3 F 6 G 6
1
A 00000
B 00001
C 0001
D 001
E 00
F 10
G 11

*/