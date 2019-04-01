#ifndef _TREEOBJECT_H
#define _TREEOBJECT_H

#include"../stdafx.h"
#include"../ExtendSpace.h"
/*
*结语:
*树毕竟只是容器 适合于增删查的动态操作 不适合用来作遍历
*这只能算是PTA和数据结构课程或者说幼稚与偏执想法的遗留产物 以后估计就算有这样的想法也鲜有这样的机会与环境了
*纠正了许多常识性但根深蒂固的认识错误
*对[树结构]的认识与理解异于常人
*对[C++的标准库]使用 以及[模板编程]{类模板 函数模板} 有了一些心得(C++高级编程)
*对[面向对象编程]多了几分理解和经验
*对[算法]优劣的衡量有一定理解(上限O 线性Θ 下限Ω)
*这一完全的树对象继承关系的优势在于相互间的转换很方便不费时(但本身效率比不上单独实现的)
*/
/*树 抽象类(接口类)*/
class Tree{
public:
	// 遍历的顺序类型枚举
	enum TraversalOrderEnum{
		ORDER_PREFIX_ROOT, // 先根序
		ORDER_INFIX_ROOT, // 中根序
		ORDER_POST_ROOT, // 后根序
		ORDER_LEVEL, // 层序
	};
	int Min(int a, int b){
		return a < b ? a : b;
	}
	int Max(int a, int b){
		return a > b ? a : b;
	}
protected:
	//数组表
	template<typename T> using ArrayList = std::vector<T>;
	template<typename T> using stack = std::stack<T>;
	template<typename T> using queue = std::queue<T>;
	template<typename T> using priority_queue = std::priority_queue<T>;
	template<typename T> using greater = std::greater<T>;
	using string = std::string;
	//template<typename T> using vector = StandardExtend::ArrayList<T>;
};

/*二叉(度)树*/
template<class T>
class BinTree :public Tree{
public:
	typedef T Element;
	/*二叉树结点*/
	typedef class BTNode{
	public:
		BTNode(){}
		//获取子类height字段
		virtual int getHeight(){
			return depthOf(this);
		}
		//设置子类height字段
		virtual void setHeight(int){}
		//获取子类的添加字段:weight
		virtual int getValue(){
			return 0;
		}
		//设置子类添加字段
		virtual void setValue(int){}
		//这样无需默认构造函数
		BTNode(Element const&tData) : Data(tData){}
		/*BTNode(Element const&tData){
			Data = tData;
		}*/
		//拷贝构造 左右孩子结点置nullptr
		BTNode(BTNode &rhs){
			(*this) = rhs;
			Left = Right = nullptr;
		}
		virtual ~BTNode(){
			DE_PRINTF("BTNode析构");
		}
		//赋值 默认的全域赋值
		BTNode& operator=(const BTNode&) = default;
		int height(){
			return depthOf(this);
		}
		//叶子结点判断
		bool isLeave() const {
			return Left == nullptr && Right == nullptr;
		}
		/*
		bool operator=(HuTrNode const &rhs){
		return Weight == rhs.Weight;
		}
		bool operator=(int const &rhs){
		return Weight == rhs;
		}
		bool operator<(HuTrNode const &rhs){
		return Weight < rhs.Weight;
		}
		bool operator>(HuTrNode const &rhs){
		return Weight > rhs.Weight;
		}
		bool operator<=(HuTrNode const &rhs){
		return Weight <= rhs.Weight;
		}
		bool operator>=(HuTrNode const &rhs){
		return Weight >= rhs.Weight;
		}
		bool operator==(HuTrNode const &rhs){
		return Weight == rhs.Weight;
		}
		bool operator!=(HuTrNode const &rhs){
		return Weight != rhs.Weight;
		}
		*/

		BTNode* Left = nullptr;		/* 指向左子树 */
		BTNode* Right = nullptr;		/* 指向右子树 */
		bool v = false;				/* 表示是否visit过此结点 */
		Element Data;				/* 结点数据:若为结构体,结构体提供比较方法key(增删查都会使用的关键字)在结构体内 否则key就是Data weight-height不能作为key(不唯一)*/

	} *Position;
	// 简单说Position有修改权限 对应的BT没有
	using BT = BTNode const *;
	using Vister = std::function<void(BinTree<T>::BT const)>;

	BinTree(){}
	// 拷贝构造 (拷贝只保证结点内容一致; 引用参数=>拷贝构造)
	BinTree(const BinTree &rhs){
		DE_PRINTF("BT拷贝构造");
		assignment(root_, rhs.root_);
		usedSize = rhs.usedSize;
	}
	// 移动构造 (保证完全一致)
	BinTree(BinTree &&rvalue) {
		DE_PRINTF("BT移动构造");
		std::swap(root_, rvalue.root_);
		std::swap(usedSize, rvalue.usedSize);
		std::swap(isInsert, rvalue.isInsert);
		std::swap(lastInsertPosition, rvalue.lastInsertPosition);
	}
	// 先中序列构造 缺省的遍历序列放置元素个数
	BinTree(Element const *preOrder, Element const *inOrder, int n){
		prefInBuild(preOrder, inOrder, root_, n);
	}
	// 中后序列构造
	BinTree(int n, Element const *inOrder, Element const *postOder){
		postInBuild(root_, inOrder, postOder, n);
	}
	
	// 先根序遍历操作堆栈构造 (堆栈操作获取方法, 结点数据获取方法)
	BinTree(std::function<bool(std::string &)> getOrder, void(*getData)(T*)){
		ArrayList<T> preOrder, inOrder;
		stack<T> s;
		std::string str;
		while (getOrder(str)) {
			int len = -1;
			Utility::AssertToSignedNum(str.length(), len);
			if (len == 4) {//Push
				T ele;
				getData(&ele);
				// 第一次遇到时(先序)
				preOrder.push_back(ele);
				s.push(ele);
			}
			else if (len == 3){//Pop
				// 第二次遇到时(中序)
				inOrder.push_back(s.top());
				s.pop();
			}
			else {
				throw std::exception("Order error!");
			}
		}
		_ASSERT_EXPR(preOrder.size() == inOrder.size(), "Size Error");
		prefInBuild(preOrder, 0, inOrder, 0, root_, preOrder.size());
	}

