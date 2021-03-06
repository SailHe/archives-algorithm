#include"stdafx.h"
#include"./ExtendSpace.h"
#include"./Graph/Graph.h"
#include"./Graph/TreeObject.h"
/*
(对隐式图的深度优先搜索算法)
回溯算法实际上一个类似枚举的搜索尝试过程，
主要是在搜索尝试过程中寻找问题的解，当发现已不满足求解条件时，就“回溯”返回，尝试别的路径。
回溯法是一种选优搜索法，按选优条件向前搜索，以达到目标。但当探索到某一步时，发现原先选择并不优或达不到目标，
就退回一步重新选择，这种走不通就退回再走的技术为回溯法，而满足回溯条件的某个状态的点称为“回溯点”。
许多复杂的，规模较大的问题都可以使用回溯法，有“通用解题方法”的美称。
*/
//回溯搜索与多重循环的区别在于: 前者循环次数的不确定性更高(一般少于后者), 
//更重要的是前者可以实现不确定的循环层数(剪枝: 即去掉不可能产生最优解的结点)
//另一种易于记忆的完全回溯的dfs写法(完全回溯指除了答案外 其余所有变量运行前与运行后完全一致, 外部无需对其初始化)
//实现递归关键是: 只处理这一层, 不管上一层或是下一层, 在需要递归的地方直接调用, 若需要回溯, 在调用后回溯相应的变量就行了



//01串 简单的二分搜索
//对于长度为5位的一个01串，每一位都可能是0或1，一共有32种可能。 每搜索到一种可能就输出
void dfs01Str(int depth = 0) {
	static std::vector<int> a(5, -1);
	if (depth == 5) {
		for (int i = 0; i < 5; ++i) {
			printf(i == 0 ? "%d" : "%d", a[i]);
		}
		puts("");
	}
	else {
		//当前位为0
		a[depth] = 0;
		dfs01Str(depth + 1);
		//当前位为1
		a[depth] = 1;
		dfs01Str(depth + 1);
		a[depth] = -1;
	}
}


