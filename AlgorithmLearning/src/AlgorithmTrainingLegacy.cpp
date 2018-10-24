#include "ExtendSpace.h"
using namespace std;
using namespace StandardExtend;

template<class ElementType>
//并查集模板类
class DisjointSet{
public:
	//using ElementType = int;
	//用集合的root下标表示集合
	using SetRoot = int;
	using SetBaseType = vector<ElementType>;
	//using SetBaseType = ElementType[MAX_N];
	//在class出现vector在http://139.196.145.92上会Runtime Error
	//vector<ElementType> debugTemp1;

	DisjointSet(){}
	DisjointSet(size_t n){
		//新的元素初始值也必须是-1 每个元素都代表一个单独的集合
		disjointSetBase.assign(n, -1);
		//memset(disjointSetBase, -1, n*sizeof(ElementType));
	}

	void unionSet(SetRoot root1, SetRoot root2){
		if (root1 == root2){
			return;
		}
		//保证小集合并入大集合
		if (disjointSetBase[root2] < disjointSetBase[root1]){
			//1并入2
			disjointSetBase[root2] += disjointSetBase[root1];
			disjointSetBase[root1] = root2;
		}
		else{
			disjointSetBase[root1] += disjointSetBase[root2];
			disjointSetBase[root2] = root1;
		}
	}

	void unionElement(ElementType x1, ElementType x2){
		unionSet(findRoot(x1), findRoot(x2));
	}

	SetRoot findRoot(ElementType x){
		//集合默认初始值为1
		return disjointSetBase[x] < 0 ? x//找到集合的根
			: disjointSetBase[x] = findRoot(disjointSetBase[x]);//继续寻找并进行路径压缩
	}

	size_t size(SetRoot root){
		return -disjointSetBase[root];
	}

	void init(){
		disjointSetBase.assign(disjointSetBase.size(), -1);
		//memset(disjointSetBase, -1, sizeof(SetBaseType));
	}

	void reSize(size_t size){
		disjointSetBase.resize(size, -1);
		//memset(disjointSetBase, -1, sizeof(SetBaseType));
	}

	//返回集合数
	size_t setCount(){
		size_t cnt = 0;
		for (size_t i = 0; i < disjointSetBase.size(); ++i){
			cnt += disjointSetBase[i] < 0 ? 1 : 0;
		}
		return cnt;
	}
	
private:
	SetBaseType disjointSetBase;
};

//任意进制加法(错误的)
void basePlus(string &lhs, string &rhs, int base, string &sum){
	int maxSize = max(lhs.size(), rhs.size());
	reverse(lhs.begin(), lhs.end());
	reverse(rhs.begin(), rhs.end());
	//cout<<lhs<<endl<<rhs<<endl;
	lhs.resize(maxSize, '0');
	rhs.resize(maxSize, '0');
	sum.resize(maxSize);
	//cout<<lhs<<endl<<rhs<<endl;
	//进位
	int pastBit = 0;
	for (int i = 0; i < maxSize; ++i){
		//cout<<toIntNum(lhs[i])<<"+"<<toIntNum(rhs[i])<<"+"<<pastBit<<endl;
		int bit = toIntNum(lhs[i]) + toIntNum(rhs[i]) + pastBit;
		sum[i] = toUppercaseAscllChar(bit % base);
		//printf("%d->%c ", bit % base, sum[i]);
		pastBit = bit / base;
	}
	while (pastBit != 0){
		int bit = pastBit;
		sum.push_back(toUppercaseAscllChar(bit % base));
		pastBit = bit / base;
	}

	reverse(lhs.begin(), lhs.end());
	reverse(rhs.begin(), rhs.end());
	reverse(sum.begin(), sum.end());
	if (sum.size() > 1){
		int non0Sub = 0;
		//cout<<"before"<<sum<<endl;
		for (auto it = sum.begin(); it != sum.end(); ++it){
			if (*it == '0'){
				++non0Sub;
			}
			else{
				break;
			}
		}
		//cout<<non0Sub<<" "<<sum<<endl;
		sum = sum.substr(non0Sub);
	}
}

struct VertexNode{
	int weight;
	int targetID;
	VertexNode(){}
	VertexNode(int targetID, int weight){
		this->weight = weight;
		this->targetID = targetID;
	}
};

vector<list<VertexNode>> nGraph;
void vOutputGraph(int n){
	puts("----------------------------");
	for (int i = 0; i < n; ++i){
		printf("vertex %d\n", i);
		for (auto it = nGraph[i].begin(); it != nGraph[i].end(); ++it){
			cout << i << " " << it->targetID << endl;
		}
	}
}

//标准的简易深度搜索 返回值是与起始点连通的顶点个数;
//PS: 回溯搜索参数名称: depth; 图的深度优先搜索参数名称: pastId
int dfsStandard(int pastId, vector<bool> &visited){
	int cnt = 1;
	visited[pastId] = true;
	for (auto it = nGraph[pastId].begin(); it != nGraph[pastId].end(); ++it){
		if (!visited[it->targetID]){
			cnt += dfsStandard(it->targetID, visited);
		}
	}
	//若回溯了则返回的是标准dfs的总迭代次数 不回溯的话事实上并没有遍历每一条边
	//visited[pastId] = false;
	return cnt;
}

int bfsStandard(int startID, vector<int> &dist){
	int cnt = 0;
	queue<int> q;
	q.push(startID);
	dist[startID] = 0;
	while (!q.empty()){
		int v = q.front();
		q.pop();
		++cnt;
		//遍历v的所有边
		for (auto it = nGraph[v].begin(); it != nGraph[v].end(); ++it){
			if (-1 == dist[it->targetID]){
				dist[it->targetID] = dist[v] + 1;
				q.push(it->targetID);
			}
		}
	}
	return cnt;
}

