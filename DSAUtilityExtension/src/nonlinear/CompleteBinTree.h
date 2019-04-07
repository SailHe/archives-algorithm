#pragma once
#include "TreeObject.h"

// 完全二叉树 (实现了完全二叉树的内存分配: 允许[]操作 + 增减都在数组的尾部; 其实没什么存在的必要 但留着好了)
template<typename T>
class CompleteBinTree :public BinTree<T> {

	using CBT = typename BinTree<T>::BT;
	using Position = typename BinTree<T>::Position;
	using BinTree<T>::BTNode;
	using BinTree<T>::empty;

	using BinTree<T>::root_;
	//using BinTree<T>::nodeManager_;

public:
	// 直接把数组copy进静态数组里
	// CompleteBinTree(T *initArr, int nSize) : CompleteBinTree(nSize){
	// 	assert(initArr != nullptr);
	// 	root_ = baseNodeArray;
	// 	for (int i = 0; i < nSize; ++i)
	// 		root_[i].Data = initArr[i];
	// 	initEnd(nSize);
	// }

	// sort(initArr, initArr + n, less<T>());
	// 翻转构造:sort(initArr, initArr + n, greater<T>()); 完全二叉树的左大右小树不支持左小右大树的镜像翻转 普通BinTree支持
	// 使用数组构造唯一的完全二叉树 tips::若数组有序 则为搜索树
	CompleteBinTree(int nSize, T *initArr) : CompleteBinTree(nSize) {
		assert(initArr != nullptr);
		// 0表示根结点的Sub
		root_ = buildCompleteTree(initArr, initArr + nSize, 0);
	}
	// 构建基于数组的虚拟链接二叉树
	CompleteBinTree(
		int nSize,
		std::function<void(T *)> getData,
		std::function<void(Sub *, Sub *)> getSub, int noneSub,
		int customRootSub = -1
	) : CompleteBinTree(nSize) {
		// 其实这个方法得到的不一定是完全二叉树 但姑且只能这么实现了
		Sub rootSub = buildBinTreeStructure(baseNodeArray, getData, getSub, noneSub, nSize);
		if (customRootSub > 0) {
			_ASSERT_EXPR(customRootSub < nSize, "指定根下标越界!");
			rootSub = customRootSub;
		}
		else {
			// DNT
		}
		root_ = position(rootSub);
	}

	CompleteBinTree(CompleteBinTree const &rhs) : BinTree<T>(rhs) {
		DE_PRINTF("CBT拷贝构造");
		*this = rhs;
	}
	CompleteBinTree(CompleteBinTree &&rvalue) : BinTree<T>(rvalue) {
		DE_PRINTF("CBT移动构造");
		*this = std::move(rvalue);
	}
	CompleteBinTree &operator= (CompleteBinTree const &rhs) {
		baseNodeArray = BinTreeUtil::CompleteNodeManager<T>(rhs.size());
		return *this;
	}
	CompleteBinTree &operator= (CompleteBinTree &&rvalue) {
		std::swap(baseNodeArray, rvalue.baseNodeArray);
		return *this;
	}

	virtual ~CompleteBinTree() {
		// delete[] baseNodeArray;
		// baseNodeArray = nullptr;
		//root_ = nullptr;
		DE_PRINTF("CBT析构");
	}

	int size() override {
		return baseNodeArray.createdNodeNum();
	}

	// 返回StaBinSearchTree左子树的规模 (总结点数)
	static int getLeftScaleL(double n) {
		// 2^h - 1 + x = n
		// 向下取整
		int h = (int)log2(n + 1);
		// 最下层单出的结点数
		int x = (int)(n + 1 - pow(2.0, h));
		return (int)pow(2.0, h - 1) - 1 + Tree::Min(x, (int)pow(2.0, h - 1));
	}

	// 完全二叉树层序遍历(顺序遍历有序数组)
	void levelTraversal(CBT bT, std::function<void(CBT)> visit) {
		//无法override 但名字应该覆盖了
		if (bT == nullptr)
			return;
		FOR(i, 0, baseNodeArray.createdNodeNum()) {
			//bT必须是root_不能是baseNodeArray(root_ = baseNodeArray+1)
			if (!visit(bT + i))
				return;//中止
		}
	}
	
