#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#define EPS 1E-5
typedef struct PolyNode *Pnode;
//double��
struct PolyNode
{
	//ϵ��   ͷ������0
	double coef;
	//ָ��  ͷ������
	double expon;
	Pnode next;
};
typedef Pnode List;
const int BT_NODE_LEN = sizeof(struct PolyNode);
//���ͷŵ�ǰ��� �����ڵ��ַ��Ϊ��һ�����ĵ�ַ û�� ����NULL
void DestroyIt(List *P)
{
	List Ptemp = NULL;
	if ((*P)->next)
		Ptemp = (*P)->next;
	free(*P);
	*P = Ptemp;
}
/*****************Ӧ��������ϰ�ȫ�ĺ����滻����һ����BUG��**********************/
//�ͷŽ��P-next P->next = P->next->next
void DestroyNext(List P) {
	List Ptemp = NULL;
	if (P->next) {
		//if (P->next->next)
		Ptemp = P->next->next;
		free(P->next);
		P->next = Ptemp;
	}
	else
		puts("ERROR");
}
//���ٵ����� ��ͷP��NULL
void Destroy(List P)
{
	List Ptemp = P->next;
	P->next = NULL;
	while (Ptemp)
	{
		P = Ptemp->next;
		free(Ptemp);
		Ptemp = P;
	}
}
//һԪ����ʽ��
void derivation(List p) {
	while (p) {/*��Ϊ��ͷ�ڵ� ����ֱ�Ӳ���p->next*/
		p->next->coef *= p->next->expon--;
		if (p->next->coef == 0)
			DestroyNext(p);
		p = p->next;
	}
}
//�ȶ�ָ�� �ٶ�ϵ��
List ReadPoly(int termsCount)
{
	List Head = NULL, Pleft = NULL, Pright = NULL;
	Pleft = Head = (List)malloc(BT_NODE_LEN);
	memset(Head, 0, BT_NODE_LEN);
	while (termsCount-- > 0)
	{
		Pright = (List)malloc(BT_NODE_LEN);
		scanf("%lf%lf", &Pright->expon, &Pright->coef);
		Pleft->next = Pright;
		Pleft = Pright;
		Pright = Pright->next;
	}
	Pleft->next = NULL;
	return Head;
}
List Add(List L1, List L2)//����Dest �����ж��Ƿ�����Դ����
{
	List Head = (List)malloc(BT_NODE_LEN), P = Head;
	List p1 = L1->next, p2 = L2->next;
	int ISAdd = 1;
	memset(Head, 0, sizeof(BT_NODE_LEN));
	while (p1 && p2)//ָ�������ȴ��Ų�� ��ȴ�Ͷ�Ų
	{
		if (ISAdd)
		{
			P->next = (List)malloc(BT_NODE_LEN);
			ISAdd = 0;
		}
		if (p1->expon == p2->expon)/*�ϲ�ͬ���� ��֮һ*/
		{
			if (p1->coef + p2->coef)
			{
				P->next->coef = p1->coef + p2->coef;
				P->next->expon = p1->expon;
				ISAdd = 1;
			}
			//p1 = p1->next;
			//p2 = p2->next;
			DestroyIt(&p1);
			DestroyIt(&p2);
		}
		else if (p1->expon > p2->expon)
		{
			if (p1->coef)
			{
				P->next->coef = p1->coef;
				P->next->expon = p1->expon;
				ISAdd = 1;
			}
			//p1 = p1->next;
			DestroyIt(&p1);
		}
		else
		{
			if (p2->coef)
			{
				P->next->coef = p2->coef;
				P->next->expon = p2->expon;
				ISAdd = 1;
			}
			//p2 = p2->next;
			DestroyIt(&p2);
		}
		ISAdd ? P = P->next : 0;
	}
	P->next = p1 ? p1 : p2;//������ ��NULLһ��
	L1->next = L2->next = NULL;
	return Head;
}
List Muit(List L1, List L2)
{
	List SumHead = (List)malloc(BT_NODE_LEN);
	List P1 = L1->next, P2 = L2->next;
	List Head = (List)malloc(BT_NODE_LEN), P = NULL;//�м�����
	memset(SumHead, 0, BT_NODE_LEN);//��֤�������Ϸ�
	memset(Head, 0, BT_NODE_LEN);
	while (P1)//��L1
	{
		P = Head;
		P2 = L2->next;
		while (P2)//�����P2 ����浽SumHead
		{
			if (P1->coef && P2->coef)
			{
				P->next = (List)malloc(BT_NODE_LEN);
				P->next->coef = P1->coef * P2->coef;
				P->next->expon = P1->expon + P2->expon;
				P = P->next;
			}
			P2 = P2->next;
		}
		P->next = NULL;
		SumHead = Add(SumHead, Head);
		P1 = P1->next;
	}
	return SumHead;
}
void PrintPoly(List P)
{
	int n = 0;
	if (P->next)
	{
		while (P->next)
		{
			if (n++ != 0)
				printf(" ");
			P = P->next;
			//cout << P->coef << " " << P->expon;
			printf("%f %f", P->coef, P->expon);
		}
	}
	else
	{
		printf("0 0");
	}
	puts("");
}

