#pragma once
#include "TreeObject.h"



/*
��̬����:��������Ϊ�ڴ��, �ڴ��ڴ����"����"���ʵ�ֶ������Ļ� ������ͨ������ʵ������ ���Ҹ�Ϊ���� ���迼��
˳�򴢴�:�����鰴˳����,1����Ϊ�����,֮�� ���Ϊ2n�����ӽ�� 2n+1�����ӽ�� ���������ȫ�������洢��ʽ
.����,���е�������������ʽ����,���Լ�ڴ�,��ȱ�����������
.���¼���������Կ��Ƿ���ʽ��������ṹ
��֪��(�������ȫ������)�Ľṹ(����Ŵ����ӽ��)	:��̬����(Ψһ����֮��)
��֪����ȫ������								:˳�򴢴�
.��֪���ݲ�ѯ(PS:�߶�Height = max(Hl, Hr) + 1)
��ͨ���鴢�� �������ݦ�(N)
��ʹ�����ݽṹ:			����O(N*logN)				binary_search O(logN))			��ɾ O(N) = ˳��ȶԲ��ƶ�
˳�򴢴����ȫ����������	����				���즨(N)		find		  O(logN)			��ɾ O(H) = ͬBST�� H(CBT) = log(N)+1 ����ɾO(logN)
AVLƽ����	���빹��O(N*logN) ������CBST�������� ��ά���ṹ��Ҫ����ʱ��
BST			���빹��O(N*logN) ��ɾO(H) log(N-1) <= H(BST) <= N, ƽ�� H = log(N)+1
ǰ�߹��췽��,����Ч��������൱, ������ֻ�Ǹ���Ҷ�ֲ�����, ά���ɱ�ԶС��ǰ��
.ʵ�ֶ�(CBT ���С��)
��ͨ����:����ά��, ������
��ʽBSTʵ�ֵ�Heap:���� ��(N),		push O(H)	pop O(H)
˳��CBTʵ�ֵ�Heap:���� O(N),		push O(logN)		pop O(logN)
ʹ��˳�򴢴�ʵ�ֶ������ѡ��
*/

//˵���˾��Ǳ��������ڴ�ռ�����Ӷ����� ��������ͨ������������(û���д��������insert����)
//�������ֳ�ͻ�ĸ���ԭ������ �ҽ����ӽ��ʵ�ֵ� ������������һ���ȳɱ�׼(�ӿ�) �����Խ�����Ϊ�˿��Ա�������ת�� ���Գ�����һЩì��
//(����������ʵ�ֵĶ�̬�Ժ��ڴ��Լ �����о�̬ʵ�ֵ��������Ч��
/*
���Դ���һ��������[]���������� ����"�ṹ�ڴ�"
�����������˳�������Ļ� �������ڲ����ֶ�̬�� ���ⲿ���־�̬��
����һ����̬ʵ��������ʵ�ֺ϶�Ϊһ�� ��������Ҳ�����ԵĿ���˵�ֻص��˵����������ӽ��ĳ���:��Լ�ڴ� �Լ���δ֪������ǰ˳��ʵ�ֵ�������
��һ������Խ��:�����ݽṹ�ڲ����������ڴ����뷽�� һ�����ڴ���������ڴ������� һ�������ⲿ����(��һ��ע�������ע����һ�����ڴ� ����ͨ����ע���baseArray(ָ������)�ķ��ʴﵽ��ʱ�������)
����ȫ������,ʼ����һ������û�н��:˳���ɾ��ĳ�����ݺ�������¼������ ������Ҫ���ڴ�ʱ���֪�������п��ڴ� ��������Ҫ����
�ðɿ��Խ�ɾ�����ڴ�ָ����ĩβ�ڴ�ָ�뽻��(�������õ���ָ������Ķ���ָ�� �൱���õ�������ָ��(��)) ��,������ͨ������ʽ����������ȫû������ ���������ϵͳ�ڴ��������ʽ?
*/
/*��̬����������Static Binary Search Tree*/
/*SequentialBinTree˳�򴢴�Ķ�����*/
//��̳�(virtual����д���м�) �����̳������μ̳е�����µĻ����Ա�ĳ�ͻ

