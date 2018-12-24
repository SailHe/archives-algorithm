#pragma once
// 其实这个集合并非真正意义上的集合 它只储存了关系 没有储存集合的元素
template<class ElementType>
//并查集模板类 unit join set
class DisjointSet {
public:
	//using ElementType = int;
	// 用集合的root下标表示集合
	using SetRoot = int;
	// 元素都表示下标
	using SetBaseType = vector<ElementType>;
	//using SetBaseType = ElementType[MAX_N];
	//在class出现vector在http://139.196.145.92上会Runtime Error
	//vector<ElementType> debugTemp1;

	DisjointSet() {}
	// O(N)
	DisjointSet(size_t n) {
		//新的元素初始值也必须是-1 每个元素都代表一个单独的集合
		disjointSetBase.assign(n, -1);
		//memset(disjointSetBase, -1, n*sizeof(ElementType));
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

	// 根据元素进行合并 O(lgN)
	// 若发生合并返回true 否则返回false
	bool unionElement(ElementType x1, ElementType x2) {
		return unionSet(findRoot(x1), findRoot(x2));
	}

	// 查询指定元素的根 O(lgN)
	SetRoot findRoot(ElementType x) {
		// 集合默认初始值为-1 (负数表示这是一个集合的根, 绝对值表示集合大小)
		return disjointSetBase[x] < 0 ? x
			// 路径压缩: 将递归找到的集合的根赋值给父结点
			: disjointSetBase[x] = findRoot(disjointSetBase[x]);
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
