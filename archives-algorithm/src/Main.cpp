#pragma warning(disable:4996)
#include "stdafx.h"
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "ExtendSpace.h"
#include "Graph\TreeObject.h"
#include "Graph\Graph.h"
#include "BigInteger.h"
#include "MathLibrary.h"
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
	for (int i = 0; i < 5; ++i) {
		lists.insertData("LinkedListData");
	}
	auto it = lists.findKth(1);
	while (it) {
		cout << *it << endl;
		++it;
	}
	
	LinkedList<stack<string>> stackList;
	for (int i = 0; i < 5; ++i) {
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
	JCE::ArrayList<int> listData1 = { -1, 1, 3, 5, 7, 9, 11, 13 };
	JCE::ArrayList<int> listData2 = { -2, 2, 4, 6, 8, 10, 12, 14 };
	for (JCE::SizeType i = 0; i < listData1.size(); ++i) {
		list1.insertData(listData1[i]);
		list2.insertData(listData2[i]);
	}
	list1.output();
	list2.output();
	puts("begain\n");

	puts("反转后list2\n");
	list2.reverse().output();
	puts("再次反转list2\n");
	list2.reverse().output();

	puts("有序合并list2到list1\n");
	list1.merge(list2).output();
	LinkedList<int>::Iterator iter = list1.findKth(1);
	n = list1.length();
	int i = 0;
	while (true) {
		++i;

		LinkedList<int>::Iterator findIter = list1.findKth(i);
		if (findIter) {
			list3.insertInP(*findIter, findIter);
			printf("find 正数第%d个元素%d\n", i, *findIter);
		}
		else {
			puts("find error");
		}

		iter = list1.findKth(-1);
		if (iter) {
			printf("find 倒数第%d个元素%d\n", 1, *iter);
		}
		else {
			puts("last element is not exist, exit!");
			break;
		}

		int deleteTemp = *iter;
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
	//qs.shell();
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
int lessCmper(int const &lhs, int const &rhs) {
	return lhs - rhs;
}
int moreCmper(int const &lhs, int const &rhs) {
	return -lessCmper(lhs, rhs);
}
int mainForTree() {
	char post[10] = "hfbdcea";
	char in[10] = "hbfadec";
	char pre[10] = "";
	BinTree<char> binTreeIns;
	binTreeIns.orderTranslation(pre, nullptr, in, post, 7);

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

	int b[10] = { 2, 1, 4, 5, 9, 3, 6, 7 };
	AvlTree<int> avlTreeIns = AvlTree<int>();
	for (int i = 0; i < 8; ++i) {
		avlTreeIns.insert(b[i]);
	}
	puts("avl-倒序");
	avlTreeIns.traversal(Tree::RE_ORDER, [&](BinTree<int>::BT node) {
		cout << node->Data << endl;
		return false;
	});

	JCE::ArrayList<int> a = { 6, 15, 3, 9, 7, 4, 12, 10, 15, 14, 5, 13 };
	Heap<int> heapIns = Heap<int>(a.size() + 5);
	heapIns.build(-1, moreCmper);
	for (JCE::SizeType i = 0; i < a.size(); heapIns.push(a[i++]));
	heapIns.pop();
	heapIns.pop();
	heapIns.pop();
	heapIns.push(8);
	heapIns.push(11);
	puts("堆-层序");
	heapIns.traversal(heapIns.ORDER_LEVEL, [&](BinTree<int>::BT b) {
		printf("%d ", b->Data);
		return false;
	});

	puts("");
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
	JCE::ArrayList<Graph::VertexKey> dist, path;
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


	for (I64 i = 0; i < 10; ++i) {
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
	for (int i = 0; i < 10; ++i) {
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
	//顺便排序测试
	int a[size] = { 6, 5, 3, 0, 1, 4, 2, 6, 7};
	Utility::BubbleSort(a, size, size, Utility::lessQsortCmp<int>);
	Utility::SlectSort(a, a + size, Utility::moreQsortCmp<int>);
	int aa[] = { 44, 12, 59, 36, 62, 43, 94, 7, 35, 52, 85 };
	Utility::shellSort<int>(aa, 11);

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

	StandardExtend::testAndDiffClock([&]() {
		double a = 3.0, b = 4.0, c = 5.0;
		vector<Utility::Double> temper;
		temper.push_back(a);
		temper.push_back(b);
		temper.push_back(c);
		if (Utility::Triangle::isTrangle(a, b, c)) {
			unsigned count = Utility::Triangle::equalityElementCount(temper);
			puts(0 == count ? "yb"
				: (count == 3 ? "db" : "dy"));
		}
		else {
			puts("no");
		}
	}, "三角形类型判断");

	cout << " ====== Algorithm test end" << endl;

	return 0;
}

int mainForBigInteger() {
	int d = 3;
	BigInteger a(1), b(2), c(d);
	//int d = c;
	//d *= a;
	testAndOut("大数四则测试 加法: ", a + b == c, true);
	testAndOut("大数四则测试 乘法: ", a * b == b, true);
	testAndOut("大数加法交换律1: ", a + b == b + a, true);
	testAndOut("大数加法交换律2: ", 1 + c == c + 1, true);
	testAndOut("大数四则测试 +=: ", a.plus(b) == (c += 0), true);
	testAndOut("大数四则测试 ==以及上一次的效果: ", a == c, true);
	testAndOut("大数乘法交换律1: ", (b * a) == c*2, true);
	testAndOut("大数乘法交换律2: ", (a * b) == 2*c, true);
	testAndOut("大数四则测试: ", c + 1 == 4, true);
	//cin >> c;
	cout << "cout 测试: " << c << endl;

	a = 1;
	//2=a*2
	a.plus(a);
	//4=a*=4
	a.plus(a);
	testAndOut("自运算", a, BigInteger(4));

	testAndOut("自运算", a.multiply(a), BigInteger(16));
	testAndOut("自运算", a, BigInteger(4));
	//16=4*4
	a.muity(a);
	testAndOut("自运算", a, BigInteger(16));
	//256 = 16*16
	a *= a;
	testAndOut("自运算", a, BigInteger(256));
	a = a * a;
	testAndOut("自运算", a, BigInteger(65536));

	testAndOut("偶数判断", a.isEvenNumber(), true);
	testAndOut("偶数判断", (++a).isEvenNumber(), false);

	string illResultStr = "34332795984163804765195977526776142032365783805375784983543400282685180793327632432791396429850988990237345920155783984828001486412574060553756854137069878601";
	BigInteger bigNumForIllResilt = BigInteger(illResultStr);
	char ill100ReslS[500] = "";
	JCE::ArrayList<BigInteger> illList;
	MathExtend::buildIllArrangeList(illList, 1001);
	// 158 bit(字符)
	int tbit = illList[100].calcTotalBitNum();
	illList[100].print(ill100ReslS, tbit+1);
	string ill100ReslStr = ill100ReslS;
	string tempStr(10, '\0');
	tempStr[0] = '1';
	testAndOut("std::string length() 和 size一样:", tempStr.length(), tempStr.size());
	testAndOut("整型以内的大数错排 100时候的错排数", ill100ReslStr, illResultStr);
	testAndOut("大数==比较: ", bigNumForIllResilt == illList[100], true);
	testAndOut("大数>比较: ", bigNumForIllResilt > illList[99], true);
	testAndOut("大数<比较: ", bigNumForIllResilt < illList[101], true);
	cout << "cout 测试: " << bigNumForIllResilt << endl;
	cout << "cout 测试: " << c << endl;

	StandardExtend::testAndDiffClock([&]() {
		testAndOut("10*3", BigInteger(10).muityDouble(3), BigInteger(30));
		testAndOut("10*4", BigInteger(10).muityDouble(4), BigInteger(40));

		testAndOut("bigNumForIllResilt*bigNumForIllResilt"
			, BigInteger(bigNumForIllResilt).muityDouble(1024), bigNumForIllResilt*1024);
		testAndOut("1024*0", BigInteger(1024).muityDouble(0), BigInteger(0));
		testAndOut("1024*1", BigInteger(1024).muityDouble(1), BigInteger(1024));
		testAndOut("512*2", BigInteger(512).muityDouble(2), BigInteger(1024));
		testAndOut("256*4", BigInteger(256).muityDouble(4), BigInteger(1024));
		testAndOut("128*8", BigInteger(128).muityDouble(8), BigInteger(1024));
	}, "基于右侧减法运算符的乘法(有时比字乘法快)");
	
	StandardExtend::testAndDiffClock([&]() {
		testAndOut("10*3", BigInteger(10).muity(3), BigInteger(30));
		testAndOut("10*4", BigInteger(10).muity(4), BigInteger(40));

		testAndOut("bigNumForIllResilt*bigNumForIllResilt"
			, BigInteger(bigNumForIllResilt).muity(1024), bigNumForIllResilt*1024);
		testAndOut("1024*0", BigInteger(1024).muity(0), BigInteger(0));
		testAndOut("1024*1", BigInteger(1024).muity(1), BigInteger(1024));
		testAndOut("512*2", BigInteger(512).muity(2), BigInteger(1024));
		testAndOut("256*4", BigInteger(256).muity(4), BigInteger(1024));
		testAndOut("128*8", BigInteger(128).muity(8), BigInteger(1024));
	}, "字乘法");

	StandardExtend::testAndDiffClock([&]() {
		string a = to_string(MAX_INT64), b = to_string(MAX_INT64);
		char result[128] = "";
		BigInteger ba(a), bb(b);
		(ba + bb).print(result, 128);
		BigInteger::bigPlush(a, b, a);
		testAndOut("64位有符号整型最大值相加: ", BigInteger::formatString(a), string(result));
	}, "字符串大数加法");
	/*
	python:
		a = 1
		n = 10000
		for i in range(1,n+1):
			a = a * i

		print(a)
	*/
	StandardExtend::testAndDiffClock([&]() {
		string resultOfFact100 = "93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000";
		string resultOfFact500 = "1220136825991110068701238785423046926253574342803192842192413588385845373153881997605496447502203281863013616477148203584163378722078177200480785205159329285477907571939330603772960859086270429174547882424912726344305670173270769461062802310452644218878789465754777149863494367781037644274033827365397471386477878495438489595537537990423241061271326984327745715546309977202781014561081188373709531016356324432987029563896628911658974769572087926928871281780070265174507768410719624390394322536422605234945850129918571501248706961568141625359056693423813008856249246891564126775654481886506593847951775360894005745238940335798476363944905313062323749066445048824665075946735862074637925184200459369692981022263971952597190945217823331756934581508552332820762820023402626907898342451712006207714640979456116127629145951237229913340169552363850942885592018727433795173014586357570828355780158735432768888680120399882384702151467605445407663535984174430480128938313896881639487469658817504506926365338175055478128640000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
		//1000阶乘共3w多位 这是前59位 28462596809170545189[0]
		string resultOfFact10000 = "28462596809170545189064132121198688901480514017027992307941";
		char *buffer = new char[42560];
		BigInteger(1).fact(100).print(buffer, 256);
		testAndOut("100的阶乘: ", string(buffer), resultOfFact100);

		BigInteger(1).fact(500).print(buffer, 2560);
		testAndOut("500的阶乘: ", string(buffer), resultOfFact500);
		
		/*//DEBUG模式下要算1min左右
		BigInteger(1).fact(10000).print(buffer, 40000);
		buffer[59] = '\0';
		testAndOut("10000的阶乘的前59位: ", string(buffer), resultOfFact10000);
		*/
		delete buffer;
		buffer = nullptr;
	}, "大数快速阶乘");
	
	//freopen("input", "r", stdin);
	/*char num[25];
	char out[25] = { 0 };
	while (cin >> num) {
		bool yes = true;
		BigInteger bnum = BigInteger(num);
		bnum.print(out);
		string num1 = string(out);
		bnum.muity(2).print(out);
		string num2 = string(out);
		for (JCE::SizeType i = 0; i < num1.size(); ++i) {
			char c = num2[i];
			int sub = num1.find(c);
			if (sub == num1.npos) {
				yes = false;
				break;
			}
			else {
				num1[sub] = ' ';
			}
		}
		puts(yes ? "Yes" : "No");
		bnum.print();
	}*/
	return 0;
}

int mainForDllTest() {
	cout << DllHelloWorld() << endl;
	cout << DllTestNameSpace::dllHelloWorldInNameSpace() << endl;
	auto temp = DllTestNameSpace::DllTestClass(200);
	cout << DllTestNameSpace::DllTestClass(1221).toString() << endl;
	// Initialize a Fibonacci relation sequence.
	fibonacci_init(1, 1);
	// Write out the sequence values until overflow.
	do {
		std::cout << fibonacci_index() << ": "
			<< fibonacci_current() << std::endl;
	} while (fibonacci_next());
	// Report count of values written before overflow.
	std::cout << fibonacci_index() + 1 <<
		" Fibonacci sequence values fit in an " <<
		"unsigned 64-bit integer." << std::endl;
	return 0;
}

int main() {
	//FILE *inFile = stdin, *outFile;
	//freopen("input", "r", stdin);
	//freopen_s(&inFile, "input", "r", stdin);
	//mainForExpressionTree();
	//mainForHuffumanTree();
	MainForLinkedList();
	MainForStack();
	mainForQueue();
	mainForDeQueue();
	mainForTree();
	mainForMath();
	StandardExtend::testAndDiffClock(mainForAlgorithm);
	StandardExtend::testAndDiffClock(mainForGraph);
	StandardExtend::testAndDiffClock(mainForBigInteger);

	mainForDllTest();

	return 0;
}