	// 返回两个结点的公共祖先 越界返回哨兵(0号结点)
	Position ancestor(int i1, int i2) {
		if (i1 > baseNodeArray.createdNodeNum() || i2 > baseNodeArray.createdNodeNum() || i1 < 0 || i2 < 0)
			return position(0);
		if (i1 == i2)
			return position(i1);
		else if (i1 > i2)
			return ancestor(index(calcParentPosition(position(i1))), i2);
		else
			ancestor(i1, index(calcParentPosition(position(i2))));//return
	}

protected:
	// 初始化构造
	CompleteBinTree(int nSize) : baseNodeArray(nSize) {
		// baseNodeArray = new BTNode<T>[nSize];
		// capacity = nSize;
	}

	Position nodeCreater(T const &tData) override {
		return baseNodeArray.nodeCreater(tData);
	}
	void nodeEraser(Position &del) override {
		return baseNodeArray.nodeEraser(del);
	}

	// [完全二叉树]的序列构建  (数据源 根结点下标) 返回根结点
	template<typename Iterator>
	Position buildCompleteTree(Iterator begin, Iterator end, int rootSub) {
		int n = end - begin;
		if (n == 0)
			return nullptr;
		int nl = getLeftScaleL(n);
		baseNodeArray[rootSub].Data = begin[nl];
		// begin由0开始数nl个即是子树根 (root_在baseNodeArray中sub为0 左子树sub = 2*rootSub + 1)
		baseNodeArray[rootSub].Left = buildCompleteTree(begin, begin + nl, 2 * rootSub + 1);
		baseNodeArray[rootSub].Right = buildCompleteTree(begin + nl + 1, end, 2 * rootSub + 2);
		return position(rootSub);
	}

	// 通过输入构建树结构 返回根结点 (数组内孩子结点的下标获取函数)
	static Sub buildBinTreeStructure(
		BinTreeUtil::CompleteNodeManager<T> &nodeArray,
		std::function<void(T *)> getData,
		std::function<void(Sub *, Sub *)> getSub,
		int noneSub, int capacity
	) {
		int nSize = capacity;
		int sum = (nSize - 1)*nSize / 2;
		int leftSub = -1, rightSub = -1;

		for (int i = 0; i < nSize; i++) {
			Position leftChild = nullptr, rightChild = nullptr;
			getData(&nodeArray[i].Data);
			getSub(&leftSub, &rightSub);
			if (leftSub != noneSub) {
				leftChild = nodeArray.position(leftSub);
				sum -= leftSub;
			}

			if (rightSub != noneSub) {
				rightChild = nodeArray.position(rightSub);
				sum -= rightSub;
			}
			nodeArray[i].Left = leftChild, nodeArray[i].Right = rightChild;
		}
		_ASSERT_EXPR(0 <= sum && sum < nSize, "给定数据有误!");
		return nSize == 0 ? -1 : sum;
	}

	bool full() const {
		return baseNodeArray.full();
	}
	// 返回数组内的结点编号 1号为root_ 0号为哨兵
	int index(CBT t) const {
		return baseNodeArray.index(t);
	}
	// 返回数组内的结点位置
	Position position(int sub) {
		// assert(0 <= sub && sub < capacity);
		// return baseNodeArray + sub;
		return baseNodeArray.position(sub);
	}
	
	// 此内存不归此类管理
	BinTreeUtil::CompleteNodeManager<T> baseNodeArray;
	// BTNode<T> *baseNodeArray = nullptr;
private:
	// int capacity = 0;
};

/*
堆 是最大(小)树, 完全二叉树
建堆O(N) 入堆O(logN) 出堆O(logN)
排序平均O(N*logN): N次出堆
*/
template<class T>
class Heap : public CompleteBinTree<T> {

	typedef T Element;
	typedef typename BinTree<T>::Position Position;

	using BinTree<T>::root_;
	using CompleteBinTree<T>::baseNodeArray;

	using CompleteBinTree<T>::position;
	using CompleteBinTree<T>::index;

public:
	using CompleteBinTree<T>::full;
	using CompleteBinTree<T>::empty;

	// (堆的大小, 哨兵, 小于比较方法, 任意初始化序列序列 该序列的元素个数) 
	// 最小堆 参数: moreCmper(大于)哨兵(最小值)	注意: 负数太小减法会变正:传入MAX_INT32/2即可
	// 最大堆 参数: lessCmper(小于)哨兵(最大值)
	Heap(int heapSize, T sentry, int(*cmper)(const T &, const T &), T *initArr = nullptr, int initArrSize = 0)
		: CompleteBinTree<T>(heapSize + 1) {
		// @TODO 原地初始化建堆(传入的类型与堆的存储类型有差异)
		build(initArr, initArr + initArrSize, sentry, cmper);
	}
	virtual ~Heap() override {
		DE_PRINTF("Heap析构");
	}

