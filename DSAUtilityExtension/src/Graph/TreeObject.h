#ifndef _TREEOBJECT_H
#define _TREEOBJECT_H

#include"../stdafx.h"
#include"../ExtendSpace.h"
/*
*����:
*���Ͼ�ֻ������ �ʺ�����ɾ��Ķ�̬���� ���ʺ�����������
*��ֻ������PTA�����ݽṹ�γ̻���˵������ƫִ�뷨���������� �Ժ���ƾ������������뷨Ҳ���������Ļ����뻷����
*��������ೣʶ�Ե�����ٹ̵���ʶ����
*��[���ṹ]����ʶ��������ڳ���
*��[C++�ı�׼��]ʹ�� �Լ�[ģ����]{��ģ�� ����ģ��} ����һЩ�ĵ�(C++�߼����)
*��[���������]���˼������;���
*��[�㷨]���ӵĺ�����һ�����(����O ���Ԧ� ���ަ�)
*��һ��ȫ��������̳й�ϵ�����������໥���ת���ܷ��㲻��ʱ(������Ч�ʱȲ��ϵ���ʵ�ֵ�)
*/
/*�� ������(�ӿ���)*/
class Tree{
public:
	// ������˳������ö��
	enum TraversalOrderEnum{
		ORDER_PREFIX_ROOT, // �ȸ���
		ORDER_INFIX_ROOT, // �и���
		ORDER_POST_ROOT, // �����
		ORDER_LEVEL, // ����
	};
	int Min(int a, int b){
		return a < b ? a : b;
	}
	int Max(int a, int b){
		return a > b ? a : b;
	}
protected:
	//�����
	template<typename T> using ArrayList = std::vector<T>;
	template<typename T> using stack = std::stack<T>;
	template<typename T> using queue = std::queue<T>;
	template<typename T> using priority_queue = std::priority_queue<T>;
	template<typename T> using greater = std::greater<T>;
	using string = std::string;
	//template<typename T> using vector = StandardExtend::ArrayList<T>;
};

/*����(��)��*/
template<class T>
class BinTree :public Tree{
public:
	typedef T Element;
	/*���������*/
	typedef class BTNode{
	public:
		BTNode(){}
		//��ȡ����height�ֶ�
		virtual int getHeight(){
			return depthOf(this);
		}
		//��������height�ֶ�
		virtual void setHeight(int){}
		//��ȡ���������ֶ�:weight
		virtual int getValue(){
			return 0;
		}
		//������������ֶ�
		virtual void setValue(int){}
		//��������Ĭ�Ϲ��캯��
		BTNode(Element const&tData) : Data(tData){}
		/*BTNode(Element const&tData){
			Data = tData;
		}*/
		//�������� ���Һ��ӽ����nullptr
		BTNode(BTNode &rhs){
			(*this) = rhs;
			Left = Right = nullptr;
		}
		virtual ~BTNode(){
			DE_PRINTF("BTNode����");
		}
		//��ֵ Ĭ�ϵ�ȫ��ֵ
		BTNode& operator=(const BTNode&) = default;
		int height(){
			return depthOf(this);
		}
		//Ҷ�ӽ���ж�
		bool isLeave() const {
			return Left == nullptr && Right == nullptr;
		}
		/*
		bool operator=(HuTrNode const &rhs){
		return Weight == rhs.Weight;
		}
		bool operator=(int const &rhs){
		return Weight == rhs;
		}
		bool operator<(HuTrNode const &rhs){
		return Weight < rhs.Weight;
		}
		bool operator>(HuTrNode const &rhs){
		return Weight > rhs.Weight;
		}
		bool operator<=(HuTrNode const &rhs){
		return Weight <= rhs.Weight;
		}
		bool operator>=(HuTrNode const &rhs){
		return Weight >= rhs.Weight;
		}
		bool operator==(HuTrNode const &rhs){
		return Weight == rhs.Weight;
		}
		bool operator!=(HuTrNode const &rhs){
		return Weight != rhs.Weight;
		}
		*/

		BTNode* Left = nullptr;		/* ָ�������� */
		BTNode* Right = nullptr;		/* ָ�������� */
		bool v = false;				/* ��ʾ�Ƿ�visit���˽�� */
		Element Data;				/* �������:��Ϊ�ṹ��,�ṹ���ṩ�ȽϷ���key(��ɾ�鶼��ʹ�õĹؼ���)�ڽṹ���� ����key����Data weight-height������Ϊkey(��Ψһ)*/

	} *Position;
	// ��˵Position���޸�Ȩ�� ��Ӧ��BTû��
	using BT = BTNode const *;
	using Vister = std::function<void(BinTree<T>::BT const)>;

	BinTree(){}
	// �������� (����ֻ��֤�������һ��; ���ò���=>��������)
	BinTree(const BinTree &rhs){
		DE_PRINTF("BT��������");
		assignment(root_, rhs.root_);
		usedSize = rhs.usedSize;
	}
	// �ƶ����� (��֤��ȫһ��)
	BinTree(BinTree &&rvalue) {
		DE_PRINTF("BT�ƶ�����");
		std::swap(root_, rvalue.root_);
		std::swap(usedSize, rvalue.usedSize);
		std::swap(isInsert, rvalue.isInsert);
		std::swap(lastInsertPosition, rvalue.lastInsertPosition);
	}
	// �������й��� ȱʡ�ı������з���Ԫ�ظ���
	BinTree(Element const *preOrder, Element const *inOrder, int n){
		prefInBuild(preOrder, inOrder, root_, n);
	}
	// �к����й���
	BinTree(int n, Element const *inOrder, Element const *postOder){
		postInBuild(root_, inOrder, postOder, n);
	}
	
	// �ȸ������������ջ���� (��ջ������ȡ����, ������ݻ�ȡ����)
	BinTree(std::function<bool(std::string &)> getOrder, void(*getData)(T*)){
		ArrayList<T> preOrder, inOrder;
		stack<T> s;
		std::string str;
		while (getOrder(str)) {
			int len = -1;
			Utility::AssertToSignedNum(str.length(), len);
			if (len == 4) {//Push
				T ele;
				getData(&ele);
				// ��һ������ʱ(����)
				preOrder.push_back(ele);
				s.push(ele);
			}
			else if (len == 3){//Pop
				// �ڶ�������ʱ(����)
				inOrder.push_back(s.top());
				s.pop();
			}
			else {
				throw std::exception("Order error!");
			}
		}
		_ASSERT_EXPR(preOrder.size() == inOrder.size(), "Size Error");
		prefInBuild(preOrder, 0, inOrder, 0, root_, preOrder.size());
	}

