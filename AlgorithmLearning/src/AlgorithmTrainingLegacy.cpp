#include "ExtendSpace.h"
using namespace std;
using namespace StandardExtend;

template<class ElementType>
//���鼯ģ����
class DisjointSet{
public:
	//using ElementType = int;
	//�ü��ϵ�root�±��ʾ����
	using SetRoot = int;
	using SetBaseType = vector<ElementType>;
	//using SetBaseType = ElementType[MAX_N];
	//��class����vector��http://139.196.145.92�ϻ�Runtime Error
	//vector<ElementType> debugTemp1;

	DisjointSet(){}
	DisjointSet(size_t n){
		//�µ�Ԫ�س�ʼֵҲ������-1 ÿ��Ԫ�ض�����һ�������ļ���
		disjointSetBase.assign(n, -1);
		//memset(disjointSetBase, -1, n*sizeof(ElementType));
	}

	void unionSet(SetRoot root1, SetRoot root2){
		if (root1 == root2){
			return;
		}
		//��֤С���ϲ���󼯺�
		if (disjointSetBase[root2] < disjointSetBase[root1]){
			//1����2
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
		//����Ĭ�ϳ�ʼֵΪ1
		return disjointSetBase[x] < 0 ? x//�ҵ����ϵĸ�
			: disjointSetBase[x] = findRoot(disjointSetBase[x]);//����Ѱ�Ҳ�����·��ѹ��
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

	//���ؼ�����
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

//������Ƽӷ�(�����)
void basePlus(string &lhs, string &rhs, int base, string &sum){
	int maxSize = max(lhs.size(), rhs.size());
	reverse(lhs.begin(), lhs.end());
	reverse(rhs.begin(), rhs.end());
	//cout<<lhs<<endl<<rhs<<endl;
	lhs.resize(maxSize, '0');
	rhs.resize(maxSize, '0');
	sum.resize(maxSize);
	//cout<<lhs<<endl<<rhs<<endl;
	//��λ
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

//��׼�ļ���������� ����ֵ������ʼ����ͨ�Ķ������;
//PS: ����������������: depth; ͼ���������������������: pastId
int dfsStandard(int pastId, vector<bool> &visited){
	int cnt = 1;
	visited[pastId] = true;
	for (auto it = nGraph[pastId].begin(); it != nGraph[pastId].end(); ++it){
		if (!visited[it->targetID]){
			cnt += dfsStandard(it->targetID, visited);
		}
	}
	//���������򷵻ص��Ǳ�׼dfs���ܵ������� �����ݵĻ���ʵ�ϲ�û�б���ÿһ����
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
		//����v�����б�
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
//�ж�ͼ����ͨ��
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
		//���е�ͼ��dfs���ص���·������ �����Ҫ�����ͨ�ԵĻ�����bfs ���� ���鼯 ���ǻ�ȡvisited���ж�
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
				//��һ�еĵ�c�к͵�c+1�����ʱ ѡȡ���С����һ��: c
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
			//��߳ɼ��������ڶ�ʮ��ѧ���ĳɼ��ֲ�
			printf("%.0f\n", maxScore - arrayA[19]);
			//��߷���ƽ���ֵķֲ�
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
������ͨ
http://139.196.145.92/contest_show.php?cid=350#problem/C
�ѵ�һ��
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
		//ѡȡ��i��վ����ΪĿ�ĵ�
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

//����ŵ���� ��򵥵ĵ�����Ŀ
int MainDominoes(){
	//��������
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

//	���ٹ�·ά������
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
		//����˵ �����ҵ����е������Ӷ� ��ÿһ���Ӷ���Ϊһ����λ���� ����������
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

//��������������: ��֪���� ���� �Ƴ����ܵĶ��������� (���ǽ�ֹ����ɵ� �����д���)
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
			//��ȥ�����
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

//���KС�ߵ�Ȩֵ ��������
int MainFindTheWeightOfTheKthEdge(){
	/*
	4 3
	-1 100 150 90
	100 -1 110 -1
	150 110 -1 70
	90 -1 70 -1
	*/
	/*
	���ٿ쵽�ˣ�ͬѧ�Ƕ����Żؼҹ��꣬·����ͬѧ����˵�㣬������;Ϳ��Իؼ�
	������·Զ��ͬѧ�Ͳ�һ���ˣ����ǰ��ϵ�ͬѧС���ļҾ���ңԶ�Ķ��������Ҳ����ؼ�
	��������𳵣�Ҫ�ܶ��죬���һ�Ҫ��ת�ܶ�Σ��������뿴����û�б��˵ĺ���
	��Ȼ�����������ߺ�����֮�֣���һ��Զ�͹󣬶���Ҳ��һ���ʺ�С������ȻС��Ҳ������������ȥ����һ���������ɻ�
	������һ�����к�Ҳ����ת�˱�Ľ�ͨ���߻ص��������ڵĳ��С��ٶ������ĺ�����Ϣ��Ŀ��վ����������С����Ҫ��
	����ΪС����Ҫ���Ƿ��̻�Ʊ���⣬�������ػ�Ʊ��ͬһ���۸�
	��������С��дһ�������ҳ�һЩ�۸�Ƚϵ͵ĺ����Ʊ��Ϣ����С��һЩ�����𰸣���С��ѡ��ο���
	Ϊ�˱�����Ϊ����Ͽ�����ɵ����ƫ����������������Ϊ��ȷ������ģ��
	����������Ϊ����������һ����Ȩ������ͼ���⣬��֪����Ȩֵ���ڽӾ���
	��-1��ʾ�����ڵ�֮��û�бߣ����KС�ߵ�Ȩֵ��
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
		//���KС�ߵ�Ȩֵ �������� �� �ȼ� (rank)
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
//ʮ����ת���������������
int MainTenToBase(){
	//__int64 n, k;
	long long n, k;
	while (2 == scanf("%lld%lld", &n, &k)){
		string ans = "";
		//Ŀ����ƵĻ�ֵ����36
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

//��Ʊ���� ��������
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
	�и��պ���ʦ��������֮��ϲ�����ɣ����һ�С���ݻ񡣺�ʦ��ͨ�����ڵĹ۲죬�����˹�Ʊ�е�С����
	�������Լ����ֵĹ��ɽ������������ۡ���ͨ�׵�˵����Ʊ��һ��һ���������Ĳ���һ���ģ������е�
	���������ǵģ�ÿһ�����Ƿ��͵���Ҳ�в�ͬ�����������ǹ涨һ������ָ���������ǻ���������
	���������������ִ���ָ��һ��ʱ���������ǣ����ǵĲ��������ǣ���Ҳ����������ڵ��Ĳ����������
	����ʦ����Ϊ�̻������ǲ��ǡ�ţ����Ҫ�������ǵ������Ͳ����Ƿ��ĳ˻��Ĵ�С�����������С��ֵϷ��Ϊ���еķ����
	��������ʦ��д��������һ�������е�һ��һ���Ĳ����з�������ֵ�Ƕ��١�
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
		//Ҫ��[�����ǵ�����]�Ͳ����Ƿ��ĳ˻��Ĵ�С: �����ǵ�
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

//ʳ���Ŷ��������
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
			} while (next_permutation(str.begin(), str.end()));//���������str
		*/
		dfsTofindN_BitNumber(string(), n, p);
	}
	return 0;
}

int MainGraphInPutAndOutPut(){
	/*
	��Ҷ�֪������ͼ�Ķ���ʱ�����ģ�
	�����塿һ������ͼ��һ����Ԫ��<E,W>�����У�1��E�Ƿǿռ��ϣ���Ϊ���㼯��
	2��V��E�е�Ԫ�ع��ɵ������Ԫ��ļ��ϣ���Ϊ�߼���Ϊ��ʹ����򵥻��������µ�����ʱ
	�����ǻ��涨��1�������Ƕ��㵽������ıߡ�2��������һ�����ظ����֣���ֻ���ۼ�ͼ��
	�ڼ�����ڲ���һ����Ȩ���޻��ļ�����ͼ�ı�ʾ�ʹ洢��һ��������˼������
	���̿������Ѿ��й���ϸ�����ۣ���������������һ������һ�����⣬�Ǿ���һ����Ȩ���޻��ļ�����ͼ�������������⡣
	������n���㣬�ֱ���1��n��ʾ��һ����Ȩ�ı߷ֱ�����㡢�յ��Ȩֵ��ʾ������������ʱ�����������
	����һ��������ǲ�����ȥ�޸ģ�������ǹ涨��������ı�ǰ���Ѿ��й�����ʱһ��������Ϊ׼�������Ȩֵ�����-1
	����ô��ʾ�����߲����ڣ�����ͳ��һ��һ���ж�������Ч�ı��Լ����Ȩֵ����СȨֵ�ıߡ�
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

/* C���ԵĶ������������: ��������Ǵ����*/
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
//�������5��*����ʼλ����û�з���null
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
	C������û��ֱ�ӽ�ACCII���Զ����Ƹ�ʽ����Ĺ��ܣ���Ҫ�Լ���ƣ����ھ�������������������

	Input
	���������ɶ��ַ���ɣ�ÿһ����5����*����Ϊ��β��ÿһ���а�������C���Թ涨���ַ������ɶμ������ܳ��Ȳ�����10000���ַ���Ϊ�˴����㣬���ǹ涨һ������5�������ġ�*������ô���Ƕ���ֽ磬��5����*�����Ƕ������ݡ����뱣֤ÿ��������5�������ġ�*����Ϊ��β��������һ������5����*��֮���������ַ�������Ϊ��Ч�ַ�������Ҫ����

	Output
	�������е������ַ��������س����з�����ת���ɶ����Ʋ���������û����Ҫת�����ַ�����ô���һ�С�Blank !����

	Sample Input
	123
	**********
	Sample Output
	00110001001100100011001100001010
	Blank !
	Hint
	�����ʹ��java���ԣ���Ϊ���ǽ��쿼�Ե����ݾ����������һ��ת�������������㲻��ʹ��ϵͳ�����������Ҫ����������ʲô�����ύ��������Ҫ�Լ�дһ����ASCII��ת���ɶ����Ƶĺ�����
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
			//һ������5�������ġ�*������ô���Ƕ���ֽ磬��5����*�����Ƕ�������
			if (isContinuous5Star(strB + i, strB + i + 5)){
				i += 5;
				--i;//�����Լ�
				valid = false;
				continue;
			}
			//���뱣֤ÿ��������5�������ġ�*����Ϊ��β��������һ������5����*��֮���������ַ�������Ϊ��Ч�ַ�������Ҫ����
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
//	���ͼ��-���ϵ��µļ�ͷ
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
//mdzz: û�м�֦(TL) + ���Ǵ�graph����(TL) + [1-n����](WA*1)
size_t dfsC(vector<vector<int>> &graph, int n, int fromVeSub, int targetVeSub, int depth = 0){
	static vector<bool> visited(1000 + 5, false);
	int cnt = 0;
	if (depth > 2){
		return 0;
	}
	if (fromVeSub == targetVeSub){
		//ͳ�Ƴ��������ڵ�֮��ֻ����һ���ڵ��·��������
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
C. ��򵥵�ͼ������
Description
ͼ�ڼ�����еı�ʾ�����ַ������ֱ����ڽӾ�����ڽӱ�����
������������ǣ�����һ����0��1��ɵ��ڽӾ����ʾ������ͼ�Լ�ָ���������յ�
������ͳ�Ƴ��������ڵ�֮��ֻ����һ���ڵ��·��������

Input
�����ж���������ݣ�����ÿһ��������ݣ������������֣���һ����ֻ��һ��
���ÿո����������������n��f��t��1<=f��t<=n<=1000��������n��ʾͼ�нڵ�ĸ���
���ڵ���λ1-n��f��ʾ��ʼ�ڵ��ţ�t��ʾ��ֹ�ڵ��ţ��ڶ���������0��1��ɵ��ڽӾ���
������n�У�ÿ��n���ɿո����������0��1��0��ʾû�бߣ�1��ʾ�бߡ�

Output
����ÿһ��������ݣ����ֻ��һ�У����ӽڵ�f���ڵ�t��ֻ����һ���ڵ��·��������

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
		//�����ڵ�֮��ֻ����һ���ڵ��·��������
		cout << dfsC(graph, n, fromVeSub, targetVeSub, 0) << endl;
	}
	return 0;
}

#define MAX_ND 100000000+5
vector<int> arrayDTable(100, 1);
vector<char> arrayDLaw(MAX_ND, 0);
//�ҹ������� ��֤��
/*
D. ����������
Description
÷ɽ����ѧУ��ǰ�����ҵ�ĸУ��2013����٣���ȥĸУ����һ�ڹ���C++��̵Ĺ���Σ�����ǰ�����˽�ҵ���ԣ�����һ�������⣬��������������Ŀ�ǣ�

����

�½���÷ɽ����ѧУ�кܶ���ң��������Щ���Ҵ�1��n��ţ�������3��ͬѧ����Կ��ȥ���ţ���һ��ͬѧ�����е��Ŷ����ˣ��ڶ���ͬѧ��ż����ŵ��Ź����ˣ�������ͬѧ�ѱ����3�Լ�3�ı������ſ��أ���ԭ�����ŵĸ����ϣ�ԭ�����ϵĸ��򿪣���������������������n(1<=n<=1000)������£��ѿ����ŵĽ��ҵı���������
�������⣩�ڵ������У������ֹ��3��ͬѧȥ���ţ�������k��ͬѧ������������n��k(1<=k<=n<=1000)������£��ѿ����ŵĽ��ҵı���������
����

���ڵ�n���˺���û���ٿ��Կ��ص����ˣ����Ե�k>nʱ�����ҵ��ŵĿ���״̬�����ٷ����κθı䣬������������ǽ��k=nʱ�����Σ�Ϊ��չ���㷨����������Ҫ���ǵ�n�Ƿǳ���ġ�

Input
�����ж���������ݣ�����ÿһ��������ݣ�����ֻ��һ�У��ÿո����������������n��f��t������1<=n<=100000000��1<=f<=t<=n��t-f<=5000��

Output
����ÿһ��������ݣ����ֻ��һ�У����ӱ��f���ſ�ʼ�����t����Ϊֹ�����ŵ����ж����ȣ��������Ϊf�ͱ��Ϊt�������ţ���

Sample Input
5 2 4
Sample Output
1
Hint
��һ��ͬѧ���������Ŷ����ˣ��ڶ���ͬѧ��2�ź�4�ŵ��Ź����ˣ�������ͬѧ��3���Ź����ˣ����ĸ�ͬѧ��4�����ִ��ˣ������ͬѧ��5���Ź����ˣ����2���š�3���ź�4������ֻ��4���ſ��š�
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
E. �ַ�ͳ�������
Description
��˵ʵ�����и����ӽ�С�������и��ݵĽ�С����С�����Ѿ����˺ü���������ʦҲһֱ��������Ӣ����ĸ��С����û��С��������������͵������ʦ�����������Ծ������Ǿ͸������˸��������ͷ���͵�����������ǰ�����Ӣ����ĸ��ASCII��Ĳ�ľ���ֵ��Ϊ�������ַ��ġ����롱���������һ���ַ��������涨��ġ����롱��ֵ����С���������������������Ӵ��ж೤����Ȼ����Ӵ������������ַ��ľ���һ�������������ľ���ֵ���װ���ͬѧ�ǣ��������ͬ���ݸ���С������ô��Ͱ���д�����򣬼���һ�·���Ҫ����Ӵ�������Ƕ��٣�

Input
�ٶ��ַ���������Ӣ��Сд��ĸ��ɵģ����Ȳ�����100000�����������У���һ��������d��ָ�����ľ���ֵ��d>=0����һ�����ַ�����

Output
���Ҳ�����У���һ�еĸ�ʽ�ǣ���From=XX,To=XX�����ڶ��еĸ�ʽ�ǣ���MaxLen=XX�����������ж���������ݡ�����ж������Ҫ��Ľ⣬��ô�����ʼλ����С�Ľ⡣

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
//	�ַ�ͳ������� ��ʱ�㷨 ��������ȷ��
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
				//[����]  �����ַ��ľ���һ�������������ľ���ֵ
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
	//�������صĺ������á��¼�����Ϣ�Ĵ��룬����Ϊ���ӣ�hook����
	//C++ʹ���麯������˿�������ʱֱ���޸��麯������������ҹ���
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
	//�麯������
	//_tmain(0, nullptr);
	srand(time(0));
	//ȡ��cin��stdin��ͬ��(��cin��scanfЧ�ʽӽ�; ����ʱ���߲��ܻ���)
	//ios::sync_with_stdio(false);
	puts("PTA �㷨��ѵ�Ų�(Algorithm training legacy) url: http://139.196.145.92/index.php");
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
����ö��+̰��
http://acm.hdu.edu.cn/showproblem.php?pid=4004
6 1 2
2

25 3 3
11
2
18
*/
//̰��
bool canJump(int s[], int n, int m, int jumpPower) {
	int step = 0;
	int from = 0;
	int to = 1;
	int out = true;
	//�����յ�ʱ����
	while (to < n) {
		out = true;
		while (to < n && s[to] - s[from] <= jumpPower) {
			//̰�ģ�ͨ�������ܶ��ʯͷ
			++to;
			out = false;
		}
		from = to - 1;
		++step;
		//˵��jumpPowerС�ˣ��еĵط�������ȥ  || ��������
		if (out || step > m)
			return false;
	}
	return true;
}
int solveFrogJump() {
	// it is to big when it to be 500002 size
	int st[5/*00002*/];
	//�ӿ�L, ʯͷ����ΪN, �����M��, ����С������Ծ��
	int L, N, M;
	while (cin >> L >> N >> M) {
		N += 2;
		//st[n]��ʾ��n��ʯͷ������ʼ��ľ���
		st[0] = 0;
		for (int i = 1; i < N - 1; scanf("%d", &st[i++]));
		sort(st + 1, st + N - 1);
		int left = st[1];
		int right = st[N - 1] = L;
		st[0] = 0;
		//����ö��(ȡһ��ʹ��canJump��������Сֵ jumpPower ��ʼʱ�ǵ�һ��ʯͷ�ľ���)
		while (left < right) {
			int mid = (left + right) >> 1;
			//��������С��m����ǰ��������ȥ ��ô����СһЩ��jumpPower,��Ҫ������һ�ε�jumpPowerֵ��mid����1
			if (canJump(st, N, M, mid)) {
				right = mid;
			}
			else {
				//���޷�������ǰ���£�midֵ���豣��
				left = mid + 1;
			}
		}
		cout << left << endl;
	}
	return 0;
}