/*��ȫ���������� ʹ�����鹹�� ʵ�ֶ���ľ�̬���ֲ��� ����Ҫ��̬��ɾ���� ��Ҫ����ת��Ϊ����������*/
/*����ֲ������������ ���ֲ��ҵ������������ ����CBST��Ҫ�������� ����������䱾����������в��������(����Ԫ���൱��������ֵ)*/
/*����ʵ�ý�˳�򴢴�Ķ�������������ȫ������ ֻ��˵��ȫ��������˳��洢������*/

/*VirtualLinkedBinTree
ʹ����������Ķ�����(����ArrayBinTree)
��̬������Static Binary Tree(�����鴢��Ķ�������һ������ȫ������)
��StaBinTreeʵ�ֱ��ڷ����ڵ������±�ļ���
*/
// ��ȫ������
template<typename T>
class CompleteBinTree :public BinTree<T> {
	using BinTree<T>::root_;
	using BinTree<T>::nodeManager;
	using BinTree<T>::BTNode;
	using CBT = typename BinTree<T>::BT;

public:
	using Position = typename BinTree<T>::Position;

	// ֱ�Ӱ�����copy����̬������
	CompleteBinTree(T *iniA, int nSize) : CompleteBinTree(nSize){
		assert(iniA != nullptr);
		root_ = baseArray;
		for (int i = 0; i < nSize; ++i)
			root_[i].Data = iniA[i];
		initEnd(nSize);
	}
	// sort(iniA, iniA + n, less<T>());
	// ��ת����:sort(iniA, iniA + n, greater<T>()); ��ȫ�������������С����֧����С�Ҵ����ľ���ת ��ͨBinTree֧��
	// ʹ�����鹹��Ψһ����ȫ������ tips::���������� ��Ϊ������
	CompleteBinTree(int nSize, T *iniA) : CompleteBinTree(nSize) {
		assert(iniA != nullptr);
		// 0��ʾ������Sub
		root_ = buildCompleteTree(iniA, iniA + nSize, 0);
		initEnd(nSize);
	}
	virtual ~CompleteBinTree() {
		DE_PRINTF("CBT����");
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
		baseArray[rootSub].Data = first[nl];
		// first��0��ʼ��nl������������ (root_��baseArray��subΪ0 ������sub = 2*rootSub + 1)
		baseArray[rootSub].Left = buildCompleteTree(first, first + nl, 2 * rootSub + 1);
		baseArray[rootSub].Right = buildCompleteTree(first + nl + 1, last, 2 * rootSub + 2);
		return baseArray + rootSub;
	}
	// ��ȫ�������������(˳�������������)
	void levelTraversal(CBT bT, std::function<void(CBT)> visit) {
		//�޷�override ������Ӧ�ø�����
		if (bT == nullptr)
			return;
		FOR(i, 0, usedSize) {
			//bT������root_������baseArray(root_ = baseArray+1)
			if (!visit(bT + i))
				return;//��ֹ
		}
	}
	
	//�����������Ĺ������� Խ�緵���ڱ�(0�Ž��)
	Position ancestor(int i1, int i2) {
		if (i1 > usedSize || i2 > usedSize || i1 < 0 || i2 < 0)
			return position(0);
		if (i1 == i2)
			return position(i1);
		else if (i1 > i2)
			return ancestor(index(getParent(position(i1))), i2);
		else
			ancestor(i1, index(getParent(position(i2))));//return
	}
protected:
	
	// ��ʼ������
	CompleteBinTree(int nSize) {
		assert(nSize > 0);
		baseArray = new BTNode<T>[nSize];
		capacity = nSize;
	}
	// ��ʼ������
	void initEnd(int nSize) {
		nodeManager = new BinTreeAlgorithm::LinearNodeManager<T>(baseArray, nSize, nSize);
	}