	// destructor
	virtual ~BinTree() {
		destroy(root_);
		DE_PRINTF("BT����");
	}

	// ��������ض�������ֵ(��ֵ��������ԭlhs ��ֻ�ܱ�֤����һ��)
	BinTree &operator= (const BinTree &rhs) {
		// ��Ҫ�����Լ���root_
		destroy(root_);
		assignment(root_, rhs.root_);
		usedSize = rhs.usedSize;
		return *this;
	}
	// ��������ض������ƶ�(ϵͳ�Զ�����ԭlhs)
	BinTree &operator= (BinTree &&rvalue) {
		std::swap(root_, rvalue.root_);
		std::swap(usedSize, rvalue.usedSize);
		std::swap(isInsert, rvalue.isInsert);
		std::swap(lastInsertPosition, rvalue.lastInsertPosition);
		return *this;
	}

	// �����������(��ʼ�����н�����Ч��, �ṹһ���ᱻ�ݻ�, root_�ᱣ��)
	virtual void clear() {
		destroy(root_);
		lastInsertPosition = nullptr;
		isInsert = false;
		usedSize = 0;
	}

	Element getRootData() const {
		return root_->Data;
	}
	BT getRoot() const {
		return root_;
	}
	// ��������Ϊ�շ���true (��������1����㺬������) O(1)
	bool empty() const {
		// (�����Ϊ�ձ�ʾ������Ϊ�� �ձ�ʾ��ǰ��㲻���ڶ��ǲ������ӽ��)
		// return empty(root_);
		return usedSize == 0;
	}
	// �������߶�|��� O(H)
	int height() const {
		// ����֪size��Ϊlog�ĸ��Ӷ�(ò�Ʋ�������)
		// ceil(log(size, 2))
		return depthOf(root_);
	}
	// ���������� => [0, height)
	int layers() const {
		return height() - 1;
	}

	// ��(2*N)     Tree::ORDER, void visit(BinTree<T>::BT node)
	void traversal(TraversalOrderEnum type, Vister visit){
		if (type == ORDER_PREFIX_ROOT)
			preTraversal(root_, visit);
		else if (type == ORDER_INFIX_ROOT)
			infTraversal(root_, visit);
		else if (type == ORDER_POST_ROOT)
			postTraversal(root_, visit);
		else if(type == ORDER_LEVEL)
			levelTraversal(root_, visit);
		else
			_ASSERT_EXPR(false, "������������ NONE_ORDER ����traversal_");
	}
	
	static void orderTranslation(Element *buf, Element *preOrder, Element *inOrder, Element *postOder, int n){
		if (inOrder == nullptr)
			throw std::exception("must know inorder");
		else if (preOrder == nullptr && postOder != nullptr)
			calcPrefOrder(buf, inOrder, postOder, n);
		else if (postOder == nullptr)
			calcPostOrder(preOrder, inOrder, buf, n);
		else
			throw std::exception("Does not meet the conversion conditions");
	}
	// ������: mirroring reversal����ת ת������ʹ��ԭ�����κλ��ڱȽϵķ���(����������:��С�Ҵ�->�����С)
	void mirReversal(){
		queue<Position> q;
		q.push(root_);
		while (!q.empty()){
			Position t = q.front();
			q.pop();
			// �����зǿ���������ת������
			if (t->Left != nullptr)
				q.push(t->Left);
			if (t->Right != nullptr)
				q.push(t->Right);
			// ����������������ת��
			std::swap(t->Left, t->Right);
		}
	}
	// ͬ���ж�
	bool omorphism(BinTree const &T2){
		return isomorphic(this->root_, T2.root_);
	}
	// �����е����ṹ���������(�����þ�̬��������������ṹ ����ԭ�ṹ)
	// ��ͨ������->�����������������Ի��ȷ����һ����
	// ��������->���ݷ��϶���������Ҫ��
	// ��ȫ������->������Ĺ��췽��(��������ȫ������������ʵ������)
	bool fillData(ArrayList<T> &dataA){
		return fillData(dataA, 0, dataA.size(), root_);
	}
	int size(){
		return usedSize;
	}
	
protected:
	const static int BT_NODE_LEN = sizeof(class BTNode);
	Position root_ = nullptr;
	int usedSize = 0;// ��Ч��Ԫ�ظ���
	Position lastInsertPosition = nullptr;// ���������������ɵĽ�� (�޷�������жϲ���ɹ����)
	bool isInsert = false;// �Ƿ�ִ���˲������(�жϲ����Ƿ�ɹ�)

	// queue<Element*> freeMem;//�����ڴ�
	// Element *memoryBlock = nullptr;//�ڴ�� �ɽ��������ľֲ����������� ��������ʹ���ⲿ������ڴ�

	// �����Ӷ������Ĺ�ģ O(N)
	static int scaleOf(BT t) {
		return empty(t) ? 0 : scaleOf(t->Left) + scaleOf(t->Right) + 1;
	}
	// ���ؽ����� (�������Ϊ�� ��root��Ϊ1)
	static int depthOf(Position bT){
		int HL, HR;
		if (bT) {
			HL = depthOf(bT->Left);
			HR = depthOf(bT->Right);
			// (���ݹ�ʽ Height = max(Hl, Hr) + 1 �ɺ�������ı�ʵ��)
			return (HL > HR ? HL : HR) + 1;
		}
		else
			return 0;
	}
	// ��ԭ�ṹ������� (����, ����������±�, ��ǰ���µ����ݸ���, �ṹ��Դ��) O(N^2)
	static bool fillData(ArrayList<T> &dataA, int dataRootSub, int dataSize, Position bt){
		if (empty(bt))
			return true;
		// ����������ģ �������СС���Ż�һ��: ��������ģ����ͨ����֪��size��ȥ������-1��� ��Ҫ�޸Ĳ����Ƚ��鷳(�Ż����Ӷ�����N^2)
		int nl = scaleOf(bt->Left), nr = scaleOf(bt->Right);
		// �������Ĺ�ģ�������ݹ�ģ���
		if (nl + nr + 1 != dataSize)
			return false;
		//�������
		bt->Data = dataA[dataRootSub + nl];
		//�������ݹ�							//1��1�������
		fillData(dataA, dataRootSub, dataSize - nr - 1, bt->Left);
		//�������ݹ�      //��1��������nl ��"С��"������
		fillData(dataA, dataRootSub + nl + 1, dataSize - nl - 1, bt->Right);
		return true;
	}
	// ���ٽ��(ֻ��֤���ú���������Ч, ��һ�����������)
	virtual void destroy(Position &r){
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
				delete current;
			}
			r = nullptr;
		}
	}

	// �������---���ϵ���->������(����ʵ��)
	static void levelTraversal(BT bT, Vister visit){
		if (!bT)return;
		queue<BT> q;
		q.push(bT);
		while (!q.empty()){
			bT = q.front(); q.pop();
			visit(bT);
			if (bT->Left)
				q.push(bT->Left);
			if (bT->Right)
				q.push(bT->Right);
		}
	}
	// ����---������->������->������;(�ݹ�ʵ��)
	static void preTraversal(BT bT, Vister visit){
		if (bT){
			visit(bT);
			preTraversal(bT->Left, visit);
			preTraversal(bT->Right, visit);
		}
	}
	// ����---������->������->������;
	static void infTraversal(BT bT, Vister visit){
		if (bT){
			infTraversal(bT->Left, visit);
			visit(bT);
			infTraversal(bT->Right, visit);
		}
	}
	// ����---������->������->������;
	static void postTraversal(BT bT, Vister visit){
		if (bT){
			postTraversal(bT->Left, visit);
			postTraversal(bT->Right, visit);
			visit(bT);
		}
	}

	//���й���
	void prefInBuild(Element const *preOrder, Element const *inOrder, Position &bt, int n){
		int Ln;/*������������*/
		if (n == 0)return;
		if (bt == nullptr)
			bt = nodeCreater(*preOrder);
		else/*ת����ϵ:��������������Ԫ�ؾ���������*/
			bt->Data = *preOrder;
		for (Ln = 0; Ln < n && inOrder[Ln] != *preOrder; Ln++);
		prefInBuild(preOrder + 1, inOrder, bt->Left, Ln);/*�����������������������һ��Ԫ�� �������鱣�ֲ���*/
		prefInBuild(preOrder + Ln + 1, inOrder + Ln + 1, bt->Right, n - Ln - 1);/*������������һ��Ԫ��*/
	}
	//���й���ArrayList
	void prefInBuild(ArrayList<Element> &preOrder, int preRoot, ArrayList<Element> &inOrder, int inRoot, Position &bt, int n){
		int Ln;/*������������*/
		if (n == 0)return;
		if (bt == nullptr)
			bt = nodeCreater(preOrder[preRoot]);
		else/*ת����ϵ:��������������Ԫ�ؾ���������*/
			bt->Data = preOrder[preRoot];
		for (Ln = 0; Ln < n && inOrder[inRoot + Ln] != preOrder[preRoot]; Ln++);
		prefInBuild(preOrder, preRoot + 1, inOrder, inRoot, bt->Left, Ln);/*�����������������������һ��Ԫ�� �������鱣�ֲ���*/
		prefInBuild(preOrder, preRoot + Ln + 1, inOrder, inRoot + Ln + 1, bt->Right, n - Ln - 1);/*������������һ��Ԫ��*/
	}
	//���й���
	void postInBuild(Position &bt, Element const *inOrder, Element const *postOder, int n){
		int Ln;/*����������*/
		if (n == 0)return;
		if (bt == nullptr)
			bt = nodeCreater(postOder[n - 1]);
		else/*ת����ϵ:������������βԪ�ؾ���������*/
			bt->Data = postOder[n - 1];
		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; Ln++);//��ȡ����������
		postInBuild(bt->Left, inOrder, postOder, Ln);/*��������������������*/
		postInBuild(bt->Right, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*����������*/
	}
	//���й��� �Ż���(@TODO size)
	static BT postInBuild(int *inOrder, int *postOder, int n){
		int Ln;/*����������*/
		if (n == 0)	return nullptr;

		Position bt = (Position)malloc(sizeof(struct TreeNode));
		memset(bt, 0, sizeof(struct TreeNode));
		bt->Element = postOder[n - 1];/*ת����ϵ:������������βԪ�ؾ���������*/

		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; Ln++);//��ȡ����������
		bt->Left = postInBuild(inOrder, postOder, Ln);/*��������������������*/
		bt->Right = postInBuild(inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*����������*/
		return bt;
	}

	static void calcPostOrder(string const &sPre, string const &sMed, string &sPostBuffer){
		//�����еĸ�����λ��
		string::size_type medRootSub = sMed.find(sPre[0]);
		//�������ĳ���
		string::size_type leftSubLen = medRootSub;
		/*
		A B CD	ǰ��(Root Left Right)
		B A DC	����(Left Root Right)

		BDC A	����(Left Right Root)
		*/

		//�����������������������ݹ�
		if (leftSubLen > 0){
			//�����һ��ֵ(root_)�ұ�ȡ�������ĳ��ȼ���������
			string sPreLeftSub = sPre.substr(1, leftSubLen);
			//root_�����
			string sMedLeftSub = sMed.substr(0, leftSubLen);
			calcPostOrder(sPreLeftSub, sMedLeftSub, sPostBuffer);
		}
		else{
			//do nothing
		}

		//�����������������������ݹ�
		if (medRootSub + 1 < sMed.size()){
			//�����һ��ֵ(root_)���ұ���������������ȡ���м���������
			string sPreRightSub = sPre.substr(1 + leftSubLen);
			//root_���ұ�
			string sMedRightSub = sMed.substr(medRootSub + 1);
			calcPostOrder(sPreRightSub, sMedRightSub, sPostBuffer);
		}
		else{
			//do nothing
		}

		if (medRootSub != string::npos){
			//����root_
			sPostBuffer += sMed.at(medRootSub);
		}
		else{
			//do nothing
		}
	}
	/*������������������������������ (�� �� �� ����Ԫ�ظ���)*/
	template<class Iterator>
	//֧�ּӼ�����ĵ�����(��ָ��Ҳ�� postOder��Ӧ�����������������n��Ԫ��)
	static void calcPostOrder(Iterator preOrder, Iterator inOrder, Iterator postOder, int n){
		int Ln;/*������������*/
		if (n == 0)return;
		postOder[n - 1] = *preOrder;/*ת����ϵ:��������������Ԫ�ؾ���������*/
		for (Ln = 0; Ln < n && inOrder[Ln] != *preOrder; ++Ln);
		calcPostOrder(preOrder + 1, inOrder, postOder, Ln);/*�����������������������һ��Ԫ�� �������鱣�ֲ���*/
		calcPostOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*������������һ��Ԫ��*/
	}
	/*������������������������������ (�� �� �� ����Ԫ�ظ���)*/
	static void calcPostOrder(Element *preOrder, Element *inOrder, Element *postOder, int n){
		int Ln;/*������������*/
		if (n == 0)return;
		postOder[n - 1] = *preOrder;/*ת����ϵ:��������������Ԫ�ؾ���������*/
		for (Ln = 0; Ln < n && inOrder[Ln] != *preOrder; Ln++);
		calcPostOrder(preOrder + 1, inOrder, postOder, Ln);/*�����������������������һ��Ԫ�� �������鱣�ֲ���*/
		calcPostOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*������������һ��Ԫ��*/
	}
	/*���ݺ������������������������� ֻ�޸��м����仰����*/
	template<class Iterator>
	static void calcPrefOrder(Iterator preOrder, Iterator inOrder, Iterator postOder, int n){
		int Ln;/*����������*/
		if (n == 0)return;
		*preOrder = postOder[n - 1];/*ת����ϵ:������������βԪ�ؾ���������*/
		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; ++Ln);//��ȡ����������
		calcPrefOrder(preOrder + 1, inOrder, postOder, Ln);/*��������������������*/
		calcPrefOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*����������*/
	}
	/*���ݺ������������������������� ֻ�޸��м����仰����*/
	static void calcPrefOrder(Element *preOrder, Element *inOrder, Element *postOder, int n){
		int Ln;/*����������*/
		if (n == 0)return;
		*preOrder = postOder[n - 1];/*ת����ϵ:������������βԪ�ؾ���������*/
		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; Ln++);//��ȡ����������
		calcPrefOrder(preOrder + 1, inOrder, postOder, Ln);/*��������������������*/
		calcPrefOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*����������*/
	}
	/*���ݺ������������������������� ����->��->������->�������*/
	static void calcLeveOrder(Element *preOrder, Element *inOrder, Element *postOder, int n){
		puts("has not imp");
		exit(-1);
	}

	// �����Ƿ�T1��T2�Ƿ�ͬ�� isomorphic(adj. [��] ͬ���ģ�ͬ�ε�)  isomorphism_(n. ����ͬ��[�ﻯ] ����ͬ����ͬ��)
	static bool isomorphic(BT T1, BT T2){
		// ��Ȳ�ͬ
		if ((T1 == nullptr && T2 != nullptr) || (T1 != nullptr && T2 == nullptr))
			return false;
		// Ҷ�ӽ��
		else if (T1 == nullptr && T2 == nullptr)
			return true;
		else {// ����������Ϊ��
			// ���ݲ�ͬ
			if (T1->Data != T2->Data)
				return false;
			else { // ��ǰ�������
				// ����ǰ˳����ƥ����������
				if (isomorphic(T1->Left, T2->Left))
					return isomorphic(T1->Right, T2->Right);
				else { // ����ת����������ƥ��
					if (isomorphic(T1->Left, T2->Right))
						return isomorphic(T1->Right, T2->Left);
					else
						return false;
				}
			}
		}
	}

	// �ݹ鿽����ֵ
	static void assignment(Position &lhs, const Position rhs){
		if (!empty(rhs)){
			lhs = new BTNode(*rhs);
			assignment(lhs->Left, rhs->Left);
			assignment(lhs->Right, rhs->Right);
		}
	}
	// �ж϶������Ƿ�Ϊ��
	static bool empty(BT bT) {
		return bT == nullptr;
	}

	//����ֵ(����)
	void reAssignment(){}
	// �ظ����봦��
	void repetitionInsert(Position bt){
		//x==bt->Data ������� �ֶ�����lastInsertPosition
		lastInsertPosition = bt;
		isInsert = false;
	}
	