	// destructor
	virtual ~BinTree() {
		destroy(root_);
		DE_PRINTF("BT析构");
	}

	// 避免无意地二叉树赋值(赋值操作析构原lhs 且只能保证内容一致)
	BinTree &operator= (const BinTree &rhs) {
		// 先要析构自己的root_
		destroy(root_);
		assignment(root_, rhs.root_);
		usedSize = rhs.usedSize;
		return *this;
	}
	// 避免无意地二叉树移动(系统自动析构原lhs)
	BinTree &operator= (BinTree &&rvalue) {
		std::swap(root_, rvalue.root_);
		std::swap(usedSize, rvalue.usedSize);
		std::swap(isInsert, rvalue.isInsert);
		std::swap(lastInsertPosition, rvalue.lastInsertPosition);
		return *this;
	}

	// 清除所有内容(初始化所有结点的有效性, 结构一定会被摧毁, root_会保留)
	virtual void clear() {
		destroy(root_);
		lastInsertPosition = nullptr;
		isInsert = false;
		usedSize = 0;
	}

	Element getRootData() const {
		return root_->Data;
	}
	BT getRoot() const {
		return root_;
	}
	// 若二叉树为空返回true (存在至少1个结点含有数据) O(1)
	bool empty() const {
		// (根结点为空表示整颗树为空 空表示当前结点不存在而非不存在子结点)
		// return empty(root_);
		return usedSize == 0;
	}
	// 返回树高度|深度 O(H)
	int height() const {
		// 若已知size则为log的复杂度(貌似差不多的样子)
		// ceil(log(size, 2))
		return depthOf(root_);
	}
	// 返回树层数 => [0, height)
	int layers() const {
		return height() - 1;
	}

	// Θ(2*N)     Tree::ORDER, void visit(BinTree<T>::BT node)
	void traversal(TraversalOrderEnum type, Vister visit){
		if (type == ORDER_PREFIX_ROOT)
			preTraversal(root_, visit);
		else if (type == ORDER_INFIX_ROOT)
			infTraversal(root_, visit);
		else if (type == ORDER_POST_ROOT)
			postTraversal(root_, visit);
		else if(type == ORDER_LEVEL)
			levelTraversal(root_, visit);
		else
			_ASSERT_EXPR(false, "遍历参数错误 NONE_ORDER 不作traversal_");
	}
	
	static void orderTranslation(Element *buf, Element *preOrder, Element *inOrder, Element *postOder, int n){
		if (inOrder == nullptr)
			throw std::exception("must know inorder");
		else if (preOrder == nullptr && postOder != nullptr)
			calcPrefOrder(buf, inOrder, postOder, n);
		else if (postOder == nullptr)
			calcPostOrder(preOrder, inOrder, buf, n);
		else
			throw std::exception("Does not meet the conversion conditions");
	}
	// 镜像树: mirroring reversal镜像反转 转换后不能使用原来的任何基于比较的方法(若是搜索树:左小右大->左大右小)
	void mirReversal(){
		queue<Position> q;
		q.push(root_);
		while (!q.empty()){
			Position t = q.front();
			q.pop();
			// 将所有非空子树送入转换队列
			if (t->Left != nullptr)
				q.push(t->Left);
			if (t->Right != nullptr)
				q.push(t->Right);
			// 将树根的左右子树转换
			std::swap(t->Left, t->Right);
		}
	}
	// 同构判断
	bool omorphism(BinTree const &T2){
		return isomorphic(this->root_, T2.root_);
	}
	// 向现有的树结构中填充数据(可利用静态二叉树储存输入结构 或是原结构)
	// 普通二叉树->传入中序遍历结果可以获得确定的一颗树
	// 有序数组->数据符合二叉搜索树要求
	// 完全二叉树->有数组的构造方法(但仅对完全二叉搜索树有实际意义)
	bool fillData(ArrayList<T> &dataA){
		return fillData(dataA, 0, dataA.size(), root_);
	}
	int size(){
		return usedSize;
	}
	
protected:
	const static int BT_NODE_LEN = sizeof(class BTNode);
	Position root_ = nullptr;
	int usedSize = 0;// 有效的元素个数
	Position lastInsertPosition = nullptr;// 结点生成器最后生成的结点 (无法用这个判断插入成功与否)
	bool isInsert = false;// 是否执行了插入操作(判断插入是否成功)

	// queue<Element*> freeMem;//空闲内存
	// Element *memoryBlock = nullptr;//内存块 可将二叉树的局部储存在这里 超出部分使用外部分配的内存

	// 返回子二叉树的规模 O(N)
	static int scaleOf(BT t) {
		return empty(t) ? 0 : scaleOf(t->Left) + scaleOf(t->Right) + 1;
	}
	// 返回结点深度 (空树深度为零 有root则为1)
	static int depthOf(Position bT){
		int HL, HR;
		if (bT) {
			HL = depthOf(bT->Left);
			HR = depthOf(bT->Right);
			// (根据公式 Height = max(Hl, Hr) + 1 由后序遍历改编实现)
			return (HL > HR ? HL : HR) + 1;
		}
		else
			return 0;
	}
	// 按原结构填充数据 (数组, 根结点数据下标, 当前根下的数据个数, 结构来源树) O(N^2)
	static bool fillData(ArrayList<T> &dataA, int dataRootSub, int dataSize, Position bt){
		if (empty(bt))
			return true;
		// 左右子树规模 这里可以小小的优化一下: 右子树规模可以通过已知的size减去左子树-1算出 但要修改参数比较麻烦(优化后复杂度仍是N^2)
		int nl = scaleOf(bt->Left), nr = scaleOf(bt->Right);
		// 整棵树的规模需与数据规模相等
		if (nl + nr + 1 != dataSize)
			return false;
		//数据填充
		bt->Data = dataA[dataRootSub + nl];
		//左子树递归							//1是1个根结点
		fillData(dataA, dataRootSub, dataSize - nr - 1, bt->Left);
		//右子树递归      //根1和左子树nl 都"小于"右子树
		fillData(dataA, dataRootSub + nl + 1, dataSize - nl - 1, bt->Right);
		return true;
	}
	// 销毁结点(只保证调用后结点内容无效, 不一定会析构结点)
	virtual void destroy(Position &r){
		if (empty(r)) {
			// DNT
		}
		else {
			std::queue<Position> q;
			q.emplace(r);
			while (!q.empty()) {
				Position current = q.front();
				q.pop();
				if (!empty(current->Left)) {
					q.emplace(current->Left);
					current->Left = nullptr;
				}
				if (!empty(current->Right)) {
					q.emplace(current->Right);
					current->Right = nullptr;
				}
				delete current;
			}
			r = nullptr;
		}
	}

