#include "stdafx.h"
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"

#include "Standard.h"
using namespace StandardExtend;

//Demo

int MainForStack() {
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
	puts("输入链表各元素 以-1结尾");
	list1.input().output();
	list2.input().output();
	puts("begain\n");

	puts("反转后list2\n");
	list2.reverse().output();

	puts("合并list1 list2\n");
	list1.merge(list2).output();
	LinkedList<int>::Iterator iter = list1.findKth(1);
	n = list1.length();
	int i = 0;
	while (iter) {
		++i;

		LinkedList<int>::Iterator findIter = list1.findKth(i);
		if (findIter) {
			list3.insertInP(*findIter, findIter);
			printf("find 正数第%d个元素%d\n", i, *findIter);
		}
		else {
			puts("find error");
		}

		LinkedList<int>::Iterator findIterRe = list1.findKth(-1);
		if (findIterRe) {
			printf("find 倒数第%d个元素%d\n", 1, *findIterRe);
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
	Queue q = Queue();
	for (int i = 0; i < 10; ++i) {
		q.offer(i);
	}
	while (!q.isEmpty()) {
		cout << q.poll() << endl;
	}
	puts("Queue test end");
	return 0;
}

int main() {
	mainForQueue();
	MainForStack();
	MainForLinkedList();
	return 0;
}

