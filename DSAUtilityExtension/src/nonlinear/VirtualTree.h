#pragma once
#include "TreeObject.h"


/*
静态储存:将数组视为内存块, 在此内存块内"申请"结点实现二叉树的话 这与普通的链接实现无异 而且更为费事 不予考虑
顺序储存:将数组按顺序编号,1号作为根结点,之后 编号为2n是左子结点 2n+1是右子结点 是理想的完全二叉树存储方式
.首先,所有的树都可以用链式储存,其节约内存,但缺乏随机访问性
.以下几种情况可以考虑非链式储存的树结构
已知树(最好是完全二叉树)的结构(用序号代表子结点)	:静态储存(唯一用武之地)
已知是完全二叉树								:顺序储存
.已知数据查询(PS:高度Height = max(Hl, Hr) + 1)
普通数组储存 读入数据Θ(N)
不使用数据结构:			排序O(N*logN)				binary_search O(logN))			增删 O(N) = 顺序比对并移动
顺序储存的完全二叉搜索树	排序				构造Θ(N)		find		  O(logN)			增删 O(H) = 同BST但 H(CBT) = log(N)+1 即增删O(logN)
AVL平衡树	插入构造O(N*logN) 其余与CBST性质类似 但维护结构需要额外时间
BST			插入构造O(N*logN) 增删O(H) log(N-1) <= H(BST) <= N, 平均 H = log(N)+1
前者构造方便,查找效率与后者相当, 但后者只是根到叶局部有序, 维护成本远小于前者
.实现堆(CBT 最大小树)
普通数组:难以维护, 不考虑
链式BST实现的Heap:构造 Ω(N),		push O(H)	pop O(H)
顺序CBT实现的Heap:构造 O(N),		push O(logN)		pop O(logN)
使用顺序储存实现堆是最佳选择
*/
/*
使用数组虚拟的二叉树(不是ArrayBinTree)
静态二叉树Static Binary Tree(用数组储存的二叉树不一定是完全二叉树)
用StaBinTree实现便于方法内的子树下标的计算
*/
template<class T>
class VirtualLinkedBinTree :public virtual BinTree<T> {
protected:
	typedef typename BinTree<T>::Position Position;
	using BinTree<T>::root_;
	using BinTree<T>::BinTreeImplType;
public:
	using BinTree<T>::layers;
	using BinTree<T>::empty;
	typedef typename BinTree<T>::BT BTS;
	// 结构已知构造(结点数目, 数据获取方法, 下标获取方法, 空下标) 若结构并不总是完整的 但root_是特定已知的 可以指定root_的下标
	VirtualLinkedBinTree(
		int nSize, 
		std::function<void(T *)> getData, 
		std::function<void(Sub *, Sub *)> getSub, int noneSub, 
		int rootSub = -1
	) :VirtualLinkedBinTree(nSize) {
		// 构建链接二叉树
		root_ = buildStructure(getData, getSub, noneSub);
		if (rootSub > 0) {
			_ASSERT_EXPR(rootSub < capacity, "指定根下标越界!");
			root_ = baseArray + rootSub;
		}
		usedSize = capacity - 1;
	}
	virtual ~VirtualLinkedBinTree() override {
		//free(baseArray);
		delete[] baseArray;
		// 保证基类的正常析构
		root_ = nullptr;
		DE_PRINTF("VBT析构");
	}
	
protected:
	typedef typename BinTree<T>::Position NodeArray;
	using BinTree<T>::BTNode;
	typedef T ElementSBT;
	// Left和Right储存左右孩子位于数组内的地址 Varrays
	NodeArray baseArray = nullptr;
	// 数组容量, 最多元素(nSize 结点个数)个数至少为1
	int capacity = 0;
	// 有效的元素个数|已储存的|已使用的容量
	using BinTree<T>::usedSize;

	// 构造静态完全二叉树 (结点个数) 向上转型时会用到的root_和usedSize 必须需要子类初始化
	VirtualLinkedBinTree(int nSize) {
		BinTreeImplType = Tree::LinearBlock;
		baseArray = new BTNode[nSize];
		capacity = nSize;
	}

