#pragma once

namespace Tree {
	// ������˳������ö��
	enum TraversalOrderEnum {
		ORDER_PREFIX_ROOT, // �ȸ���
		ORDER_INFIX_ROOT, // �и���
		ORDER_POST_ROOT, // �����
		ORDER_LEVEL, // ����
	};
	enum TreeImplTypeEnum {
		// �����ڴ�� Virtual Linked
		LinearBlock,
		// �������ڴ�� Real Linked
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
	//�����
	template<typename T> using ArrayList = std::vector<T>;
	template<typename T> using stack = std::stack<T>;
	template<typename T> using queue = std::queue<T>;
	template<typename T> using priority_queue = std::priority_queue<T>;
	template<typename T> using greater = std::greater<T>;
	using string = std::string;
	//template<typename T> using vector = StandardExtend::ArrayList<T>;
}

// ������Ч���㷨��
namespace BinTreeAlgorithm {

	// ���������
	template<typename T>
	class BinTreeNode {
	public:
		typedef T Element;
		BinTreeNode() {}
		// ��ȡ����height�ֶ�
		virtual int getHeight() {
			// return depthOf(this);
			return -1;
		}
		// ��������height�ֶ�
		virtual void setHeight(int) {}
		// ��ȡ���������ֶ�:weight (�����ǹ�����������ֶ� �����رȽϷ��� ����������������Լ����������ܶ����ƹ�����)
		virtual int getValue() {
			return 0;
		}
		// ������������ֶ�
		virtual void setValue(int) {}

		// ��������Ĭ�Ϲ��캯��
		BinTreeNode(Element const&tData) : Data(tData) {}
		// �������� ���������� �ӽ����nullptr
		BinTreeNode(BinTreeNode &rhs) {
			(*this) = rhs;
			Left = Right = nullptr;
		}
		virtual ~BinTreeNode() {
			DE_PRINTF("BinTreeNode����");
		}
		// ��ֵ Ĭ�ϵ�ȫ��ֵ
		BinTreeNode &operator=(const BinTreeNode&) = default;

		int height() {
			// return depthOf(this);
			return -1;
		}
		// Ҷ�ӽ���ж�
		bool isLeave() const {
			return Left == nullptr && Right == nullptr;
		}

		// ָ��������
		BinTreeNode* Left = nullptr;
		// ָ��������
		BinTreeNode* Right = nullptr;
		// ��ʾ�Ƿ�visit���˽��
		bool v = false;
		// �������:��Ϊ�ṹ��,�ṹ���ṩ�ȽϷ���key(��ɾ�鶼��ʹ�õĹؼ���)�ڽṹ���� ����key����Data weight-height������Ϊkey(��Ψһ)
		Element Data;

	};



	// ��������
	// (�򵥴ֱ���ʵ�ֵĻ�����ֱ���ж�����ж� �������������ڲ��ٴ��������1����: cap, ���Ҳ����������඼��ʹ�� ��˲��ü̳з�ʽ)
	template<typename T>
	class NodeManager {
	public:
		typedef typename BinTreeAlgorithm::BinTreeNode<T> *Position;
		// ��������� ����һ��δʹ�õĽ�� ��������δʹ�ý�� ����nullptr ֻ�ܲ���ʹ��
		virtual Position nodeCreater(T const &tData) = 0;
		// �������� �������Ϊδʹ��״̬
		virtual void nodeEraser(Position &del) = 0;
		// �����ִ���Ч�Ľ����
		int validNodeCnt() {
			return validNodeCnter;
		}
		NodeManager() {}
		virtual Tree::TreeImplTypeEnum getTreeImplType() = 0;
		virtual ~NodeManager() {}
		int validNodeCnter = 0;// ��Ч��Ԫ�ظ���
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
			// 0�����ʼ״̬ ֻΪ�˱�ʶ�� ���������ô�
			// malloc��Ӧ��memeset��ʼ��free�ͷ� new�Զ���ʼ�� ��ֵ��ʼ�� delete�ͷ�
			// del->Data = 0; // {}
			//��̬�����ɾ������ʵ��ɾ��
			del->Left = del->Right = nullptr;
			del = nullptr;
			--validNodeCnter;
		}
		// ͨ�����빹�����ṹ ���ظ���� (�����ں��ӽ����±��ȡ����)
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
			_ASSERT_EXPR(0 <= sum && sum < nSize, "������������!");
			if (customRootSub > 0) {
				_ASSERT_EXPR(customRootSub < capacity, "ָ�����±�Խ��!");
				validNodeCnter = capacity - 1;
				return baseArray + customRootSub;
			}
			else {
				return nSize == 0 ? nullptr : position(sum);
			}
		}
		// ���������ڵĽ��λ��
		Position position(int sub) {
			assert(0 <= sub && sub < capacity);
			return baseArray + sub;
		}
		// Left��Right�������Һ���λ�������ڵĵ�ַ Varrays
		NodeArray baseArray = nullptr;
		// ��������, ���Ԫ��(nSize ������)��������Ϊ1
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
