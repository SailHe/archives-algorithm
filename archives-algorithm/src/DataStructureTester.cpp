#include "Teater.h"
#include "linear/Varrays.h"
#include "./nonlinear/ExpressionTree.h"
#include "./nonlinear/CoordinatesMap.h"

using namespace std;

int testForStack() {
	int i = 0;

	std::cout << "Stack<int>: " << std::endl;
	Stack<int> s;
	for (i = 0; i < 10; ++i) {
		s.push(i);
	}
	while (!s.isEmpty()) {
		StandardExtend::testAndOut("", s.pop(), --i);
	}

	std::cout << "Stack<string>: " << std::endl;
	Stack<string> ss;
	for (i = 0; i < 10; ++i) {
		ss.push(string("StackData") + std::to_string(i));
	}
	while (!ss.isEmpty()) {
		StandardExtend::testAndOut("", ss.pop(), string("StackData") + std::to_string(--i));
	}
	puts(" ====== Stack test end");
	return 0;
}
int testForLinkedList() {
	int i = 0;

	std::cout << "LinkedList<string>: " << std::endl;
	LinkedList<string> lists = LinkedList<string>();
	for (i = 0; i < 5; ++i) {
		lists.insertData("LinkedListData" + std::to_string(i));
	}
	
	auto it = lists.findKth(1);
	i = -1;
	while (it) {
		StandardExtend::testAndOut("", *it, "LinkedListData" + std::to_string(++i));
		++it;
	}

	std::cout << "LinkedList<std::stack<string>>: " << std::endl;
	LinkedList<stack<string>> stackList;
	for (i = 0; i < 5; ++i) {
		stack<string> s;
		for (int j = 0; j < 2; ++j) {
			s.push("List data in std::stack" + std::to_string(i) + std::to_string(j));
		}
		stackList.insertData(s);
	}
	i = -1;
	auto it2 = stackList.findKth(1);
	StandardExtend::iterate(it2, stackList.end(), [&](auto it) {
		auto s = *it;
		++i;
		int j = 2;
		while (!s.empty()) {
			StandardExtend::testAndOut("", s.top(), "List data in std::stack" + std::to_string(i) + std::to_string(--j));
			s.pop();
		}
	});
	/*
	1 3 5 7 9 11 -1
	2 4 6 8 10 12 -1
	*/
	std::function<std::string(int)> intToString = [](int i) {
		return std::to_string(i);
	};
	LinkedList<int> list1 = LinkedList<int>();
	LinkedList<int> list2 = LinkedList<int>();
	LinkedList<int> list3 = LinkedList<int>();
	int n;
	JCE::ArrayList<int> listData1 = { -1, 1, 3, 5, 7, 9, 11, 13 };
	JCE::String listData1Str = "-1 1 3 5 7 9 11 13";
	JCE::ArrayList<int> listData2 = { -2, 2, 4, 6, 8, 10, 12, 14 };
	JCE::String listData2Str = "-2 2 4 6 8 10 12 14";
	for (JCE::SizeType i = 0; i < listData1.size(); ++i) {
		list1.insertData(listData1[i]);
		list2.insertData(listData2[i]);
	}
	StandardExtend::testAndOut("", list1.toString(intToString), listData1Str);
	StandardExtend::testAndOut("", list2.toString(intToString), listData2Str);
	puts("begain\n");

	puts("��תlist2\n");
	std::vector<int> listData2R = listData2;
	std::reverse(listData2R.begin(), listData2R.end());
	auto listTmp = std::vector<int>();
	list2.reverse();
	std::for_each(list2.begin(), list2.end(), [&listTmp](int ele) {
		listTmp.emplace_back(ele);
	});
	StandardExtend::testAssert(listTmp, listData2R);

	puts("�ٴη�תlist2\n");
	listTmp.clear();
	list2.reverse();
	std::reverse(listData2R.begin(), listData2R.end());
	std::for_each(list2.begin(), list2.end(), [&listTmp](int ele) {
		listTmp.emplace_back(ele);
	});
	StandardExtend::testAssert(listTmp, listData2R);

	puts("����ϲ�list2��list1\n");
	std::for_each(list1.begin(), list1.end(), [&listTmp](int ele) {
		listTmp.emplace_back(ele);
	});
	
	std::sort(listTmp.begin(), listTmp.end());

	int index = -1;
	std::string resultReal;
	std::for_each(listTmp.begin(), listTmp.end(), [&index, &resultReal](int ele) {
		resultReal += (++index == 0) ? std::to_string(ele) : " " + std::to_string(ele);
	});
	std::string result = list1.merge(list2).toString(intToString);
	StandardExtend::testAssert(result, resultReal);
	LinkedList<int>::Iterator iter = list1.findKth(1);
	n = list1.length();
	i = 0;
	while (iter) {
		++i;
		std::cout << list1.toString(intToString) << std::endl;
		LinkedList<int>::Iterator findIter = list1.findKth(i);
		if (findIter) {
			StandardExtend::testAndOut("find ������" + std::to_string(i) + "��Ԫ��", *findIter, listTmp[i-1]);
		}
		else {
			// δ�ҵ�
		}

		iter = list1.findKth(-1);
		if (iter) {
			StandardExtend::testAndOut("find ������1��Ԫ��", *iter, listTmp[listTmp.size() - 1]);

			int deleteTemp = *iter;
			LinkedList<int>::Iterator findDataPo = list1.findData(deleteTemp);
			if (list1.deleteInP(findDataPo)) {
				int dT = listTmp.back();
				listTmp.pop_back();
				StandardExtend::testAndOut("deleteԪ��", deleteTemp, dT);
			}
			else {
				_ASSERT_EXPR(false, "�쳣");
			}
		}
		else {
			// δ�ҵ�
		}
	}

	cout << " ====== LinkedList test end" << endl;
	return 0;
}
int testForVarrays() {
	Varrays<int> a;
	for (int i = 0; i < 10; ++i) {
		a.push_back(i);
	}
	StandardExtend::testAssert(a.size(), 10u);
	for (int i = 0; i < 10; ++i) {
		StandardExtend::testAssert(a[i], i);
	}
	a.resize(a.size() + 1);
	StandardExtend::testAssert(a.size(), 11u);
	a[10] = 10;
	StandardExtend::testAssert(a[10], 10);
	Varrays<int> b;
	b.assign(a.begin(), a.end());
	a.clear();
	StandardExtend::testAssert(a.size(), 0u);
	a.assign(b.begin(), b.end());
	for (int i = 0; i < 11; ++i) {
		StandardExtend::testAssert(a[i], i);
		StandardExtend::testAssert(b[i], i);
	}


	Varrays<std::string> c;
	for (int i = 0; i < 10; ++i) {
		c.push_back(std::to_string(i));
	}
	StandardExtend::testAssert(c.size(), 10u);
	for (int i = 0; i < 10; ++i) {
		StandardExtend::testAssert(c[i], std::to_string(i));
	}
	c.reserver(1);
	StandardExtend::testAssert(c.capacity(), 1u);

	return 11;
}
int subTestForQueueInt(Queue<int> &qInterface) {
	int i;
	for (i = 0; i < 10; ++i) {
		qInterface.offer(i);
	}
	i = -1;
	while (!qInterface.isEmpty()) {
		StandardExtend::testAssert(qInterface.poll(), ++i);
	}
	return 0;
}
int subTestForQueueStr(Queue<std::string> &qsInterface) {
	int i;
	for (i = 0; i < 10; ++i) {
		qsInterface.offer(string("Queue data") + std::to_string(i));
	}
	i = -1;
	while (!qsInterface.isEmpty()) {
		StandardExtend::testAssert(qsInterface.poll(), string("Queue data") + std::to_string(++i));
	}
	return 0;
}
int subTestForQueueList(Queue<LinkedList<std::string>> &qlInterface) {
	std::function<std::string(std::string const&)> stringToString = [](std::string const &i) {
		return i;
	};
	for (int i = 0; i < 10; ++i) {
		LinkedList<string> temp;
		temp.insertData(string("Deque data in list"));
		qlInterface.offer(temp);
	}
	while (!qlInterface.isEmpty()) {
		qlInterface.poll().toString(stringToString);
	}
	return 0;
}
int testForQueue() {
	LinkedQueue<int> q;
	subTestForQueueInt(q);
	LinkedQueue<string> qs;
	subTestForQueueStr(qs);
	puts(" ====== Queue test end");
	return 0;
}
int testForDeQueue() {
	Deque<int> q;
	subTestForQueueInt(q);
	Deque<string> qs;
	subTestForQueueStr(qs);
	Deque<LinkedList<string>> ql;
	subTestForQueueList(ql);
	//ql.shell();
	puts(" ====== Deque test end");
	return 0;
}