	// 层序遍历---从上到下->从左到右(队列实现)
	static void levelTraversal(BT bT, Vister visit){
		if (!bT)return;
		queue<BT> q;
		q.push(bT);
		while (!q.empty()){
			bT = q.front(); q.pop();
			visit(bT);
			if (bT->Left)
				q.push(bT->Left);
			if (bT->Right)
				q.push(bT->Right);
		}
	}
	// 先序---子树根->左子树->右子树;(递归实现)
	static void preTraversal(BT bT, Vister visit){
		if (bT){
			visit(bT);
			preTraversal(bT->Left, visit);
			preTraversal(bT->Right, visit);
		}
	}
	// 中序---左子树->子树根->右子树;
	static void infTraversal(BT bT, Vister visit){
		if (bT){
			infTraversal(bT->Left, visit);
			visit(bT);
			infTraversal(bT->Right, visit);
		}
	}
	// 后序---左子树->右子树->子树根;
	static void postTraversal(BT bT, Vister visit){
		if (bT){
			postTraversal(bT->Left, visit);
			postTraversal(bT->Right, visit);
			visit(bT);
		}
	}

	//先中构建
	void prefInBuild(Element const *preOrder, Element const *inOrder, Position &bt, int n){
		int Ln;/*左子子树长度*/
		if (n == 0)return;
		if (bt == nullptr)
			bt = nodeCreater(*preOrder);
		else/*转化关系:先序遍历数组的首元素就是子树根*/
			bt->Data = *preOrder;
		for (Ln = 0; Ln < n && inOrder[Ln] != *preOrder; Ln++);
		prefInBuild(preOrder + 1, inOrder, bt->Left, Ln);/*先序遍历数组向左子树遍历一个元素 其余数组保持不变*/
		prefInBuild(preOrder + Ln + 1, inOrder + Ln + 1, bt->Right, n - Ln - 1);/*向右子树遍历一个元素*/
	}
	//先中构建ArrayList
	void prefInBuild(ArrayList<Element> &preOrder, int preRoot, ArrayList<Element> &inOrder, int inRoot, Position &bt, int n){
		int Ln;/*左子子树长度*/
		if (n == 0)return;
		if (bt == nullptr)
			bt = nodeCreater(preOrder[preRoot]);
		else/*转化关系:先序遍历数组的首元素就是子树根*/
			bt->Data = preOrder[preRoot];
		for (Ln = 0; Ln < n && inOrder[inRoot + Ln] != preOrder[preRoot]; Ln++);
		prefInBuild(preOrder, preRoot + 1, inOrder, inRoot, bt->Left, Ln);/*先序遍历数组向左子树遍历一个元素 其余数组保持不变*/
		prefInBuild(preOrder, preRoot + Ln + 1, inOrder, inRoot + Ln + 1, bt->Right, n - Ln - 1);/*向右子树遍历一个元素*/
	}
	//后中构建
	void postInBuild(Position &bt, Element const *inOrder, Element const *postOder, int n){
		int Ln;/*左子树长度*/
		if (n == 0)return;
		if (bt == nullptr)
			bt = nodeCreater(postOder[n - 1]);
		else/*转化关系:后序遍历数组的尾元素就是子树根*/
			bt->Data = postOder[n - 1];
		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; Ln++);//获取左子树长度
		postInBuild(bt->Left, inOrder, postOder, Ln);/*先序遍历数组进入左子树*/
		postInBuild(bt->Right, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*进入右子树*/
	}
	//后中构建 优化版(@TODO size)
	static BT postInBuild(int *inOrder, int *postOder, int n){
		int Ln;/*左子树长度*/
		if (n == 0)	return nullptr;

		Position bt = (Position)malloc(sizeof(struct TreeNode));
		memset(bt, 0, sizeof(struct TreeNode));
		bt->Element = postOder[n - 1];/*转化关系:后序遍历数组的尾元素就是子树根*/

		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; Ln++);//获取左子树长度
		bt->Left = postInBuild(inOrder, postOder, Ln);/*先序遍历数组进入左子树*/
		bt->Right = postInBuild(inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*进入右子树*/
		return bt;
	}

