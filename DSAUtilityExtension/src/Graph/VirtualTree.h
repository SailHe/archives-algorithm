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
public:
	using BinTree<T>::layers;
	using BinTree<T>::empty;
	using BinTree<T>::root_;
	typedef typename BinTree<T>::BT BTS;
	// 结构已知构造(结点数目, 数据获取方法, 下标获取方法, 空下标) 若结构并不总是完整的 但root_是特定已知的 可以指定root_的下标
	VirtualLinkedBinTree(int nSize, std::function<void(T *)> getData, std::function<void(Sub *, Sub *)> getSub, int noneSub, int rootSub = -1) {
		// 将这段数组当内存
		reCapacity(nSize);
		// 构建链接二叉树
		root_ = buildStructure(getData, getSub, noneSub);
		if (rootSub > 0) {
			_ASSERT_EXPR(rootSub < capacity, "指定根下标越界!");
			root_ = struA + rootSub;
		}
		usedSize = capacity - 1;
	}
	virtual ~VirtualLinkedBinTree() override {
		//free(struA);
		delete[] struA;
		// 保证基类的正常析构
		root_ = NULL;
		DE_PRINTF("VBT析构");
	}
	virtual void destroy(Position &r) override {
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
		}
		r = nullptr;
	}
	// 计算第layer层的叶结点个数 (layer越界返回-1) O(N)
	int leavesCount(int layer) const {
		// 级(level) 第layer层最多的结点个数: 2^layer layer=[0, height)
		assert(layer >= 0);
		if (layer > layers())
			return -1;
		int count = 0;
		//t, layer
		JCE::queue<JCE::pair<BTS, int>> q;
		q.push(std::make_pair(root_, 0));
		while (!q.empty()) {
			auto p = q.front();
			q.pop();
			if (!empty(p.first->Left))
				q.push({ p.first->Left, p.second + 1 });
			if (!empty(p.first->Right))
				q.push({ p.first->Right, p.second + 1 });
			if (p.first->isLeave() && p.second == layer)
				++count;
			else if (p.second > layer)
				break;
			//else continue;
		}
		return count;
	}
protected:
	typedef typename BinTree<T>::Position StructArray;
	using BinTree<T>::BTNode;
	using BinTree<T>::BT_NODE_LEN;
	typedef T ElementSBT;
	// Left和Right储存左右孩子位于数组内的地址
	StructArray struA = NULL;
	// 数组长度, 所有元素个数(nSize+1) 至少为1
	int capacity = 0;
	// 有效的元素个数 (理论上<==>capacity-1)
	// 由于Heap中0号充当哨兵, 不计入总结点数, 故Heap中既是struA尾元素的下标, 又是结点总数
	// 0号若正常使用, 含有效数据, 则意味着需要计入总结点数, 于是其仅仅已储存的或已使用的容量而不是下标
	using BinTree<T>::usedSize;

	// 构造静态完全二叉树 (结点个数) 向上转型时会用到的root_和usedSize 必须需要子类初始化
	VirtualLinkedBinTree(int nSize) {
		reCapacity(nSize);
	}
	//通过输入构建树结构 返回根结点 (数组内孩子结点的下标获取函数)
	Position buildStructure(std::function<void(T *)> getData, std::function<void(Sub *, Sub *)> getSub, int noneSub) {
		int nSize = capacity - 1;
		int i, sum = (nSize - 1)*nSize / 2;
		int leftSub, rightSub;
		Position rt = struA + 1;

		for (i = 0; i < nSize; i++) {
			Position leftChild = NULL, rightChild = NULL;
			getData(&rt[i].Data);
			getSub(&leftSub, &rightSub);
			if (leftSub != noneSub) {
				leftChild = rt + leftSub;
				sum -= leftSub;
			}

			if (rightSub != noneSub) {
				rightChild = rt + rightSub;
				sum -= rightSub;
			}
			rt[i].Left = leftChild, rt[i].Right = rightChild;
		}
		_ASSERT_EXPR(0 <= sum && sum < nSize, "给定数据有误!");
		return nSize == 0 ? NULL : rt + sum;
	}
	bool full() {
		return usedSize == capacity - 1;
	}
	// 构建结构体数组 (结点数目)
	void reCapacity(int nSize) {
		// 自动多申请一个用于存储0号哨兵
		++nSize;
		struA = new BTNode[nSize];
		/*
		struA = (StructArray)realloc(struA, BT_NODE_LEN*nSize);
		if (capacity == 0)//首次申请->全域初始化(排除已使用的usedSize 一般是0)
			memset(struA + usedSize, 0, BT_NODE_LEN*(nSize - usedSize));
		else//再次申请->root_域[struA+cap, struA+cap](排除原有的cap)初始化, struA+cap已经是数组末尾的后一个
			memset(struA + capacity, 0, BT_NODE_LEN*(nSize - capacity));
		*/
		capacity = nSize;
	}
	// 初始化root_ 
	void initRoot() {
		// root_不包括哨兵(0号)
		root_ = struA + 1;
	}
	Position getRoot_(){
		return root_;
	}
	// 返回数组内的结点编号 1号为root_ 0号为哨兵
	int index(BTS t) const {
		/*第一个元素插入时root_=NULL 所以链接左右孩子链接方法不能用root_ 更何况root_=1时的规律是针对数组成立的*/
		return t - struA;
	}
	// 返回数组内的结点位置
	Position position(int sub) {
		return struA + sub;
	}
};