// ���Խṹ����������
int mainForLinearStructure() {
	testForVarrays();
	testForLinkedList();
	testForStack();
	testForQueue();
	testForDeQueue();
	return 0;
}

int subTestForBinTree() {
	// ����ôΪ�˷������ûʹ��char
	const int len = 7;
	std::string pre[len] = { "a", "b", "h", "f", "e", "d", "c" };
	std::string in[len] = { "h", "b", "f", "a", "d", "e", "c" };
	std::string post[len] = { "h", "f", "b", "d", "c", "e", "a" };
	std::string preReal[len] = {};
	std::string postReal[len] = {};
	// ��������ת��
	BinTreeAlgorithm::calcPrefOrder(preReal, in, post, len);
	StandardExtend::testAssert(StandardExtend::isEqual(preReal, preReal + len, pre), true);
	BinTreeAlgorithm::calcPostOrder(pre, in, postReal, len);
	StandardExtend::testAssert(StandardExtend::isEqual(postReal, postReal + len, post), true);
	
	const int size = 6;
	auto subTest = [&size](BinTree<char> &btIns) {
		StandardExtend::testAssert(btIns.size(), size);
		StandardExtend::testAssert(btIns.height(), (int)std::ceil(MathExtend::logRadix(size, 2.0)));
		StandardExtend::testAssert(btIns.getRoot()->Data, '1');
		std::string resultReal;
		btIns.traversal(Tree::ORDER_PREFIX_ROOT, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
		});
		StandardExtend::testAndOut("BT �ȸ���", resultReal, std::string("1 2 3 4 5 6"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_POST_ROOT, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
		});
		StandardExtend::testAndOut("BT �����", resultReal, std::string("3 4 2 6 5 1"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_INFIX_ROOT, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
		});
		StandardExtend::testAndOut("BT �и���", resultReal, std::string("3 2 4 1 6 5"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_LEVEL, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
			return false;
		});
		StandardExtend::testAndOut("BT ����", resultReal, std::string("1 2 5 3 4 6"));
	};
	
	/*
	�ȸ��������ջ
	6
	Push 1
	Push 2
	Push 3
	Pop
	Pop
	Push 4
	Pop
	Pop
	Push 5
	Push 6
	Pop
	Pop
	output:3 4 2 6 5 1
	
	�ṹ
		1
	 2     5
	3 4   6 *
	*/
	int index = 0;
	static char order2D[12][size] = { "Push","Push","Push","Pop","Pop","Push","Pop","Pop","Push","Push","Pop","Pop" };
	LinkedBinTree<char> btIns1 = LinkedBinTree<char>([&index](std::string &order) {
		order = std::string(order2D[index++]);
		return index < 13;
	}, [](char *num) {
		static int i = 0;
		static char nums[] = { '1','2','3','4','5','6' };
		*num = nums[i++];
	});
	subTest(btIns1);

	// char�ȽϺò���
	// ����[1] 2  3  4  5  6
	// { 1, 2, 3, 4, 5, 6 };
	char preOrder[size + 1] = "123456";
	// ���� 3  2  4 [1] 6  5
	// { 3, 2, 4, 1, 6, 5 }
	char inOrder[size + 1] = "324165";
	// ���� 3  4  2  6  5 [1]
	// { 3, 4, 2, 6, 5, 1 }
	char postOrder[size + 1] = "342651";
	LinkedBinTree<char> btIns2 = LinkedBinTree<char>(preOrder, inOrder, size);
	subTest(btIns2);
	LinkedBinTree<char> btIns3 = LinkedBinTree<char>(size, inOrder, postOrder);
	subTest(btIns3);
	// ��������
	LinkedBinTree<char> btIns4 = btIns3;
	subTest(btIns4);

	auto newBtFun = [&size, &inOrder, &postOrder]() {
		return LinkedBinTree<char>(size, inOrder, postOrder);
	};

	// �ƶ�����(@TODO �˴������ƶ�����)
	LinkedBinTree<char> btIns5(newBtFun());
	btIns5 = LinkedBinTree<char>(size, inOrder, postOrder);
	btIns5.clear();
	btIns5 = LinkedBinTree<char>(size, inOrder, postOrder);
	subTest(btIns5);
	// ����stl��bug��? һ��Ҫ��ԭ�ռ����ܸ���
	JCE::ArrayList<char> fillData(size + 1);
	std::copy(std::begin(inOrder), std::end(inOrder), fillData.begin());
	fillData.resize(size);
	btIns5.fillData(fillData);
	subTest(btIns5);
	btIns5.mirReversal();
	StandardExtend::testAssert(btIns5.omorphism(btIns4), true);
	auto subTestR = [&size](BinTree<char> &btIns) {
		StandardExtend::testAssert(btIns.size(), size);
		StandardExtend::testAssert(btIns.height(), (int)std::ceil(MathExtend::logRadix(size, 2.0)));
		StandardExtend::testAssert(btIns.getRoot()->Data, '1');
		std::string resultReal;
		btIns.traversal(Tree::ORDER_PREFIX_ROOT, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
		});
		// ����������ԭ�����෴
		StandardExtend::testAndOut("BT �����ȸ���", resultReal, std::string("1 5 6 2 4 3"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_POST_ROOT, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
		});
		// ���������ԭ�����෴
		StandardExtend::testAndOut("BT ��������", resultReal, std::string("6 5 4 3 2 1"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_INFIX_ROOT, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
		});
		// ����������ԭ�����෴
		StandardExtend::testAndOut("BT �����и���", resultReal, std::string("5 6 1 4 2 3"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_LEVEL, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
			return false;
		});
		// ���������ԭ�����ϵ��������(�����һ����ȫ�������Ļ�����ÿ2^d��ԭ����ת dΪ���)
		StandardExtend::testAndOut("BT �������", resultReal, std::string("1 5 2 6 4 3"));
	};
	subTestR(btIns5);

	/*
	8
	1 -
	- -
	0 -
	2 7
	- -
	- -
	5 -
	4 6

	����˳���ַ����� 0~7 ��8�����
		����ṹ
			   3
		   2       7
		 0   -   4   6
		1 -         5 -

		�洢�ṹ
		[ 0  1  2  3  4  5  6  7]
		 1- -- 0- 27 -- -- 5- 46
	*/
	// StandardExtend::refreshStdin();
	char btsSub[][4] = { "1 -", "- -", "0 -","2 7", "- -", "- -", "5 -", "4 6" };
	LinkedBinTree<char> btStIns1 = LinkedBinTree<char>(8, [](char *tData) {
		static int i = 0;
		*tData = TransitionUtility::toAlphOrAscllNum(i++);
	}, [&btsSub](int *lSub, int *rSub) {
		static int r = 0;
		char lSubC, rSubC;
		lSubC = btsSub[r][0];
		rSubC = btsSub[r][2];
		++r;
		*lSub = lSubC -'0', *rSub = rSubC -'0';
	}, '-' - '0');
	std::string resultReal;
	btStIns1.traversal(Tree::ORDER_PREFIX_ROOT, [&resultReal](BinTree<char>::BT bNode) {
		std::string tmp;
		tmp += bNode->Data;
		StandardExtend::formatStrAppend(resultReal, tmp);
	});
	/*
	9
	1 6
	2 3
	-1 -1
	-1 4
	5 -1
	-1 -1
	7 -1
	-1 8
	-1 -1
	����ṹ
			    0
		   1         6
		 2   3     7   -
		      4   - 8 5 -
		     5 -
	73 45 11 58 82 25 67 38 42
	*/
	const int nSizeBtS2 = 9;
	std::vector<std::vector<int>> nBstSub = { {1, 6},{2, 3},{-1, -1},{-1, 4 },{5, -1 },{-1, -1},{7, -1 },{-1, 8 },{-1, -1} };
	std::vector<int> nPre = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	LinkedBinTree<int> btStIns2 = LinkedBinTree<int>(nSizeBtS2, [](int *tData){
		static int i = 0;
		*tData = i++;
	}, [&nBstSub](int *lSub, int *rSub) {
		static int r = 0;
		*lSub = nBstSub[r][0];
		*rSub = nBstSub[r][1];
		++r;
	}, -1);
	std::vector<int> preBts2Real;
	btStIns2.traversal(Tree::ORDER_PREFIX_ROOT, [&preBts2Real](BinTree<int>::BT bNode) {
		preBts2Real.emplace_back(bNode->Data);
	});
	StandardExtend::testAssert(preBts2Real, nPre);
	StandardExtend::testAssert(btStIns2.leavesCount(0), 0);
	StandardExtend::testAssert(btStIns2.leavesCount(1), 0);
	StandardExtend::testAssert(btStIns2.leavesCount(2), 1);
	StandardExtend::testAssert(btStIns2.leavesCount(3), 1);
	StandardExtend::testAssert(btStIns2.leavesCount(4), 1);
	StandardExtend::testAssert(btStIns2.leavesCount(5), -1);

	// Bintree���Ʋ���16������, ��������(ʵ����ֻ��һ������ȫ���Ե�) ���з���19��, ��������
	// VirtualLinkedBinTree���Ʋ���3������, ��������, ���з���3��, ��ȫ����
	return 16;
}
/**
int subTestForCompleteBinSearchTree() {
	// 0 1 2 [[3 4 5 [6] 7 8]] 9
	int preData1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	CompleteBinSearchTree<int> cbtIns1 = CompleteBinSearchTree<int>(10, preData1);
	StandardExtend::testAssert(cbtIns1.find(0)->Data, 0);
	StandardExtend::testAssert(cbtIns1.find(4)->Data, 4);
	StandardExtend::testAssert(cbtIns1.find(8)->Data, 8);
	StandardExtend::testAssert(cbtIns1.getRootData(), 6);
	return 2;
}
/**/
int subTestForBinSearchTree(LinkedBinSearchTree<std::string> &bst, std::string const &insName) {

	bst.clear();
	std::string resultBuffer;
	int b[10] = { 2, 1, 4, 5, 9, 3, 6, 7, 8, 0 };
	for (int i = 0; i < 10; ++i) {
		StandardExtend::testAssert(bst.insert(std::to_string(b[i])).second, true);
		StandardExtend::formatStrAppend(resultBuffer, std::to_string(i));
	}
	std::string resultBufferReal;

	StandardExtend::testAndOut(insName + "find", bst.find(string("5"))->Data, std::string("5"));

	bst.traversal(Tree::ORDER_INFIX_ROOT, [&resultBufferReal](LinkedBinSearchTree<string>::BT node) {
		StandardExtend::formatStrAppend(resultBufferReal, node->Data);
	});
	StandardExtend::testAndOut(insName + "�ȸ���", resultBufferReal, resultBuffer);

	resultBufferReal.clear();
	bst.traversal(LinkedBinSearchTree<std::string>::ORDER_SEQUENCE, [&resultBufferReal](const LinkedBinSearchTree<std::string>::BT node) {
		StandardExtend::formatStrAppend(resultBufferReal, node->Data);
		return node->Data == string("5") ? true : false;;
	});
	// 0 1 2 3 4 5
	std::string subS = resultBuffer.substr(0, resultBuffer.length() - resultBuffer.find('4'));
	StandardExtend::testAndOut(insName + "˳����ֹ", resultBufferReal, subS);

	resultBufferReal.clear();
	bst.traversal(LinkedBinSearchTree<std::string>::ORDER_REVERSE, [&resultBufferReal](LinkedBinSearchTree<string>::BT node) {
		StandardExtend::formatStrAppend(resultBufferReal, node->Data);
		return node->Data == string("5") ? true : false;
	});
	std::string subR = resultBuffer.substr(resultBuffer.find('5'), resultBuffer.length() - resultBuffer.find('5'));
	std::reverse(subR.begin(), subR.end());
	StandardExtend::testAndOut(insName + "������ֹ", resultBufferReal, subR);

	resultBufferReal.clear();
	bst.traversal(LinkedBinSearchTree<std::string>::ORDER_REVERSE, [&resultBufferReal](LinkedBinSearchTree<std::string>::BT node) {
		StandardExtend::formatStrAppend(resultBufferReal, node->Data);
		return false;
	});
	std::string bufR = resultBuffer;
	std::reverse(bufR.begin(), bufR.end());
	StandardExtend::testAndOut(insName + "����", resultBufferReal, bufR);

	int idel = 0;
	while (!bst.empty()) {
		bst.erase(std::to_string(idel++));
	}
	resultBufferReal.clear();
	bst.traversal(LinkedBinSearchTree<std::string>::ORDER_REVERSE, [&resultBufferReal](LinkedBinSearchTree<std::string>::BT node) {
		StandardExtend::formatStrAppend(resultBufferReal, node->Data);
		return false;
	});
	StandardExtend::testAndOut(insName + "ɾ�����", resultBufferReal, std::string(""));
	
	bst.insert("sss");
	bst.clear();
	resultBufferReal.clear();
	bst.traversal(LinkedBinSearchTree<std::string>::ORDER_REVERSE, [&resultBufferReal](LinkedBinSearchTree<std::string>::BT node) {
		StandardExtend::formatStrAppend(resultBufferReal, node->Data);
		return false;
	});
	StandardExtend::testAndOut(insName + "clear���", resultBufferReal, std::string(""));

	return 9;
}
int LessIntegerCmper(int const &lhs, int const &rhs) {
	// return lhs - rhs;// ����Խ��
	return lhs < rhs ? -1 : 
		lhs == rhs ? 
		0 : 1;
}
// more std::greater<int>()
int GreaterIntegerCmper(int const &lhs, int const &rhs) {
	return -LessIntegerCmper(lhs, rhs);
}
int subTestForHeapRebuild() {
	JCE::ArrayList<int> heapData = { 6, 2, 3, 15, 9, 7, 4, 12, 10, 15, 14, 5, 13, 0, 8, 1, 11 };
	JCE::ArrayList<int> heapRealDataList;

	// 1. ��С���ؽ�Ϊ����(������պ�push) PS: �ڱ�����MIN_INT32, ��˿����ų��ȽϷ�����Խ�����
	Heap<int> heapIns = Heap<int>(heapData.size() + 4, -1, GreaterIntegerCmper, &heapData[0], (int)heapData.size());
	heapIns.clear();
	heapIns.initialize(MAX_INT32, LessIntegerCmper);
	heapIns.rebuild();
	for (JCE::SizeType i = 0; i < heapData.size(); heapIns.push(heapData[i++]));
	std::string resultHeapStr;
	heapIns.traversal(Tree::ORDER_LEVEL, [&resultHeapStr](BinTree<int>::BT b) {
		StandardExtend::formatStrAppend(resultHeapStr, std::to_string(b->Data));
	});
	StandardExtend::testAndOut("Heap-����", resultHeapStr, std::string("15 15 13 11 14 7 8 10 9 6 12 3 5 0 4 1 2"));
	heapRealDataList.clear();
	while (!heapIns.empty()) {
		heapRealDataList.emplace_back(heapIns.pop());
	}
	StandardExtend::testAssert(heapRealDataList, std::vector<int>({ 15,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 }));

	// 2. ʹ���µ����ݹ���
	heapIns.build(&heapData[0], (int)heapData.size(), MIN_INT32, GreaterIntegerCmper);
	heapRealDataList.clear();
	while (!heapIns.empty()) {
		heapRealDataList.emplace_back(heapIns.pop());
	}
	StandardExtend::testAssert(heapRealDataList, std::vector<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15 }));

	// 3. ʹ���µ����ݹ���(��һ��������ȷ�ϴ˷��������ؽ�������Ķ�)
	heapIns.build(&heapData[0], (int)heapData.size(), MIN_INT32, GreaterIntegerCmper);
	// ��С���ؽ�Ϊ����(���ݲ���)
	heapIns.initialize(MAX_INT32, LessIntegerCmper);
	heapIns.rebuild();
	heapRealDataList.clear();
	while (!heapIns.empty()) {
		heapRealDataList.emplace_back(heapIns.pop());
	}
	StandardExtend::testAssert(heapRealDataList, std::vector<int>({ 15,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 }));

	// 4. build������Ӳ���
	heapIns.clear();
	resultHeapStr.clear();
	heapIns.build(&heapData[0], (int)heapData.size(), MIN_INT32, GreaterIntegerCmper);
	heapIns.traversal(Tree::ORDER_LEVEL, [&resultHeapStr](BinTree<int>::BT b) {
		StandardExtend::formatStrAppend(resultHeapStr, std::to_string(b->Data));
	});
	StandardExtend::testAndOut("Heap-����", resultHeapStr, std::string("0 1 3 2 9 5 4 11 10 15 14 7 13 6 8 12 15"));

	// ��ʽrebuild, һ������, ��3������
	return 1;
}
int subTestForHeapClearBuild() {
	JCE::ArrayList<int> heapData = { 6, 2, 3, 15, 9, 7, 4, 12, 10, 15, 14, 5, 13 };
	JCE::ArrayList<int> heapRealDataList;

	// Build + pop;
	Heap<int> heapIns0 = Heap<int>(heapData.size(), - 1, GreaterIntegerCmper, &heapData[0], (int)heapData.size());
	// heapIns0.rebuild(-1, GreaterIntegerCmper);
	while (!heapIns0.empty()) {
		heapRealDataList.emplace_back(heapIns0.pop());
	}
	// "Heap-Pop With Build"
	StandardExtend::testAssert(heapRealDataList, std::vector<int>({ 2, 3, 4, 5, 6, 7, 9, 10, 12, 13, 14, 15, 15 }));

	// rebuild + clear + push + traversal, p1:clear��ֱ��push�Ļ�root����û�и���
	Heap<int> heapIns = Heap<int>(heapData.size() + 4, -1, GreaterIntegerCmper, &heapData[0], (int)heapData.size());
	// heapIns.rebuild(-1, GreaterIntegerCmper);
	heapIns.clear();
	// heapIns.initialize(-1);
	for (JCE::SizeType i = 0; i < heapData.size(); heapIns.push(heapData[i++]));
	heapIns.push(0);
	heapIns.push(1);
	heapIns.push(8);
	heapIns.push(11);
	std::string resultHeap;
	heapIns.traversal(Tree::ORDER_LEVEL, [&resultHeap](BinTree<int>::BT b) {
		StandardExtend::formatStrAppend(resultHeap, std::to_string(b->Data));
	});
	StandardExtend::testAndOut("Heap-����", resultHeap, std::string("0 6 1 8 9 5 2 10 12 15 14 7 13 4 3 15 11"));
	
	heapRealDataList.clear();
	while (!heapIns.empty()) {
		heapRealDataList.emplace_back(heapIns.pop());
	}
	// "Heap-Pop With push"
	StandardExtend::testAssert(heapRealDataList, std::vector<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15 }));

	// �²��Ե�ֻ�� clear ��1�����з���
	return 1;
}
int subTestForHeap() {
	// ������ȫ������ �����Ƕ���������
	JCE::ArrayList<int> heapData = { 6, 2, 3, 15, 9, 7, 4, 12, 10, 15, 14, 5, 13 , 0, 1, 8, 11};
	JCE::ArrayList<int> heapRealDataList;
	Heap<int> heapIns = Heap<int>(heapData.size() + 4, -1, GreaterIntegerCmper);
	std::vector<int> tmp;
	for (JCE::SizeType i = 0; i < heapData.size(); ++i) {
		/*
		tmp.clear();
		heapIns.traversal(heapIns.ORDER_LEVEL, [&tmp](BinTree<int>::BT b) {
			tmp.emplace_back(b->Data);
		});
		MathExtend::outputCompleteTreeLevel(tmp.begin(), tmp.end(), tmp.size());
		*/
		heapIns.push(heapData[i]);
	}
	std::string resultHeapStr;
	heapIns.traversal(Tree::ORDER_LEVEL, [&resultHeapStr](BinTree<int>::BT b) {
		StandardExtend::formatStrAppend(resultHeapStr, std::to_string(b->Data));
	});
	StandardExtend::testAndOut("Heap-����", resultHeapStr, std::string("0 6 1 8 9 5 2 10 12 15 14 7 13 4 3 15 11"));
	heapRealDataList.emplace_back(heapIns.pop());
	resultHeapStr.clear();
	heapIns.traversal(Tree::ORDER_LEVEL, [&resultHeapStr](BinTree<int>::BT b) {
		StandardExtend::formatStrAppend(resultHeapStr, std::to_string(b->Data));
	});
	// �м�����Ƿ���������
	StandardExtend::testAssert(resultHeapStr, std::string("1 6 2 8 9 5 3 10 12 15 14 7 13 4 11 15"));

	while (!heapIns.empty()) {
		/*
		tmp.clear();
		heapIns.traversal(heapIns.ORDER_LEVEL, [&tmp, &resultHeapStr](BinTree<int>::BT b) {
			tmp.emplace_back(b->Data);
		});
		MathExtend::outputCompleteTreeLevel(tmp.begin(), tmp.end(), tmp.size());
		*/
		int tmpP = heapIns.pop();
		//std::cout << "pop: " << tmpP << std::endl;
		heapRealDataList.emplace_back(tmpP);
	}
	// "Heap-Pop"
	StandardExtend::testAssert(heapRealDataList, std::vector<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15 }));

	resultHeapStr.clear();
	heapIns.traversal(Tree::ORDER_LEVEL, [&resultHeapStr](BinTree<int>::BT b) {
		StandardExtend::formatStrAppend(resultHeapStr, std::to_string(b->Data));
	});
	StandardExtend::testAssert(resultHeapStr, std::string(""));
	// ��Ҫ�� ���� ���� ��rebuild push pop empty ���ڵ�5�����з���
	return 6 + subTestForHeapClearBuild() + subTestForHeapRebuild();
}
int testForHuffumanTree() {
	/*
	Сд��ĸ��01������2��Ի�����2���غ�
	A 1 B 1 C 1 D 3 E 3 F 6 G 6
	*/
	// ������Ԫ�ظ���
	const int CH_N = 7;
	char chArr[CH_N + 1] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', '\0'};
	int freqArr[CH_N] = { 1, 1, 1, 3, 3, 6, 6 };
	const int WPL_STAND = 6*2 + 6*2 + 3*2 + 3*3 + 4*1 + 5*1 + 5*1;
	char codesArr[CH_N][CH_N + 1] = { "10001", "10000", "1001", "11", "101", "01", "00"}; // ����ǰ׺��֮һ

	vector<std::string> codesArr1 = { "01010", "01011", "0100", "011", "10", "11", "00"}; // ����ǰ׺��֮һ
	vector<std::string> codesArr2 = { "00000", "00001", "0001", "001", "01", "10", "11"}; // ����ǰ׺��֮һ
	std::string codesArr3[CH_N] = { "000", "001", "010", "011", "100", "101", "110"}; // �޹�����ǰ׺��
	char codesArr4[CH_N][CH_N + 1] = { "00000", "00001", "0001", "001", "00", "10", "11"}; // ��ǰ׺��
	std::string codesArr5[CH_N] = { "000000", "000001", "00001", "0001", "001", "01", "1"}; // ǰ׺�� ������

	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr, codesArr + CH_N), true);
	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr1.begin(), codesArr1.end()), true);
	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr2.begin(), codesArr2.end()), true);
	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr3, codesArr3 + CH_N), true);
	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr4, codesArr4 + CH_N), false);
	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr5, codesArr5 + CH_N), true);

	HuffmanTree<char> huffT(chArr, freqArr, CH_N);
	StandardExtend::testAssert(huffT.wpl(), WPL_STAND);
	// ��Ȩ·������WPL<==>����ı����볤��
	// ��������WPL<==>���������볤��<==>����ǰ׺����ı����볤��
	//  codingLen
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr), std::end(codesArr), freqArr), WPL_STAND);
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr1), std::end(codesArr1), freqArr), WPL_STAND);
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr2), std::end(codesArr2), freqArr), WPL_STAND);
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr3), std::end(codesArr3), freqArr) == WPL_STAND, false);
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr4), std::end(codesArr4), freqArr), WPL_STAND);
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr5), std::end(codesArr5), freqArr) == WPL_STAND, false);
	// string const codes[]          ���
	// LinkedList<std::string> codes ��̬��ǿ
	// ArrayList<std::string> codes  ��������Ը�
	// ArrayList<char const*> codes  �ڴ���Ҫ�Լ�����, ���켰�䲻����(��Ҫ�ֶ�һ����push_back��ȥ)
	// Iteratorģ��					 ��������������, ���ӿڱȽϺ���

	const int N_HT = 4;
	int w[N_HT] = { 1, 2, 3, 4 };
	int resultHt[][4] = {
		{ 1, 5, 0, 0},
		{ 2, 5, 0, 0},
		{ 3, 6, 0, 0},
		{ 4, 7, 0, 0},
		{ 3, 6, 1, 2},
		{ 6, 7, 3, 5},
		{ 10, 0, 4, 6}
	};
	std::vector<std::string> huffmanCodes;
	ArrayHuffman arrHufT(w, N_HT, huffmanCodes);
	ArrayHuffman::HuffmanTreeArray ht = arrHufT.getHuffmanTreeArray();
	// 0���ǿս��
	for (int i = 1; i < N_HT; ++i) {
		StandardExtend::testAssert(ht[i].weight, resultHt[i - 1][0]);
		StandardExtend::testAssert(ht[i].parent, resultHt[i - 1][1]);
		StandardExtend::testAssert(ht[i].lchild, resultHt[i - 1][2]);
		StandardExtend::testAssert(ht[i].rchild, resultHt[i - 1][3]);
	}

	// ������볤��(��������ʽ������������)
	auto ArrayHuffCodeLen = [&huffmanCodes](int *hufWeightList, int N_HT) {
		huffmanCodes.clear();
		ArrayHuffman arrHufT1(hufWeightList, N_HT, huffmanCodes);
		return HuffmanTree<char>::textLen(huffmanCodes.begin(), huffmanCodes.end(), hufWeightList);
	};
	int hufWeightList1[] = { 1, 2, 3, 4, 5 };
	int hufWeightList2[] = { 3, 8, 8 };
	int hufWeightList3[] = { 1, 1, MAX_INT32-4 };
	StandardExtend::testAndOut("huffCodeLen: ", ArrayHuffCodeLen(hufWeightList1, 5), 33);
	StandardExtend::testAndOut("��ȨhuffCodeLen: ", ArrayHuffCodeLen(hufWeightList2, 3), 30);
	StandardExtend::testAndOut("�ٽ�ֵhuffCodeLen: ", ArrayHuffCodeLen(hufWeightList3, 3), MAX_INT32);

	// 4������ʽ���з��� + 5������ʽ���з���(����������) PS: ��һ������, �����ж����������
	return 4 + 5;
}

