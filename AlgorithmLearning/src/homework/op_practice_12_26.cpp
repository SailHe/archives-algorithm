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
/* 位示图结构 */
struct BitMap
{
	int map[N][N];
	int freeBlockNum;
}bitMap;
/* 进程控制块信息结构 */
struct PCBInfo
{
	char ProcName[30];     /* 进程名 */
	int ProcIndex;         /* 进程序号 */
	int ProcLength;        /* 进程大小 */
	int * ProcPageTable;   /* 进程页表起始地址 */
};

/* 假定系统中最多存在5个进程 */
struct PCBInfo Processes[5] =
{
	{"Proc1", 1, 4000, NULL},
	{"Proc2", 2, 4800, NULL}
};
/* 系统中已存在的进程数   */
int procNum = 2;
/* 假定每个进程最多申请8KB的内存  */
int ProcPageTable[MAX_PROC][PROC_MAX_PAGE] =
{
	{0, 3, 5, 7, -1, -1, -1,-1},
	{1, 2, 9, 13, 16, -1, -1, -1}
};
void printMenu()
{
	printf("****************使用位示图法管理主存空间****************\n");
	printf("\t\t1 显示位示图\n");
	printf("\t\t2 显示进程页表\n");
	printf("\t\t3 为进程分配主存\n");
	printf("\t\t4 回收进程占用的主存\n");
	printf("\t\t5 退出系统\n");
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
			printf("--------第%d号进程页表--------\n", Processes[i].ProcIndex);
			printf("%6s\t", "页号");
			printf("%6s\n", "块号");
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
 *                alloc()函数
 *	函数功能：根据位示图如能找到满足要求的主存空间，为进程分配主存空间。
 *	函数参数：param1：procIdx ——申请进程的进程号
 *            param2：procName ——申请进程的进程名
 *            param3：procSize ——进程申请的主存空间大小
 *	函数返回值：根据位示图如能找到满足要求的主存空间，则返回该进程的页表长
*              度（页数，或分配的块数）；否则，返回0
 * *--------------------------------------------------------------------------------------------------------------------*/
int  alloc(int procIdx, char procName[], int procSize)
{
	int pageNum;
	int i, j, k = 0;
	int blockNum, ret;
	/* 计算进程申请的空间共需要多少内存块  */
	pageNum = (int)ceil(procSize * 1.0 / PAGE_SIZE);
	/* 根据位示图检查空闲块总数是否满足申请进程的要求 */
	if (pageNum <= PROC_MAX_PAGE && pageNum <= bitMap.freeBlockNum)
	{
		/* 动态申请页表空间，并把进程的页表项都初始化为-1 */
		Processes[procNum].ProcPageTable = (int *)malloc(pageNum * sizeof(int));
		memset(Processes[procNum].ProcPageTable, -1, pageNum * sizeof(int));
		ret = pageNum;
		/* 填充进程相关信息*/
		Processes[procNum].ProcIndex = procIdx;
		strcpy(Processes[procNum].ProcName, procName);
		Processes[procNum].ProcLength = procSize;
		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
			{

				if (bitMap.map[i][j] == 0)
				{
					blockNum = i * N + j;   /* 根据行号列号求出对应的块号*/
					/* 填充进程页表 */
					Processes[procNum].ProcPageTable[k++] = blockNum;
					/* 修改位示图该位的标记，为1表示已被占用 */
					bitMap.map[i][j] = 1;
					bitMap.freeBlockNum--;  /* 空闲块总数减1 */
					if (k == pageNum)  /* 进程所有页都已分配完空间，跳出循环 */
					{
						procNum++;     /* 系统已有进程数增1  */
						return ret;
					}
				}
			}
	}
	/* 如果申请的页数大于规定的进程最多页数 */
	else if (pageNum > PROC_MAX_PAGE)
	{
		printf("You can request 8KB at most!\n");
		return 0;
	}
	else   /* 如果已无足够的空闲块分配给该进程 */
	{
		printf("bitmap shows there's not enough free blocks.");
		return 0;
	}
END:return ret;
}

/* --------------------------------------------------------------------------
 *                setFree()函数
 *	函数功能：释放某进程所占用的主存空间，并更新位示图。
 *	函数参数：param1：procIdx ——释放进程的进程号
 *	函数返回值：无返回值。
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
			/* 根据块号得到对应的位示图的行号和列号，并更新位示图*/
			for (j = 0; j < blockCount; j++)
			{
				row = Processes[i].ProcPageTable[j] / N;
				colum = Processes[i].ProcPageTable[j] % N;
				bitMap.map[row][colum] = 0;
				bitMap.freeBlockNum++;
			}
			/* 回收进程页表空间*/
			free(Processes[i].ProcPageTable);
			Processes[i].ProcPageTable = NULL;
			/* 标识该进程的进程号为-1，以区别于其他正常进程 */
			Processes[i].ProcIndex = -1;

			/* 进程列表重新排序*/
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
			/* 系统中已有进程数减1 */
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
	int newProcIdx = 3;   // 新建的进程号从3开始，有2个已有进程
	int delProcIdx;
	int pageNum;

	initBitMap();    // 初始化位示图
	initProcPageTable();  // 初始化进程页表
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
			/* 该进程完成后，新的进程号会递增，用过的进程号不会被重复使用 */
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

