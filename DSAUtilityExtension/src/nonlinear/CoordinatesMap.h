#pragma once
#include "../stdafx.h"
#include "../ExtendSpace.h"
#include "./GraphUtil.h"
#include "Graph.h"


//坐标地图类(等权图)
//与数学中的坐标系类似一个坐标代表一个权值(用距离更合适一些)为1的顶点 但原点在左上角
//等权其权只是一个有效标记 实际就是无权图 里面的算法(尤其是bfs dfs都是指无权图搜索算法)
//template<class Vertex>
class CoordinatesMap {

	//下标类型: 必须带有r和c; 定义顺序不能变 或 提供了该顺序的构造函数(支持{ r, c })
	typedef struct SubTwain ArraySub;
public:
	/*typedef int Vertex;
	顶点数据类型: 必须支持 等于比较==int bool 以及 赋值=int bool
	内部只用于表示该坐标点是否有效 外部可以设置 但设置的符号并不暴露给外界
	于是内部的符号统一了 不需要标记符号域并且可以直接使用符号而非接口
	位域(仅含1字节) 有效域固定1位, 值域支持7位-64~63的int值状态
	*/
	class Vertex {
	public:
		//构造
		Vertex(bool valid, short value) :valid(valid), vertexValue(value) {
			assert(-2e15 <= value && value < 2e15);
		}
		//用bool构造
		Vertex(bool valid) :valid(valid), vertexValue(32767/*15字节无穷大*/) {}
		//用bool赋值有效域
		Vertex &operator =(bool rhs) {
			valid = rhs;
			return *this;
		}
		//用short赋值值域
		Vertex &operator =(short rhs) {
			vertexValue = rhs;
			return *this;
		}
		//对int进行直接比较
		bool operator ==(int const rhs)const {
			return vertexValue == rhs;
		}
		//对bool进行直接比较
		bool operator ==(bool const rhs)const {
			return valid == rhs;
		}
		//向int转换
		operator int()const {
			return vertexValue;
		}
		//向bool转换
		operator bool()const {
			return valid;
		}
		// cout<<; 只输出vertexValue
		friend std::ostream &operator<<(std::ostream &os, const CoordinatesMap::Vertex &rhs) {
			int out = rhs;
			os << out;
			return os;
		}
	private:
		bool valid : 1;//有效性(可通过否)
		int vertexValue : 15;//2e7 = 64; 2e15 = 32768
	};

	CoordinatesMap(size_t limitRow, size_t limitCol, DirCountEnum dirCount);
	~CoordinatesMap();

	// 坐标图概览(如果值宽度大于2格式可能不好看)
	void output(size_t coutWidth = 2);

	//设置目的坐标点
	void setDest(int r, int c);
	void setDest(ArraySub const & de);

	//使sub处的坐标点有效(可通过)
	inline void setValid(size_t r, size_t c);
	//使sub处的坐标点无效(不可通过)
	inline void setInvalid(size_t r, size_t c);
	//设置一个有效的坐标点sub并传入一个可用15位表示的short值初始化(若无法表示那么必须在外部自行拓展 sort是2字节16位)
	inline void setValidVertex(ArraySub const &sub, short value);
	inline void setInvalidVertex(ArraySub const &sub, short value);
	int getVertexValue(size_t r, size_t c) const;
	//若该顶点有效返回true
	bool isVertexValid(size_t r, size_t c) const;
	/*深度匹配搜索 从图中匹配字串str */
	void dfsMach(int r, int c, char *str, int *count);

	template<class Fun>
	//API文档见Graph bfs
	bool bfs(size_t originR, size_t originC, JCE::ArrayList<int> &dist, JCE::ArrayList<int> &predecessor,
		Fun const &customVisit) {

		static JCE::queue<ArraySub> canVisitBuffer;
		ArraySub present = { originR, originC }, next;

		const int vertexNum = limitR * limitC;
		dist.assign(vertexNum, MAX_INT32);
		predecessor.assign(vertexNum, -1);
		dist[present.number(limitR)] = 0;
		canVisitBuffer.push(present);

		while (!canVisitBuffer.empty()) {
			present = canVisitBuffer.front();
			canVisitBuffer.pop();
			customVisit(present.r, present.c);
			//结束条件验证
			if (present == dest) {
				return true;
			}
			else {
				//setInvalid(present);
				//加入与当前坐标点连通的所有坐标点
				FOR(i, 0, dirCount) {
					next = present + DIR84[i];
					if (canVisit(next.r, next.c)
						&& dist[next.number(limitR)] == MAX_INT32) {//若未被访问过

						dist[next.number(limitR)] = dist[present.number(limitR)] + 1;//距离更新
						predecessor[next.number(limitR)] = present.number(limitR);//路径记录
						canVisitBuffer.push(next);
					}
				}
				//setValid(present);//bfs无法回溯
			}
		}
		return false;
	}

