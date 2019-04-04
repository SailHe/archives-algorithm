#pragma once
#include "TreeObject.h"
#include "VirtualTree.h"


/*
没有度为1的结点
n个叶子结点的huffman树的节点数=2n-1
*/
/*带权路径最小的二叉树称为最优二叉树或huffman树(结构并不唯一)
huffman树的判断标准:最优前缀编码树 <==>最优编码(WPL或textLen最小) && 前缀码(短码不能是长码的前缀)*/
template<class T>
class HuffmanTree : public BinTree<T> {
protected:
	using BinTree<T>::root_;
	//子类需要递归子结点的地方必定用到Left-Right只能用BinTree的Position-论getValue()存在的必要性
	using Position = typename BinTree<T>::Position;
	using Element = typename BinTree<T>::Element;
	typedef class HuTrNode : public BinTreeAlgorithm::BinTreeNode<T> {
	public:
		int Weight = 0;//权重  Data作为 编码字符
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
			DE_PRINTF("HuTrNode析构");
		}
	}*HuTr;//哈夫曼树
public:
	/*
	任意句子构造
	哈夫曼树的构建 最小堆实现 (字符序列, 权重序列(一般是频值freq), 序列公共大小, huf叶子结点数)
	频值向量freq[0]代表权值个数 权值freq[i]>=0; i[1,*freq) O(N*logN)
	*/
	HuffmanTree(T *character, int *freq, int nHuf) {
		// 使用new构造哨兵(没有重载运算符: 赋值, 比较)
		HuTr sentry = new HuTrNode(0, -MAX_INT32 / 2);
		Heap<HuTr> h = Heap<HuTr>(nHuf, sentry, [](const HuTr &lhs, const HuTr &rhs) {
			return rhs->Weight - lhs->Weight;
			// return rhs->Weight < lhs->Weight ? : -1 : rhs->Weight == lhs->Weight ? 0 : 1;
		});
		for (int i = 0; i < nHuf; ++i)	h.push(new HuTrNode(character[i], freq[i]));
		// 做validNodeNum-1次合并 每次将权值最小的两颗树合并 但是运算途中validNodeNum会变化 所以用n
		while (nHuf-- > 1) {
			// 当泛型为char时 无效结点为'\0'不会输出(自动省略无效的非叶结点) 否则需要重写遍历方法
			HuTr ht = new HuTrNode(0, 0);
			ht->Left = h.pop();
			ht->Right = h.pop();
			// 计算新权值
			ht->setValue(ht->Left->getValue() + ht->Right->getValue());
			h.push(ht);
		}
		root_ = h.pop();
		// 除了哨兵 其余所有new的元素都在ht中受其管理
		delete sentry;
	}
	~HuffmanTree()override {
		DE_PRINTF("HuffmanTree析构");
	}

	//清空输入流的换行与空格并返回非换行空格的那个字符 不可见的(Invisible)
	static void flushBlanckLF() {
		char c;
		while ((c = getchar()) == '\n' || c == ' ');
		ungetc(c, stdin);
	}
	//返回长度为maxLen的char型字符串 若get = true自动调用gets(str);
	static char* newSentence(int maxLen, bool get = false, char end = '\n') {
		//str.resize(maxLen);//str的length() validNodeNum都是maxLen 于是弃用 但是若换成new char 又回到了内存管理的问题
		//cin.getline(&str[0], maxLen, end);
		char* str = (char*)malloc(sizeof(char)* maxLen);
		memset(str, 0, sizeof(char)* maxLen);
		if (get) gets(str);
		return str;
	}
	// 返回编码长度|即霍夫曼树中的路径长度 (编码字串)
	static int codeLen(char const *code) {
		int len = -1;
		Utility::tryToSignedNum(strlen(code), len);
		return len;
	}
	static int codeLen(JCE::String const &code) {
		return code.length();
	}
	// 返回第一个子串的下标 没有返回-1
	static int findSubStr(char const *code, char const *subCode) {
		char const *fR = std::strstr(code, subCode);
		return fR == NULL ? -1 : code - fR;
	}
	static int findSubStr(JCE::String const &code, JCE::String const &subCode) {
		return code.find(subCode);
	}
	/*tips:
	depth = codeLen+1 <-> depth(root_)=1; (书上P119的depth定义, 但是维基上定义的不同, 根结点层数为0, 高度, 深度都是同一个概念, 这个倒是一致)
	depth = codeLen  <--> depth(root_)=0
	(所以: 文本长度=权重*编码长度, WPL=权重*(编码长度+1))
	*/
	// 直接计算文本的WPL StringIterator里面必须是string或char*
	template<typename StringIterator, typename FreqIterator>
	static int textLen(StringIterator codeBegin, StringIterator codeEnd, FreqIterator freqBegin) {
		// 文本编码长度|文本长度 = (空间占位 = codeLen*1)
		int textLenSum = 0;
		while (codeBegin != codeEnd) {
			textLenSum += *freqBegin * codeLen(*codeBegin);
			++codeBegin;
			++freqBegin;
		}
		return textLenSum;
	}
	// 若文本是前缀码返回true
	template<typename StringIterator>
	static bool isPrefixCodes(StringIterator codeBegin, StringIterator codeEnd) {
		while (codeBegin != codeEnd) {
			StringIterator codeBegin0 = codeBegin;
			++codeBegin;
			StringIterator codeBegin1 = codeBegin;
			while (codeBegin1 != codeEnd) {
				// 首个子串位于0时表示前缀冲突
				if (findSubStr(*codeBegin0, *codeBegin1) == 0)
					return false;
				++codeBegin1;
			}
		}
		return true;
	}
	// 返回本huffman编码的带权路径长度 [路径长度]:从根结点到目标结点须经过的总边数|分支数 (经过的结点总数-1)
	int wpl() {
		return WPL(root_, 0);
	}
	//判断是否最优编码
	bool isOptimalCoding(JCE::ArrayList<JCE::String> &codes, int *freq) {
		int codeLen = 0, i = 0, len = 0, n = codes.size();
		for (; i < n; ++i) {
			len = textLen(codes[i]);
			// n个结点最长编码长度为n-1 (效率优于直接计算并比较codeLen)
			if (len > n - 1)
				return false;
			codeLen += freq[i] * len;
		}
		return codeLen == wpl() ? true : false;
	}
	// 返回本huffman编码文本的空间占用=codeLen*sizeof(字符)
	int memory() {
		return wpl() * sizeof(T);
	}
