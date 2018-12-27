/* example1.c */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#define PAGE_SIZE 1024
#define BLOCK_NUM 64
#define MAX_PROC 5
#define PROC_MAX_PAGE 8
#define N 8
/* λʾͼ�ṹ */
struct BitMap
{
	int map[N][N];
	int freeBlockNum;
}bitMap;
/* ���̿��ƿ���Ϣ�ṹ */
struct PCBInfo
{
	char ProcName[30];     /* ������ */
	int ProcIndex;         /* ������� */
	int ProcLength;        /* ���̴�С */
	int * ProcPageTable;   /* ����ҳ����ʼ��ַ */
};

/* �ٶ�ϵͳ��������5������ */
struct PCBInfo Processes[5] =
{
	{"Proc1", 1, 4000, NULL},
	{"Proc2", 2, 4800, NULL}
};
/* ϵͳ���Ѵ��ڵĽ�����   */
int procNum = 2;
/* �ٶ�ÿ�������������8KB���ڴ�  */
int ProcPageTable[MAX_PROC][PROC_MAX_PAGE] =
{
	{0, 3, 5, 7, -1, -1, -1,-1},
	{1, 2, 9, 13, 16, -1, -1, -1}
};
void printMenu()
{
	printf("****************ʹ��λʾͼ����������ռ�****************\n");
	printf("\t\t1 ��ʾλʾͼ\n");
	printf("\t\t2 ��ʾ����ҳ��\n");
	printf("\t\t3 Ϊ���̷�������\n");
	printf("\t\t4 ���ս���ռ�õ�����\n");
	printf("\t\t5 �˳�ϵͳ\n");
	printf("********************************************************\n");
}
void initBitMap()
{
	bitMap.freeBlockNum = 55;
	memset(bitMap.map, 0, sizeof(bitMap.map));
	bitMap.map[0][0] = 1;
	bitMap.map[0][1] = 1;
	bitMap.map[0][2] = 1;
	bitMap.map[0][3] = 1;
	bitMap.map[0][5] = 1;
	bitMap.map[0][7] = 1;
	bitMap.map[1][1] = 1;
	bitMap.map[1][5] = 1;
	bitMap.map[2][0] = 1;
}
void initProcPageTable(void)
{
	int i, j, pageNum;
	for (i = 0; i < 2; i++)
	{
		pageNum = (int)ceil(Processes[i].ProcLength * 1.0 / PAGE_SIZE);
		Processes[i].ProcPageTable = (int *)malloc(pageNum * sizeof(int));
		for (j = 0; j < pageNum; j++)
		{
			Processes[i].ProcPageTable[j] = ProcPageTable[i][j];
		}
	}
}
void printBitMap()
{
	int i, j;
	printf("%4s", " ");
	for (i = 0; i < N; i++)
	{
		printf("%4d", i);
	}
	printf("\n");
	printf("%7s", " ");
	for (i = 0; i < N; i++)
	{
		printf("----");
	}
	printf("\n");
	for (i = 0; i < N; i++)
	{
		printf("%3d|", i);
		for (j = 0; j < N; j++)
		{
			printf("%4d", bitMap.map[i][j]);
		}
		printf("\n");
	}
}
void printPageTable(int procNum)
{
	int i, j;
	int pageNum;
	if (procNum == 0)
	{
		printf("there's no processes now.\n");
	}
	else
	{
		for (i = 0; i < procNum; i++)
		{
			printf("--------��%d�Ž���ҳ��--------\n", Processes[i].ProcIndex);
			printf("%6s\t", "ҳ��");
			printf("%6s\n", "���");
			pageNum = (int)ceil(Processes[i].ProcLength * 1.0 / PAGE_SIZE);
			for (j = 0; j < pageNum; j++)
			{
				printf("%4d\t", j);
				printf("%4d\n", Processes[i].ProcPageTable[j]);
			}
		}
	}
}
/* ----------------------------------------------------------------------------------------------------------------------
 *                alloc()����
 *	�������ܣ�����λʾͼ�����ҵ�����Ҫ�������ռ䣬Ϊ���̷�������ռ䡣
 *	����������param1��procIdx ����������̵Ľ��̺�
 *            param2��procName ����������̵Ľ�����
 *            param3��procSize �����������������ռ��С
 *	��������ֵ������λʾͼ�����ҵ�����Ҫ�������ռ䣬�򷵻ظý��̵�ҳ��
*              �ȣ�ҳ���������Ŀ����������򣬷���0
 * *--------------------------------------------------------------------------------------------------------------------*/