	template<class Fun>
	//返回到目的坐标点的路径总数
	int dfs(size_t presentR, size_t presentC, Fun const &customVisit) {
		//没有customVisit()时
		//[37, 64](now) ~ [58, 104](canvisit) ~ [136, 237](基本参数+全内联) ~ [260, 395](双域对象+全内联)
		//然而开了编译器优化后上面的效果都是[11, 30][27] 于是 项目中除非调试时速度实在难以忍受 就不用做这些优化了 更应该看重的是算法的优劣
		//有可能一个在debug下的实际表现极差的复杂度低的算法在开了编译优化后其速度快过另一个算法上千倍(坐标点图的dfs不同实现就是一个很好的例子)
		//这么说吧: 4*4的那张满边坐标图直接dfs与转换为邻接表再用邻接表图dfs其最终速度仍然快于前者[8, 15][11](坐标图的满边指所有坐标点都可通过与坐标点图的满边不同)

		//customVisit()稳定在[50, 95(偶尔)]

		//0.每个分支要做的事
		customVisit(presentR, presentC);
		//1.结束条件判断
		if (dest.r == presentR && dest.c == presentC) {
			return 1;
		}
		else {//2.方向分支
			//setInvalid(presentR, presentC);
			validCoordinates[presentR][presentC] = invalidSign;
			int count = 0;
			int nextR, nextC;
			FOR(i, 0, dirCount) {
				nextR = presentR + DIR84[i].first;
				nextC = presentC + DIR84[i].second;
				//3.访问验证
				if (canVisit(nextR, nextC)) {
					count += dfs(nextR, nextC, customVisit);
				}
				else {
					continue;
				}
			}
			validCoordinates[presentR][presentC] = validSign;
			//setValid(presentR, presentC );//搜索回溯recall(不会重复访问 减少递归)
			return count;
		}
	}

	//坐标图一定可以转化为边稀疏的顶点图 可以直接转化然后调用邻接表的API(快过自带方法 但后者可自定义访问)
	//得到一个可访问的临时等价无向顶点图(但难以物理上继承实现)
	//isWqualWeighted为true时得到的图是以坐标图中的值作为权重的图 默认是等权图(权值为1)
	//template<class T>
	AdjacentListGraph &parityGraph(bool isWqualWeighted = false);
	ArraySub const &getLimitSub();
protected:
	//测试用访问方法：访问坐标点{ r, c }
	inline virtual void visit(int r, int c);
	//访问验证: 若坐标点{ r, c }可访问(未越界+可通过)返回true
	inline bool canVisit(size_t r, size_t c) const;
private:
	ArraySub dest;//destination目的坐标点
	size_t dirCount;//指定是几个方向
	size_t limitR, limitC;
	//用于储存坐标点: 需要支持键值对查询(最好支持[][], 如关联容器map)
	JCE::ArrayList<JCE::ArrayList<Vertex>> validCoordinates;

	static const bool validSign = true;//有效坐标点记号
	static const bool invalidSign = false;//无效坐标点记号
};


CoordinatesMap::CoordinatesMap(size_t limitRow, size_t limitCol, DirCountEnum dirCount)
	:dirCount(dirCount) {
	dest = ArraySub();
	limitR = limitRow, limitC = limitCol;
	validCoordinates.resize(limitRow);
	static Vertex initVertex(invalidSign, -1);
	FOR(i, 0, limitRow) {
		validCoordinates[i].assign(limitCol, initVertex);
	}
}
CoordinatesMap::~CoordinatesMap() {}

