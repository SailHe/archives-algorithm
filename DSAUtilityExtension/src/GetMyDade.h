#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define R 3
#define C 5
#define N R*C
#define name_size 30
#define swap(a, b) laber temp = (a); (a) = (b);(b) = temp
struct test
{
	char Flag[name_size], ID[name_size], Title[name_size];
	double Ratio_ac[3];//�ύ����
	double User[3];//��������
};
struct laber
{
	char name[15];
	int done;//������
	int sp;//������
	test exam;
};
laber group[N];
int r = C;//���п���
int sub;
char user_name[name_size];
char opname[name_size] = {}, savename[name_size] = {};
const char acquiesce[name_size] = { "D:\\LaberDate\\" };
void find(int *sub);
int search(char name[]);
void pr(int lf);
void Over_UI();
void getFlag_or_ID(char s[])
{
	int c;
	memset(s, 0, sizeof(s));
	scanf("%s", s);
	while ((c = getchar()) != 32);
	ungetc(c, stdin);
}
void getTitle(char s[])
{
	int temp;
	int u, i;
	memset(s, 0, name_size);
	for (i = 0;; i++)
	{
		if ((s[i] = getchar()) == '/')
		{
			if (s[i - 1] == 32 && (temp = getchar()) == 32)
			{
				ungetc(temp, stdin);
				for (u = 0;; u++)
				{
					ungetc(s[i - u], stdin);
					if (isdigit(s[i - u]))
					{
						s[i - u] = 0;
						break;
					}	
				}
				break;
			}
		}
	}
}
void getRatio_or_User(double s[])
{
	int temp;
	memset(s, 0, sizeof(s));
	scanf("%lf", &s[1]);
	while (!isdigit(temp = getchar()));
	ungetc(temp, stdin);
	scanf("%lf", &s[2]);
	s[0] = s[1] / s[2];
}
void verify()
{
	int order;
	while (true)
	{
		puts("��������û�����");
		scanf("%s", user_name);
		if (sub = search(user_name) != -1)
		{
			puts("��֤�ɹ������������Խ��в�����");
			break;
		}
		else
		{
			printf("��Ǹ��δ�ҵ�%s", user_name);
			puts("try aging?\n1:yes  0:no");
			scanf("%d", &order);
			switch (order)
			{
			case 1:break;
			default:exit(0);
			}
		}
	}
}
//��ȡ�û�����
void Get_OJ_Date()
{
	int order;
start:
	verify();
	while (true)
	{
		puts("0:��������  1:��������  2:��ӡ����  3:����  ����:����ģ��");
		scanf("%d", &order);
		switch (order)
		{
		case 0:Over_UI(); break;
		case 1:
			getFlag_or_ID(group[sub].exam.Flag); 
			getFlag_or_ID(group[sub].exam.ID); 
			getTitle(group[sub].exam.Title); 
			getRatio_or_User(group[sub].exam.Ratio_ac); 
			getRatio_or_User(group[sub].exam.User); 
			break;
		case 2:pr(r); break;
		case 3:system("cls"); break;
		default:{getchar(); goto start; }
		}
	}
	
}
/*
���ȼ�1.����5����������Ŀ�ؽ���
���ȼ�2.����Ҫ�󽲵���Ŀѡ����
���ȼ�3.ac�ʵ���30%����Ŀѡ����
Yes A The Triangle 7 / 8 7 / 7
*/