#include "stdafx.h"
#include "LinkedList.h"

int main() {
	LinkedList list1 = LinkedList();
	LinkedList list2 = LinkedList();
	LinkedList list3 = LinkedList();
	int n;
	puts("���������Ԫ�� ��-1��β");
	list1.input().output();
	list2.input().output();
	puts("begain\n");

	puts("��ת��list2\n");
	list2.reverse().output();

	puts("�ϲ�list1 list2\n");
	list1.merge(list2).output();
	LinkedList::Iterator iter = list1.findKth(1);
	n = list1.length();
	int i = 0;
	while (iter->next) {
		++i;

		LinkedList::Iterator findIter = list1.findKth(i);
		if (findIter != NULL) {
			list3.insertInP(findIter->Data, findIter);
			printf("find ������%d��Ԫ��%d\n", i, findIter->Data);
		}
		else {
			puts("find error");
		}

		LinkedList::Iterator findIterRe = list1.findKth(-1);
		if (findIterRe != NULL) {
			printf("find ������%d��Ԫ��%d\n", 1, findIterRe->Data);
		}
		else {
			puts("find error");
		}

		LinkedList::ElementType deleteTemp = findIterRe->Data;
		LinkedList::Iterator findDataPo = list1.findData(deleteTemp);
		if (list1.deleteInP(findDataPo) != NULL) {
			printf("delete %d\n", deleteTemp);
			list1.output();
		}
		else {
			puts("delete error");
		}
	}
	return 0;
}
