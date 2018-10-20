#ifndef _TREEOBJECT_H
#define _TREEOBJECT_H

#include"stdafx.h"
/*
*����:
*���Ͼ�ֻ������ �ʺ�����ɾ��Ķ�̬���� ���ʺ�����������
*��ֻ������PTA�����ݽṹ�γ̻���˵������ƫִ�뷨���������� �Ժ���ƾ������������뷨Ҳ���������Ļ����뻷����
*���������ೣʶ�Ե�����ٹ̵���ʶ����
*��[���ṹ]����ʶ���������ڳ���
*��[C++�ı�׼��]ʹ�� �Լ�[ģ����]{��ģ�� ����ģ��} ����һЩ�ĵ�(C++�߼����)
*��[���������]���˼�������;���
*��[�㷨]���ӵĺ�����һ������(����O ���Ԧ� ���ަ�)
*��һ��ȫ��������̳й�ϵ�����������໥���ת���ܷ��㲻��ʱ(������Ч�ʱȲ��ϵ���ʵ�ֵ�)
*/
/*�� ������(�ӿ���)*/
class Tree{
public:
	//������˳������ö��
	enum TraversalOrderEnum{
		ORDER_PREFIX_ROOT, //�ȸ���
		ORDER_INFIX_ROOT, //�и���
		ORDER_POST_ROOT, //�����
		ORDER_LEVEL, //����
		ORDER, //˳��(�����ȸ��� ��������ֹ����)
		RE_ORDER, //����
		NONE_ORDER	//��: δָ��
	};
	int Min(int a, int b){
		return a < b ? a : b;
	}
	int Max(int a, int b){
		return a > b ? a : b;
	}
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
		//��ȡ����������ֶ�:weight
		virtual int getValue(){
			return 0;
		}
		//�������������ֶ�
		virtual void setValue(int){}
		//��������Ĭ�Ϲ��캯��
		BTNode(Element const&data_) : Data(data_){}
		/*BTNode(Element const&data_){
			Data = data_;
		}*/
		//�������� ���Һ��ӽ����NULL
		BTNode(BTNode &rhs){
			(*this) = rhs;
			Left = Right = NULL;
		}
		virtual ~BTNode(){
			DEPRINTF("BTNode����");
		}
		//��ֵ Ĭ�ϵ�ȫ��ֵ
		BTNode& operator=(const BTNode&) = default;
		int height(){
			return depthOf(this);
		}
		//Ҷ�ӽ���ж�
		bool isLeave(){
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

	} *BT, *Position;
	BinTree(){}
	//�������� ����ֻ��������(�����Ĵ��ݱ������ǿ�������)
	BinTree(const BinTree &rhs){
		DEPRINTF("BT��������");
		destroy(root);
		assignment(root, rhs.root);
	}
	/*���й��췽��*/
	//ȱʡ�ı������з���Ԫ�ظ���
	BinTree(Element const *preOrder, Element const *inOrder, int n){
		prefInBuild(preOrder, inOrder, root, n);
	}
	BinTree(int n, Element const *inOrder, Element const *postOder){
		postInBuild(root, inOrder, postOder, n);
	}
	/*6
	Push 1
	Push 2
	Push 3
	Pop
	Pop
	Push 4
	Pop
	Pop
	Push 5
	Push 6
	Pop
	Pop
	output:3 4 2 6 5 1
	BinTree<T> t = BinTree<T>(
	[](char *ORDER){ return (bool)~scanf("%s", ORDER); },
	[](T* num){scanf("%d", num); });*/
	//��ջ����ʽ���� (��ջ������ȡ����, ������ݻ�ȡ����)
	BinTree(bool(*getOrder)(char *), void(*getData)(T*)){
		vector<T> preOrder, inOrder;
		stack<T> s;
		char str[5];
		while (getOrder(str)) {
			int len = strlen(str);
			if (len == 4) {//Push
				int num;
				getData(&num);
				preOrder.push_back(num);//��һ������ʱ(����)
				s.push(num);
			}
			else if (len == 3){//Pop
				inOrder.push_back(s.top());//�ڶ�������ʱ(����)
				s.pop();
			}//else ����
		}
		//if(preOrder.size() != inOrder.size());//����
		prefInBuild(preOrder, 0, inOrder, 0, root, preOrder.size());
	}
	//destructor
	virtual ~BinTree(){
		destroy(root);
		DEPRINTF("BT����");
	}

	virtual void clear(){
		destroy(root);
		lastInsertPosition = nullptr;
		size_ = 0;
	}

	Element getRootData(){
		return root->Data;
	}
	const Position getRoot(){
		return root;
	}
	//�ж϶������Ƿ�Ϊ��
	bool empty(BT bT){
		return bT == NULL;
	}
	//��������
	int height(){
		return depthOf(root);
	}

	template<class Fun>
	//��(2*N)     Tree::ORDER, bool visit(Bintree<T>::BT Node)
	void traversal(TraversalOrderEnum type, Fun visit){
		if (type == ORDER_PREFIX_ROOT)
			preTraversal(root, visit);
		else if (type == ORDER_INFIX_ROOT)
			infTraversal(root, visit);
		else if (type == ORDER_POST_ROOT)
			postTraversal(root, visit);
		else if (type == ORDER_LEVEL)
			levelTraversal(root, visit);
		else if (type == ORDER)
			orderTraversal(root, visit);
		else if (type == RE_ORDER)
			reOrderTraversal(root, visit);
		else;//NONE_ORDER ����traversal_
	}
	/*
	����[1] 2  3  4  5  6
	���� 3  2  4 [1] 6  5
	���� 3  4  2  6  5 [1]
	1
	2     5
	3 4   * 6
	*/
	void orderTranslation(Element *buf, Element *preOrder, Element *inOrder, Element *postOder, int n){
		if (inOrder == nullptr)
			puts("must know inorder");
		else if (preOrder == nullptr && postOder != nullptr)
			calcPrefOrder(buf, inOrder, postOder, n);
		else if (postOder == nullptr)
			calcPostOrder(preOrder, inOrder, buf, n);
		else
			printf("preOrder == null %d\nin == null %d\npost == null %d\n"
			, preOrder == nullptr, inOrder == nullptr, postOder == nullptr);
	}
	//������: mirroring reversal����ת ת������ʹ��ԭ�����κλ��ڱȽϵķ���(����������:��С�Ҵ�->�����С)
	void mirReversal(){
		queue<BT> q;
		q.push(root);
		while (!q.empty()){
			BT t = q.front(); q.pop();
			//�����зǿ���������ת������
			if (t->Left) q.push(t->Left);
			if (t->Right) q.push(t->Right);
			//����������������ת��
			swap(t->Left, t->Right);
		}
	}
	//ͬ���ж�
	bool omorphism(BinTree const&T2){
		return isomorphic(this->root, T2.root);
	}
	//�������ڵ������������е����ṹ��
	//tips:1.��Ϊ���������������Ϻ���һ�ζ��������� 2.�������þ�̬��������������ṹ ����ԭ�ṹ
	//��ȫ���������Ը�������ֱ�ӹ���(��ֻ����ȫ���������������Ĺ�����ʵ������)
	//��ͨ������ֻ������֪�ṹ��ǰ����ʹ�����ƵĽ������������ṹ�Ĺ���
	bool fillData(vector<T> &dataA){
		return fillData(dataA, 0, dataA.size(), root);
	}
	int size(){
		return size_;
	}
protected:
	const int LEN = sizeof(class BTNode);
	BT root = NULL;
	Position lastInsertPosition = NULL;//(���������������ɵĽ��)�޷�������жϲ���ɹ����
	bool isInsert = false;//�Ƿ�ִ���˲������(�жϲ����Ƿ�ɹ�)
	int size_ = 0;//��Ч��Ԫ�ظ���
	queue<Element*> freeMem;//�����ڴ�
	Element* memoryBlock = NULL;//�ڴ����Խ��������ľֲ����������� ��������ʹ���ⲿ������ڴ�
	//�����������ֵ ���������ֶ�û�и���
	BinTree& operator= (const BinTree& rhs){
		DEPRINTF("BT��ֵ");
		//��Ҫ�����Լ���root
		destroy(root);
		assignment(root, rhs.root);
		return *this;
	}
	//�����Ӷ������Ĺ�ģ
	int scaleOf(BT t){
		return empty(t) ? 0 : scaleOf(t->Left) + scaleOf(t->Right) + 1;
	}
	//���ؽ����ȣ�(���ݹ�ʽ Height = max(Hl, Hr) + 1 �ɺ�������ı�ʵ��)
	static int depthOf(Position bT){
		int HL, HR;
		if (bT){
			HL = depthOf(bT->Left);
			HR = depthOf(bT->Right);
			return (HL > HR ? HL : HR) + 1;
		}
		else
			return 0;//�������Ϊ��
	}
	//(����, ����������±�, ��ǰ���µ����ݸ���, �ṹ��Դ��)
	bool fillData(vector<T> &dataA, int dataRootSub, int n, BT t){
		if (empty(t))
			return true;
		int nl = scaleOf(t->Left), nr = scaleOf(t->Right);//����������ģ
		//�������Ĺ�ģ�Ƚ�
		if (nl + nr + 1 != n)
			return false;
		//�������
		t->Data = dataA[dataRootSub + nl];
		//�������ݹ�							//1��1�������
		fillData(dataA, dataRootSub, n - nr - 1, t->Left);
		//�������ݹ�      //��1��������nl ��"С��"������
		fillData(dataA, dataRootSub + nl + 1, n - nl - 1, t->Right);
		return true;
	}
	//���ָ�����ٷ���(�൱�ڽ�������)
	virtual void destroy(BT &r){
		if (r){
			destroy(r->Left);
			destroy(r->Right);
			delete r;
			r = NULL;
		}
	}

