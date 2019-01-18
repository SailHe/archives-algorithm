#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
#include<math.h>
#include<iostream>
using namespace std;
#define INIF_INT32 2147483647 // 0x7FFFFFFF
#define MIN_INT32 -INIF_INT32 - 1 // 0x80000000 ���ǲ�����ô��ʼ��

// memeset int����ʼ��ֵ(д��1λ7f��80���� �˴���Ϊ�˱��ڱȽ�)
#define MAX_INITER_INT32 0x7f7f7f7f
#define MIN_INITER_INT32 0x80808080
// #include"ExtendSpace.h"


#define MONSTER_NUM 100000+5
#define ATTACKER__NUM 1000+5
// @see http://acm.hdu.edu.cn/search.php?action=listproblem
// @see http://acm.hdu.edu.cn/showproblem.php?pid=6082
// 2017"�ٶ�֮��"������ƴ��� - �ʸ��� �ȶ�����а���ħ��
struct Monster {
	int hp;
	int def;
};
Monster monster[MONSTER_NUM];

// ����
struct Attacker {
	//���ľ�ʯ��
	int cost;
	//�˺�ֵ
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

//costDp[hp][def] ��ʾ��ɱѪ��Ϊhp����ֵΪdef��Monsterʱ����С����
int costDp[1001][11];
// ö�� + ����
void solution(int n, int m, long long &sumCost) {
	Monster mster;
	// 999*11 �� 1000*10 tm�Ĳ�һ���ð�!
	// �ο�Ӧ����1000*11 �� 1001*10 tm�Ĳ�һ���ð�!
	// PS ������û�н��޼���, ��oj�ᱨ�ڴ�Խ��! vector���б��뱨����ͱ�������һ����, �����Ǻܱ��ڵ���
	// ���������ֳ�ʼ�������sb���ֻ�в��Ա߽���һ��·
	memset(costDp + 1, 0x3f, 1000 * 11 * sizeof(int));
	// ��ʼ��hp=0������defö�ٵķ���Ϊ0
	memset(costDp, 0, 11 * sizeof(int));
	// ������ö�� (1<=m<=1000)
	// ���ڻ���ִ�����ͬ����(hp��def)һ�µ�Monster �����ظ����� �˴�ֱ������һ���ڲ�nѭ��(1<=n<=100000)
	for (int i = 0; i < m; ++i) {
		// ����ֵö��
		for (mster.def = 0; mster.def < 11; ++mster.def) {
			// �˺�
			int damage = attacker[i].value - mster.def;
			// || damage > 1000 �˺�С�ڷ��� ����������Ч
			if (damage <= 0) {
				break;
			}
			// �ɻ�ɱ��Monster��Ѫ��ö��
			for (mster.hp = 1; mster.hp < damage; ++mster.hp) {
				costDp[mster.hp][mster.def] = min(costDp[mster.hp][mster.def], attacker[i].cost);
			}
			// ��̬�滮�Ƴ������Ļ���
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
// ����һ��[��ȫ����]�е���Ʒ���� ==> ��N����Ʒ��һ������Ϊcapacity�ı�����ÿ����Ʒ�������޼����á�
template<class Number, class Function>
void CompletePackProcess(Number backPack[], Number capacity, Number currentCost, Number currentValue, Function maxFun) {
	for (Number v = currentCost; v <= capacity; ++v) {
		backPack[v] = maxFun(backPack[v], backPack[v - currentCost] + currentValue);
	}
}
// ������ǵ�̫������, ŭ��!
void solution1(int n, int m, long long &sumCost) {
	// ��ͳ�����㷨����Ǹ���{����, ����, ����}���������µ�����ֵ, ����(����)<=����(����) �����
	// ��������ȷ�ϵ�ֵ, �����ɷ��þ���, ��Ҫ������Ǽ���, �Ӷ�����޶������µ�����ֵ
	// ���������ڻ�ɱMonsterǰ���µ����پ�ʯ����(����). ���ڲ�����ѡ�� ���ø������˺�>hp+def�ļ���(��Ʒ)
	// ������˼·: ����(����ֵ���뱣֤�����); �������������; lambda���ʽ
	// costDp[hp] ��ʾɱ������ֻ������Ѫ��Ϊhpʱ����С����
	// ����, ��Monster��hp+def��Ϊ���������ǲ����е�(ʵ�����˺�=����-def����hp+def��Ϊ����)
	// ���ֻ����������Χ��������˺�
	// ����������Χ������С��ʯ��(���ʯ���ĸ�ֵ) ������hp ������damage costDpOne[h]��ʾhp=h��Monster�ܵ�damage=hp���˺���Ҫ�����پ�ʯ��
	bool processed = false;
	for (int iMon = 0; iMon < n; ++iMon) {
		int sumDamage = 0;
		// int cap = monster[iMon].hp + monster[iMon].def;
		int cap = 1001;
		for (int iAttacker = 0; iAttacker < m; ++iAttacker) {
			// �˺�����def��������˺�
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
			// Ѱ��һ���ܻ�ɱMonster����С����
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
	// n�����ޣ�m�ּ���
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
ԭ������
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
��������
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
