#include"stdafx.h"
/*
(����ʽͼ��������������㷨)
�����㷨ʵ����һ������ö�ٵ��������Թ��̣�
��Ҫ�����������Թ�����Ѱ������Ľ⣬�������Ѳ������������ʱ���͡����ݡ����أ����Ա��·����
���ݷ���һ��ѡ������������ѡ��������ǰ�������ԴﵽĿ�ꡣ����̽����ĳһ��ʱ������ԭ��ѡ�񲢲��Ż�ﲻ��Ŀ�꣬
���˻�һ������ѡ�������߲�ͨ���˻����ߵļ���Ϊ���ݷ������������������ĳ��״̬�ĵ��Ϊ�����ݵ㡱��
��ิ�ӵģ���ģ�ϴ�����ⶼ����ʹ�û��ݷ����С�ͨ�ý��ⷽ���������ơ�
*/
//�������������ѭ������������: ǰ��ѭ�������Ĳ�ȷ���Ը���(һ�����ں���), 
//����Ҫ����ǰ�߿���ʵ�ֲ�ȷ����ѭ������(��֦: ��ȥ�������ܲ������Ž�Ľ��)
//��һ�����ڼ������ȫ���ݵ�dfsд��(��ȫ����ָ���˴��� �������б�������ǰ�����к���ȫһ��, �ⲿ��������ʼ��)
//ʵ�ֵݹ�ؼ���: ֻ������һ��, ������һ�������һ��, ����Ҫ�ݹ�ĵط�ֱ�ӵ���, ����Ҫ����, �ڵ��ú������Ӧ�ı���������

//01�� �򵥵Ķ�������
//���ڳ���Ϊ5λ��һ��01����ÿһλ��������0��1��һ����32�ֿ��ܡ� ÿ������һ�ֿ��ܾ����
void dfs(int depth = 0) {
	static vector<int> a(5, -1);
	if (depth == 5) {
		for (int i = 0; i < 5; ++i) {
			printf(i == 0 ? "%d" : "%d", a[i]);
		}
		puts("");
	}
	else {
		//��ǰλΪ0
		a[depth] = 0;
		dfs(depth + 1);
		//��ǰλΪ1
		a[depth] = 1;
		dfs(depth + 1);
		a[depth] = -1;
	}
}


//�η� 1494: 7.װ������
/* ��� ������:http://blog.csdn.net/helihui123/article/details/4476624 */
/*
5(n������) 10(c��������)
2 2 3 6 9
��wl�������9������10: http://blog.csdn.net/gakki_529/article/details/78699622

output: 6 7 8 9 10 10 10 10 11 12 13

input:

5 10
11 6 16 5 13

5 10
11 2 16 5 13

5 10
7 11 6 5 1

4 10
11 6 16 9

5 10
2 2 3 6 9

5 10
7 2 6 5 2

5 10
7 2 6 5 4

5 10
11 2 6 5 3

6 11
11 6 16 8 1 1

7 12
11 6 16 8 1 1 2

7 13
11 6 16 8 1 1 2

*/
int n, c;
int a[1000];
int ans;
void dfs(int depth){
	static int sum = 0;
	if (ans == c){//ֻ��Ҫ���ֵ�ܴﵽ����û��Ҫ�ڴﵽ���޺��������
		return;
	}
	else{
		for (int i = depth; i <= n; ++i){
			if (sum + a[i] > c){
				//a[i+1] >= a[i] ��㼰֮��Ķ���û��Ҫ����(���򲻽���������һ��Ķ������� ����������һ�������)
				return;
			}
			else{
				//sum��״̬��Ҫ��visit��ʱ����, 
				//���������visit���õ���sum��״̬, ������ǰ���ֻ��ݷ�ֻ�ܴ�����һ���sum״̬, ����һ��������������һ�㲻��������ʱsum��״̬�޷�������һ��
				sum += a[i];

				//visit(��ǰ��)
				if (sum > ans){
					ans = sum;
				}
				else{
					//do nothing
				}

				dfs(i + 1);//������һ��

				sum -= a[i];//������һ��
			}
		}
	}
}
int solution(){
	for (int i = 1; i <= n; i++){
		scanf("%d", &a[i]);
		/// fprintf(fp, (i == 1 ? "%d" : " %d"), a[i]); 
	}
	///fprintf(fp, "\n");

	sort(a + 1, a + n + 1);//������������Ҫ
	FOR(i, 0, 100000){
		ans = 0;
		dfs(1);
	}
	printf("%d\n", ans);
	return 0;
}