	bool full() {
		return usedSize == capacity;
	}
	// ���������ڵĽ���� 1��Ϊroot_ 0��Ϊ�ڱ�
	int index(CBT t) const {
		// ��һ��Ԫ�ز���ʱroot_=nullptr �����������Һ������ӷ���������root_ ���ο�root_=1ʱ�Ĺ�����������������
		return t - baseArray;
	}
	// ���������ڵĽ��λ��
	Position position(int sub) {
		assert(0 <= sub && sub < capacity);
		return baseArray + sub;
	}
	

	Position baseArray = nullptr;
	int usedSize = 0;
private:
	int capacity = 0;
};

/*
C++��heap���㷨����ʽ�ṩ��Ҫʹ���⼸��api��Ҫ#include <algorithm>

make_heap:		����ָ���ĵ����������Լ�һ����ѡ�ıȽϺ�����������һ��heap. O(N)
push_heap:		��ָ����������һ��Ԫ�ز��뵽heap��. O(logN)
pop_heap:		����heap��Ԫ��, �������������ĩβ. O(logN)
sort_heap��		�������㷨��ͨ��ͨ����������pop_heap��ʵ��. N*O(logN)
C++11�����������³�Ա��
is_heap:		�жϸ��������Ƿ���һ��heap. O(N)
is_heap_until:	�ҳ������е�һ��������heap������λ��. O(N)

���㷨���������ʽʵ��
����ŷǻ�������ʱ �����ظ�ֵ��
*/
/*
�� �����(С)��, ��ȫ������
����O(N) ���O(logN) ����O(logN)
����ƽ��O(N*logN): N�γ���
*/
template<class T>
class Heap : public CompleteBinTree<T> {
	typedef T Element;
	typedef typename CompleteBinTree<T>::Position Position;
	using BinTree<T>::root_;
	using BinTree<T>::nodeManager;
	using CompleteBinTree<T>::baseArray;
	// ����Heap��0�ų䵱�ڱ�, �������ܽ����, ��Heap�м���baseArrayβԪ�ص��±�, ���ǽ������
	using CompleteBinTree<T>::usedSize;
	using CompleteBinTree<T>::initEnd;

	using CompleteBinTree<T>::position;
	using CompleteBinTree<T>::index;

	//ʹ��less<T> greater<T> �� �ȽϷ������� ����ͬ��������: ����û��ͬһ����ʾ����(��������()�ĸ���)�����ڴ��С�ѵļ�ʱ�ع�
	int(*cmper_)(const T &, const T &) = nullptr;
public:
	using CompleteBinTree<T>::full;
	using CompleteBinTree<T>::empty;

	/*
	(�ѵĴ�С, �ڱ�, С�ڱȽϷ���, �����ʼ���������� �����е�Ԫ�ظ���) 
	��С�� ����: moreCmper(����)�ڱ�(��Сֵ)	ע��: ����̫С���������:����MAX_INT32/2����
	���� ����: lessCmper(С��)�ڱ�(���ֵ)
	ע��: �����鹹��Ķ�������ʼ���Ķ� �Ľṹ��һ��
	*/
	Heap(int heapSize, T sentry, int(*cmper)(const T &, const T &), T *iniA = nullptr, int IniASize = 0)
		: CompleteBinTree<T>(heapSize + 1) {
		build(iniA, IniASize, sentry, cmper);
		initEnd(IniASize);
	}
	virtual ~Heap() override {
		DE_PRINTF("Heap����");
	}

	bool push(Element Item) {
		bool result = true;
		if (isNotInit()) {
			// һ��ֻ������pop�����, ����ʹ��֮ǰ�����ݳ�ʼ������
			initialize(baseArray[0].Data);
		}
		if (full()) {
			result = false;
			throw std::exception("������ �޷�ѹ��");
		}
		else {
			// @TODO �������percolateUp��������ô����
			// iָ��[�����]�������һ��Ԫ�ص�λ�� i=[1, capbility)
			int i = ++usedSize;
			for (int parI = -1; cmper_(baseArray[(parI = getParentSub(i))].Data, Item) < 0; i = parI) {
				// �����һ���к��ӵĽ�㿪ʼ ���Ϲ��˽��
				baseArray[i].Data = baseArray[parI].Data;
			}
			// ѹ��
			baseArray[i].Data = Item;
			// ��Ϊ�¼����Ԫ��ʼ����β�� ���ֱ������β����Ԫ�ؼ��ɱ�֤����Ԫ�ص�����
			linkToChildren(getParent(usedSize));
		}
		return result;
	}

