#pragma once
#include "TreeObject.h"

// 完全二叉树 (实现了完全二叉树的内存分配: 允许[]操作 + 增减都在数组的尾部)
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
	virtual ~CompleteBinTree() {
		delete[] baseNodeArray;
		baseNodeArray = nullptr;
		DE_PRINTF("CBT析构");
	}

	int size() override {
		return validNodeNum;
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

	// [完全二叉树]的序列构建  (数据源 根结点下标) 返回根结点
	Position buildCompleteTree(T *first, T *last, int rootSub) {
		int n = last - first;
		if (n == 0)
			return nullptr;
		int nl = getLeftScaleL(n);
		baseNodeArray[rootSub].Data = first[nl];
		// first由0开始数nl个即是子树根 (root_在baseNodeArray中sub为0 左子树sub = 2*rootSub + 1)
		baseNodeArray[rootSub].Left = buildCompleteTree(first, first + nl, 2 * rootSub + 1);
		baseNodeArray[rootSub].Right = buildCompleteTree(first + nl + 1, last, 2 * rootSub + 2);
		return position(rootSub);
	}
	// 完全二叉树层序遍历(顺序遍历有序数组)
	void levelTraversal(CBT bT, std::function<void(CBT)> visit) {
		//无法override 但名字应该覆盖了
		if (bT == nullptr)
			return;
		FOR(i, 0, validNodeNum) {
			//bT必须是root_不能是baseNodeArray(root_ = baseNodeArray+1)
			if (!visit(bT + i))
				return;//中止
		}
	}
	
	//返回两个结点的公共祖先 越界返回哨兵(0号结点)
	Position ancestor(int i1, int i2) {
		if (i1 > validNodeNum || i2 > validNodeNum || i1 < 0 || i2 < 0)
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
	CompleteBinTree(int nSize) {
		baseNodeArray = new BTNode<T>[nSize];
		capacity = nSize;
	}
	void destroy(Position &r) override {
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
				// nodeEraser(current);
				--validNodeNum;
			}
			r = nullptr;
		}
	}
	bool full() const {
		return validNodeNum == capacity;
	}
	// 返回数组内的结点编号 1号为root_ 0号为哨兵
	int index(CBT t) const {
		// 第一个元素插入时root_=nullptr 所以链接左右孩子链接方法不能用root_ 更何况root_=1时的规律是针对数组成立的
		return t - baseNodeArray;
	}
	// 返回数组内的结点位置
	Position position(int sub) {
		assert(0 <= sub && sub < capacity);
		return baseNodeArray + sub;
	}
	
	// 此内存不归此类管理
	// BinTreeAlgorithm::LinearNodeManager<T> &baseNodeArray;
	BTNode<T> *baseNodeArray = nullptr;
	// 由于Heap中0号充当哨兵, 不计入总结点数, 故Heap中既是baseNodeArray尾元素的下标, 又是结点总数
	// 只有Heap中的这个值的含义与其余的有差别 故这个挪到这里才最合适
	int validNodeNum = 0;
	int capacity = 0;
private:
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
	//using BinTree<T>::nodeManager_;
	using CompleteBinTree<T>::baseNodeArray;
	using CompleteBinTree<T>::validNodeNum;
	using CompleteBinTree<T>::capacity;

	using CompleteBinTree<T>::position;
	using CompleteBinTree<T>::index;