	//�������---���ϵ���->������(����ʵ��)
	template<class Fun>
	void levelTraversal(BT bT, Fun visit){
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
	//����---������->������->������;(�ݹ�ʵ��)
	template<class Fun>
	void preTraversal(BT const bT, Fun visit){
		if (bT){
			visit(bT);
			preTraversal(bT->Left, visit);
			preTraversal(bT->Right, visit);
		}
	}
	//����---������->������->������;
	template<class Fun>
	void infTraversal(BT const bT, Fun visit){
		if (bT){
			infTraversal(bT->Left, visit);
			visit(bT);
			infTraversal(bT->Right, visit);
		}
	}
	template<class Fun>//˳�����(����ֹ���������)
	bool orderTraversal(BT const bT, Fun visit){
		if (bT){
			if (orderTraversal(bT->Left, visit))
				return true;
			if (visit(bT))//����ֵ��ʾ�Ƿ���ֹ
				return true;
			return orderTraversal(bT->Right, visit);
		}
		return false;
	}
	template<class Fun>//�������(tips:�����ظ���Ԫ��ʱ��˳����������������������)
	bool reOrderTraversal(BT const bT, Fun visit){
		if (bT){
			if (reOrderTraversal(bT->Right, visit))
				return true;
			if (visit(bT))//����ֵ��ʾ�Ƿ���ֹ
				return true;
			return reOrderTraversal(bT->Left, visit);
		}
		return false;
	}
	//����---������->������->������;
	template<class Fun>
	void postTraversal(BT const bT, Fun visit){
		if (bT){
			postTraversal(bT->Left, visit);
			postTraversal(bT->Right, visit);
			visit(bT);
		}
	}

	//���й���
	void prefInBuild(Element const *preOrder, Element const *inOrder, BT &t, int n){
		int Ln;/*������������*/
		if (n == 0)return;
		if (t == NULL)
			t = nodeCreater(*preOrder);
		else/*ת����ϵ:��������������Ԫ�ؾ���������*/
			t->Data = *preOrder;
		for (Ln = 0; Ln < n && inOrder[Ln] != *preOrder; Ln++);
		prefInBuild(preOrder + 1, inOrder, t->Left, Ln);/*�����������������������һ��Ԫ�� �������鱣�ֲ���*/
		prefInBuild(preOrder + Ln + 1, inOrder + Ln + 1, t->Right, n - Ln - 1);/*������������һ��Ԫ��*/
	}
	//���й���vector
	void prefInBuild(vector<Element> &preOrder, int preRoot, vector<Element> &inOrder, int inRoot, BT &t, int n){
		int Ln;/*������������*/
		if (n == 0)return;
		if (t == NULL)
			t = nodeCreater(preOrder[preRoot]);
		else/*ת����ϵ:��������������Ԫ�ؾ���������*/
			t->Data = preOrder[preRoot];
		for (Ln = 0; Ln < n && inOrder[inRoot + Ln] != preOrder[preRoot]; Ln++);
		prefInBuild(preOrder, preRoot + 1, inOrder, inRoot, t->Left, Ln);/*�����������������������һ��Ԫ�� �������鱣�ֲ���*/
		prefInBuild(preOrder, preRoot + Ln + 1, inOrder, inRoot + Ln + 1, t->Right, n - Ln - 1);/*������������һ��Ԫ��*/
	}
	//���й���
	void postInBuild(BT &t, Element const *inOrder, Element const *postOder, int n){
		int Ln;/*����������*/
		if (n == 0)return;
		if (t == NULL)
			t = nodeCreater(postOder[n - 1]);
		else/*ת����ϵ:������������βԪ�ؾ���������*/
			t->Data = postOder[n - 1];
		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; Ln++);//��ȡ����������
		postInBuild(t->Left, inOrder, postOder, Ln);/*��������������������*/
		postInBuild(t->Right, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*����������*/
	}
	//���й��� �Ż���
	BT postInBuild(int *inOrder, int *postOder, int n){
		int Ln;/*����������*/
		if (n == 0)	return NULL;

		Position t = (Position)malloc(sizeof(struct TreeNode));
		memset(t, 0, sizeof(struct TreeNode));
		t->Element = postOder[n - 1];/*ת����ϵ:������������βԪ�ؾ���������*/

		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; Ln++);//��ȡ����������
		t->Left = postInBuild(inOrder, postOder, Ln);/*��������������������*/
		t->Right = postInBuild(inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*����������*/
		return t;
	}

	void calcPostOrder(string const &sPre, string const &sMed, string &sPostBuffer){
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
			//�����һ��ֵ(root)�ұ�ȡ�������ĳ��ȼ���������
			string sPreLeftSub = sPre.substr(1, leftSubLen);
			//root�����
			string sMedLeftSub = sMed.substr(0, leftSubLen);
			calcPostOrder(sPreLeftSub, sMedLeftSub, sPostBuffer);
		}
		else{
			//do nothing
		}

		//�����������������������ݹ�
		if (medRootSub + 1 < sMed.size()){
			//�����һ��ֵ(root)���ұ���������������ȡ���м���������
			string sPreRightSub = sPre.substr(1 + leftSubLen);
			//root���ұ�
			string sMedRightSub = sMed.substr(medRootSub + 1);
			calcPostOrder(sPreRightSub, sMedRightSub, sPostBuffer);
		}
		else{
			//do nothing
		}

		if (medRootSub != string::npos){
			//����root
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
	void calcPostOrder(Element *preOrder, Element *inOrder, Element *postOder, int n){
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
	void calcPrefOrder(Element *preOrder, Element *inOrder, Element *postOder, int n){
		int Ln;/*����������*/
		if (n == 0)return;
		*preOrder = postOder[n - 1];/*ת����ϵ:������������βԪ�ؾ���������*/
		for (Ln = 0; Ln < n && inOrder[Ln] != postOder[n - 1]; Ln++);//��ȡ����������
		calcPrefOrder(preOrder + 1, inOrder, postOder, Ln);/*��������������������*/
		calcPrefOrder(preOrder + Ln + 1, inOrder + Ln + 1, postOder + Ln, n - Ln - 1);/*����������*/
	}
	/*���ݺ������������������������� ����->��->������->�������*/
	void calcLeveOrder(Element *preOrder, Element *inOrder, Element *postOder, int n){
		puts("has not imp");
		exit(-1);
	}

	//�����Ƿ�T1��T2�Ƿ�ͬ�� isomorphic(adj. [��] ͬ���ģ�ͬ�ε�)  isomorphism_(n. ����ͬ��[�ﻯ] ����ͬ����ͬ��)
	bool isomorphic(BT T1, BT T2){
		if ((T1 == NULL && T2 != NULL) || (T1 != NULL && T2 == NULL))//��Ȳ�ͬ
			return false;
		else if (T1 == NULL && T2 == NULL)
			return true;
		else/*����������Ϊ��*/
		{
			if (T1->Data != T2->Data)//���ݲ�ͬ
				return false;
			else/*��ǰ�������*/
			{	/*����ǰ˳����ƥ����������*/
				if (isomorphic(T1->Left, T2->Left))
					return isomorphic(T1->Right, T2->Right);
				else/*����ת����������ƥ��*/
				{
					if (isomorphic(T1->Left, T2->Right))
						return isomorphic(T1->Right, T2->Left);
					else
						return false;
				}
			}
		}
	}

	//�ݹ鿽����ֵ
	void assignment(BT &lhs, const BT rhs){
		if (!empty(rhs)){
			lhs = new BTNode(*rhs);
			assignment(lhs->Left, rhs->Left);
			assignment(lhs->Right, rhs->Right);
		}
	}
	//����ֵ(����)
	void reAssignment(){}
	//�ظ����봦��
	void repetitionInsert(Position t){
		//x==t->Data ������� �ֶ�����lastInsertPosition
		lastInsertPosition = t;
		isInsert = false;
	}
public:
	//��������� ����һ��δʹ�õĽ�� ��������δʹ�ý�� ����NULL ֻ�ܲ���ʹ��
	virtual BT nodeCreater(Element const &data_){
		lastInsertPosition = new BTNode(data_);
		isInsert = lastInsertPosition == NULL ? false : true;
		++size_;
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
		--size_;
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
class BinSearchTree : public virtual BinTree<T> {
public:
	typedef typename BinTree<T>::BT BST;
	//using BinTree<T>::Element;�����÷�
	//using Position = BinTree<T>::Position;�����÷�
	typedef typename BinTree<T>::Position Position;
	typedef typename BinTree<T>::Element Element;
	using BinTree<T>::root;
	using BinTree<T>::lastInsertPosition;
	using BinTree<T>::traversal;
	using BinTree<T>::destroy;
	using BinTree<T>::nodeCreater;
	using BinTree<T>::nodeEraser;
	//constructor
	BinSearchTree(){}
	//�������� ֱ�ӵ��ĸ��෽��
	BinSearchTree(const BinSearchTree &rhs) :BinTree<T>(rhs)/*������δ����ǿ��ת��*/{
		DEPRINTF("BST��������");
	}
	/*ֻҪ���ı������������Ե����������������������ȹ������������ȹ����������޹�
	��������ͨ���빹�����е�����:
	[��������]:[82] 76 23 80 | 90 95
	[��������] 82 90 76 23 95 80 ��������������ͬһ����*/
	//������ (��������������,�����Ҷ�+Ԫ�ظ���)
	BinSearchTree(Element *preLeft, Element *preRight, int eqaul = 0){
		while (preLeft < preRight){
			if (!insert(*preLeft)){
				puts("����ʧ��!");
				system("pause");
			}
			++preLeft;
		}
	}
	//������ vector
	BinSearchTree(vector<Element> &preOrder, int eqaul = 0){
		/*��preOrder������������
		**�ᷢ��:
		**vector��������
		**vector����
		**BT��������
		**BST��������
		**BST����
		**BT����
		**vector����(��֪�������ǹ��������������� ���ǹ���һ������һ��)
		(������Ϊʲô�ڹ��캯���� �ô�ֵ���������������һϵ�� �Ҿ�����ȫû��Ҫ��
		������һ��ʼû��д��Ӧ����������...)
		ע��!!!
		������BinSearchTree t = BinSearchTree(vector<>);
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
	virtual ~BinSearchTree() override{
		DEPRINTF("BST����");
	}
	//��ֵ �ܷ�ʱ�� ���������Ķ�������ֵ
	BinSearchTree& operator= (const BinSearchTree& rhs){
		DEPRINTF("BST��ֵ");
		//<==>���θ��࿽������ һ�θ��ำֵ ���θ�������
		//(BinTree)(*this) = (BinTree)rhs;
		// ==>ǿ��ת����ʵ�ǵ��õĿ������췽��(����Ч�ʲ���) ����Ӧ��Ϊ�����д�Լ��ĸ�ֵ����(��ʹû�������ӵ���)
		BinTree<T>::destroy(root);//��Ҫ�����Լ���root
		BinTree<T>::assignment(root, rhs.root);
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
		traversal(Tree::ORDER, [&](Position bt){
			t = bt;
			return id++ == k;//�ҵ�����ֹ
		});
		return t;
	}
	//�����ض�Ԫ�صĵȼ�(�Ȳ������Ԫ�صĸ���) O(2*N)
	int getRankOf(T &x){
		int rank = 1;
		traversal(Tree::RE_ORDER, [&](Position bt){
			//15 30 40 45 50 50   rank(45) = 3 �������3 ������2
			if (bt->Data > x)//ȥ�������������
				++rank;
			return bt->Data == x;//�ҵ�����ֹ
		});
		return rank;
	}
	//��������Ԫ�صĵȼ� (������Ϻ�total sortһ���������Ƶķ�������һ��ͳ�Ƴ�����������) ��(2*N)����
	//��ȻN��insert(O(logN))��N����ȡ��sort(O(N*logN))��Ч�ʲ�� ���۱���Ч��tree ��(2*N), vector ��(N)
	void calcRank(vector<T> &total, int cnt = -1){
		cnt = cnt < 0 ? BinTree<T>::size() : cnt;//Ĭ�ϼ��������˵�����
		int pastId = 0;//���
		int reCnt = 0;//��ǰԪ�����ڵ��ظ�����
		Position pastP = NULL;
		traversal(Tree::RE_ORDER, [&](Position bt){
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
		BST bST = root;
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
		BST bST = root;
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
			else//���ҽ��� ������ʽ��visit������ʽ��ͬʱ ��ǰ���û�б������ӽ��ض�û�б�����
				return false;//��ֹ
		});
	}


	/*����x�ڶ���������bST�е�λ��; ���Ҳ����򷵻�NULL*/
	Position find(T const&x){
		return findOf(root, x);
	}
	//�Զ������(�Զ���С�ڷ���)
	template<typename LessCmp>
	Position find(T const&x, LessCmp less){
		return findOf(root, x, less);
	}
	//���� O(logN)
	pair<Position, bool> insert(Element const&x){
		root = Insert(root, x);
		//��map��insert����ֵ���ƣ��ظ�insert ����<�ظ�Position, false>�������������������ҳ�2��������ͬ�����ֵ�ʱ������Ч
		return{ BinTree<T>::lastInsertPosition, BinTree<T>::isInsert };
	}
	//���в��� �ֲ��������O(1) ����O(logN) �����Avlʹ�ô˲����ʹAvl�˻�Ϊ��ͨBST(�൱���Ż�������: ����Ч�ʱ�� ����Ч�����Avl�˻�)
	//(����654789)->4 5 [6] Orderly����false 7 [�ֶ�]��true 8 9 ��ʵ�־ֲ��������
	pair<Position, bool> insertOrderly(Element const&x, bool &Orderly){
		if (!Orderly){//���Ѿ���������Ĳ��� ���˻�Ϊ��ͨ����
			root = Insert(root, x);
		}
		else{
			if (!BinTree<T>::empty(root)){
				if (x < root->Data){//�����������������������
					if (x < lastInsertPosition->Data){//x��λ�������� ��x����lastInsertPositionС���߱ض�Ҳ��������
						Insert(lastInsertPosition, x);//����ֱ����lastInsertPosition����߲弴��
						Orderly = true;
					}
					else{//����Ӧ�ò����λ��λ��������:root��lastInsertPosition�м�
						Orderly = false;
						root = Insert(root, x);
					}
				}
				else{//���ʱֱ�Ӵ���ʹInsert�ж�
					if (lastInsertPosition->Data < x){///@x > lastInsertPosition->Data
						Insert(lastInsertPosition, x);
						Orderly = true;
					}
					else{
						Orderly = false;
						root = Insert(root, x);
					}
				}
			}
			else{
				Orderly = true;
				root = Insert(root, x);
			}
		}
		return{ lastInsertPosition, BinTree<T>::isInsert };
	}
	//ɾ�� ɾ���ɹ�����true
	bool erase(Element const&x){
		bool succeed = true;
		root = Delete(root, x, succeed);
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
	/*����Insert��x�������������t�����ؽ�����ĸ����ָ��*/
	virtual BST Insert(BST t, Element const&x){
		if (!t)//���� ����һ��δʹ�ý��
			t = nodeCreater(x);
		else if (x < t->Data)
			t->Left = Insert(t->Left, x);
		else if (t->Data < x)//@x > t->Data
			t->Right = Insert(t->Right, x);
		else {
			BinTree<T>::repetitionInsert(t);
		}
		return t;
	}
	/*�Ӷ���������bST��ɾ��x,�����¸�; ��x��������tag->!tag ����ԭ����*/
	virtual BST Delete(BST bST, Element const&x, bool &tag){
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
	Position findMinOf(BST bST){
		while (bST && bST->Left)
			bST = bST->Left;
		return bST;
	}
	Position findMaxOf(BST bST){
		while (bST && bST->Right)
			bST = bST->Right;
		return bST;
	}
};

/*
����������ϸ�ƽ����������Ҫ��ÿ�β���ɾ����ʱ�������avl���ϸ�ƽ������ÿ�β���ɾ����ʱ��Ҫ��ת��ƽ��״̬��
��ͳ�ƽǶ���˵�����������Ҫ��һ�㣬�����Ҳ����������ϸ�֤O(logN)���������һ���������޸ĵĻ���AVL�����ܿ�һ�㣬
����˵����������������޸ĵĻ�����������+���ֲ������ܱȸ�������Ҫ�á�
*/
/*AVLƽ���� -------------------����Ϲ�ö������Ĳ���(����ɾ��)*/
template<class T>//���ؼ̳�
class AvlTree :public BinSearchTree<T> {
	typedef typename BinSearchTree<T>::BST AVLT;
	typedef typename BinTree<T>::BT BT;
	using Element = T;
	using Tree::Max;
	using BinSearchTree<T>::root;
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
		AvlNode(int data_, int height_)
			:BinTree<T>::BTNode(data_){
				Height = height_;
			}
		virtual ~AvlNode()override{
			DEPRINTF("AvlNode����");
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
	���������ⷽʽ�����ԭ��: ����֪���ڷ���Ч��; ����֪���ڷ�������
	���ֵ���������Ӧ�ò�ͬ�ĸ�����ͺͼ������������ì��
	�����˺ܾ��������������(��ת����̫����� ���������Ҿ�������ν��������ʵ������ ���Ҿ�������תҲ���Ͳ�ͨ)
	����ԭ���㷨һ���µĽ��� �ﵽ�������������Ч��*/

	/*�������(LL) :�������ӽڵ�B��������A��,A��ΪB���������ϵ��ӽ�� �������� �����¸�B*/
	AVLT leftElevate(AVLT A){
		AVLT B = A->Left;
		A->Left = B->Right;
		B->Right = A;
		A->setHeight(Max(heightOf(A->Left), heightOf(A->Right)) + 1);
		B->setHeight(Max(heightOf(B->Left), A->getHeight()) + 1);//a��height�����Ѿ����� ֱ�ӻ�ȡ����
		return B;
	}
	/*�Ҳ�����(RR) :�������ӽڵ�B��������A��,A��ΪB���������ϵ��ӽ�� �������� �����¸�B*/
	AVLT rightElevate(AVLT A){
		AVLT B = A->Right;
		A->Right = B->Left;
		B->Left = A;
		A->setHeight(Max(heightOf(A->Left), heightOf(A->Right)) + 1);
		B->setHeight(Max(heightOf(B->Right), A->getHeight()) + 1);
		return B;
	}
	/*������������������Ҳ�:�ȶ�A�����������Ҳ����� �ٶ�A���������*/
	AVLT LR(AVLT A){
		A->Left = rightElevate(A->Left);
		return leftElevate(A);
	}
	/*��������������������:�ȶ�A����������������� �ٶ�A���Ҳ�����*/
	AVLT RL(AVLT A){
		A->Right = leftElevate(A->Right);/*����A��������*/
		return rightElevate(A);
	}
	/*��x����ƽ���� �����ص������ƽ����*/
	BT Insert(BT t, Element const&x)override{
		if (!t){/*��������� ���½�һ������������*/
			t = BinTree<T>::nodeCreater(x);
		}
		else if (x < t->Data){/*������������*/
			t->Left = Insert(t->Left, x);/*t��ƽ������BF*/
			if (heightOf(t->Left) - heightOf(t->Right) == 2){/*���������߶�height��ƽ��*/
				if (x < t->Left->Data)/*LL�� ֱ���������������*/
					t = leftElevate(t);
				else/*LR�͵���LR����*/
					t = LR(t);
			}
		}
		else if (x > t->Data){
			t->Right = Insert(t->Right, x);
			if (heightOf(t->Left) - heightOf(t->Right) == -2){
				if (x > t->Right->Data)
					t = rightElevate(t);
				else
					t = RL(t);
			}
		}
		else{
			BinTree<T>::repetitionInsert(t);
		}
		t->setHeight(Max(heightOf(t->Left), heightOf(t->Right)) + 1);/*��������*/
		return t;
	}
public:
	AvlTree(){}
	~AvlTree(){
		DEPRINTF("AVL����");
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
/*��̬������Static Binary Tree(�����鴢��Ķ�������һ������ȫ������)*/
template<class T>
class StaBinTree :public virtual BinTree<T>{
protected:
	typedef typename BinTree<T>::BT BT;//levl
	typedef typename BinTree<T>::BT structArray;
	typedef typename BinTree<T>::Position Position;
	using BinTree<T>::root;
	using BinTree<T>::LEN;
	typedef T ElementSBT;
	structArray struA = NULL;//Left��Right�������Һ���λ�������ڵĵ�ַ
	int capacity = 0;//���鳤��, ����Ԫ�ظ���(size+1) ����Ϊ1
	int size = 0;//�Ѵ���,��Ч ��Ԫ�ظ��� (cap-1) struAβԪ���±�

	//ͨ�����빹�����ṹ ���ظ���� tips: getSub���뽫������Ϊ�����ں��ӽ����±�(int)
	template<class Fun1, class Fun2>
	Position buildStructure(Fun1 getData, Fun2 getSub, int nullChar){
		int n = capacity - 1;
		int i, sum = (n - 1)*n / 2;
		int leftSub, rightSub;
		Position rt = struA + 1;

		while ((i = getchar()) == '\n' || i == ' ');
		ungetc(i, stdin);//��������� bug:����ʹ��ʱӦȥ��

		for (i = 0; i < n; i++){
			Position leftChild = NULL, rightChild = NULL;
			getData(&rt[i].Data);
			getSub(&leftSub, &rightSub);
			if (leftSub != nullChar){
				leftChild = rt + leftSub;
				sum -= leftSub;
			}

			if (rightSub != nullChar){
				rightChild = rt + rightSub;
				sum -= rightSub;
			}
			rt[i].Left = leftChild, rt[i].Right = rightChild;
		}
		return n == 0 ? NULL :
			0 < sum&&sum < size ? rt + sum : NULL;
	}
	bool full(){
		return size == capacity - 1;
	}

	bool empty(){
		return size == 0;
	}
	//�����ṹ������ ���Զ�������һ�����ڴ洢0���ڱ�
	void reCapacity(int capacity_){
		struA = (structArray)realloc(struA, LEN*++capacity_);
		if (capacity == 0)//�״�����->ȫ���ʼ��(�ų���ʹ�õ�size һ����0)
			memset(struA + size, 0, LEN*(capacity_ - size));
		else//�ٴ�����->root��[struA+cap, struA+cap](�ų�ԭ�е�cap)��ʼ��, struA+cap�Ѿ�������ĩβ�ĺ�һ��
			memset(struA + capacity, 0, LEN*(capacity_ - capacity));
		capacity = capacity_;
	}
public:
	using BinTree<T>::height;
	//��ʼ��root 
	void iniARoot(){//root�������ڱ�(0��)
		root = struA + 1;
	}
	//���쾲̬��ȫ������ root��size��Ҫ�����ʼ�� ����ת��ʱ����root (�ɴ洢��Ԫ�ظ���)
	StaBinTree(int n){
		reCapacity(n);
	}
	/*
	8
	1 -
	- -
	0 -
	2 7
	- -
	- -
	5 -
	4 6
	*/
	/*StaBinTree<T> t = StaBinTree<T>(n,
	[](T*data_){static int i = 0; *data_ = i++; },
	[](int* lSub, int* rSub){char l, r; scanf("%c %c", &l, &r); *lSub = l-'0', *rSub = r-'0'; },
	'-'-'0');
	t.traversal(t.ORDER_POST_ROOT);
	t.traversalDisplay();*/
	/*
	9
	1 6
	2 3
	-1 -1
	-1 4
	5 -1
	-1 -1
	7 -1
	-1 8
	-1 -1
	73 45 11 58 82 25 67 38 42
	*/
	/*StaBinTree<T> t = StaBinTree<T>(n,
	[](T*data_){static int i = 0; *data_ = i++; },
	[](int* lSub, int* rSub){scanf("%d %d", lSub, rSub); },
	-1);*/
	//�ṹ��֪����(������Ŀ, ���ݻ�ȡ����, �±��ȡ����, ������ֵ)
	//��������� �⹹�������˳�򴢴�Ķ����������� ����ȫȫֻ�ǰ�������鵱�ڴ�ʹ��Ȼ�󹹽����Ӷ���������
	/*lambda��ͨ������һ�������˲�����()��С����ʵ�ֵģ�һ��lambda�����Ǹ����һ��ʵ��
	**����lambda������ľ����Զ��洢ʱ�ڵı���������������Щ�����ļ��Ϻ������бհ���
	**һ��û��ָ���κβ����lambda����,������ʽת����һ��������ͬ������ʽ����ָ��
	**���������б�����ʱ������ڰ�����ʵ����ʱ�Բ������ķ�װ���������������ʵ���������Ա�����ݴ��ڵģ�����޷���ת���ɺ���ָ�롣*/
	//�˰汾����ʹ��lambda�����б� ���ṹ�������������� ��root���ض��� ����ָ��root���±�
	template<class Fun1, class Fun2>
	StaBinTree(int n, Fun1 getData, Fun2 getSub, int nullChar, int rootSub = -1){
		reCapacity(n);
		root = buildStructure(getData, getSub, nullChar);//��StaBinTreeʵ�ֱ��ڷ����ڵ������±�ļ���
		if (rootSub > 0)
			root = struA + rootSub;
		size = capacity - 1;
	}
	virtual ~StaBinTree() override{
		free(struA);//malloc ����delete?
		root = NULL;//��֤�������������
		DEPRINTF("SCT����");
	}
	//���������ڵĽ���� 1��Ϊroot 0��Ϊ�ڱ�
	int index(Position t){
		/*��һ��Ԫ�ز���ʱroot=NULL �����������Һ������ӷ���������root ���ο�root=1ʱ�Ĺ�����������������*/
		return t - struA;
	}
	//���������ڵĽ��λ��
	Position position(int i){
		return struA + i;
	}
	//���ص�lv�����Ľ�����
	//���ص�i���Ҷ������ lvԽ�緵��-1
	int leavesCount(int lv){
		if (lv > height())
			return -1;
		int count = 0;
		queue<pair<BT, int>> q;//t, lv
		q.push(make_pair(root, 0));
		while (!q.empty()){
			auto p = q.front();
			q.pop();
			if (!empty(p.first->Left))
				q.push({ p.first->Left, p.second + 1 });
			if (!empty(p.first->Right))
				q.push({ p.first->Right, p.second + 1 });
			if (p.first->isLeave() && p.second == lv)
				++count;
			else if (p.second > lv)
				break;
			//else continue;
		}
		return count;
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
�ðɿ��Խ�ɾ�����ڴ�ָ����ĩβ�ڴ�ָ�뽻��(�������õ���ָ������Ķ���ָ�� �൱���õ�������ָ��(��)) ��,������ͨ������ʽ����������ȫû������ ����������ϵͳ�ڴ��������ʽ?
*/
/*��̬����������Static Binary Search Tree*/
/*SequentialBinTree˳�򴢴�Ķ�����*/
template<class T>//��̳�(virtual����д���м�) �����̳������μ̳е�����µĻ����Ա�ĳ�ͻ
class StaBinSearchTree :public virtual StaBinTree<T>, public virtual BinSearchTree<T>{
public:
	StaBinSearchTree(int n) :StaBinTree<T>(n){
		size = n;
	}
	~StaBinSearchTree() override{
		DEPRINTF("SCBT����");
	}
protected:
	typedef typename BinTree<T>::Position Position;
	typedef T Element;
	typedef Position BT;
	typedef BT structArray;
	using BinTree<T>::root;
	using StaBinTree<T>::struA;
	using StaBinTree<T>::size;
	using StaBinTree<T>::capacity;
	using StaBinTree<T>::full;
	void nodeEraser(Position &del)override{
		del->Data = 0;//0������ʼ״̬ ֻΪ�˱�ʶ�� ���������ô�
		del->Left = del->Right = NULL;//��̬�����ɾ������ʵ��ɾ��
		del = NULL;
		--size;///
	}
	BT nodeCreater(Element data_)override{
		if (full())
			return NULL;
		struA[size + 1].Data = data_;//0��Ԫ�������ڱ�
		++size;///
		/*
		isInsert = lastInsertPosition == NULL ? false : true;  bug
		BinTree<T>::lastInsertPosition = struA + size + 1;
		*/
		return struA + size + 1;
	}
};

/*��ȫ���������� ʹ�����鹹�� ʵ�ֶ���ľ�̬���ֲ��� ����Ҫ��̬��ɾ���� ��Ҫ����ת��Ϊ����������*/
/*����ֲ������������ ���ֲ��ҵ������������ ����CBST��Ҫ�������� ����������䱾�����������в��������(����Ԫ���൱��������ֵ)*/
/*����ʵ�ý�˳�򴢴�Ķ�������������ȫ������ ֻ��˵��ȫ��������˳��洢������*/
template<class T>
class CompleteBinSearchTree :public StaBinTree<T>{
public:
	using Position = typename StaBinTree<T>::Position;
	using StaBinTree<T>::full;
	using StaBinTree<T>::index;
	using StaBinTree<T>::position;
protected:
	typedef typename BinTree<T>::BT BT;
	typedef Position structArray;
	typedef T Element;
	using StaBinTree<T>::root;
	using StaBinTree<T>::struA;
	using StaBinTree<T>::size;
	using StaBinTree<T>::capacity;
	using Tree::Min;

	/*����StaBinSearchTree�������Ĺ�ģ (�ܽ����)*/
	int getLeftScaleL(double n){
		/*2^h - 1 + x = n*/
		int h = (int)log2(n + 1);/*����ȡ��*/
		int x = n + 1 - (int)pow(2.0, h);/*���²㵥���Ľ����*/
		return (int)pow(2.0, h - 1) - 1 + Min(x, (int)pow(2.0, h - 1));
	}
	Position getLeftChild(Position t){
		int i = index(t);
		return 2 * i <= size ? struA + 2 * i : nullptr;
	}
	Position getRightChild(Position t){
		int i = index(t);
		return 2 * i + 1 <= size ? struA + 2 * i + 1 : nullptr;
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
		/* 0 1 2 [[3 4 5 [6] 7 8]] 9 */
	}
	/*��ȫ�������������(˳�������������)*/
	template<class Fun>
	void levelTraversal(BT bT, Fun visit){//�޷�override ������Ӧ�ø�����
		if (bT == NULL)return;
		FOR(i, 0, size){//bT����ʱroot������struA(root = struA+1)
			if (!visit(bT + i))
				return;//��ֹ
		}
	}
public:
	/*sort(iniA, iniA + n, less<T>());
	**��ת����:sort(iniA, iniA + n, greater<T>()); ��ȫ�������������С����֧����С�Ҵ����ľ���ת ��ͨBinTree֧��*/
	//ʹ�����鹹��Ψһ����ȫ������ (������ȫ������������ɾ û��size��capacity֮��) tips::���������� ��Ϊ������
	CompleteBinSearchTree(int n, T *iniA = NULL)
		:StaBinTree<T>(n){
			if (iniA != NULL && n > 0){
				root = buildComplete(iniA, iniA + n);
				size = n;
			}
		}
	//ֱ�Ӱ�����copy����̬������
	CompleteBinSearchTree(T *iniA, int n)
		:StaBinTree<T>(n){
			if (iniA != NULL && n > 0){
				root = struA + 1;
				for (int i = 0; i < n; ++i)
					root[i].Data = iniA[i];
				size = n;
			}
		}
	~CompleteBinSearchTree() override{
		DEPRINTF("CBT����");
	}

	Position find(Element x){
		return NULL;//not impl
	}
	//�����������Ĺ������� Խ�緵���ڱ�(0�Ž��)
	Position ancestor(int i1, int i2){
		if (i1 > size || i2 > size || i1 < 0 || i2 < 0)
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
	typedef typename BinSearchTree<T>::Position Position;
	using BinTree<T>::root;
	using StaBinTree<T>::struA;
	using StaBinTree<T>::size;
	using StaBinTree<T>::capacity;
	using StaBinTree<T>::full;
	using StaBinTree<T>::empty;
	using CompleteBinSearchTree<T>::getLeftChild;
	using CompleteBinSearchTree<T>::getRightChild;
	using CompleteBinSearchTree<T>::getParent;
	using CompleteBinSearchTree<T>::getSibling;

	//ʹ��less<T> greater<T> �� �ȽϷ������� ����ͬ��������: ����û��ͬһ����ʾ����(��������()�ĸ���)�����ڴ��С�ѵļ�ʱ�ع�
	int(*cmper)(const T &min, const T &max) = NULL;
public:
	/*(�ѵĴ�С С�ڱȽϷ��� �����ʼ���������� �����е�Ԫ�ظ���)  ע��: �����鹹��Ķ�������ʼ���Ķ� �Ľṹ��һ��*/
	Heap(int sizeHeap, T *iniA = NULL, int nIniA = 0)
		: CompleteBinSearchTree<T>(sizeHeap){
			root = struA + 1;//û����ЧԪ��ҲҪ��root��ֵ
			size = nIniA;
			for (Position t = struA + 1; t < root + nIniA; ++t) {/*Ϊÿ��Ԫ�ظ���ʼȨֵ*/
				t->Data = iniA[t - root];//��T���Ͳ��ǻ������� ��Ҫ���ظ�ֵ��
				linkToChildren(t);//ֻҪûԽ�������->����ʱ��sizeΪ���������ı�־ �������ӽ���Ƿ�Ϊ��?
			}
		}
	~Heap()override{
		DEPRINTF("Heap����");
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
		PercoDown(1, "ɾ��");//ע��::������size--��
		unlinkToParent(struA + size);
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
		i = ++size;/*iָ��[�����]�������һ��Ԫ�ص�λ��*/
		for (; cmper(struA[i / 2].Data, Item) < 0; i /= 2)
			struA[i].Data = struA[i / 2].Data;//�����һ���к��ӵĽ�㿪ʼ ���Ϲ��˽��
		struA[i].Data = Item;/*����*/
		linkToChildren(struA + size);
		return true;
	}
	/*����һ���Ѷ�������ݵ���Ҫ������������С����С��(���¶��ϵ���)
	��[���һ���ӽڵ�ĸ��ڵ� �� ������һ���ж��ӵĽ��]
	ÿ�δ����ҽ������һ��"��"�Ľ�������˲�����ʼ���������ڵ�1*/
	/*���Զ��ع� ���Ӷ�O(N)*/
	//��С�� ����: maxCmp(����)�ڱ�(��Сֵ)	ע��: ����̫С���������:����infinity/2����
	//���� ����: mixCmp(С��)�ڱ�(���ֵ)
	void build(T sentry, int(*cmper_)(const T &min, const T &max)){
		cmper = cmper_;
		struA[0].Data = sentry;
		for (int i = size / 2; i > 0; i--){
			PercoDown(i, "����");
		}
	}
protected:
	//���� insert����
	void percolateUp(int i, Element *struA, int size){
		Element Item = struA[i];//iָ�����������Ԫ�ص�λ��
		for (; struA[i / 2] - Item > 0; i /= 2)
			struA[i] = struA[i / 2];//�����˽��ĸ���㿪ʼ ���Ϲ��˽�� �����������ӽ�������
		struA[i] = Item;
	}
	//���� :build����(��ĩ��㿪ʼ���� �ȼ��ڴ����һ������㿪ʼ����) delete����
	void percolateDown(int Parent, Element *struA, int size){
		int Child;
		Element x = struA[Parent];//ȡ����Ҫ���˵�ֵ
		for (; 2 * Parent <= size; Parent = Child){
			Child = 2 * Parent;//�������==size; ���Ҷ��Ӳ�����
			if (Child != size && struA[Child] - struA[Child + 1] > 0)
				++Child;//ѡȡ���Ҷ�����С��һ��
			if (x - struA[Child] < 0)//����Ԫ��С��Ŀ��ʱ��ֹ ����ʱ����
				break;
			else//����������<==>��temp����
				struA[Parent] = struA[Child];
		}
		struA[Parent] = x;
	}

	/*���˺��� ��H����H->Data[p]Ϊ�����Ӷѵ���Ϊ������С����С��*/
	void PercoDown(int Start, char *Order){
		int Parent, Child;
		Element x;
		/*���ˣ�ȡ���Լ�          ���Լ���ʼ�ҵ�һ�����ʵ�λ��*/
		/*ɾ����ȡ��βԪ�� Size-- �ӶѸ���ʼ*/
		int p = strcmp(Order, "ɾ��") == 0 ? size-- : Start;
		x = struA[p].Data;/*ȡ����Ҫ���˵�ֵ*/
		for (Parent = Start; 2 * Parent <= size; Parent = Child){
			Child = 2 * Parent;/*�������==size; ���Ҷ��Ӳ�����*/
			if (Child != size && cmper(struA[Child].Data, struA[Child + 1].Data) < 0)
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
		if (!empty(parent))
			linkToChildren(parent);//��size�Ѿ�--��ôԽ����Чvalid���ӽ��ᱻ��NULL
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
	using BinTree<T>::root;
	using Position = typename BinTree<T>::Position;//������Ҫ�ݹ��ӽ��ĵط��ض��õ�Left-Rightֻ����BinTree��Position-��getValue()���ڵı�Ҫ��
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
		HuTrNode(int data_, int weight_)
			:BinTree<T>::BTNode(data_){
				Weight = weight_;
			}
		~HuTrNode()override{
			DEPRINTF("HuTrNode����");
		}
	}*HuTr;//��������
	HuTr huffRoot = NULL;
public:
	/*���������Ĺ��� ��С��ʵ�� (�ַ�����, Ȩ������(һ����Ƶֵfreq), ���й�����С, hufҶ�ӽ����)*/
	HuffmanTree(T* character, int *freq, int nHuf) {
		Heap<HuTr> h = Heap<HuTr>(nHuf);
		HuTr sentry = new HuTrNode(0, -infinity / 2);//û�и�ֵ���� ֻ��new?
		//lambda����ʽ, �������� p345
		h.build(sentry, [/*�����б�*/](const HuTr &min, const HuTr &max){
			return max->Weight - min->Weight;
		});
		for (int i = 0; i < nHuf; ++i)	h.push(new HuTrNode(character[i], freq[i]));
		/*��size-1�κϲ� ÿ�ν�Ȩֵ��С���������ϲ� ��������;��size��仯 ������n*/
		while (nHuf-- > 1){
			HuTr t = new HuTrNode(0, 0);//������Ϊcharʱ ��Ч���Ϊ'\0'�������(�Զ�ʡ����Ч�ķ�Ҷ���) ������Ҫ��д��������
			t->Left = h.pop();
			t->Right = h.pop();
			t->setValue(t->Left->getValue() + t->Right->getValue());/*������Ȩֵ*/
			h.push(t);
		}
		huffRoot = h.pop();
		root = huffRoot;
		delete sentry;
	}/*Ƶֵ����freq[0]����Ȩֵ���� 0<=freq[i])��Ȩֵ i[1,*freq O(N log(N))*/
	//��������ӹ���huffman��

	~HuffmanTree()override{
		DEPRINTF("HuffmanTree����");
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
		//str.resize(maxLen);//str��length() size����maxLen �������� ����������new char �ֻص����ڴ����������
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
	static int codeLen(string const&code){
		return code.length();
	}
	/*tips:
	depth = codeLen+1 <-> depth(root)=1; (�����, ���ϵ�depth����)
	depth = codeLen  <--> depth(root)=0
	(����: �ı�����=Ȩ��*���볤��, WPL=Ȩ��*(���볤��+1))
	*/
	//�����ı���WPL
	static int wpl(vector<char*> &s, int *freq){
		int textLen, i, n = s.size();
		for (textLen = i = 0; i < n; ++i){
			textLen += freq[i] * (codeLen(s[i]) + 1);
		}
		return textLen;
	}
	//����һ�ױ�����ı����� = freq*codeLen
	static int textLen(vector<char*> &s, int *freq){
		int textLen, i, n = s.size();
		for (textLen = i = 0; i < n; ++i){
			textLen += freq[i] * codeLen(s[i]);
		}
		return textLen;
	}
	//����ı��Ƿ�ǰ׺�� //string const codes[]-> ����vector��Ч�����?
	static bool isPrefixCodes(vector<string> const &codes) {
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
	bool isOptimalCoding(vector<string> &codes, int *freq){
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
	HuTr buildPreCodeTree(vector<string> const &codes, int *freq = NULL){
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
			return t->getValue()*(depth - 1);//tips:depth(root) = 1
		else/*����һ������������*/
			return textLen(t->Left, depth + 1) + textLen(t->Right, depth + 1);
	}
	//���ؽ�����дӢ�ľ��ӵ�Ƶ������ ��Ϊmap?
	vector<int> frequency(string sentence){
		vector<int> freq;
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
	int huffmanMemory(string sentence){
		int freq[MAXN];
		memset(freq, 0, MAXN * 4);

		for (unsigned i = 0; i < sentence.length(); ++i){
			if (sentence[i] == '_')
				freq[26]++;
			else{
				freq[sentence[i] - 'A']++;
			}
		}
		priority_queue<int, vector<int>, greater<int> > q;

		for (int i = 0; i < MAXN; ++i){
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
class ArrayHuffman{
	typedef struct {
		int weight;//Ȩ��
		int parent;//�����
		int lchild;//����
		int rchild;//�Һ���
	} HTNode, *HuffmanTree;
	typedef char ** HuffmanCode;
	void swap(int &s1, int &s2){
		int tmp = s1;
		s1 = s2;
		s2 = tmp;
	}
	void swapC(char &s1, char &s2){
		char tmp = s1;
		s1 = s2;
		s2 = tmp;
	}
	//��1��upbound���ҳ�fatherΪ0�Ľڵ㸳��s1,s2,��Ϊ�˱�֤��Ψһ������s1�Ľڵ���С��s2��
	void SelectTwoMin(int upbound, HuffmanTree ht/*��tm�Ǹ�����!!!*/, int &s1, int &s2){
		s1 = s2 = 0;
		//�ҳ�weight��С��������sub
		for (int i = 1; i < upbound; ++i){
			if (ht[i].parent == 0){
				if (ht[s1].weight > ht[i].weight)
					s1 = i;
			}
		}
		for (int i = 1; i < upbound; ++i){
			if (ht[i].parent == 0){
				if (ht[s2].weight > ht[i].weight && i != s1)
					s2 = i;
			}
		}
		if (s1 > s2){
			swap(s1, s2);
		}
	}
	//���Ҳ������������������ĳ��Ȩֵ�Ľ����±� �����ڷ���0
	int sub(HuffmanTree ht, int wei, int n){
		for (int i = 1; i < n; ++i){
			if (ht[i].weight == wei)
				return i;
		}
		return 0;
	}
	//�����������ht �� �������������hc
	void HuffmanCoding(HuffmanTree &ht, HuffmanCode &hc, int *w, int n){
		/*�����������(α)*/
		ht = (HuffmanTree)malloc(sizeof(HTNode)*(2 * n));
		memset(ht, 0, (sizeof(HTNode)*(2 * n)));
		ht[0].weight = 1001;
		for (int i = 0; i < n; ++i) ht[i + 1].weight = w[i];
		/*��n-1�κϲ� ÿ�ν�Ȩֵ��С���������ϲ�*/
		int size = n;
		for (int i = 1; i < n; ++i){
			++size;
			SelectTwoMin(size, ht, ht[size].lchild, ht[size].rchild);//"pop" and "push"
			ht[size].weight = ht[ht[size].lchild].weight + ht[ht[size].rchild].weight;/*������Ȩֵ*/
			ht[ht[size].lchild].parent = ht[ht[size].rchild].parent = size;
		}
		/*�������*/
		hc = (HuffmanCode)malloc(sizeof(char*)*n);
		memset(hc, 0, (sizeof(char)*n));
		for (int i = 0; i < n; ++i){
			hc[i] = (char*)malloc(sizeof(char)* n);
			memset(hc[i], 0, sizeof(char)* n); //n�������볤��n - 1+'\0' = n
			for (int c = sub(ht, w[i], size), codeIndex = 0; c != size/*�����*/; c = ht[c].parent){
				int parent = ht[c].parent;
				hc[i][codeIndex++] = ht[parent].lchild == c ? '0' : '1';
			}
		}
		for (int i = 0; i < n; ++i){//��ÿ��������ת
			for (int lhs = 0, len = strlen(hc[i]); lhs < len / 2; ++lhs){
				int rhs = len - lhs - 1;
				swapC(hc[i][lhs], hc[i][rhs]);
			}
		}
		/*ht[i]:
		1 5 0 0
		2 5 0 0
		3 6 0 0
		4 7 0 0
		3 6 1 2
		6 7 3 5
		10 0 4 6
		*/
	}
	/*
	int w[4] = { 1, 2, 3, 4 };
	HuffmanTree ht;
	HuffmanCode hc;
	HuffmanCoding(ht, hc, w, n);
	*/
};

/*
TEST Fun:
int main(){
	freopen("input", "r", stdin);
	char s[40] = "1 2 +";//�Լ��ĸ�
	while (gets(s)){
		ExpressionTree ET = ExpressionTree(s, ExpressionTree::POST_EXPRESSION);
		auto ans = ET.eval();
		if (ans == infinity){
			puts("ERROR");
		}
		else{
			printf("%.1lf\n", ans);
		}
	}
	return 0;
}
*/
//����ʽ�� @TODO ��δͨ��PTA����(��Ҫ�Ǻ�׺����ʽ����ʱ�ĸ������� ��������ǰ׺����ʽ����)
class ExpressionTree : private BinTree<string>{
public:
	enum ExpressionTypeEnum{ 
		PREFIX_EXPRESSION,//ǰ׺����ʽ
		INFIX_EXPRESSION,//�б���ʽ
		POST_EXPRESSION,//��׺����ʽ
		ERROR_EXPRESSION//����ı���ʽ
	};
	//��׺����ʽ�����ı���ʽ����Ψһ
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
				//�������������:��ȷ�Ķ�Ԫǰ׺����ʽ�У��ÿո����Ĳ�������Ȼ�Ȳ�������һ��
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
	//����ʽת��  �����������ֱ�ӵ���
	string prefixToPost(){
		string next = nextOper(), ans;
		if (!isdigit(next[0])){
			auto node = nodeCreater(next);//�ݹ鹹����
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
				return infinity;
		}
		else if (type == PREFIX_EXPRESSION){
			toPost();
			return eval();
		}
		else if (type == ERROR_EXPRESSION)
			return infinity;
		else{
			_DEBUG_ERROR("�޷�ֱ�Ӽ�����׺����ʽ");
			return infinity;
		}
	}
	/*��׺����ʽ����*/

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
	//���غ�׺����ʽans��ֵ ���Ϸ�����infinity
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
					return infinity;
				}
				po1 = ps.top();
				ps.pop();
				if (ps.empty()){
					return infinity;
				}
				po2 = ps.top(); ps.pop();
				switch (c){
				case '+':ps.push(po2 + po1); break;
				case '-':ps.push(po2 - po1); break;
				case '*':ps.push(po2 * po1); break;
				case '/':{
							 if (po1 - 0.0 == 0){//����0����
								 return infinity;
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
			return infinity;
		}
	}
private:
	//BinTree base;
	string expression;
	//����ʽ��ǰ����
	ExpressionTypeEnum type;
};

template<class Base>
/*ָ������ ������ʵ��HashTable дһ��hash()��������������  ���������ȽϷ���*/
/*������ͨ�ṹ��Ҫע��  �״η���ʱҪ���ⲿ����ĸ�������(�Զ�����)  ��ֵʱע�ⲻ�ı�ԭָ��*/
class PtrArray{
	vector<Base*> table;
	unsigned size_ = 0;
public:
	PtrArray(){}
	//���Ԧ�(size)
	PtrArray(int siz_){
		size_ = siz_;
		table.resize(size_);
	}
	~PtrArray(){
		for (size_t i = 0; i < table.size(); ++i){
			delete table[i];
		}
	}
	int size(){
		return size_;//vector ��size ����ʵ����ֻ������ָ����������
	}
	//����[]�����������ع�����ָ����Ϊ��ֵ Խ���Զ������ڴ�
	Base*& operator[](unsigned index){
		if (index >= size_){
			size_ = index + 1;//ʵ��ʹ���ڴ�
			table.resize(index * 2 + 1);
			//_DEBUG_ERROR("PtrArray subscript out of range");
		}
		if (table[index] == NULL)table[index] = new Base();
		return table[index];
	}
	//�ַ��� hash����ʾ��
	static int strHash(char *key){
		//���Ϊ182789
		return
			(key[0] - 'A') * 26 * 26 * 10 +
			(key[1] - 'A') * 26 * 10 +
			(key[2] - 'A') * 10 +
			(key[3] - '0');
	}
	//�����ֵ��ַ�����ʽhash���� 5λmax=9w
	int numStrHash(char *key){
		int sum =
			(key[0] - '0') * 10000 +
			(key[1] - '0') * 1000 +
			(key[2] - '0') * 100 +
			(key[3] - '0') * 10 +
			(key[4] - '0') * 1;
		return sum % 30000;
	}
};

//���������
struct FamilyTreeNode{
	int bCnt = -1;//����ǰ�м����ո� ����������������һ��
	string father;
	string name;
	bool operator>(FamilyTreeNode const &rhs){
		return name > rhs.name;
	}
	bool operator==(FamilyTreeNode const &rhs){
		return name == rhs.name;
	}
	bool operator<=(FamilyTreeNode const &rhs){
		return name <= rhs.name;
	}
	bool operator>=(FamilyTreeNode const &rhs){
		return name >= rhs.name;
	}
	bool operator<(FamilyTreeNode const &rhs){
		return name < rhs.name;
	}
	FamilyTreeNode(){}
	FamilyTreeNode(string name_, int cnt){
		name = name_;
		bCnt = cnt;
	}
	FamilyTreeNode(string name_){
		name = name_;
	}
	~FamilyTreeNode(){}
	//���ص�һ���ɼ��ַ�ǰ�Ŀո���
	static int blnakCount(){
		int count = 0;
		char c;
		while ((c = getchar()) == ' ' || c == '\n'){
			if (c == ' ')
				++count;
		}
		ungetc(c, stdin);
		return count;
	}
};

//Ŀ¼(Catalog)�ṹ
struct CatalogNode{
	string name;
	//(set�ڵ�Ԫ���������)
	set<string> subFolder;//���ļ���������
	set<string> file;//�ļ�������
	CatalogNode(){}
	CatalogNode(string name_){
		name = name_;
	}
	~CatalogNode(){}
	bool operator==(CatalogNode const &rhs)const{
		return name == rhs.name;
	}
	bool operator!=(CatalogNode const &rhs)const{
		return !(*this == rhs);
	}
	bool operator<(CatalogNode const &rhs)const{
		return name < rhs.name;
	}
	bool operator>(CatalogNode const &rhs)const{
		return name > rhs.name;
	}
	bool operator<=(CatalogNode const &rhs)const{
		return !(*this > rhs);
	}
	bool operator>=(CatalogNode const &rhs)const{
		return !(*this < rhs);
	}
	//����������ȡһ�����ִ��ڶ���name��   �ļ���������true  �ļ�������false
	bool getAName(){
		name.clear();
		char c;
		while ((c = getchar()) != '\n' && c != '\\'){
			name += c;
		}
		return c == '\\';
	}
	void print(int blankCnt){
		for (int i = 0; i < blankCnt; ++i)
			printf(" ");
		//cout << name << endl;
		puts(&(name)[0]);
	}
	//������ļ����ڵ���������(����������, root���ǰ�Ŀո�����)
	void output(AvlTree<CatalogNode> *t, int blankCnt){
		static CatalogNode tmp;
		print(blankCnt);
		//������ļ���
		for (auto i = subFolder.begin(); i != subFolder.end(); ++i){
			tmp.name = *i;
			t->find(tmp)->Data.output(t, blankCnt + 2);
		}//����ļ�name
		for (auto i = file.begin(); i != file.end(); ++i){
			for (int j = 0; j < blankCnt + 2; ++j)
				printf(" ");
			puts(&(*i)[0]);
		}
	}
};

struct Accounts{
	using myInt = int;
	myInt account;//unsigned �޷���ȫӳ��10λ�� ���г�ͻ
	string password;
	bool operator<(Accounts const &rhs)const{
		return account < rhs.account;
	}
	bool operator>(Accounts const &rhs)const{
		return account > rhs.account;
	}
	bool operator==(Accounts const &rhs)const{
		return account == rhs.account;
	}
	//δ֪�ط�����Ĭ�Ϲ���
	Accounts(){}
	//������
	Accounts(myInt a, string &pa){
		account = a, password = pa;
	}
	//������
	Accounts(myInt a){
		account = a;
	}
	myInt hash()const{
		return account % 70000;//�ƺ���������������С���ʱЧ�����
	}
	/*
	map<string, string>
	0	sample ȫ��5�������Ϣ											����ȷ	2 ms	252KB
	1	������ȫ���������룬����ȫ��16λ									����ȷ	715 ms	19760KB
	2	N��Lָ���һ�룬����������ʺ������ȡ�����½硣���������ȡ�����½�	����ȷ	669 ms	10576KB

	HashAvlTable<Acounts> t = HashAvlTable<Acounts>(100000)
	0	sample ȫ��5�������Ϣ											����ȷ	7.6 ms	8012KB
	1	������ȫ���������룬����ȫ��16λ									����ȷ	842 ms	21340KB
	2	N��Lָ���һ�룬����������ʺ������ȡ�����½硣���������ȡ�����½�	����ȷ	601 ms	15432KB

	AvlTree<Acounts>
	0	sample ȫ��5�������Ϣ											����ȷ	2 ms	248KB
	1	������ȫ���������룬����ȫ��16λ									���г�ʱ	0 ms	0KB  > 1200ms
	2	N��Lָ���һ�룬����������ʺ������ȡ�����½硣���������ȡ�����½�	���г�ʱ	0 ms	0KB
	*/
};

struct Olympic{
	int id = -1, gold = 0, medal = 0, population = 0;
	//�漰double�Ƚ� �ظ�Ԫ��
	//double data_[5];//0ID 1���� 2���� 3�˾����� 4�˾�����(per capita)
	Olympic(){}
	~Olympic(){}
	bool operator<(Olympic const &rhs)const{
		return id < rhs.id;
	}
	bool operator>(Olympic const &rhs)const{
		return id > rhs.id;
	}
	bool operator==(Olympic const &rhs)const{
		return id == rhs.id;
	}
};

//������score �����id
struct Student{
	char id[14];//����(13λ������)
	int score = 0;//�÷֣�Ϊ[0,100]�����ڵ�������
	int place = 0, rank = 1;//������ ����
	int localRank = 1;//�ڸÿ��������
	Student(){
		memset(id, 0, 14 * sizeof(char));
	}
	Student(Student const&rhs){
		*this = rhs;
	}
	~Student(){}
	//������score �������ʱ���μ��Ӵ�С����
	//RE_ORDER�ɼ��Ըߵ��� id��С���� (����<ʱid�� > ����ʵ�����Ǵ�С ��������RE_ORDER ���С����)
	bool operator<(Student const &rhs)const{
		/*�ȼ�д��
		if (score == rhs.score)
		return strcmp(id, rhs.id) > 0;
		return score < rhs.score;
		*/
		return score < rhs.score ||
			score == rhs.score && strcmp(id, rhs.id) > 0;
	}
	bool operator>(Student const &rhs)const{
		return score > rhs.score ||
			score == rhs.score && strcmp(id, rhs.id) < 0;
	}
	bool operator==(Student const &rhs)const{
		return score == rhs.score && strcmp(id, rhs.id) == 0;
	}
	//���š����������������š��ڸÿ����������
	void outPut(){
		printf("%s %d %d %d\n", id, rank, place, localRank);
	}
};

#endif