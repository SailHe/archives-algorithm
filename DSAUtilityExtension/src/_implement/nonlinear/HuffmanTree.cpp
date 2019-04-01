#include "../../nonlinear/HuffmanTree.h"

void ArrayHuffman::selectTwoMinNode(int upbound, HuffmanTreeArray htArr, int &s1, int &s2) {
	s1 = s2 = 0;
	// �ҳ�weight��С��������sub
	for (int i = 1; i < upbound; ++i) {
		if (htArr[i].parent == 0) {
			if (htArr[s1].weight > htArr[i].weight)
				s1 = i;
		}
	}
	for (int i = 1; i < upbound; ++i) {
		if (htArr[i].parent == 0) {
			if (htArr[s2].weight > htArr[i].weight && i != s1)
				s2 = i;
		}
	}
	if (s1 > s2) {
		std::swap(s1, s2);
	}
}
int ArrayHuffman::findSub(HTNodeArr htArr, int wei, int n) {
	for (int i = 1; i < n; ++i) {
		if (!htArr[i].isUsed && htArr[i].weight == wei) {
			htArr[i].isUsed = true;
			return i;
		}
	}
	return 0;
}

void ArrayHuffman::huffmanCoding(int const *w, int n, std::vector<std::string> &huffmanCodes) {
	HuffmanCode hc = (HuffmanCode)malloc(sizeof(char*)*n);
	memset(hc, 0, (sizeof(char)*n));
	for (int i = 0; i < n; ++i) {
		hc[i] = (char*)malloc(sizeof(char)* n);
		memset(hc[i], 0, sizeof(char)* n);
		// n�������볤��n - 1+'\0' = n �±�cΪ��������±�currentSubʱ��ֹ
		for (int c = findSub(htArr, w[i], currentSub), codeIndex = 0; c != currentSub; c = htArr[c].parent) {
			int parent = htArr[c].parent;
			//hc[i][codeIndex++] = htArr[parent].lchild == c ? '0' : '1';
			// ����������Ƶ�� < ������Ƶ�� ����Ϊ0
			hc[i][codeIndex++] = htArr[parent].lchild == c ? '1' : '0';
		}
	}
	for (int i = 0; i < n; ++i) {
		// ��ÿ��������ת
		for (int lhs = 0, len = strlen(hc[i]); lhs < len / 2; ++lhs) {
			int rhs = len - lhs - 1;
			std::swap(hc[i][lhs], hc[i][rhs]);
		}
	}

	for (int i = 0; i < n; ++i) {
		huffmanCodes.emplace_back(hc[i]);
	}
	free(hc);
	hc = nullptr;
}

ArrayHuffman::ArrayHuffman(int const *w, int n, std::vector<std::string> &huffmanCodes) {
	// �����������(α)
	htArr = (HTNodeArr)malloc(sizeof(HTNode)*(2 * n));
	memset(htArr, 0, (sizeof(HTNode)*(2 * n)));
	htArr[0].weight = MAX_INT32;
	for (int i = 0; i < n; ++i) htArr[i + 1].weight = w[i];
	// ��n-1�κϲ� ÿ�ν�Ȩֵ��С���������ϲ�
	currentSub = n;
	for (int i = 1; i < n; ++i) {
		++currentSub;
		// "pop" and "push"
		selectTwoMinNode(currentSub, htArr, htArr[currentSub].lchild, htArr[currentSub].rchild);
		// ������Ȩֵ
		htArr[currentSub].weight = htArr[htArr[currentSub].lchild].weight + htArr[htArr[currentSub].rchild].weight;
		// ��ֹ���
		assert(htArr[currentSub].weight > htArr[htArr[currentSub].lchild].weight);
		assert(htArr[currentSub].weight > htArr[htArr[currentSub].rchild].weight);
		htArr[htArr[currentSub].lchild].parent = htArr[htArr[currentSub].rchild].parent = currentSub;
	}
	huffmanCoding(w, n, huffmanCodes);
}
ArrayHuffman::~ArrayHuffman() {
	free(htArr);
	htArr = nullptr;
}

ArrayHuffman::HuffmanTreeArray ArrayHuffman::getHuffmanTreeArray() {
	return htArr;
}