int  alloc(int procIdx, char procName[], int procSize)
{
	int pageNum;
	int i, j, k = 0;
	int blockNum, ret;
	/* �����������Ŀռ乲��Ҫ�����ڴ��  */
	pageNum = (int)ceil(procSize * 1.0 / PAGE_SIZE);
	/* ����λʾͼ�����п������Ƿ�����������̵�Ҫ�� */
	if (pageNum <= PROC_MAX_PAGE && pageNum <= bitMap.freeBlockNum)
	{
		/* ��̬����ҳ��ռ䣬���ѽ��̵�ҳ�����ʼ��Ϊ-1 */
		Processes[procNum].ProcPageTable = (int *)malloc(pageNum * sizeof(int));
		memset(Processes[procNum].ProcPageTable, -1, pageNum * sizeof(int));
		ret = pageNum;
		/* �����������Ϣ*/
		Processes[procNum].ProcIndex = procIdx;
		strcpy(Processes[procNum].ProcName, procName);
		Processes[procNum].ProcLength = procSize;
		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
			{

				if (bitMap.map[i][j] == 0)
				{
					blockNum = i * N + j;   /* �����к��к������Ӧ�Ŀ��*/
					/* ������ҳ�� */
					Processes[procNum].ProcPageTable[k++] = blockNum;
					/* �޸�λʾͼ��λ�ı�ǣ�Ϊ1��ʾ�ѱ�ռ�� */
					bitMap.map[i][j] = 1;
					bitMap.freeBlockNum--;  /* ���п�������1 */
					if (k == pageNum)  /* ��������ҳ���ѷ�����ռ䣬����ѭ�� */
					{
						procNum++;     /* ϵͳ���н�������1  */
						return ret;
					}
				}
			}
	}
	/* ��������ҳ�����ڹ涨�Ľ������ҳ�� */
	else if (pageNum > PROC_MAX_PAGE)
	{
		printf("You can request 8KB at most!\n");
		return 0;
	}
	else   /* ��������㹻�Ŀ��п������ý��� */
	{
		printf("bitmap shows there's not enough free blocks.");
		return 0;
	}
END:return ret;
}

/* --------------------------------------------------------------------------
 *                setFree()����
 *	�������ܣ��ͷ�ĳ������ռ�õ�����ռ䣬������λʾͼ��
 *	����������param1��procIdx �����ͷŽ��̵Ľ��̺�
 *	��������ֵ���޷���ֵ��
 * -------------------------------------------------------------------------- */
void setFree(int procIdx)
{
	int row, colum;
	int blockCount, blockNo;
	int i, j, k;
	for (i = 0; i < MAX_PROC; i++)
	{
		if (Processes[i].ProcIndex == procIdx)
		{
			for (blockCount = 0; Processes[i].ProcPageTable[blockCount] >= 0; blockCount++);
			/* ���ݿ�ŵõ���Ӧ��λʾͼ���кź��кţ�������λʾͼ*/
			for (j = 0; j < blockCount; j++)
			{
				row = Processes[i].ProcPageTable[j] / N;
				colum = Processes[i].ProcPageTable[j] % N;
				bitMap.map[row][colum] = 0;
				bitMap.freeBlockNum++;
			}
			/* ���ս���ҳ��ռ�*/
			free(Processes[i].ProcPageTable);
			Processes[i].ProcPageTable = NULL;
			/* ��ʶ�ý��̵Ľ��̺�Ϊ-1���������������������� */
			Processes[i].ProcIndex = -1;

			/* �����б���������*/
			for (k = 0; k < procNum; k++)
			{
				if (Processes[k].ProcIndex < 0)
				{
					for (j = k + 1; j < procNum; k++, j++)
					{
						Processes[k].ProcIndex = Processes[j].ProcIndex;
						Processes[k].ProcLength = Processes[j].ProcLength;
						strcpy(Processes[k].ProcName, Processes[j].ProcName);
						Processes[k].ProcPageTable = Processes[j].ProcPageTable;
					}
				}
			}
			/* ϵͳ�����н�������1 */
			procNum--;
			printf("memory occupied by process %d has been released.\n", procIdx);
			break;
		}
	}
	if (i >= MAX_PROC)
	{
		printf("the proc index doesn't exist.\n");
	}

}
int main()
{

	int choice;
	char procName[30];
	int procSize;
	int newProcIdx = 3;   // �½��Ľ��̺Ŵ�3��ʼ����2�����н���
	int delProcIdx;
	int pageNum;

	initBitMap();    // ��ʼ��λʾͼ
	initProcPageTable();  // ��ʼ������ҳ��
	printMenu();
	printf("Your choice: ");
	scanf("%d", &choice);
	while (choice != 5)
	{
		switch (choice)
		{
		case 1:
			printBitMap();
			break;
		case 2:
			printPageTable(procNum);
			break;
		case 3:
			printf("Please input process name: ");
			scanf("%s", &procName);
			printf("Please input memory size the process requested(B): ");
			scanf("%d", &procSize);
			pageNum = alloc(newProcIdx, procName, procSize);
			if (pageNum)
			{
				newProcIdx++;
				printf("allocation is successful!\n");
				printPageTable(procNum);
			}
			else
			{
				printf("allocation failed.Please wait...\n");
			}
			break;
		case 4:
			printf("Please input process index:\n");
			scanf("%d", &delProcIdx);
			/* �ý�����ɺ��µĽ��̺Ż�������ù��Ľ��̺Ų��ᱻ�ظ�ʹ�� */
			setFree(delProcIdx);
			printPageTable(procNum);
			break;
		default:
			printf("your choice is error, please retry.\n");
			break;
		}
		printMenu();
		printf("Your choice: ");
		scanf("%d", &choice);
	}
	return 0;
}