public:
	//��������� ����һ��δʹ�õĽ�� ��������δʹ�ý�� ����nullptr ֻ�ܲ���ʹ��
	virtual Position nodeCreater(Element const &tData){
		lastInsertPosition = new BTNode(tData);
		isInsert = lastInsertPosition == nullptr ? false : true;
		++usedSize;
		return lastInsertPosition;
		/*
		bST = (BST)malloc(sizeof(struct TNode));
		memset(bST, 0, sizeof(struct TNode));
		bST->Element = x;
		*/
	}
	//�������� �������Ϊδʹ��״̬
	virtual void nodeEraser(Position &del){
		// free(del); del = nullptr;
		delete del;
		del = nullptr;
		--usedSize;
	}
};

/*�������������� ���� �ж�����*/
/*�۰���ҵ��ж���:���ձȽϵĴ��������ж������Ƚ�1�ε��Ǹ���㣬�Ƚ�2�ε��ڵڶ��㣬�Ƚ�3�ε��ڵ�����...
��ʵ���ǵݹ�Ľ�mid��Ϊ�����
����ע��(��С�Ҵ�Ķ���������):1 2 (mid=1)   4 5 (mid=4)��С�Ľ����Ϊ�����Ŀ����Ը���
���п��ܳ���135�ȵ���б; �������������ܶ�һЩ*/
/*
���ʣ�
�ٷǿ� �����������м�ֵ С������ڵ�ļ�ֵ
�ڷǿ� �����������м�ֵ ����������ļ�ֵ
�������������Ƕ���������
��û�м�ֵ��ȵĽڵ�
���Ԫ�أ����ҷ�֧��һֱbST = bST->Right������
��СԪ�أ������֧

n�������ж���depthOf = [log2(n)]+1
�ж�����ÿ�����Ĳ��Ҵ���==���ڲ���
���ҳɹ�����<=depthOf
4������������ƽ�����Ҵ���ASL = (4*8+3*4+2*2+1*1)/15
*/
template<class T>
class LinkedBinSearchTree : public virtual BinTree<T> {
public:
	enum TraversalOrderStopEnum {
		ORDER_SEQUENCE, // ˳�� (<=>����ֹ���ȸ���)
		ORDER_REVERSE, // ����
	};
	typedef typename BinTree<T>::BT BST;
	using StopVister = std::function<bool(BST const)>;
	//using BinTree<T>::Element;�����÷�
	//using Position = BinTree<T>::Position;�����÷�
	typedef typename BinTree<T>::Position Position;
	typedef typename BinTree<T>::Element Element;
	using BinTree<T>::root_;
	using BinTree<T>::lastInsertPosition;
	using BinTree<T>::traversal;
	using BinTree<T>::empty;
	using BinTree<T>::destroy;
	using BinTree<T>::nodeCreater;
	using BinTree<T>::nodeEraser;
	//constructor
	LinkedBinSearchTree(){}
	//�������� ֱ�ӵ��ĸ��෽��
	LinkedBinSearchTree(const LinkedBinSearchTree &rhs) :BinTree<T>(rhs)/*������δ����ǿ��ת��*/{
		DE_PRINTF("BST��������");
	}
	/*ֻҪ���ı������������Ե����������������������ȹ������������ȹ����������޹�
	��������ͨ���빹�����е�����:
	[��������]:[82] 76 23 80 | 90 95
	[��������] 82 90 76 23 95 80 ��������������ͬһ����*/
	//������ (��������������,�����Ҷ�+Ԫ�ظ���)
	LinkedBinSearchTree(Element *preLeft, Element *preRight, int eqaul = 0){
		while (preLeft < preRight){
			if (!insert(*preLeft)){
				puts("����ʧ��!");
				system("pause");
			}
			++preLeft;
		}
	}
	//������ ArrayList
	LinkedBinSearchTree(JCE::ArrayList<Element> &preOrder, int eqaul = 0){
		/*��preOrder������������
		**�ᷢ��:
		**ArrayList��������
		**ArrayList����
		**BT��������
		**BST��������
		**BST����
		**BT����
		**ArrayList����(��֪�������ǹ��������������� ���ǹ���һ������һ��)
		(�����Ϊʲô�ڹ��캯���� �ô�ֵ���������������һϵ�� �Ҿ�����ȫû��Ҫ��
		������һ��ʼû��д��Ӧ����������...)
		ע��!!!
		������LinkedBinSearchTree t = LinkedBinSearchTree(ArrayList<>);
		���������Ҳ������ø�ֵ���� ���ǵ��õĿ������캯��
		preOrder�����õĻ��������е�ƨ�¶�û��
		*/
		for (int i = 0; i < preOrder.size(); ++i) {
			if (!insert(preOrder[i])){
				puts("����ʧ��!");
				system("pause");
			}
		}
	}
	//����deleter
	virtual ~LinkedBinSearchTree() override{
		DE_PRINTF("BST����");
	}
	//��ֵ �ܷ�ʱ�� ���������Ķ�������ֵ
	LinkedBinSearchTree& operator= (const LinkedBinSearchTree& rhs){
		DE_PRINTF("BST��ֵ");
		//<==>���θ��࿽������ һ�θ��ำֵ ���θ�������
		//(BinTree)(*this) = (BinTree)rhs;
		// ==>ǿ��ת����ʵ�ǵ��õĿ������췽��(����Ч�ʲ���) ����Ӧ��Ϊ�����д�Լ��ĸ�ֵ����(��ʹû�������ӵ���)
		BinTree<T>::destroy(root_);//��Ҫ�����Լ���root_
		BinTree<T>::assignment(root_, rhs.root_);
		return *this;
	}