private:
	//返回给出的编码的前缀编码树 若所给的编码不是前缀码返回 NULL 若有频值则按频值依次填入 否则全部置1(有未知bug 若判断一套编码是否huffman编码)
	HuTr buildPreCodeTree(JCE::ArrayList<JCE::String> const &codes, int *freq = NULL) {
		int n = codes.size(), sub = 0;
		HuTr rt = new HuTrNode(0, 0);
		for (int i = 0; i < n; i++) {
			int len = codeLen(codes[i]);
			Position t = rt;
			for (int j = 0; j < len; j++) {
				if (codes[i][j] == '1') {//1向左
					if (!t->Left)//为空 建结点
						t->Left = new HuTrNode(0, 0);
					else if (t->getValue() != 0) {//不为空判断继续进行还是中止
						destroy(rt);
						return NULL;
					}
					t = t->Left;
				}
				else {//0向右
					if (!t->Right)
						t->Right = new HuTrNode(0, 0);
					else if (t->getValue() != 0) {//建树途中遇见带权结点 ->该编码并非前缀码
						destroy(rt);
						return NULL;
					}//else do nothing
					t = t->Right;
				}
			}
			if (!t->isLeave())//若不是叶结点 则必不是前缀码
				return NULL;
			t->setValue(freq == NULL ? 1 : freq[sub++]);//赋权
		}
		return rt;
	}
	//[最短]文本长度|带权路径长度(weightted path length)=sum(weight*depth)
	//tips: depth(root_) = 1; 路径长度(即路径上的链接的个数, 即二值编码的个数, 即编码的长度)
	int WPL(Position t, int depth) {
		if (t->Left == nullptr && t->Right == nullptr)
			return t->getValue()*depth;
		else// 否则一定有两个孩子
			return WPL(t->Left, depth + 1) + WPL(t->Right, depth + 1);
	}
	//返回仅含大写英文句子的频率数组 改为map?
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
	//返回一个仅含大写英文字母和下划线的句子的哈夫曼编码所占内存,普通编码memory = sentence.length()*8
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
			mem += tmp;//其实就是把越小的频率反复多加几次，越大的频率少加几次
		}
		return mem == 0 ? sentence.length() : mem;
	}
};

/*数组式huffman*/
class DSAUTILITYEXTENSION_API ArrayHuffman {
	struct HTNode {
		//权重
		int weight;
		//父结点
		int parent;
		//左孩子
		int lchild;
		//右孩子
		int rchild;
		//只用于sub方法查找是是否已使用的判断 注意此处借用了memset的初始化巧合
		bool isUsed;
	};
	typedef char ** HuffmanCode;
public:
	typedef HTNode const *HuffmanTreeArray;

protected:
	typedef HTNode *HTNodeArr;
	// 从1到upbound中找出father为0的节点赋给s1,s2 (为保证答案唯一: assert(s1 <= s2))
	static void selectTwoMinNode(int upbound, HuffmanTreeArray htArr, int &s1, int &s2);
	// 查找并返回数组哈夫曼树中某个权值的结点的下标 不存在返回0
	static int findSub(HTNodeArr htArr, int wei, int n);

	// 计算哈夫曼编码
	void huffmanCoding(int const *w, int n, std::vector<std::string> &huffmanCodes);

public:
	// (权值|频率数组, 权值个数, 待计算的hf编码)
	ArrayHuffman(int const *w, int n, std::vector<std::string> &huffmanCodes);
	~ArrayHuffman();

	HuffmanTreeArray getHuffmanTreeArray();

private:
	int currentSub = 0;
	HTNodeArr htArr = nullptr;
};