	int size() override {
		assert(validNodeNum == baseNodeArray.createdNodeNum() - 1);
		return validNodeNum;
	}

	// 向堆中压入一个新项 注意: 用数组初始化构造的堆与push出的堆两者结构不一样
	bool push(Element const &item) {
		bool result = true;
		if (isNotInit()) {
			// 一般只可能是pop变空了, 重新使用之前的数据初始化即可
			initialize(baseNodeArray[0].Data);
		}
		if (full()) {
			result = false;
			throw std::exception("堆已满 无法压入");
		}
		else {
			// [插入后]堆中最后一个元素的下标
			int puSub = validNodeNum + 1;
			// 新增一个元素(validNodeNum++)
			linkNewNodeToParent(item);
			percolateUp(puSub);
		}
		return result;
	}

	// 弹出堆中的根结点对应的数据
	Element pop() {
		if (empty()) {
			throw std::exception("堆已空 无法抛出");
		}
		else {
			// 取出即将返回的值
			Element item = root_->Data;
			// 当前尾元素下标
			int delSub = validNodeNum;
			// 删除尾元素(validNodeNum--)
			unlinkLastNodeToparent();
			// 从堆根开始下滤
			percolateDown(1, delSub);
			return item;
		}
	}

	// 重新构建堆 时间复杂度O(N)
	void rebuild() {
		if (isNotInit()) {
			throw std::exception("未初始化");
		}
		// 对于一个已读入的数据但需要调整的最大或最小或最小堆(自下而上调整)
		// 从[最后一个子节点的父节点 即 倒数第一个有儿子的结点]开始
		// 取出该结点后每次从左右结点中挑一个"大"的结点做[下滤]操作开始调整到根节点1
		for (int i = calcParentSubPosition(validNodeNum); i > 0; i--) {
			percolateDown(i, i);
		}
	}

	// 初始化
	void initialize(T sentry, int(*cmper)(const T &, const T &) = nullptr) {
		// assert(validNodeNum = 0); // 初始化前使用clear或pop使容器变空的情况如此  不过rebuild时有区别(保留原有数据)
		baseNodeArray.clear();
		linkToParent(validNodeNum, sentry);

		// 更改比较方法一定得更改哨兵 反之不一定
		if (cmper != nullptr) {
			cmperFun = cmper;
		}
		assert(cmperFun != nullptr);

		if (empty(root_)) {
			// root_不包括哨兵(0号) 内存空间会自动多申请一个
			root_ = position(1);
		}
	}

	// 构建堆 (支持自加和解引用的迭代器范围, 哨兵, 比较方法)
	template<typename Iterator>
	void build(Iterator begin, Iterator end, T sentry, int(*cmper)(const T &, const T &) = nullptr) {
		initialize(sentry, cmper);
		while(begin != end) {
			// 确保哨兵是列表中所有元素的极值(最小堆对应极小值, 最大堆对应极大值), 即lhs比rhs大或小恒成立, 成立则比较值>0,不成立则<0,相等则=0
			assert(cmperFun(baseNodeArray[0].Data, *begin) > 0);
			linkNewNodeToParent(*begin);
			++begin;
		}

		// 数据改变一定得重建堆
		rebuild();
	}

protected:
	void nodeEraser(Position &del) override {
		baseNodeArray.nodeEraser(del);
		validNodeNum = baseNodeArray.createdNodeNum() - 1;
	}
	// 上滤 将尾部元素item与父结点逐个比较, 并置于合适的位置; push调整
	void percolateUp(Sub puSub) {
		Element item = baseNodeArray[puSub].Data;
		for (int parentSub = -1;
			cmperFun(baseNodeArray[(parentSub = calcParentSubPosition(puSub))].Data, item) < 0; puSub = parentSub) {
			// 从最后一个有孩子的结点开始 向上过滤结点
			baseNodeArray[puSub].Data = baseNodeArray[parentSub].Data;
		}
		// 压入
		baseNodeArray[puSub].Data = item;
	}

