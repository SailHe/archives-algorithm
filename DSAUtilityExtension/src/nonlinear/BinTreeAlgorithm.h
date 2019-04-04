#pragma once

namespace Tree {
	// 遍历的顺序类型枚举
	enum TraversalOrderEnum {
		ORDER_PREFIX_ROOT, // 先根序
		ORDER_INFIX_ROOT, // 中根序
		ORDER_POST_ROOT, // 后根序
		ORDER_LEVEL, // 层序
	};
	enum TreeImplTypeEnum {
		// 线性内存块 Virtual Linked
		LinearBlock,
		// 非线性内存块 Real Linked
		NonlinearBlock,
	};
	template<typename T>
	int Min(T a, T b) {
		return a < b ? a : b;
	}
	template<typename T>
	int Max(T a, T b) {
		return a > b ? a : b;
	}
	//数组表
	template<typename T> using ArrayList = std::vector<T>;
	template<typename T> using stack = std::stack<T>;
	template<typename T> using queue = std::queue<T>;
	template<typename T> using priority_queue = std::priority_queue<T>;
	template<typename T> using greater = std::greater<T>;
	using string = std::string;
	//template<typename T> using vector = StandardExtend::ArrayList<T>;
}

// 二度树效用算法集
namespace BinTreeAlgorithm {

	// 二叉树结点
	template<typename T>
	class BinTreeNode {
	public:
		typedef T Element;
		BinTreeNode() {}
		// 获取子类height字段
		virtual int getHeight() {
			// return depthOf(this);
			return -1;
		}
		// 设置子类height字段
		virtual void setHeight(int) {}
		// 获取子类的添加字段:weight (曾考虑过在这里添加字段 并重载比较符号 但极容易引起误会以及存在其它很多劣势故作罢)
		virtual int getValue() {
			return 0;
		}
		// 设置子类添加字段
		virtual void setValue(int) {}

		// 这样无需默认构造函数
		BinTreeNode(Element const&tData) : Data(tData) {}
		// 拷贝构造 仅拷贝数据 子结点置nullptr
		BinTreeNode(BinTreeNode &rhs) {
			(*this) = rhs;
			Left = Right = nullptr;
		}
		virtual ~BinTreeNode() {
			DE_PRINTF("BinTreeNode析构");
		}
		// 赋值 默认的全域赋值
		BinTreeNode &operator=(const BinTreeNode&) = default;

		int height() {
			// return depthOf(this);
			return -1;
		}
		// 叶子结点判断
		bool isLeave() const {
			return Left == nullptr && Right == nullptr;
		}

		// 指向左子树
		BinTreeNode* Left = nullptr;
		// 指向右子树
		BinTreeNode* Right = nullptr;
		// 表示是否visit过此结点
		bool v = false;
		// 结点数据:若为结构体,结构体提供比较方法key(增删查都会使用的关键字)在结构体内 否则key就是Data weight-height不能作为key(不唯一)
		Element Data;

	};



	// 结点管理类
	// (简单粗暴的实现的话可以直接判断语句判断 但这样会在类内部再次添加至少1个域: cap, 而且并非所有子类都会使用 因此采用继承方式)
	template<typename T>
	class NodeManager {
	public:
		typedef typename BinTreeAlgorithm::BinTreeNode<T> *Position;
		// 结点生成器 返回一个未使用的结点 若不存在未使用结点 返回nullptr 只能插入使用
		virtual Position nodeCreater(T const &tData) = 0;
		// 结点擦除器 将结点置为未使用状态
		virtual void nodeEraser(Position &del) = 0;
		// 返回现存有效的结点数
		int validNodeCnt() {
			return validNodeCnter;
		}
		NodeManager() {}
		virtual Tree::TreeImplTypeEnum getTreeImplType() = 0;
		virtual ~NodeManager() {}
		int validNodeCnter = 0;// 有效的元素个数
	private:
	};
	template<typename T>
	class LinearNodeManager :public NodeManager<T> {
	public:
		typedef typename NodeManager<T>::Position Position;
		typedef typename NodeManager<T>::Position NodeArray;
		using NodeManager<T>::validNodeCnter;
		LinearNodeManager(int nSize) : capacity(nSize) {
			baseArray = new BinTreeAlgorithm::BinTreeNode<T>[nSize];
		}
		LinearNodeManager(NodeArray baseArray, int validNodeCnt, int nSize) :
			baseArray(baseArray), capacity(nSize) {
			validNodeCnter = validNodeCnt;
		}
		Tree::TreeImplTypeEnum getTreeImplType() override {
			return Tree::LinearBlock;
		}
		virtual ~LinearNodeManager() override {
			delete[] baseArray;
			baseArray = nullptr;
		}
		bool full() {
			return validNodeCnter == capacity;
		}
		Position nodeCreater(T const &tData) override {
			Position newNode = nullptr;
			if (full()) {
				// DNT
			}
			else {
				baseArray[validNodeCnter].Data = tData;
				newNode = baseArray + (validNodeCnter++);
			}
			return newNode;
		}
		void nodeEraser(Position &del) override {
			// 0代表初始状态 只为了标识用 并无特殊用处
			// malloc就应用memeset初始化free释放 new自动初始化 赋值初始化 delete释放
			// del->Data = 0; // {}
			//静态数组的删除并非实际删除
			del->Left = del->Right = nullptr;
			del = nullptr;
			--validNodeCnter;
		}
		// 通过输入构建树结构 返回根结点 (数组内孩子结点的下标获取函数)
		Position buildBinTreeStructure(std::function<void(T *)> getData, std::function<void(Sub *, Sub *)> getSub, int noneSub, int customRootSub) {
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
			if (customRootSub > 0) {
				_ASSERT_EXPR(customRootSub < capacity, "指定根下标越界!");
				validNodeCnter = capacity - 1;
				return baseArray + customRootSub;
			}
			else {
				return nSize == 0 ? nullptr : position(sum);
			}
		}
		// 返回数组内的结点位置
		Position position(int sub) {
			assert(0 <= sub && sub < capacity);
			return baseArray + sub;
		}
		// Left和Right储存左右孩子位于数组内的地址 Varrays
		NodeArray baseArray = nullptr;
		// 数组容量, 最多元素(nSize 结点个数)个数至少为1
		int capacity = 0;
		//private:
	};
	template<typename T>
	class NonLinearNodeManager :public NodeManager<T> {
	public:
		typedef typename NodeManager<T>::Position Position;
		using NodeManager<T>::validNodeCnter;

		NonLinearNodeManager() {}
		Tree::TreeImplTypeEnum getTreeImplType() override {
			return Tree::NonlinearBlock;
		}
		// virtual ~NonLinearNodeManager() override {}
		Position nodeCreater(T const &tData) override {
			++validNodeCnter;
			return new BinTreeAlgorithm::BinTreeNode<T>(tData);
			/*
			bST = (BST)malloc(sizeof(struct TNode));
			memset(bST, 0, sizeof(struct TNode));
			bST->Element = x;
			*/
		}
		void nodeEraser(Position &del) override {
			// free(del); del = nullptr;
			delete del;
			del = nullptr;
			--validNodeCnter;
		}
		//private:
	};

}
