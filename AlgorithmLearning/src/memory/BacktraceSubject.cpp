#include"stdafx.h"
#include"./ExtendSpace.h"
#include"./Graph/Graph.h"
#include"./Graph/TreeObject.h"
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
void dfs01Str(int depth = 0) {
	static std::vector<int> a(5, -1);
	if (depth == 5) {
		for (int i = 0; i < 5; ++i) {
			printf(i == 0 ? "%d" : "%d", a[i]);
		}
		puts("");
	}
	else {
		//��ǰλΪ0
		a[depth] = 0;
		dfs01Str(depth + 1);
		//��ǰλΪ1
		a[depth] = 1;
		dfs01Str(depth + 1);
		a[depth] = -1;
	}
}


//1494: 7.װ������
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
int nLoadProblem, cLoadProblem;
int arrLoadProblem[1000];
int ansLoadProblem;
void dfsLoadProblem(int depth){
	static int sum = 0;
	if (ansLoadProblem == cLoadProblem){//ֻ��Ҫ���ֵ�ܴﵽ����û��Ҫ�ڴﵽ���޺��������
		return;
	}
	else{
		for (int i = depth; i <= nLoadProblem; ++i){
			if (sum + arrLoadProblem[i] > cLoadProblem){
				//arrLoadProblem[i+1] >= arrLoadProblem[i] ��㼰֮��Ķ���û��Ҫ����(���򲻽���������һ��Ķ������� ����������һ�������)
				return;
			}
			else{
				//sum��״̬��Ҫ��visit��ʱ����, 
				//���������visit���õ���sum��״̬, ������ǰ���ֻ��ݷ�ֻ�ܴ�����һ���sum״̬, ����һ��������������һ�㲻��������ʱsum��״̬�޷�������һ��
				sum += arrLoadProblem[i];

				//visit(��ǰ��)
				if (sum > ansLoadProblem){
					ansLoadProblem = sum;
				}
				else{
					//do nothing
				}

				dfsLoadProblem(i + 1);//������һ��

				sum -= arrLoadProblem[i];//������һ��
			}
		}
	}
}
int solutionForLoadProblem(){
	for (int i = 1; i <= nLoadProblem; i++){
		scanf("%d", &arrLoadProblem[i]);
		/// fprintf(fp, (i == 1 ? "%d" : " %d"), arrLoadProblem[i]); 
	}
	///fprintf(fp, "\n");

	std::sort(arrLoadProblem + 1, arrLoadProblem + nLoadProblem + 1);//������������Ҫ
	FOR(i, 0, 100000){
		ansLoadProblem = 0;
		dfsLoadProblem(1);
	}
	printf("%d\n", ansLoadProblem);
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
int nStrFullPermutation,
letterCount[100], //��¼ÿһ����ĸ���ַ����г��ֵĴ���
schemeCount;
void dfsStrFullPermutation(int depth = 0){
	static char schemeBuffer[1000]; //��¼һ�����з���
	if (depth == nStrFullPermutation){
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
				dfsStrFullPermutation(depth + 1);
				++letterCount[letterSub];//����һ��  
			}
		}
	}
}
int solutionStrFullPermutation(){
	char str[1000];
	std::cin >> str;
	memset(letterCount, 0, sizeof(letterCount));
	schemeCount = 0;
	for (int i = 0; i < nStrFullPermutation; ++i){
		++letterCount[str[i] - 'a'];
	}
	dfsStrFullPermutation();
	printf("%d\n", schemeCount);
	return 0;
}
//�⺯����
int solutionStrFullPermutation1(){
	int schemeCount = 0;
	char str[1000];
	std::cin >> str;
	std::sort(str, str + nStrFullPermutation);
	do{
		++schemeCount;
		//cout << str << endl;//50ms
		puts(str);//32ms
	} while (std::next_permutation(str, str + nStrFullPermutation));
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
int nTribeBodyguard, mTribeBodyguard;//���ñ���
struct TribeResident{
	static int ID;
public:
	TribeResident(){
		id = ++ID;
	}
	int id;
	std::list<int> enemy;//��жԵľ���id
	bool operator<(TribeResident const &rhs)const{
		int ls = enemy.size(), rs = rhs.enemy.size();
		return ls == rs ? id < rhs.id :
			ls < rs;
	}
};
int TribeResident::ID = 0;
bool ptrLessTribeBodyguard(TribeResident const *lhs, TribeResident const *rhs){
	return (*lhs) < (*rhs);
}
//����ʵ��ʱ�临�Ӷ�ΪO(N^2) (�������˻���ж�ʱΪ����)
//������: ���Ѿ���(��)һ�����(���ݹ�һ��)�Ŀ�����Ϊ1�Ļ� ��ʵ�ֱȽ����������ҪСһЩ
int solutionTribeBodyguard(){

	TribeResident allPeople[100];
	TribeResident *allPeoplePtr[100];
	int group[100];
	memset(group, 0, sizeof(int)*nTribeBodyguard);

	FOR(i, 0, nTribeBodyguard){
		allPeople[i].enemy.clear();
	}
	FOR(c, 0, nTribeBodyguard){
		allPeoplePtr[c] = allPeople + c;
	}
	FOR(c, 0, mTribeBodyguard){
		int le, ri;
		scanf("%d%d", &le, &ri);
		///fprintf(fp, (c == 0 ? "%d %d\n" : "%d %d\n"), le, ri); fprintf(fp, "\n"); ///
		allPeople[le - 1].enemy.push_back(ri);
		allPeople[ri - 1].enemy.push_back(le);
	}

	std::sort(allPeoplePtr, allPeoplePtr + nTribeBodyguard, ptrLessTribeBodyguard);

	FOR(i, 0, nTribeBodyguard){
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
	FOR(i, 0, nTribeBodyguard){
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
				std::swap(group[i], group[sub]);
			}
		}
	}
	FOR(i, 0, nTribeBodyguard){
		printf(i == 0 ? "%d" : " %d", !group[i]);//Ҫ��1��ʾ�ڶ���
	}

	puts("");
	return 0;
}

