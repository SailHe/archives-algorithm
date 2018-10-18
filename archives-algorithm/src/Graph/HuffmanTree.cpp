#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 63
#define ERRORHeap NULL
typedef struct HuffmanTreeNode *HuffmanTree;/*��������*/
struct HuffmanTreeNode{
	int Weight;
	char c;//�����ַ�
	HuffmanTree Left, Right;
};
enum HeapType{MinHeap = -1,MAXHeap = 1};
typedef HuffmanTree ElementTypeHeap;
typedef	struct HeapStruct *Heap;/*������С��*/
struct HeapStruct{
	ElementTypeHeap *Data;/*�����Ԫ�ص�����*/
	int Size;/*��ǰ�ѵĴ�С*/
	int Capacity;/*������*/
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
/*������H������ ��������Ԫ�ظ���*/
int GetHeapData(Heap H, int NUM0, int *Freq)
{/*���±�1��ʼ������� 0����ڱ�*/
	int MaxSize = *Freq;
	int Size = sizeof(ElementTypeHeap)*(MaxSize + 1);/*����Ԫ������*/
	H->Data = (ElementTypeHeap*)malloc(Size);
	for (int i = 0; i <= MaxSize; i++)/*���������ÿ��Ԫ��*/{
		H->Data[i] = (ElementTypeHeap)malloc(sizeof(HuffmanTreeNode));
		memset(H->Data[i], 0, sizeof(HuffmanTreeNode));
		H->Data[i]->Weight = Freq[i];
	}
	H->Data[0]->Weight = NUM0;/*�ڱ�*/
	return MaxSize;
}
/*����0��Ԫ��ֵΪNUM0����ΪMaxSize�Ķ�*/
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
		puts(H->Type == MAXHeap ? "��������" : "��С������");
		return;
	}
	/*iָ������������һ��Ԫ�ص�λ��*/
	i = ++H->Size;
	for (; Cmp(H->Data[i / 2], Item) < 0; i /= 2)
	//for (; H->Data[i / 2]->Weight > Item->Weight; i /= 2)
		H->Data[i] = H->Data[i / 2];/*���Ϲ��˽��*/
	H->Data[i] = Item;/*����*/
}
/*���˺��� ��H����H->Data[p]Ϊ�����Ӷѵ���Ϊ������С����С��*/
void PercoDown(Heap H, int Start, int Cmp(const void *min, const void *max), char *Order)
{
	int Parent, Child;
	ElementTypeHeap X;
	/*���ˣ�ȡ���Լ�          ���Լ���ʼ�ҵ�һ�����ʵ�λ��*/
	/*ɾ����ȡ��βԪ�� Size-- �ӶѸ���ʼ*/
	int p = strcmp(Order, "ɾ��") == 0 ? H->Size-- : Start;
	X = H->Data[p];/*ȡ����Ҫ���˵�ֵ*/
	for (Parent = Start; 2 * Parent <= H->Size; Parent = Child){
		Child = 2*Parent;/*�������==H->Size; ���Ҷ��Ӳ�����*/
		if (Child != H->Size && Cmp(H->Data[Child], H->Data[Child + 1]) < 0)/*ע������ָ��*/
		//if (Child != H->Size && H->Data[Child]->Weight > H->Data[Child + 1]->Weight)
			Child++;/*ѡȡ���Ҷ����д��С��һ��*/
		if (Cmp(X, H->Data[Child]) >= 0)
		//if (X->Weight <= H->Data[Child]->Weight)
			break;
		else/*����������<==>��temp����*/
			H->Data[Parent] = H->Data[Child];
	}
	H->Data[Parent] = X;
}
ElementTypeHeap DeleteHeap(Heap H, int Cmp(const void *min, const void *max))
{
	ElementTypeHeap Item;
	if (IsEmptyHeap(H)){
		puts(H->Type == MAXHeap ? "�����ѿ� �޷�ɾ��" : "��С���ѿ� �޷�ɾ��");
		return ERRORHeap;
	}
	Item = H->Data[1];/*ȡ���������ص�ֵ*/
	/*ì������:Size--ɾ������βԪ�� ���Ǹ��ݶѵĶ�������Ӧ��ɾ��1��Ԫ��
	��βԪ���滻����ʱ��pԪ������ �������ڲ��� ��ʵ����ɾ������1��Ԫ��*/
	PercoDown(H, 1, Cmp, "ɾ��");
	return Item;
}
/*����H����Ϊ���ѻ���С�� ���Ӷ�O(N) */
/* �������N����¼���ж�������������ʱ�临�Ӷ���O(NlogN) */
/*
��ʮ����С�ģ�������򵥵ķ���ֻҪһ�����ȹ�ȥ����10N�ͺá�
1���������������10������Ҳ��˵ֻ��Ҫһ����СΪ10����С�Ѿ����ˡ�
�ӵ�11������ʼ��ÿ������һ�������ȶѶ���Ļ������滻���Ѷ�Ԫ�ء�
����1w������������С���оͱ���������10�����֡�
���Ӷ��� 1W * log��10���� ����˵ N log��k�� ���� k��һ��Զ��NС�ĳ���
����������򵥵ķ�������Ҫ 1W * 10��Ҳ����N*k
���￴�������ָ��ӶȾͲ��� 2����
���N=10w�� k=1000�� ���ӶȾͲ���100��
*/
void BuildHeadp(Heap H, int Cmp(const void *min, const void *max))
{/*����һ���Զ������ݵ���Ҫ������������С����С�� �����һ���ӽڵ�ĸ��ڵ㿪ʼ���������ڵ�1(���¶��Ͻ���)*/
	for (int i = H->Size / 2; i > 0; i--){
		PercoDown(H, i, Cmp, "����");
	}
}