public:
	using CompleteBinTree<T>::full;
	using CompleteBinTree<T>::empty;

	// (堆的大小, 哨兵, 小于比较方法, 任意初始化序列序列 该序列的元素个数) 
	// 最小堆 参数: moreCmper(大于)哨兵(最小值)	注意: 负数太小减法会变正:传入MAX_INT32/2即可
	// 最大堆 参数: lessCmper(小于)哨兵(最大值)
	// 注意: 用数组构造的堆与插入初始化的堆 的结构不一样
	Heap(int heapSize, T sentry, int(*cmper)(const T &, const T &), T *initArr = nullptr, int initArrSize = 0)
		: CompleteBinTree<T>(heapSize + 1) {
		build(initArr, initArrSize, sentry, cmper);
	}
	virtual ~Heap() override {
		DE_PRINTF("Heap析构");
	}

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
			percolateUp(item);
			// 因为新加入的元素始终在尾部 因此直接链接尾部的元素即可保证新添元素的链接
			linkToParent(validNodeNum + 1);
		}
		return result;
	}

	Element pop() {
		if (empty()) {
			throw std::exception("堆已空 无法抛出");
		}
		else {
			// 取出即将返回的值
			Element item = root_->Data;
			int delSub = validNodeNum;
			// 解除已删除的尾元素与父结点的链接
			unlinkToParent(delSub);
			// 从堆根开始下滤 删除尾元素(validNodeNum--即可)
			percolateDown(1, delSub);
			return item;
		}
	}

	// 重新构建堆 复杂度O(N)
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
		// 更改比较方法一定得更改哨兵 反之不一定
		baseNodeArray[0].Data = sentry;
		if (cmper != nullptr) {
			cmperFun = cmper;
		}
		assert(cmperFun != nullptr);
		if (empty(root_)) {
			// root_不包括哨兵(0号) 内存空间会自动多申请一个
			root_ = position(1);
			validNodeNum = 0;
		}
	}

	// 构建堆
	void build(T *initArr, int initArrSize, T sentry, int(*cmper)(const T &, const T &) = nullptr) {
		initialize(sentry, cmper);
		for (int i = 0; i < initArrSize; ++i) {
			// 确保哨兵是列表中所有元素的极值(最小堆对应极小值, 最大堆对应极大值), 即lhs比rhs大或小恒成立, 成立则比较值>0,不成立则<0,相等则=0
			assert(cmperFun(baseNodeArray[0].Data, *initArr) > 0);
			baseNodeArray[1 + i].Data = *initArr;
			linkToParent(1 + i);
			++initArr;
		}

		// 数据改变一定得重建堆
		rebuild();
	}

protected:
	// 上滤 将尾部元素item与父结点逐个比较, 并置于合适的位置; push调整
	void percolateUp(Element const &item) {
		// puSub指向[插入后]堆中最后一个元素的位置 puSub=[1, capbility)
		int puSub = validNodeNum + 1;
		for (int parentSub = -1; cmperFun(baseNodeArray[(parentSub = calcParentSubPosition(puSub))].Data, item) < 0; puSub = parentSub) {
			// 从最后一个有孩子的结点开始 向上过滤结点
			baseNodeArray[puSub].Data = baseNodeArray[parentSub].Data;
		}
		// 压入
		baseNodeArray[puSub].Data = item;
	}

	// 下滤 将Heap中pdSub处的元素提升至parentSub处 然后将以 position(parentSub) 为根的子堆调整为最大或最小或最小堆; build|pop调整
	void percolateDown(int parentSub, int pdSub) {
		// 取出需要下滤的值
		Element pdEle = baseNodeArray[pdSub].Data;
		for (int childSub = -1; (childSub = calcLeftChildSub(parentSub)) <= validNodeNum; parentSub = childSub) {
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

	// =>链接关系只与位置有关 与数据无关 因此除非是实际使用的位置增减 否则不用重链

	// 在指定子结点和其父结点间建立链接
	void linkToParent(int childIndex) {
		int parentIndex = calcParentSubPosition(childIndex);
		Position parent = position(parentIndex);
		if (!empty(parent)) {
			int lSub = calcLeftChildSub(parentIndex);
			int rSub = calcRightChildSub(parentIndex);
			if (lSub == childIndex) {
				// 若抽象成功 则就算之前使用数组实现的Heap都可以转换为链接实现了
				// baseNodeArray.nodeCreater(parent->Left);
				parent->Left = position(lSub);
				++validNodeNum;
			}
			else if (rSub == childIndex) {
				// baseNodeArray.nodeCreater(parent->Right);
				parent->Right = position(rSub);
				++validNodeNum;
			}
			else;
		}
	}
	// 解除指定子结点和其父结点间的链接
	void unlinkToParent(int childIndex) {
		Position parent = calcParentPosition(childIndex);
		if (!empty(parent)) {
			Sub parentIndex = index(parent);
			// 置空无效子结点
			int lSub = calcLeftChildSub(parentIndex);
			int rSub = calcRightChildSub(parentIndex);
			if (lSub == childIndex) {
				// baseNodeArray.nodeEraser(parent->Left);
				parent->Left = nullptr;
				--validNodeNum;
			}
			else if (rSub == childIndex) {
				// baseNodeArray.nodeEraser(parent->Right);
				parent->Right = nullptr;
				--validNodeNum;
			}
			else;

			// 当子结点是root_的时候需要将root_置为空
			if (parent == position(0)) {
				root_ = nullptr;
			}
		}
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
};