int enemyTribeBodyguard[100][100];
int groupTribeBodyguard[100];
int maxNum;
//������ǿŷ���ȫN���� h: O(N)(û�ежԹ�ϵ�ǿ���ȫN���� ��ʱh=N) �ڲ���һ��O(N^2)�ĵ���(�����˻���ж�ʱ��N^2)
//����ʵ�������Ϻ�ƽӹ ����������һЩ �������Ҫ������˳��Ļ� ����������ı�
void dfsTribeBodyguard(int depth = 0){
	//��Ϊ�����ڵĵж���std::vector�ھ����Լ�visitʱ������ ���ڻ���ǰ����Ҫ������ж���
	//һ�����Եİ취��ʹ�������std::list �������˾���Ч��
	static std::list<int> groupListBuffer;//��ʾsub(ģ��Ķ�ջ)

	//��֦: ��ǰѡ�õ� + ��û���������� < ��ǰ��֪�����ֵ(���ڵ�ʱ��Ҫ��0�����ж���)
	int tmp = MAX_INT32;
	Utility::AssertToSignedNum(groupListBuffer.size() + nTribeBodyguard - depth, tmp);
	if (tmp < maxNum)
		return;

	if (depth == nTribeBodyguard){
		return;
	}
	else{
		//����һ��: sub < depth��Ԫ����ǰ������߹���(ע��, һ����ܾ��߶��Ԫ��)
		//ÿ����߳���Ԫ�ؼ��Ϲ�����һ���Ψһ���Ž��(Ψһ���������Դ��Գ�����)
		FOR(i, depth, nTribeBodyguard){
			bool hasEnemy = false;
			//����������Ƿ����Լ��ĳ�� (��������Ǳ����, ������O(N^2))
			for (auto it = groupListBuffer.begin(); it != groupListBuffer.end(); ++it){
				if (enemyTribeBodyguard[*it][i]){
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
				int tmp = MAX_INT32;
				Utility::AssertToSignedNum(groupListBuffer.size(), tmp);
				if (tmp > maxNum){
					maxNum = groupListBuffer.size();
					memset(groupTribeBodyguard, 0, sizeof(groupTribeBodyguard));
					for (auto it = groupListBuffer.begin(); it != groupListBuffer.end(); ++it){
						groupTribeBodyguard[*it] = 1;
					}
				}

				dfsTribeBodyguard(i + 1);
				groupListBuffer.pop_back();
			}
		}
	}
}
//������ǿ���ȫ������ h=N (��wl����һ���� ��Ч����� �жԹ�ϵ�϶�ʱʹ������汾�Ŀ�����С)
void dfsBinTribeBodyguard(int depth = 0){
	static std::list<int> groupListBuffer;//��ʾsub

	//��֦: ��ǰѡ�õ� + ��û���������� < ��ǰ��֪�����ֵ(���ڵ�ʱ��Ҫ��0�����ж���) ��������� ��ô���ıض��ǿ�CBT
	int tmp = MAX_INT32;
	Utility::AssertToSignedNum(groupListBuffer.size() + nTribeBodyguard - depth, tmp);
	if (tmp < maxNum)
		return;

	if (depth == nTribeBodyguard){
		//visit
		tmp = -1;
		Utility::AssertToSignedNum(groupListBuffer.size(), tmp);
		if (tmp > maxNum){
			maxNum = groupListBuffer.size();
			memset(groupTribeBodyguard, 0, sizeof(groupTribeBodyguard));
			for (auto it = groupListBuffer.begin(); it != groupListBuffer.end(); ++it){
				groupTribeBodyguard[*it] = 1;
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
			if (enemyTribeBodyguard[*it][depth]){
				hasEnemy = true;
				break;
			}
		}

		if (hasEnemy){
			//��ǰԪ����ͼ���ӵ����ܽ��� do nothing
		}
		else{
			//���Լ��ӽ����� //��Ϊstd::vector��visitʱ�е������ ��std::list��Ԫ�ظ�����Ҫ������ ����ʵ�Ǹ�std::listģ��Ķ�ջ
			groupListBuffer.push_back(depth);
			dfsBinTribeBodyguard(depth + 1);
			groupListBuffer.pop_back();
		}
		// ��ǰԪ�ز����ӵ����: ֱ�Ӿ�����һ��
		dfsBinTribeBodyguard(depth + 1);
	}
}
int solutionTribeBodyguard1(){
	memset(enemyTribeBodyguard, 0, sizeof(enemyTribeBodyguard));
	FOR(c, 0, mTribeBodyguard){
		int le, ri;
		scanf("%d%d", &le, &ri);
		///fprintf(fp, (c == 0 ? "%d %d\n" : "%d %d\n"), le, ri); fprintf(fp, "\n"); ///
		enemyTribeBodyguard[le - 1][ri - 1] = enemyTribeBodyguard[ri - 1][le - 1] = 1;
	}
	maxNum = 0;
	//dfs();
	dfsBinTribeBodyguard();
	FOR(i, 0, nTribeBodyguard){
		printf(i == 0 ? "%d" : " %d", groupTribeBodyguard[i]);//Ҫ��1��ʾ�ڶ���
	}

	puts("");
	return 0;
}


// ������n��������1,2,3,4..n���һ������ʹ�����ڵ���������֮�;�Ϊ����������ǣ�������1��ʼ��ʱ������ n <= 16
// ��ӡ������: ÿ�������ӵڶ�����(��depth=1)��ʼ����(���������ظ��Ļ� �������Բ���)
// �е�������11��û����������
void dfsPrimeCircle(int depth = 1, int nPrimeCircle = 16){
	static const int N = 105;
	static std::unique_ptr<int> primeRep(MathExtend::generateSievePrimeS(25));
	static bool visited[N] = { false };
	static int ansTemp[N] = { 1 };//��һ������1 std::vector<int> ansTemp(N, 1)
	int *prime = primeRep.get();
	if (depth == nPrimeCircle
		&& prime[ansTemp[0] + ansTemp[depth - 1]] == 0){//��ʱ��������һ��ʱ �ж���ʱ����(��һ��)
		for (int j = 0; j < nPrimeCircle; ++j){
			//printf(j == 0 ? "%d" : " %d", ansTemp[j]);
		}
		//puts("");
	}
	else{//�ڶ�����ֵ�ķ�Χ��[2, nPrimeCircle]
		for (int num = 2; num < nPrimeCircle + 1; ++num){
			if (!visited[num]
				&& prime[ansTemp[depth - 1] + num] == 0){//��ʱ��ǰ�����ж�
				visited[num] = true;
				ansTemp[depth] = num;
				dfsPrimeCircle(depth + 1, nPrimeCircle);
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
#define N_SpiderCard 105

int nSpiderCard = 10;//��֪�̶�����10����
int minDisSpiderCard;
//�洢i����������ڵ�һ���Ƶ�λ��
int positionSpiderCard[N_SpiderCard] = { 0 };

void dfsForSpiderCard(int depth = 0){
	//λ��һ���������м���false��ʾ ÿ�����������һ���Ʊ�ʾ
	static bool visited[N_SpiderCard] = { false };
	static int dis = 0;
	//��֦
	if (dis >= minDisSpiderCard) {
		//����ǰ��Ҫ�ƶ��ľ����Ѿ�������֪�����ž��� ��ôֹͣ��һ����֧������
		return;
	}

	//��Ϊ���ڵ�һ���ƵĻ����� ������ �ʺ���ѡȡ������Ӧ����nSpiderCard-1 
	if (depth == nSpiderCard - 1){
		if (dis < minDisSpiderCard){
			minDisSpiderCard = dis;
		}
	}
	else{
		//�������е����� ѡȡ��һ������  �Ƶ�����ֵ��Χ��[1, 10] ��һ������ĵ�һ���Ƶķ�Χ��[1, 9](�����ѡ�����ڶ���������) 
		for (int i = 1; i < nSpiderCard; ++i){
			if (!visited[i]){
				//����һ����Ϊi���������  ���õ�����ѡ���ĵ�j������ĺ��� 
				visited[i] = true;
				//ѡȡ�ڶ������� (����Ŀ������)
				for (int j = i + 1; j <= nSpiderCard; ++j){
					if (!visited[j]){
						//visit: ������� 
						dis += abs(positionSpiderCard[i] - positionSpiderCard[j]);
						//�����Դ�Ϊ��������һ������ 
						dfsForSpiderCard(depth + 1);
						//�������
						dis -= abs(positionSpiderCard[i] - positionSpiderCard[j]);
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
int solutionForSpiderCard(){
	//freopen("input.in", "r", stdin);
	int t;
	scanf("%d", &t);
	while (t-- > 0){
		int sub = 0;
		for (int i = 0; i < nSpiderCard; ++i){
			scanf("%d", &sub);
			//����ĵ�һ���Ƶ���Ϊԭ��
			positionSpiderCard[sub] = i;
		}
		minDisSpiderCard = MAX_INT32;
		dfsForSpiderCard();
		printf("%d\n", minDisSpiderCard);
	}
	return 0;
}

int main() {
	double avlTime;
	// ͼ�����ռ�
	// �ݹ麯��ָ������
	std::unique_ptr<Graph> g(new AdjacentMatrixGraph(100));
	g->insertEdge(Graph::Edge(0, 1, 10));
	g->insertEdge(Graph::Edge(1, 2, 2));
	g->insertEdge(Graph::Edge(0, 2, 11));
	g->insertEdge(Graph::Edge(2, 3, 3));
	g->setCustomVisit([](Graph::VertexKey v) {
		//printf(" ���ʶ���: %d", v);
	});
	g->setDestVertex(3);
	// �����صĺ���ģ����ɫ����ͨ����һ��
	avlTime = StandardExtend::testAndDiffClock([&]() {
		g->dfs(0);
	}, 10, "ͼdfs");

	// �ݹ�lambda���������
	std::unique_ptr<LinkedBinSearchTree<int>> bt(new LinkedBinSearchTree<int>());
	for (int i = 0; i < 2000; ++i) {
		bt->insert(i);
	}
	avlTime = StandardExtend::testAndDiffClock([&]() {
		bt->traversal(Tree::ORDER_LEVEL, [](BinTree<int>::BT b) {
			//printf(" ��������: %d", b->Data);
			return false;
		});
	}, 10, "������������˳�����");

	// �ݹ鴫�ε����
	avlTime = StandardExtend::testAndDiffClock([&]() {
		dfsPrimeCircle();
	}, 10, "������");
	// release��(ע���˷����ڵ�printf���) 100��ƽ�� ������
	// �ݹ���� 0.07s(���Ȳ���) 0.0752 0.0726 0.0698 0.0737
	// ȫ�ֵ�һ��int���� 0.0194s(żȻ) 0.0717; 0.0701 0.0714 0.0694 0.0727
	// ������: �в��� �����첻��, һ�������������΢�Ľ��㷨�����ֲ�
	//solutionForSpiderCard();
	return 0;
}