	// 通过输入构建树结构 返回根结点 (数组内孩子结点的下标获取函数)
	Position buildStructure(std::function<void(T *)> getData, std::function<void(Sub *, Sub *)> getSub, int noneSub) {
		int nSize = capacity;
		int i, sum = (nSize - 1)*nSize / 2;
		int leftSub, rightSub;

		for (i = 0; i < nSize; i++) {
			Position leftChild = nullptr, rightChild = nullptr;
			getData(&baseArray[i].Data);
			getSub(&leftSub, &rightSub);
			if (leftSub != noneSub) {
				leftChild = baseArray + leftSub;
				sum -= leftSub;
			}

			if (rightSub != noneSub) {
				rightChild = baseArray + rightSub;
				sum -= rightSub;
			}
			baseArray[i].Left = leftChild, baseArray[i].Right = rightChild;
		}
		_ASSERT_EXPR(0 <= sum && sum < nSize, "给定数据有误!");
		return nSize == 0 ? nullptr : baseArray + sum;
	}
	bool full() {
		return usedSize == capacity;
	}
	// 返回数组内的结点编号 1号为root_ 0号为哨兵
	int index(BTS t) const {
		/*第一个元素插入时root_=nullptr 所以链接左右孩子链接方法不能用root_ 更何况root_=1时的规律是针对数组成立的*/
		return t - baseArray;
	}
	// 返回数组内的结点位置
	Position position(int sub) {
		return baseArray + sub;
	}
};

// 完全二叉树
template<typename T>
class CompleteBinTree :public VirtualLinkedBinTree<T> {
	using VirtualLinkedBinTree<T>::root_;
public:
	// 直接把数组copy进静态数组里
	CompleteBinTree(T *iniA, int nSize): VirtualLinkedBinTree<T>(nSize){
		assert(iniA != nullptr && nSize > 0);
		root_ = baseArray;
		for (int i = 0; i < nSize; ++i)
			root_[i].Data = iniA[i];
		usedSize = nSize;
	}
	virtual ~CompleteBinTree() {
		DE_PRINTF("CBT析构");
	}

protected:
	using VirtualLinkedBinTree<T>::baseArray;
	using VirtualLinkedBinTree<T>::usedSize;
	using VirtualLinkedBinTree<T>::index;
	using Position = typename VirtualLinkedBinTree<T>::Position;

	// 用于Heap和VirtualLinkedBinSearchTree的初始化
	CompleteBinTree(int nSize) : VirtualLinkedBinTree<T>(nSize) {}

	Sub getLeftChildSub(int parentIndex) {
		return 2 * parentIndex;
	}
	Position getLeftChild(int parentIndex) {
		int sub = getLeftChildSub(parentIndex);
		return 0 < sub && sub <= usedSize ? baseArray + sub : nullptr;
	}
	Sub getRightChildSub(int parentIndex) {
		return 2 * parentIndex + 1;
	}
	Position getRightChild(int parentIndex) {
		int sub = getRightChildSub(parentIndex);
		return 0 < sub && sub <= usedSize ? baseArray + sub : nullptr;
	}
	// 返回父结点下标 1号是根结点没有父结点返回哨兵:0
	Sub getParentSub(int sonIndex) {
		return sonIndex / 2;
	}
	Position getParent(int sonIndex) {
		return baseArray + getParentSub(sonIndex);
	}
	Position getSibling(int SibIndex) {
		// 1号是根结点没有兄弟
		return SibIndex > 1 ?
			SibIndex % 2 == 0 ? baseArray + SibIndex + 1 : baseArray + SibIndex - 1
			: nullptr;
	}

};


