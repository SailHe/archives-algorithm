#pragma once
// ��ʵ������ϲ������������ϵļ��� ��ֻ�����˹�ϵ û�д��漯�ϵ�Ԫ��
template<class ElementType>
//���鼯ģ���� unit join set
class DisjointSet {
public:
	//using ElementType = int;
	// �ü��ϵ�root�±��ʾ����
	using SetRoot = int;
	// Ԫ�ض���ʾ�±�
	using SetBaseType = vector<ElementType>;
	//using SetBaseType = ElementType[MAX_N];
	//��class����vector��http://139.196.145.92�ϻ�Runtime Error
	//vector<ElementType> debugTemp1;

	DisjointSet() {}
	// O(N)
	DisjointSet(size_t n) {
		//�µ�Ԫ�س�ʼֵҲ������-1 ÿ��Ԫ�ض�����һ�������ļ���
		disjointSetBase.assign(n, -1);
		//memset(disjointSetBase, -1, n*sizeof(ElementType));
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

	// ����Ԫ�ؽ��кϲ� O(lgN)
	// �������ϲ�����true ���򷵻�false
	bool unionElement(ElementType x1, ElementType x2) {
		return unionSet(findRoot(x1), findRoot(x2));
	}

	// ��ѯָ��Ԫ�صĸ� O(lgN)
	SetRoot findRoot(ElementType x) {
		// ����Ĭ�ϳ�ʼֵΪ-1 (������ʾ����һ�����ϵĸ�, ����ֵ��ʾ���ϴ�С)
		return disjointSetBase[x] < 0 ? x
			// ·��ѹ��: ���ݹ��ҵ��ļ��ϵĸ���ֵ�������
			: disjointSetBase[x] = findRoot(disjointSetBase[x]);
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