/*���������Ĺ��� ��С��ʵ��*/
/*
û�ж�Ϊ1�Ľ��
n��Ҷ�ӽ���huffman���Ľڵ���=2n-1
*/
/*Ƶֵ����Freq[0]����Ȩֵ���� 0<=Freq[i])��Ȩֵ i[1,*Freq O(N log(N))*/
HuffmanTree Huffman(int *Freq)
{
	int LEN = sizeof(struct HuffmanTreeNode);
	HuffmanTree T;
	Heap H = CreateHeap(-1, Freq);H->Type = MinHeap;
	BuildHeadp(H, HuffmanCmp);/*��Ȩֵ����Ϊ��С��*/
	while ((*Freq)-- > 1){/*��H->Size-1�κϲ� ÿ�ν�Ȩֵ��С���������ϲ� ��������;��Size��仯*/
		T = (HuffmanTree)malloc(LEN);
		T->Left = DeleteHeap(H, HuffmanCmp);
		T->Right = DeleteHeap(H, HuffmanCmp);
		T->Weight = T->Left->Weight + T->Right->Weight;/*������Ȩֵ*/
		InsertHeap(H, T, HuffmanCmp);
	}
	return DeleteHeap(H, HuffmanCmp);
}
//��Ȩ·������
int WPL(HuffmanTree T, int Depth)
{
	if (!T->Left && !T->Right)
		return Depth*T->Weight;
	else/*����һ������������*/
		return WPL(T->Left, Depth + 1) + WPL(T->Right, Depth + 1);
}
bool JudgeCodeLen(int n, char s[/*n*/][2*N], int *Freq, int STDWPL)
{
	int CodeLen, i, len;
	for (CodeLen = i = 0; i < n; i++){
		len = strlen(s[i]);
		if ((len = strlen(s[i])) > n - 1)/*n�������볤��Ϊn-1*/
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

//����һ��������дӢ����ĸ���»��ߵľ��ӵĹ�����������ռ�ڴ�,��ͨ����memory = sentence.length()*8
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
		mem += tmp;//��ʵ���ǰ�ԽС��Ƶ�ʷ�����Ӽ��Σ�Խ���Ƶ���ټӼ���
	}
	return mem == 0 ? sentence.length() : mem;
}
/**
int main()
{
	int CodeWPL;//��׼
	int i, j, n, m, Freq[N + 1] = { 0 };
	HuffmanTree T;
	char ch[N+1], s[N][2*N];
	while (~scanf("%d\n", &n)){
		Freq[0] = n; memset(s, 0, N*N);
		memset(ch, 0, N + 1); ch[0] = 32;//��ʼ��
		for(i = 1;i <= n;i++){
			ch[i] = getchar();
			scanf("%d", Freq + i);
			getchar();
		}
		T = Huffman(Freq);
		CodeWPL = WPL(T, 0);
		scanf("%d\n", &m);//ѧ����
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
Сд��ĸ��01������2��Ի�����2���غ�
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