// 完全二叉树
template<typename T>
class CompleteBinTree :public VirtualLinkedBinTree<T> {
public:
	virtual ~CompleteBinTree() {
		DE_PRINTF("CBT析构");
	}

protected:
	using VirtualLinkedBinTree<T>::struA;
	using VirtualLinkedBinTree<T>::usedSize;
	using VirtualLinkedBinTree<T>::capacity;
	using VirtualLinkedBinTree<T>::index;
	using Position = typename VirtualLinkedBinTree<T>::Position;

	// 用于Heap和VirtualLinkedBinSearchTree的初始化
	CompleteBinTree(int nSize) : VirtualLinkedBinTree<T>(nSize) {}

	Sub getLeftChildSub(int parentIndex) {
		return 2 * parentIndex;
	}
	Position getLeftChild(int parentIndex) {
		int sub = getLeftChildSub(parentIndex);
		return sub <= usedSize ? struA + sub : nullptr;
	}
	Sub getRightChildSub(int parentIndex) {
		return 2 * parentIndex + 1;
	}
	Position getRightChild(int parentIndex) {
		int sub = getRightChildSub(parentIndex);
		return sub <= usedSize ? struA + sub : nullptr;
	}
	// 返回父结点下标 1号是根结点没有父结点返回哨兵:0
	Sub getParentSub(int sonIndex) {
		return sonIndex / 2;
	}
	Position getParent(int sonIndex) {
		return struA + getParentSub(sonIndex);
	}
	Position getSibling(int SibIndex) {
		// 1号是根结点没有兄弟
		return SibIndex > 1 ?
			SibIndex % 2 == 0 ? struA + SibIndex + 1 : struA + SibIndex - 1
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
上一问题可以解决:树数据结构内部存在两种内存申请方法 一种是在传入的容器内存里申请 一种是在外部申请(有一个注册表用于注销这一部分内存 可以通过对注册表struA(指针数组)的访问达到即时随机访问)
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
	using VirtualLinkedBinTree<T>::struA;
	using VirtualLinkedBinTree<T>::usedSize;
	using VirtualLinkedBinTree<T>::capacity;
	using VirtualLinkedBinTree<T>::full;
	// 仅用于CompleteBinSearchTree的初始化
	VirtualLinkedBinSearchTree(int nSize) : VirtualLinkedBinTree<T>(nSize) {}

	Position nodeCreater(Element const &tData)override {
		if (full())
			return nullptr;
		++usedSize;
		struA[usedSize].Data = tData;
		/*
		isInsert = lastInsertPosition == NULL ? false : true;  bug
		BinTree<T>::lastInsertPosition = struA + usedSize + 1;
		*/
		return struA + usedSize;
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
	using VirtualLinkedBinTree<T>::struA;
	using VirtualLinkedBinTree<T>::usedSize;
	using VirtualLinkedBinTree<T>::capacity;
	using BinTree<T>::empty;
	using Tree::Min;

	/*返回StaBinSearchTree左子树的规模 (总结点数)*/
	int getLeftScaleL(double n) {
		/*2^h - 1 + x = n*/
		int h = (int)log2(n + 1);/*向下取整*/
		int x = (int)(n + 1 - pow(2.0, h));/*最下层单出的结点数*/
		return (int)pow(2.0, h - 1) - 1 + Min(x, (int)pow(2.0, h - 1));
	}

	//[完全二叉树]的序列构建  数据源数组(fitst, last) 目标数组struA		func(A, A+n, 0);
	Position buildComplete(T *first, T *last, int TRoot = 1) {
		int n = last - first;
		if (n == 0)
			return NULL;
		int nl = getLeftScaleL(n);
		struA[TRoot].Data = first[nl];/*first由0开始数nl个即是子树根*//*左子树从1开始编号下标是2i*/
		struA[TRoot].Left = buildComplete(first, first + nl, 2 * TRoot);
		struA[TRoot].Right = buildComplete(first + nl + 1, last, 2 * TRoot + 1);
		return struA + TRoot;
	}
	/*完全二叉树层序遍历(顺序遍历有序数组)*/
	template<class Fun>
	void levelTraversal(BT bT, Fun visit) {//无法override 但名字应该覆盖了
		if (bT == NULL)return;
		FOR(i, 0, usedSize) {//bT必须时root_不能是struA(root_ = struA+1)
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
		root_ = buildComplete(iniA, iniA + nSize);
		this->usedSize = nSize;
	}
	//直接把数组copy进静态数组里
	/*CompleteBinSearchTree(T *iniA, int nSize): VirtualLinkedBinSearchTree<T>(nSize){
		assert(iniA != nullptr && nSize > 0);
		root_ = struA + 1;
		for (int i = 0; i < nSize; ++i)
			root_[i].Data = iniA[i];
		usedSize = nSize;
	}*/
	virtual ~CompleteBinSearchTree() override {
		DE_PRINTF("CBT析构");
	}

	/*Position find(Element x){
		return NULL;//not impl
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

/*	make_heap:		根据指定的迭代器区间以及一个可选的比较函数，来创建一个heap. O(N)
push_heap:		把指定区间的最后一个元素插入到heap中. O(logN)
pop_heap:		弹出heap顶元素, 将其放置于区间末尾. O(logN)
sort_heap：		堆排序算法，通常通过反复调用pop_heap来实现. N*O(logN)
C++11加入了两个新成员：
is_heap:		判断给定区间是否是一个heap. O(N)
is_heap_until:	找出区间中第一个不满足heap条件的位置. O(N)*/
/*C++中heap以算法的形式提供，要使用这几个api需要#include <algorithm>*/
/*本算法以面向对象式实现
若存放非基本类型时 需重载赋值号
若存放指针时 需要自行销毁哨兵以及自己申请的内存 建议使用插入构造*/
/*堆 是最大(小)树, 完全二叉树*/
//如果仅仅是出堆而没有入堆的话 排个序O(N*logN) 此时入堆O(N) 出堆O(1)
template<class T>
class Heap : public CompleteBinTree<T> {
	typedef T Element;
	typedef typename LinkedBinSearchTree<T>::Position Position;
	using BinTree<T>::root_;
	using CompleteBinTree<T>::struA;
	using CompleteBinTree<T>::usedSize;
	using CompleteBinTree<T>::capacity;

	using VirtualLinkedBinTree<T>::index;
	using VirtualLinkedBinTree<T>::initRoot;
	using VirtualLinkedBinTree<T>::nodeEraser;
	using CompleteBinTree<T>::getLeftChildSub;
	using CompleteBinTree<T>::getLeftChild;
	using CompleteBinTree<T>::getRightChildSub;
	using CompleteBinTree<T>::getRightChild;
	using CompleteBinTree<T>::getParentSub;
	using CompleteBinTree<T>::getParent;
	using CompleteBinTree<T>::getSibling;

	//使用less<T> greater<T> 或 比较符号重载 都有同样地问题: 两者没有同一个表示方法(带有重载()的父类)不便于大堆小堆的即时重构
	int(*cmper_)(const T &min, const T &max) = nullptr;
public:
	using CompleteBinTree<T>::full;
	using CompleteBinTree<T>::empty;

	/*(堆的大小 任意初始化序列序列 该序列的元素个数)  注意: 用数组构造的堆与插入初始化的堆 的结构不一样*/
	Heap(int heapSize, T *iniA = nullptr, int IniASize = 0) : CompleteBinTree<T>(heapSize) {
		initRoot();
		// 先初始化已使用结点数目否则无法链接
		usedSize = IniASize;
		// 为每个元素赋初始权值
		for (Position t = root_; t < root_ + IniASize; ++t) {
			// 若T类型不是基本类型 需要重载赋值号
			t->Data = iniA[t - root_];
			// 只要没越界就链接->遍历时以usedSize为结束遍历的标志 而不是子结点是否为空?
			linkToChildren(t);
		}
	}
	virtual ~Heap()override {
		DE_PRINTF("Heap析构");
	}

	bool push(Element Item) {
		bool result = true;
		if (notBuild()) {
			result = false;
			throw std::exception("未构建");
		}
		if (full()) {
			result = false;
			throw std::exception("堆已满 无法压入");
		}
		else {
			// @TODO 已提出的percolateUp函数是怎么回事
			// i指向[插入后]堆中最后一个元素的位置 i=[1, capbility)
			int i = ++usedSize;
			for (int parI = -1; cmper_(struA[(parI = getParentSub(i))].Data, Item) < 0; i = parI) {
				// 从最后一个有孩子的结点开始 向上过滤结点
				struA[i].Data = struA[parI].Data;
			}
			// 压入
			struA[i].Data = Item;
			// 因为新加入的元素始终在尾部 因此直接链接尾部的元素即可保证新添元素的链接
			linkToChildren(getParent(usedSize));
		}
		return result;
	}

	Element pop() {
		if (empty()) {
			throw std::exception("堆已空 无法删除");
			return NULL;
		}
		if (notBuild()) {
			throw std::exception("未构建");
			return false;
		}
		// 取出即将返回的值
		Element Item = root_->Data;
		// 从堆根开始下滤 删除尾元素(useedSize--即可)
		PercoDown(index(root_), usedSize--);
		// 解除已删除的尾元素与父结点的链接
		unlinkToParent(usedSize + 1);
		return Item;
	}
	// 最小堆 参数: moreCmper(大于)哨兵(最小值)	注意: 负数太小减法会变正:传入MAX_INT32/2即可
	// 最大堆 参数: lessCmper(小于)哨兵(最大值)
	// 构建堆 复杂度O(N) (哨兵, 小于比较方法)
	void rebuild(T sentry, int(*cmper)(const T &, const T &)) {
		if (empty(root_)) {
			initRoot();
			// 链接root_
			linkToChildren(root_);
		}
		cmper_ = cmper;
		struA[0].Data = sentry;
		/*
		对于一个已读入的数据但需要调整的最大或最小或最小堆(自下而上调整)
		从[最后一个子节点的父节点 即 倒数第一个有儿子的结点]开始
		取出该结点
		每次从左右结点中挑一个"大"的结点做[下滤]操作开始调整到根节点1
		*/
		for (int i = getParentSub(usedSize); i > 0; i--) {
			PercoDown(i, i);
		}
	}
protected:
	//上滤 push调整
	void percolateUp(int i, Element *struA, int usedSize) {
		// i指向堆中需上滤元素的位置
		Element Item = struA[i];
		for (; struA[i / 2] - Item > 0; i /= 2) {
			// 从上滤结点的父结点开始 向上过滤结点 若父结点大于子结点则继续
			struA[i] = struA[i / 2];
		}
		struA[i] = Item;
	}
	//下滤 :rebuild|pop调整(从末结点开始提升 等价于从最后一个父结点开始下滤)
	void percolateDown(int Parent, Element *struA, int usedSize) {
		int Child;
		Element x = struA[Parent];//取出需要下滤的值
		for (; 2 * Parent <= usedSize; Parent = Child) {
			Child = 2 * Parent;//若左儿子==usedSize; 则右儿子不存在
			if (Child != usedSize && struA[Child] - struA[Child + 1] > 0)
				++Child;//选取左右儿子中小的一个
			if (x - struA[Child] < 0)//插入元素小于目标时终止 大于时继续
				break;
			else//将孩子上移<==>将temp下移
				struA[Parent] = struA[Child];
		}
		struA[Parent] = x;
	}

	/*下滤函数 将H中以H->Data[p]为根的子堆调整为最大或最小或最小堆*/
	void PercoDown(int parentSub, int p) {
		Element x = struA[p].Data;/*取出需要下滤的值*/
		for (int childSub = -1; (childSub = getLeftChildSub(parentSub)) <= usedSize; parentSub = childSub) {
			// 选取存在的左右儿子中大或小的一个
			if (childSub != usedSize && cmper_(struA[childSub].Data, struA[childSub + 1].Data) < 0)
				childSub++;

			if (cmper_(x, struA[childSub].Data) >= 0)
				break;
			else {
				// 将孩子上移 <= = > 将x下移 由于childSub还要作为parentSub参与后续的比较因此不能移动
				struA[parentSub].Data = struA[childSub].Data;
			}
		}
		struA[parentSub].Data = std::move(x);
	}
	//返回是否已经构建
	bool notBuild() {
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
			if (parent == struA) {
				root_ = nullptr;
			}
		}
	}
};