int testForExpressionTree() {
	//�Լ��ĸ�
	char s[40] = "1 2 +";
	while (gets(s)) {
		//ExpressionTree ET = ExpressionTree(s, ExpressionTree::POST_EXPRESSION);
		//ExpressionTree ET = ExpressionTree(s, ExpressionTree::PREFIX_EXPRESSION);
		ExpressionTree ET = ExpressionTree(s, ExpressionTree::INFIX_EXPRESSION);
		auto ans = ET.eval();
		if (ans == MAX_INT32) {
			puts("ERROR Expression");
		}
		else {
			printf("%.1lf\n", ans);
		}
	}
	return 0;
}

int testForTree() {
	int sumTestCnt = 0;
	sumTestCnt += subTestForBinTree();
	LinkedBinSearchTree<std::string> bsTreeIns = LinkedBinSearchTree<std::string>();
	sumTestCnt += subTestForBinSearchTree(bsTreeIns, "BST-");
	AvlTree<std::string> avlTreeIns = AvlTree<std::string>();
	sumTestCnt += subTestForBinSearchTree(avlTreeIns, "AVLT-");
	//sumTestCnt += subTestForCompleteBinSearchTree();
	//std::string preData3[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	//CompleteBinSearchTree<std::string> cbtIns3 = CompleteBinSearchTree<std::string>(10, preData3);
	//sumTestCnt += subTestForBinSearchTree(cbtIns3, "CBST-");
	sumTestCnt += subTestForHeap();

	sumTestCnt += testForHuffumanTree();
	//testForExpressionTree();

	// �����: ������ָ������ָ���ʵ�ִӶ�ʵ�ֶ�̬
	std::cout << " ====== Tree test end ���Ե�С��: " << sumTestCnt << std::endl;
	return sumTestCnt;
}

