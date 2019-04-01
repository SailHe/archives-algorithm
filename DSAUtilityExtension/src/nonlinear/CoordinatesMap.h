#pragma once
#include "../stdafx.h"
#include "../ExtendSpace.h"
#include "./GraphUtil.h"
#include "Graph.h"


//�����ͼ��(��Ȩͼ)
//����ѧ�е�����ϵ����һ���������һ��Ȩֵ(�þ��������һЩ)Ϊ1�Ķ��� ��ԭ�������Ͻ�
//��Ȩ��Ȩֻ��һ����Ч��� ʵ�ʾ�����Ȩͼ ������㷨(������bfs dfs����ָ��Ȩͼ�����㷨)
//template<class Vertex>
class CoordinatesMap {

	//�±�����: �������r��c; ����˳���ܱ� �� �ṩ�˸�˳��Ĺ��캯��(֧��{ r, c })
	typedef struct SubTwain ArraySub;
public:
	/*typedef int Vertex;
	������������: ����֧�� ���ڱȽ�==int bool �Լ� ��ֵ=int bool
	�ڲ�ֻ���ڱ�ʾ��������Ƿ���Ч �ⲿ�������� �����õķ��Ų�����¶�����
	�����ڲ��ķ���ͳһ�� ����Ҫ��Ƿ������ҿ���ֱ��ʹ�÷��Ŷ��ǽӿ�
	λ��(����1�ֽ�) ��Ч��̶�1λ, ֵ��֧��7λ-64~63��intֵ״̬
	*/
	class Vertex {
	public:
		//����
		Vertex(bool valid, short value) :valid(valid), vertexValue(value) {
			assert(-2e15 <= value && value < 2e15);
		}
		//��bool����
		Vertex(bool valid) :valid(valid), vertexValue(32767/*15�ֽ������*/) {}
		//��bool��ֵ��Ч��
		Vertex &operator =(bool rhs) {
			valid = rhs;
			return *this;
		}
		//��short��ֵֵ��
		Vertex &operator =(short rhs) {
			vertexValue = rhs;
			return *this;
		}
		//��int����ֱ�ӱȽ�
		bool operator ==(int const rhs)const {
			return vertexValue == rhs;
		}
		//��bool����ֱ�ӱȽ�
		bool operator ==(bool const rhs)const {
			return valid == rhs;
		}
		//��intת��
		operator int()const {
			return vertexValue;
		}
		//��boolת��
		operator bool()const {
			return valid;
		}
		// cout<<; ֻ���vertexValue
		friend std::ostream &operator<<(std::ostream &os, const CoordinatesMap::Vertex &rhs) {
			int out = rhs;
			os << out;
			return os;
		}
	private:
		bool valid : 1;//��Ч��(��ͨ����)
		int vertexValue : 15;//2e7 = 64; 2e15 = 32768
	};

	CoordinatesMap(size_t limitRow, size_t limitCol, DirCountEnum dirCount);
	~CoordinatesMap();

	// ����ͼ����(���ֵ��ȴ���2��ʽ���ܲ��ÿ�)
	void output(size_t coutWidth = 2);

	//����Ŀ�������
	void setDest(int r, int c);
	void setDest(ArraySub const & de);

	//ʹsub�����������Ч(��ͨ��)
	inline void setValid(size_t r, size_t c);
	//ʹsub�����������Ч(����ͨ��)
	inline void setInvalid(size_t r, size_t c);
	//����һ����Ч�������sub������һ������15λ��ʾ��shortֵ��ʼ��(���޷���ʾ��ô�������ⲿ������չ sort��2�ֽ�16λ)
	inline void setValidVertex(ArraySub const &sub, short value);
	inline void setInvalidVertex(ArraySub const &sub, short value);
	int getVertexValue(size_t r, size_t c) const;
	//���ö�����Ч����true
	bool isVertexValid(size_t r, size_t c) const;
	/*���ƥ������ ��ͼ��ƥ���ִ�str */
	void dfsMach(int r, int c, char *str, int *count);

