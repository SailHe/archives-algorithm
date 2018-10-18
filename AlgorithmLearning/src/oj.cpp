/*Online Judge在线评测系统(例如pta, hdu等等) 提交用文件(Practice的精简副本)*/

#include <stdio.h>  
#include <string.h>  
#include <queue>  
#include <set>  
#include <iostream>  
#define N 105
using namespace std;
typedef pair<int, int> DirectionVector;//在做加法运算时 它的first 与 second与习惯的表示xy rc的意义相同
const DirectionVector DIR84[8] = {//加了extern 会出现重定义问题
	/*r轴正方向->第4123象限 便于调试 顺序不能随意更改*/
	{ 1, 0 }/*下*/,
	{ 0, 1 }/*右*/,
	{ -1, 0 }/*上*/,
	{ 0, -1 }/*左*/,

	{ 1, 1 }/*右下*/,
	{ -1, 1 }/*右上*/,
	{ -1, -1 }/*左上*/,
	{ 1, -1 }/*左下*/
};

int ans;
int n;
char cityMap[N][N];
//若该点可以放置炮台 返回true
bool cheak(int rP, int cP){
	//检测行
	for (int r = 0; r < n; ++r){
		if (cityMap[r][cP] == '*'){
			//遇到炮台
			return false;
		}
		//遇到wall直接break
		else if (cityMap[r][cP] == 'X'){
			break;
		}
	}
	//检测列
	for (int c = 0; c < n; ++c){

	}
}
//r == n - 1 && c == n - 1
void dfs(int r = 0, int c = 0){
	if (cityMap[r][c] == 'X'){
		//遇到wall此次搜索完毕
	}
	else{
		for (int i = 0; i < 4; ++i){
			int nr = r + DIR84[i].first, nc = c + DIR84[i].second;
			if (nr >= 0 && nr < n &&  nc >= 0 && nc < n){
				cityMap[r][c] = '*';//在此处添加炮台  '*' 表示城堡(即炮台)
				dfs(nr, nc);
				cityMap[r][c] = '.';//回溯
			}
		}
	}
}

int main_(){
	freopen("input", "r", stdin);
	vector<short> temp(10, 1);
	while (~scanf("%d", &n) && n != 0){
		getchar();
		for (int r = 0; r < n; ++r){
			gets(cityMap[r]);
		}
		ans = 0;
		dfs();
		printf("%d\n", ans);
	}
	return 0;
}