	static void calcPostOrder(string const &sPre, string const &sMed, string &sPostBuffer){
		//中序中的根所在位置
		string::size_type medRootSub = sMed.find(sPre[0]);
		//左子树的长度
		string::size_type leftSubLen = medRootSub;
		/*
		A B CD	前序(Root Left Right)
		B A DC	中序(Left Root Right)

		BDC A	后序(Left Right Root)
		*/

		//若存在左子树则向左子树递归
		if (leftSubLen > 0){
			//先序第一个值(root_)右边取左子树的长度即是左子树
			string sPreLeftSub = sPre.substr(1, leftSubLen);
			//root_的左边
			string sMedLeftSub = sMed.substr(0, leftSubLen);
			calcPostOrder(sPreLeftSub, sMedLeftSub, sPostBuffer);
		}
		else{
			//do nothing
		}

		//若存在右子树则向右子树递归
		if (medRootSub + 1 < sMed.size()){
			//先序第一个值(root_)的右边跳过左子树长度取所有即是右子树
			string sPreRightSub = sPre.substr(1 + leftSubLen);
			//root_的右边
			string sMedRightSub = sMed.substr(medRootSub + 1);
			calcPostOrder(sPreRightSub, sMedRightSub, sPostBuffer);
		}
		else{
			//do nothing
		}

		if (medRootSub != string::npos){
			//加上root_
			sPostBuffer += sMed.at(medRootSub);
		}
		else{
			//do nothing
		}
	}
	/*根据先序和中序遍历输出后序遍历序列 (先 中 后 子树元素个数)*/
	template<class Iterator>
	//支持加减运算的迭代器(裸指针也行 postOder对应的容器必须具有至少n个元素)
	static void calcPostOrder(Iterator preOrder, Iterator inOrder, Iterator postOder, int n){
		int Ln;/*左子子树长度*/
		if (n == 0)return;
		postOder[n - 1] = *preOrder;/*转化关系:先序遍历数组的首元素就是子树根*/
		for (Ln = 0; Ln < n && inOrder[Ln] != *preOrder; ++Ln);
		calcPostOrder(preOrder + 1, inOrder, postOder, Ln);/*先序遍历数组向左子树遍历一个元素 其余数组保持不变*/
		calcPostOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*向右子树遍历一个元素*/
	}
	/*根据先序和中序遍历输出后序遍历序列 (先 中 后 子树元素个数)*/
	static void calcPostOrder(Element *preOrder, Element *inOrder, Element *postOder, int n){
		int Ln;/*左子子树长度*/
		if (n == 0)return;
		postOder[n - 1] = *preOrder;/*转化关系:先序遍历数组的首元素就是子树根*/
		for (Ln = 0; Ln < n && inOrder[Ln] != *preOrder; Ln++);
		calcPostOrder(preOrder + 1, inOrder, postOder, Ln);/*先序遍历数组向左子树遍历一个元素 其余数组保持不变*/
		calcPostOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*向右子树遍历一个元素*/
	}
	/*根据后序和中序遍历输出先序遍历序列 只修改中间两句话即可*/
	template<class Iterator>
	static void calcPrefOrder(Iterator preOrder, Iterator inOrder, Iterator postOder, int n){
		int Ln;/*左子树长度*/
		if (n == 0)return;
		*preOrder = postOder[n - 1];/*转化关系:后序遍历数组的尾元素就是子树根*/
		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; ++Ln);//获取左子树长度
		calcPrefOrder(preOrder + 1, inOrder, postOder, Ln);/*先序遍历数组进入左子树*/
		calcPrefOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*进入右子树*/
	}
	/*根据后序和中序遍历输出先序遍历序列 只修改中间两句话即可*/
	static void calcPrefOrder(Element *preOrder, Element *inOrder, Element *postOder, int n){
		int Ln;/*左子树长度*/
		if (n == 0)return;
		*preOrder = postOder[n - 1];/*转化关系:后序遍历数组的尾元素就是子树根*/
		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; Ln++);//获取左子树长度
		calcPrefOrder(preOrder + 1, inOrder, postOder, Ln);/*先序遍历数组进入左子树*/
		calcPrefOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*进入右子树*/
	}
	/*根据后序和中序遍历输出层序遍历序列 后中->先->先序建树->层序遍历*/
	static void calcLeveOrder(Element *preOrder, Element *inOrder, Element *postOder, int n){
		puts("has not imp");
		exit(-1);
	}

	// 返回是否T1与T2是否同构 isomorphic(adj. [物] 同构的；同形的)  isomorphism_(n. 类质同像，[物化] 类质同晶；同形)
	static bool isomorphic(BT T1, BT T2){
		// 深度不同
		if ((T1 == nullptr && T2 != nullptr) || (T1 != nullptr && T2 == nullptr))
			return false;
		// 叶子结点
		else if (T1 == nullptr && T2 == nullptr)
			return true;
		else {// 两颗树都不为空
			// 数据不同
			if (T1->Data != T2->Data)
				return false;
			else { // 当前数据相等
				// 按当前顺序尝试匹配左右子树
				if (isomorphic(T1->Left, T2->Left))
					return isomorphic(T1->Right, T2->Right);
				else { // 否则转换左右子树匹配
					if (isomorphic(T1->Left, T2->Right))
						return isomorphic(T1->Right, T2->Left);
					else
						return false;
				}
			}
		}
	}

	// 递归拷贝赋值
	static void assignment(Position &lhs, const Position rhs){
		if (!empty(rhs)){
			lhs = new BTNode(*rhs);
			assignment(lhs->Left, rhs->Left);
			assignment(lhs->Right, rhs->Right);
		}
	}
	// 判断二叉树是否为空
	static bool empty(BT bT) {
		return bT == nullptr;
	}

	//镜像赋值(负号)
	void reAssignment(){}
	// 重复插入处理
	void repetitionInsert(Position bt){
		//x==bt->Data 无需插入 手动更新lastInsertPosition
		lastInsertPosition = bt;
		isInsert = false;
	}
	
public:
	//结点生成器 返回一个未使用的结点 若不存在未使用结点 返回nullptr 只能插入使用
	virtual Position nodeCreater(Element const &tData){
		lastInsertPosition = new BTNode(tData);
		isInsert = lastInsertPosition == nullptr ? false : true;
		++usedSize;
		return lastInsertPosition;
		/*
		bST = (BST)malloc(sizeof(struct TNode));
		memset(bST, 0, sizeof(struct TNode));
		bST->Element = x;
		*/
	}
	//结点擦除器 将结点置为未使用状态
	virtual void nodeEraser(Position &del){
		// free(del); del = nullptr;
		delete del;
		del = nullptr;
		--usedSize;
	}
};

