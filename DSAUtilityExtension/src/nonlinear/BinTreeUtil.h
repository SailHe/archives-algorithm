#pragma once
#include "../stdafx.h"
#include "../DSAUEdll.h"

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

// ������Ч�ù��߼�
namespace BinTreeUtil {

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
		typedef typename BinTreeUtil::BinTreeNode<T> *Position;
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

	// [��ȫ]�������� (��ȫ: ����ȫ����������ȫ���ֶ���һ��)
	template<typename T>
	class CompleteNodeManager :public NodeManager<T> {

	public:
		typedef typename BinTreeUtil::BinTreeNode<T> *Position;
		typedef typename BinTreeUtil::BinTreeNode<T> const *Block;
		typedef typename Position NodeArray;
		using NodeManager<T>::nodeBlockCounter;
		CompleteNodeManager(int nSize) : capacity(nSize) {
			momoryPool = new BinTreeUtil::BinTreeNode<T>[nSize];
		}
		virtual ~CompleteNodeManager() override {
			delete[] momoryPool;
			momoryPool = nullptr;
		}

		Position nodeCreater(T const &tData) override {
			Position newNode = nullptr;
			if (full()) {
				// DNT
			}
			else {
				newNode = position(nodeBlockCounter);
				newNode->Data = tData;
				++nodeBlockCounter;
			}
			return newNode;
		}
		void nodeEraser(Position &del) override {
			if (del != nullptr) {
				// ��������ӹ�ϵ
				del->Left = del->Right = nullptr;
				// ��ɾ
				del = nullptr;
				--nodeBlockCounter;
			}
		}
		void clear() {
			nodeBlockCounter = 0;
		}
		int index(Block b) const {
			return b - momoryPool;
		}
		bool full() const {
			return nodeBlockCounter == capacity;
		}
		// ���������ڵĽ��λ��
		Position position(int sub) {
			assert(0 <= sub && sub < capacity);
			return momoryPool + sub;
		}
		Tree::TreeImplTypeEnum getTreeImplType() override {
			return Tree::LinearBlock;
		}
		
		BinTreeUtil::BinTreeNode<T> &operator[](int i) {
			assert(0 <= i && i < capacity);
			return momoryPool[i];
		}

	private:
		// '�ڴ�'��: Left��Right�������Һ���λ�ڳ��ڵĵ�ַ Varrays
		NodeArray momoryPool = nullptr;
		// '�ڴ�'������ | ���Ԫ�ظ��� | ������   ����Ϊ1
		int capacity = 0;
	};

	// �����Խ������� �������ڴ���û�й��ɵ�
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
			return new BinTreeUtil::BinTreeNode<T>(tData);
			/*
			bST = (BST)malloc(sizeof(struct TNode));
			memset(bST, 0, sizeof(struct TNode));
			bST->Element = x;
			*/
		}
		void nodeEraser(Position &del) override {
			// malloc��Ӧ��memeset��ʼ��free�ͷ� new�Զ���ʼ�� ��ֵ��ʼ�� delete�ͷ�
			// free(del); del = nullptr;
			delete del;
			del = nullptr;
			--nodeBlockCounter;
		}

	private:
	};

	// ���Խ������� ������û�й��� ��ʹ��������Ϊһ��'�����ڴ�' ����Ч��[����]�Ը� (����ϵ: ��CompleteNodeManagerͬ������������)
	// class LinearNodeManager
	// queue<Element*> freeBlock; // ���п�
	// Element *blockPool = nullptr; // ���

	// must know inorder

	/*������������������������������ (�� �� �� ����Ԫ�ظ���)*/
	void calcPostOrder(Tree::string const &sPre, Tree::string const &sMed, Tree::string &sPostBuffer);
	template<class Iterator>
	// ֧�ּӼ�����ĵ�����(��ָ��Ҳ�� postOder��Ӧ�����������������n��Ԫ��)
	void calcPostOrder(Iterator preOrder, Iterator inOrder, Iterator postOder, int n) {
		int Ln;/*������������*/
		if (n == 0)return;
		postOder[n - 1] = *preOrder;/*ת����ϵ:��������������Ԫ�ؾ���������*/
		for (Ln = 0; Ln < n && inOrder[Ln] != *preOrder; ++Ln);
		calcPostOrder(preOrder + 1, inOrder, postOder, Ln);/*�����������������������һ��Ԫ�� �������鱣�ֲ���*/
		calcPostOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*������������һ��Ԫ��*/
	}
	template<class Element>
	void calcPostOrder(Element *preOrder, Element *inOrder, Element *postOder, int n) {
		int Ln;/*������������*/
		if (n == 0)return;
		postOder[n - 1] = *preOrder;/*ת����ϵ:��������������Ԫ�ؾ���������*/
		for (Ln = 0; Ln < n && inOrder[Ln] != *preOrder; ++Ln);
		calcPostOrder(preOrder + 1, inOrder, postOder, Ln);/*�����������������������һ��Ԫ�� �������鱣�ֲ���*/
		calcPostOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*������������һ��Ԫ��*/
	}

	/*���ݺ������������������������� ֻ�޸��м����仰����*/
	template<class Iterator>
	void calcPrefOrder(Iterator preOrder, Iterator inOrder, Iterator postOder, int n) {
		int Ln;/*����������*/
		if (n == 0)return;
		*preOrder = postOder[n - 1];/*ת����ϵ:������������βԪ�ؾ���������*/
		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; ++Ln);//��ȡ����������
		calcPrefOrder(preOrder + 1, inOrder, postOder, Ln);/*��������������������*/
		calcPrefOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*����������*/
	}
	template<class Element>
	void calcPrefOrder(Element *preOrder, Element *inOrder, Element *postOder, int n) {
		int Ln;/*����������*/
		if (n == 0)return;
		*preOrder = postOder[n - 1];/*ת����ϵ:������������βԪ�ؾ���������*/
		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; Ln++);//��ȡ����������
		calcPrefOrder(preOrder + 1, inOrder, postOder, Ln);/*��������������������*/
		calcPrefOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*����������*/
	}

	/*���ݺ������������������������� ����->��->������->�������*/
	template<class Element>
	void calcLeveOrder(Element *preOrder, Element *inOrder, Element *postOder, int n) {
		_ASSERT_EXPR(false, "has not impl");
		exit(-1);
	}
}