/*
6 10
1 2
1 3
1 4
1 5
1 6
2 3
2 4
3 4
3 5
3 6

4 3
1 2
1 3
2 3
*/
//判断图的连通性
int MainJudgeConnectivity(){
	int n, e;
	vector<bool> visited;
	vector<int> dist;
	while (2 == scanf("%d%d", &n, &e)){
		AdjacentListGraph adjGraph(n);
		DisjointSet<int> nDisjointSet(n);
		//nDisjointSet.reSize(n);
		//nDisjointSet.init();
		nGraph.clear();
		nGraph.resize(n);
		int n1, n2;
		for (int i = 0; i < e; ++i){
			scanf("%d%d", &n1, &n2);
			--n1;
			--n2;
			nDisjointSet.unionElement(n1, n2);
			adjGraph.insertEdgeUndirected(Graph::Edge(n1, n2, 1));
			nGraph[n1].push_back(VertexNode(n2, 1));
			nGraph[n2].push_back(VertexNode(n1, 1));
		}
		//vOutputGraph(n);
		adjGraph.setDestVertex(n - 1);
		//库中的图类dfs返回的是路径条数 如果需要检测连通性的话调用bfs 或是 并查集 或是获取visited做判断
		int adjDfsCnt = adjGraph.dfs(0);
		adjGraph.setDestVertex(- 1);
		int adjBfsCnt = adjGraph.bfs(0, dist, vector<int>());
		visited.assign(n, false);
		dist.assign(n, -1);
		int disjointSetCnt = nDisjointSet.size(nDisjointSet.findRoot(0));
		int bfsCnt = bfsStandard(0, dist);
		int dfsCnt = dfsStandard(0, visited);
		puts(bfsCnt == n ? "Yes" : "No");
	}
	return 0;
}

/*
2

5
13
11 7
12 7 26
6 14 15 8
12 7 13 24 11

5
13
11 8
12 7 26
6 14 15 8
12 7 13 24 11
*/
int rows, group;
int array2D[MAX_R][MAX_C];
int arrayAAO[MAX_R][MAX_C];

void vTrangleMinValueAndPathOutput(vector<int> &path){
	auto it = path.begin();
	if (it != path.end()){
		cout << *it++ << endl;
	}
	bool outPutFlag = false;
	if (it != path.end()){
		cout << *it++;
		outPutFlag = true;
	}
	while (it != path.end()){
		cout << "-->" << *it++;
	}
	if (outPutFlag){
		puts("");
	}
}

int pathColAA[MAX_R][MAX_C];

void TrangleMinValueAndPath(){
	vector<int> resultPath;
	int r = rows - 2;
	int pastPathSub = -1;
	for (int c = 0; c < rows; ++c){
		pathColAA[rows - 1][c] = c;
	}
	for (; r >= 0; --r){
		for (int c = 0; c < r + 1; ++c){
			int add = 0;
			if (array2D[r + 1][c] > array2D[r + 1][c + 1]){
				add = array2D[r + 1][c + 1];
				pastPathSub = c + 1;
			}
			else{
				//下一行的第c列和第c+1列相等时 选取序号小的那一列: c
				add = array2D[r + 1][c];
				pastPathSub = c;
			}
			array2D[r][c] += add;
			pathColAA[r][c] = pastPathSub;
		}
	}
	resultPath.push_back(array2D[0][0]);
	int nextcolSub = 0;
	for (int r = 0; r < rows; ++r){
		resultPath.push_back(arrayAAO[r][nextcolSub]);
		nextcolSub = pathColAA[r][nextcolSub];
	}
	vTrangleMinValueAndPathOutput(resultPath);
}

int MainTrangleMinValueAndPath(){
	while (1 == scanf("%d", &group)){
		for (int i = 0; i < group; ++i){
			scanf("%d", &rows);
			for (int r = 0; r < rows; ++r){
				for (int c = 0; c < r + 1; ++c){
					scanf("%d", &array2D[r][c]);
					arrayAAO[r][c] = array2D[r][c];
				}
			}
			//outPut2DArray(array2D, rows);
			TrangleMinValueAndPath();
		}
	}
	return 0;
}


/*************pta_5_7_13*****************/

int MainStatisticsScore(){
	vector<double> arrayA;
	int group, n;
	double maxScore, avlScore, sumScore;
	while (1 == scanf("%d", &group)){
		//cout<<"group:"<<group<<endl;
		while (group-- > 0){
			scanf("%d", &n);
			arrayA.clear();
			arrayA.resize(n);
			//cout<<"n:"<<n<<endl;
			sumScore = 0.0;
			maxScore = -1.0;
			for (int i = 0; i < n; ++i){
				scanf("%lf", &arrayA[i]);
				sumScore += arrayA[i];
				maxScore = max(maxScore, arrayA[i]);
			}
			avlScore = sumScore / n;
			sort(arrayA.rbegin(), arrayA.rend());
			//outputArray(arrayA);
			//cout<<"sum:"<<sumScore<<" max:"<<maxScore<<" avl:"<<avlScore<<" p20:"<<arrayA[20-1]<<endl;
			//最高成绩与排名第二十名学生的成绩分差
			printf("%.0f\n", maxScore - arrayA[19]);
			//最高分与平均分的分差
			printf("%.2f\n", maxScore - avlScore);
		}
	}
	return 0;
}

int MainChoseBabies(){
	vector<int> arrayA;
	int group, n;
	int needCnt, needCntSub, minHeight, maxHeight;
	while (1 == scanf("%d", &group)){
		//cout<<"group:"<<group<<endl;
		while (group-- > 0){
			scanf("%d%d", &n, &needCnt);
			//cout<<"n:"<<n<<" needCnt:"<<needCnt<<endl;
			needCntSub = needCnt - 1;
			arrayA.clear();
			arrayA.resize(needCnt);
			scanf("%d%d", &minHeight, &maxHeight);
			for (int i = 0; i < n; ++i){
				int sex, hei, gr;
				scanf("%d%d%d", &sex, &hei, &gr);
				if (sex == 0 && (gr == 1 || gr == 2) && minHeight <= hei && hei <= maxHeight && needCnt > 0){
					//arrayA.push_back(i+1);
					if (needCnt > 0){
						arrayA[arrayA.size() - needCnt] = i + 1;
					}
					--needCnt;
				}
			}
			//outputArray(arrayA);
			if (needCnt == 0){
				printf("%d\n", arrayA[needCntSub]);
			}
			else{
				puts("No such student!");
			}
		}
	}
	return 0;
}