//说白了就是被限制了内存空间的链接二叉树 但由于普通二叉树不常用(没有有存在意义的insert方法)
//出现这种冲突的根本原因在于 我将链接结点实现的 二叉树当成了一个既成标准(接口) 而初衷仅仅是为了可以便于向上转型 所以出现了一些矛盾
//(既想有链接实现的动态性和内存节约 又想有静态实现的随机访问效率
/*
可以传入一个重载了[]的容器给树 当作"结构内存"
如果该容器是顺序容器的话 在树的内部呈现动态性 在外部呈现静态性
这样一来静态实现与链接实现合二为一了 可是劣势也是明显的可以说又回到了当初创造链接结点的初衷:节约内存 以及在未知数据面前顺序实现的无力性
上一问题可以解决:树数据结构内部存在两种内存申请方法 一种是在传入的容器内存里申请 一种是在外部申请(有一个注册表用于注销这一部分内存 可以通过对注册表baseArray(指针数组)的访问达到即时随机访问)
以上全是意淫,始终有一个问题没有解决:顺序表删除某条内容后那条记录作废了 但你需要新内存时如何知道哪里有空内存 于是又需要查找
好吧可以将删除的内存指针与末尾内存指针交换(二叉树用的是指向数组的二级指针 相当于用到了三级指针(呸)) 但,这与普通的链接式二叉树又完全没区别了 这是理解了系统内存的运作方式?
*/
/*静态二叉搜索树Static Binary Search Tree*/
/*SequentialBinTree顺序储存的二叉树*/
template<class T>//虚继承(virtual必须写在中间) 解决多继承中菱形继承的情况下的基类成员的冲突
class VirtualLinkedBinSearchTree :public VirtualLinkedBinTree<T>, public virtual LinkedBinSearchTree<T> {
public:
	virtual ~VirtualLinkedBinSearchTree() override {
		DE_PRINTF("SCBT析构");
	}
protected:
	typedef typename BinTree<T>::Position Position;
	typedef typename BinTree<T>::BT VBT;
	typedef T Element;
	typedef Position structArray;
	using BinTree<T>::root_;
	using VirtualLinkedBinTree<T>::baseArray;
	using VirtualLinkedBinTree<T>::usedSize;
	using VirtualLinkedBinTree<T>::full;
	// 仅用于CompleteBinSearchTree的初始化
	VirtualLinkedBinSearchTree(int nSize) : VirtualLinkedBinTree<T>(nSize) {}

	Position nodeCreater(Element const &tData)override {
		Position newNode = nullptr;
		if (full()) {
			// DNT
		}
		else {
			baseArray[usedSize].Data = tData;
			newNode = baseArray + (usedSize++);
			BinTree<T>::lastInsertPosition = newNode;
			BinTree<T>::isInsert = BinTree<T>::lastInsertPosition == nullptr ? false : true;
		}
		return newNode;
	}
	virtual void nodeEraser(Position &del) override {
		// 0代表初始状态 只为了标识用 并无特殊用处
		// malloc就应用memeset初始化free释放 new自动初始化 赋值初始化 delete释放
		// del->Data = 0; // {}
		//静态数组的删除并非实际删除
		del->Left = del->Right = nullptr;
		del = nullptr;
		--usedSize;
	}
};

/*完全二叉搜索树 使用数组构造 实现对其的静态二分查找 若需要动态增删功能 需要向上转型为二叉搜索树*/
/*与二分查找是有区别的 二分查找的序列是有序的 建立CBST需要有序序列 但建立完后其本身的内置序列并非有序的(其首元素相当于序列中值)*/
/*这其实该叫顺序储存的二叉树而不是完全二叉树 只是说完全二叉树用顺序存储很完美*/
template<class T>
class CompleteBinSearchTree :public CompleteBinTree<T>, public virtual VirtualLinkedBinSearchTree<T> {
public:
	using Position = typename VirtualLinkedBinSearchTree<T>::Position;
	using VirtualLinkedBinTree<T>::full;
	using VirtualLinkedBinTree<T>::index;
	using VirtualLinkedBinTree<T>::position;
protected:
	typedef typename BinTree<T>::BT BT;
	typedef Position structArray;
	typedef T Element;
	using VirtualLinkedBinTree<T>::root_;
	using VirtualLinkedBinTree<T>::baseArray;
	using VirtualLinkedBinTree<T>::usedSize;
	using BinTree<T>::empty;
	using Tree::Min;

	/*返回StaBinSearchTree左子树的规模 (总结点数)*/
	int getLeftScaleL(double n) {
		/*2^h - 1 + x = n*/
		int h = (int)log2(n + 1);/*向下取整*/
		int x = (int)(n + 1 - pow(2.0, h));/*最下层单出的结点数*/
		return (int)pow(2.0, h - 1) - 1 + Min(x, (int)pow(2.0, h - 1));
	}

