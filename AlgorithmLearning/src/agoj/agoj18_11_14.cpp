#include<stdio.h>
#include<string.h>
#include<string>
#include<cmath>
#include<vector>
#include<iostream>
#include<algorithm>
#include<queue>
#include<functional>//greater
#include<list>
#include<stack>
#include<map>
#include<iomanip>//cout���ֲ�����
#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

#define MAX_IN32 2147483647
#define MIN_IN32 (-MAX_IN32-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>
#define FOR(it, lowerBound, terminator) for (auto it = lowerBound; it != terminator; ++it)

using namespace std;

template<class T> using ArrayList = vector<T>;
template<class T> using LinkedList = list<T>;
using SizeType = size_t;
using I64 = long long;

/*����ʽhuffman ͬ11-14ex�汾*/
class ArrayHuffman {
	struct HTNode{
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

int hufWeightList[1000 * 2];

//http://139.196.145.92/contest_show.php?cid=622#problem/A
int mainForSolveA_11_14() {
	int n;
	while (cin >> n) {
		//ArrayList<int> hufList(n * 2);
		ArrayHuffman::HuffmanTree hufTree = nullptr;
		ArrayHuffman::HuffmanCode hufCode = nullptr;
		memset(hufWeightList, 0, n * 2 * sizeof(int));
		for (int i = 0; i < n; ++i) {
			scanf("%d", hufWeightList + i);
		}
		ArrayHuffman::HuffmanCoding(hufTree, hufCode, hufWeightList, n);
		printf("%d\n", ArrayHuffman::codingLen(hufCode, n, hufWeightList));
		free(hufTree); hufTree = nullptr;
		free(hufCode); hufCode = nullptr;
	}
	return 0;
}

//������ѹ�� û��д�� 
int mainForSolveE_11_14() {
	int nCase;
	string buffer;
	buffer.resize(10000);
	while (cin >> nCase) {
		for (int caseCnt = 0; caseCnt < nCase; ++caseCnt) {
			//ArrayList<int> hufList(n * 2);
			ArrayHuffman::HuffmanTree hufTree = nullptr;
			ArrayHuffman::HuffmanCode hufCode = nullptr;
			string str = "";
			str.reserve(100);
			//�пո���ֱ��cin>>str
			char tempC = getchar();
			while((tempC = getchar()) != '\n') {
				str.push_back(tempC);
			}
			int n = (int)str.length(), testCnt = -1;
			memset(hufWeightList, 0, n * 2 * sizeof(int));
			for (int i = 0; i < n; ++i) {
				scanf("%d", hufWeightList + i);
			}
			//�������������
			ArrayHuffman::HuffmanCoding(hufTree, hufCode, hufWeightList, n);
			std::map<char, string> dictionaries;
			//������ת��Ϊ�ֵ�
			for (int i = 0; i < n; ++i) {
				dictionaries.emplace(str[i], string(hufCode[i]));
			}
			cin >> testCnt;
			tempC = getchar();
			
			for (int i = 0; i < testCnt; ++i) {
				//�˴���string��char*��
				gets_s(&buffer[0], buffer.length());
				int len = strlen(&buffer[0]);
				for (int j = 0; j < len; ++j) {
					tempC = buffer[j];
					printf("%s ", dictionaries[tempC].c_str());
				}
			}
			//printf("%d\n", codingLen(hufCode, n, hufWeightList));
			free(hufTree); hufTree = nullptr;
			free(hufCode); hufCode = nullptr;
		}
	}
	return 0;
}


int main() {
	mainForSolveA_11_14();
	mainForSolveE_11_14();
	return 0;
}