/*
公共交通
http://139.196.145.92/contest_show.php?cid=350#problem/C
难的一版
http://139.196.145.92/contest_show.php?cid=354#problem/E
5
14 12 2 18 15
6
4 9 0 16 16 12
*/
int MainBusStop(){
	vector<int> arrayA;
	vector<int> arraySum;
	int n;
	int targetSub, minSumStep, sumStep;
	while (1 == scanf("%d", &n)){
		arrayA.clear();
		arrayA.resize(n);
		arraySum.clear();
		arraySum.resize(n);
		for (int i = 0; i < n; ++i){
			scanf("%d", &arrayA[i]);
		}
		minSumStep = 2147483647;
		//选取第i个站点作为目的地
		for (int i = 0; i < n; ++i){
			sumStep = 0;
			for (int j = 0; j < n; j++){
				sumStep += arrayA[j] * (abs(j - i));
			}
			//cout<<"sumStep:"<<sumStep<<endl;
			if (sumStep < minSumStep){
				targetSub = i;
				minSumStep = sumStep;
			}
			//minSumStep = min(minSumStep, sumStep);
		}
		cout << targetSub + 1 << " " << minSumStep << endl;
	}
	return 0;
}


/*************test7_13******************/

//多米诺骨牌 最简单的递推题目
int MainDominoes(){
	//递推数组
	vector<int> recursiveArray;
	int n = 45 + 2;
	recursiveArray.clear();
	recursiveArray.resize(n);
	recursiveArray[0] = 0;
	recursiveArray[1] = 1;
	recursiveArray[2] = 2;
	for (int i = 3; i < n; ++i){
		recursiveArray[i] = recursiveArray[i - 1] + recursiveArray[i - 2];
	}
	while (1 == scanf("%d", &n)){
		printf("%d\n", recursiveArray[n]);
	}
	return 0;
}

//	高速公路维修问题
int MainTenanceHighway(){
	/*
	4 50
	8 59 60 61

	8 50
	8 59 60 110 111 112 113 114

	4 50
	10 50 61 99

	2 50
	10 61

	4 50
	10 59 60 61
	*/
	int ansCnt;
	int n;
	int minLen, cntDFS, ansCntDFS;
	vector<int> arrayA;
	while (2 == scanf("%d%d", &n, &minLen)){
		arrayA.clear();
		arrayA.resize(n);
		for (int i = 0; i < n; ++i){
			scanf("%d", &arrayA[i]);
		}
		sort(arrayA.begin(), arrayA.end());
		//outputArray(arrayA);
		ansCnt = 0;
		cntDFS = ansCntDFS = 0;
		//简单来说 就是找到所有的最优子段 将每一个子段作为一个单位计数 最后输出总数
		for (int i = 0; i < n;){
			int rightSubI = i, maxCntI = MIN_INT_NUM, cntI = 0;
			for (int j = rightSubI; j < n; ++j){
				if (arrayA[j] - arrayA[i] <= minLen){
					++cntI;
					if (cntI > maxCntI){
						maxCntI = cntI;
						rightSubI = j;
					}
					//cout<<"maxCntI: "<<maxCntI<<endl;
				}
				else{
					cntI = 0;
					break;
				}
			}
			i = rightSubI + 1;
			++ansCnt;
			//cout<<"rightSubI: "<<rightSubI<<endl;
		}
		//vDfsC(0, arrayA);
		//cout<<ansCntDFS<<endl;
		cout << ansCnt << endl;
	}
	return 0;
}

void BeerProblem(int bottleCnt, int capCnt, int &beerCnt){
	bool nextSolve = false;
	if (bottleCnt > 1){
		int cnt = bottleCnt / 2;
		bottleCnt %= 2;
		bottleCnt += cnt;
		beerCnt += cnt;
		capCnt += cnt;
		nextSolve = true;
	}
	if (capCnt > 3){
		int cnt = capCnt / 4;
		capCnt %= 4;
		capCnt += cnt;
		bottleCnt += cnt;
		beerCnt += cnt;
		nextSolve = true;
	}
	if (nextSolve){
		BeerProblem(bottleCnt, capCnt, beerCnt);
	}
}

int MainBeerProblem(){
	vector<int> arrayA;
	int n;
	int bottleCnt, capCnt;
	int beerCnt;
	while (1 == scanf("%d", &n)){
		beerCnt = n / 2;
		bottleCnt = capCnt = beerCnt;
		BeerProblem(bottleCnt, capCnt, beerCnt);
		printf("%d\n", beerCnt);
	}
	return 0;
}

//二叉树遍历问题: 已知先序 后序 推出可能的二叉树个数 (这是截止后完成的 可能有错误)
/*
3
ABDCEFG
DBEGFCA
A
A
AB
BA

*/
void PrePostToBinCnt(string &sPre, string &sPost, int &cnt, bool root = true){
	cout << "root: " << sPre[0] << " pre: " << sPre << " post: " << sPost << endl;
	if (sPre.size() == 1 && sPost.size() == 1){
		cnt += root ? 1 : 2;
	}
	else{
		size_t leftSub = sPost.find(sPre[0]);
		int leftSubLen = leftSub;
		if (leftSubLen != string::npos){
			string subPre, subPost;
			//减去根结点
			int rightSubLen = sPre.size() - leftSubLen - 1;
			cout << "leftSubLen: " << leftSubLen << endl;
			if (leftSubLen > 0){
				subPre = sPre.substr(1, leftSubLen);
				subPost = sPost.substr(0, leftSubLen);
				PrePostToBinCnt(subPre, subPost, cnt, false);
			}
			cout << "rightSubLen: " << rightSubLen << endl;
			if (rightSubLen > 0){
				subPre = sPre.substr(leftSubLen + 1, rightSubLen);
				subPost = sPost.substr(leftSubLen + 1, rightSubLen);
				PrePostToBinCnt(subPre, subPost, cnt, false);
			}
		}
	}
}

int MainPrePostToBinCnt(){
	int group;
	int cnt;
	string sPre, sPost;
	while (1 == scanf("%d", &group)){
		while (group-- > 0){
			cin >> sPre >> sPost;
			cnt = 0;
			PrePostToBinCnt(sPre, sPost, cnt);
			printf("%d\n", cnt);
		}
	}
	return 0;
}