	// 下滤 将Heap中pdSub处的元素提升至parentSub处 然后将以 position(parentSub) 为根的子堆调整为最大或最小或最小堆; build | pop调整
	void percolateDown(int parentSub, int pdSub) {
		// 取出需要下滤的值
		Element pdEle = baseNodeArray[pdSub].Data;
		for (int childSub = -1; 
			(childSub = calcLeftChildSub(parentSub)) <= validNodeNum; parentSub = childSub) {
			// 选取存在的左右儿子中大或小的一个
			if (childSub != validNodeNum && cmperFun(baseNodeArray[childSub].Data, baseNodeArray[childSub + 1].Data) < 0)
				childSub++;

			if (cmperFun(pdEle, baseNodeArray[childSub].Data) >= 0)
				break;
			else {
				// 将孩子上移 <==> 将pdEle下移 由于childSub还要作为parentSub参与后续的比较因此不能移动
				baseNodeArray[parentSub].Data = baseNodeArray[childSub].Data;
			}
		}
		baseNodeArray[parentSub].Data = std::move(pdEle);
	}

	// 返回是否已初始化
	bool isNotInit() {
		return root_ == nullptr;
	}

	// =>连接关系只与位置有关 与数据无关 因此除非是实际使用的位置增减 否则不用重链

	// 在指定子结点和间建立连接
	void linkToParent(int childIndex, Element const &item) {
		int parentIndex = calcParentSubPosition(childIndex);
		Position parent = position(parentIndex);
		assert(!empty(parent));
		const int lSub = calcLeftChildSub(parentIndex);
		const int rSub = calcRightChildSub(parentIndex);
		if(childIndex == lSub) {
			parent->Left = baseNodeArray.nodeCreater(item);
			// parent->Left = position(lSub);
			// parent->Left->Data = item;
		}
		else if (childIndex == rSub) {
			parent->Right = baseNodeArray.nodeCreater(item);
			// parent->Right = position(rSub);
			// parent->Right->Data = item;
		}
		else {
			assert(false);
		}
	}
	// 在尾部新建一个结点并与其父结点连接
	void linkNewNodeToParent(Element const &item) {
		linkToParent(validNodeNum + 1, item);
		++validNodeNum;
	}
	// 解除指定子结点和其父结点间的连接
	void unlinkToParent(int childIndex) {
		Position parent = calcParentPosition(childIndex);
		assert(!empty(parent));
		Sub parentIndex = index(parent);
		// 置空无效子结点
		int lSub = calcLeftChildSub(parentIndex);
		int rSub = calcRightChildSub(parentIndex);
		if (lSub == childIndex) {
			baseNodeArray.nodeEraser(parent->Left);
			// parent->Left = nullptr;
		}
		else if (rSub == childIndex) {
			baseNodeArray.nodeEraser(parent->Right);
			// parent->Right = nullptr;
		}
		else {
			assert(false);
		}

		// 当子结点是root_的时候需要将root_置为空
		if (parent == position(0)) {
			root_ = nullptr;
		}
	}
	// 解除尾部结点与其父结点的连接
	void unlinkLastNodeToparent() {
		unlinkToParent(validNodeNum);
		--validNodeNum;
	}

	// =>0号是哨兵; 1号是根结点

	// 返回左子树下标
	Sub calcLeftChildSub(int parentIndex) {
		return 2 * parentIndex;
	}
	Position calcLeftChildPosition(int parentIndex) {
		int sub = calcLeftChildSub(parentIndex);
		return 0 < sub && sub <= validNodeNum ? position(sub) : nullptr;
	}
	// 返回右子树下标
	Sub calcRightChildSub(int parentIndex) {
		return 2 * parentIndex + 1;
	}
	Position getRightChild(int parentIndex) {
		int sub = calcRightChildSub(parentIndex);
		return 0 < sub && sub <= validNodeNum ? position(sub) : nullptr;
	}
	// 返回父结点下标 root_的父结点对应哨兵
	Sub calcParentSubPosition(int sonIndex) {
		return sonIndex / 2;
	}
	Position calcParentPosition(int sonIndex) {
		return position(calcParentSubPosition(sonIndex));
	}
	// 返回兄弟结点
	Position calcSiblingPosition(int sibIndex) {
		// 1号是根结点没有兄弟
		return sibIndex > 1 ?
			sibIndex % 2 == 0 ? position(sibIndex + 1) : position(sibIndex - 1)
			: nullptr;
	}

private:
	// 使用less<T> greater<T> 或 比较符号重载 都有同样地问题: 两者没有同一个表示方法(带有重载()的父类)不便于大堆小堆的即时重构
	int(*cmperFun)(const T &, const T &) = nullptr;
	// 由于Heap中0号充当哨兵, 不计入总结点数, 故Heap中既是baseNodeArray尾元素的下标, 又是结点总数
	// 只有Heap中的这个值的含义与其余的有差别 故这个挪到这里才最合适
	int validNodeNum = 0;
};