	Element pop() {
		if (empty()) {
			throw std::exception("���ѿ� �޷��׳�");
		}
		else {
			// ȡ���������ص�ֵ
			Element Item = root_->Data;
			// �ӶѸ���ʼ���� ɾ��βԪ��(useedSize--����)
			percolateDown(index(root_), usedSize--);
			// �����ɾ����βԪ���븸��������
			unlinkToParent(usedSize + 1);
			return Item;
		}
	}

	// ���¹����� ���Ӷ�O(N)
	void rebuild() {
		if (isNotInit()) {
			throw std::exception("δ��ʼ��");
		}
		/*
		����һ���Ѷ�������ݵ���Ҫ������������С����С��(���¶��ϵ���)
		��[���һ���ӽڵ�ĸ��ڵ� �� ������һ���ж��ӵĽ��]��ʼ
		ȡ���ý��
		ÿ�δ����ҽ������һ��"��"�Ľ����[����]������ʼ���������ڵ�1
		*/
		for (int i = getParentSub(usedSize); i > 0; i--) {
			percolateDown(i, i);
		}
	}

	// ��ʼ��
	void initialize(T sentry, int(*cmper)(const T &, const T &) = nullptr) {
		// ���ıȽϷ���һ���ø����ڱ� ��֮��һ��
		baseArray[0].Data = sentry;
		if (cmper != nullptr) {
			cmper_ = cmper;
		}
		assert(cmper_ != nullptr);
		if (empty(root_)) {
			// root_�������ڱ�(0��) �ڴ�ռ���Զ�������һ��
			root_ = baseArray + 1;
			usedSize = nodeManager->validNodeCnt();
		}
	}

	// ������
	void build(T *iniA, int IniASize, T sentry, int(*cmper)(const T &, const T &) = nullptr) {
		initialize(sentry, cmper);
		// ������ʹ�ý����Ŀ(�����޷�����)
		usedSize = IniASize;
		// ����root_
		linkToChildren(root_);
		// Ϊÿ��Ԫ�ظ���ʼȨֵ
		for (Position t = root_; t < root_ + IniASize; ++t) {
			// ��T���Ͳ��ǻ������� ��Ҫ���ظ�ֵ��
			t->Data = iniA[t - root_];
			// ȷ���ڱ����б�������Ԫ�صļ�ֵ(��С�Ѷ�Ӧ��Сֵ, ���Ѷ�Ӧ����ֵ), ��lhs��rhs���С�����, ������Ƚ�ֵ>0,��������<0,�����=0
			assert(cmper_(baseArray[0].Data, t->Data) > 0);
			// ֻҪûԽ�������->����ʱ��usedSizeΪ���������ı�־ �������ӽ���Ƿ�Ϊ��?
			linkToChildren(t);
		}

		// ���ݸı�һ�����ؽ���
		rebuild();
	}

protected:
	
	//���� push����
	void PercolateUp_DEL(int i, Element *baseArray, int usedSize) {
		// iָ�����������Ԫ�ص�λ��
		Element Item = baseArray[i];
		for (; baseArray[i / 2] - Item > 0; i /= 2) {
			// �����˽��ĸ���㿪ʼ ���Ϲ��˽�� �����������ӽ�������
			baseArray[i] = baseArray[i / 2];
		}
		baseArray[i] = Item;
	}
	//���� :rebuild|pop����(��ĩ��㿪ʼ���� �ȼ��ڴ����һ������㿪ʼ����)
	void PercolateDown_DEL(int Parent, Element *baseArray, int usedSize) {
		int Child;
		Element x = baseArray[Parent];//ȡ����Ҫ���˵�ֵ
		for (; 2 * Parent <= usedSize; Parent = Child) {
			Child = 2 * Parent;//�������==usedSize; ���Ҷ��Ӳ�����
			if (Child != usedSize && baseArray[Child] - baseArray[Child + 1] > 0)
				++Child;//ѡȡ���Ҷ�����С��һ��
			if (x - baseArray[Child] < 0)//����Ԫ��С��Ŀ��ʱ��ֹ ����ʱ����
				break;
			else//����������<==>��temp����
				baseArray[Parent] = baseArray[Child];
		}
		baseArray[Parent] = x;
	}

