#pragma once
#include "TreeObject.h"

// ��ȫ������ (ʵ������ȫ���������ڴ����: ����[]���� + �������������β��)
template<typename T>
class CompleteBinTree :public BinTree<T> {

	using CBT = typename BinTree<T>::BT;
	using Position = typename BinTree<T>::Position;
	using BinTree<T>::BTNode;
	using BinTree<T>::empty;

	using BinTree<T>::root_;
	//using BinTree<T>::nodeManager_;

public:

	// ֱ�Ӱ�����copy����̬������
	// CompleteBinTree(T *initArr, int nSize) : CompleteBinTree(nSize){
	// 	assert(initArr != nullptr);
	// 	root_ = baseNodeArray;
	// 	for (int i = 0; i < nSize; ++i)
	// 		root_[i].Data = initArr[i];
	// 	initEnd(nSize);
	// }

	// sort(initArr, initArr + n, less<T>());
	// ��ת����:sort(initArr, initArr + n, greater<T>()); ��ȫ�������������С����֧����С�Ҵ����ľ���ת ��ͨBinTree֧��
	// ʹ�����鹹��Ψһ����ȫ������ tips::���������� ��Ϊ������
	CompleteBinTree(int nSize, T *initArr) : CompleteBinTree(nSize) {
		assert(initArr != nullptr);
		// 0��ʾ������Sub
		root_ = buildCompleteTree(initArr, initArr + nSize, 0);
	}
	virtual ~CompleteBinTree() {
		delete[] baseNodeArray;
		baseNodeArray = nullptr;
		DE_PRINTF("CBT����");
	}

	int size() override {
		return validNodeNum;
	}

	// ����StaBinSearchTree�������Ĺ�ģ (�ܽ����)
	static int getLeftScaleL(double n) {
		// 2^h - 1 + x = n
		// ����ȡ��
		int h = (int)log2(n + 1);
		// ���²㵥���Ľ����
		int x = (int)(n + 1 - pow(2.0, h));
		return (int)pow(2.0, h - 1) - 1 + Tree::Min(x, (int)pow(2.0, h - 1));
	}

	// [��ȫ������]�����й���  (����Դ ������±�) ���ظ����
	Position buildCompleteTree(T *first, T *last, int rootSub) {
		int n = last - first;
		if (n == 0)
			return nullptr;
		int nl = getLeftScaleL(n);
		baseNodeArray[rootSub].Data = first[nl];
		// first��0��ʼ��nl������������ (root_��baseNodeArray��subΪ0 ������sub = 2*rootSub + 1)
		baseNodeArray[rootSub].Left = buildCompleteTree(first, first + nl, 2 * rootSub + 1);
		baseNodeArray[rootSub].Right = buildCompleteTree(first + nl + 1, last, 2 * rootSub + 2);
		return position(rootSub);
	}
	// ��ȫ�������������(˳�������������)
	void levelTraversal(CBT bT, std::function<void(CBT)> visit) {
		//�޷�override ������Ӧ�ø�����
		if (bT == nullptr)
			return;
		FOR(i, 0, validNodeNum) {
			//bT������root_������baseNodeArray(root_ = baseNodeArray+1)
			if (!visit(bT + i))
				return;//��ֹ
		}
	}
	
	//�����������Ĺ������� Խ�緵���ڱ�(0�Ž��)
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
	// ��ʼ������
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
	// ���������ڵĽ���� 1��Ϊroot_ 0��Ϊ�ڱ�
	int index(CBT t) const {
		// ��һ��Ԫ�ز���ʱroot_=nullptr �����������Һ������ӷ���������root_ ���ο�root_=1ʱ�Ĺ�����������������
		return t - baseNodeArray;
	}
	// ���������ڵĽ��λ��
	Position position(int sub) {
		assert(0 <= sub && sub < capacity);
		return baseNodeArray + sub;
	}
	
	// ���ڴ治��������
	// BinTreeAlgorithm::LinearNodeManager<T> &baseNodeArray;
	BTNode<T> *baseNodeArray = nullptr;
	// ����Heap��0�ų䵱�ڱ�, �������ܽ����, ��Heap�м���baseNodeArrayβԪ�ص��±�, ���ǽ������
	// ֻ��Heap�е����ֵ�ĺ�����������в�� �����Ų������������
	int validNodeNum = 0;
	int capacity = 0;
private:
};