	//���ص�����k���Ԫ��λ�� �����ڷ���nullptr;(����BST���������˳����ص�:������������)
	BST findRKth(BST t, int *k){
		if (t){
			BST r = findRKth(t->Right, k);//1.<�׶�һ>һֱ�����ұ�
			if (r) return r;//��ת����: ������2���ض���(3��4)�򷵻�

			if (--(*k) == 0)//3.<�׶ζ�> ��˳�������ʼ��k�����
				return t;//�յ㷵��: ���ǵ�����k�����

			return findRKth(t->Left, k);//4.<�׶���>ǰ�涼û�з��� ˵������Ҫ��Ľ����������(����):�൱��һ���µĵ���
			//���÷���: ���ظ�1; ���ظ�4; ���ظ���ʼ����0; ������1 2(nullptr ��ʵҲ��4 nullptr�����ֻ����������ߵ�ͷ��û�еõ�3�ķ���) 3���ض���
		}
		return nullptr;//2.�յ㷵��: ��ͷ ����nullptr
	}
	//����������k��Ԫ��(���Ϊk��Ԫ�� ��1��ʼ)
	BST findKth(int k){
		int id = 1;
		Position t;
		traversal(Tree::ORDER_SEQUENCE, [&](Position bt){
			t = bt;
			return id++ == k;//�ҵ�����ֹ
		});
		return t;
	}
	//�����ض�Ԫ�صĵȼ�(�Ȳ������Ԫ�صĸ���) O(2*N)
	int getRankOf(T &x){
		int rank = 1;
		traversal(Tree::ORDER_REVERSE, [&](Position bt){
			//15 30 40 45 50 50   rank(45) = 3 �������3 ������2
			if (bt->Data > x)//ȥ�������������
				++rank;
			return bt->Data == x;//�ҵ�����ֹ
		});
		return rank;
	}
	//��������Ԫ�صĵȼ� (������Ϻ�total sortһ���������Ƶķ�������һ��ͳ�Ƴ�����������) ��(2*N)����
	//��ȻN��insert(O(logN))��N����ȡ��sort(O(N*logN))��Ч�ʲ�� ���۱���Ч��tree ��(2*N), ArrayList ��(N)
	void calcRank(JCE::ArrayList<T> &total, int cnt = -1){
		cnt = cnt < 0 ? BinTree<T>::size() : cnt;//Ĭ�ϼ��������˵�����
		int pastId = 0;//���
		int reCnt = 0;//��ǰԪ�����ڵ��ظ�����
		Position pastP = nullptr;
		traversal(Tree::ORDER_REVERSE, [&](Position bt){
			if (pastP != nullptr && pastP->Data.score == bt->Data.score)
				++reCnt;
			else
				reCnt = 0;
			bt->Data.rank = ++pastId - reCnt;//�ȼ� = ��� - �ظ�����
			total.push_back(bt->Data);//ͳ��������
			pastP = bt;
			return cnt-- == 0;//����ǰcnt�� ��cntΪ������ֹ
		});
	}
	/*���ض���������bST����СԪ����ָ�룻*/
	Position findMin(){
		BST bST = root_;
		while (bST && bST->Left)
			bST = bST->Left;
		return bST;
		/*
		//1.
		if (!bST)//�������壺��Ϊ�շ���nullptr
		return nullptr;
		else if (!bST->Left)
		return bST;//�������壺����Ҷ�������СԪ��
		else
		return FindMin(bST->Left);//�ݹ鶨�壺��������Ҷʱ������������������
		//2.�����װ�: ������������ �о͵ݹ� û�оͷ���
		return t->Left ? findMin(t->Left) : t->Data;
		*/
	}
	/*���ض���������bST�����Ԫ����ָ�롣*/
	Position findMax(){
		BST bST = root_;
		while (bST && bST->Right)
			bST = bST->Right;
		return bST;
	}

