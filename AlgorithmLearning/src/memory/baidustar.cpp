#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
#include<math.h>
#include<iostream>
using namespace std;
#define INIF_INT32 2147483647 // 0x7FFFFFFF
#define MIN_INT32 -INIF_INT32 - 1 // 0x80000000 但是不能这么初始化

// memeset int最大初始化值(写出1位7f或80就行 此处是为了便于比较)
#define MAX_INITER_INT32 0x7f7f7f7f
#define MIN_INITER_INT32 0x80808080
// #include"ExtendSpace.h"


#define MONSTER_NUM 100000+5
#define ATTACKER__NUM 1000+5
// @see http://acm.hdu.edu.cn/search.php?action=listproblem
// @see http://acm.hdu.edu.cn/showproblem.php?pid=6082
// 2017"百度之星"程序设计大赛 - 资格赛 度度熊与邪恶大魔王
struct Monster {
	int hp;
	int def;
};
Monster monster[MONSTER_NUM];

// 技能
struct Attacker {
	//消耗晶石数
	int cost;
	//伤害值
	int value;
};
Attacker attacker[ATTACKER__NUM];
void getMosters(int n) {
	for (int i = 0; i < n; i++) {
		scanf("%d%d", &monster[i].hp, &monster[i].def);
		// cin >> monster[i].hp >> monster[i].def;
	}
}
void getAttackers(int m) {
	for (int i = 0; i < m; i++) {
		scanf("%d%d", &attacker[i].cost, &attacker[i].value);
		// cin >> attacker[i].cost >> attacker[i].value;
	}
}

//costDp[hp][def] 表示击杀血量为hp防御值为def的Monster时的最小消费
int costDp[1001][11];
// 枚举 + 背包
void solution(int n, int m, long long &sumCost) {
	Monster mster;
	// 999*11 和 1000*10 tm的不一样好吧!
	// 何况应该是1000*11 和 1001*10 tm的不一样好吧!
	// PS 数组是没有界限检查的, 但oj会报内存越界! vector是有编译报错这就避免了这一错误, 但不是很便于调试
	// 但遇到这种初始化错误的sb情况只有测试边界这一条路
	memset(costDp + 1, 0x3f, 1000 * 11 * sizeof(int));
	// 初始化hp=0的所有def枚举的费用为0
	memset(costDp, 0, 11 * sizeof(int));
	// 技能数枚举 (1<=m<=1000)
	// 由于会出现大量的同类型(hp或def)一致的Monster 导致重复计算 此处直接少了一个内层n循环(1<=n<=100000)
	for (int i = 0; i < m; ++i) {
		// 防御值枚举
		for (mster.def = 0; mster.def < 11; ++mster.def) {
			// 伤害
			int damage = attacker[i].value - mster.def;
			// || damage > 1000 伤害小于防御 攻击不会奏效
			if (damage <= 0) {
				break;
			}
			// 可击杀的Monster的血量枚举
			for (mster.hp = 1; mster.hp < damage; ++mster.hp) {
				costDp[mster.hp][mster.def] = min(costDp[mster.hp][mster.def], attacker[i].cost);
			}
			// 动态规划推出后续的花费
			for (; mster.hp <= 1000; ++mster.hp) {
				costDp[mster.hp][mster.def] = min(
					costDp[mster.hp][mster.def]
					, costDp[mster.hp - damage][mster.def] + attacker[i].cost
				);
			}
		}
	}
	for (int i = 0; i < n; ++i) {
		int cost = costDp[monster[i].hp][monster[i].def];
		if (cost == 0x3f3f3f3f) {
			sumCost = -1;
			break;
		}
		else {
			sumCost += cost;
		}
	}
}


int costDpOne[1001];
// 处理一件[完全背包]中的物品过程 ==> 有N种物品和一个容量为capacity的背包，每种物品都有无限件可用。
template<class Number, class Function>
void CompletePackProcess(Number backPack[], Number capacity, Number currentCost, Number currentValue, Function maxFun) {
	for (Number v = currentCost; v <= capacity; ++v) {
		backPack[v] = maxFun(backPack[v], backPack[v - currentCost] + currentValue);
	}
}
// 这个考虑得太复杂了, 怒弃!
void solution1(int n, int m, long long &sumCost) {
	// 传统背包算法求的是各种{容量, 费用, 件数}限制条件下的最大价值, 费用(重量)<=容量(限重) 恒成立
	// 容量是已确认的值, 而不由费用决定, 需要计算的是件数, 从而算出限定容量下的最大价值
	// 此题是求在击杀Monster前提下的最少晶石数量(费用). 存在并允许选择 费用更少且伤害>hp+def的技能(物品)
	// 逆运算思路: 负数(负极值必须保证不溢出); 类型运算符重载; lambda表达式
	// costDp[hp] 表示杀死第这只怪在其血量为hp时的最小消费
	// 综上, 用Monster的hp+def作为背包容量是不可行的(实际上伤害=攻击-def不能hp+def作为容量)
	// 因此只能是容量范围内求最大伤害
	// 或是容量范围内求最小晶石数(最大晶石数的负值) 容量是hp 花费是damage costDpOne[h]表示hp=h的Monster受到damage=hp的伤害需要的最少晶石数
	bool processed = false;
	for (int iMon = 0; iMon < n; ++iMon) {
		int sumDamage = 0;
		// int cap = monster[iMon].hp + monster[iMon].def;
		int cap = 1001;
		for (int iAttacker = 0; iAttacker < m; ++iAttacker) {
			// 伤害低于def不会造成伤害
			if (attacker[iAttacker].value > monster[iMon].def) {
				int damage = attacker[iAttacker].value - monster[iMon].def;
				CompletePackProcess(
					costDpOne
					, cap
					, damage
					, -attacker[iAttacker].cost
					, [](int lhs, int rhs) {return min(lhs, rhs); }
				);
			}
			int index = monster[iMon].hp;// +monster[iMon].def;
			int minCostOnePro = 0;
			// 寻找一个能击杀Monster的最小花费
			while (index < cap) {
				int costBuf = costDpOne[index];
				++index;
				if (costBuf != 0) {
					processed = true;
					minCostOnePro = -costBuf;
					sumDamage += index - 1;
					break;
				}
			}
			memset(costDpOne, 0, cap*sizeof(int));
			if (sumDamage >= monster[iMon].hp) {
				sumCost += minCostOnePro;
			}
		}
	}
	if (!processed) {
		sumCost = -1;
	}
}


int main() {
	// n个怪兽，m种技能
	int n, m;
	while (2 == scanf("%d%d", &n, &m)) {
		getMosters(n);
		getAttackers(m);
		// (0<=cost[i]<=100000)
		long long sumCost = 0;
		solution(n, m, sumCost);
		// solution1(n, m, sumCost);
		printf("%I64d\n", sumCost);
	}
	return 0;
}

/*
原题输入
1 2
3 5
7 10
6 8

1 2
3 5
10 7
8 6

ouput
6
18
*/

/*
测试输入
1 1
999 1
1 2

1 1
1000 10
100000 1

2 1
3 5
4 5
10 500

2 2
3 5
4 5
1 4
1 5

1 2
3 5
7 10
6 8

1 2
3 5
10 7
8 6

2 1
3 5
4 5
10 5

4 4
3 3
4 5
5 5
6 6
2 2
1 1
4 7
1 2

4 4
3 3
4 5
5 5
6 6
2 2
2 5
4 7
1 5

4 4
3 3
4 5
5 5
6 6
2 2
1 5
4 7
2 5

output
-1
999
20
-1
6
18
-1
48
46
46

*/