	template<class Fun>
	//API�ĵ���Graph bfs
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
			//����������֤
			if (present == dest) {
				return true;
			}
			else {
				//setInvalid(present);
				//�����뵱ǰ�������ͨ�����������
				FOR(i, 0, dirCount) {
					next = present + DIR84[i];
					if (canVisit(next.r, next.c)
						&& dist[next.number(limitR)] == MAX_INT32) {//��δ�����ʹ�

						dist[next.number(limitR)] = dist[present.number(limitR)] + 1;//�������
						predecessor[next.number(limitR)] = present.number(limitR);//·����¼
						canVisitBuffer.push(next);
					}
				}
				//setValid(present);//bfs�޷�����
			}
		}
		return false;
	}

	template<class Fun>
	//���ص�Ŀ��������·������
	int dfs(size_t presentR, size_t presentC, Fun const &customVisit) {
		//û��customVisit()ʱ
		//[37, 64](now) ~ [58, 104](canvisit) ~ [136, 237](��������+ȫ����) ~ [260, 395](˫�����+ȫ����)
		//Ȼ�����˱������Ż��������Ч������[11, 30][27] ���� ��Ŀ�г��ǵ���ʱ�ٶ�ʵ���������� �Ͳ�������Щ�Ż��� ��Ӧ�ÿ��ص����㷨������
		//�п���һ����debug�µ�ʵ�ʱ��ּ���ĸ��Ӷȵ͵��㷨�ڿ��˱����Ż������ٶȿ����һ���㷨��ǧ��(�����ͼ��dfs��ͬʵ�־���һ���ܺõ�����)
		//��ô˵��: 4*4��������������ͼֱ��dfs��ת��Ϊ�ڽӱ������ڽӱ�ͼdfs�������ٶ���Ȼ����ǰ��[8, 15][11](����ͼ������ָ��������㶼��ͨ���������ͼ�����߲�ͬ)

		//customVisit()�ȶ���[50, 95(ż��)]

		//0.ÿ����֧Ҫ������
		customVisit(presentR, presentC);
		//1.���������ж�
		if (dest.r == presentR && dest.c == presentC) {
			return 1;
		}
		else {//2.�����֧
			//setInvalid(presentR, presentC);
			validCoordinates[presentR][presentC] = invalidSign;
			int count = 0;
			int nextR, nextC;
			FOR(i, 0, dirCount) {
				nextR = presentR + DIR84[i].first;
				nextC = presentC + DIR84[i].second;
				//3.������֤
				if (canVisit(nextR, nextC)) {
					count += dfs(nextR, nextC, customVisit);
				}
				else {
					continue;
				}
			}
			validCoordinates[presentR][presentC] = validSign;
			//setValid(presentR, presentC );//��������recall(�����ظ����� ���ٵݹ�)
			return count;
		}
	}

	//����ͼһ������ת��Ϊ��ϡ��Ķ���ͼ ����ֱ��ת��Ȼ������ڽӱ��API(����Դ����� �����߿��Զ������)
	//�õ�һ���ɷ��ʵ���ʱ�ȼ����򶥵�ͼ(�����������ϼ̳�ʵ��)
	//isWqualWeightedΪtrueʱ�õ���ͼ��������ͼ�е�ֵ��ΪȨ�ص�ͼ Ĭ���ǵ�Ȩͼ(ȨֵΪ1)
	//template<class T>
	AdjacentListGraph &parityGraph(bool isWqualWeighted = false);
	ArraySub const &getLimitSub();
protected:
	//�����÷��ʷ��������������{ r, c }
	inline virtual void visit(int r, int c);
	//������֤: �������{ r, c }�ɷ���(δԽ��+��ͨ��)����true
	inline bool canVisit(size_t r, size_t c) const;
private:
	ArraySub dest;//destinationĿ�������
	size_t dirCount;//ָ���Ǽ�������
	size_t limitR, limitC;
	//���ڴ��������: ��Ҫ֧�ּ�ֵ�Բ�ѯ(���֧��[][], ���������map)
	JCE::ArrayList<JCE::ArrayList<Vertex>> validCoordinates;

	static const bool validSign = true;//��Ч�����Ǻ�
	static const bool invalidSign = false;//��Ч�����Ǻ�
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
	std::cout << "����ͼ����(��Чʹ��'x'��ʾ): " << std::endl;
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
		if (*(str + 1) == 0)//�����ֻ�����ִ���ԭ��
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

	//���ط���һ�����еı�
	sparseUndirectedGraphBuffer.setDestVertex(dest.r*limitR + dest.c);
	ArraySub start = ArraySub(0, 0), next;
	Graph::Edge edge = Graph::Edge(-1, -1, 0);
	FOR(r, 0, limitR) {
		FOR(c, 0, limitC) {
			start.r = r, start.c = c;
			if (canVisit(start.r, start.c)) {
				//ÿ�μ��һ���������²�0, �Ҳ�1, ���²�4, ���ϲ�5 ��Ŀ���������Ч�����
				for (int i = 0; i < 2; i += 2) {//�������������ҷ���
					FOR(j, 0, 2) {
						next = start + DIR84[i++];
						if (canVisit(next.r, next.c)) {
							edge.ownerID = start.number(limitR, limitC);
							edge.targetID = next.number(limitR, limitC);
							edge.weight = isWqualWeighted ? getVertexValue(next.r, next.c) : 1;//��Ȩͼ
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
	/*����������ΧΧ��һȦ��Ч�����ôһ����ʡȥ�����Խ������ ��ʡȥ������*/
	return 0 <= r && 0 <= c && r < limitR && c < limitC && //���޼��
		validCoordinates[r][c] == validSign; //�������Ч��(��ͨ��)����
	//����ʹ��ArraySub const &locationʱ: ������һЩ(һ�ǽṹ����ķ����ƺ���ʱ �����������Ҳ��һ������ʱ)
}