	//[完全二叉树]的序列构建  数据源数组(fitst, last) 目标数组baseArray		func(A, A+n, 0);
	Position buildComplete(T *first, T *last, int rootSub) {
		int n = last - first;
		if (n == 0)
			return nullptr;
		int nl = getLeftScaleL(n);
		baseArray[rootSub].Data = first[nl];
		// first由0开始数nl个即是子树根 (root_在baseArray中sub为0 左子树sub = 2*rootSub + 1)
		baseArray[rootSub].Left = buildComplete(first, first + nl, 2 * rootSub + 1);
		baseArray[rootSub].Right = buildComplete(first + nl + 1, last, 2 * rootSub + 2);
		return baseArray + rootSub;
	}
	/*完全二叉树层序遍历(顺序遍历有序数组)*/
	template<class Fun>
	void levelTraversal(BT bT, Fun visit) {//无法override 但名字应该覆盖了
		if (bT == nullptr)return;
		FOR(i, 0, usedSize) {//bT必须时root_不能是baseArray(root_ = baseArray+1)
			if (!visit(bT + i))
				return;//中止
		}
	}
public:
	/*sort(iniA, iniA + n, less<T>());
	**翻转构造:sort(iniA, iniA + n, greater<T>()); 完全二叉树的左大右小树不支持左小右大树的镜像翻转 普通BinTree支持*/
	//使用数组构造唯一的完全二叉树 (由于完全二叉树不能增删 没有usedSize和capacity之分) tips::若数组有序 则为搜索树
	CompleteBinSearchTree(int nSize, T *iniA) : CompleteBinTree<T>(nSize), VirtualLinkedBinSearchTree<T>(nSize) {
		assert(iniA != nullptr && nSize > 0);
		// 0表示根结点的Sub
		root_ = buildComplete(iniA, iniA + nSize, 0);
		this->usedSize = nSize;
	}
	virtual ~CompleteBinSearchTree() override {
		DE_PRINTF("CBT析构");
	}

	/*Position find(Element x){
		return nullptr;//not impl
	}*/
	//返回两个结点的公共祖先 越界返回哨兵(0号结点)
	Position ancestor(int i1, int i2) {
		if (i1 > usedSize || i2 > usedSize || i1 < 0 || i2 < 0)
			return position(0);
		if (i1 == i2)
			return position(i1);
		else if (i1 > i2)
			ancestor(index(getParent(position(i1))), i2);//return 加不加只要最终有返回都会返回 效果一样
		else
			ancestor(i1, index(getParent(position(i2))));//return
	}
};

/*
C++中heap以算法的形式提供，要使用这几个api需要#include <algorithm>

make_heap:		根据指定的迭代器区间以及一个可选的比较函数，来创建一个heap. O(N)
push_heap:		把指定区间的最后一个元素插入到heap中. O(logN)
pop_heap:		弹出heap顶元素, 将其放置于区间末尾. O(logN)
sort_heap：		堆排序算法，通常通过反复调用pop_heap来实现. N*O(logN)
C++11加入了两个新成员：
is_heap:		判断给定区间是否是一个heap. O(N)
is_heap_until:	找出区间中第一个不满足heap条件的位置. O(N)

本算法以面向对象式实现
若存放非基本类型时 需重载赋值号
*/
/*
堆 是最大(小)树, 完全二叉树
建堆O(N) 入堆O(logN) 出堆O(logN)
排序平均O(N*logN): N次出堆
*/
template<class T>
class Heap : public CompleteBinTree<T> {
	typedef T Element;
	typedef typename LinkedBinSearchTree<T>::Position Position;
	using BinTree<T>::root_;
	using CompleteBinTree<T>::baseArray;
	// 由于Heap中0号充当哨兵, 不计入总结点数, 故Heap中既是baseArray尾元素的下标, 又是结点总数
	using CompleteBinTree<T>::usedSize;

	using VirtualLinkedBinTree<T>::index;
	using VirtualLinkedBinTree<T>::nodeEraser;
	using CompleteBinTree<T>::getLeftChildSub;
	using CompleteBinTree<T>::getLeftChild;
	using CompleteBinTree<T>::getRightChildSub;
	using CompleteBinTree<T>::getRightChild;
	using CompleteBinTree<T>::getParentSub;
	using CompleteBinTree<T>::getParent;
	using CompleteBinTree<T>::getSibling;

