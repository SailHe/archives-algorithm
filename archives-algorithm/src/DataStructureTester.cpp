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

	puts("反转list2\n");
	std::vector<int> listData2R = listData2;
	std::reverse(listData2R.begin(), listData2R.end());
	auto listTmp = std::vector<int>();
	list2.reverse();
	std::for_each(list2.begin(), list2.end(), [&listTmp](int ele) {
		listTmp.emplace_back(ele);
	});
	StandardExtend::testAssert(listTmp, listData2R);

	puts("再次反转list2\n");
	listTmp.clear();
	list2.reverse();
	std::reverse(listData2R.begin(), listData2R.end());
	std::for_each(list2.begin(), list2.end(), [&listTmp](int ele) {
		listTmp.emplace_back(ele);
	});
	StandardExtend::testAssert(listTmp, listData2R);

	puts("有序合并list2到list1\n");
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
			StandardExtend::testAndOut("find 正数第" + std::to_string(i) + "个元素", *findIter, listTmp[i-1]);
		}
		else {
			// 未找到
		}

		iter = list1.findKth(-1);
		if (iter) {
			StandardExtend::testAndOut("find 倒数第1个元素", *iter, listTmp[listTmp.size() - 1]);

			int deleteTemp = *iter;
			LinkedList<int>::Iterator findDataPo = list1.findData(deleteTemp);
			if (list1.deleteInP(findDataPo)) {
				int dT = listTmp.back();
				listTmp.pop_back();
				StandardExtend::testAndOut("delete元素", deleteTemp, dT);
			}
			else {
				_ASSERT_EXPR(false, "异常");
			}
		}
		else {
			// 未找到
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

// 线性结构测试主函数
int mainForLinearStructure() {
	testForVarrays();
	testForLinkedList();
	testForStack();
	testForQueue();
	testForDeQueue();
	return 0;
}

int subTestForBinTree() {
	// 这里么为了方便测试没使用char
	const int len = 7;
	std::string pre[len] = { "a", "b", "h", "f", "e", "d", "c" };
	std::string in[len] = { "h", "b", "f", "a", "d", "e", "c" };
	std::string post[len] = { "h", "f", "b", "d", "c", "e", "a" };
	std::string preReal[len] = {};
	std::string postReal[len] = {};
	// 遍历序列转换
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
		StandardExtend::testAndOut("BT 先根序", resultReal, std::string("1 2 3 4 5 6"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_POST_ROOT, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
		});
		StandardExtend::testAndOut("BT 后根序", resultReal, std::string("3 4 2 6 5 1"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_INFIX_ROOT, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
		});
		StandardExtend::testAndOut("BT 中根序", resultReal, std::string("3 2 4 1 6 5"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_LEVEL, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
			return false;
		});
		StandardExtend::testAndOut("BT 层序", resultReal, std::string("1 2 5 3 4 6"));
	};
	
	/*
	先根序遍历堆栈
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
	
	结构
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

	// char比较好测试
	// 先序[1] 2  3  4  5  6
	// { 1, 2, 3, 4, 5, 6 };
	char preOrder[size + 1] = "123456";
	// 中序 3  2  4 [1] 6  5
	// { 3, 2, 4, 1, 6, 5 }
	char inOrder[size + 1] = "324165";
	// 后序 3  4  2  6  5 [1]
	// { 3, 4, 2, 6, 5, 1 }
	char postOrder[size + 1] = "342651";
	LinkedBinTree<char> btIns2 = LinkedBinTree<char>(preOrder, inOrder, size);
	subTest(btIns2);
	LinkedBinTree<char> btIns3 = LinkedBinTree<char>(size, inOrder, postOrder);
	subTest(btIns3);
	// 拷贝构造
	LinkedBinTree<char> btIns4 = btIns3;
	subTest(btIns4);

	auto newBtFun = [&size, &inOrder, &postOrder]() {
		return LinkedBinTree<char>(size, inOrder, postOrder);
	};

	// 移动构造(@TODO 此处不是移动构造)
	LinkedBinTree<char> btIns5(newBtFun());
	btIns5 = LinkedBinTree<char>(size, inOrder, postOrder);
	btIns5.clear();
	btIns5 = LinkedBinTree<char>(size, inOrder, postOrder);
	subTest(btIns5);
	// 这算stl的bug吗? 一定要比原空间大才能复制
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
		// 镜像先序与原后序相反
		StandardExtend::testAndOut("BT 镜像先根序", resultReal, std::string("1 5 6 2 4 3"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_POST_ROOT, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
		});
		// 镜像后序与原先序相反
		StandardExtend::testAndOut("BT 镜像后根序", resultReal, std::string("6 5 4 3 2 1"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_INFIX_ROOT, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
		});
		// 镜像中序与原中序相反
		StandardExtend::testAndOut("BT 镜像中根序", resultReal, std::string("5 6 1 4 2 3"));

		resultReal.clear();
		btIns.traversal(Tree::ORDER_LEVEL, [&resultReal](BinTree<char>::BT bNode) {
			std::string tmp;
			tmp += bNode->Data;
			StandardExtend::formatStrAppend(resultReal, tmp);
			return false;
		});
		// 镜像层序与原层序关系难以描述(如果是一颗完全二叉树的话就是每2^d与原层序反转 d为深度)
		StandardExtend::testAndOut("BT 镜像层序", resultReal, std::string("1 5 2 6 4 3"));
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

	构造顺序字符序列 0~7 共8个结点
		抽象结构
			   3
		   2       7
		 0   -   4   6
		1 -         5 -

		存储结构
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
	抽象结构
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

	// Bintree共计测试16个方法, 两组用例(实际上只有一组是完全测试的) 公有方法19个, 基本覆盖
	// VirtualLinkedBinTree共计测试3个方法, 两组用例, 公有方法3个, 完全覆盖
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
	StandardExtend::testAndOut(insName + "先根序", resultBufferReal, resultBuffer);

	resultBufferReal.clear();
	bst.traversal(LinkedBinSearchTree<std::string>::ORDER_SEQUENCE, [&resultBufferReal](const LinkedBinSearchTree<std::string>::BT node) {
		StandardExtend::formatStrAppend(resultBufferReal, node->Data);
		return node->Data == string("5") ? true : false;;
	});
	// 0 1 2 3 4 5
	std::string subS = resultBuffer.substr(0, resultBuffer.length() - resultBuffer.find('4'));
	StandardExtend::testAndOut(insName + "顺序中止", resultBufferReal, subS);

	resultBufferReal.clear();
	bst.traversal(LinkedBinSearchTree<std::string>::ORDER_REVERSE, [&resultBufferReal](LinkedBinSearchTree<string>::BT node) {
		StandardExtend::formatStrAppend(resultBufferReal, node->Data);
		return node->Data == string("5") ? true : false;
	});
	std::string subR = resultBuffer.substr(resultBuffer.find('5'), resultBuffer.length() - resultBuffer.find('5'));
	std::reverse(subR.begin(), subR.end());
	StandardExtend::testAndOut(insName + "倒序中止", resultBufferReal, subR);

	resultBufferReal.clear();
	bst.traversal(LinkedBinSearchTree<std::string>::ORDER_REVERSE, [&resultBufferReal](LinkedBinSearchTree<std::string>::BT node) {
		StandardExtend::formatStrAppend(resultBufferReal, node->Data);
		return false;
	});
	std::string bufR = resultBuffer;
	std::reverse(bufR.begin(), bufR.end());
	StandardExtend::testAndOut(insName + "倒序", resultBufferReal, bufR);

	int idel = 0;
	while (!bst.empty()) {
		bst.erase(std::to_string(idel++));
	}
	resultBufferReal.clear();
	bst.traversal(LinkedBinSearchTree<std::string>::ORDER_REVERSE, [&resultBufferReal](LinkedBinSearchTree<std::string>::BT node) {
		StandardExtend::formatStrAppend(resultBufferReal, node->Data);
		return false;
	});
	StandardExtend::testAndOut(insName + "删除变空", resultBufferReal, std::string(""));
	
	bst.insert("sss");
	bst.clear();
	resultBufferReal.clear();
	bst.traversal(LinkedBinSearchTree<std::string>::ORDER_REVERSE, [&resultBufferReal](LinkedBinSearchTree<std::string>::BT node) {
		StandardExtend::formatStrAppend(resultBufferReal, node->Data);
		return false;
	});
	StandardExtend::testAndOut(insName + "clear变空", resultBufferReal, std::string(""));

	return 9;
}
int LessIntegerCmper(int const &lhs, int const &rhs) {
	// return lhs - rhs;// 容易越界
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

	// 1. 最小堆重建为最大堆(数据清空后push) PS: 哨兵不是MIN_INT32, 因此可以排除比较方法的越界错误
	Heap<int> heapIns = Heap<int>(heapData.size() + 4, -1, GreaterIntegerCmper, &heapData[0], (int)heapData.size());
	heapIns.clear();
	heapIns.initialize(MAX_INT32, LessIntegerCmper);
	heapIns.rebuild();
	for (JCE::SizeType i = 0; i < heapData.size(); heapIns.push(heapData[i++]));
	std::string resultHeapStr;
	heapIns.traversal(Tree::ORDER_LEVEL, [&resultHeapStr](BinTree<int>::BT b) {
		StandardExtend::formatStrAppend(resultHeapStr, std::to_string(b->Data));
	});
	StandardExtend::testAndOut("Heap-层序", resultHeapStr, std::string("15 15 13 11 14 7 8 10 9 6 12 3 5 0 4 1 2"));
	heapRealDataList.clear();
	while (!heapIns.empty()) {
		heapRealDataList.emplace_back(heapIns.pop());
	}
	StandardExtend::testAssert(heapRealDataList, std::vector<int>({ 15,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 }));

	// 2. 使用新的数据构建
	heapIns.build(&heapData[0], (int)heapData.size(), MIN_INT32, GreaterIntegerCmper);
	heapRealDataList.clear();
	while (!heapIns.empty()) {
		heapRealDataList.emplace_back(heapIns.pop());
	}
	StandardExtend::testAssert(heapRealDataList, std::vector<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15 }));

	// 3. 使用新的数据构建(上一个用例已确认此方法可以重建出合理的堆)
	heapIns.build(&heapData[0], (int)heapData.size(), MIN_INT32, GreaterIntegerCmper);
	// 最小堆重建为最大堆(数据不变)
	heapIns.initialize(MAX_INT32, LessIntegerCmper);
	heapIns.rebuild();
	heapRealDataList.clear();
	while (!heapIns.empty()) {
		heapRealDataList.emplace_back(heapIns.pop());
	}
	StandardExtend::testAssert(heapRealDataList, std::vector<int>({ 15,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 }));

	// 4. build后的链接测试
	heapIns.clear();
	resultHeapStr.clear();
	heapIns.build(&heapData[0], (int)heapData.size(), MIN_INT32, GreaterIntegerCmper);
	heapIns.traversal(Tree::ORDER_LEVEL, [&resultHeapStr](BinTree<int>::BT b) {
		StandardExtend::formatStrAppend(resultHeapStr, std::to_string(b->Data));
	});
	StandardExtend::testAndOut("Heap-层序", resultHeapStr, std::string("0 1 3 2 9 5 4 11 10 15 14 7 13 6 8 12 15"));

	// 花式rebuild, 一组数据, 共3组用例
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

	// rebuild + clear + push + traversal, p1:clear后直接push的话root可能没有更新
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
	StandardExtend::testAndOut("Heap-层序", resultHeap, std::string("0 6 1 8 9 5 2 10 12 15 14 7 13 4 3 15 11"));
	
	heapRealDataList.clear();
	while (!heapIns.empty()) {
		heapRealDataList.emplace_back(heapIns.pop());
	}
	// "Heap-Pop With push"
	StandardExtend::testAssert(heapRealDataList, std::vector<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15 }));

	// 新测试的只有 clear 这1个公有方法
	return 1;
}
int subTestForHeap() {
	// 堆是完全二叉树 但不是二叉搜索树
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
	StandardExtend::testAndOut("Heap-层序", resultHeapStr, std::string("0 6 1 8 9 5 2 10 12 15 14 7 13 4 3 15 11"));
	heapRealDataList.emplace_back(heapIns.pop());
	resultHeapStr.clear();
	heapIns.traversal(Tree::ORDER_LEVEL, [&resultHeapStr](BinTree<int>::BT b) {
		StandardExtend::formatStrAppend(resultHeapStr, std::to_string(b->Data));
	});
	// 中间过程是否正常链接
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
	// 主要是 构造 析构 空rebuild push pop empty 在内的5个公有方法
	return 6 + subTestForHeapClearBuild() + subTestForHeapRebuild();
}
int testForHuffumanTree() {
	/*
	小写字母，01反、且2点对换；有2点重合
	A 1 B 1 C 1 D 3 E 3 F 6 G 6
	*/
	// 待编码元素个数
	const int CH_N = 7;
	char chArr[CH_N + 1] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', '\0'};
	int freqArr[CH_N] = { 1, 1, 1, 3, 3, 6, 6 };
	const int WPL_STAND = 6*2 + 6*2 + 3*2 + 3*3 + 4*1 + 5*1 + 5*1;
	char codesArr[CH_N][CH_N + 1] = { "10001", "10000", "1001", "11", "101", "01", "00"}; // 最优前缀码之一

	vector<std::string> codesArr1 = { "01010", "01011", "0100", "011", "10", "11", "00"}; // 最优前缀码之一
	vector<std::string> codesArr2 = { "00000", "00001", "0001", "001", "01", "10", "11"}; // 最优前缀码之一
	std::string codesArr3[CH_N] = { "000", "001", "010", "011", "100", "101", "110"}; // 无关最优前缀码
	char codesArr4[CH_N][CH_N + 1] = { "00000", "00001", "0001", "001", "00", "10", "11"}; // 非前缀码
	std::string codesArr5[CH_N] = { "000000", "000001", "00001", "0001", "001", "01", "1"}; // 前缀码 非最优

	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr, codesArr + CH_N), true);
	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr1.begin(), codesArr1.end()), true);
	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr2.begin(), codesArr2.end()), true);
	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr3, codesArr3 + CH_N), true);
	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr4, codesArr4 + CH_N), false);
	StandardExtend::testAssert(HuffmanTree<char>::isPrefixCodes(codesArr5, codesArr5 + CH_N), true);

	HuffmanTree<char> huffT(chArr, freqArr, CH_N);
	StandardExtend::testAssert(huffT.wpl(), WPL_STAND);
	// 带权路径长度WPL<==>最短文本编码长度
	// 哈夫曼的WPL<==>哈夫曼编码长度<==>最优前缀码的文本编码长度
	//  codingLen
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr), std::end(codesArr), freqArr), WPL_STAND);
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr1), std::end(codesArr1), freqArr), WPL_STAND);
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr2), std::end(codesArr2), freqArr), WPL_STAND);
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr3), std::end(codesArr3), freqArr) == WPL_STAND, false);
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr4), std::end(codesArr4), freqArr), WPL_STAND);
	StandardExtend::testAssert(HuffmanTree<char>::textLen(std::begin(codesArr5), std::end(codesArr5), freqArr) == WPL_STAND, false);
	// string const codes[]          最方便
	// LinkedList<std::string> codes 动态性强
	// ArrayList<std::string> codes  随机访问性高
	// ArrayList<char const*> codes  内存需要自己管理, 构造及其不方便(需要手动一个个push_back进去)
	// Iterator模板					 兼容性上述所有, 当接口比较合适

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
	// 0号是空结点
	for (int i = 1; i < N_HT; ++i) {
		StandardExtend::testAssert(ht[i].weight, resultHt[i - 1][0]);
		StandardExtend::testAssert(ht[i].parent, resultHt[i - 1][1]);
		StandardExtend::testAssert(ht[i].lchild, resultHt[i - 1][2]);
		StandardExtend::testAssert(ht[i].rchild, resultHt[i - 1][3]);
	}

	// 计算编码长度(基于数组式哈夫曼编码树)
	auto ArrayHuffCodeLen = [&huffmanCodes](int *hufWeightList, int N_HT) {
		huffmanCodes.clear();
		ArrayHuffman arrHufT1(hufWeightList, N_HT, huffmanCodes);
		return HuffmanTree<char>::textLen(huffmanCodes.begin(), huffmanCodes.end(), hufWeightList);
	};
	int hufWeightList1[] = { 1, 2, 3, 4, 5 };
	int hufWeightList2[] = { 3, 8, 8 };
	int hufWeightList3[] = { 1, 1, MAX_INT32-4 };
	StandardExtend::testAndOut("huffCodeLen: ", ArrayHuffCodeLen(hufWeightList1, 5), 33);
	StandardExtend::testAndOut("等权huffCodeLen: ", ArrayHuffCodeLen(hufWeightList2, 3), 30);
	StandardExtend::testAndOut("临界值huffCodeLen: ", ArrayHuffCodeLen(hufWeightList3, 3), MAX_INT32);

	// 4个数组式公有方法 + 5个链接式公有方法(含构造析构) PS: 共一组用例, 其中有多组测试数据
	return 4 + 5;
}

