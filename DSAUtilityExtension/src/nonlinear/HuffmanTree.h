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
	typedef class HuTrNode : public BinTreeAlgorithm::BinTreeNode<T> {
	public:
		int Weight = 0;//Ȩ��  Data��Ϊ �����ַ�
		int getValue()override {
			return Weight;
		}
		void setValue(int w)override {
			Weight = w;
		}
		HuTrNode(int tData, int weight_)
			:BinTreeAlgorithm::BinTreeNode<T>(tData) {
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
		// ��validNodeNum-1�κϲ� ÿ�ν�Ȩֵ��С���������ϲ� ��������;��validNodeNum��仯 ������n
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
		//str.resize(maxLen);//str��length() validNodeNum����maxLen �������� ����������new char �ֻص����ڴ���������
		//cin.getline(&str[0], maxLen, end);
		char* str = (char*)malloc(sizeof(char)* maxLen);
		memset(str, 0, sizeof(char)* maxLen);
		if (get) gets(str);
		return str;
	}
	// ���ر��볤��|�����������е�·������ (�����ִ�)
	static int codeLen(char const *code) {
		int len = -1;
		Utility::tryToSignedNum(strlen(code), len);
		return len;
	}
	static int codeLen(JCE::String const &code) {
		return code.length();
	}
	// ���ص�һ���Ӵ����±� û�з���-1
	static int findSubStr(char const *code, char const *subCode) {
		char const *fR = std::strstr(code, subCode);
		return fR == NULL ? -1 : code - fR;
	}
	static int findSubStr(JCE::String const &code, JCE::String const &subCode) {
		return code.find(subCode);
	}
	/*tips:
	depth = codeLen+1 <-> depth(root_)=1; (����P119��depth����, ����ά���϶���Ĳ�ͬ, ��������Ϊ0, �߶�, ��ȶ���ͬһ������, �������һ��)
	depth = codeLen  <--> depth(root_)=0
	(����: �ı�����=Ȩ��*���볤��, WPL=Ȩ��*(���볤��+1))
	*/
	// ֱ�Ӽ����ı���WPL StringIterator���������string��char*
	template<typename StringIterator, typename FreqIterator>
	static int textLen(StringIterator codeBegin, StringIterator codeEnd, FreqIterator freqBegin) {
		// �ı����볤��|�ı����� = (�ռ�ռλ = codeLen*1)
		int textLenSum = 0;
		while (codeBegin != codeEnd) {
			textLenSum += *freqBegin * codeLen(*codeBegin);
			++codeBegin;
			++freqBegin;
		}
		return textLenSum;
	}
	// ���ı���ǰ׺�뷵��true
	template<typename StringIterator>
	static bool isPrefixCodes(StringIterator codeBegin, StringIterator codeEnd) {
		while (codeBegin != codeEnd) {
			StringIterator codeBegin0 = codeBegin;
			++codeBegin;
			StringIterator codeBegin1 = codeBegin;
			while (codeBegin1 != codeEnd) {
				// �׸��Ӵ�λ��0ʱ��ʾǰ׺��ͻ
				if (findSubStr(*codeBegin0, *codeBegin1) == 0)
					return false;
				++codeBegin1;
			}
		}
		return true;
	}
	// ���ر�huffman����Ĵ�Ȩ·������ [·������]:�Ӹ���㵽Ŀ�����뾭�����ܱ���|��֧�� (�����Ľ������-1)
	int wpl() {
		return WPL(root_, 0);
	}
	//�ж��Ƿ����ű���
	bool isOptimalCoding(JCE::ArrayList<JCE::String> &codes, int *freq) {
		int codeLen = 0, i = 0, len = 0, n = codes.size();
		for (; i < n; ++i) {
			len = textLen(codes[i]);
			// n���������볤��Ϊn-1 (Ч������ֱ�Ӽ��㲢�Ƚ�codeLen)
			if (len > n - 1)
				return false;
			codeLen += freq[i] * len;
		}
		return codeLen == wpl() ? true : false;
	}
	// ���ر�huffman�����ı��Ŀռ�ռ��=codeLen*sizeof(�ַ�)
	int memory() {
		return wpl() * sizeof(T);
	}
private:
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
	//[���]�ı�����|��Ȩ·������(weightted path length)=sum(weight*depth)
	//tips: depth(root_) = 1; ·������(��·���ϵ����ӵĸ���, ����ֵ����ĸ���, ������ĳ���)
	int WPL(Position t, int depth) {
		if (t->Left == nullptr && t->Right == nullptr)
			return t->getValue()*depth;
		else// ����һ������������
			return WPL(t->Left, depth + 1) + WPL(t->Right, depth + 1);
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
class DSAUTILITYEXTENSION_API ArrayHuffman {
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
	typedef char ** HuffmanCode;
public:
	typedef HTNode const *HuffmanTreeArray;

protected:
	typedef HTNode *HTNodeArr;
	// ��1��upbound���ҳ�fatherΪ0�Ľڵ㸳��s1,s2 (Ϊ��֤��Ψһ: assert(s1 <= s2))
	static void selectTwoMinNode(int upbound, HuffmanTreeArray htArr, int &s1, int &s2);
	// ���Ҳ������������������ĳ��Ȩֵ�Ľ����±� �����ڷ���0
	static int findSub(HTNodeArr htArr, int wei, int n);

	// �������������
	void huffmanCoding(int const *w, int n, std::vector<std::string> &huffmanCodes);

public:
	// (Ȩֵ|Ƶ������, Ȩֵ����, �������hf����)
	ArrayHuffman(int const *w, int n, std::vector<std::string> &huffmanCodes);
	~ArrayHuffman();

	HuffmanTreeArray getHuffmanTreeArray();

private:
	int currentSub = 0;
	HTNodeArr htArr = nullptr;
};