	//使用less<T> greater<T> 或 比较符号重载 都有同样地问题: 两者没有同一个表示方法(带有重载()的父类)不便于大堆小堆的即时重构
	int(*cmper_)(const T &, const T &) = nullptr;
public:
	using CompleteBinTree<T>::full;
	using CompleteBinTree<T>::empty;

	/*
	(堆的大小, 哨兵, 小于比较方法, 任意初始化序列序列 该序列的元素个数) 
	最小堆 参数: moreCmper(大于)哨兵(最小值)	注意: 负数太小减法会变正:传入MAX_INT32/2即可
	最大堆 参数: lessCmper(小于)哨兵(最大值)
	注意: 用数组构造的堆与插入初始化的堆 的结构不一样
	*/
	Heap(int heapSize, T sentry, int(*cmper)(const T &, const T &), T *iniA = nullptr, int IniASize = 0) 
		: CompleteBinTree<T>(heapSize + 1) {
		build(iniA, IniASize, sentry, cmper);
	}
	virtual ~Heap()override {
		DE_PRINTF("Heap析构");
	}

	bool push(Element Item) {
		bool result = true;
		if (isNotInit()) {
			// 一般只可能是pop变空了, 重新使用之前的数据初始化即可
			initialize(baseArray[0].Data);
		}
		if (full()) {
			result = false;
			throw std::exception("堆已满 无法压入");
		}
		else {
			// @TODO 已提出的percolateUp函数是怎么回事
			// i指向[插入后]堆中最后一个元素的位置 i=[1, capbility)
			int i = ++usedSize;
			for (int parI = -1; cmper_(baseArray[(parI = getParentSub(i))].Data, Item) < 0; i = parI) {
				// 从最后一个有孩子的结点开始 向上过滤结点
				baseArray[i].Data = baseArray[parI].Data;
			}
			// 压入
			baseArray[i].Data = Item;
			// 因为新加入的元素始终在尾部 因此直接链接尾部的元素即可保证新添元素的链接
			linkToChildren(getParent(usedSize));
		}
		return result;
	}

	Element pop() {
		if (empty()) {
			throw std::exception("堆已空 无法抛出");
		}
		else {
			// 取出即将返回的值
			Element Item = root_->Data;
			// 从堆根开始下滤 删除尾元素(useedSize--即可)
			percolateDown(index(root_), usedSize--);
			// 解除已删除的尾元素与父结点的链接
			unlinkToParent(usedSize + 1);
			return Item;
		}
	}

	// 重新构建堆 复杂度O(N)
	void rebuild() {
		if (isNotInit()) {
			throw std::exception("未初始化");
		}
		/*
		对于一个已读入的数据但需要调整的最大或最小或最小堆(自下而上调整)
		从[最后一个子节点的父节点 即 倒数第一个有儿子的结点]开始
		取出该结点
		每次从左右结点中挑一个"大"的结点做[下滤]操作开始调整到根节点1
		*/
		for (int i = getParentSub(usedSize); i > 0; i--) {
			percolateDown(i, i);
		}
	}

	// 初始化
	void initialize(T sentry, int(*cmper)(const T &, const T &) = nullptr) {
		// 更改比较方法一定得更改哨兵 反之不一定
		baseArray[0].Data = sentry;
		if (cmper != nullptr) {
			cmper_ = cmper;
		}
		assert(cmper_ != nullptr);
		if (empty(root_)) {
			// root_不包括哨兵(0号) 内存空间会自动多申请一个
			root_ = baseArray + 1;
		}
	}

	// 构建堆
	void build(T *iniA, int IniASize, T sentry, int(*cmper)(const T &, const T &) = nullptr) {
		initialize(sentry, cmper);
		// 重设已使用结点数目(否则无法链接)
		usedSize = IniASize;
		// 链接root_
		linkToChildren(root_);
		// 为每个元素赋初始权值
		for (Position t = root_; t < root_ + IniASize; ++t) {
			// 若T类型不是基本类型 需要重载赋值号
			t->Data = iniA[t - root_];
			// 确保哨兵是列表中所有元素的极值(最小堆对应极小值, 最大堆对应极大值), 即lhs比rhs大或小恒成立, 成立则比较值>0,不成立则<0,相等则=0
			assert(cmper_(baseArray[0].Data, t->Data) > 0);
			// 只要没越界就链接->遍历时以usedSize为结束遍历的标志 而不是子结点是否为空?
			linkToChildren(t);
		}

		// 数据改变一定得重建堆
		rebuild();
	}

protected:
	
