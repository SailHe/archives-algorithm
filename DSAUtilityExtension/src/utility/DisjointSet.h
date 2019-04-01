#pragma once
#include<vector>

// ֻ���漯�Ϲ�ϵ û�д��漯�ϵ�Ԫ��(�±� <==> Ԫ��)
//���鼯ģ���� unit join set
class DisjointSet {
public:
	// �ü��ϵ�root�±��ʾ����
	using SetRoot = int;
	// �ü���������±��ʾԪ��
	using ElementIndexType = int;
	// �����value��ʾ�伯�ϵ�'�����'(����Ԫ�ض�������Ϊ����� ��ͬһ������������Ԫ�ص�root�������ֻ��1��)
	using SetBaseType = std::vector<ElementIndexType>;
	//using SetBaseType = ElementIndexType[MAX_N];
	//��class����vector��http://139.196.145.92�ϻ�Runtime Error
	//vector<ElementIndexType> debugTemp1;

	DisjointSet() {}
	// ��ʼ��һ��������:[0, elementNum)�ĺ���elementNum�����ϵĲ��鼯 O(N)
	DisjointSet(size_t elementNum) {
		//�µ�Ԫ�س�ʼֵҲ������-1 ÿ��Ԫ�ض�����һ�������ļ���
		disjointSetBase.assign(elementNum, -1);
		//memset(disjointSetBase, -1, elementNum*sizeof(ElementIndexType));
	}

	// �ϲ��������� O(lgN)
	// �������ϲ�����true ���򷵻�false
	bool unionSet(SetRoot root1, SetRoot root2) {
		if (root1 == root2) {
			return false;
		}
		else {
			//��֤С���ϲ���󼯺�
			if (disjointSetBase[root2] < disjointSetBase[root1]) {
				//1����2
				disjointSetBase[root2] += disjointSetBase[root1];
				disjointSetBase[root1] = root2;
			}
			else {
				disjointSetBase[root1] += disjointSetBase[root2];
				disjointSetBase[root2] = root1;
			}
			return true;
		}
	}

	// ����Ԫ���������кϲ� O(lgN) <==> unionSet(findRoot(lhsIndex), findRoot(rhsIndex));
	// �������ϲ�����true ���򷵻�false
	bool unionElement(ElementIndexType lhsIndex, ElementIndexType rhsIndex) {
		return unionSet(findRoot(lhsIndex), findRoot(rhsIndex));
	}

	// ��ѯָ��Ԫ�صĸ� O(lgN) (Ԫ�ص�������)
	SetRoot findRoot(ElementIndexType eleIndex) {
		// ����Ĭ�ϳ�ʼֵΪ-1 (������ʾ����һ�����ϵĸ�, ����ֵ��ʾ���ϴ�С)
		return disjointSetBase[eleIndex] < 0 ? eleIndex
			// ·��ѹ��: ���ݹ��ҵ��ļ��ϵĸ���ֵ�������
			: disjointSetBase[eleIndex] = findRoot(disjointSetBase[eleIndex]);
	}

	// ����ָ�����ϵ�Ԫ�ظ���(��С) O(1)
	size_t size(SetRoot root) {
		return -disjointSetBase[root];
	}

	// ��ʼ�� O(N)
	void init() {
		disjointSetBase.assign(disjointSetBase.size(), -1);
		//memset(disjointSetBase, -1, sizeof(SetBaseType));
	}

	// ��ԭ���Ļ����������С O(N)
	void reSize(size_t size) {
		disjointSetBase.resize(size, -1);
		//memset(disjointSetBase, -1, sizeof(SetBaseType));
	}

	// ������geter seter�������
	// ���ؼ����� O(N)
	size_t calcSetCount() {
		size_t cnt = 0;
		for (size_t i = 0; i < disjointSetBase.size(); ++i) {
			cnt += disjointSetBase[i] < 0 ? 1 : 0;
		}
		return cnt;
	}

private:
	SetBaseType disjointSetBase;
};
