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
		// �����Ѵ����Ľ����
		int createdNodeNum() {
			return nodeBlockCounter;
		}
		NodeManager() {}
		virtual Tree::TreeImplTypeEnum getTreeImplType() = 0;
		virtual ~NodeManager() {}

	protected:
		// ���������
		int nodeBlockCounter = 0;
	};
	template<typename T>
	class LinearNodeManager :public NodeManager<T> {

	public:
		typedef typename BinTreeAlgorithm::BinTreeNode<T> *Position;
		typedef typename BinTreeAlgorithm::BinTreeNode<T> const *Block;
		typedef typename Position NodeArray;
		using NodeManager<T>::nodeBlockCounter;
		LinearNodeManager(int nSize) : capacity(nSize) {
			momoryPool = new BinTreeAlgorithm::BinTreeNode<T>[nSize];
		}
		virtual ~LinearNodeManager() override {
			delete[] momoryPool;
			momoryPool = nullptr;
		}

		Position nodeCreater(T const &tData) override {
			Position newNode = nullptr;
			if (full()) {
				// DNT
			}
			else {
				momoryPool[nodeBlockCounter].Data = tData;
				newNode = momoryPool + (nodeBlockCounter++);
			}
			return newNode;
		}
		void nodeEraser(Position &del) override {
			// 0�����ʼ״̬ ֻΪ�˱�ʶ�� ���������ô�
			// malloc��Ӧ��memeset��ʼ��free�ͷ� new�Զ���ʼ�� ��ֵ��ʼ�� delete�ͷ�
			// del->Data = 0; // {}
			//��̬�����ɾ������ʵ��ɾ��
			if (del != nullptr) {
				del->Left = del->Right = nullptr;
				del = nullptr;
				--nodeBlockCounter;
			}
		}

		Tree::TreeImplTypeEnum getTreeImplType() override {
			return Tree::LinearBlock;
		}
		
		bool full() const {
			return nodeBlockCounter == capacity;
		}
		// �����ڴ���ڵĽ����
		int index(Block block) const {
			return block - momoryPool;
		}
		// ���������ڵĽ��λ��
		Position position(int sub) {
			assert(0 <= sub && sub < capacity);
			return momoryPool + sub;
		}

		BinTreeAlgorithm::BinTreeNode<T> &operator[](int i) {
			assert(0 <= i && i < capacity);
			return momoryPool[i];
		}

	private:
		// �ڴ��: Left��Right�������Һ���λ�ڳ��ڵĵ�ַ Varrays
		NodeArray momoryPool = nullptr;
		// �ڴ������|���Ԫ�ظ���|������, ����Ϊ1
		int capacity = 0;
		// queue<Element*> freeMem;//�����ڴ�
		// Element *memoryBlock = nullptr;//�ڴ�� �ɽ��������ľֲ����������� ��������ʹ���ⲿ������ڴ�
	};
	template<typename T>
	class NonLinearNodeManager :public NodeManager<T> {

	public:
		typedef typename NodeManager<T>::Position Position;
		using NodeManager<T>::nodeBlockCounter;

		NonLinearNodeManager() {}
		Tree::TreeImplTypeEnum getTreeImplType() override {
			return Tree::NonlinearBlock;
		}
		// virtual ~NonLinearNodeManager() override {}
		Position nodeCreater(T const &tData) override {
			++nodeBlockCounter;
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
			--nodeBlockCounter;
		}

	private:
	};

}
