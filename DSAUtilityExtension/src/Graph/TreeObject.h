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
		//�������� ���Һ��ӽ����NULL
		BTNode(BTNode &rhs){
			(*this) = rhs;
			Left = Right = NULL;
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
			return Left == NULL && Right == NULL;
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

		BTNode* Left = NULL;		/* ָ�������� */
		BTNode* Right = NULL;		/* ָ�������� */
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
		lastInsertPosition = NULL;
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
			if (t->Left != NULL)
				q.push(t->Left);
			if (t->Right != NULL)
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
	Position root_ = NULL;
	int usedSize = 0;// ��Ч��Ԫ�ظ���
	Position lastInsertPosition = NULL;// ���������������ɵĽ�� (�޷�������жϲ���ɹ����)
	bool isInsert = false;// �Ƿ�ִ���˲������(�жϲ����Ƿ�ɹ�)

	// queue<Element*> freeMem;//�����ڴ�
	// Element *memoryBlock = NULL;//�ڴ�� �ɽ��������ľֲ����������� ��������ʹ���ⲿ������ڴ�

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
		if (r){
			destroy(r->Left);
			destroy(r->Right);
			delete r;
			r = NULL;
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
		if (bt == NULL)
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
		if (bt == NULL)
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
		if (bt == NULL)
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
		if (n == 0)	return NULL;

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
		if ((T1 == NULL && T2 != NULL) || (T1 != NULL && T2 == NULL))
			return false;
		// Ҷ�ӽ��
		else if (T1 == NULL && T2 == NULL)
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
		return bT == NULL;
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
	//��������� ����һ��δʹ�õĽ�� ��������δʹ�ý�� ����NULL ֻ�ܲ���ʹ��
	virtual Position nodeCreater(Element const &tData){
		lastInsertPosition = new BTNode(tData);
		isInsert = lastInsertPosition == NULL ? false : true;
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
		free(del);
		del = NULL;
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

	//���ص�����k���Ԫ��λ�� �����ڷ���NULL;(����BST���������˳����ص�:������������)
	BST findRKth(BST t, int *k){
		if (t){
			BST r = findRKth(t->Right, k);//1.<�׶�һ>һֱ�����ұ�
			if (r) return r;//��ת����: ������2���ض���(3��4)�򷵻�

			if (--(*k) == 0)//3.<�׶ζ�> ��˳�������ʼ��k�����
				return t;//�յ㷵��: ���ǵ�����k�����

			return findRKth(t->Left, k);//4.<�׶���>ǰ�涼û�з��� ˵������Ҫ��Ľ����������(����):�൱��һ���µĵ���
			//���÷���: ���ظ�1; ���ظ�4; ���ظ���ʼ����0; ������1 2(NULL ��ʵҲ��4 NULL�����ֻ����������ߵ�ͷ��û�еõ�3�ķ���) 3���ض���
		}
		return NULL;//2.�յ㷵��: ��ͷ ����NULL
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
		Position pastP = NULL;
		traversal(Tree::ORDER_REVERSE, [&](Position bt){
			if (pastP != NULL && pastP->Data.score == bt->Data.score)
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
		if (!bST)//�������壺��Ϊ�շ���NULL
		return NULL;
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
			return ((find(t, u) && find(t, v)) ? t : NULL);
		}
		return NULL;
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
	/*����x�ڶ���������bST�е�λ��; ���Ҳ����򷵻�NULL*/
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
		return NULL;
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
		return NULL;
	}
	/*����Insert��x�������������bST�����ؽ�����ĸ����ָ��*/
	virtual Position Insert(Position bST, Element const&x){
		if (!bST)//���� ����һ��δʹ�ý��
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
		else {//ֻ��һ�������޽��(����Left��NULL)
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
/*
ʹ����������Ķ�����(����ArrayBinTree)
��̬������Static Binary Tree(�����鴢��Ķ�������һ������ȫ������)
��StaBinTreeʵ�ֱ��ڷ����ڵ������±�ļ���
*/
template<class T>
class VirtualLinkedBinTree :public virtual BinTree<T>{
	typedef typename BinTree<T>::Position Position;
	using BinTree<T>::layers;
	using BinTree<T>::empty;
public:
	typedef typename BinTree<T>::BT BTS;
	// �ṹ��֪����(�����Ŀ, ���ݻ�ȡ����, �±��ȡ����, ���±�) ���ṹ�������������� ��root_���ض���֪�� ����ָ��root_���±�
	VirtualLinkedBinTree(int nSize, std::function<void(T *)> getData, std::function<void(Sub *, Sub *)> getSub, int noneSub, int rootSub = -1){
		// ��������鵱�ڴ�
		reCapacity(nSize);
		// �������Ӷ�����
		root_ = buildStructure(getData, getSub, noneSub);
		if (rootSub > 0) {
			_ASSERT_EXPR(rootSub < capacity, "ָ�����±�Խ��!");
			root_ = struA + rootSub;
		}
		usedSize = capacity - 1;
	}
	virtual ~VirtualLinkedBinTree() override{
		//free(struA);
		delete[] struA;
		// ��֤�������������
		root_ = NULL;
		DE_PRINTF("SCT����");
	}
	virtual void destroy(Position &r) override {
		root_ = struA + 1;
		usedSize = 0;
	}
	// �����layer���Ҷ������ (layerԽ�緵��-1) O(N)
	int leavesCount(int layer) const {
		// ��(level) ��layer�����Ľ�����: 2^layer layer=[0, height)
		assert(layer >= 0);
		if (layer > layers())
			return -1;
		int count = 0;
		//t, layer
		JCE::queue<JCE::pair<BTS, int>> q;
		q.push(std::make_pair(root_, 0));
		while (!q.empty()){
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
	using BinTree<T>::root_;
	using BinTree<T>::BTNode;
	using BinTree<T>::BT_NODE_LEN;
	typedef T ElementSBT;
	// Left��Right�������Һ���λ�������ڵĵ�ַ
	StructArray struA = NULL;
	// ���鳤��, ����Ԫ�ظ���(nSize+1) ����Ϊ1
	int capacity = 0;
	// �Ѵ���/��Ч ��Ԫ�ظ��� (������<==>capacity-1, ��struAβԪ�ص��±�)
	using BinTree<T>::usedSize;

	// ���쾲̬��ȫ������ (������) ����ת��ʱ���õ���root_��usedSize ������Ҫ�����ʼ��
	VirtualLinkedBinTree(int nSize) {
		reCapacity(nSize);
	}
	//ͨ�����빹�����ṹ ���ظ���� (�����ں��ӽ����±��ȡ����)
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
		_ASSERT_EXPR(0 <= sum && sum < nSize, "������������!");
		return nSize == 0 ? NULL : rt + sum;
	}
	bool full() {
		return usedSize == capacity - 1;
	}
	// �����ṹ������ (�����Ŀ)
	void reCapacity(int nSize) {
		// �Զ�������һ�����ڴ洢0���ڱ�
		++nSize;
		struA = new BTNode[nSize];
		/*
		struA = (StructArray)realloc(struA, BT_NODE_LEN*nSize);
		if (capacity == 0)//�״�����->ȫ���ʼ��(�ų���ʹ�õ�usedSize һ����0)
			memset(struA + usedSize, 0, BT_NODE_LEN*(nSize - usedSize));
		else//�ٴ�����->root_��[struA+cap, struA+cap](�ų�ԭ�е�cap)��ʼ��, struA+cap�Ѿ�������ĩβ�ĺ�һ��
			memset(struA + capacity, 0, BT_NODE_LEN*(nSize - capacity));
		*/
		capacity = nSize;
	}
	// ��ʼ��root_ 
	void iniTRoot() {
		// root_�������ڱ�(0��)
		root_ = struA + 1;
	}
	// ���������ڵĽ���� 1��Ϊroot_ 0��Ϊ�ڱ�
	int index(BTS t) const {
		/*��һ��Ԫ�ز���ʱroot_=NULL �����������Һ������ӷ���������root_ ���ο�root_=1ʱ�Ĺ�����������������*/
		return t - struA;
	}
	// ���������ڵĽ��λ��
	Position position(int sub) {
		return struA + sub;
	}
};

//˵���˾��Ǳ��������ڴ�ռ�����Ӷ����� ��������ͨ������������(û���д��������insert����)
//�������ֳ�ͻ�ĸ���ԭ������ �ҽ����ӽ��ʵ�ֵ� ������������һ���ȳɱ�׼(�ӿ�) �����Խ�����Ϊ�˿��Ա�������ת�� ���Գ�����һЩì��
//(����������ʵ�ֵĶ�̬�Ժ��ڴ��Լ �����о�̬ʵ�ֵ��������Ч��
/*
���Դ���һ��������[]���������� ����"�ṹ�ڴ�"
�����������˳�������Ļ� �������ڲ����ֶ�̬�� ���ⲿ���־�̬��
����һ����̬ʵ��������ʵ�ֺ϶�Ϊһ�� ��������Ҳ�����ԵĿ���˵�ֻص��˵����������ӽ��ĳ���:��Լ�ڴ� �Լ���δ֪������ǰ˳��ʵ�ֵ�������
��һ������Խ��:�����ݽṹ�ڲ����������ڴ����뷽�� һ�����ڴ���������ڴ������� һ�������ⲿ����(��һ��ע�������ע����һ�����ڴ� ����ͨ����ע���struA(ָ������)�ķ��ʴﵽ��ʱ�������)
����ȫ������,ʼ����һ������û�н��:˳���ɾ��ĳ�����ݺ�������¼������ ������Ҫ���ڴ�ʱ���֪�������п��ڴ� ��������Ҫ����
�ðɿ��Խ�ɾ�����ڴ�ָ����ĩβ�ڴ�ָ�뽻��(�������õ���ָ������Ķ���ָ�� �൱���õ�������ָ��(��)) ��,������ͨ������ʽ����������ȫû������ ���������ϵͳ�ڴ��������ʽ?
*/
/*��̬����������Static Binary Search Tree*/
/*SequentialBinTree˳�򴢴�Ķ�����*/
template<class T>//��̳�(virtual����д���м�) �����̳������μ̳е�����µĻ����Ա�ĳ�ͻ
class VirtualLinkedBinSearchTree :public VirtualLinkedBinTree<T>, public virtual LinkedBinSearchTree<T> {
public:
	~VirtualLinkedBinSearchTree() override{
		DE_PRINTF("SCBT����");
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

	VirtualLinkedBinSearchTree(int nSize) :VirtualLinkedBinTree<T>(nSize){}
	Position nodeCreater(Element const &tData)override {
		if (full())
			return NULL;
		struA[usedSize + 1].Data = tData;//0��Ԫ�������ڱ�
		++usedSize;
		/*
		isInsert = lastInsertPosition == NULL ? false : true;  bug
		BinTree<T>::lastInsertPosition = struA + usedSize + 1;
		*/
		return struA + usedSize + 1;
	}
	void nodeEraser(Position &del)override{
		// 0�����ʼ״̬ ֻΪ�˱�ʶ�� ���������ô�
		// malloc��Ӧ��memeset��ʼ��free�ͷ� new�Զ���ʼ�� ��ֵ��ʼ�� delete�ͷ�
		// del->Data = 0; // {}
		//��̬�����ɾ������ʵ��ɾ��
		del->Left = del->Right = NULL;
		del = NULL;
		--usedSize;
	}
};

/*��ȫ���������� ʹ�����鹹�� ʵ�ֶ���ľ�̬���ֲ��� ����Ҫ��̬��ɾ���� ��Ҫ����ת��Ϊ����������*/
/*����ֲ������������ ���ֲ��ҵ������������ ����CBST��Ҫ�������� ����������䱾����������в��������(����Ԫ���൱��������ֵ)*/
/*����ʵ�ý�˳�򴢴�Ķ�������������ȫ������ ֻ��˵��ȫ��������˳��洢������*/
template<class T>
class CompleteBinSearchTree :public VirtualLinkedBinSearchTree<T>{
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

	/*����StaBinSearchTree�������Ĺ�ģ (�ܽ����)*/
	int getLeftScaleL(double n){
		/*2^h - 1 + x = n*/
		int h = (int)log2(n + 1);/*����ȡ��*/
		int x = (int)(n + 1 - pow(2.0, h));/*���²㵥���Ľ����*/
		return (int)pow(2.0, h - 1) - 1 + Min(x, (int)pow(2.0, h - 1));
	}
	Position getLeftChild(Position t){
		int i = index(t);
		return 2 * i <= usedSize ? struA + 2 * i : nullptr;
	}
	Position getRightChild(Position t){
		int i = index(t);
		return 2 * i + 1 <= usedSize ? struA + 2 * i + 1 : nullptr;
	}
	Position getParent(Position t){
		int i = index(t) / 2;
		return i > 0 ? struA + (i) : nullptr;//1���Ǹ����û�и����(1/2==0)
	}
	Position getSibling(Position t){
		int i = index(t);
		//1���Ǹ����û���ֵ�
		return i > 1 ?
			i % 2 == 0 ? struA + i + 1 : struA + i - 1
			: nullptr;
	}

	//[��ȫ������]�����й���  ����Դ����(fitst, last) Ŀ������struA		func(A, A+n, 0);
	Position buildComplete(T *first, T *last, int TRoot = 1){
		int n = last - first;
		if (n == 0)
			return NULL;
		int nl = getLeftScaleL(n);
		struA[TRoot].Data = first[nl];/*first��0��ʼ��nl������������*//*��������1��ʼ����±���2i*/
		struA[TRoot].Left = buildComplete(first, first + nl, 2 * TRoot);
		struA[TRoot].Right = buildComplete(first + nl + 1, last, 2 * TRoot + 1);
		return struA + TRoot;
	}
	/*��ȫ�������������(˳�������������)*/
	template<class Fun>
	void levelTraversal(BT bT, Fun visit){//�޷�override ������Ӧ�ø�����
		if (bT == NULL)return;
		FOR(i, 0, usedSize){//bT����ʱroot_������struA(root_ = struA+1)
			if (!visit(bT + i))
				return;//��ֹ
		}
	}
	// �����ڶѵĳ�ʼ��
	CompleteBinSearchTree(int nSize) : VirtualLinkedBinSearchTree<T>(nSize) {}
public:
	/*sort(iniA, iniA + n, less<T>());
	**��ת����:sort(iniA, iniA + n, greater<T>()); ��ȫ�������������С����֧����С�Ҵ����ľ���ת ��ͨBinTree֧��*/
	//ʹ�����鹹��Ψһ����ȫ������ (������ȫ������������ɾ û��usedSize��capacity֮��) tips::���������� ��Ϊ������
	CompleteBinSearchTree(int nSize, T *iniA): VirtualLinkedBinSearchTree<T>(nSize){
		assert(iniA != nullptr && nSize > 0);
		root_ = buildComplete(iniA, iniA + nSize);
		this->usedSize = nSize;
	}
	//ֱ�Ӱ�����copy����̬������
	/*CompleteBinSearchTree(T *iniA, int nSize): VirtualLinkedBinSearchTree<T>(nSize){
		assert(iniA != nullptr && nSize > 0);
		root_ = struA + 1;
		for (int i = 0; i < nSize; ++i)
			root_[i].Data = iniA[i];
		usedSize = nSize;
	}*/
	~CompleteBinSearchTree() override{
		DE_PRINTF("CBT����");
	}

	/*Position find(Element x){
		return NULL;//not impl
	}*/
	//�����������Ĺ������� Խ�緵���ڱ�(0�Ž��)
	Position ancestor(int i1, int i2){
		if (i1 > usedSize || i2 > usedSize || i1 < 0 || i2 < 0)
			return position(0);
		if (i1 == i2)
			return position(i1);
		else if (i1 > i2)
			ancestor(index(getParent(position(i1))), i2);//return �Ӳ���ֻҪ�����з��ض��᷵�� Ч��һ��
		else
			ancestor(i1, index(getParent(position(i2))));//return
	}
};

/*	make_heap:		����ָ���ĵ����������Լ�һ����ѡ�ıȽϺ�����������һ��heap. O(N)
push_heap:		��ָ����������һ��Ԫ�ز��뵽heap��. O(logN)
pop_heap:		����heap��Ԫ��, �������������ĩβ. O(logN)
sort_heap��		�������㷨��ͨ��ͨ����������pop_heap��ʵ��. N*O(logN)
C++11�����������³�Ա��
is_heap:		�жϸ��������Ƿ���һ��heap. O(N)
is_heap_until:	�ҳ������е�һ��������heap������λ��. O(N)*/
/*C++��heap���㷨����ʽ�ṩ��Ҫʹ���⼸��api��Ҫ#include <algorithm>*/
/*���㷨���������ʽʵ��
����ŷǻ�������ʱ �����ظ�ֵ��
�����ָ��ʱ ��Ҫ���������ڱ��Լ��Լ�������ڴ� ����ʹ�ò��빹��*/
/*�� �����(С)��, ��ȫ������*/
//��������ǳ��Ѷ�û����ѵĻ� �Ÿ���O(N*logN) ��ʱ���O(N) ����O(1)
template<class T>
class Heap : public CompleteBinSearchTree<T>{
	typedef T Element;
	typedef typename LinkedBinSearchTree<T>::Position Position;
	using BinTree<T>::root_;
	using VirtualLinkedBinTree<T>::struA;
	using VirtualLinkedBinTree<T>::usedSize;
	using VirtualLinkedBinTree<T>::capacity;
	using VirtualLinkedBinTree<T>::full;
	using VirtualLinkedBinTree<T>::empty;
	using CompleteBinSearchTree<T>::getLeftChild;
	using CompleteBinSearchTree<T>::getRightChild;
	using CompleteBinSearchTree<T>::getParent;
	using CompleteBinSearchTree<T>::getSibling;

	//ʹ��less<T> greater<T> �� �ȽϷ������� ����ͬ��������: ����û��ͬһ����ʾ����(��������()�ĸ���)�����ڴ��С�ѵļ�ʱ�ع�
	int(*cmper)(const T &min, const T &max) = NULL;
public:
	/*(�ѵĴ�С С�ڱȽϷ��� �����ʼ���������� �����е�Ԫ�ظ���)  ע��: �����鹹��Ķ�������ʼ���Ķ� �Ľṹ��һ��*/
	Heap(int sizeHeap, T *iniA = NULL, int nIniA = 0)
		: CompleteBinSearchTree<T>(sizeHeap) {
		// û����ЧԪ��ҲҪ��root_��ֵ
			root_ = struA + 1;
			usedSize = nIniA;
			// Ϊÿ��Ԫ�ظ���ʼȨֵ
			for (Position t = struA + 1; t < root_ + nIniA; ++t) {
				// ��T���Ͳ��ǻ������� ��Ҫ���ظ�ֵ��
				t->Data = iniA[t - root_];
				// ֻҪûԽ�������->����ʱ��usedSizeΪ���������ı�־ �������ӽ���Ƿ�Ϊ��?
				linkToChildren(t);
			}
		}
	~Heap()override{
		DE_PRINTF("Heap����");
	}

	Element pop(){
		if (empty()){
			puts("���ѿ� �޷�ɾ��");
			return NULL;
		}
		if (notBuild()){
			puts("δ����");
			return false;
		}
		Element Item = struA[1].Data;/*ȡ���������ص�ֵ*/
		/*ì������:Size--ɾ������βԪ�� ���Ǹ��ݶѵĶ�������Ӧ��ɾ��1��Ԫ��
		��βԪ���滻����ʱ��pԪ������ �������ڲ��� ��ʵ����ɾ������1��Ԫ��*/
		PercoDown(1, "ɾ��");//ע��::������usedSize--��
		unlinkToParent(struA + usedSize);
		return Item;
	}
	bool push(Element Item){
		int i;
		if (full()){
			puts("������ �޷�����");
			return false;
		}
		if (notBuild()){
			puts("δ����");
			return false;
		}
		i = ++usedSize;/*iָ��[�����]�������һ��Ԫ�ص�λ��*/
		for (; cmper(struA[i / 2].Data, Item) < 0; i /= 2)
			struA[i].Data = struA[i / 2].Data;//�����һ���к��ӵĽ�㿪ʼ ���Ϲ��˽��
		struA[i].Data = Item;/*����*/
		linkToChildren(struA + usedSize);
		return true;
	}
	/*����һ���Ѷ�������ݵ���Ҫ������������С����С��(���¶��ϵ���)
	��[���һ���ӽڵ�ĸ��ڵ� �� ������һ���ж��ӵĽ��]
	ÿ�δ����ҽ������һ��"��"�Ľ�������˲�����ʼ���������ڵ�1*/
	/*���Զ��ع� ���Ӷ�O(N)*/
	//��С�� ����: maxCmp(����)�ڱ�(��Сֵ)	ע��: ����̫С���������:����MAX_INT32/2����
	//���� ����: mixCmp(С��)�ڱ�(���ֵ)
	void build(T sentry, int(*cmper_)(const T &min, const T &max)){
		cmper = cmper_;
		struA[0].Data = sentry;
		for (int i = usedSize / 2; i > 0; i--){
			PercoDown(i, "����");
		}
	}
protected:
	//���� insert����
	void percolateUp(int i, Element *struA, int usedSize){
		Element Item = struA[i];//iָ�����������Ԫ�ص�λ��
		for (; struA[i / 2] - Item > 0; i /= 2)
			struA[i] = struA[i / 2];//�����˽��ĸ���㿪ʼ ���Ϲ��˽�� �����������ӽ�������
		struA[i] = Item;
	}
	//���� :build����(��ĩ��㿪ʼ���� �ȼ��ڴ����һ������㿪ʼ����) delete����
	void percolateDown(int Parent, Element *struA, int usedSize){
		int Child;
		Element x = struA[Parent];//ȡ����Ҫ���˵�ֵ
		for (; 2 * Parent <= usedSize; Parent = Child){
			Child = 2 * Parent;//�������==usedSize; ���Ҷ��Ӳ�����
			if (Child != usedSize && struA[Child] - struA[Child + 1] > 0)
				++Child;//ѡȡ���Ҷ�����С��һ��
			if (x - struA[Child] < 0)//����Ԫ��С��Ŀ��ʱ��ֹ ����ʱ����
				break;
			else//����������<==>��temp����
				struA[Parent] = struA[Child];
		}
		struA[Parent] = x;
	}

	/*���˺��� ��H����H->Data[p]Ϊ�����Ӷѵ���Ϊ������С����С��*/
	void PercoDown(int Start, char const *Order){
		int Parent, Child;
		Element x;
		/*���ˣ�ȡ���Լ�          ���Լ���ʼ�ҵ�һ�����ʵ�λ��*/
		/*ɾ����ȡ��βԪ�� Size-- �ӶѸ���ʼ*/
		int p = strcmp(Order, "ɾ��") == 0 ? usedSize-- : Start;
		x = struA[p].Data;/*ȡ����Ҫ���˵�ֵ*/
		for (Parent = Start; 2 * Parent <= usedSize; Parent = Child){
			Child = 2 * Parent;/*�������==usedSize; ���Ҷ��Ӳ�����*/
			if (Child != usedSize && cmper(struA[Child].Data, struA[Child + 1].Data) < 0)
				Child++;/*ѡȡ���Ҷ����д��С��һ��*/
			if (cmper(x, struA[Child].Data) >= 0)
				break;
			else/*����������<==>��temp����*/
				struA[Parent].Data = struA[Child].Data;
		}
		struA[Parent].Data = x;
	}
	//�����Ƿ��Ѿ�����
	bool notBuild(){
		return struA[0].Data == NULL;
	}
	//�ӽ������:���ӵ�ǰ���ĺ��ӽ��
	void linkToChildren(Position parent){
		parent->Left = getLeftChild(parent);
		parent->Right = getRightChild(parent);
	}
	//�ӽ�����:������׽��Ժ��ӽ�������
	void unlinkToParent(Position delChild){
		Position parent = getParent(delChild);
		//if (!empty(parent))
		if (parent != nullptr)
			linkToChildren(parent);//��usedSize�Ѿ�--��ôԽ����Чvalid���ӽ��ᱻ��NULL
	}
};

/*
û�ж�Ϊ1�Ľ��
n��Ҷ�ӽ���huffman���Ľڵ���=2n-1
*/
/*��Ȩ·����С�Ķ�������Ϊ���Ŷ�������huffman��(�ṹ����Ψһ)
huffman�����жϱ�׼:����ǰ׺������ <==>���ű���(WPL��textLen��С) && ǰ׺��(���벻���ǳ����ǰ׺)*/
template<class T>
class HuffmanTree : public BinTree<T>{
protected:
	using BinTree<T>::root_;
	//������Ҫ�ݹ��ӽ��ĵط��ض��õ�Left-Rightֻ����BinTree��Position-��getValue()���ڵı�Ҫ��
	using Position = typename BinTree<T>::Position;
	using Element = typename BinTree<T>::Element;
	typedef class HuTrNode : public BinTree<T>::BTNode{
	public:
		int Weight = 0;//Ȩ��  Data��Ϊ �����ַ�
		int getValue()override{
			return Weight;
		}
		void setValue(int w)override{
			Weight = w;
		}
		HuTrNode(int tData, int weight_)
			:BinTree<T>::BTNode(tData){
				Weight = weight_;
			}
		~HuTrNode()override{
			DE_PRINTF("HuTrNode����");
		}
	}*HuTr;//��������
	HuTr huffRoot = NULL;
public:
	/*���������Ĺ��� ��С��ʵ�� (�ַ�����, Ȩ������(һ����Ƶֵfreq), ���й�����С, hufҶ�ӽ����)*/
	HuffmanTree(T* character, int *freq, int nHuf) {
		Heap<HuTr> h = Heap<HuTr>(nHuf);
		HuTr sentry = new HuTrNode(0, -MAX_INT32 / 2);//û�и�ֵ���� ֻ��new?
		//lambda���ʽ, �������� p345
		h.build(sentry, [/*�����б�*/](const HuTr &min, const HuTr &max){
			return max->Weight - min->Weight;
		});
		for (int i = 0; i < nHuf; ++i)	h.push(new HuTrNode(character[i], freq[i]));
		/*��usedSize-1�κϲ� ÿ�ν�Ȩֵ��С���������ϲ� ��������;��usedSize��仯 ������n*/
		while (nHuf-- > 1){
			HuTr t = new HuTrNode(0, 0);//������Ϊcharʱ ��Ч���Ϊ'\0'�������(�Զ�ʡ����Ч�ķ�Ҷ���) ������Ҫ��д��������
			t->Left = h.pop();
			t->Right = h.pop();
			t->setValue(t->Left->getValue() + t->Right->getValue());/*������Ȩֵ*/
			h.push(t);
		}
		huffRoot = h.pop();
		root_ = huffRoot;
		delete sentry;
	}/*Ƶֵ����freq[0]����Ȩֵ���� 0<=freq[i])��Ȩֵ i[1,*freq O(N log(N))*/
	//��������ӹ���huffman��

	~HuffmanTree()override{
		DE_PRINTF("HuffmanTree����");
	}
	//���������ֱ������endǰ�����ַ�
	static void flushInput(char end){
		char c;
		while ((c = getchar()) != end);
		ungetc(c, stdin);
	}
	//����������Ļ�����ո񲢷��طǻ��пո���Ǹ��ַ� ���ɼ���(Invisible)
	static void flushBlanckLF(){
		char c;
		while ((c = getchar()) == '\n' || c == ' ');
		ungetc(c, stdin);
	}
	//���س���ΪmaxLen��char���ַ��� ��get = true�Զ�����gets(str);
	static char* newSentence(int maxLen, bool get = false, char end = '\n'){
		//str.resize(maxLen);//str��length() usedSize����maxLen �������� ����������new char �ֻص����ڴ���������
		//cin.getline(&str[0], maxLen, end);
		char* str = (char*)malloc(sizeof(char)* maxLen);
		memset(str, 0, sizeof(char)* maxLen);
		if (get) gets(str);
		return str;
	}
	//���ر��볤��(�����ִ�)
	static int codeLen(char *code){
		return strlen(code);
	}
	static int codeLen(JCE::String const&code){
		return code.length();
	}
	/*tips:
	depth = codeLen+1 <-> depth(root_)=1; (�����, ���ϵ�depth����)
	depth = codeLen  <--> depth(root_)=0
	(����: �ı�����=Ȩ��*���볤��, WPL=Ȩ��*(���볤��+1))
	*/
	//�����ı���WPL
	static int wpl(JCE::ArrayList<char*> &s, int *freq){
		int textLen, i, n = s.size();
		for (textLen = i = 0; i < n; ++i){
			textLen += freq[i] * (codeLen(s[i]) + 1);
		}
		return textLen;
	}
	//����һ�ױ�����ı����� = freq*codeLen
	static int textLen(JCE::ArrayList<char*> &s, int *freq){
		int textLen, i, n = s.size();
		for (textLen = i = 0; i < n; ++i){
			textLen += freq[i] * codeLen(s[i]);
		}
		return textLen;
	}
	//����ı��Ƿ�ǰ׺�� //string const codes[]-> ����ArrayList��Ч�����?
	static bool isPrefixCodes(JCE::ArrayList<JCE::String> const &codes) {
		int n = codes.size();
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j != i && j < n; j++) {
				if (codes[j].find(codes[i]) == 0)//ǰ׺��ͻ
					return false;
			}
		}
		return true;
	}
	//���ر�huffman����Ĵ�Ȩ·������ [·������]:�Ӹ���㵽Ŀ�����뾭�����ܱ���(��֧�� )(�����Ľ������-1)
	int wpl(){
		return WPL(huffRoot, 1);
	}
	//���ر�huffman�����(���)�ı����� (�ռ�ռλ = textLen*1)
	int textLen(){
		return textLen(huffRoot, 1);
	}
	//�ж��Ƿ����ű���
	bool isOptimalCoding(JCE::ArrayList<JCE::String> &codes, int *freq){
		//Ҳ���ԱȽ�wpl
		int textLen, i, len, n = codes.size();
		for (textLen = i = 0; i < n; ++i){
			len = codeLen(codes[i]);
			if (len > n - 1)/*n���������볤��Ϊn-1*/
				return false;//Ч������ֱ�Ӽ��㲢�Ƚ�textLen
			textLen += freq[i] * len;
		}
		return textLen == this->textLen() ? true : false;
	}
	//���ر�huffman�����ı��Ŀռ�ռ��=codeLen*sizeof(�ַ�)
	int memory(){
		return textLen()*sizeof(T);
	}
private:
	void destroy(HuTr &t){
		Position tmp = t;
		//����ֱ�ӵ��� �����ò��ܴ�������
		BinTree<T>::destroy(tmp);
		t = NULL;
	}
	//���ظ����ı����ǰ׺������ �������ı��벻��ǰ׺�뷵�� NULL ����Ƶֵ��Ƶֵ�������� ����ȫ����1(��δ֪bug ���ж�һ�ױ����Ƿ�huffman����)
	HuTr buildPreCodeTree(JCE::ArrayList<JCE::String> const &codes, int *freq = NULL){
		int n = codes.size(), sub = 0;
		HuTr rt = new HuTrNode(0, 0);
		for (int i = 0; i < n; i++){
			int len = codeLen(codes[i]);
			Position t = rt;
			for (int j = 0; j < len; j++){
				if (codes[i][j] == '1'){//1����
					if (!t->Left)//Ϊ�� �����
						t->Left = new HuTrNode(0, 0);
					else if (t->getValue() != 0){//��Ϊ���жϼ������л�����ֹ
						destroy(rt);
						return NULL;
					}
					t = t->Left;
				}
				else{//0����
					if (!t->Right)
						t->Right = new HuTrNode(0, 0);
					else if (t->getValue() != 0){//����;��������Ȩ��� ->�ñ��벢��ǰ׺��
						destroy(rt);
						return NULL;
					}//else do nothing
					t = t->Right;
				}
			}
			if (!t->isLeave())//������Ҷ��� ��ز���ǰ׺��
				return NULL;
			t->setValue(freq == NULL ? 1 : freq[sub++]);//��Ȩ
		}
		return rt;
	}
	//��Ȩ·������(weightted path length)=sum(weight*depth)
	int WPL(Position t, int depth){
		if (!t->Left && !t->Right)
			return t->getValue()*depth;
		else/*����һ������������*/
			return WPL(t->Left, depth + 1) + WPL(t->Right, depth + 1);
	}
	//(���)�ı����� = freq[i]*codeLen
	int textLen(Position t, int depth){
		if (!t->Left && !t->Right)
			return t->getValue()*(depth - 1);//tips:depth(root_) = 1
		else/*����һ������������*/
			return textLen(t->Left, depth + 1) + textLen(t->Right, depth + 1);
	}
	//���ؽ�����дӢ�ľ��ӵ�Ƶ������ ��Ϊmap?
	JCE::ArrayList<int> frequency(JCE::String sentence){
		JCE::ArrayList<int> freq;
		freq.resize(26);
		for (unsigned i = 0; i < sentence.length(); ++i){
			if (sentence[i] == '_')
				freq[26]++;
			else{
				freq[sentence[i] - 'A']++;
			}
		}
	}
	//����һ��������дӢ����ĸ���»��ߵľ��ӵĹ�����������ռ�ڴ�,��ͨ����memory = sentence.length()*8
	int huffmanMemory(JCE::String sentence){
		static const int MAX_FREQ_SIZE = 128;
		int freq[MAX_FREQ_SIZE];
		memset(freq, 0, MAX_FREQ_SIZE * sizeof(int));

		for (unsigned i = 0; i < sentence.length(); ++i){
			if (sentence[i] == '_')
				freq[26]++;
			else{
				freq[sentence[i] - 'A']++;
			}
		}
		std::priority_queue<int, JCE::ArrayList<int>, JCE::greater<int> > q;

		for (int i = 0; i < MAX_FREQ_SIZE; ++i){
			if (freq[i] != 0)
				q.push(freq[i]);
		}
		int tmp = 0, mem = 0;
		while (q.size() > 1){
			tmp = q.top(); q.pop();
			tmp += q.top();
			q.pop();
			q.push(tmp);
			mem += tmp;//��ʵ���ǰ�ԽС��Ƶ�ʷ�����Ӽ��Σ�Խ���Ƶ���ټӼ���
		}
		return mem == 0 ? sentence.length() : mem;
	}
};

/*����ʽhuffman*/
class ArrayHuffman {
	struct HTNode {
		//Ȩ��
		int weight;
		//�����
		int parent;
		//����
		int lchild;
		//�Һ���
		int rchild;
		//ֻ����sub�����������Ƿ���ʹ�õ��ж� ע��˴�������memset�ĳ�ʼ���ɺ�
		bool isUsed;
	};
public:
	typedef HTNode *HuffmanTree;
	typedef char ** HuffmanCode;
protected:
	static void swap(int &s1, int &s2) {
		int tmp = s1;
		s1 = s2;
		s2 = tmp;
	}
	static void swapC(char &s1, char &s2) {
		char tmp = s1;
		s1 = s2;
		s2 = tmp;
	}
	//��1��upbound���ҳ�fatherΪ0�Ľڵ㸳��s1,s2,��Ϊ�˱�֤��Ψһ������s1�Ľڵ���С��s2��
	static void SelectTwoMin(int upbound, HuffmanTree ht/*��tm�Ǹ�����!!!*/, int &s1, int &s2) {
		s1 = s2 = 0;
		//�ҳ�weight��С��������sub
		for (int i = 1; i < upbound; ++i) {
			if (ht[i].parent == 0) {
				if (ht[s1].weight > ht[i].weight)
					s1 = i;
			}
		}
		for (int i = 1; i < upbound; ++i) {
			if (ht[i].parent == 0) {
				if (ht[s2].weight > ht[i].weight && i != s1)
					s2 = i;
			}
		}
		if (s1 > s2) {
			swap(s1, s2);
		}
	}
	//���Ҳ������������������ĳ��Ȩֵ�Ľ����±� �����ڷ���0
	static int sub(HuffmanTree ht, int wei, int n) {
		for (int i = 1; i < n; ++i) {
			if (!ht[i].isUsed && ht[i].weight == wei) {
				ht[i].isUsed = true;
				return i;
			}
		}
		return 0;
	}
public:
	//�����������ht �� �������������hc wΪȨֵ(Ƶ��)���� nΪȨֵ���� (��Ҫ�ֶ�free)
	static void HuffmanCoding(HuffmanTree &ht, HuffmanCode &hc, int *w, int n) {
		// �����������(α)
		ht = (HuffmanTree)malloc(sizeof(HTNode)*(2 * n));
		memset(ht, 0, (sizeof(HTNode)*(2 * n)));
		ht[0].weight = 1001;
		for (int i = 0; i < n; ++i) ht[i + 1].weight = w[i];
		//��n-1�κϲ� ÿ�ν�Ȩֵ��С���������ϲ�
		int size = n;
		for (int i = 1; i < n; ++i) {
			++size;
			//"pop" and "push"
			SelectTwoMin(size, ht, ht[size].lchild, ht[size].rchild);
			//������Ȩֵ
			ht[size].weight = ht[ht[size].lchild].weight + ht[ht[size].rchild].weight;
			ht[ht[size].lchild].parent = ht[ht[size].rchild].parent = size;
		}
		// �������
		hc = (HuffmanCode)malloc(sizeof(char*)*n);
		memset(hc, 0, (sizeof(char)*n));
		for (int i = 0; i < n; ++i) {
			hc[i] = (char*)malloc(sizeof(char)* n);
			memset(hc[i], 0, sizeof(char)* n);
			//n�������볤��n - 1+'\0' = n �±�cΪ��������±�sizeʱ��ֹ
			for (int c = sub(ht, w[i], size), codeIndex = 0; c != size; c = ht[c].parent) {
				int parent = ht[c].parent;
				//hc[i][codeIndex++] = ht[parent].lchild == c ? '0' : '1';
				//����������Ƶ�� < ������Ƶ�� ����Ϊ0
				hc[i][codeIndex++] = ht[parent].lchild == c ? '1' : '0';
			}
		}
		for (int i = 0; i < n; ++i) {
			//��ÿ��������ת
			for (int lhs = 0, len = strlen(hc[i]); lhs < len / 2; ++lhs) {
				int rhs = len - lhs - 1;
				swapC(hc[i][lhs], hc[i][rhs]);
			}
		}
	}

	//���ر��볤��
	static int codingLen(ArrayHuffman::HuffmanCode const hufCode, int n, int *weightList) {
		int sum = 0;
		for (int i = 0; i < n; ++i) {
			sum += weightList[i] * strlen(hufCode[i]);
		}
		return sum;
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
