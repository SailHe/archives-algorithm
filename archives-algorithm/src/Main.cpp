#pragma warning(disable:4996)
#include "stdafx.h"
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "Standard.h"
using namespace StandardExtend;

//Demo

int MainForStack() {
	LinkedList<string> lists = LinkedList<string>();
	for (int i = 0; i < 1; ++i) {
		lists.insertData("LinkedListData");
	}
	Stack<int> s;
	for (int i = 0; i < 10; ++i) {
		s.push(i);
	}
	while (!s.isEmpty()) {
		printf("%d\n", s.pop());
	}
	
	Stack<string> ss;
	for (int i = 0; i < 10; ++i) {
		ss.push(string("StackData"));
	}
	while (!ss.isEmpty()) {
		cout << ss.pop() << endl;
	}
	puts("Stack test end");
	return 0;
}

int MainForLinkedList() {

	LinkedList<string> lists = LinkedList<string>();
	for (int i = 0; i < 10; ++i) {
		lists.insertData("LinkedListData");
	}
	auto it = lists.findKth(1);
	while (it) {
		cout << *it << endl;
		++it;
	}
	
	LinkedList<stack<string>> stackList;
	for (int i = 0; i < 10; ++i) {
		stack<string> s;
		for (int i = 0; i < 2; ++i) {
			s.push("Stack data in list");
		}
		stackList.insertData(s);
	}
	auto it2 = stackList.findKth(1);
	StandardExtend::iterate(it2, stackList.end(), [&](auto it) {
		auto s = *it;
		while (!s.empty()) {
			cout << s.top() << endl;
			s.pop();
		}
	});

	LinkedList<int> list1 = LinkedList<int>();
	LinkedList<int> list2 = LinkedList<int>();
	LinkedList<int> list3 = LinkedList<int>();
	int n;
	puts("���������Ԫ�� ��-1��β");
	list1.input().output();
	list2.input().output();
	puts("begain\n");

	puts("��ת��list2\n");
	list2.reverse().output();

	puts("�ϲ�list1 list2\n");
	list1.merge(list2).output();
	LinkedList<int>::Iterator iter = list1.findKth(1);
	n = list1.length();
	int i = 0;
	while (iter) {
		++i;

		LinkedList<int>::Iterator findIter = list1.findKth(i);
		if (findIter) {
			list3.insertInP(*findIter, findIter);
			printf("find ������%d��Ԫ��%d\n", i, *findIter);
		}
		else {
			puts("find error");
		}

		LinkedList<int>::Iterator findIterRe = list1.findKth(-1);
		if (findIterRe) {
			printf("find ������%d��Ԫ��%d\n", 1, *findIterRe);
		}
		else {
			puts("find error");
		}

		int deleteTemp = *findIterRe;
		LinkedList<int>::Iterator findDataPo = list1.findData(deleteTemp);
		if (list1.deleteInP(findDataPo)) {
			printf("delete %d\n", deleteTemp);
			list1.output();
		}
		else {
			puts("delete error");
		}
	}
	return 0;
}

int mainForQueue() {
	Queue<int> q;
	for (int i = 0; i < 10; ++i) {
		q.offer(i);
	}
	while (!q.isEmpty()) {
		cout << q.poll() << endl;
	}
	Queue<string> qs;
	for (int i = 0; i < 10; ++i) {
		qs.offer(string("Queue data"));
	}
	while (!qs.isEmpty()) {
		cout << qs.poll() << endl;
	}
	puts("Queue test end");
	return 0;
}

int mainForDeQueue() {
	Deque<int> q;
	for (int i = 0; i < 10; ++i) {
		q.offer(i);
	}
	while (!q.isEmpty()) {
		cout << q.poll() << endl;
	}

	Deque<string> qs;
	for (int i = 0; i < 10; ++i) {
		qs.offer(string("Deque data"));
	}
	while (!qs.isEmpty()) {
		cout << qs.poll() << endl;
	}
	
	Deque<LinkedList<string>> ql;
	for (int i = 0; i < 10; ++i) {
		LinkedList<string> temp;
		temp.insertData(string("Deque data in list"));
		ql.offer(temp);
	}
	while (!ql.isEmpty()) {
		ql.poll().output();
	}
	puts("Deque test end");
	return 0;
}

int main() {
	//FILE *inFile = stdin, *outFile;
	//freopen("input", "r", stdin);
	//freopen_s(&inFile, "input", "r", stdin);
	MainForStack();
	mainForQueue();
	mainForDeQueue();
	MainForLinkedList();
	return 0;
}

