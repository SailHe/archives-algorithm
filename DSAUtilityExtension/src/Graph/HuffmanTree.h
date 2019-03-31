#pragma once
#include "TreeObject.h"
#include "VirtualTree.h"


/*
û�ж�Ϊ1�Ľ��
n��Ҷ�ӽ���huffman���Ľڵ���=2n-1
*/
/*��Ȩ·����С�Ķ�������Ϊ���Ŷ�������huffman��(�ṹ����Ψһ)
huffman�����жϱ�׼:����ǰ׺������ <==>���ű���(WPL��textLen��С) && ǰ׺��(���벻���ǳ����ǰ׺)*/
template<class T>
class HuffmanTree : public BinTree<T> {
protected:
	using BinTree<T>::root_;
	//������Ҫ�ݹ��ӽ��ĵط��ض��õ�Left-Rightֻ����BinTree��Position-��getValue()���ڵı�Ҫ��
	using Position = typename BinTree<T>::Position;
	using Element = typename BinTree<T>::Element;
	typedef class HuTrNode : public BinTree<T>::BTNode {
	public:
		int Weight = 0;//Ȩ��  Data��Ϊ �����ַ�
		int getValue()override {
			return Weight;
		}
		void setValue(int w)override {
			Weight = w;
		}
		HuTrNode(int tData, int weight_)
			:BinTree<T>::BTNode(tData) {
			Weight = weight_;
		}
		~HuTrNode()override {
			DE_PRINTF("HuTrNode����");
		}
	}*HuTr;//��������
public:
	/*
	������ӹ���
	���������Ĺ��� ��С��ʵ�� (�ַ�����, Ȩ������(һ����Ƶֵfreq), ���й�����С, hufҶ�ӽ����)
	Ƶֵ����freq[0]����Ȩֵ���� Ȩֵfreq[i]>=0; i[1,*freq) O(N*logN)
	*/
	HuffmanTree(T *character, int *freq, int nHuf) {
		// ʹ��new�����ڱ�(û�����������: ��ֵ, �Ƚ�)
		HuTr sentry = new HuTrNode(0, -MAX_INT32 / 2);
		Heap<HuTr> h = Heap<HuTr>(nHuf, sentry, [](const HuTr &lhs, const HuTr &rhs) {
			return rhs->Weight - lhs->Weight;
			// return rhs->Weight < lhs->Weight ? : -1 : rhs->Weight == lhs->Weight ? 0 : 1;
		});
		for (int i = 0; i < nHuf; ++i)	h.push(new HuTrNode(character[i], freq[i]));
		// ��usedSize-1�κϲ� ÿ�ν�Ȩֵ��С���������ϲ� ��������;��usedSize��仯 ������n
		while (nHuf-- > 1) {
			// ������Ϊcharʱ ��Ч���Ϊ'\0'�������(�Զ�ʡ����Ч�ķ�Ҷ���) ������Ҫ��д��������
			HuTr ht = new HuTrNode(0, 0);
			ht->Left = h.pop();
			ht->Right = h.pop();
			// ������Ȩֵ
			ht->setValue(ht->Left->getValue() + ht->Right->getValue());
			h.push(ht);
		}
		root_ = h.pop();
		// �����ڱ� ��������new��Ԫ�ض���ht���������
		delete sentry;
	}
	~HuffmanTree()override {
		DE_PRINTF("HuffmanTree����");
	}

