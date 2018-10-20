#include "../Stack.h"

typedef int ElementTypeList;
typedef struct ArrayLinkedListNode *ArrayLinkedList;
struct ArrayLinkedListNode
{
	ElementTypeList Data;
	//用虚拟空间的下标代表地址
	int Next;
};
ArrayLinkedList CreatArrayLinkedList(int MAX_SIZE);
ArrayLinkedList AddressSpace = CreatArrayLinkedList(100005);
//创建并初始化
ArrayLinkedList CreatArrayLinkedList(int MAX_SIZE)
{
	int LEN_ARRAY_LINKED_LIST = sizeof(struct ArrayLinkedListNode);
	ArrayLinkedList arList = (ArrayLinkedList)malloc(LEN_ARRAY_LINKED_LIST*MAX_SIZE);
	memset(arList, -1, LEN_ARRAY_LINKED_LIST*MAX_SIZE);
	return arList;
}
/*将以root为根的数组链表的前k个元素逆转 subK: 被改变第k个元素的下标 返回新根节点下标*/
int ReversalListKInterval(int k, int *subK, int root, Stack<int> &subStack)
{
	int sub = 0;
	if (root == -1)
		return root;
	for (sub = root; k-- >= 0; sub = AddressSpace[sub].Next) {
		*subK = sub;
		if (sub == -1 && k >= 0)/*不足k个且不是最后一个*/
			return root;
	}
	for (sub = root; sub != *subK; sub = AddressSpace[sub].Next) {
		subStack.push(sub);
		root = sub;
	}
	for (sub = root; !subStack.isEmpty(); sub = AddressSpace[sub].Next)
	{
		AddressSpace[sub].Next = subStack.pop();
	}
	AddressSpace[sub].Next = *subK;/*保证当前的连通*/
	*subK = sub;
	return root;
}
/*
最大N,最后剩K-1不反转
00002 6 2
00000 2 00004
00001 4 00005
00002 1 00000
00003 6 -1
00004 3 00001
00005 5 00003
*/
int mainForRek() {
	int n, k;
	int root, sub, subK;
	Stack<int> subStack;
	while (~scanf("%d%d%d\n", &root, &n, &k)) {
		while (n-- > 0) {
			scanf("%d", &sub);
			scanf("%d%d", &AddressSpace[sub].Data, &AddressSpace[sub].Next);
		}
		puts("");
		root = ReversalListKInterval(k, &subK, root, subStack);
		while (subK != -1 && AddressSpace[subK].Next != -1) {
			sub = subK;
			AddressSpace[sub].Next = ReversalListKInterval(k, &subK, AddressSpace[subK].Next, subStack);
		}
		for (sub = root; sub != -1; sub = AddressSpace[sub].Next) {
			if (AddressSpace[sub].Next == -1)
				break;
			printf("%05d %d %05d\n", sub, AddressSpace[sub].Data, AddressSpace[sub].Next);
		}
		printf("%05d %d %d\n", sub, AddressSpace[sub].Data, AddressSpace[sub].Next);
	}
	return 0;
}