/*二叉搜索（查找 排序 判定）树*/
/*折半查找的判定树:按照比较的次数生成判定树，比较1次的是根结点，比较2次的在第二层，比较3次的在第三层...
其实就是递归的将mid作为父结点
不过注意(左小右大的二叉搜索树):1 2 (mid=1)   4 5 (mid=4)即小的结点作为父结点的可能性更大
更有可能呈现135度的倾斜; 右子树结点更可能多一些*/
/*
性质：
①非空 左子树的所有键值 小于其根节点的键值
②非空 右子树的所有键值 大于其根结点的键值
③左右子树都是二叉搜索树
④没有键值相等的节点
最大元素：最右分支（一直bST = bST->Right）即可
最小元素：最左分支

n个结点的判定树depthOf = [log2(n)]+1
判定树上每个结点的查找次数==所在层数
查找成功次数<=depthOf
4层满二叉树的平均查找次数ASL = (4*8+3*4+2*2+1*1)/15
*/
template<class T>
class LinkedBinSearchTree : public virtual BinTree<T> {
public:
	enum TraversalOrderStopEnum {
		ORDER_SEQUENCE, // 顺序 (<=>可中止的先根序)
		ORDER_REVERSE, // 逆序
	};
	typedef typename BinTree<T>::BT BST;
	using StopVister = std::function<bool(BST const)>;
	//using BinTree<T>::Element;错误用法
	//using Position = BinTree<T>::Position;错误用法
	typedef typename BinTree<T>::Position Position;
	typedef typename BinTree<T>::Element Element;
	using BinTree<T>::root_;
	using BinTree<T>::lastInsertPosition;
	using BinTree<T>::traversal;
	using BinTree<T>::empty;
	using BinTree<T>::destroy;
	using BinTree<T>::nodeCreater;
	using BinTree<T>::nodeEraser;
	//constructor
	LinkedBinSearchTree(){}
	//拷贝构造 直接调的父类方法
	LinkedBinSearchTree(const LinkedBinSearchTree &rhs) :BinTree<T>(rhs)/*这样并未发生强制转换*/{
		DE_PRINTF("BST拷贝构造");
	}
	/*只要不改变左右子树各自的先序插入序列其插入结果与先构建左子树或先构建右子树无关
	先序与普通插入构建序列的区别:
	[先序序列]:[82] 76 23 80 | 90 95
	[插入序列] 82 90 76 23 95 80 构建的树与其是同一棵树*/
	//先序构造 (先序遍历序列左端,序列右端+元素个数)
	LinkedBinSearchTree(Element *preLeft, Element *preRight, int eqaul = 0){
		while (preLeft < preRight){
			if (!insert(*preLeft)){
				puts("构造失败!");
				system("pause");
			}
			++preLeft;
		}
	}
	//先序构造 ArrayList
	LinkedBinSearchTree(JCE::ArrayList<Element> &preOrder, int eqaul = 0){
		/*若preOrder参数不是引用
		**会发生:
		**ArrayList拷贝构造
		**ArrayList析构
		**BT拷贝构造
		**BST拷贝构造
		**BST析构
		**BT析构
		**ArrayList析构(不知道究竟是构造两次析构两次 还是构造一次析构一次)
		(不理解为什么在构造函数里 用传值拷贝参数会调用这一系列 我觉得完全没必要啊
		尤其是一开始没有写相应函数出错了...)
		注意!!!
		即便是LinkedBinSearchTree t = LinkedBinSearchTree(ArrayList<>);
		这样的语句也不会调用赋值函数 而是调用的拷贝构造函数
		preOrder是引用的话上面所有的屁事都没了
		*/
		for (int i = 0; i < preOrder.size(); ++i) {
			if (!insert(preOrder[i])){
				puts("构造失败!");
				system("pause");
			}
		}
	}
	//析构deleter
	virtual ~LinkedBinSearchTree() override{
		DE_PRINTF("BST析构");
	}
	//赋值 很费时间 避免无意间的二叉树赋值
	LinkedBinSearchTree& operator= (const LinkedBinSearchTree& rhs){
		DE_PRINTF("BST赋值");
		//<==>两次父类拷贝构造 一次父类赋值 两次父类析构
		//(BinTree)(*this) = (BinTree)rhs;
		// ==>强制转换其实是调用的拷贝构造方法(这样效率不高) 所以应当为子类编写自己的赋值函数(即使没有新增加的域)
		BinTree<T>::destroy(root_);//先要销毁自己的root_
		BinTree<T>::assignment(root_, rhs.root_);
		return *this;
	}

	//返回倒数第k大的元素位置 不存在返回nullptr;(利用BST中序遍历是顺序的特点:反过来是逆序)
	BST findRKth(BST t, int *k){
		if (t){
			BST r = findRKth(t->Right, k);//1.<阶段一>一直到最右边
			if (r) return r;//中转返回: 若不是2返回而来(3或4)则返回

			if (--(*k) == 0)//3.<阶段二> 按顺序由最后开始退k个结点
				return t;//终点返回: 已是倒数第k个结点

			return findRKth(t->Left, k);//4.<阶段三>前面都没有返回 说明满足要求的结点在左子树(中序):相当于一个新的调用
			//调用返回: 返回给1; 返回给4; 返回给初始调用0; 可能由1 2(nullptr 其实也是4 nullptr的情况只可能是最左边到头仍没有得到3的返回) 3返回而来
		}
		return nullptr;//2.终点返回: 到头 返回nullptr
	}
	//返回正数第k个元素(序号为k的元素 从1开始)
	BST findKth(int k){
		int id = 1;
		Position t;
		traversal(Tree::ORDER_SEQUENCE, [&](Position bt){
			t = bt;
			return id++ == k;//找到就中止
		});
		return t;
	}
	//返回特定元素的等级(比参数大的元素的个数) O(2*N)
	int getRankOf(T &x){
		int rank = 1;
		traversal(Tree::ORDER_REVERSE, [&](Position bt){
			//15 30 40 45 50 50   rank(45) = 3 不是序号3 更不是2
			if (bt->Data > x)//去掉这句就是求序号
				++rank;
			return bt->Data == x;//找到就中止
		});
		return rank;
	}
	//计算所有元素的等级 (计算完毕后将total sort一下再用类似的方法遍历一下统计出总排名即可) Θ(2*N)范程
	//虽然N个insert(O(logN))与N个读取后sort(O(N*logN))的效率差不多 但论遍历效率tree Θ(2*N), ArrayList Θ(N)
	void calcRank(JCE::ArrayList<T> &total, int cnt = -1){
		cnt = cnt < 0 ? BinTree<T>::size() : cnt;//默认计算所有人的排名
		int pastId = 0;//序号
		int reCnt = 0;//当前元素现在的重复个数
		Position pastP = nullptr;
		traversal(Tree::ORDER_REVERSE, [&](Position bt){
			if (pastP != nullptr && pastP->Data.score == bt->Data.score)
				++reCnt;
			else
				reCnt = 0;
			bt->Data.rank = ++pastId - reCnt;//等级 = 序号 - 重复个数
			total.push_back(bt->Data);//统计总排名
			pastP = bt;
			return cnt-- == 0;//计算前cnt名 若cnt为零则中止
		});
	}
	/*返回二叉搜索树bST中最小元结点的指针；*/
	Position findMin(){
		BST bST = root_;
		while (bST && bST->Left)
			bST = bST->Left;
		return bST;
		/*
		//1.
		if (!bST)//基本定义：树为空返回nullptr
		return nullptr;
		else if (!bST->Left)
		return bST;//基本定义：左子叶结点是最小元素
		else
		return FindMin(bST->Left);//递归定义：不是左子叶时进入左子树继续查找
		//2.更简易版: 还有左子树吗 有就递归 没有就返回
		return t->Left ? findMin(t->Left) : t->Data;
		*/
	}
	/*返回二叉搜索树bST中最大元结点的指针。*/
	Position findMax(){
		BST bST = root_;
		while (bST && bST->Right)
			bST = bST->Right;
		return bST;
	}