	//����������Ļ�����ո񲢷��طǻ��пո���Ǹ��ַ� ���ɼ���(Invisible)
	static void flushBlanckLF() {
		char c;
		while ((c = getchar()) == '\n' || c == ' ');
		ungetc(c, stdin);
	}
	//���س���ΪmaxLen��char���ַ��� ��get = true�Զ�����gets(str);
	static char* newSentence(int maxLen, bool get = false, char end = '\n') {
		//str.resize(maxLen);//str��length() usedSize����maxLen �������� ����������new char �ֻص����ڴ���������
		//cin.getline(&str[0], maxLen, end);
		char* str = (char*)malloc(sizeof(char)* maxLen);
		memset(str, 0, sizeof(char)* maxLen);
		if (get) gets(str);
		return str;
	}
	//���ر��볤��(�����ִ�)
	static int codeLen(char *code) {
		return strlen(code);
	}
	static int codeLen(JCE::String const&code) {
		return code.length();
	}
	/*tips:
	depth = codeLen+1 <-> depth(root_)=1; (�����, ���ϵ�depth����)
	depth = codeLen  <--> depth(root_)=0
	(����: �ı�����=Ȩ��*���볤��, WPL=Ȩ��*(���볤��+1))
	*/
	//�����ı���WPL
	static int wpl(JCE::ArrayList<char*> &s, int *freq) {
		int textLen, i, n = s.size();
		for (textLen = i = 0; i < n; ++i) {
			textLen += freq[i] * (codeLen(s[i]) + 1);
		}
		return textLen;
	}
	//����һ�ױ�����ı����� = freq*codeLen
	static int textLen(JCE::ArrayList<char*> &s, int *freq) {
		int textLen, i, n = s.size();
		for (textLen = i = 0; i < n; ++i) {
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
	int wpl() {
		return WPL(root_, 1);
	}
	//���ر�huffman�����(���)�ı����� (�ռ�ռλ = textLen*1)
	int textLen() {
		return textLen(root_, 1);
	}
	//�ж��Ƿ����ű���
	bool isOptimalCoding(JCE::ArrayList<JCE::String> &codes, int *freq) {
		//Ҳ���ԱȽ�wpl
		int textLen, i, len, n = codes.size();
		for (textLen = i = 0; i < n; ++i) {
			len = codeLen(codes[i]);
			if (len > n - 1)/*n���������볤��Ϊn-1*/
				return false;//Ч������ֱ�Ӽ��㲢�Ƚ�textLen
			textLen += freq[i] * len;
		}
		return textLen == this->textLen() ? true : false;
	}
	//���ر�huffman�����ı��Ŀռ�ռ��=codeLen*sizeof(�ַ�)
	int memory() {
		return textLen() * sizeof(T);
	}
private:
	void destroy(HuTr &t) {
		Position tmp = t;
		//����ֱ�ӵ��� �����ò��ܴ�������
		BinTree<T>::destroy(tmp);
		t = NULL;
	}
	//���ظ����ı����ǰ׺������ �������ı��벻��ǰ׺�뷵�� NULL ����Ƶֵ��Ƶֵ�������� ����ȫ����1(��δ֪bug ���ж�һ�ױ����Ƿ�huffman����)
	HuTr buildPreCodeTree(JCE::ArrayList<JCE::String> const &codes, int *freq = NULL) {
		int n = codes.size(), sub = 0;
		HuTr rt = new HuTrNode(0, 0);
		for (int i = 0; i < n; i++) {
			int len = codeLen(codes[i]);
			Position t = rt;
			for (int j = 0; j < len; j++) {
				if (codes[i][j] == '1') {//1����
					if (!t->Left)//Ϊ�� �����
						t->Left = new HuTrNode(0, 0);
					else if (t->getValue() != 0) {//��Ϊ���жϼ������л�����ֹ
						destroy(rt);
						return NULL;
					}
					t = t->Left;
				}
				else {//0����
					if (!t->Right)
						t->Right = new HuTrNode(0, 0);
					else if (t->getValue() != 0) {//����;��������Ȩ��� ->�ñ��벢��ǰ׺��
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
	int WPL(Position t, int depth) {
		if (!t->Left && !t->Right)
			return t->getValue()*depth;
		else/*����һ������������*/
			return WPL(t->Left, depth + 1) + WPL(t->Right, depth + 1);
	}
	//(���)�ı����� = freq[i]*codeLen
	int textLen(Position t, int depth) {
		if (!t->Left && !t->Right)
			return t->getValue()*(depth - 1);//tips:depth(root_) = 1
		else/*����һ������������*/
			return textLen(t->Left, depth + 1) + textLen(t->Right, depth + 1);
	}
	//���ؽ�����дӢ�ľ��ӵ�Ƶ������ ��Ϊmap?
	JCE::ArrayList<int> frequency(JCE::String sentence) {
		JCE::ArrayList<int> freq;
		freq.resize(26);
		for (unsigned i = 0; i < sentence.length(); ++i) {
			if (sentence[i] == '_')
				freq[26]++;
			else {
				freq[sentence[i] - 'A']++;
			}
		}
	}
	//����һ��������дӢ����ĸ���»��ߵľ��ӵĹ�����������ռ�ڴ�,��ͨ����memory = sentence.length()*8
	int huffmanMemory(JCE::String sentence) {
		static const int MAX_FREQ_SIZE = 128;
		int freq[MAX_FREQ_SIZE];
		memset(freq, 0, MAX_FREQ_SIZE * sizeof(int));

		for (unsigned i = 0; i < sentence.length(); ++i) {
			if (sentence[i] == '_')
				freq[26]++;
			else {
				freq[sentence[i] - 'A']++;
			}
		}
		std::priority_queue<int, JCE::ArrayList<int>, JCE::greater<int> > q;

		for (int i = 0; i < MAX_FREQ_SIZE; ++i) {
			if (freq[i] != 0)
				q.push(freq[i]);
		}
		int tmp = 0, mem = 0;
		while (q.size() > 1) {
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