//1483 �ַ���ȫ����
/*
output(ֻд������): 6 120
input:
4
aacc

5
dabce

*/
int n,
letterCount[100], //��¼ÿһ����ĸ���ַ����г��ֵĴ���
schemeCount;
void dfs(int depth = 0){
	static char schemeBuffer[1000]; //��¼һ�����з���
	if (depth == n){
		++schemeCount;//��¼��������
		puts(schemeBuffer);
		return;
	}
	else{
		for (int letterSub = 0; letterSub < 26; ++letterSub){
			//ÿһ�������ǰ���26��Сд��ĸѭ������������������0����ȡ�ø���ĸ
			if (letterCount[letterSub] > 0){
				--letterCount[letterSub];//ÿ��ȡ��һ����ĸ���ļ�������-1
				schemeBuffer[depth] = letterSub + 'a';//visit: depth��ʾ����length
				dfs(depth + 1);
				++letterCount[letterSub];//����һ��  
			}
		}
	}
}
int solution(){
	char str[1000];
	cin >> str;
	memset(letterCount, 0, sizeof(letterCount));
	schemeCount = 0;
	for (int i = 0; i < n; ++i){
		++letterCount[str[i] - 'a'];
	}
	dfs();
	printf("%d\n", schemeCount);
	return 0;
}
//�⺯����
int solution1(){
	int schemeCount = 0;
	char str[1000];
	cin >> str;
	sort(str, str + n);
	do{
		++schemeCount;
		//cout << str << endl;//50ms
		puts(str);//32ms
	} while (next_permutation(str, str + n));
	printf("%d\n", schemeCount);
	return 0;
}


//1481 ������� �����ṩ������ʵ��+һ�����Ƶ�wlʵ��(δ�ṩ����)
/*
output������±���9*2ҳ
input:
9 8
1 3
1 4
2 3
2 5
5 7
5 8
5 9
1 2

9 8
1 3
1 4
2 3
2 5
5 7
5 8
5 9
1 6

9 9
1 3
1 4
2 3
2 5
5 7
5 8
5 9
4 6
3 6

6 6
1 3
1 4
2 4
2 5
2 6
3 5

8 8
1 5
1 8
2 3
2 6
3 7
4 5
4 8
6 7

6 6
1 2
2 3
3 6
6 5
5 4
4 1

9 7
1 3
1 4
2 3
2 5
5 7
5 8
5 9

6 7
1 2
1 3
2 4
2 5
2 6
3 4
3 5

7 10
1 2
1 4
2 4
2 3
2 5
2 6
3 5
3 6
4 5
5 6

3 2
1 2
1 3

*/
int n, m;//���ñ���


struct TribeResident{
	static int ID;
public:
	TribeResident(){
		id = ++ID;
	}
	int id;
	list<int> enemy;//��жԵľ���id
	bool operator<(TribeResident const &rhs)const{
		int ls = enemy.size(), rs = rhs.enemy.size();
		return ls == rs ? id < rhs.id :
			ls < rs;
	}
};
int TribeResident::ID = 0;
bool ptrLess(TribeResident const *lhs, TribeResident const *rhs){
	return (*lhs) < (*rhs);
}
//����ʵ��ʱ�临�Ӷ�ΪO(N^2) (�������˻���ж�ʱΪ����)
//������: ���Ѿ���(��)һ�����(���ݹ�һ��)�Ŀ�����Ϊ1�Ļ� ��ʵ�ֱȽ����������ҪСһЩ
int solution1(){

	TribeResident allPeople[100];
	TribeResident *allPeoplePtr[100];
	int group[100];
	memset(group, 0, sizeof(int)*n);

	FOR(i, 0, n){
		allPeople[i].enemy.clear();
	}
	FOR(c, 0, n){
		allPeoplePtr[c] = allPeople + c;
	}
	FOR(c, 0, m){
		int le, ri;
		scanf("%d%d", &le, &ri);
		///fprintf(fp, (c == 0 ? "%d %d\n" : "%d %d\n"), le, ri); fprintf(fp, "\n"); ///
		allPeople[le - 1].enemy.push_back(ri);
		allPeople[ri - 1].enemy.push_back(le);
	}

	sort(allPeoplePtr, allPeoplePtr + n, ptrLess);

	FOR(i, 0, n){
		if (group[allPeoplePtr[i]->id - 1] == 1){
			//��֪���ڶ���ʱ���� 1��ʾ���ڶ��� Ĭ����Ҫ�����˶��ڶ���
		}
		else{
			//�ж�����������ԭ�� ���ж�������ٵĵж����ų�
			for (auto it = allPeoplePtr[i]->enemy.begin(); it != allPeoplePtr[i]->enemy.end(); ++it){
				group[*it - 1] = 1;
			}
		}
	}
	FOR(i, 0, n){
		if (group[i] == 1){//������ڶ���
			int count = 0, sub = -1;
			//id����ԭ��: ���Լ��ĵж�����ֻ��һ���ڶ���(�滻��û�г�ͻ) ����id�����Լ�ʱ
			for (auto it = allPeople[i].enemy.begin(); it != allPeople[i].enemy.end(); ++it){
				if (group[*it - 1] == 0){
					++count;
					sub = *it - 1;
				}
			}
			if (count == 1 && i < sub){
				swap(group[i], group[sub]);
			}
		}
	}
	FOR(i, 0, n){
		printf(i == 0 ? "%d" : " %d", !group[i]);//Ҫ��1��ʾ�ڶ���
	}

	puts("");
	return 0;
}