/***************pta6_7_14*******************/

/*
4 3
-1 100 150 70
100 -1 110 -1
150 110 -1 70
70 -1 70 -1

4 5
-1 100 150 70
100 -1 110 -1
150 110 -1 70
70 -1 70 -1

4 3
-1 100 150 90
100 -1 110 -1
150 110 -1 70
90 -1 70 -1
*/

//求第K小边的权值 曲解题意
int MainFindTheWeightOfTheKthEdge(){
	/*
	4 3
	-1 100 150 90
	100 -1 110 -1
	150 110 -1 70
	90 -1 70 -1
	*/
	/*
	寒假快到了，同学们都想着回家过年，路近的同学还好说点，坐个大巴就可以回家
	，但是路远的同学就不一样了，我们班上的同学小明的家就在遥远的东北边陲，他也很想回家
	，如果坐火车，要很多天，而且还要中转很多次，所以他想看看有没有便宜的航班
	，然而航班有热线和冷线之分，不一定远就贵，而且也不一定适合小明，当然小明也可以先坐汽车去另外一个城市坐飞机
	，到达一个城市后也可以转乘别的交通工具回到他家所在的城市。假定给定的航班信息的目的站都可以满足小明的要求
	，因为小明还要考虑返程机票问题，所以来回机票视同一个价格
	，请你帮帮小明写一个程序，找出一些价格比较低的航班机票信息，给小明一些辅助答案，供小明选择参考。
	为了避免因为表达上可能造成的理解偏差，这里给出出题者认为正确的问题模型
	，出题者认为这个问题就是一个带权的无向图问题，已知的是权值的邻接矩阵
	，-1表示两个节点之间没有边，求第K小边的权值。
	*/
	int n, k;
	vector<vector<int>> array2D;
	vector<int> arrayA;
	while (2 == scanf("%d%d", &n, &k)){
		array2D.clear();
		array2D.resize(n);
		arrayA.clear();
		//arrayA.resize(n);
		int minE = MAX_INT_NUM;
		for (int r = 0; r < n; ++r){
			array2D[r].clear();
			array2D[r].resize(n);
			for (int c = 0; c < n; ++c){
				scanf("%d", &array2D[r][c]);
				if (c > r && array2D[r][c] != -1){
					arrayA.push_back(array2D[r][c]);
				}
			}
		}
		sort(arrayA.begin(), arrayA.end());
		//outputArray(arrayA);
		//求第K小边的权值 不是排名 或 等级 (rank)
		if (k <= arrayA.size()){
			printf("%d\n", arrayA[k - 1]);
		}
		else{
			puts("Error !");
		}
	}
	return 0;
}

/*
1024 36
1000 37
*/
//十进制转换成其他任意进制
int MainTenToBase(){
	//__int64 n, k;
	long long n, k;
	while (2 == scanf("%lld%lld", &n, &k)){
		string ans = "";
		//目标进制的基值超过36
		if (k > 36){
			puts("Error !");
		}else{
			do{
				ans += toUppercaseAscllChar(n % k);
				n /= k;
			} while (n != 0);
			reverse(ans.begin(), ans.end());
			cout << ans << endl;
		}
	}
	return 0;
}

//股票问题 曲解题意
int MainStockIssue(){
	/*
	6
	2000 1900 2100 2300 2700 2500
	6
	2000 1900 1900 2300 2700 2500
	3
	2000 1900 1900
	5
	3000 2000 2000 2300 2400
	5
	3000 2000 2000 2300 2700
	*/
	/*
	有个姓胡的师傅，工作之余喜欢炒股，而且还小有暂获。胡师傅通过长期的观察，发现了股票有点小规律
	，他把自己发现的规律叫做“波浪理论”，通俗地说，股票是一波一波好像大海里的波浪一样的，有涨有跌
	，跌跌涨涨的，每一波的涨幅和跌幅也有不同，在这里我们规定一个波段指的是连续涨或者连续跌
	，如果连续两天出现大盘指数一样时，可以算涨（在涨的波段内算涨），也可以算跌（在跌的波段内算跌）
	，胡师傅认为刻画股市是不是“牛”，要看连续涨的天数和波段涨幅的乘积的大小，他把这个大小的值戏称为股市的风向标
	，请你帮胡师傅写个程序，算一算在所有的一波一波的波段中风向标最大值是多少。
	*/
	int n;
	vector<int> arrayA;
	while (1 == scanf("%d", &n)){
		arrayA.clear();
		arrayA.resize(n);
		for (int r = 0; r < n; ++r){
			scanf("%d", &arrayA[r]);
		}
		double maxIndexNumSum = 0, maxIndexPlushNumSum = 0, maxIndexSubtractNumSum = 0, indexNumSum;
		//要看[连续涨的天数]和波段涨幅的乘积的大小: 不考虑跌
		for (int i = 0; i < n; ++i){
			int plush = 0;
			indexNumSum = 0;
			for (int j = i + 1; j < n; ++j){
				//indexNumSum += abs(arrayA[j] - arrayA[j-1]);
				indexNumSum += arrayA[j] - arrayA[j - 1];
				if (arrayA[j] > arrayA[j - 1]){
					if (-1 == plush){
						//cout<<"plush break;";
						break;
					}
					else{
						maxIndexPlushNumSum = max(maxIndexPlushNumSum, (j - i)*indexNumSum);
					}
					plush = 1;
				}
				if (arrayA[j] < arrayA[j - 1]){
					if (1 == plush){
						//cout<<"subtract break;";
						break;
					}
					else{
						//maxIndexSubtractNumSum = max(maxIndexSubtractNumSum, (j-i)*indexNumSum);
					}
					plush = -1;
				}
				if (arrayA[j] == arrayA[j - 1]){
					//cout<<"equal";
					maxIndexPlushNumSum = max(maxIndexPlushNumSum, (j - i)*indexNumSum);
					//maxIndexSubtractNumSum = max(maxIndexSubtractNumSum, (j-i)*indexNumSum);
				}
				//maxIndexNumSum = max(maxIndexPlushNumSum, maxIndexSubtractNumSum);
				maxIndexNumSum = max(maxIndexPlushNumSum, maxIndexNumSum);
				//cout<<" plush: "<<plush<<" i: "<<i<<" j: "<<j<<" maxIndexNumSum: "<<maxIndexNumSum<<" indexNumSum: "<<((j-i)*indexNumSum)<<endl;
			}
		}
		printf("%.f\n", maxIndexNumSum);
		//cout<<maxIndexNumSum<<endl;
	}
	return 0;
}