/*
�� �����(С)��, ��ȫ������
����O(N) ���O(logN) ����O(logN)
����ƽ��O(N*logN): N�γ���
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

	// (�ѵĴ�С, �ڱ�, С�ڱȽϷ���, �����ʼ���������� �����е�Ԫ�ظ���) 
	// ��С�� ����: moreCmper(����)�ڱ�(��Сֵ)	ע��: ����̫С���������:����MAX_INT32/2����
	// ���� ����: lessCmper(С��)�ڱ�(���ֵ)
	// ע��: �����鹹��Ķ�������ʼ���Ķ� �Ľṹ��һ��
	Heap(int heapSize, T sentry, int(*cmper)(const T &, const T &), T *initArr = nullptr, int initArrSize = 0)
		: CompleteBinTree<T>(heapSize + 1) {
		build(initArr, initArrSize, sentry, cmper);
	}
	virtual ~Heap() override {
		DE_PRINTF("Heap����");
	}

	bool push(Element const &item) {
		bool result = true;
		if (isNotInit()) {
			// һ��ֻ������pop�����, ����ʹ��֮ǰ�����ݳ�ʼ������
			initialize(baseNodeArray[0].Data);
		}
		if (full()) {
			result = false;
			throw std::exception("������ �޷�ѹ��");
		}
		else {
			percolateUp(item);
			// ��Ϊ�¼����Ԫ��ʼ����β�� ���ֱ������β����Ԫ�ؼ��ɱ�֤����Ԫ�ص�����
			linkToParent(validNodeNum + 1);
		}
		return result;
	}

	Element pop() {
		if (empty()) {
			throw std::exception("���ѿ� �޷��׳�");
		}
		else {
			// ȡ���������ص�ֵ
			Element item = root_->Data;
			int delSub = validNodeNum;
			// �����ɾ����βԪ���븸��������
			unlinkToParent(delSub);
			// �ӶѸ���ʼ���� ɾ��βԪ��(validNodeNum--����)
			percolateDown(1, delSub);
			return item;
		}
	}

	// ���¹����� ���Ӷ�O(N)
	void rebuild() {
		if (isNotInit()) {
			throw std::exception("δ��ʼ��");
		}
		// ����һ���Ѷ�������ݵ���Ҫ������������С����С��(���¶��ϵ���)
		// ��[���һ���ӽڵ�ĸ��ڵ� �� ������һ���ж��ӵĽ��]��ʼ
		// ȡ���ý���ÿ�δ����ҽ������һ��"��"�Ľ����[����]������ʼ���������ڵ�1
		for (int i = calcParentSubPosition(validNodeNum); i > 0; i--) {
			percolateDown(i, i);
		}
	}

	// ��ʼ��
	void initialize(T sentry, int(*cmper)(const T &, const T &) = nullptr) {
		// ���ıȽϷ���һ���ø����ڱ� ��֮��һ��
		baseNodeArray[0].Data = sentry;
		if (cmper != nullptr) {
			cmperFun = cmper;
		}
		assert(cmperFun != nullptr);
		if (empty(root_)) {
			// root_�������ڱ�(0��) �ڴ�ռ���Զ�������һ��
			root_ = position(1);
			validNodeNum = 0;
		}
	}

	// ������
	void build(T *initArr, int initArrSize, T sentry, int(*cmper)(const T &, const T &) = nullptr) {
		initialize(sentry, cmper);
		for (int i = 0; i < initArrSize; ++i) {
			// ȷ���ڱ����б�������Ԫ�صļ�ֵ(��С�Ѷ�Ӧ��Сֵ, ���Ѷ�Ӧ����ֵ), ��lhs��rhs���С�����, ������Ƚ�ֵ>0,��������<0,�����=0
			assert(cmperFun(baseNodeArray[0].Data, *initArr) > 0);
			baseNodeArray[1 + i].Data = *initArr;
			linkToParent(1 + i);
			++initArr;
		}

		// ���ݸı�һ�����ؽ���
		rebuild();
	}

protected:
	// ���� ��β��Ԫ��item�븸�������Ƚ�, �����ں��ʵ�λ��; push����
	void percolateUp(Element const &item) {
		// puSubָ��[�����]�������һ��Ԫ�ص�λ�� puSub=[1, capbility)
		int puSub = validNodeNum + 1;
		for (int parentSub = -1; cmperFun(baseNodeArray[(parentSub = calcParentSubPosition(puSub))].Data, item) < 0; puSub = parentSub) {
			// �����һ���к��ӵĽ�㿪ʼ ���Ϲ��˽��
			baseNodeArray[puSub].Data = baseNodeArray[parentSub].Data;
		}
		// ѹ��
		baseNodeArray[puSub].Data = item;
	}

	// ���� ��Heap��pdSub����Ԫ��������parentSub�� Ȼ���� position(parentSub) Ϊ�����Ӷѵ���Ϊ������С����С��; build|pop����
	void percolateDown(int parentSub, int pdSub) {
		// ȡ����Ҫ���˵�ֵ
		Element pdEle = baseNodeArray[pdSub].Data;
		for (int childSub = -1; (childSub = calcLeftChildSub(parentSub)) <= validNodeNum; parentSub = childSub) {
			// ѡȡ���ڵ����Ҷ����д��С��һ��
			if (childSub != validNodeNum && cmperFun(baseNodeArray[childSub].Data, baseNodeArray[childSub + 1].Data) < 0)
				childSub++;

			if (cmperFun(pdEle, baseNodeArray[childSub].Data) >= 0)
				break;
			else {
				// ���������� <==> ��pdEle���� ����childSub��Ҫ��ΪparentSub��������ıȽ���˲����ƶ�
				baseNodeArray[parentSub].Data = baseNodeArray[childSub].Data;
			}
		}
		baseNodeArray[parentSub].Data = std::move(pdEle);
	}

	// �����Ƿ��ѳ�ʼ��
	bool isNotInit() {
		return root_ == nullptr;
	}

	// =>���ӹ�ϵֻ��λ���й� �������޹� ��˳�����ʵ��ʹ�õ�λ������ ����������

	// ��ָ���ӽ����丸���佨������
	void linkToParent(int childIndex) {
		int parentIndex = calcParentSubPosition(childIndex);
		Position parent = position(parentIndex);
		if (!empty(parent)) {
			int lSub = calcLeftChildSub(parentIndex);
			int rSub = calcRightChildSub(parentIndex);
			if (lSub == childIndex) {
				// ������ɹ� �����֮ǰʹ������ʵ�ֵ�Heap������ת��Ϊ����ʵ����
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
	// ���ָ���ӽ����丸���������
	void unlinkToParent(int childIndex) {
		Position parent = calcParentPosition(childIndex);
		if (!empty(parent)) {
			Sub parentIndex = index(parent);
			// �ÿ���Ч�ӽ��
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

			// ���ӽ����root_��ʱ����Ҫ��root_��Ϊ��
			if (parent == position(0)) {
				root_ = nullptr;
			}
		}
	}

	// =>0�����ڱ�; 1���Ǹ����

	// �����������±�
	Sub calcLeftChildSub(int parentIndex) {
		return 2 * parentIndex;
	}
	Position calcLeftChildPosition(int parentIndex) {
		int sub = calcLeftChildSub(parentIndex);
		return 0 < sub && sub <= validNodeNum ? position(sub) : nullptr;
	}
	// �����������±�
	Sub calcRightChildSub(int parentIndex) {
		return 2 * parentIndex + 1;
	}
	Position getRightChild(int parentIndex) {
		int sub = calcRightChildSub(parentIndex);
		return 0 < sub && sub <= validNodeNum ? position(sub) : nullptr;
	}
	// ���ظ�����±� root_�ĸ�����Ӧ�ڱ�
	Sub calcParentSubPosition(int sonIndex) {
		return sonIndex / 2;
	}
	Position calcParentPosition(int sonIndex) {
		return position(calcParentSubPosition(sonIndex));
	}
	// �����ֵܽ��
	Position calcSiblingPosition(int sibIndex) {
		// 1���Ǹ����û���ֵ�
		return sibIndex > 1 ?
			sibIndex % 2 == 0 ? position(sibIndex + 1) : position(sibIndex - 1)
			: nullptr;
	}

private:
	// ʹ��less<T> greater<T> �� �ȽϷ������� ����ͬ��������: ����û��ͬһ����ʾ����(��������()�ĸ���)�����ڴ��С�ѵļ�ʱ�ع�
	int(*cmperFun)(const T &, const T &) = nullptr;
};
