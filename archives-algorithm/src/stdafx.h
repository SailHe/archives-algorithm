#ifndef __STDAFX_H
#define __STDAFX_H
/*Standard Application Framework Extensions����׼Ӧ�ó����ܵ���չ��*/
/*���������ļ���Ҫ��ͷ�ļ�(��ʱ�䰴����ĸ����)*/
#include <assert.h>//����
#include <windows.h>//QueryPerformanceFrequency//
#include <conio.h>//getch//
#include <time.h>
#include<cstring>//memset//
#include<cstdio>//scanf//
#include <fstream>
#include <iostream>
#include<algorithm>
#include<cmath>
#include<iomanip>
#include<set>
#include<string>
#include<stack>
#include<queue>
#include<list>
#include<map>
#include<functional>//greater<int>
#include<memory>/*����ָ��:shared_ptr<>; unique_ptr<>;---p400*/
#include <unordered_set>
#include <unordered_map>

/*
explicit(��ʽ/���/��ȷ),ֻ�����ڴ����������Ĺ��캯��ǰ�����߱�����������ʹ�øù��캯��������ʽ����ת����
���ȷʵ��ת��������������ʽ������ת����ʽ�����С��Ӷ������˿��ܳ��ֵ�����Ͷ���ĵ��ÿ�����

<�������ģʽ> ��ģʽʹ�� C++ �����д�ó��Ķ����š�
:������һ�����뵥Ԫ��Translation Unit���ж��壨define�����������͡������ȣ�����һ�����뵥Ԫ�������ǡ�
��������Compiler�����������з��뵥Ԫ��
��������Linker����������������ָ�� extern ���ŵ����ã�
�Ӷ����ɵ�һ��ִ���ļ���
extern(������) Ϊ�˷����������뵥Ԫ������һ�����ļ����еı�������󣬶���ͨ���ͣ��������������ࡢ�ṹ���ࣩ
export(����/����/���) �ڶ���ģ��������ģ�庯��ʱ��extern����
*/


//���Կ��� �رպ�ɼ���һЩ����Ҫ���ж� �������Ч��
#define DEBUG false

//����һ����������  �����: ֧�ֵ�����������
#define PARAMETER_ALL_OBJECT(container) container.begin(), container.end()

//limit(�޶ȣ�����)��һ�����޼�: > limitʱ�˳�
//�޽�for
#define FOR_LIMIT(i, lowerBound, limit) for (auto i = lowerBound; i < limit; ++i)

//�޷����޽�for
#define FOR_LIMIT_UNSIGNED(i, lowerBound, limit) for (size_t i = lowerBound; i < limit; ++i)

//it���û�ָ����һ������, ���������½�(lowerBound)����, ������Χ(bound)�ڱ��뱣֤�������ֹ��(terminator: �ս���)
//��ֹfor �����: ָ��������, ����1, ��2 (��12��֧��!=����)
#define FOR(it, lowerBound, terminator) for (auto it = lowerBound; it != terminator; ++it)

//for����������Ԫ�� �����: ֧�ֵ�����������  (�����������end()�Ż���FOR)
#define FOR_ALL_OBJECT(container) for (auto &element : container)

//ĩβ�޿ո�Ĵ�ӡ(cout)
#define DISPLAY(A, lowerBound, terminator) {FOR(i, lowerBound, terminator){printf(i == lowerBound ? "" : " ");cout << A[i] << endl;}}

//�����ô�ӡ
#define DEPRINTF(s){if(DEBUG)printf("---%s\n", s);}


//4�ֽ�32λ �з��������0x7FFFFFFF; ˫�ֽ��޷������������ֵ��65535
#define infinity 2147483647
using namespace std;
using namespace std::placeholders;

/*��Biology��ص���������:���� ���� ����Ա*/
enum TYPE{ PLAYER, PEOPLE, MANAGER, MONSTER };
enum STATUS{ NORMAL, ABNORMAL, CONK, QUIT, BLOCKING, BYPASS };//���� �쳣 ����(�����ó�Ա�����ж�) �˳� ���赲 ����״̬
/*��ͼ: up(����)->��Ӱ, down->����, left->��, right->��  ViewCount:��ͼ����*/
enum View{ BACK, FRONT, LEFT, RIGHT, ViewCount };
//����ָ��������ֹʱ�ǲ�����һ֡(��ʼ֡, ��һ֡)
enum StaticFrameEnum{ ORIGIN_FRAME, PAST_FRAME };
enum STEP{ STEP_X_AXIS, STEP_Y_AXIS };//��������
enum MODE{ ON, OFF, UNDEFINED };//ģʽ
//constant.h
const View defaultView = BACK;//Ĭ����ͼ(��ֵת��Ϊint������0)
const double EPSINON = 1e-15;
const double EPS = 1e-9;
const double PI = 3.14159265;//��λ��
const double PIA = acos(-1.0);//���ű�����
/*
const int DIR4x[] = { -1, 0, 1, 0 };
const int DIR4y[] = { 0, 1, 0, -1 };
extern const int Dir8[8][2];
extern const int Dir4[4][2];
*/
const int DIR84R[] = { 1, 0, -1, 0, 1, -1, -1, 1 };
const int DIR84C[] = { 0, 1, 0, -1, 1, 1, -1, -1 };
enum DirCountEnum{ DIR4 = 4, DIR8 = 8 };
typedef pair<int, int> DirectionVector;//�����ӷ�����ʱ ����first �� second��ϰ�ߵı�ʾxy rc��������ͬ
const DirectionVector DIR84[8] = {//����extern ������ض�������
	/*r��������->��4123���� ���ڵ��� ˳�����������*/
	{ 1, 0 }/*��*/,
	{ 0, 1 }/*��*/,
	{ -1, 0 }/*��*/,
	{ 0, -1 }/*��*/,

	{ 1, 1 }/*����*/,
	{ 1, -1 }/*����*/,
	{ -1, -1 }/*����*/,
	{ -1, 1 }/*����*/,
};
//�ɱ�����
//template<typename T>using Varrays = vector<T>;
////�����
//template<typename T>using ArrayList = vector<T>;
////�ڽӱ�
//template<typename T>using LinkedList = list<T>;
//
//using Package = vector<pair<int, int>>;/*������*/
//typedef int Sub;//�±�����
//typedef Sub RowsSub;//�б�����
//typedef Sub ColsSub;//�б�����
//typedef __int64 I64;
//_ASSERTE(_CrtCheckMemory());//�ڴ���

#endif