/*************test7_14*****************/

int OutPutLinkedGraphMain(){
	int n, line, group = 0;
	vector<vector<int>> graph;
	while (2 == scanf("%d%d", &n, &line)){
		graph.clear();
		graph.resize(n);
		int p1, p2;
		for (int i = 0; i < line; ++i){
			scanf("%d%d", &p1, &p2);
			--p1;
			--p2;
			graph[p1].push_back(p2);
			graph[p2].push_back(p1);
		}
		//cout<<"----"<<endl;
		for (int i = 0; i < n; ++i){
			printf("Node %d:", i + 1);
			int pCnt = 0;
			sort(graph[i].begin(), graph[i].end());
			for (auto it = graph[i].begin(); it != graph[i].end(); ++it){
				printf(pCnt++ == 0 ? "%d" : "-->%d", *it + 1);
			}
			puts(pCnt == 0 ? "No AdjNode" : "");
		}
		printf("============\n");
	}
	return 0;
}

//食堂排队买菜问题
int MainCanteensLineUpToBuyFood(){
	/*
	4
	5 9 6 7

	PS:
	5*4 20
	6*3 18
	7*2 14
	9*1 9
	*/
	int n;
	vector<int> arrayA;
	while (1 == scanf("%d", &n)){
		arrayA.clear();
		arrayA.resize(n);
		for (int i = 0; i < n; ++i){
			scanf("%d", &arrayA[i]);
		}
		sort(arrayA.begin(), arrayA.end());
		int sumTime = 0;
		for (int i = 0; i < n; ++i){
			sumTime += (n - i)*arrayA[i];
		}
		printf("%d\n", sumTime);
	}
	return 0;
}

void dfsTofindN_BitNumber(string buffer, int n, int p, int depth = 0){
	static vector<bool> visited(10, false);
	if (depth == n){
		cout << buffer << endl;
	}
	else{
		for (int i = 1; i <= p; ++i){
			dfsTofindN_BitNumber(buffer + toUppercaseAscllChar(i), n, p, depth + 1);
		}
	}
}

int MainBitNumber(){
	int n, p;
	while (2 == scanf("%d%d", &n, &p)){
		string str;
		str.clear();
		for (int i = 1; i <= p; ++i){
			str += toUppercaseAscllChar(i);
		}
		/*
		do{
				cout << str << endl;
			} while (next_permutation(str.begin(), str.end()));//升序的序列str
		*/
		dfsTofindN_BitNumber(string(), n, p);
	}
	return 0;
}

int MainGraphInPutAndOutPut(){
	/*
	大家都知道无向图的定义时这样的：
	【定义】一个无向图是一个二元组<E,W>，其中：1、E是非空集合，称为顶点集。
	2、V是E中的元素构成的无序二元组的集合，称为边集。为了使问题简单化，在以下的讨论时
	，我们还规定：1、不考虑定点到其自身的边。2、不允许一条边重复出现，即只讨论简单图。
	在计算机内部，一个带权的无环的简单无向图的表示和存储是一个很有意思的问题
	，教科书上已经有过详细的讨论，今天我们来讨论一下另外一个问题，那就是一个带权的无环的简单无向图的输入和输出问题。
	现在有n个点，分别用1到n表示，一条带权的边分别用起点、终点和权值表示，由于在输入时，常常会输错
	，但一旦输错我们不方便去修改，因此我们规定，当输入的边前面已经有过输入时一律以最后的为准；输入的权值如果是-1
	，那么表示这条边不存在；请你统计一下一共有多少条有效的边以及最大权值和最小权值的边。
	5 18
	1 3 10
	1 4 20
	1 5 15
	2 1 -1
	2 3 13
	2 4 16
	2 5 -1
	3 4 12
	3 1 14
	3 5 -1
	4 1 -1
	4 2 20
	4 3 -1
	4 5 21
	5 1 14
	5 2 -1
	5 3 -1
	5 4 15
	3 2
	1 3 -1
	4 6 10
	3 4
	1 3 -1
	1 2 10
	2 3 -1
	1 2 -1
	*/
	int n, line;
	vector<map<int, int>> graph;
	while (2 == scanf("%d%d", &n, &line)){
		graph.clear();
		graph.resize(n);
		int leftE = -1, rightE = -1, weightE = -1;
		for (int i = 0; i < line; ++i){
			scanf("%d%d%d", &leftE, &rightE, &weightE);
			--leftE;
			--rightE;
			if (0 <= leftE && leftE < n && 0 <= rightE && rightE < n && leftE != rightE){
				graph[leftE][rightE] = weightE;
				graph[rightE][leftE] = weightE;
			}
		}
		int minWeight = MAX_INT_NUM, maxWeight = -1, edgeCnt = 0;
		for (int v = 0; v < n; ++v){
			for (auto it = graph[v].begin(); it != graph[v].end(); ++it){
				//it->first, it->second.rightE, it->second.weightE
				//printf(" -- L:%d R:%d W:%d\n", v, it->first, it->second);
				if (it->second != -1){
					++edgeCnt;
					maxWeight = max(maxWeight, it->second);
					minWeight = min(minWeight, it->second);
				}
			}
		}
		if (edgeCnt == 0){
			puts("Error !");
		}
		else{
			cout << "Edges=" << edgeCnt / 2 << endl;
			cout << "MaxEdge=" << maxWeight << endl;
			cout << "MinEdge=" << minWeight << endl;
		}
	}
	return 0;
}