	//�����Ƿ��Ƕ���������
	bool IsBST(BST t){
		if (t){
			if (!IsBST(t->Left))
				return false;
			if (!IsBST(t->Right))
				return false;
			if (t->Left){
				bool is = t->Left->Data < t->Data;
				if (t->Right){
					is = is && t->Data < t->Right->Data;
					if (!is) return false;
					is = is && findMin(t->Right) > t->Data;//��������СԪ�ش��ڸ����
					if (!is) return false;
				}
				is = is && findMax(t->Left) < t->Data;//���������Ԫ��С�ڸ����
				return is;
			}
		}
		return true;
	}
	//�����������Ĺ�������(����) �Ż�������ʽ: �㷨Ч��O(n)->O(h)��������ذ�������������� LCA
	Position ancestor(Position t, int u, int v){
		if (t){
			/*��С���� ��������*/
			if (u < t->Data && v < t->Data)
				return ancestor(t->Left, u, v);
			if (u > t->Data && v > t->Data)
				return ancestor(t->Right, u, v);
			/*һ��һС ���� �Ż������ж�*/
			return ((find(t, u) && find(t, v)) ? t : nullptr);
		}
		return nullptr;
	}
	void clearVisit(Tree::TraversalOrderEnum type_){
		BinTree<T>::traversal(type_, [&](Position const t){
			if (t->v){
				t->v = false;
				return true;
			}
			else//���ҽ��� ������ʽ��visit����ʽ��ͬʱ ��ǰ���û�б������ӽ��ض�û�б�����
				return false;//��ֹ
		});
	}

	//  ����������ֵΪtrue��ʾ������ֹ����
	void traversal(TraversalOrderStopEnum type, StopVister visit) {
		if (type == ORDER_SEQUENCE)
			orderTraversal(root_, visit);
		else if (type == ORDER_REVERSE)
			reOrderTraversal(root_, visit);
		else
			_ASSERT_EXPR(false, "������������ NONE_ORDER ����traversal_");
	}
	/*����x�ڶ���������bST�е�λ��; ���Ҳ����򷵻�nullptr*/
	Position find(T const&x){
		return findOf(root_, x);
	}
	//�Զ������(�Զ���С�ڷ���)
	template<typename LessCmp>
	Position find(T const&x, LessCmp less){
		return findOf(root_, x, less);
	}
	//���� O(logN)
	JCE::pair<Position, bool> insert(Element const&x){
		root_ = Insert(root_, x);
		//��map��insert����ֵ���ƣ��ظ�insert ����<�ظ�Position, false>�������������������ҳ�2��������ͬ�����ֵ�ʱ������Ч
		return{ BinTree<T>::lastInsertPosition, BinTree<T>::isInsert };
	}
	//���в��� �ֲ��������O(1) ����O(logN) �����Avlʹ�ô˲����ʹAvl�˻�Ϊ��ͨBST(�൱���Ż�������: ����Ч�ʱ�� ����Ч�����Avl�˻�)
	//(����654789)->4 5 [6] Orderly����false 7 [�ֶ�]��true 8 9 ��ʵ�־ֲ��������
	JCE::pair<Position, bool> insertOrderly(Element const&x, bool &Orderly){
		if (!Orderly){//���Ѿ���������Ĳ��� ���˻�Ϊ��ͨ����
			root_ = Insert(root_, x);
		}
		else{
			if (!BinTree<T>::empty(root_)){
				if (x < root_->Data){//�����������������������
					if (x < lastInsertPosition->Data){//x��λ�������� ��x����lastInsertPositionС���߱ض�Ҳ��������
						Insert(lastInsertPosition, x);//����ֱ����lastInsertPosition����߲弴��
						Orderly = true;
					}
					else{//����Ӧ�ò����λ��λ��������:root_��lastInsertPosition�м�
						Orderly = false;
						root_ = Insert(root_, x);
					}
				}
				else{//���ʱֱ�Ӵ���ʹInsert�ж�
					if (lastInsertPosition->Data < x){///@x > lastInsertPosition->Data
						Insert(lastInsertPosition, x);
						Orderly = true;
					}
					else{
						Orderly = false;
						root_ = Insert(root_, x);
					}
				}
			}
			else{
				Orderly = true;
				root_ = Insert(root_, x);
			}
		}
		return{ lastInsertPosition, BinTree<T>::isInsert };
	}
	//ɾ�� ɾ���ɹ�����true
	bool erase(Element const&x){
		bool succeed = true;
		root_ = Delete(root_, x, succeed);
		return succeed;
	}
protected:
	Position findOf(Position bST, T const &x){
		while (bST){
			//@���Խ�� ��ɾ�Ĳ� �����˳��Ӧ�ñ���һ�� ����ֻ��һ�ֱȽϷ���
			if (x < bST->Data)
				bST = bST->Left;
			else if (bST->Data < x)//@x > t->Data
				bST = bST->Right;/*���������ƶ���������*/
			else
				return  bST;
 		}
		return nullptr;
	}
	template<typename LessCmp>
	Position findOf(Position bST, T const&x, LessCmp less){
		while (bST){
			if (less(bST->Data, x))
				bST = bST->Right;/*���������ƶ���������*/
			else if (less(x, bST->Data))
				bST = bST->Left;
			else
				return  bST;
		}
		return nullptr;
	}
	/*����Insert��x�������������bST�����ؽ�����ĸ����ָ��*/
	virtual Position Insert(Position bST, Element const&x){
		if (empty(bST))//���� ����һ��δʹ�ý��
			bST = nodeCreater(x);
		else if (x < bST->Data)
			bST->Left = Insert(bST->Left, x);
		else if (bST->Data < x)//@x > bST->Data
			bST->Right = Insert(bST->Right, x);
		else {
			BinTree<T>::repetitionInsert(bST);
		}
		return bST;
	}
	/*�Ӷ���������bST��ɾ��x,�����¸�; ��x��������tag->!tag ����ԭ����*/
	virtual Position Delete(Position bST, Element const&x, bool &tag){
		Position del = bST;
		/*�׶�һ:���Ҷ�ӦԪ��*/
		if (!bST)//δ�ҵ�ƥ��Ԫ�� ɾ��ʧ��
			tag = false;//@���Խ�� ԭ���� tag=false; tag = !tag;
		else if (x < bST->Data)
			bST->Left = Delete(bST->Left, x, tag);//�������ڵ���ܻᷢ���仯
		else if (bST->Data < x)//@x > t->Data
			bST->Right = Delete(bST->Right, x, tag);
		/*�׶ζ�:ɾ��ƥ��Ԫ��*/
		else if (bST->Left && bST->Right){
			del = findMinOf(bST->Right);//������������С(�����������)Ԫ�������ɾ���Ľ��
			bST->Data = del->Data;
			/*�׶���:ά�ֽṹ*/
			bST->Right = Delete(bST->Right, del->Data, tag);//ɾ������������СԪ��
		}
		else {//ֻ��һ�������޽��(����Left��nullptr)
			if (!bST->Left)
				bST = bST->Right;
			else
				bST = bST->Left;
			nodeEraser(del);
		}
		return bST;
	}
	//DeleteʱҪ��
	Position findMinOf(Position bST){
		while (bST && bST->Left)
			bST = bST->Left;
		return bST;
	}
	Position findMaxOf(BST bST){
		while (bST && bST->Right)
			bST = bST->Right;
		return bST;
	}
	// ˳�����(����ֹ���������)
	bool orderTraversal(BST bT, StopVister visit) {
		if (bT) {
			if (orderTraversal(bT->Left, visit))
				return true;
			if (visit(bT))//����ֵ��ʾ�Ƿ���ֹ
				return true;
			return orderTraversal(bT->Right, visit);
		}
		return false;
	}
	// �������(tips:�����ظ���Ԫ��ʱ��˳����������������������)
	bool reOrderTraversal(BST bT, StopVister visit) {
		if (bT) {
			if (reOrderTraversal(bT->Right, visit))
				return true;
			if (visit(bT))//����ֵ��ʾ�Ƿ���ֹ
				return true;
			return reOrderTraversal(bT->Left, visit);
		}
		return false;
	}
};