int testForExpressionTree() {
	//自己改改
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

	// 虚存树: 构造器指定函数指针的实现从而实现多态
	std::cout << " ====== Tree test end 测试点小计: " << sumTestCnt << std::endl;
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
		// 由0号顶点出发的最短距离值: 
		// StandardExtend::outPutIterable(dist.begin(), dist.end(), 2);
		StandardExtend::testAssert(dist, resultDist);
		// 由0号顶点出发的最短路径: 
		// StandardExtend::outPutIterable(path.begin(), path.end(), 2);
		StandardExtend::testAssert(path, resultPath);
	};
	auto judgeGraph = [&judgeGraphFun](Graph *&&g) {
		for (int i = 0; i < 10; ++i) {
			g->insertEdge(Graph::Edge(0, i, 10));
			// 重复添加视为更新权值
			g->insertEdge(Graph::Edge(0, i, 11));
		}
		StandardExtend::testAssert(g->getEdgeNum(), 10u);
		for (int i = 0; i < 10; ++i) {
			g->insertEdge(Graph::Edge(1, i, 5));
			g->insertEdge(Graph::Edge(i, 1, 5));
		}
		// 总共30条有向边, 0->1和1->0这两条分别重复添加了两次不增加总边数
		// 需要注意的是: 自己到自己也算一条边(可以理解为停留在原地的消费) 当然 最短路径时出发点是自己 因此与自己的距离是0 这么实现不知道有没有问题
		StandardExtend::testAssert(g->getEdgeNum(), 28u);
		// 0=11=>所有(除开0) 1<=5=>所有
		judgeGraphFun(*g, std::vector<int>({ 0,5,10,10,10,10,10,10,10,10 }), std::vector<int>({ -1,0,1,1,1,1,1,1,1,1 }));

		// 负权正常情况(9 =-2=> 8)
		// 0=9=>8: 0=11-> 9=-2=> 8
		// 0=8=>8: 0=5-> 1=5=> 9=-2=> 8
		g->insertEdge(Graph::Edge(9, 8, -2));
		judgeGraphFun(*g, std::vector<int>({ 0,5,10,10,10,10,10,10,8,10 }), std::vector<int>({ -1,0,1,1,1,1,1,1,9,1 }));

		delete g; g = nullptr;
	};
	judgeGraph(new AdjacentMatrixGraph(10));
	judgeGraph(new AdjacentListGraph(10));
	// 3*3坐标图 共9个顶点
	CoordinatesMap cg(3, 3, DirCountEnum::DIR8);
	// 默认全是无效的坐标点 此句只用于演示API
	cg.setInvalid(1, 0);
	// 设置是否可以通过 这两个相邻的有效点视为为这两个坐标点之间存在无向边
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
	// 转为等权图
	judgeGraphFun(cg.parityGraph(), 
		std::vector<int>({ 0,1,2,Graph::INF,Graph::INF,3,Graph::INF,Graph::INF,Graph::INF }),
		std::vector<int>({ -1,0,1,-1,-1,2,-1,-1,-1 })
	);
	// 转为权重图(负权回路->最短路执行失败)
	judgeGraphFun(cg.parityGraph(true),
		std::vector<int>({ -16,-15,-14,Graph::INF,Graph::INF,Graph::INF,Graph::INF,Graph::INF,Graph::INF }),
		std::vector<int>({ 1,0,1,-1,-1,-1,-1,-1,-1 }),
		false
	);

	//设置有效的同时设置顶点值(转换为权重)
	//由于默认的值可能是任何值(实际是-1 但不应该把这个当成有效值)而没有设置值的有效顶点转换为权值就会出问题, 两个API最好不要混用
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

// 非线性结构测试主函数
int mainForNonlinearStructure() {
	int sumTestCnt = 0;
	sumTestCnt += testForTree();
	StandardExtend::testAndDiffClock(testForGraph);
	StandardExtend::testAndDiffClock(testForGraphTemplate);
	return 0;
}

int runDataStructureTest() {
	/*
	lambda表达式(匿名函数)
		P345
		[捕获列表](函数签名) {函数体};
		捕获列表:
			lambda是通过创建一个重载了操作符()的小类来实现的，一个lambda函数是该类的一个实例
			捕获lambda函数外的具有自动存储时期的变量。函数体与这些变量的集合合起来叫闭包。
			一个没有指定任何捕获的lambda函数,可以显式转换成一个具有相同声明形式函数指针
			但当捕获列表出现时，其入口包含了实例化时对捕获对象的封装，而这个捕获对象的实体是以类成员的身份存在的，因此无法被转化成函数指针

	 __vfptr: 虚函数表vtable指针
	 多态实现: 函数指针, 虚函数
	*/
	mainForLinearStructure();
	mainForNonlinearStructure();
	return 0;
}