/* C语言的二进制输出问题: 这个程序是错误的*/
char strB[10000 + 10];
string toBinary(int num){
	string binary;
	if (255 < num || num < 0){
		//do nothing
	}
	else{
		do{
			binary += toUppercaseAscllChar(num % 2);
			num /= 2;
		} while (num != 0);
		for (size_t i = binary.size(); i < 8; ++i){
			binary += '0';
		}
		reverse(binary.begin(), binary.end());
	}
	return binary;
}
bool isContinuous5Star(char *left, char *right){
	size_t len = right - left;
	//cout<<endl<<string(left, 5)<<endl;
	if (len < 5){
		return false;
	}
	for (int i = 0; i < 5; ++i){
		if (left[i] == '*'){
			//do nothing
		}
		else{
			return false;
		}
	}
	return true;
}
//返回最后5个*的起始位置若没有返回null
char *last5StarLeft(size_t len){
	char *last5Star = nullptr;
	for (auto right = strB + len, left = right - 5; left >= strB; --right){
		left = right - 5;
		if (isContinuous5Star(left, right)){
			last5Star = left;
			if (last5Star - 1 >= strB && *(last5Star - 1) != '*'){
				break;
			}
			else{
				//continue;
				break;
			}
		}
		else{
			// do nothing
		}
	}
	//if(last5Star != nullptr)cout<<"last: "<<string(last5Star, 5+1)<<endl;
	return last5Star;
}
int MainCLanguageBinaryOutputProblem(){
	/*
	Description
	C语言中没有直接将ACCII码以二进制格式输出的功能，需要自己设计，现在就请你来完成这个函数。

	Input
	输入由若干段字符组成，每一段由5个“*”作为结尾；每一段中包含符合C语言规定的字符；若干段加起来总长度不超过10000个字符。为了处理方便，我们规定一旦遇到5个连续的“*”，那么就是段落分界，这5个“*”不是段落内容。输入保证每个段落有5个连续的“*”作为结尾。如果最后一次连续5个“*”之后还有其他字符，就视为无效字符，不需要处理。

	Output
	将输入中的所有字符（包括回车换行符）都转换成二进制并输出。如果没有需要转换的字符，那么输出一行“Blank !”。

	Sample Input
	123
	**********
	Sample Output
	00110001001100100011001100001010
	Blank !
	Hint
	如果你使用java语言，因为我们今天考试的内容就是请你设计一个转换函数，所以你不能使用系统函数来解决，要求你无论用什么语言提交，都必须要自己写一个将ASCII码转换成二进制的函数。
	*/
	/*
	00110001
	00110010
	00110011
	00001010

	1*****2*****3
	123
	**********
	*/
	while (gets(strB)){
		bool valid = true;
		size_t len = strlen(strB);
		int pCnt = 0;
		memset(strB + len, 0, sizeof(strB)-(len*sizeof(char)));
		char *last5StarLeftPtr = last5StarLeft(len);
		//printf("%c: %d\n", strB[0], strB[0]);
		for (size_t i = 0; i < len; ++i){
			//一旦遇到5个连续的“*”，那么就是段落分界，这5个“*”不是段落内容
			if (isContinuous5Star(strB + i, strB + i + 5)){
				i += 5;
				--i;//抵消自减
				valid = false;
				continue;
			}
			//输入保证每个段落有5个连续的“*”作为结尾。如果最后一次连续5个“*”之后还有其他字符，就视为无效字符，不需要处理。
			if (last5StarLeftPtr != nullptr && strB + i >= last5StarLeftPtr){
				break;
			}
			else{
				//puts(strB+i);
				++pCnt;
				//cout<<" "<<strB[i]<<":"<<toBinary(strB[i]);
				cout << toBinary(strB[i]);
			}
		}

		if (valid){
			cout << toBinary('\n') << endl;
		}
		else{
			puts(pCnt == 0 ? "Blank !" : "");
		}
		//printf("valid: %d\n", valid);
		//cout<<toBinary(0)<<toBinary(255+1)<<toBinary(-1)<<toBinary(255)<<endl;
	}
	return 0;
}

/**************Test****************/
void printNChar(int width, char c){
	for (int i = 0; i < width; ++i){
		printf("%c", c);
	}
}
//	输出图案-从上到下的箭头
int MainArrow(){
	int n;
	while (1 == scanf("%d", &n)){
		int height = n * 2;
		int width = height - 1;
		int pWidth = height + width;
		for (int i = 0; i < height; ++i){
			printNChar(n, ' ');
			printNChar(width, '*');
			puts("");
		}
		int pWidthTemp = pWidth;
		int blank = 0;
		while (0 < pWidthTemp){
			printNChar(blank++, ' ');
			printNChar(pWidthTemp, '*');
			puts("");
			pWidthTemp -= 2;
		}
	}
	return 0;
}

/*
6 2 5
0 1 1 0 1 0
1 0 1 1 0 1
1 1 0 0 1 1
0 1 0 0 1 0
1 0 1 1 0 1
0 1 1 0 1 0

3 1 3
0 1 0
1 0 1
0 1 0
*/
//mdzz: 没有剪枝(TL) + 忘记传graph进来(TL) + [1-n看错](WA*1)
size_t dfsC(vector<vector<int>> &graph, int n, int fromVeSub, int targetVeSub, int depth = 0){
	static vector<bool> visited(1000 + 5, false);
	int cnt = 0;
	if (depth > 2){
		return 0;
	}
	if (fromVeSub == targetVeSub){
		//统计出这两个节点之间只经过一个节点的路径条数。
		if (depth == 2){
			++cnt;
			//cout<<"find one"<<endl;
		}
		else{
			//cout<<"-----"<<endl;
		}
	}
	else{
		for (int i = 0; i < n; ++i){
			if (!visited[i] && 1 == graph[fromVeSub][i]){
				visited[i] = true;
				cnt += dfsC(graph, n, i, targetVeSub, depth + 1);
				visited[i] = false;
			}
		}
	}
	return cnt;
}
/*
C. 最简单的图论问题
Description
图在计算机中的表示有两种方法，分别是邻接矩阵和邻接表方法。
今天你的任务是：给你一个由0、1组成的邻接矩阵表示的无向图以及指定的起点和终点
，请你统计出这两个节点之间只经过一个节点的路径条数。

Input
本题有多组测试数据，对于每一组测试数据，输入有两部分，第一部分只有一行
，用空格隔开的三个正整数n、f、t（1<=f，t<=n<=1000），其中n表示图中节点的个数
，节点编号位1-n，f表示起始节点编号，t表示终止节点编号；第二部分是由0、1组成的邻接矩阵
，共有n行，每行n个由空格隔开的整数0或1，0表示没有边，1表示有边。

Output
对于每一组测试数据，输出只有一行，即从节点f到节点t且只经过一个节点的路径条数。

Sample Input
6 2 5
0 1 1 0 1 0
1 0 1 1 0 1
1 1 0 0 1 1
0 1 0 0 1 0
1 0 1 1 0 1
0 1 1 0 1 0
Sample Output
4
*/
int main_C(){
	int n, fromVeSub, targetVeSub;
	//vector<map<int, int>> graph;
	vector<vector<int>> graph;
	while (3 == scanf("%d%d%d", &n, &fromVeSub, &targetVeSub)){
		--fromVeSub;
		--targetVeSub;
		graph.clear();
		graph.resize(n);
		int weightE = -1;
		for (int leftE = 0; leftE < n; ++leftE){
			graph[leftE].clear();
			graph[leftE].resize(n);
			for (int rightE = 0; rightE < n; ++rightE){
				scanf("%d", &weightE);
				graph[leftE][rightE] = weightE;
			}
		}
		//output2Array(graph);
		//两个节点之间只经过一个节点的路径条数。
		cout << dfsC(graph, n, fromVeSub, targetVeSub, 0) << endl;
	}
	return 0;
}