/*
����������ϸ�ƽ����������Ҫ��ÿ�β���ɾ����ʱ�������avl���ϸ�ƽ������ÿ�β���ɾ����ʱ��Ҫ��ת��ƽ��״̬��
��ͳ�ƽǶ���˵�����������Ҫ��һ�㣬�����Ҳ����������ϸ�֤O(logN)���������һ���������޸ĵĻ���AVL�����ܿ�һ�㣬
����˵����������������޸ĵĻ�����������+���ֲ������ܱȸ�������Ҫ�á�
*/
/*AVLƽ���� -------------------����Ϲ�ö������Ĳ���(����ɾ��)*/
template<class T>//���ؼ̳�
class AvlTree :public LinkedBinSearchTree<T> {
	typedef typename LinkedBinSearchTree<T>::BST AVLT;
	typedef typename LinkedBinSearchTree<T>::Position Position;
	typedef typename BinTree<T>::BT BT;
	using Element = T;
	using Tree::Max;
	using LinkedBinSearchTree<T>::root_;
	typedef class AvlNode : public BinTree<T>::BTNode{
	public:
		int Height = 0;//����  Data����ͬBinTree
		int getValue()override{
			return Height;
		}
		void setValue(int h)override{
			Height = h;
		}
		int getHeight()override{
			return Height;
		}
		void setHeight(int h)override{
			Height = h;
		}
		AvlNode(int tData, int height_)
			:BinTree<T>::BTNode(tData){
				Height = height_;
			}
		virtual ~AvlNode()override{
			DE_PRINTF("AvlNode����");
		}
	}*AvlTr;

	//����avl��t�ĸ߶�(��)
	int heightOf(AVLT t){//��ʵ��BinTree��depthһ�� ֻ����Ϊ�����Ч��avl������һ��height���Ҷ��㷨�����Ż�
		if (!t)
			return 0;
		else if (t->Left && t->Right)
			return Max(t->Left->getHeight(), t->Right->getHeight()) + 1;
		else if (t->Left)
			return t->Left->getHeight() + 1;
		else if (t->Right)
			return t->Right->getHeight() + 1;
		else
			return 1;
		/*չ����һ���Ч������ر�� ��������ļ���д��*/
		//return t ? Max(heightOf(t->Left), heightOf(t->Right)) + 1 : 0;
	}
	/*���ָ���:Rotation(��ת) �� Elevate����
	(��ʵ�������Ͼ���ʵ����ͬ ����û����ת����)
	��������ⷽʽ�����ԭ��: ����֪���ڷ���Ч��; ����֪���ڷ�������
	���ֵ���������Ӧ�ò�ͬ�ĸ�����ͺͼ������������ì��
	�����˺ܾ��������������(��ת����̫����� ���������Ҿ�������ν��������ʵ������ ���Ҿ�������תҲ���Ͳ�ͨ)
	����ԭ���㷨һ���µĽ��� �ﵽ������������Ч��*/

	/*�������(LL) :�������ӽڵ�B��������A��,A��ΪB���������ϵ��ӽ�� �������� �����¸�B*/
	Position leftElevate(Position A){
		Position B = A->Left;
		A->Left = B->Right;
		B->Right = A;
		A->setHeight(Max(heightOf(A->Left), heightOf(A->Right)) + 1);
		B->setHeight(Max(heightOf(B->Left), A->getHeight()) + 1);//a��height�����Ѿ����� ֱ�ӻ�ȡ����
		return B;
	}
	/*�Ҳ�����(RR) :�������ӽڵ�B��������A��,A��ΪB���������ϵ��ӽ�� �������� �����¸�B*/
	Position rightElevate(Position A){
		Position B = A->Right;
		A->Right = B->Left;
		B->Left = A;
		A->setHeight(Max(heightOf(A->Left), heightOf(A->Right)) + 1);
		B->setHeight(Max(heightOf(B->Right), A->getHeight()) + 1);
		return B;
	}
	/*������������������Ҳ�:�ȶ�A�����������Ҳ����� �ٶ�A���������*/
	Position LR(Position A){
		A->Left = rightElevate(A->Left);
		return leftElevate(A);
	}
	/*��������������������:�ȶ�A����������������� �ٶ�A���Ҳ�����*/
	Position RL(Position A){
		A->Right = leftElevate(A->Right);/*����A��������*/
		return rightElevate(A);
	}
	/*��x����ƽ���� �����ص������ƽ����*/
	Position Insert(Position avlT, Element const&x)override{
		if (!avlT){/*��������� ���½�һ������������*/
			avlT = BinTree<T>::nodeCreater(x);
		}
		else if (x < avlT->Data){/*������������*/
			avlT->Left = Insert(avlT->Left, x);/*avlT��ƽ������BF*/
			if (heightOf(avlT->Left) - heightOf(avlT->Right) == 2){/*���������߶�height��ƽ��*/
				if (x < avlT->Left->Data)/*LL�� ֱ���������������*/
					avlT = leftElevate(avlT);
				else/*LR�͵���LR����*/
					avlT = LR(avlT);
			}
		}
		else if (x > avlT->Data){
			avlT->Right = Insert(avlT->Right, x);
			if (heightOf(avlT->Left) - heightOf(avlT->Right) == -2){
				if (x > avlT->Right->Data)
					avlT = rightElevate(avlT);
				else
					avlT = RL(avlT);
			}
		}
		else{
			BinTree<T>::repetitionInsert(avlT);
		}
		avlT->setHeight(Max(heightOf(avlT->Left), heightOf(avlT->Right)) + 1);/*��������*/
		return avlT;
	}
public:
	AvlTree(){}
	~AvlTree(){
		DE_PRINTF("AVL����");
	}
};

