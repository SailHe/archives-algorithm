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
	typedef class HuTrNode : public BinTree<T>::BTNode {
	public:
		int Weight = 0;//权重  Data作为 编码字符
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
		// 做usedSize-1次合并 每次将权值最小的两颗树合并 但是运算途中usedSize会变化 所以用n
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
		//str.resize(maxLen);//str的length() usedSize都是maxLen 于是弃用 但是若换成new char 又回到了内存管理的问题
		//cin.getline(&str[0], maxLen, end);
		char* str = (char*)malloc(sizeof(char)* maxLen);
		memset(str, 0, sizeof(char)* maxLen);
		if (get) gets(str);
		return str;
	}
	//返回编码长度(编码字串)
	static int codeLen(char *code) {
		return strlen(code);
	}
	static int codeLen(JCE::String const&code) {
		return code.length();
	}
	/*tips:
	depth = codeLen+1 <-> depth(root_)=1; (正规的, 书上的depth定义)
	depth = codeLen  <--> depth(root_)=0
	(所以: 文本长度=权重*编码长度, WPL=权重*(编码长度+1))
	*/
	//返回文本的WPL
	static int wpl(JCE::ArrayList<char*> &s, int *freq) {
		int textLen, i, n = s.size();
		for (textLen = i = 0; i < n; ++i) {
			textLen += freq[i] * (codeLen(s[i]) + 1);
		}
		return textLen;
	}
	//返回一套编码的文本长度 = freq*codeLen
	static int textLen(JCE::ArrayList<char*> &s, int *freq) {
		int textLen, i, n = s.size();
		for (textLen = i = 0; i < n; ++i) {
			textLen += freq[i] * codeLen(s[i]);
		}
		return textLen;
	}
	//检查文本是否前缀码 //string const codes[]-> 与用ArrayList存效果差不多?
	static bool isPrefixCodes(JCE::ArrayList<JCE::String> const &codes) {
		int n = codes.size();
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j != i && j < n; j++) {
				if (codes[j].find(codes[i]) == 0)//前缀冲突
					return false;
			}
		}
		return true;
	}
	//返回本huffman编码的带权路径长度 [路径长度]:从根结点到目标结点须经过的总边数(分支数 )(经过的结点总数-1)
	int wpl() {
		return WPL(root_, 1);
	}
	//返回本huffman编码的(最短)文本长度 (空间占位 = textLen*1)
	int textLen() {
		return textLen(root_, 1);
	}
	//判断是否最优编码
	bool isOptimalCoding(JCE::ArrayList<JCE::String> &codes, int *freq) {
		//也可以比较wpl
		int textLen, i, len, n = codes.size();
		for (textLen = i = 0; i < n; ++i) {
			len = codeLen(codes[i]);
			if (len > n - 1)/*n个结点最长编码长度为n-1*/
				return false;//效率优于直接计算并比较textLen
			textLen += freq[i] * len;
		}
		return textLen == this->textLen() ? true : false;
	}
	//返回本huffman编码文本的空间占用=codeLen*sizeof(字符)
	int memory() {
		return textLen() * sizeof(T);
	}
private:
	void destroy(HuTr &t) {
		Position tmp = t;
		//可以直接调用 但引用不能传给父类
		BinTree<T>::destroy(tmp);
		t = NULL;
	}
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
	//带权路径长度(weightted path length)=sum(weight*depth)
	int WPL(Position t, int depth) {
		if (!t->Left && !t->Right)
			return t->getValue()*depth;
		else/*否则一定有两个孩子*/
			return WPL(t->Left, depth + 1) + WPL(t->Right, depth + 1);
	}
	//(最短)文本长度 = freq[i]*codeLen
	int textLen(Position t, int depth) {
		if (!t->Left && !t->Right)
			return t->getValue()*(depth - 1);//tips:depth(root_) = 1
		else/*否则一定有两个孩子*/
			return textLen(t->Left, depth + 1) + textLen(t->Right, depth + 1);
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
class ArrayHuffman {
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
	//从1到upbound中找出father为0的节点赋给s1,s2,（为了保证答案唯一，请让s1的节点编号小于s2）
	static void SelectTwoMin(int upbound, HuffmanTree ht/*这tm是个数组!!!*/, int &s1, int &s2) {
		s1 = s2 = 0;
		//找出weight最小的两个的sub
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
	//查找并返回数组哈夫曼树中某个权值的结点的下标 不存在返回0
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
	//构造哈夫曼树ht 并 计算哈夫曼编码hc w为权值(频率)数组 n为权值个数 (需要手动free)
	static void HuffmanCoding(HuffmanTree &ht, HuffmanCode &hc, int *w, int n) {
		// 构造哈夫曼树(伪)
		ht = (HuffmanTree)malloc(sizeof(HTNode)*(2 * n));
		memset(ht, 0, (sizeof(HTNode)*(2 * n)));
		ht[0].weight = 1001;
		for (int i = 0; i < n; ++i) ht[i + 1].weight = w[i];
		//做n-1次合并 每次将权值最小的两颗树合并
		int size = n;
		for (int i = 1; i < n; ++i) {
			++size;
			//"pop" and "push"
			SelectTwoMin(size, ht, ht[size].lchild, ht[size].rchild);
			//计算新权值
			ht[size].weight = ht[ht[size].lchild].weight + ht[ht[size].rchild].weight;
			ht[ht[size].lchild].parent = ht[ht[size].rchild].parent = size;
		}
		// 计算编码
		hc = (HuffmanCode)malloc(sizeof(char*)*n);
		memset(hc, 0, (sizeof(char)*n));
		for (int i = 0; i < n; ++i) {
			hc[i] = (char*)malloc(sizeof(char)* n);
			memset(hc[i], 0, sizeof(char)* n);
			//n结点最长编码长度n - 1+'\0' = n 下标c为顶级结点下标size时终止
			for (int c = sub(ht, w[i], size), codeIndex = 0; c != size; c = ht[c].parent) {
				int parent = ht[c].parent;
				//hc[i][codeIndex++] = ht[parent].lchild == c ? '0' : '1';
				//左子树代表频度 < 右子树频度 编码为0
				hc[i][codeIndex++] = ht[parent].lchild == c ? '1' : '0';
			}
		}
		for (int i = 0; i < n; ++i) {
			//将每个编码逆转
			for (int lhs = 0, len = strlen(hc[i]); lhs < len / 2; ++lhs) {
				int rhs = len - lhs - 1;
				swapC(hc[i][lhs], hc[i][rhs]);
			}
		}
	}

	//返回编码长度
	static int codingLen(ArrayHuffman::HuffmanCode const hufCode, int n, int *weightList) {
		int sum = 0;
		for (int i = 0; i < n; ++i) {
			sum += weightList[i] * strlen(hufCode[i]);
		}
		return sum;
	}
};