#define MAX_ND 100000000+5
vector<int> arrayDTable(100, 1);
vector<char> arrayDLaw(MAX_ND, 0);
//找规律入门 见证题
/*
D. 开关门问题
Description
梅山中心学校的前身是我的母校，2013年暑假，我去母校做了一期关于C++编程的公益课，结束前进行了结业考试，试题一共有七题，其中最后两题的题目是：

……

新建的梅山中心学校有很多教室，如果把这些教室从1到n编号，现在有3个同学拿着钥匙去开门，第一个同学把所有的门都打开了；第二个同学把偶数编号的门关上了；第三个同学把编号是3以及3的倍数的门开关（即原来开着的给关上，原来关上的给打开）；现在请你编程序在输入n(1<=n<=1000)的情况下，把开着门的教室的编号输出来。
（附加题）在第六题中，如果不止是3个同学去开门，假设有k个同学，请你在输入n和k(1<=k<=n<=1000)的情况下，把开着门的教室的编号输出来。
……

由于第n个人后面没有再可以开关的门了，所以当k>n时，教室的门的开关状态不会再发生任何改变，今天你的任务是解决k=n时的情形，为了展现算法的魅力，你要考虑的n是非常大的。

Input
本题有多组测试数据，对于每一组测试数据，输入只有一行，用空格隔开的三个正整数n、f和t，其中1<=n<=100000000，1<=f<=t<=n，t-f<=5000。

Output
对于每一组测试数据，输出只有一行，即从编号f的门开始到编号t的门为止，开着的门有多少扇（包括编号为f和编号为t的两扇门）？

Sample Input
5 2 4
Sample Output
1
Hint
第一个同学，把所有门都打开了；第二个同学把2号和4号的门关上了；第三个同学把3号门关上了；第四个同学把4号门又打开了；第五个同学把5号门关上了；最后2号门、3号门和4号门中只有4号门开着。
*/
void calcSwitchDoorProblem(){
	for (int k = 2; k <= 100; ++k){
		for (int i = k - 1; i < 100; i += k){
			arrayDTable[i] = arrayDTable[i] == 1 ? 0 : 1;
		}
	}
	long long pro = 0;
	for (long long i = 0; i < MAX_ND; i += pro + 1){
		pro += 2;
		arrayDLaw[i] = 1;
	}
}

void DebugPrintSwitchDoorProblem(){
	outPutIterable(arrayDTable.begin(), arrayDTable.end(), ' ', 2, 10);
	outPutIterable(arrayDTable.begin(), arrayDTable.end(), ' ', 2, [&](vector<int>::iterator left, vector<int>::iterator right){
		return 1 == *left;
	});
	puts("---DebugPrintSwitchDoorProblem end");
	//outPutIterable(arrayDLaw.begin(), arrayDLaw.end());
}

int MainSwitchDoorProblem(){
	int n, fromVeSub, targetVeSub;
	calcSwitchDoorProblem();
	//cout<<arrayDLaw.size()<<"last arrayDLaw: "<<arrayDLaw[MAX_ND-1000]<<endl;
	while (3 == scanf("%d%d%d", &n, &fromVeSub, &targetVeSub)){
		--fromVeSub;
		--targetVeSub;
		int cnt = 0;
		for (; fromVeSub <= targetVeSub; ++fromVeSub){
			cnt += arrayDLaw[fromVeSub] == 1 ? 1 : 0;
		}
		printf("%d\n", cnt);
	}
	return 0;
}