//���ʽ�� @TODO ��δͨ��PTA����(��Ҫ�Ǻ�׺���ʽ����ʱ�ĸ������� �������ǰ׺���ʽ����)
class ExpressionTree : private BinTree<JCE::String>{
public:
	enum ExpressionTypeEnum{ 
		PREFIX_EXPRESSION,//ǰ׺���ʽ
		INFIX_EXPRESSION,//�б��ʽ
		POST_EXPRESSION,//��׺���ʽ
		ERROR_EXPRESSION//����ı��ʽ
	};
	//��׺���ʽ�����ı��ʽ����Ψһ
	ExpressionTree(string express, ExpressionTypeEnum type_){
		expression = express;
		type = type_;
	}
	//���ͼ�� �����ɺ�������ΪwillBe?
	bool check(ExpressionTypeEnum should, ExpressionTypeEnum willBe){
		if (type != should){
			return false;
		}
		else{
			if (should == ORDER_PREFIX_ROOT){
				int i = 0, countNum = 0, countOp = 0;
				char num[10] = { 0 };
				do{
					if (getNum(&i, expression.c_str(), num) != '\0')
						++countOp;
					else
						++countNum;
				} while (expression[i++] != '\0');
				//�������������:��ȷ�Ķ�Ԫǰ׺���ʽ�У��ÿո����Ĳ�������Ȼ�Ȳ�������һ��
				if (countNum - countOp != 1){
					type = ERROR_EXPRESSION;
					return false;
				}
				else{
					type = willBe;
					return true;
				}
			}
			else{
				_DEBUG_ERROR("��׺��ǰ׺ת����ûд");
				return false;
			}
		}
	}
	//������һ���������
	string nextOper(){
		static size_t ind = 0;
		string next;
		for (; ind < expression.size(); ind++){
			if (expression[ind] != ' ')
				next += expression[ind];
			else
				break;
		}
		ind++;
		return next;
	}
	/*
	1. ��������ɨ�贮
	��2. ������������ݹ鹹�����ڵ㣬��ǰ�������Ǹ��ڵ㣬���ݹ鹹�������ӽڵ�
	 ��3. ���������ǰ�����������
	  */
	//���ʽת��  ���������ֱ�ӵ���
	string prefixToPost(){
		string next = nextOper(), ans;
		if (!isdigit(next[0])){
			Position node = nodeCreater(next);//�ݹ鹹����
			node->Left = nodeCreater(prefixToPost());
			node->Right = nodeCreater(prefixToPost());
			postTraversal(node, [&](BT const bT){
				if (ans != "")
					ans += " " + bT->Data;
				else
					ans = bT->Data;
			});//�������ת��
			delete node->Left;
			delete node->Right;
			delete node;
			return ans;
		}
		else
			return next;
	}
	//transform
	string toPost(){
		if (type == PREFIX_EXPRESSION){
			if (check(PREFIX_EXPRESSION, POST_EXPRESSION))
				return expression = prefixToPost();
			else
				return "0";
		}
		else{
			_DEBUG_ERROR("�������ô���: ֻ����ǰ׺���׺ת��");
			return "0";
		}
	}
	double eval(){
		if (type == POST_EXPRESSION){
			if (check(POST_EXPRESSION, POST_EXPRESSION))
				return evalPostfix();
			else
				return MAX_INT32;
		}
		else if (type == PREFIX_EXPRESSION){
			toPost();
			return eval();
		}
		else if (type == ERROR_EXPRESSION)
			return MAX_INT32;
		else{
			_DEBUG_ERROR("�޷�ֱ�Ӽ�����׺���ʽ");
			return MAX_INT32;
		}
	}
	/*��׺���ʽ����*/

	/*��������ո�֮���ǲ������򷵻ظò��� �������ִ���num�в�����'\0'*/
	char getNum(int *i, char const*ori, char *num){
		int sub = 0;
		while (1){
			if (ori[*i] == ' ' || ori[*i] == '\0'){
				num[sub] = '\0';
				if (sub == 1 && !isdigit(num[0]))
					return num[0];
				else
					return '\0';
			}
			else{
				num[sub++] = ori[(*i)++];
				continue;
			}
		}
	}
	//���غ�׺���ʽans��ֵ ���Ϸ�����MAX_INT32
	double evalPostfix(){
		int i = 0;
		char c = 0;
		static char num[50] = { 0 };
		stack<double> ps;
		double po1, po2;
		do{
			if ((c = getNum(&i, expression.c_str(), num)) == '\0'){
				ps.push(atof(num));
			}
			else{
				if (ps.empty()){//�������ȱ�� ���߶���
					return MAX_INT32;
				}
				po1 = ps.top();
				ps.pop();
				if (ps.empty()){
					return MAX_INT32;
				}
				po2 = ps.top(); ps.pop();
				switch (c){
				case '+':ps.push(po2 + po1); break;
				case '-':ps.push(po2 - po1); break;
				case '*':ps.push(po2 * po1); break;
				case '/':{
							 if (po1 - 0.0 == 0){//����0����
								 return MAX_INT32;
							 }
							 else
								 ps.push(po2 / po1); break;
				}
				default:break;
				}
			}
		} while (expression[i++] != 0);
		double re = ps.top(); ps.pop();
		if (ps.empty()){
			return re;
		}
		else{//����������
			return MAX_INT32;
		}
	}
private:
	//BinTree base;
	string expression;
	//���ʽ��ǰ����
	ExpressionTypeEnum type;
};

#endif