	// ���� ��Heap��pdIndex����Ԫ��������parentSub�� Ȼ���� baseArray + parentSub Ϊ�����Ӷѵ���Ϊ������С����С��
	void percolateDown(int parentSub, int pdIndex) {
		// ȡ����Ҫ���˵�ֵ
		Element pdEle = baseArray[pdIndex].Data;
		for (int childSub = -1; (childSub = getLeftChildSub(parentSub)) <= usedSize; parentSub = childSub) {
			// ѡȡ���ڵ����Ҷ����д��С��һ��
			if (childSub != usedSize && cmper_(baseArray[childSub].Data, baseArray[childSub + 1].Data) < 0)
				childSub++;

			if (cmper_(pdEle, baseArray[childSub].Data) >= 0)
				break;
			else {
				// ���������� <==> ��pdEle���� ����childSub��Ҫ��ΪparentSub��������ıȽ���˲����ƶ�
				baseArray[parentSub].Data = baseArray[childSub].Data;
			}
		}
		baseArray[parentSub].Data = std::move(pdEle);
	}
	// �����Ƿ��ѳ�ʼ��
	bool isNotInit() {
		return root_ == nullptr;
	}
	//�ӽ������:���ӵ�ǰ���ĺ��ӽ�� ���ӹ�ϵֻ��λ���й� �������޹� ��˳�����ʵ��ʹ�õ�λ������ ����������
	void linkToChildren(Position parent) {
		int i = index(parent);
		parent->Left = getLeftChild(i);
		parent->Right = getRightChild(i);
	}
	//�ӽ�����:������׽��Ժ��ӽ�������
	void unlinkToChildren(int parentIndex) {
		Position parent = getParent(parentIndex);
		if (!empty(parent)) {
			// �ÿ���Ч�ӽ��
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
	//�ӽ�����:������׽��Ժ��ӽ�������
	void unlinkToParent(int childIndex) {
		Position parent = getParent(childIndex);
		if (!empty(parent)) {
			Sub parentIndex = index(parent);
			// �ÿ���Ч�ӽ��
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

			// ���ӽ����root_��ʱ����Ҫ��root_��Ϊ��
			if (parent == baseArray) {
				root_ = nullptr;
			}
		}
	}

	// 0�����ڱ�; 1���Ǹ����
	// �����������±�
	Sub getLeftChildSub(int parentIndex) {
		return 2 * parentIndex;
	}
	Position getLeftChild(int parentIndex) {
		int sub = getLeftChildSub(parentIndex);
		return 0 < sub && sub <= usedSize ? position(sub) : nullptr;
	}
	// �����������±�
	Sub getRightChildSub(int parentIndex) {
		return 2 * parentIndex + 1;
	}
	Position getRightChild(int parentIndex) {
		int sub = getRightChildSub(parentIndex);
		return 0 < sub && sub <= usedSize ? baseArray + sub : nullptr;
	}
	// ���ظ�����±� root_�ĸ�����Ӧ�ڱ�
	Sub getParentSub(int sonIndex) {
		return sonIndex / 2;
	}
	Position getParent(int sonIndex) {
		return baseArray + getParentSub(sonIndex);
	}
	// �����ֵܽ��
	Position getSibling(int sibIndex) {
		// 1���Ǹ����û���ֵ�
		return sibIndex > 1 ?
			sibIndex % 2 == 0 ? baseArray + sibIndex + 1 : baseArray + sibIndex - 1
			: nullptr;
	}

};

