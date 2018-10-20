#include "../Stack.h"

typedef int ElementTypeList;
typedef struct ArrayLinkedListNode *ArrayLinkedList;
struct ArrayLinkedListNode
{
	ElementTypeList Data;
	//������ռ���±�����ַ
	int Next;
};
ArrayLinkedList CreatArrayLinkedList(int MAX_SIZE);
ArrayLinkedList AddressSpace = CreatArrayLinkedList(100005);
//��������ʼ��
ArrayLinkedList CreatArrayLinkedList(int MAX_SIZE)
{
	int LEN_ARRAY_LINKED_LIST = sizeof(struct ArrayLinkedListNode);
	ArrayLinkedList arList = (ArrayLinkedList)malloc(LEN_ARRAY_LINKED_LIST*MAX_SIZE);
	memset(arList, -1, LEN_ARRAY_LINKED_LIST*MAX_SIZE);
	return arList;
}
/*����rootΪ�������������ǰk��Ԫ����ת subK: ���ı��k��Ԫ�ص��±� �����¸��ڵ��±�*/
int ReversalListKInterval(int k, int *subK, int root, Stack<int> &subStack)
{
	int sub = 0;
	if (root == -1)
		return root;
	for (sub = root; k-- >= 0; sub = AddressSpace[sub].Next) {
		*subK = sub;
		if (sub == -1 && k >= 0)/*����k���Ҳ������һ��*/
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
	AddressSpace[sub].Next = *subK;/*��֤��ǰ����ͨ*/
	*subK = sub;
	return root;
}
/*
���N,���ʣK-1����ת
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