	//返回是否是二叉搜索树
	bool IsBST(BST t){
		if (t){
			if (!IsBST(t->Left))
				return false;
			if (!IsBST(t->Right))
				return false;
			if (t->Left){
				bool is = t->Left->Data < t->Data;
				if (t->Right){
					is = is && t->Data < t->Right->Data;
					if (!is) return false;
					is = is && findMin(t->Right) > t->Data;//右子树最小元素大于根结点
					if (!is) return false;
				}
				is = is && findMax(t->Left) < t->Data;//左子树最大元素小于根结点
				return is;
			}
		}
		return true;
	}
	//返回两个结点的公共祖先(先序) 优化遍历方式: 算法效率O(n)->O(h)不再死板地按照先序先左后右 LCA
	Position ancestor(Position t, int u, int v){
		if (t){
			/*都小向左 都大向右*/
			if (u < t->Data && v < t->Data)
				return ancestor(t->Left, u, v);
			if (u > t->Data && v > t->Data)
				return ancestor(t->Right, u, v);
			/*一大一小 返回 优化存在判断*/
			return ((find(t, u) && find(t, v)) ? t : nullptr);
		}
		return nullptr;
	}
	void clearVisit(Tree::TraversalOrderEnum type_){
		BinTree<T>::traversal(type_, [&](Position const t){
			if (t->v){
				t->v = false;
				return true;
			}
			else//当且仅当 遍历方式与visit清理方式相同时 当前结点没有遍历过子结点必定没有遍历过
				return false;//中止
		});
	}

	//  遍历器返回值为true表示立即中止遍历
	void traversal(TraversalOrderStopEnum type, StopVister visit) {
		if (type == ORDER_SEQUENCE)
			orderTraversal(root_, visit);
		else if (type == ORDER_REVERSE)
			reOrderTraversal(root_, visit);
		else
			_ASSERT_EXPR(false, "遍历参数错误 NONE_ORDER 不作traversal_");
	}
	/*返回x在二叉搜索树bST中的位置; 若找不到则返回nullptr*/
	Position find(T const&x){
		return findOf(root_, x);
	}
	//自定义查找(自定义小于符号)
	template<typename LessCmp>
	Position find(T const&x, LessCmp less){
		return findOf(root_, x, less);
	}
	//插入 O(logN)
	JCE::pair<Position, bool> insert(Element const&x){
		root_ = Insert(root_, x);
		//与map的insert返回值类似，重复insert 返回<重复Position, false>，这个技巧在面试如何找出2个数组相同的数字的时候有奇效
		return{ BinTree<T>::lastInsertPosition, BinTree<T>::isInsert };
	}
	//序列插入 局部有序插入O(1) 无序O(logN) 但如果Avl使用此插入会使Avl退化为普通BST(相当于优化的链表: 插入效率变高 查找效率相对Avl退化)
	//(例如654789)->4 5 [6] Orderly被置false 7 [手动]置true 8 9 可实现局部有序插入
	JCE::pair<Position, bool> insertOrderly(Element const&x, bool &Orderly){
		if (!Orderly){//若已经不是有序的插入 则退化为普通插入
			root_ = Insert(root_, x);
		}
		else{
			if (!BinTree<T>::empty(root_)){
				if (x < root_->Data){//若即将插入的数据在左子树
					if (x < lastInsertPosition->Data){//x已位于左子树 若x还比lastInsertPosition小后者必定也在左子树
						Insert(lastInsertPosition, x);//于是直接往lastInsertPosition的左边插即可
						Orderly = true;
					}
					else{//否则应该插入的位置位于左子树:root_和lastInsertPosition中间
						Orderly = false;
						root_ = Insert(root_, x);
					}
				}
				else{//相等时直接传入使Insert判断
					if (lastInsertPosition->Data < x){///@x > lastInsertPosition->Data
						Insert(lastInsertPosition, x);
						Orderly = true;
					}
					else{
						Orderly = false;
						root_ = Insert(root_, x);
					}
				}
			}
			else{
				Orderly = true;
				root_ = Insert(root_, x);
			}
		}
		return{ lastInsertPosition, BinTree<T>::isInsert };
	}
	//删除 删除成功返回true
	bool erase(Element const&x){
		bool succeed = true;
		root_ = Delete(root_, x, succeed);
		return succeed;
	}
protected:
	Position findOf(Position bST, T const &x){
		while (bST){
			//@调试结果 增删改查 的这个顺序应该保持一致 或者只用一种比较符号
			if (x < bST->Data)
				bST = bST->Left;
			else if (bST->Data < x)//@x > t->Data
				bST = bST->Right;/*向右子树移动继续查找*/
			else
				return  bST;
 		}
		return nullptr;
	}
	template<typename LessCmp>
	Position findOf(Position bST, T const&x, LessCmp less){
		while (bST){
			if (less(bST->Data, x))
				bST = bST->Right;/*向右子树移动继续查找*/
			else if (less(x, bST->Data))
				bST = bST->Left;
			else
				return  bST;
		}
		return nullptr;
	}
	/*函数Insert将x插入二叉搜索树bST并返回结果树的根结点指针*/
	virtual Position Insert(Position bST, Element const&x){
		if (empty(bST))//树空 返回一个未使用结点
			bST = nodeCreater(x);
		else if (x < bST->Data)
			bST->Left = Insert(bST->Left, x);
		else if (bST->Data < x)//@x > bST->Data
			bST->Right = Insert(bST->Right, x);
		else {
			BinTree<T>::repetitionInsert(bST);
		}
		return bST;
	}
	/*从二叉搜索树bST中删除x,返回新根; 若x不在树中tag->!tag 返回原树根*/
	virtual Position Delete(Position bST, Element const&x, bool &tag){
		Position del = bST;
		/*阶段一:查找对应元素*/
		if (!bST)//未找到匹配元素 删除失败
			tag = false;//@调试结果 原来是 tag=false; tag = !tag;
		else if (x < bST->Data)
			bST->Left = Delete(bST->Left, x, tag);//子树根节点可能会发生变化
		else if (bST->Data < x)//@x > t->Data
			bST->Right = Delete(bST->Right, x, tag);
		/*阶段二:删除匹配元素*/
		else if (bST->Left && bST->Right){
			del = findMinOf(bST->Right);//用右子树的最小(或左子树最大)元素填充需删除的结点
			bST->Data = del->Data;
			/*阶段三:维持结构*/
			bST->Right = Delete(bST->Right, del->Data, tag);//删除右子树的最小元素
		}
		else {//只有一个结点或无结点(返回Left的nullptr)
			if (!bST->Left)
				bST = bST->Right;
			else
				bST = bST->Left;
			nodeEraser(del);
		}
		return bST;
	}
	//Delete时要用
	Position findMinOf(Position bST){
		while (bST && bST->Left)
			bST = bST->Left;
		return bST;
	}
	Position findMaxOf(BST bST){
		while (bST && bST->Right)
			bST = bST->Right;
		return bST;
	}
	// 顺序遍历(可中止的中序遍历)
	bool orderTraversal(BST bT, StopVister visit) {
		if (bT) {
			if (orderTraversal(bT->Left, visit))
				return true;
			if (visit(bT))//返回值表示是否中止
				return true;
			return orderTraversal(bT->Right, visit);
		}
		return false;
	}
	// 逆序遍历(tips:含有重复的元素时用顺序计算逆序排名是有问题的)
	bool reOrderTraversal(BST bT, StopVister visit) {
		if (bT) {
			if (reOrderTraversal(bT->Right, visit))
				return true;
			if (visit(bT))//返回值表示是否中止
				return true;
			return reOrderTraversal(bT->Left, visit);
		}
		return false;
	}
};