//�ж�doubleֵ�Ƿ�Ϊ0
int IsDoubleZero(double num) {
	return fabs(num) < EPS;
}
//�ж�doubleֵ����1λ��������Ƿ�Ϊ0 ��Ϊ0����0
int judgeDoubleTainOne(double num) {
	//return num >= 0.05 ? 1 : 0;
	return num > 0.05 || IsDoubleZero(num - 0.05) == 1 ? 1 : 0;
	//num>=0.05����һλ��Ϊ0.1��Ϊ0 ��֮ϵ����Ϊ0->����0
}
//�������������ݽ�����+1
int Length(List L) {/*��ʵ�ֻ��ں�ͷ�ڵ������*/
	List p = L;
	int n = 0;
	while (p)
		++n, p = p->next;
	return n;
}
//���ͷŵ�ǰ��� �����ڵ��ַ��Ϊ��һ�����ĵ�ַ û�� ��NULL
void DestroyNode(List *P)
{
	List Ptemp = NULL;
	if ((*P)->next)
		Ptemp = (*P)->next;
	free(*P);
	*P = Ptemp;
}
//�޳�����һλС����Ϊ0���� ����ִ�к��������
int TainOne(List PHead) {
	//int length = 0;
	List P = PHead;
	while (P && P->next) {
		if (judgeDoubleTainOne(P->next->coef) == 1) {
			//do nothing
			//++length;
		}
		else {
			DestroyNode(&P->next);
		}
		P = P->next;
	}
	return Length(PHead->next);
}

//���������������(ϵ������һλ��Ϊ0��)
void PrintPolyOnereTain(List P){
	int n = 0;
	int length = TainOne(P);
	if (P->next && length != 0){
		printf("%d ", length);
		while (P->next){
			P = P->next;
			if (n++ != 0)
				printf(" ");
			printf("%.f %.1f", P->expon, P->coef);
		}
	}
	else{
		printf("0 0 0.0");
	}
	puts("");
}
int main()
{
	//freopen("input", "r", stdin);
	List P1, P2, PP, PS;
	int termsCount = -1;
	while (1 == scanf("%d", &termsCount)) {
		P1 = ReadPoly(termsCount);
		scanf("%d", &termsCount);
		P2 = ReadPoly(termsCount);
		//PP = Muit(P1, P2);
		//PrintPoly(PP);
		PS = Add(P1, P2);
		PrintPolyOnereTain(PS);

		Destroy(P1);
		Destroy(P2);
		Destroy(PS);
	}
	return 0;
}

/*
input:
2
1 2.4 0 3.2
2
2 1.5 1 0.5

ouput: 3 2 1.5 1 2.9 0 3.2

2
1 2.4 0 3.2
2
2 0.05 1 0.5

1 1 0.03
1 1 0.01

0 0 0.0

1 1 0.03
2 1 0.01 0 0.05
*/