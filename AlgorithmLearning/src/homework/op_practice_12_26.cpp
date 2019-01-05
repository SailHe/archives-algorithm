/*
origin: teacher's example1.c
对已有程序作出改善

分页管理的主存分配与回收

*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include <list>

// 一页的大小 PAGE_SIZE byte; 1KB
#define PAGE_SIZE 1024
// 进程的上限数目 (假定系统中最多存在5个进程)
#define MAX_PROC 5
// 每个进程最多能申请的页面数 (假定每个进程最多申请8KB的内存)
#define PROC_MAX_PAGE 8
// 物理内存模拟表(位视图)的每维大小
#define BIT_MAP_N 8
// 块数
#define BLOCK_NUM BIT_MAP_N*BIT_MAP_N

/*
位示图结构
位示图是利用二进制的一位来表示磁盘中的一个盘块的使用情况。
这里的字节号 和 位号不是指主存的Byte和bit, 仅仅表示其使用一个一位数组中的每一个1bit来表示一个主存块的使用情况
若每一个数组元素为1Byte大小的话, 那么就相当于一个二维数组了, 此处实现应该只是为了方便
*/
struct BitMap{
	// (行表示字节号, 列表示位号) 值为1表示占用 0表示未被占用 块号=row*BIT_MAP_N_COL
	int map[BIT_MAP_N][BIT_MAP_N];
	// 空闲块数
	int freeBlockNum;
} bitMap;

/* 进程控制块信息结构 */
struct PCBInfo{
	/* 进程名 */
	char ProcName[30];
	/* 进程序号 */
	int ProcIndex;
	/* 进程大小 单位: Byte */
	int ProcSize;
	/* 进程页表起始地址 (每个进程都有一个自己的页表用于 页号与块号之间的映射 index表示页号, value表示块号)*/
	int *ProcPageTable;
};

/* 进程列表 
struct PCBInfo ProcessesArrList[MAX_PROC] = {
	{"null", -1, -1, NULL},
	{"null", -1, -1, NULL},
	{"null", -1, -1, NULL},
	{"null", -1, -1, NULL},
	{"null", -1, -1, NULL}
};*/

/* 进程链表 */
std::list<PCBInfo> ProcessLinkList;

//typedef PCBInfo * IteratorPro;
typedef std::list<PCBInfo>::iterator IteratorPro;

/* 系统中已存在的进程数 */
int procNum = 0;
/* 此表仅用于初始化已有的两个进程的页表 */
int InitProcPageTable[MAX_PROC][PROC_MAX_PAGE] = {
	{0, 3, 5, 7, -1, -1, -1,-1},
	{1, 2, 9, 13, 16, -1, -1, -1}
};

// ---- 向进程表中填充一个新进程 若失败返回0
int addAndFillNewPro(PCBInfo bufferPCB) {
	//IteratorPro left = ProcessesArrList, right = ProcessesArrList + procNum;
	//IteratorPro left = ProcessLinkList.begin(), right = ProcessLinkList.end();
	//ProcessesArrList[procNum] = bufferPCB;
	ProcessLinkList.push_back(bufferPCB);
	// 系统已有进程数增1
	++procNum;
	return 1;
}

// ---- 根据进程索引查询进程
// 返回进程的迭代器 若不存在返回 end迭代器
IteratorPro findProcess(int procIdx) {
	//IteratorPro left = ProcessesArrList, right = ProcessesArrList + procNum;
	IteratorPro left = ProcessLinkList.begin(), right = ProcessLinkList.end();
	for (; left != right; ++left) {
		if (left->ProcIndex == procIdx) {
			break;
		}
		else {
			continue;
		}
		
	}
	return left;
}

// 删除一个进程 返回值是更新后的容器的被删除元素的后一个元素的迭代器
IteratorPro deleteProc(IteratorPro left) {
	/* 回收进程页表空间 
	free(left->ProcPageTable);
	left->ProcPageTable = NULL;
	// 标识该进程的进程号为-1，以区别于其他正常进程
	left->ProcIndex = -1;
	left = NULL;
	*/
	left = ProcessLinkList.erase(left);
	return left;
}

