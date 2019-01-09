#pragma once
#include<vector>

// 只储存集合关系 没有储存集合的元素(下标 <==> 元素)
//并查集模板类 unit join set
class DisjointSet {
public:
	// 用集合的root下标表示集合
	using SetRoot = int;
	// 用集合数组的下标表示元素
	using ElementIndexType = int;
	// 数组的value表示其集合的'父结点'(任意元素都可以作为父结点 但同一个集合中所有元素的root结点有且只有1个)
	using SetBaseType = std::vector<ElementIndexType>;
	//using SetBaseType = ElementIndexType[MAX_N];
	//在class出现vector在http://139.196.145.92上会Runtime Error
	//vector<ElementIndexType> debugTemp1;

	DisjointSet() {}
	// 初始化一个索引号:[0, elementNum)的含有elementNum个集合的并查集 O(N)
	DisjointSet(size_t elementNum) {
		//新的元素初始值也必须是-1 每个元素都代表一个单独的集合
		disjointSetBase.assign(elementNum, -1);
		//memset(disjointSetBase, -1, elementNum*sizeof(ElementIndexType));
	}

	// 合并两个集合 O(lgN)
	// 若发生合并返回true 否则返回false
	bool unionSet(SetRoot root1, SetRoot root2) {
		if (root1 == root2) {
			return false;
		}
		else {
			//保证小集合并入大集合
			if (disjointSetBase[root2] < disjointSetBase[root1]) {
				//1并入2
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

	// 根据元素索引进行合并 O(lgN) <==> unionSet(findRoot(lhsIndex), findRoot(rhsIndex));
	// 若发生合并返回true 否则返回false
	bool unionElement(ElementIndexType lhsIndex, ElementIndexType rhsIndex) {
		return unionSet(findRoot(lhsIndex), findRoot(rhsIndex));
	}

	// 查询指定元素的根 O(lgN) (元素的索引号)
	SetRoot findRoot(ElementIndexType eleIndex) {
		// 集合默认初始值为-1 (负数表示这是一个集合的根, 绝对值表示集合大小)
		return disjointSetBase[eleIndex] < 0 ? eleIndex
			// 路径压缩: 将递归找到的集合的根赋值给父结点
			: disjointSetBase[eleIndex] = findRoot(disjointSetBase[eleIndex]);
	}

	// 返回指定集合的元素个数(大小) O(1)
	size_t size(SetRoot root) {
		return -disjointSetBase[root];
	}

	// 初始化 O(N)
	void init() {
		disjointSetBase.assign(disjointSetBase.size(), -1);
		//memset(disjointSetBase, -1, sizeof(SetBaseType));
	}

	// 在原来的基础上重设大小 O(N)
	void reSize(size_t size) {
		disjointSetBase.resize(size, -1);
		//memset(disjointSetBase, -1, sizeof(SetBaseType));
	}

	// 避免与geter seter产生误会
	// 返回集合数 O(N)
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