int testForGraph() {
	auto judgeGraphFun = [](Graph &g, std::vector<int> const &resultDist, std::vector<int> const &resultPath, bool suc = true) {
		JCE::ArrayList<Graph::VertexKey> dist, path;
		StandardExtend::testAssert(g.shortestPath(0, dist, path), suc);
		/*std::cout << SoftwareTestSpace::getInitString<std::vector<int>::iterator, int>(dist.begin(), dist.end(), [](int ele) {
			return std::to_string(ele);
		}) << std::endl;
		std::cout << SoftwareTestSpace::getInitString<std::vector<int>::iterator, int>(path.begin(), path.end(), [](int ele) {
			return std::to_string(ele);
		}) << std::endl;*/
		// ��0�Ŷ����������̾���ֵ: 
		// StandardExtend::outPutIterable(dist.begin(), dist.end(), 2);
		StandardExtend::testAssert(dist, resultDist);
		// ��0�Ŷ�����������·��: 
		// StandardExtend::outPutIterable(path.begin(), path.end(), 2);
		StandardExtend::testAssert(path, resultPath);
	};
	auto judgeGraph = [&judgeGraphFun](Graph *&&g) {
		for (int i = 0; i < 10; ++i) {
			g->insertEdge(Graph::Edge(0, i, 10));
			// �ظ������Ϊ����Ȩֵ
			g->insertEdge(Graph::Edge(0, i, 11));
		}
		StandardExtend::testAssert(g->getEdgeNum(), 10u);
		for (int i = 0; i < 10; ++i) {
			g->insertEdge(Graph::Edge(1, i, 5));
			g->insertEdge(Graph::Edge(i, 1, 5));
		}
		// �ܹ�30�������, 0->1��1->0�������ֱ��ظ���������β������ܱ���
		// ��Ҫע�����: �Լ����Լ�Ҳ��һ����(�������Ϊͣ����ԭ�ص�����) ��Ȼ ���·��ʱ���������Լ� ������Լ��ľ�����0 ��ôʵ�ֲ�֪����û������
		StandardExtend::testAssert(g->getEdgeNum(), 28u);
		// 0=11=>����(����0) 1<=5=>����
		judgeGraphFun(*g, std::vector<int>({ 0,5,10,10,10,10,10,10,10,10 }), std::vector<int>({ -1,0,1,1,1,1,1,1,1,1 }));

		// ��Ȩ�������(9 =-2=> 8)
		// 0=9=>8: 0=11-> 9=-2=> 8
		// 0=8=>8: 0=5-> 1=5=> 9=-2=> 8
		g->insertEdge(Graph::Edge(9, 8, -2));
		judgeGraphFun(*g, std::vector<int>({ 0,5,10,10,10,10,10,10,8,10 }), std::vector<int>({ -1,0,1,1,1,1,1,1,9,1 }));

		delete g; g = nullptr;
	};
	judgeGraph(new AdjacentMatrixGraph(10));
	judgeGraph(new AdjacentListGraph(10));
	// 3*3����ͼ ��9������
	CoordinatesMap cg(3, 3, DirCountEnum::DIR8);
	// Ĭ��ȫ����Ч������� �˾�ֻ������ʾAPI
	cg.setInvalid(1, 0);
	// �����Ƿ����ͨ�� ���������ڵ���Ч����ΪΪ�����������֮����������
	cg.setValid(0, 0);
	cg.setValid(0, 1);
	cg.setValid(0, 2);
	cg.setValid(1, 2);
	/*
	-1 -1 -1
	xx xx -1
	xx xx xx
	*/
	cg.output();
	// תΪ��Ȩͼ
	judgeGraphFun(cg.parityGraph(), 
		std::vector<int>({ 0,1,2,Graph::INF,Graph::INF,3,Graph::INF,Graph::INF,Graph::INF }),
		std::vector<int>({ -1,0,1,-1,-1,2,-1,-1,-1 })
	);
	// תΪȨ��ͼ(��Ȩ��·->���·ִ��ʧ��)
	judgeGraphFun(cg.parityGraph(true),
		std::vector<int>({ -16,-15,-14,Graph::INF,Graph::INF,Graph::INF,Graph::INF,Graph::INF,Graph::INF }),
		std::vector<int>({ 1,0,1,-1,-1,-1,-1,-1,-1 }),
		false
	);

	//������Ч��ͬʱ���ö���ֵ(ת��ΪȨ��)
	//����Ĭ�ϵ�ֵ�������κ�ֵ(ʵ����-1 ����Ӧ�ð����������Чֵ)��û������ֵ����Ч����ת��ΪȨֵ�ͻ������, ����API��ò�Ҫ����
	cg.setValidVertex({ 0, 0 }, 00);
	cg.setValidVertex({ 0, 1 }, 01);
	cg.setValidVertex({ 0, 2 }, 02);
	cg.setValidVertex({ 1, 2 }, 12);
	/*
	00 01 02
	xx xx 12
	xx xx xx
	*/
	cg.output();
	judgeGraphFun(cg.parityGraph(true), 
		std::vector<int>({ 0,1,3,Graph::INF,Graph::INF,15,Graph::INF,Graph::INF,Graph::INF }), 
		std::vector<int>({ -1,0,1,-1,-1,2,-1,-1,-1 })
	);

	cout << " ====== Graph test end" << endl;
	return 0;
}