void printMenu() {
	printf("****************使用位示图法管理主存空间****************\n");
	printf("\t\t1 显示位示图\n");
	printf("\t\t2 显示进程页表\n");
	printf("\t\t3 为进程分配主存\n");
	printf("\t\t4 回收进程占用的主存\n");
	printf("\t\t5 退出系统\n");
	printf("********************************************************\n");
}
void initBitMap(){
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
void initProcPageTable(void){
	int i = 0, j, pageNum;
	PCBInfo p1 = { "Proc1", 1, 4000, NULL }, p2 = { "Proc2", 2, 4800, NULL };
	addAndFillNewPro(p1);
	addAndFillNewPro(p2);
	//IteratorPro left = ProcessesArrList, right = ProcessesArrList + procNum;
	IteratorPro left = ProcessLinkList.begin(), right = ProcessLinkList.end();
	// 初始化已有进程的页表
	for (; left != right; ++left, ++i){
		// 计算进程所占页面数 向上取整
		pageNum = (int)ceil(left->ProcSize * 1.0 / PAGE_SIZE);
		left->ProcPageTable = (int *)malloc(pageNum * sizeof(int));
		for (j = 0; j < pageNum; j++){
			left->ProcPageTable[j] = InitProcPageTable[i][j];
		}
	}
}
void printBitMap(){
	int i, j;
	printf(" B\\b");
	//printf("%4s", " ");
	for (i = 0; i < BIT_MAP_N; i++){
		printf("%4d", i);
	}
	printf("\n");
	printf("%7s", " ");
	for (i = 0; i < BIT_MAP_N; i++){
		printf("----");
	}
	printf("\n");
	for (i = 0; i < BIT_MAP_N; i++){
		printf("%3d|", i);
		for (j = 0; j < BIT_MAP_N; j++){
			printf("%4d", bitMap.map[i][j]);
		}
		printf("\n");
	}
}
void printPageTable(int procNum){
	int j;
	int pageNum;
	if (procNum == 0){
		printf("there's no processes now.\n");
	}
	else{
		//IteratorPro left = ProcessesArrList, right = ProcessesArrList + procNum;
		IteratorPro left = ProcessLinkList.begin(), right = ProcessLinkList.end();
		for (; left != right; ++left){
			printf("--------%d号进程页表--------\n", left->ProcIndex);
			printf("名称: %4s\n", left->ProcName);
			printf("%4s\t", "页号");
			printf("%4s\n", "块号");
			pageNum = (int)ceil(left->ProcSize * 1.0 / PAGE_SIZE);
			for (j = 0; j < pageNum; j++){
				printf("%4d\t", j);
				printf("%4d\n", left->ProcPageTable[j]);
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
 * --------------------------------------------------------------------------------------------------------------------- */
int  alloc(int procIdx, char procName[], int procSize){
	int pageNum;
	int i, j, subProcPageTable = 0;
	int blockNum, ret;
	/* 计算进程申请的空间共需要多少内存块  */
	pageNum = (int)ceil(procSize * 1.0 / PAGE_SIZE);
	/* 根据位示图检查空闲块总数是否满足申请进程的要求 */
	if (pageNum <= PROC_MAX_PAGE && pageNum <= bitMap.freeBlockNum){
		PCBInfo tmpPCB;
		/* 动态申请页表空间，并把进程的页表项都初始化为-1 */
		tmpPCB.ProcPageTable = (int *)malloc(pageNum * sizeof(int));
		memset(tmpPCB.ProcPageTable, -1, pageNum * sizeof(int));
		ret = pageNum;
		/* 填充进程相关信息*/
		tmpPCB.ProcIndex = procIdx;
		strcpy(tmpPCB.ProcName, procName);
		tmpPCB.ProcSize = procSize;
		for (i = 0; i < BIT_MAP_N; i++) {
			for (j = 0; j < BIT_MAP_N; j++){
				if (bitMap.map[i][j] == 0){
					blockNum = i * BIT_MAP_N + j;   /* 根据行号列号求出对应的块号*/
					printf("行: %d, 列: %d, 块号: %d\n", i, j, blockNum);
					/* 填充进程页表 */
					tmpPCB.ProcPageTable[subProcPageTable++] = blockNum;
					/* 修改位示图该位的标记，为1表示已被占用 */
					bitMap.map[i][j] = 1;
					bitMap.freeBlockNum--;  /* 空闲块总数减1 */
					if (subProcPageTable == pageNum) {/* 进程所有页都已分配完空间，跳出循环 */
						addAndFillNewPro(tmpPCB);
						return ret;
					}
				}
			}
		}
	}
	/* 如果申请的页数大于规定的进程最多页数 */
	else if (pageNum > PROC_MAX_PAGE){
		printf("You can request 8KB at most!\n");
		return 0;
	}
	else {/* 如果已无足够的空闲块分配给该进程 */
		printf("bitmap shows there's not enough free blocks.");
		return 0;
	}
	return ret;
}

void sortInvalidProc() {
	//IteratorPro left1 = ProcessesArrList, right1 = ProcessesArrList + procNum;
	IteratorPro left1 = ProcessLinkList.begin(), right1 = ProcessLinkList.end();
	for (; left1 != right1; ++left1) {
		if (left1->ProcIndex < 0) {
			//IteratorPro left2 = ProcessesArrList, right2 = ProcessesArrList + procNum;
			IteratorPro left2 = left1, right2 = ProcessLinkList.end();
			for (++left2; left2 != right2; ++left1, ++left2) {
				left1->ProcIndex = left2->ProcIndex;
				left1->ProcSize = left2->ProcSize;
				strcpy(left1->ProcName, left2->ProcName);
				left1->ProcPageTable = left2->ProcPageTable;
			}
		}
	}
}

/* --------------------------------------------------------------------------
 *                setFree()函数
 *	函数功能：释放某进程所占用的主存空间，并更新位示图。
 *	函数参数：param1：procIdx ——释放进程的进程号
 *	函数返回值：无返回值。
 * -------------------------------------------------------------------------- */
void setFree(int procIdx){
	int row, colum;
	// blockNo
	int blockCount;
	int j;
	//IteratorPro left = ProcessesArrList, right = ProcessesArrList + procNum;
	IteratorPro left = ProcessLinkList.begin(), right = ProcessLinkList.end();
	left = findProcess(procIdx);
	if (left == right){
		printf("the proc index doesn't exist.\n");
	}
	else {
		for (blockCount = 0; left->ProcPageTable[blockCount] >= 0; blockCount++);
		/* 根据块号得到对应的位示图的行号和列号，并更新位示图*/
		for (j = 0; j < blockCount; j++) {
			row = left->ProcPageTable[j] / BIT_MAP_N;
			colum = left->ProcPageTable[j] % BIT_MAP_N;
			bitMap.map[row][colum] = 0;
			bitMap.freeBlockNum++;
		}
		deleteProc(left);
		/* 进程列表重新排序*/
		sortInvalidProc();
		/* 系统中已有进程数减1 */
		procNum--;
		printf("memory occupied by process %d has been released.\n", procIdx);
	}

}
int main(){

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
	while (choice != 5){
		switch (choice){
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
			if (pageNum){
				newProcIdx++;
				printf("allocation is successful!\n");
				printPageTable(procNum);
			}
			else{
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