void CoordinatesMap::output(size_t coutWidth) {
	std::cout << "坐标图概览(无效使用'x'表示): " << std::endl;
	// StandardExtend::outPutIterable(validCoordinates.begin(), validCoordinates.end(), 2, '\0', validCoordinates.size());
	// StandardExtend::outPut2DArrayList(validCoordinates, '\0', coutWidth);
	std::for_each(validCoordinates.begin(), validCoordinates.end(), [](std::vector<Vertex> const &cur) {
		int i = -1;
		std::for_each(cur.begin(), cur.end(), [&i](Vertex const &v) {
			std::cout << (++i == 0 ? "" : " ")
				<< std::setw(2) << std::setfill('0') << (v ? std::to_string((int)v) : "xx");
		});
		std::cout << std::endl;
	});
}

void CoordinatesMap::setDest(int r, int c) {
	dest.c = c, dest.r = r;
}
void CoordinatesMap::setDest(ArraySub const & de) {
	dest = de;
}

inline void CoordinatesMap::setValid(size_t r, size_t c) {
	validCoordinates[r][c] = validSign;
}
inline void CoordinatesMap::setInvalid(size_t r, size_t c) {
	validCoordinates[r][c] = invalidSign;
}
inline void CoordinatesMap::setValidVertex(ArraySub const &sub, short value) {
	assert(-2e15 <= value && value < 2e15);
	validCoordinates[sub.r][sub.c] = validSign;
	validCoordinates[sub.r][sub.c] = value;
}
inline void CoordinatesMap::setInvalidVertex(ArraySub const &sub, short value) {
	assert(-2e15 <= value && value < 2e15);
	validCoordinates[sub.r][sub.c] = invalidSign;
	validCoordinates[sub.r][sub.c] = value;
}
int CoordinatesMap::getVertexValue(size_t r, size_t c) const {
	return validCoordinates[r][c];
}
bool CoordinatesMap::isVertexValid(size_t r, size_t c) const {
	return canVisit(r, c);
}
void CoordinatesMap::dfsMach(int r, int c, char *str, int *count) {
	if (validCoordinates[r][c] == (int)*str) {
		if (*(str + 1) == 0)//这就是只能是字串的原因
			(*count)++;
		else {
			str++;
			for (int i = 0; i < 4; i++) {
				dfsMach(r + DIR84[i].first, c + DIR84[i].second, str, count);
			}
		}
	}
}

AdjacentListGraph &CoordinatesMap::parityGraph(bool isWqualWeighted) {
	//static AdjacentMatrixGraph sparseUndirectedGraphBuffer = AdjacentMatrixGraph(0);
	static AdjacentListGraph sparseUndirectedGraphBuffer = AdjacentListGraph(0);
	sparseUndirectedGraphBuffer.resize(0);
	sparseUndirectedGraphBuffer.resize(limitR*limitC);

	//最快地访问一次所有的边
	sparseUndirectedGraphBuffer.setDestVertex(dest.r*limitR + dest.c);
	ArraySub start = ArraySub(0, 0), next;
	Graph::Edge edge = Graph::Edge(-1, -1, 0);
	FOR(r, 0, limitR) {
		FOR(c, 0, limitC) {
			start.r = r, start.c = c;
			if (canVisit(start.r, start.c)) {
				//每次检查一个坐标点的下侧0, 右侧1, 右下侧4, 右上侧5 若目标坐标点有效就添加
				for (int i = 0; i < 2; i += 2) {//现在是上下左右方向
					FOR(j, 0, 2) {
						next = start + DIR84[i++];
						if (canVisit(next.r, next.c)) {
							edge.ownerID = start.number(limitR, limitC);
							edge.targetID = next.number(limitR, limitC);
							edge.weight = isWqualWeighted ? getVertexValue(next.r, next.c) : 1;//等权图
							sparseUndirectedGraphBuffer.insertEdgeUndirected(edge);
						}
					}
				}
			}
		}
	}
	return sparseUndirectedGraphBuffer;
}
CoordinatesMap::ArraySub const &CoordinatesMap::getLimitSub() {
	static ArraySub limitSub;
	limitSub = ArraySub(limitR, limitC);
	return limitSub;
}
inline void CoordinatesMap::visit(int r, int c) {
	//printf("%d %d\n", r, c);
}
inline bool CoordinatesMap::canVisit(size_t r, size_t c) const {
	/*可以在最外围围上一圈无效标记这么一来就省去多余的越界检查了 还省去两个域*/
	return 0 <= r && 0 <= c && r < limitR && c < limitC && //界限检查
		validCoordinates[r][c] == validSign; //坐标点有效性(可通过)检验
	//参数使用ArraySub const &location时: 开销大一些(一是结构内域的访问似乎费时 二是条件语句也比一条语句费时)
}