//1494: 7.装载问题
/* 另参 最大个数:http://blog.csdn.net/helihui123/article/details/4476624 */
/*
5(n货物数) 10(c最大承载量)
2 2 3 6 9
此wl代码输出9而不是10: http://blog.csdn.net/gakki_529/article/details/78699622

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
	if (ansLoadProblem == cLoadProblem){//只需要最大值能达到多少没必要在达到上限后继续计算
		return;
	}
	else{
		for (int i = depth; i <= nLoadProblem; ++i){
			if (sum + arrLoadProblem[i] > cLoadProblem){
				//arrLoadProblem[i+1] >= arrLoadProblem[i] 这层及之后的都层没必要搜索(排序不仅避免了这一层的多余搜索 还避免了下一层的搜索)
				return;
			}
			else{
				//sum的状态需要在visit的时候用, 
				//由于这里的visit内用到了sum的状态, 采用以前那种回溯法只能处理上一层的sum状态, 当这一层满足条件但下一层不满足条件时sum的状态无法带到下一层
				sum += arrLoadProblem[i];

				//visit(当前层)
				if (sum > ansLoadProblem){
					ansLoadProblem = sum;
				}
				else{
					//do nothing
				}

				dfsLoadProblem(i + 1);//搜索下一层

				sum -= arrLoadProblem[i];//回溯这一层
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

	std::sort(arrLoadProblem + 1, arrLoadProblem + nLoadProblem + 1);//这里的排序很重要
	FOR(i, 0, 100000){
		ansLoadProblem = 0;
		dfsLoadProblem(1);
	}
	printf("%d\n", ansLoadProblem);
	return 0;
}



//1483 字符串全排列
/*
output(只写出个数): 6 120
input:
4
aacc

5
dabce

*/
int nStrFullPermutation,
letterCount[100], //记录每一个字母在字符串中出现的次数
schemeCount;
void dfsStrFullPermutation(int depth = 0){
	static char schemeBuffer[1000]; //记录一个排列方案
	if (depth == nStrFullPermutation){
		++schemeCount;//记录方案总数
		puts(schemeBuffer);
		return;
	}
	else{
		for (int letterSub = 0; letterSub < 26; ++letterSub){
			//每一层搜索是按照26个小写字母循环，若计数器不等于0，就取用该字母
			if (letterCount[letterSub] > 0){
				--letterCount[letterSub];//每次取出一个字母它的计数器就-1
				schemeBuffer[depth] = letterSub + 'a';//visit: depth表示的是length
				dfsStrFullPermutation(depth + 1);
				++letterCount[letterSub];//回溯一步  
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
//库函数版
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


//1481 部落居民 本文提供了三种实现+一种类似的wl实现(未提供代码)
/*
output详见记事本第9*2页
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
int nTribeBodyguard, mTribeBodyguard;//共用变量
struct TribeResident{
	static int ID;
public:
	TribeResident(){
		id = ++ID;
	}
	int id;
	std::list<int> enemy;//存敌对的居民id
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
//这种实现时间复杂度为O(N^2) (当所有人互相敌对时为最坏情况)
//理论上: 若把决策(建)一个结点(即递归一次)的开销记为1的话 此实现比建结果树开销要小一些
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
			//已知不在队内时跳过 1表示不在队内 默认需要所有人都在队内
		}
		else{
			//敌对者最少优先原则 将敌对者相对少的敌对者排除
			for (auto it = allPeoplePtr[i]->enemy.begin(); it != allPeoplePtr[i]->enemy.end(); ++it){
				group[*it - 1] = 1;
			}
		}
	}
	FOR(i, 0, nTribeBodyguard){
		if (group[i] == 1){//如果不在队中
			int count = 0, sub = -1;
			//id优先原则: 在自己的敌对者中只有一个在队中(替换后没有冲突) 且其id大于自己时
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
		printf(i == 0 ? "%d" : " %d", !group[i]);//要求1表示在队内
	}

	puts("");
	return 0;
}

int enemyTribeBodyguard[100][100];
int groupTribeBodyguard[100];
int maxNum;
//结果树是颗非完全N叉树 h: O(N)(没有敌对关系是颗完全N叉树 此时h=N) 内部有一个O(N^2)的迭代(所有人互相敌对时是N^2)
//这种实现理论上很平庸 相对易于理解一些 不过如果要求队伍的顺序的话 可以用这个改编
void dfsTribeBodyguard(int depth = 0){
	//改为队伍内的敌对者std::vector在决策以及visit时有优势 但在回溯前后需要遍历其敌对者
	//一个折衷的办法是使用有序的std::list 这提升了决策效率
	static std::list<int> groupListBuffer;//表示sub(模拟的堆栈)

	//剪枝: 当前选好的 + 还没有搜索到的 < 当前已知的最大值(等于的时候要非0懒得判断了)
	int tmp = MAX_INT32;
	Utility::AssertToSignedNum(groupListBuffer.size() + nTribeBodyguard - depth, tmp);
	if (tmp < maxNum)
		return;

	if (depth == nTribeBodyguard){
		return;
	}
	else{
		//基于一点: sub < depth的元素在前几层决策过了(注意, 一层可能决策多个元素)
		//每层决策出的元素集合构成那一层的唯一最优结点(唯一与最优是试错试出来的)
		FOR(i, depth, nTribeBodyguard){
			bool hasEnemy = false;
			//检查卫队内是否有自己的仇敌 (这个遍历是必须的, 这里是O(N^2))
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
				//把自己加进卫队
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
//结果树是颗完全二叉树 h=N (与wl版有一点差别 但效果差不多 敌对关系较多时使用这个版本的开销更小)
void dfsBinTribeBodyguard(int depth = 0){
	static std::list<int> groupListBuffer;//表示sub

	//剪枝: 当前选好的 + 还没有搜索到的 < 当前已知的最大值(等于的时候要非0懒得判断了) 若不加这句 那么建的必定是颗CBT
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
		//每个元素只有进队 与不进队两种状态 答案与进队顺序无关 所以建一颗二叉树(每层决策一个元素)就可以了没必要n叉

		//决策当前元素可否进队

		bool hasEnemy = false;
		//检查卫队内是否有自己的仇敌 (这个遍历是必须的)
		for (auto it = groupListBuffer.begin(); it != groupListBuffer.end(); ++it){
			if (enemyTribeBodyguard[*it][depth]){
				hasEnemy = true;
				break;
			}
		}

		if (hasEnemy){
			//当前元素试图进队但不能进队 do nothing
		}
		else{
			//把自己加进卫队 //换为std::vector在visit时有点点优势 但std::list对元素个数的要求更灵活 这其实是个std::list模拟的堆栈
			groupListBuffer.push_back(depth);
			dfsBinTribeBodyguard(depth + 1);
			groupListBuffer.pop_back();
		}
		// 当前元素不进队的情况: 直接决策下一层
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
		printf(i == 0 ? "%d" : " %d", groupTribeBodyguard[i]);//要求1表示在队内
	}

	puts("");
	return 0;
}


// 正整数n，把整数1,2,3,4..n组成一个环，使得相邻的两个整数之和均为素数。输出是，从整数1开始逆时针排列 n <= 16
// 打印素数环: 每个环都从第二个数(即depth=1)开始搜索(否则会出现重复的环 而且难以查重)
// 有的数比如11是没有素数环的
void dfsPrimeCircle(int depth = 1, int nPrimeCircle = 16){
	static const int N = 105;
	static std::unique_ptr<int> primeRep(MathExtend::generateSievePrimeS(25));
	static bool visited[N] = { false };
	static int ansTemp[N] = { 1 };//第一个数是1 std::vector<int> ansTemp(N, 1)
	int *prime = primeRep.get();
	if (depth == nPrimeCircle
		&& prime[ansTemp[0] + ansTemp[depth - 1]] == 0){//现时结点是最后一个时 判断现时与后继(第一个)
		for (int j = 0; j < nPrimeCircle; ++j){
			//printf(j == 0 ? "%d" : " %d", ansTemp[j]);
		}
		//puts("");
	}
	else{//第二个数值的范围是[2, nPrimeCircle]
		for (int num = 2; num < nPrimeCircle + 1; ++num){
			if (!visited[num]
				&& prime[ansTemp[depth - 1] + num] == 0){//现时与前驱的判断
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
HDU 1584蜘蛛牌
https://www.dianlujitao.com/archives/109
*/
#define N_SpiderCard 105

int nSpiderCard = 10;//已知固定共有10张牌
int minDisSpiderCard;
//存储i这张牌相对于第一张牌的位置
int positionSpiderCard[N_SpiderCard] = { 0 };

void dfsForSpiderCard(int depth = 0){
	//位于一个牌纵列中间用false表示 每组牌列用其第一张牌表示
	static bool visited[N_SpiderCard] = { false };
	static int dis = 0;
	//剪枝
	if (dis >= minDisSpiderCard) {
		//若当前需要移动的距离已经超过已知的最优距离 那么停止这一个分支的搜索
		return;
	}

	//因为是在第一张牌的基础上 搜索的 故后面选取的牌数应当是nSpiderCard-1 
	if (depth == nSpiderCard - 1){
		if (dis < minDisSpiderCard){
			minDisSpiderCard = dis;
		}
	}
	else{
		//遍历所有的牌组 选取第一个牌组  牌的牌面值范围是[1, 10] 第一个牌组的第一张牌的范围是[1, 9](否则就选不出第二个牌组了) 
		for (int i = 1; i < nSpiderCard; ++i){
			if (!visited[i]){
				//将第一张牌为i的这个牌组  放置到待会选出的第j个牌组的后面 
				visited[i] = true;
				//选取第二个牌组 (放置目标牌组)
				for (int j = i + 1; j <= nSpiderCard; ++j){
					if (!visited[j]){
						//visit: 距离计算 
						dis += abs(positionSpiderCard[i] - positionSpiderCard[j]);
						//搜索以此为基础的下一个牌组 
						dfsForSpiderCard(depth + 1);
						//距离回溯
						dis -= abs(positionSpiderCard[i] - positionSpiderCard[j]);
						//试过最优的那个可放置(即编号比第一个牌组的第一张牌大1的 不然无法完成移动)的牌组后之后  就直接跳出这一个牌组的搜索 不然会严重超时 
						break;
						//1 10 2 3 4 5 6 7 8 9
					}
				}
				//牌组放置动作回溯 
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
			//读入的第一张牌的作为原点
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
	// 图命名空间
	// 递归函数指针的情况
	std::unique_ptr<Graph> g(new AdjacentMatrixGraph(100));
	g->insertEdge(Graph::Edge(0, 1, 10));
	g->insertEdge(Graph::Edge(1, 2, 2));
	g->insertEdge(Graph::Edge(0, 2, 11));
	g->insertEdge(Graph::Edge(2, 3, 3));
	g->setCustomVisit([](Graph::VertexKey v) {
		//printf(" 访问顶点: %d", v);
	});
	g->setDestVertex(3);
	// 有重载的函数模板颜色与普通函数一致
	avlTime = StandardExtend::testAndDiffClock([&]() {
		g->dfs(0);
	}, 10, "图dfs");

	// 递归lambda函数的情况
	std::unique_ptr<LinkedBinSearchTree<int>> bt(new LinkedBinSearchTree<int>());
	for (int i = 0; i < 2000; ++i) {
		bt->insert(i);
	}
	avlTime = StandardExtend::testAndDiffClock([&]() {
		bt->traversal(Tree::ORDER_LEVEL, [](BinTree<int>::BT b) {
			//printf(" 访问数据: %d", b->Data);
			return false;
		});
	}, 10, "二叉搜索树的顺序遍历");

	// 递归传参的情况
	avlTime = StandardExtend::testAndDiffClock([&]() {
		dfsPrimeCircle();
	}, 10, "素数环");
	// release下(注释了方法内的printf语句) 100遍平均 测两次
	// 递归参数 0.07s(精度不足) 0.0752 0.0726 0.0698 0.0737
	// 全局的一个int变量 0.0194s(偶然) 0.0717; 0.0701 0.0714 0.0694 0.0727
	// 结论是: 有差异 但差异不大, 一个输出语句或是稍微改进算法就能弥补
	//solutionForSpiderCard();
	return 0;
}