int enemy[100][100];
int gr[100];//group
int maxNum;
//������ǿŷ���ȫN���� h: O(N)(û�ежԹ�ϵ�ǿ���ȫN���� ��ʱh=N) �ڲ���һ��O(N^2)�ĵ���(�����˻���ж�ʱ��N^2)
//����ʵ�������Ϻ�ƽӹ ����������һЩ �������Ҫ������˳��Ļ� ����������ı�
void dfs(int depth = 0){
	//��Ϊ�����ڵĵж���vector�ھ����Լ�visitʱ������ ���ڻ���ǰ����Ҫ������ж���
	//һ�����Եİ취��ʹ�������list �������˾���Ч��
	static list<int> groupListBuffer;//��ʾsub(ģ��Ķ�ջ)

	//��֦: ��ǰѡ�õ� + ��û���������� < ��ǰ��֪�����ֵ(���ڵ�ʱ��Ҫ��0�����ж���)
	if (groupListBuffer.size() + n - depth < maxNum)
		return;

	if (depth == n){
		return;
	}
	else{
		//����һ��: sub < depth��Ԫ����ǰ������߹���(ע��, һ����ܾ��߶��Ԫ��)
		//ÿ����߳���Ԫ�ؼ��Ϲ�����һ���Ψһ���Ž��(Ψһ���������Դ��Գ�����)
		FOR(i, depth, n){
			bool hasEnemy = false;
			//����������Ƿ����Լ��ĳ�� (��������Ǳ����, ������O(N^2))
			for (auto it = groupListBuffer.begin(); it != groupListBuffer.end(); ++it){
				if (enemy[*it][i]){
					hasEnemy = true;
					break;
				}
			}

			if (hasEnemy){
				//do nothing
			}
			else{
				//���Լ��ӽ�����
				groupListBuffer.push_back(i);
				//visit
				if (groupListBuffer.size() > maxNum){
					maxNum = groupListBuffer.size();
					memset(gr, 0, sizeof(gr));
					for (auto it = groupListBuffer.begin(); it != groupListBuffer.end(); ++it){
						gr[*it] = 1;
					}
				}

				dfs(i + 1);
				groupListBuffer.pop_back();
			}
		}
	}
}
//������ǿ���ȫ������ h=N (��wl����һ���� ��Ч����� �жԹ�ϵ�϶�ʱʹ������汾�Ŀ�����С)
void dfsBin(int depth = 0){
	static list<int> groupListBuffer;//��ʾsub

	//��֦: ��ǰѡ�õ� + ��û���������� < ��ǰ��֪�����ֵ(���ڵ�ʱ��Ҫ��0�����ж���) ��������� ��ô���ıض��ǿ�CBT
	if (groupListBuffer.size() + n - depth < maxNum)
		return;

	if (depth == n){
		//visit
		if (groupListBuffer.size() > maxNum){
			maxNum = groupListBuffer.size();
			memset(gr, 0, sizeof(gr));
			for (auto it = groupListBuffer.begin(); it != groupListBuffer.end(); ++it){
				gr[*it] = 1;
			}
		}
		return;
	}
	else{
		//ÿ��Ԫ��ֻ�н��� �벻��������״̬ �������˳���޹� ���Խ�һ�Ŷ�����(ÿ�����һ��Ԫ��)�Ϳ�����û��Ҫn��

		//���ߵ�ǰԪ�ؿɷ����

		bool hasEnemy = false;
		//����������Ƿ����Լ��ĳ�� (��������Ǳ����)
		for (auto it = groupListBuffer.begin(); it != groupListBuffer.end(); ++it){
			if (enemy[*it][depth]){
				hasEnemy = true;
				break;
			}
		}

		if (hasEnemy){
			//��ǰԪ����ͼ���ӵ����ܽ��� do nothing
		}
		else{
			//���Լ��ӽ����� //��Ϊvector��visitʱ�е������ ��list��Ԫ�ظ�����Ҫ������ ����ʵ�Ǹ�listģ��Ķ�ջ
			groupListBuffer.push_back(depth);
			dfsBin(depth + 1);
			groupListBuffer.pop_back();
		}
		// ��ǰԪ�ز����ӵ����: ֱ�Ӿ�����һ��
		dfsBin(depth + 1);
	}
}
int solution(){
	memset(enemy, 0, sizeof(enemy));
	FOR(c, 0, m){
		int le, ri;
		scanf("%d%d", &le, &ri);
		///fprintf(fp, (c == 0 ? "%d %d\n" : "%d %d\n"), le, ri); fprintf(fp, "\n"); ///
		enemy[le - 1][ri - 1] = enemy[ri - 1][le - 1] = 1;
	}
	maxNum = 0;
	//dfs();
	dfsBin();
	FOR(i, 0, n){
		printf(i == 0 ? "%d" : " %d", gr[i]);//Ҫ��1��ʾ�ڶ���
	}

	puts("");
	return 0;
}