int testForGraphTemplate() {
	GraphTemplate<int> *g = new AdjacentMatrixGraphTemplate<int>(10);
	GraphTemplate<double> *gd = new AdjacentMatrixGraphTemplate<double>(10);
	// GraphTemplate<PointGeographic> *gp = new AdjacentMatrixGraphTemplate<PointGeographic>(10);
	// GraphTemplate<DisSet> *gDisList = new AdjacentMatrixGraphTemplate<DisSet>(10);
	gd->insertEdge(GraphTemplate<double>::Edge(0, 1, 10.0));
	delete g;
	g = nullptr;
	return 0;
}

// �����Խṹ����������
int mainForNonlinearStructure() {
	int sumTestCnt = 0;
	sumTestCnt += testForTree();
	StandardExtend::testAndDiffClock(testForGraph);
	StandardExtend::testAndDiffClock(testForGraphTemplate);
	return 0;
}

int runDataStructureTest() {
	/*
	lambda���ʽ(��������)
		P345
		[�����б�](����ǩ��) {������};
		�����б�:
			lambda��ͨ������һ�������˲�����()��С����ʵ�ֵģ�һ��lambda�����Ǹ����һ��ʵ��
			����lambda������ľ����Զ��洢ʱ�ڵı���������������Щ�����ļ��Ϻ������бհ���
			һ��û��ָ���κβ����lambda����,������ʽת����һ��������ͬ������ʽ����ָ��
			���������б����ʱ������ڰ�����ʵ����ʱ�Բ������ķ�װ���������������ʵ���������Ա����ݴ��ڵģ�����޷���ת���ɺ���ָ��

	 __vfptr: �麯����vtableָ��
	 ��̬ʵ��: ����ָ��, �麯��
	*/
	mainForLinearStructure();
	mainForNonlinearStructure();
	return 0;
}