/*
红黑树不是严格平衡树，不需要在每次插入删除的时候调整，avl是严格平衡树，每次插入删除的时候都要旋转到平衡状态。
从统计角度来说红黑树的性能要高一点，但查找操作并不能严格保证O(logN)，所以如果一棵树很少修改的话用AVL或许能快一点，
但话说回来，如果几乎不修改的话用有序数组+二分查找性能比各种树都要好。
*/
/*AVL平衡树 -------------------不能瞎用二叉树的操作(比如删除)*/
template<class T>//多重继承
class AvlTree :public LinkedBinSearchTree<T> {
	typedef typename LinkedBinSearchTree<T>::BST AVLT;
	typedef typename LinkedBinSearchTree<T>::Position Position;
	typedef typename BinTree<T>::BT BT;
	using Element = T;
	using Tree::Max;
	using LinkedBinSearchTree<T>::root_;
	typedef class AvlNode : public BinTree<T>::BTNode{
	public:
		int Height = 0;//树高  Data意义同BinTree
		int getValue()override{
			return Height;
		}
		void setValue(int h)override{
			Height = h;
		}
		int getHeight()override{
			return Height;
		}
		void setHeight(int h)override{
			Height = h;
		}
		AvlNode(int tData, int height_)
			:BinTree<T>::BTNode(tData){
				Height = height_;
			}
		virtual ~AvlNode()override{
			DE_PRINTF("AvlNode析构");
		}
	}*AvlTr;

	//计算avl树t的高度(深)
	int heightOf(AVLT t){//其实与BinTree的depth一样 只不过为了提高效率avl增加了一个height域并且对算法略作优化
		if (!t)
			return 0;
		else if (t->Left && t->Right)
			return Max(t->Left->getHeight(), t->Right->getHeight()) + 1;
		else if (t->Left)
			return t->Left->getHeight() + 1;
		else if (t->Right)
			return t->Right->getHeight() + 1;
		else
			return 1;
		/*展开下一层后效率提高特别大 优于下面的简易写法*/
		//return t ? Max(heightOf(t->Left), heightOf(t->Right)) + 1 : 0;
	}
	/*两种概念:Rotation(旋转) 和 Elevate提升
	(本实现与书上经典实现相同 但并没有旋转概念)
	有两种理解方式或设计原则: 见名知意于方法效果; 见名知意于方法调用
	四种调整策略中应用不同的概念解释和记忆或多或少自相矛盾
	斟酌了很久最后用提升概念(旋转概念太相对了 而且老让我觉得他所谓的左旋其实是右旋 而且就算是旋转也解释不通)
	给了原有算法一个新的解释 达到了无歧义的理解效果*/

	/*左侧提升(LL) :将的左子节点B做提升至A处,A变为B提升方向上的子结点 更新树高 返回新根B*/
	Position leftElevate(Position A){
		Position B = A->Left;
		A->Left = B->Right;
		B->Right = A;
		A->setHeight(Max(heightOf(A->Left), heightOf(A->Right)) + 1);
		B->setHeight(Max(heightOf(B->Left), A->getHeight()) + 1);//a的height上面已经计算 直接获取即可
		return B;
	}
	/*右侧提升(RR) :将的左子节点B做提升至A处,A变为B提升方向上的子结点 更新树高 返回新根B*/
	Position rightElevate(Position A){
		Position B = A->Right;
		A->Right = B->Left;
		B->Left = A;
		A->setHeight(Max(heightOf(A->Left), heightOf(A->Right)) + 1);
		B->setHeight(Max(heightOf(B->Right), A->getHeight()) + 1);
		return B;
	}
	/*问题出现在左子树的右侧:先对A的左子树做右侧提升 再对A做左侧提升*/
	Position LR(Position A){
		A->Left = rightElevate(A->Left);
		return leftElevate(A);
	}
	/*问题出现在右子树的左侧:先对A的右子树做左侧提升 再对A做右侧提升*/
	Position RL(Position A){
		A->Right = leftElevate(A->Right);/*更新A的右子树*/
		return rightElevate(A);
	}
	/*将x插入平衡树 并返回调整后的平衡树*/
	Position Insert(Position avlT, Element const&x)override{
		if (!avlT){/*若插入空树 则新建一个包含结点的树*/
			avlT = BinTree<T>::nodeCreater(x);
		}
		else if (x < avlT->Data){/*往左子树插入*/
			avlT->Left = Insert(avlT->Left, x);/*avlT的平衡因子BF*/
			if (heightOf(avlT->Left) - heightOf(avlT->Right) == 2){/*若插入后其高度height不平衡*/
				if (x < avlT->Left->Data)/*LL型 直接做左侧提升即可*/
					avlT = leftElevate(avlT);
				else/*LR型调用LR方法*/
					avlT = LR(avlT);
			}
		}
		else if (x > avlT->Data){
			avlT->Right = Insert(avlT->Right, x);
			if (heightOf(avlT->Left) - heightOf(avlT->Right) == -2){
				if (x > avlT->Right->Data)
					avlT = rightElevate(avlT);
				else
					avlT = RL(avlT);
			}
		}
		else{
			BinTree<T>::repetitionInsert(avlT);
		}
		avlT->setHeight(Max(heightOf(avlT->Left), heightOf(avlT->Right)) + 1);/*更新树高*/
		return avlT;
	}
public:
	AvlTree(){}
	~AvlTree(){
		DE_PRINTF("AVL析构");
	}
};