#define N 105
int n;
//��ӡ������: ÿ�������ӵڶ�����(��depth=1)��ʼ����(���������ظ��Ļ� �������Բ���)
void dfs(int depth = 1){
	static auto prime = primeSieve(25);//���㷨��myAlgorithm.h
	static bool visited[N] = { false };
	static int ansTemp[N] = { 1 };//��һ������1 vector<int> ansTemp(N, 1)
	if (depth == n
		&& prime[ansTemp[0] + ansTemp[depth - 1]] == 0){//��ʱ��������һ��ʱ �ж���ʱ����(��һ��)
		for (int j = 0; j < n; ++j){
			printf(j == 0 ? "%d" : " %d", ansTemp[j]);
		}
		puts("");
	}
	else{//�ڶ�����ֵ�ķ�Χ��[2, n]
		for (int num = 2; num < n + 1; ++num){
			if (!visited[num]
				&& prime[ansTemp[depth - 1] + num] == 0){//��ʱ��ǰ�����ж�
				visited[num] = true;
				ansTemp[depth] = num;
				dfs(depth + 1);
				ansTemp[depth] = 0;
				visited[num] = false;
			}
		}
	}
}


/*
HDU 1584֩����
https://www.dianlujitao.com/archives/109
*/
#define N 105

int n = 10;//��֪�̶�����10���� 
int minDis;
//�洢i����������ڵ�һ���Ƶ�λ��
int position[N] = { 0 };

void dfs(int depth = 0){
	//λ��һ���������м���false��ʾ ÿ�����������һ���Ʊ�ʾ
	static bool visited[N] = { false };
	static int dis = 0;
	//��֦
	if (dis >= minDis) {
		//����ǰ��Ҫ�ƶ��ľ����Ѿ�������֪�����ž��� ��ôֹͣ��һ����֧������
		return;
	}

	//��Ϊ���ڵ�һ���ƵĻ����� ������ �ʺ���ѡȡ������Ӧ����n-1 
	if (depth == n - 1){
		if (dis < minDis){
			minDis = dis;
		}
	}
	else{
		//�������е����� ѡȡ��һ������  �Ƶ�����ֵ��Χ��[1, 10] ��һ������ĵ�һ���Ƶķ�Χ��[1, 9](�����ѡ�����ڶ���������) 
		for (int i = 1; i < n; ++i){
			if (!visited[i]){
				//����һ����Ϊi���������  ���õ�����ѡ���ĵ�j������ĺ��� 
				visited[i] = true;
				//ѡȡ�ڶ������� (����Ŀ������)
				for (int j = i + 1; j <= n; ++j){
					if (!visited[j]){
						//visit: ������� 
						dis += abs(position[i] - position[j]);
						//�����Դ�Ϊ��������һ������ 
						dfs(depth + 1);
						//�������
						dis -= abs(position[i] - position[j]);
						//�Թ����ŵ��Ǹ��ɷ���(����űȵ�һ������ĵ�һ���ƴ�1�� ��Ȼ�޷�����ƶ�)�������֮��  ��ֱ��������һ����������� ��Ȼ�����س�ʱ 
						break;
						//1 10 2 3 4 5 6 7 8 9
					}
				}
				//������ö������� 
				visited[i] = false;
			}
		}
	}
}

int main(){
	//freopen("input.in", "r", stdin);
	int t;
	scanf("%d", &t);
	while (t-- > 0){
		int sub = 0;
		for (int i = 0; i < n; ++i){
			scanf("%d", &sub);
			//����ĵ�һ���Ƶ���Ϊԭ��
			position[sub] = i;
		}
		minDis = 2147483647;
		dfs();
		printf("%d\n", minDis);
	}
	return 0;
}
/*
*/