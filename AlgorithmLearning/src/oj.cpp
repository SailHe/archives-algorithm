/*Online Judge��������ϵͳ(����pta, hdu�ȵ�) �ύ���ļ�(Practice�ľ��򸱱�)*/

#include <stdio.h>  
#include <string.h>  
#include <queue>  
#include <set>  
#include <iostream>  
#define N 105
using namespace std;
typedef pair<int, int> DirectionVector;//�����ӷ�����ʱ ����first �� second��ϰ�ߵı�ʾxy rc��������ͬ
const DirectionVector DIR84[8] = {//����extern ������ض�������
	/*r��������->��4123���� ���ڵ��� ˳�����������*/
	{ 1, 0 }/*��*/,
	{ 0, 1 }/*��*/,
	{ -1, 0 }/*��*/,
	{ 0, -1 }/*��*/,

	{ 1, 1 }/*����*/,
	{ -1, 1 }/*����*/,
	{ -1, -1 }/*����*/,
	{ 1, -1 }/*����*/
};

int ans;
int n;
char cityMap[N][N];
//���õ���Է�����̨ ����true
bool cheak(int rP, int cP){
	//�����
	for (int r = 0; r < n; ++r){
		if (cityMap[r][cP] == '*'){
			//������̨
			return false;
		}
		//����wallֱ��break
		else if (cityMap[r][cP] == 'X'){
			break;
		}
	}
	//�����
	for (int c = 0; c < n; ++c){

	}
}
//r == n - 1 && c == n - 1
void dfs(int r = 0, int c = 0){
	if (cityMap[r][c] == 'X'){
		//����wall�˴��������
	}
	else{
		for (int i = 0; i < 4; ++i){
			int nr = r + DIR84[i].first, nc = c + DIR84[i].second;
			if (nr >= 0 && nr < n &&  nc >= 0 && nc < n){
				cityMap[r][c] = '*';//�ڴ˴������̨  '*' ��ʾ�Ǳ�(����̨)
				dfs(nr, nc);
				cityMap[r][c] = '.';//����
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