	//上滤 push调整
	void PercolateUp_DEL(int i, Element *baseArray, int usedSize) {
		// i指向堆中需上滤元素的位置
		Element Item = baseArray[i];
		for (; baseArray[i / 2] - Item > 0; i /= 2) {
			// 从上滤结点的父结点开始 向上过滤结点 若父结点大于子结点则继续
			baseArray[i] = baseArray[i / 2];
		}
		baseArray[i] = Item;
	}
	//下滤 :rebuild|pop调整(从末结点开始提升 等价于从最后一个父结点开始下滤)
	void PercolateDown_DEL(int Parent, Element *baseArray, int usedSize) {
		int Child;
		Element x = baseArray[Parent];//取出需要下滤的值
		for (; 2 * Parent <= usedSize; Parent = Child) {
			Child = 2 * Parent;//若左儿子==usedSize; 则右儿子不存在
			if (Child != usedSize && baseArray[Child] - baseArray[Child + 1] > 0)
				++Child;//选取左右儿子中小的一个
			if (x - baseArray[Child] < 0)//插入元素小于目标时终止 大于时继续
				break;
			else//将孩子上移<==>将temp下移
				baseArray[Parent] = baseArray[Child];
		}
		baseArray[Parent] = x;
	}

	// 下滤 将Heap中pdIndex处的元素提升至parentSub处 然后将以 baseArray + parentSub 为根的子堆调整为最大或最小或最小堆
	void percolateDown(int parentSub, int pdIndex) {
		// 取出需要下滤的值
		Element pdEle = baseArray[pdIndex].Data;
		for (int childSub = -1; (childSub = getLeftChildSub(parentSub)) <= usedSize; parentSub = childSub) {
			// 选取存在的左右儿子中大或小的一个
			if (childSub != usedSize && cmper_(baseArray[childSub].Data, baseArray[childSub + 1].Data) < 0)
				childSub++;

			if (cmper_(pdEle, baseArray[childSub].Data) >= 0)
				break;
			else {
				// 将孩子上移 <==> 将pdEle下移 由于childSub还要作为parentSub参与后续的比较因此不能移动
				baseArray[parentSub].Data = baseArray[childSub].Data;
			}
		}
		baseArray[parentSub].Data = std::move(pdEle);
	}
	// 返回是否已初始化
	bool isNotInit() {
		return root_ == nullptr;
	}
	//子结点链接:链接当前结点的孩子结点 链接关系只与位置有关 与数据无关 因此除非是实际使用的位置增减 否则不用重链
	void linkToChildren(Position parent) {
		int i = index(parent);
		parent->Left = getLeftChild(i);
		parent->Right = getRightChild(i);
	}
	//子结点分离:解除父亲结点对孩子结点的链接
	void unlinkToChildren(int parentIndex) {
		Position parent = getParent(parentIndex);
		if (!empty(parent)) {
			// 置空无效子结点
			int lSub = getLeftChildSub(parentIndex);
			if (lSub > usedSize) {
				parent->Left = nullptr;
			}
			else;
			int rSub = getRightChildSub(parentIndex);
			if (rSub > usedSize) {
				parent->Right = nullptr;
			}
			else;
		}
	}
	//子结点分离:解除父亲结点对孩子结点的链接
	void unlinkToParent(int childIndex) {
		Position parent = getParent(childIndex);
		if (!empty(parent)) {
			Sub parentIndex = index(parent);
			// 置空无效子结点
			int lSub = getLeftChildSub(parentIndex);
			if (lSub > usedSize) {
				parent->Left = nullptr;
			}
			else;
			int rSub = getRightChildSub(parentIndex);
			if (rSub > usedSize) {
				parent->Right = nullptr;
			}
			else;

			// 当子结点是root_的时候需要将root_置为空
			if (parent == baseArray) {
				root_ = nullptr;
			}
		}
	}
};