bool isEqualInDis(char a, char b, int dis){
	return abs(a - b) <= dis;
}
/*
E. 字符统计问题二
Description
话说实验室有个胖子叫小明，还有个瘦的叫小兵，小兵他已经来了好几天啦，老师也一直叫他练打英文字母，小兵就没有小明听话，他老是偷懒，老师想让他动动脑经，于是就给他出了个问题来惩罚他偷懒。现在我们把两个英文字母的ASCII码的差的绝对值称为这两个字符的“距离”，如果给你一个字符串和上面定义的“距离”的值，问小兵在这个串里最长的连续的子串有多长？当然这个子串的任意两个字符的距离一定不超过给定的距离值！亲爱的同学们，如果你们同情瘦个子小兵，那么你就帮他写个程序，计算一下符合要求的子串最长到底是多少？

Input
假定字符串都是由英文小写字母组成的，长度不超过100000。输入有两行，第一行是整数d即指给定的距离值，d>=0；另一行是字符串。

Output
输出也有两行，第一行的格式是：“From=XX,To=XX”，第二行的格式是：“MaxLen=XX”。本问题有多组测试数据。如果有多组符合要求的解，那么输出起始位置最小的解。

Sample Input
2
vdtjajcrnamkzequrcnojumsjnbyzsotkebnrkycbvbmcsqdvjght
15
hpenlvlknkrtpmkmkbxcpbbpxxipxmgxnysryttuowyxppgmndoxzizdilnjdqawieab
Sample Output
From=11,To=12
MaxLen=2
From=32,To=46
MaxLen=15
*/
//	字符统计问题二 超时算法 可能是正确的
int MainCharacterStatisticsQuestionTwo(){
	int n;
	string str;
	while (1 == scanf("%d", &n)){
		cin >> str;

		/*
		cout<<"n: "<<n<<endl;
		for(int i = 0; i < str.size()-1; ++i){
		printf(" %d:%c+%d", i+1, str[i], abs(str[i+1]-str[i]));
		} printf(" %d:%c\n", str.size(), str[str.size()-1]);
		*/

		int fromSub = 1, toSub = 1, len = 1, maxLen = MIN_INT_NUM;
		for (size_t i = 0; i < str.size(); ++i){
			len = 1;
			int minCharA = 255, maxCharA = -1;
			for (size_t j = i + 1; j < str.size(); ++j){
				//[任意]  两个字符的距离一定不超过给定的距离值
				minCharA = min(minCharA, (int)str[j]);
				maxCharA = max(maxCharA, (int)str[j]);
				//cout<<"minCharA: "<<minCharA<<" maxCharA: "<<maxCharA<<endl;
				if (maxCharA - minCharA <= n && isEqualInDis(str[j], str[j - 1], n)){
					++len;
					if (len > maxLen){
						maxLen = len;
						fromSub = i, toSub = j;
					}
				}
				else{
					break;
				}
			}
		}
		printf("From=%d,To=%d\n", fromSub + 1, toSub + 1);
		printf("MaxLen=%d\n", maxLen);
	}
	return 0;
}


using ticket = int;
class VirtualTable {  // example class  
public:
	virtual void VirtualFunction01(ticket);
};

void VirtualTable::VirtualFunction01(ticket)  {
	printf("VirtualFunction01 called");
}

typedef void(__thiscall* VirtualFunction01_t)(ticket* thisptr);
VirtualFunction01_t g_org_VirtualFunction01;

//our detour function  
void __fastcall hk_VirtualFunction01(ticket* thisptr, int edx)  {
	printf("Custom function called");
	//call the original function  
	g_org_VirtualFunction01(thisptr);
}
int _tmain(int argc, TCHAR* argv[])  {

	DWORD oldProtection;
	//https://zh.wikipedia.org/wiki/%E9%92%A9%E5%AD%90%E7%BC%96%E7%A8%8B
	VirtualTable* myTable = new VirtualTable();
	void** base = *(void***)myTable;
	//处理被拦截的函数调用、事件、消息的代码，被称为钩子（hook）。
	//C++使用虚函数，因此可在运行时直接修改虚函数表的内容来挂钩。
	VirtualProtect(&base[0], 4, PAGE_EXECUTE_READWRITE, &oldProtection);
	//save the original function  
	g_org_VirtualFunction01 = (VirtualFunction01_t)base[0];
	//overwrite  
	base[0] = &hk_VirtualFunction01;
	VirtualProtect(&base[0], 4, oldProtection, 0);

	//call the virtual function (now hooked) from our class instance  
	myTable->VirtualFunction01(10);

	return 0;
}

int main___45(){
	//虚函数拦截
	//_tmain(0, nullptr);
	srand(time(0));
	//取消cin与stdin的同步(是cin与scanf效率接近; 但此时两者不能混用)
	//ios::sync_with_stdio(false);
	puts("PTA 算法集训遗产(Algorithm training legacy) url: http://139.196.145.92/index.php");
	freopen("input", "r", stdin);
	//MainSwitchDoorProblem();
	//DebugPrintSwitchDoorProblem();
	//MainArrow();
	MainJudgeConnectivity();
	vector<vector<int>> d2(10, vector<int>(10, 1));
	for (size_t r = 0; r < d2.size(); ++r){
		iterate(d2[r].begin(), d2[r].end(), [&](vector<int>::iterator left, vector<int>::iterator right){
			*left = rand() % 100;
		});
	}
	toLowerAlph(10);
	outPut2DArrayList(d2);
	cout << minValueStatistics(d2[0].begin(), d2[0].end(), MAX_INT_NUM) << endl;
	cout << maxValueStatistics(d2[0].begin(), d2[0].end(), MIN_INT_NUM) << endl;
	cout << sumValueStatistics(d2[0].begin(), d2[0].end(), 0) << endl;
	cout << avlValueStatistics(d2[0].begin(), d2[0].end(), 0) << endl;
	return 0;
}

/*
二分枚举+贪心
http://acm.hdu.edu.cn/showproblem.php?pid=4004
6 1 2
2

25 3 3
11
2
18
*/
//贪心
bool canJump(int s[], int n, int m, int jumpPower) {
	int step = 0;
	int from = 0;
	int to = 1;
	int out = true;
	//到达终点时结束
	while (to < n) {
		out = true;
		while (to < n && s[to] - s[from] <= jumpPower) {
			//贪心，通过尽可能多的石头
			++to;
			out = false;
		}
		from = to - 1;
		++step;
		//说明jumpPower小了，有的地方跳不过去  || 步数过多
		if (out || step > m)
			return false;
	}
	return true;
}
int solveFrogJump() {
	// it is to big when it to be 500002 size
	int st[5/*00002*/];
	//河宽L, 石头数量为N, 最多跳M次, 求最小所需跳跃力
	int L, N, M;
	while (cin >> L >> N >> M) {
		N += 2;
		//st[n]表示第n块石头距离起始点的距离
		st[0] = 0;
		for (int i = 1; i < N - 1; scanf("%d", &st[i++]));
		sort(st + 1, st + N - 1);
		int left = st[1];
		int right = st[N - 1] = L;
		st[0] = 0;
		//二分枚举(取一个使得canJump成立的最小值 jumpPower 初始时是第一块石头的距离)
		while (left < right) {
			int mid = (left + right) >> 1;
			//若可以在小于m步的前提下跳过去 那么尝试小一些的jumpPower,但要保留上一次的jumpPower值：mid不减1
			if (canJump(st, N, M, mid)) {
				right = mid;
			}
			else {
				//在无法跳过的前提下，mid值无需保留
				left = mid + 1;
			}
		}
		cout << left << endl;
	}
	return 0;
}

