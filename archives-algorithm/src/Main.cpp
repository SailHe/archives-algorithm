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

int mainForBinSearchTree() {
	BinSearchTree<string> bt = BinSearchTree<string>();
	for (int i = 0; i < 10; ++i) {
		bt.insert(string("BST data") + std::to_string(i));
	}
	puts("˳��");
	bt.traversal(Tree::ORDER, [&](BinTree<string>::BT node) {
		cout << node->Data << endl;
		return false;
	});
	puts("���� + ��ֹ");
	bt.traversal(Tree::RE_ORDER, [&](BinTree<string>::BT node) {
		cout << node->Data << endl;
		return node->Data == string("BST data5") ? true : false;
	});
	puts("�ȸ���");
	bt.traversal(Tree::ORDER_INFIX_ROOT, [&](BinTree<string>::BT node) {
		cout << node->Data << endl;
		//��Ч����ֵ
		return false;
	});
	cout << bt.find(string("BST data5"))->Data << endl;
	puts(" ====== Tree test end");
	return 0;
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
int mainForHuffumanTree()
{
	const int N = 63;
	int CodeWPL;//��׼
	int i, n, Freq[N + 1] = { 0 };
	//int j, m;
	char ch[N + 1], s[N][2 * N];
	while (~scanf("%d\n", &n)) {
		Freq[0] = n; memset(s, 0, N*N);
		memset(ch, 0, N + 1); ch[0] = 32;//��ʼ��
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
	cout << "����: " << g.getEdgeNum() << endl;
	StandardExtend::ArrayList<Graph::VertexKey> dist, path;
	g.shortestPath(0, dist, path);
	cout << "��0�Ŷ����������̾���ֵ: " << endl;
	StandardExtend::outPutIterable(dist.begin(), dist.end(), 2);
	cout << "��0�Ŷ�����������·��: " << endl;
	StandardExtend::outPutIterable(path.begin(), path.end(), 2);
	puts("");
}

void NewGraphDemo(Graph *&&g) {
	for (int i = 0; i < 10; ++i) {
		g->insertEdge(Graph::Edge(0, i, 10));
		//�ظ������Ϊ����
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
	//3*3����ͼ ��9������
	//CoordinatesMap cg(3, 3, Graph::DirCountEnum::DIR8);
	CoordinatesMap cg(3, 3, DirCountEnum::DIR8);
	//Ĭ��ȫ����Ч������� �˾�ֻ������ʾAPI
	cg.setInvalid(1, 0);
	//�����Ƿ����ͨ�� ���������ڵ���Ч����ΪΪ�����������֮����������
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

	//������Ч��ͬʱ���ö���ֵ(ת��ΪȨ��)
	//����Ĭ�ϵ�ֵ�������κ�ֵ(ʵ����-1 ����Ӧ�ð����������Чֵ)��û������ֵ����Ч����ת��ΪȨֵ�ͻ������, ����API��ò�Ҫ����
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
	cout << name + " ʵ��ֵ: " << realValue << "; ����ֵ: " << expectValue << endl;
	return true;
}

int mainForMath() {
	testAndOut("2018���Ƿ�����: ", (bool)StandardExtend::isIntercalary(2018), false);
	testAndOut("2018-10-22��2018��ĵڼ���: ", StandardExtend::calcNumberOfDays(2018, 10, 22), 295);

	puts("ȫ����: ");
	MathExtend::pentration(3);
	puts("�׳�: ");
	printf("%.2f\n", MathExtend::fact(10));
	testAndOut("����� 3ȡ2", MathExtend::C(3, 2), 3);
	testAndOut("����� 3ȡ1", MathExtend::C(3, 1), 3);
	testAndOut("����� 49ȡ6", MathExtend::C(49, 6), 13983816);
	testAndOut("������ 3ȡ2", MathExtend::A(3, 2), 6);
	testAndOut("������ 8ȡ3", MathExtend::A(8, 3), 336);


	for (I64 i = 0; i < 20; ++i) {
		testAndOut("������ "+ std::to_string(i) +"^2", MathExtend::quickPow(i, 2), (I64)std::pow(i, 2));
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
	}, "��ͨ������");
	StandardExtend::testAndDiffClock([&]() {
		for (int i = 0; i < testCount; ++i) {
			MathExtend::quickPow(2, 62, MAX_INT64);
		}
	}, "��mod�Ŀ�����");
	StandardExtend::testAndDiffClock([&]() {
		for (int i = 0; i < testCount; ++i) {
			MathExtend::quickPow_OLD(2, 62, MAX_INT64);
		}
	}, "����quickFact�Ŀ�����");
	testAndOut("������ 10^18", MathExtend::quickPow_OLD(10, 18, MAX_INT64), (I64)std::pow(10, 18));
	for (int i = MAX_INT32-1; i < MAX_INT32; ++i) {
		auto a = MathExtend::powLastBit(i, 1) % 10;
		auto b = MathExtend::quickPow(i, 2, 10);
		testAndOut(std::to_string(i) + "^2" + "�����1bit", a, b);
	}

	auto primeTable = MathExtend::generateSievePrimeS(100);
	int count = 0;
	for (int i = 0; i < 50; ++i) {
		bool isP = MathExtend::isPrime(i);
		testAndOut("����" + to_string(i) + " �Ƿ�����:", isP, primeTable[i] == 0);
	}
	free(primeTable);



	Utility::Double::setEPS(EPS_DOUBLE);
	auto f1 = [](double x) {
		return pow(x, 2) + 2 * x + 1;
	};
	auto x1 = MathExtend::trichotomy<double, double>(-10.0, 10.0, [&](double x) {
		//��x=-1.0ʱ y��С=0.0
		return f1(x);
	}, EPS);
	testAndOut("һԪ���η�����С���ƽ�: ", Utility::Double(x1), Utility::Double(-1.0));
	testAndOut("һԪ���η�����С����ֵ: ", Utility::Double(f1(x1)), Utility::Double(0.0));

	auto f2 = [](double x) {
		return -pow(x, 2) + 2 * x + 1;
	};
	auto x2 = MathExtend::trichotomy<double, double>(-10.0, 10.0, [&](double x) {
		//��x=1.0ʱ y���=2.0 (�˷���ֻ�ṩ��Сֵ ����ǰ����'-'��)
		return -(f2(x));
	}, EPS);
	testAndOut("һԪ���η��������ƽ�: ", Utility::Double(x2), Utility::Double(1.0));
	testAndOut("һԪ���η���������ֵ: ", Utility::Double(f2(x2)), Utility::Double(2.0));

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
		testAndOut("2*x + 1*y + 5 = 0����С�Ǹ�������x�Ƿ����: ", (bool)MathExtend::linearEquationCondition1(2LL, x, 1LL, y, 5LL), true);
		testAndOut("��x: ", x, 0LL);
		testAndOut("��y: ", y, 5LL);
	}, "��Ԫһ�����Է�����С�Ǹ�������");
	
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
			testAndOut("��x: ", x, aaOutput[i][0]);
			testAndOut("��y: ", y, aaOutput[i][1]);
		}
	}, "��Ԫһ�����Է��� ����|x| + |y|��С�Ľ�");
	StandardExtend::testAndDiffClock([&]() {
		int x, y, x1, y1, x2, y2;
		for (int i = 0; i < 7; ++i) {
			MathExtend::linearEquationCondition1(aaInput[i][0], x1, aaInput[i][1], y1, aaInput[i][2]);
			//����
			MathExtend::linearEquationCondition1(aaInput[i][1], x2, aaInput[i][0], y2, aaInput[i][2]);
			if ((abs(x1) + abs(y1)) < (abs(x2) + abs(y2))) {
				x = abs(x1);
				y = abs(y1);
			}
			else {
				//�����Ƿ���
				x = abs(y2);
				y = abs(x2);
			}
			testAndOut("��x: ", x, aaOutput[i][0]);
			testAndOut("��y: ", y, aaOutput[i][1]);
		}
	}, "��Ԫһ�����Է��� ����|x| + |y|��С�Ľ�");
	
	StandardExtend::testAndDiffClock([&]() {
		int m[] = {3, 5, 7};
		int a[] = {2, 3, 2};
		int minResult = MathExtend::chineseReminder(m, a, 3, 0);
		testAndOut("��С��������: ", minResult, 23);
	}, "�й�ʣ�ඨ�� ������ͬ�෽����");

	StandardExtend::testAndDiffClock([&]() {
		I64 illTable[] = {0, 0, 1, 2, 9, 44, 265, 1854 };
		JCE::ArrayList<I64> illArrangeList;
		MathExtend::buildIllArrangeList(illArrangeList);
		for (JCE::SizeType i = 0; i < 8; ++i) {
			testAndOut("����Value: ", illArrangeList[i], illTable[i]);
		}
		StandardExtend::outPutIterable(illArrangeList.begin(), illArrangeList.end(), 0, '\0', ',');
		StandardExtend::outPutIterable(illArrangeList.begin(), illArrangeList.end(), 20, '\0', (JCE::SizeType)5);
		StandardExtend::outPutIterable(illArrangeList.begin(), illArrangeList.end(), 20, '\0', [](auto left) {
			static int count = 0;
			return ++count % 5 == 0;
		});
	}, "���ű�");

	cout << "��ӡn�е��������" << endl;
	int table[10][StandardExtend::MAX_C] = { 0 };
	MathExtend::buildPtriangleTable(table, 10);
	StandardExtend::outPut2DArrayTrangle(table, 10, 3);

	cout << " ====== Math test end" << endl;
	return 0;
}

int mainForAlgorithm() {
	//@see http://acm.hdu.edu.cn/showproblem.php?pid=2602
	//��Ʒ����, ��������, �����ö�̬�滮�����С
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