//表达式树 @TODO 并未通过PTA测试(主要是后缀表达式计算时的负号问题 考虑添加前缀表达式计算)
class ExpressionTree : private BinTree<JCE::String>{
public:
	enum ExpressionTypeEnum{ 
		PREFIX_EXPRESSION,//前缀表达式
		INFIX_EXPRESSION,//中表达式
		POST_EXPRESSION,//后缀表达式
		ERROR_EXPRESSION//错误的表达式
	};
	//中缀表达式构建的表达式树不唯一
	ExpressionTree(string express, ExpressionTypeEnum type_){
		expression = express;
		type = type_;
	}
	//类型检查 检查完成后将类型置为willBe?
	bool check(ExpressionTypeEnum should, ExpressionTypeEnum willBe){
		if (type != should){
			return false;
		}
		else{
			if (should == ORDER_PREFIX_ROOT){
				int i = 0, countNum = 0, countOp = 0;
				char num[10] = { 0 };
				do{
					if (getNum(&i, expression.c_str(), num) != '\0')
						++countOp;
					else
						++countNum;
				} while (expression[i++] != '\0');
				//操作符个数检查:正确的二元前缀表达式中，用空格间隔的操作数必然比操作符多一个
				if (countNum - countOp != 1){
					type = ERROR_EXPRESSION;
					return false;
				}
				else{
					type = willBe;
					return true;
				}
			}
			else{
				_DEBUG_ERROR("后缀向前缀转换还没写");
				return false;
			}
		}
	}
	//返回下一个运算符号
	string nextOper(){
		static size_t ind = 0;
		string next;
		for (; ind < expression.size(); ind++){
			if (expression[ind] != ' ')
				next += expression[ind];
			else
				break;
		}
		ind++;
		return next;
	}
	/*
	1. 从左往右扫描串
	　2. 遇到操作符则递归构造树节点，当前操作符是根节点，并递归构造左右子节点
	 　3. 后序遍历当前结果，并返回
	  */
	//表达式转换  不允许不检查直接调用
	string prefixToPost(){
		string next = nextOper(), ans;
		if (!isdigit(next[0])){
			Position node = nodeCreater(next);//递归构造树
			node->Left = nodeCreater(prefixToPost());
			node->Right = nodeCreater(prefixToPost());
			postTraversal(node, [&](BT const bT){
				if (ans != "")
					ans += " " + bT->Data;
				else
					ans = bT->Data;
			});//后序遍历转换
			delete node->Left;
			delete node->Right;
			delete node;
			return ans;
		}
		else
			return next;
	}
	//transform
	string toPost(){
		if (type == PREFIX_EXPRESSION){
			if (check(PREFIX_EXPRESSION, POST_EXPRESSION))
				return expression = prefixToPost();
			else
				return "0";
		}
		else{
			_DEBUG_ERROR("方法调用错误: 只能是前缀向后缀转换");
			return "0";
		}
	}
	double eval(){
		if (type == POST_EXPRESSION){
			if (check(POST_EXPRESSION, POST_EXPRESSION))
				return evalPostfix();
			else
				return MAX_INT32;
		}
		else if (type == PREFIX_EXPRESSION){
			toPost();
			return eval();
		}
		else if (type == ERROR_EXPRESSION)
			return MAX_INT32;
		else{
			_DEBUG_ERROR("无法直接计算中缀表达式");
			return MAX_INT32;
		}
	}
	/*后缀表达式计算*/

	/*如果两个空格之间是操作数则返回该操作 否则将数字存于num中并返回'\0'*/
	char getNum(int *i, char const*ori, char *num){
		int sub = 0;
		while (1){
			if (ori[*i] == ' ' || ori[*i] == '\0'){
				num[sub] = '\0';
				if (sub == 1 && !isdigit(num[0]))
					return num[0];
				else
					return '\0';
			}
			else{
				num[sub++] = ori[(*i)++];
				continue;
			}
		}
	}
	//返回后缀表达式ans的值 不合法返回MAX_INT32
	double evalPostfix(){
		int i = 0;
		char c = 0;
		static char num[50] = { 0 };
		stack<double> ps;
		double po1, po2;
		do{
			if ((c = getNum(&i, expression.c_str(), num)) == '\0'){
				ps.push(atof(num));
			}
			else{
				if (ps.empty()){//运算符号缺少 或者多余
					return MAX_INT32;
				}
				po1 = ps.top();
				ps.pop();
				if (ps.empty()){
					return MAX_INT32;
				}
				po2 = ps.top(); ps.pop();
				switch (c){
				case '+':ps.push(po2 + po1); break;
				case '-':ps.push(po2 - po1); break;
				case '*':ps.push(po2 * po1); break;
				case '/':{
							 if (po1 - 0.0 == 0){//除以0错误
								 return MAX_INT32;
							 }
							 else
								 ps.push(po2 / po1); break;
				}
				default:break;
				}
			}
		} while (expression[i++] != 0);
		double re = ps.top(); ps.pop();
		if (ps.empty()){
			return re;
		}
		else{//运算数多余
			return MAX_INT32;
		}
	}
private:
	//BinTree base;
	string expression;
	//表达式当前类型
	ExpressionTypeEnum type;
};

#endif
