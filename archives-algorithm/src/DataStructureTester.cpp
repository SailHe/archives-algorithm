#include "Teater.h"

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

	puts("��ת��list2\n");
	list2.reverse().output();
	puts("�ٴη�תlist2\n");
	list2.reverse().output();

	puts("����ϲ�list2��list1\n");
	list1.merge(list2).output();
	LinkedList<int>::Iterator iter = list1.findKth(1);
	n = list1.length();
	int i = 0;
	while (true) {
		++i;

		LinkedList<int>::Iterator findIter = list1.findKth(i);
		if (findIter) {
			list3.insertInP(*findIter, findIter);
			printf("find ������%d��Ԫ��%d\n", i, *findIter);
		}
		else {
			puts("find error");
		}

		iter = list1.findKth(-1);
		if (iter) {
			printf("find ������%d��Ԫ��%d\n", 1, *iter);
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
void testForMersenneTwister() {
	// non-deterministic generator  
	random_device rd;
	//÷ɭ��ת���㷨��Mersenne twister����һ��α����������㷨��
	// ���ɱ����������ʿ[1]��1997�꿪�����������޶������ֶ��ϵľ������Եݹ顣
	// ���Կ��ٲ�����������α������������˹ŵ�����������㷨�ĺܶ�ȱ�ݡ�
	mt19937 gen(rd());
	// to seed mersenne twister.  
	// replace the call to rd() with a constant value to get repeatable results.  

	// ��ͨ�����ʹ��ʱ����Ϊ���� ʹ��0�Ļ�ÿ�β��������ֵ��һ����
	srand(clock());
	for (int i = 0; i < 5; ++i) {
		// print the raw output of the generator.  
		cout << "÷ɭ�����: " << gen() << " ";
		cout << "��ͨ�����: " << rand() << endl;
	}
	cout << endl;
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

	int b[10] = { 2, 1, 4, 5, 9, 3, 6, 7 };
	AvlTree<int> avlTreeIns = AvlTree<int>();
	for (int i = 0; i < 8; ++i) {
		avlTreeIns.insert(b[i]);
	}
	puts("avl-����");
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
	puts("��-����");
	heapIns.traversal(heapIns.ORDER_LEVEL, [&](BinTree<int>::BT b) {
		printf("%d ", b->Data);
		return false;
	});

	puts("");
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

int testerForArrayHuff(int *hufWeightList, int n) {
	ArrayHuffman::HuffmanTree hufTree = nullptr;
	ArrayHuffman::HuffmanCode hufCode = nullptr;
	ArrayHuffman::HuffmanCoding(hufTree, hufCode, hufWeightList, n);
	int result = ArrayHuffman::codingLen(hufCode, n, hufWeightList);
	free(hufTree); hufTree = nullptr;
	free(hufCode); hufCode = nullptr;
	return result;
}

int mainForHuffumanTree() {
	const int n = 4;
	int w[n] = { 1, 2, 3, 4 };
	int result[][4] = {
		{ 1, 5, 0, 0},
		{ 2, 5, 0, 0},
		{ 3, 6, 0, 0},
		{ 4, 7, 0, 0},
		{ 3, 6, 1, 2},
		{ 6, 7, 3, 5},
		{ 10, 0, 4, 6}
	};
	ArrayHuffman::HuffmanTree ht;
	ArrayHuffman::HuffmanCode hc;
	ArrayHuffman::HuffmanCoding(ht, hc, w, n);
	//0���ǿս��
	for (int i = 1; i < n; ++i) {
		StandardExtend::testAndOut("weight" + to_string(i) + ": ", ht[i].weight, result[i - 1][0]);
		StandardExtend::testAndOut("parent" + to_string(i) + ": ", ht[i].parent, result[i - 1][1]);
		StandardExtend::testAndOut("lchild" + to_string(i) + ": ", ht[i].lchild, result[i - 1][2]);
		StandardExtend::testAndOut("rchild" + to_string(i) + ": ", ht[i].rchild, result[i - 1][3]);
	}
	int hufWeightList1[] = { 1, 2, 3, 4, 5 };
	int hufWeightList2[] = { 3, 8, 8 };
	StandardExtend::testAndOut("���������볤��: ", testerForArrayHuff(hufWeightList1, 5), 33);
	StandardExtend::testAndOut("��Ȩ���������볤��: ", testerForArrayHuff(hufWeightList2, 3), 30);
	/*const int N = 63;
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
	}*/
	return 0;
}


void outPutGraph(Graph &g) {
	cout << "����: " << g.getEdgeNum() << endl;
	JCE::ArrayList<Graph::VertexKey> dist, path;
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

int mainForGraphTemplate() {
	GraphTemplate<int> *g = new AdjacentMatrixGraphTemplate<int>(10);
	GraphTemplate<double> *gd = new AdjacentMatrixGraphTemplate<double>(10);
	// GraphTemplate<PointGeographic> *gp = new AdjacentMatrixGraphTemplate<PointGeographic>(10);
	// GraphTemplate<DisSet> *gDisList = new AdjacentMatrixGraphTemplate<DisSet>(10);
	gd->insertEdge(GraphTemplate<double>::Edge(0, 1, 10.0));
	delete g;
	g = nullptr;
	return 0;
}


int runDataStructureTest() {
	testForMersenneTwister();
	mainForHuffumanTree();
	MainForLinkedList();
	MainForStack();
	mainForQueue();
	mainForDeQueue();
	mainForTree();
	//mainForExpressionTree();
	StandardExtend::testAndDiffClock(mainForGraph);
	StandardExtend::testAndDiffClock(mainForGraphTemplate);

	return 0;
}
