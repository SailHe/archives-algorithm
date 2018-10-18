#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
#include<math.h>
#include<iostream>
using namespace std;
#define N 100000+5
#define M 1000+5
#define infinity 2147483647
struct Monster{
	int hp;
	int def;
};
Monster monster[N];
struct Attacker{
	int cost;//消耗晶石数
	int value;//伤害值
};
Attacker attacker[M];
void getMosters(int n){
	for (int i = 0; i < n; i++){
		cin >> monster[i].hp >> monster[i].def;
	}
}
void getAttackers(int m){
	for (int i = 0; i < m; i++){
		cin >> attacker[i].cost >> attacker[i].value;
	}
}
//01背包 V是背包总容量
void zoBack(int V, int back[], int cost, int value)
{
	for (int v = V; v >= cost; v--)
	{
		back[v] = min(back[v], back[v - cost] + value);
	}
}
int dp[1000];//dp[hp] 表示杀死第这只怪在其血量为hp时的最小消费
int main()
{
	const int n = 5;
	const int V = 10;
	int value[n] = { 6, 3, 5, 4, 6 };
	int cost[n] = { 2, 2, 6, 5, 4 };
	int dp[n+1][V+1] = { 0 };
	int Cost = cost[n-1];
	/*基本版*/
	for (int i = n-1; i >= 0; i--){
		Cost = min(Cost, cost[i]);
		for (int v = Cost; v < V+1; v++){
			dp[i][v] = max(dp[i + 1][v - cost[i]] + value[i], dp[i + 1][v]);
		}
	}cout << dp[0][10]<<endl;
	/*空间优化版*/
	int dp2[V+1] = { 0 };
	//for (int i = 1; i < V + 1; dp2[i++] = -infinity);//恰好装满
	for (int i = 0; i < n; i++){
		for (int v = V; v >= cost[i]; v--){
			dp2[v] = max(dp2[v], dp2[v - cost[i]]+value[i]);
		}
	}
	/*以上两种时间复杂度O(n*(V-min(cost)))*/
	/*完全背包*/
	for (int i = 0; i < n; i++){
		for (int cnt = 1; cnt*cost[i] <= V; cnt++){
			for (int v = V; v >= cost[i] * cnt; v--){
				dp[v] = max(dp[v], dp[v - cost[i] * cnt] + cnt);
			}
		}
	}
	return 0;
}