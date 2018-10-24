#pragma warning(disable:4996)
#include "stdafx.h"
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "ExtendSpace.h"
#include "Graph\TreeObject.h"
#include "Graph\Graph.h"
//using namespace StandardExtend;
using namespace std;

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
	puts(" ===== Stack test end");
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
			s.push("List data in Stack");
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

	cout << " ====== LinkedList test end" << endl;
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
	puts(" ====== Queue test end");
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
	qs.shell();
	//ql.shell();
	puts(" ====== Deque test end");
	return 0;
}

int mainForExpressionTree() {
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

int mainForBinSearchTree() {
	BinSearchTree<string> bt = BinSearchTree<string>();
	for (int i = 0; i < 10; ++i) {
		bt.insert(string("BST data") + std::to_string(i));
	}
	puts("顺序");
	bt.traversal(Tree::ORDER, [&](BinTree<string>::BT node) {
		cout << node->Data << endl;
		return false;
	});
	puts("倒序 + 中止");
	bt.traversal(Tree::RE_ORDER, [&](BinTree<string>::BT node) {
		cout << node->Data << endl;
		return node->Data == string("BST data5") ? true : false;
	});
	puts("先根序");
	bt.traversal(Tree::ORDER_INFIX_ROOT, [&](BinTree<string>::BT node) {
		cout << node->Data << endl;
		//无效返回值
		return false;
	});
	cout << bt.find(string("BST data5"))->Data << endl;
	puts(" ====== Tree test end");
	return 0;
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
int mainForHuffumanTree()
{
	const int N = 63;
	int CodeWPL;//标准
	int i, n, Freq[N + 1] = { 0 };
	//int j, m;
	char ch[N + 1], s[N][2 * N];
	while (~scanf("%d\n", &n)) {
		Freq[0] = n; memset(s, 0, N*N);
		memset(ch, 0, N + 1); ch[0] = 32;//初始化
		for (i = 1; i <= n; i++) {
			ch[i] = getchar();
			scanf("%d", Freq + i);
			getchar();
		}
		HuffmanTree<char> T(ch, Freq, n);
		CodeWPL = T.wpl();
	}return 0;
}

void outPutGraph(Graph &g) {
	cout << "边数: " << g.getEdgeNum() << endl;
	StandardExtend::ArrayList<Graph::VertexKey> dist, path;
	g.shortestPath(0, dist, path);
	cout << "由0号顶点出发的最短距离值: " << endl;
	StandardExtend::outPutIterable(dist.begin(), dist.end(), 2);
	cout << "由0号顶点出发的最短路径: " << endl;
	StandardExtend::outPutIterable(path.begin(), path.end(), 2);
	puts("");
}

void NewGraphDemo(Graph *&&g) {
	for (int i = 0; i < 10; ++i) {
		g->insertEdge(Graph::Edge(0, i, 10));
		//重复添加视为更新
		g->insertEdge(Graph::Edge(0, i, 11));
	}
	for (int i = 0; i < 10; ++i) {
		g->insertEdge(Graph::Edge(1, i, 5));
		g->insertEdge(Graph::Edge(i, 1, 5));
	}
	outPutGraph(*g);
	delete g; g = nullptr;
}

int mainForGraph() {
	NewGraphDemo(new AdjacentMatrixGraph(10));
	NewGraphDemo(new AdjacentListGraph(10));
	//3*3坐标图 共9个顶点
	//CoordinatesMap cg(3, 3, Graph::DirCountEnum::DIR8);
	CoordinatesMap cg(3, 3, DirCountEnum::DIR8);
	//默认全是无效的坐标点 此句只用于演示API
	cg.setInvalid(1, 0);
	//设置是否可以通过 这两个相邻的有效点视为为这两个坐标点之间存在无向边
	cg.setValid(0, 0);
	cg.setValid(0, 1);
	cg.setValid(0, 2);
	cg.setValid(1, 2);
	/*
	01 01 01
	-- -- 01
	-- -- --
	*/
	cg.output();
	outPutGraph(cg.parityGraph());

	//设置有效的同时设置顶点值(转换为权重)
	//由于默认的值可能是任何值(实际是-1 但不应该把这个当成有效值)而没有设置值的有效顶点转换为权值就会出问题, 两个API最好不要混用
	cg.setValidVertex({ 0, 0 }, 00);
	cg.setValidVertex({ 0, 1 }, 01);
	cg.setValidVertex({ 0, 2 }, 02);
	cg.setValidVertex({ 1, 2 }, 12);
	/*
	00 01 02
	-- -- 12
	-- -- --
	*/
	cg.output();
	outPutGraph(cg.parityGraph(true));
	cout << " ====== Graph test end" << endl;
	return 0;
}

template<class T>
bool testAndOut(JCE::String const &name, T realValue, T expectValue) {
	assert(realValue == expectValue);
	cout << name + " 实际值: " << realValue << "; 期望值: " << expectValue << endl;
	return true;
}

int mainForMath() {
	testAndOut("2018年是否闰年: ", (bool)StandardExtend::isIntercalary(2018), false);
	testAndOut("2018-10-22是2018年的第几天: ", StandardExtend::calcNumberOfDays(2018, 10, 22), 295);

	puts("全排列: ");
	MathExtend::pentration(3);
	puts("阶乘: ");
	printf("%.2f\n", MathExtend::fact(10));
	testAndOut("组合数 3取2", MathExtend::C(3, 2), 3);
	testAndOut("组合数 3取1", MathExtend::C(3, 1), 3);
	testAndOut("组合数 49取6", MathExtend::C(49, 6), 13983816);
	testAndOut("排列数 3取2", MathExtend::A(3, 2), 6);
	testAndOut("排列数 8取3", MathExtend::A(8, 3), 336);


	for (I64 i = 0; i < 20; ++i) {
		testAndOut("快速幂 "+ std::to_string(i) +"^2", MathExtend::quickPow(i, 2), (I64)std::pow(i, 2));
	}
	int testCount = 500;
	StandardExtend::testAndDiffClock([&]() {
		for (int i = 0; i < testCount; ++i) {
			std::pow(2, 62);
		}
	}, "std::pow");
	StandardExtend::testAndDiffClock([&]() {
		for (int i = 0; i < testCount; ++i) {
			MathExtend::quickPow(2, 62);
		}
	}, "普通快速幂");
	StandardExtend::testAndDiffClock([&]() {
		for (int i = 0; i < testCount; ++i) {
			MathExtend::quickPow(2, 62, MAX_INT64);
		}
	}, "带mod的快速幂");
	StandardExtend::testAndDiffClock([&]() {
		for (int i = 0; i < testCount; ++i) {
			MathExtend::quickPow_OLD(2, 62, MAX_INT64);
		}
	}, "基于quickFact的快速幂");
	testAndOut("快速幂 10^18", MathExtend::quickPow_OLD(10, 18, MAX_INT64), (I64)std::pow(10, 18));
	for (int i = MAX_INT32-1; i < MAX_INT32; ++i) {
		auto a = MathExtend::powLastBit(i, 1) % 10;
		auto b = MathExtend::quickPow(i, 2, 10);
		testAndOut(std::to_string(i) + "^2" + "的最后1bit", a, b);
	}

	auto primeTable = MathExtend::generateSievePrimeS(100);
	int count = 0;
	for (int i = 0; i < 50; ++i) {
		bool isP = MathExtend::isPrime(i);
		testAndOut("数字" + to_string(i) + " 是否素数:", isP, primeTable[i] == 0);
	}
	free(primeTable);



	Utility::Double::setEPS(EPS_DOUBLE);
	auto f1 = [](double x) {
		return pow(x, 2) + 2 * x + 1;
	};
	auto x1 = MathExtend::trichotomy<double, double>(-10.0, 10.0, [&](double x) {
		//当x=-1.0时 y最小=0.0
		return f1(x);
	}, EPS);
	testAndOut("一元二次方程最小近似解: ", Utility::Double(x1), Utility::Double(-1.0));
	testAndOut("一元二次方程最小近似值: ", Utility::Double(f1(x1)), Utility::Double(0.0));

	auto f2 = [](double x) {
		return -pow(x, 2) + 2 * x + 1;
	};
	auto x2 = MathExtend::trichotomy<double, double>(-10.0, 10.0, [&](double x) {
		//当x=1.0时 y最大=2.0 (此方法只提供最小值 故最前面添'-'号)
		return -(f2(x));
	}, EPS);
	testAndOut("一元二次方程最大近似解: ", Utility::Double(x2), Utility::Double(1.0));
	testAndOut("一元二次方程最大近似值: ", Utility::Double(f2(x2)), Utility::Double(2.0));

	const int size = 9;
	int a[size] = { 6, 5, 3, 0, 1, 4, 2, 6, 7};
	Utility::BubbleSort(a, size, size, Utility::lessQsortCmp<int>);
	Utility::SlectSort(a, a + size, Utility::moreQsortCmp<int>);
	std::sort(a, a + size);
	auto it1 = MathExtend::dichotomy(a, size, 6);
	bool exist = std::binary_search(a, a + size, 6);
	auto pair = std::equal_range(a, a + size, 6);

	StandardExtend::testAndDiffClock([&]() {
		I64 x, y;
		testAndOut("2*x + 1*y + 5 = 0的最小非负整数解x是否存在: ", (bool)MathExtend::linearEquationCondition1(2LL, x, 1LL, y, 5LL), true);
		testAndOut("解x: ", x, 0LL);
		testAndOut("解y: ", y, 5LL);
	}, "二元一次线性方程最小非负整数解");
	
	int aaInput[][3] = {
		{700 ,300, 200 },
		{500 ,200, 300 },
		{500 ,200, 500 },
		{275 ,110, 330 },
		{275 ,110, 385 },
		{648 ,375, 4002},
		{3, 1 ,10000 }
	};
	int aaOutput[][2] = {
		{1, 3},
		{1, 1},
		{1, 0},
		{0, 3},
		{1, 1},
		{49, 74},
		{3333, 1}
	};
	StandardExtend::testAndDiffClock([&]() {
		int x, y;
		for (int i = 0; i < 7; ++i) {
			MathExtend::linearEquationCondition2(aaInput[i][0], x, aaInput[i][1], y, aaInput[i][2]);
			x = abs(x);
			y = abs(y);
			testAndOut("解x: ", x, aaOutput[i][0]);
			testAndOut("解y: ", y, aaOutput[i][1]);
		}
	}, "二元一次线性方程 满足|x| + |y|最小的解");
	StandardExtend::testAndDiffClock([&]() {
		int x, y, x1, y1, x2, y2;
		for (int i = 0; i < 7; ++i) {
			MathExtend::linearEquationCondition1(aaInput[i][0], x1, aaInput[i][1], y1, aaInput[i][2]);
			//反的
			MathExtend::linearEquationCondition1(aaInput[i][1], x2, aaInput[i][0], y2, aaInput[i][2]);
			if ((abs(x1) + abs(y1)) < (abs(x2) + abs(y2))) {
				x = abs(x1);
				y = abs(y1);
			}
			else {
				//这里是反的
				x = abs(y2);
				y = abs(x2);
			}
			testAndOut("解x: ", x, aaOutput[i][0]);
			testAndOut("解y: ", y, aaOutput[i][1]);
		}
	}, "二元一次线性方程 满足|x| + |y|最小的解");
	
	StandardExtend::testAndDiffClock([&]() {
		int m[] = {3, 5, 7};
		int a[] = {2, 3, 2};
		int minResult = MathExtend::chineseReminder(m, a, 3, 0);
		testAndOut("最小正整数解: ", minResult, 23);
	}, "中国剩余定理 求线性同余方程组");

	StandardExtend::testAndDiffClock([&]() {
		I64 illTable[] = {0, 0, 1, 2, 9, 44, 265, 1854 };
		JCE::ArrayList<I64> illArrangeList;
		MathExtend::buildIllArrangeList(illArrangeList);
		for (JCE::SizeType i = 0; i < 8; ++i) {
			testAndOut("错排Value: ", illArrangeList[i], illTable[i]);
		}
		StandardExtend::outPutIterable(illArrangeList.begin(), illArrangeList.end(), 0, '\0', ',');
		StandardExtend::outPutIterable(illArrangeList.begin(), illArrangeList.end(), 20, '\0', (JCE::SizeType)5);
		StandardExtend::outPutIterable(illArrangeList.begin(), illArrangeList.end(), 20, '\0', [](auto left) {
			static int count = 0;
			return ++count % 5 == 0;
		});
	}, "错排表");

	cout << "打印n行的杨辉三角" << endl;
	int table[10][StandardExtend::MAX_C] = { 0 };
	MathExtend::buildPtriangleTable(table, 10);
	StandardExtend::outPut2DArrayTrangle(table, 10, 3);

	cout << " ====== Math test end" << endl;
	return 0;
}

int mainForAlgorithm() {
	//@see http://acm.hdu.edu.cn/showproblem.php?pid=2602
	//物品数量, 背包容量, 解题用动态规划数组大小
	const int itemNum = 5, cap = 10, dpSize = cap + 1;
	int dp[dpSize] = { 0 };
	int cost[itemNum] = { 5, 4, 3, 2, 1 }, value[itemNum] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < itemNum; ++i) {
		MathExtend::ZeroOneBackpackProcess(dp, cap, cost[i], value[i]);
		//MathExtend::ZOBackpackProcess(dp, cap, 0, value[i]);
	}
	StandardExtend::outPutIterable(dp, dp + dpSize);

	memset(dp, 0, dpSize);
	for (int i = 0; i < itemNum; ++i) {
		MathExtend::CompletePackProcess(dp, cap, cost[i], value[i]);
	}
	StandardExtend::outPutIterable(dp, dp + dpSize);

	memset(dp, 0, dpSize);
	//{ 1,2,3,4,5,6,7,8,9,10 };
	int number[itemNum] = { 1,2,3,4,5 };
	for (int i = 0; i < itemNum; ++i) {
		MathExtend::MultiplyPackProcess(dp, cap, cost[i], value[i], number[i]);
	}
	StandardExtend::outPutIterable(dp, dp + dpSize);

	cout << " ====== Algorithm test end" << endl;

	return 0;
}

int main() {
	//FILE *inFile = stdin, *outFile;
	//freopen("input", "r", stdin);
	//freopen_s(&inFile, "input", "r", stdin);
	//mainForExpressionTree();
	//mainForHuffumanTree();
	mainForMath();
	StandardExtend::testAndDiffClock(mainForAlgorithm);
	StandardExtend::testAndDiffClock(mainForGraph);
	mainForBinSearchTree();
	MainForStack();
	mainForQueue();
	mainForDeQueue();
	MainForLinkedList();
	return 0;
}

