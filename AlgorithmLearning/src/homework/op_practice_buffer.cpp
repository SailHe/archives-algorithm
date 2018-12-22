#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// 空闲区说明表中的记录数上限
#define N 5
// 空闲区说明表
struct freePartition{
	// 空闲区起始地址
	int startAddress;
	// 空闲区大小
	int size;
	// 空闲区状态：0为空表目，1为可用空闲块
	int state;
};

// 进程内存划分
struct progressPartition {
	// 进程标识号
	int pid;
	// 进程内存起始地址
	int startAddress;
	// 进程内存大小
	int size;
	// 1表示此表项有效 0表示无效 (是否有效的进程)
	int valid;
};
// ---- 空闲块的数目(已使用的数目)
int freeBlockCnt = 4;
// 用于进程标识号的产生(已有两个)
int pidOrigin = 2;
// 内存占用说明表 的进程数
int progressCnt = 2;
// ---- 空闲区说明表
struct freePartition freeBlock[N] = { {20, 20, 1}, {80, 50, 1}, {150, 100, 1}, {300, 30, 0}, {600, 100, 1} };
// ---- 内存占用说明表
struct progressPartition progressBlock[N] = { {1, 300, 10, 1} , {0, 310, 20, 1} };

// ---- 清屏
void clear() {
	system("cls");
}

// 查询指定进程标识号的进程
// 返回指定进程的内存占用说明表下标 若不存在则返回负数 或>=N的值
int findProgressBlockSub(int pid) {
	int i = -1;
	for (i = 0; i < N; ++i) {
		if (progressBlock[i].pid == pid) {
			break;
		}
	}
	return i;
}

// ---- 新建一个内存为0的进程 可以指定pid(但只能大于之前所有pid中最大的)
// ---- 返回新进程的pid 若进程说明表已满 则返回-1
int addProgressBlock() {
	int resultPid = -1;
	if (progressCnt < N) {
		resultPid = progressBlock[progressCnt].pid = pidOrigin++;
		progressBlock[progressCnt].valid = 1;
		progressBlock[progressCnt].size = 0;
		progressBlock[progressCnt].startAddress = 0;
		++progressCnt;
	}
	else {
		resultPid = -1;
	}
	return resultPid;
}

// ---- 寻找一个未使用的表项将参数给定的块放入其中 如果成功返回表项下标 失败(不存在)返回一个负数
int findAndSetBlock(int startAddress, int size) {
	int i;
	for (i = 0; i < N; ++i)
	{
		if (freeBlock[i].state == 0)
		{
			freeBlock[i].startAddress = startAddress;
			freeBlock[i].size = size;
			freeBlock[i].state = 1;
			break;
		}
		else {
			continue;
		}
	}
	return i >= N ? -1 : i;
}

// ---- 比较两个分区块表项的位置{相离(负数), 相邻(0), 相交(正数)} 返回距离 (PS: 参数必须保证lhs在左, rhs在右)
int calcDistenceBetweenLeftRight(int lhsLeftAddr, int lhsSize, int rhsLeftAddr, int rhsSize) {
	int lhsRightAddr = lhsLeftAddr + lhsSize;
	int rhsRightAddr = rhsLeftAddr + rhsSize;
	int dis = lhsRightAddr - rhsLeftAddr;
	return dis;
}

// ---- 比较两个分区块表项的位置{相离(负数), 相邻(0), 相交(正数)} 返回距离 (自动考虑参数位置)
int calcDistenceBetween(int addr1, int size1, int addr2, int size2) {
	int dis = 0;
	if (addr1 < addr2) {
		dis = calcDistenceBetweenLeftRight(addr1, size1, addr2, size2);
	}
	//  (addr1 >= addr2)
	else {
		dis = calcDistenceBetweenLeftRight(addr2, size2, addr1, size1);
	}
	return dis;
}

/* ----------------------------------------------------------------------------------------------------------------------
 *                alloc()函数
 *	函数功能：为作业分配主存空间
 *	函数参数：param1：sizeRequest——作业所申请的空闲区大小
 *	函数返回值：如果找到符合要求的空闲区，则返回为作业分配的空闲区的起始地址
 *              否则，返回-1
* -------------------------------------------------------------------------------------------------------------------*/
int alloc(int sizeRequest)
{
	int i;
	/* flagFind为是否找到符合要求的空闲块的标记：找到为1，未找到为0  */
	int flagFind = 0;
	int returnAddress;
	for (i = 0; i < N; i++)
	{
		if (freeBlock[i].state == 1 && freeBlock[i].size >= sizeRequest)
		{
			/* 返回为作业分配的主存地址  */
			returnAddress = freeBlock[i].startAddress;
			flagFind = 1;
			if (freeBlock[i].size > sizeRequest)
			{
				int newAddres = freeBlock[i].startAddress + sizeRequest;
				int newSize = freeBlock[i].size - sizeRequest;
				if (findAndSetBlock(newAddres, newSize) > 0) {
					// ----如果请求分配的内存块将切分已有表项 需要将切分后的碎片也加入表中 如果超过表的大小 请求失败
					// freeBlock[i].startAddress = newAddres;
					// freeBlock[i].size = newSize;
					freeBlock[i].size = sizeRequest;
					freeBlock[i].state = 0;
				}
				else {
					clear();
					printf("请求后[空闲区说明表]无法容纳表项, 请求失败! (可以发起合适的分配请求使某一个表项变为未使用状态)\n");
				}
				goto END;
			}
			if (freeBlock[i].size == sizeRequest)
			{
				--freeBlockCnt;
				freeBlock[i].state = 0;
				goto END;
			}
		}
	}
	if (flagFind == 0)
	{
		returnAddress = -1;
	}
END:return returnAddress;
}

/* --------------------------------------------------------------------------
 *                setFree()函数
 *	  函数功能：主存回收函数
 *	  函数参数：param1:startAddress 回收区的起始地址
 *              param2:size         回收区的大小
 *	  函数返回值：无返回值
 * -------------------------------------------------------------------------- */
void setFree(int startAddress, int size)
{
	int i, indexLinkNext, indexLinkPre;
	/* tagLinkNext用来标记回收区的高地址是否邻接一个空闲区, 邻接为1，否则为0; */
	/* tagLinkPre用来标记回收区的地地址是否邻接一个空闲区，邻接为1，否则为0; */
	int tagLinkPre = 0, tagLinkNext = 0;
	for (i = 0; i < N; i++)
	{
		/* 检查当前空闲区说明表中的表目是否是回收区的F2邻接区 */
		if (freeBlock[i].startAddress == startAddress + size && freeBlock[i].state == 1)
		{
			tagLinkNext = 1;
			indexLinkNext = i;
		}
		/* 检查当前空闲区说明表中的表目是否是回收区的F1邻接区 */
		if (freeBlock[i].startAddress + freeBlock[i].size == startAddress
			&& freeBlock[i].state == 1)
		{
			tagLinkPre = 1;
			indexLinkPre = i;
		}
		/* 如果回收区的F1邻接区和F2邻接区都已找到，则跳出循环  */
		if (tagLinkPre && tagLinkNext)
		{
			break;
		}
	}
	if (tagLinkPre && tagLinkNext)   /* 如果回收区与F1和F2都邻接 */
	{
		freeBlock[indexLinkPre].size += freeBlock[indexLinkNext].size + size;
		freeBlock[indexLinkNext].state = 0;
	}
	else if (tagLinkNext)            /* 如果回收区与F2相邻接  */
	{
		freeBlock[indexLinkNext].startAddress = startAddress;
		freeBlock[indexLinkNext].size += size;
	}
	else if (tagLinkPre)              /* 如果回收区与F1相邻接   */
	{
		freeBlock[indexLinkPre].size += size;
	}
	else                            /* 如果回收区与F1、F2都不邻接 */
	{
		// ---- 上面是对释放内存块后邻接的表项进行拼接
		// ---- 这里是寻找是否存在未使用的表项容纳已释放的空闲块
		if (freeBlockCnt < N) {
			for (i = 0; i < N; i++)
			{
				if (freeBlock[i].state == 1){
					// ---- 检查准备释放的表项块是否与已有块冲突 (由于是排过序的1 必然出现在0前面)
					if (calcDistenceBetween(freeBlock[i].startAddress, freeBlock[i].size, startAddress, size) < 0) {
						continue;
					}
					else {
						// 不符合
						clear();
						printf("请求释放的块与 当前空闲区说明表已有表项冲突! 释放请求忽略\n");
						printf("冲突项 第%d号: ", i + 1);
						printf("\t| %3d\t\t%3d\t\t%3d\t\t|\n",
							freeBlock[i].startAddress, freeBlock[i].size, freeBlock[i].state);
						break;
					}
				}
				else {
					freeBlock[i].startAddress = startAddress;
					freeBlock[i].size = size;
					freeBlock[i].state = 1;
					++freeBlockCnt;
					break;
				}
			}
		}
		else {
			clear();
			printf("当前空闲区说明表 无法容纳更多的表项 释放请求忽略\n");
		}
	}
}

/* --------------------------------------------------------------------------
 *                adjustFreeTable()函数
 *	  函数功能：对空闲区说明表中的空闲区进行调整
 *              使空闲区按起始地址从小到大排列，空表目放在后面
 *	  函数参数：无参数
 *	  函数返回值：无返回值
 * -------------------------------------------------------------------------- */
void adjustFreeTable()
{
	struct freePartition tmp;
	int numEmptyRecord = 0;
	int emptyRecord[N];
	int i, j;
	for (i = 0; i < N; i++)
	{
		emptyRecord[i] = -1;
	}
	/*统计空闲分区说明表中的空表目数，并在emptyRecord中记录空表目的编号*/
	for (i = 0, j = 0; i < N; i++)
	{
		if (freeBlock[i].state == 0)
		{
			numEmptyRecord++;
			emptyRecord[j++] = i;
		}
	}

	/* 逆序搜索空闲分区说明表中的空闲块登记项，并将其与空白表项交换 */
	for (i = N - 1, j = 0; i >= N - numEmptyRecord; i--)
	{
		if (freeBlock[i].state == 1)
		{
			// 把原本散落在空闲表中的空白项先换至空闲表尾部
			tmp.state = freeBlock[i].state;
			tmp.startAddress = freeBlock[i].startAddress;
			tmp.size = freeBlock[i].size;

			freeBlock[i].state = freeBlock[emptyRecord[j]].state;
			freeBlock[i].startAddress = freeBlock[emptyRecord[j]].startAddress;
			freeBlock[i].size = freeBlock[emptyRecord[j]].size;

			freeBlock[emptyRecord[j]].state = tmp.state;
			freeBlock[emptyRecord[j]].startAddress = tmp.startAddress;
			freeBlock[emptyRecord[j]].size = tmp.size;

			j++;
		}
	}

	/* 再把前N-numEmptyRecord个记录用冒泡排序法按地址从小到大的顺序排列 */
	for (i = 0; i < N - numEmptyRecord; i++)
	{
		for (j = 0; j < N - numEmptyRecord - 1 - i; j++)
		{
			if (freeBlock[j].startAddress > freeBlock[j + 1].startAddress)
			{
				tmp.startAddress = freeBlock[j].startAddress;
				tmp.size = freeBlock[j].size;
				tmp.state = freeBlock[j].state;

				freeBlock[j].startAddress = freeBlock[j + 1].startAddress;
				freeBlock[j].size = freeBlock[j + 1].size;
				freeBlock[j].state = freeBlock[j + 1].state;

				freeBlock[j + 1].startAddress = tmp.startAddress;
				freeBlock[j + 1].size = tmp.size;
				freeBlock[j + 1].state = tmp.state;
			}
		}
	}
}

/* --------------------------------------------------------------------------
 *                printFreeTable()函数
 *	  函数功能：打印空闲区说明表
 *	  函数参数：无参数
 *	  函数返回值：无返回值
 * -------------------------------------------------------------------------- */
void printFreeTable()
{
	int i;
	printf("\t\t| --------------------------------------------\t|\n");
	printf("\t\t| [空闲区说明表]空闲块(已使用表项): %d\t\t|\n", freeBlockCnt);
	printf("\t\t| start\t\tsize\t\tstate\t\t|\n");
	printf("\t\t| --------------------------------------------\t|\n");

	for (i = 0; i < N; i++)
	{
		printf("\t\t| %3d\t\t%3d\t\t%3d\t\t|\n",
			freeBlock[i].startAddress, freeBlock[i].size, freeBlock[i].state);
		printf("\t\t| --------------------------------------------\t|\n");
	}
}

// ----- 打印进程内存表
void printProgressTable(){
	int i;
	printf("\t\t| --------------------------------------------\t|\n");
	printf("\t\t| [进程内存占用说明表]\t\t\t\t|\n");
	printf("\t\t|  pid\tstart\t\tsize\t\t\t|\n");
	printf("\t\t| --------------------------------------------\t|\n");

	for (i = 0; i < N; ++i){
		if (progressBlock[i].valid == 1) {
			printf("\t\t| %3d\t\t%3d\t%3d\t\t\t|\n",
				progressBlock[i].pid, progressBlock[i].startAddress, progressBlock[i].size);
			printf("\t\t| --------------------------------------------\t|\n");
		}
	}
	printf("\n");
}

// --- 如果用户的输入导致程序终止 返回0
// --- 第一题的shell
int shellFor1() {
	int result = 1;
	int requestSize, startAddr;
	int releaseStartAddr, releaseSize;
	int i;
	char orderBuffer[1024] = {};
	adjustFreeTable();
	printFreeTable();
	printf("\n分配请求: a; 释放请求: f; 退出程序: e ; 清屏: else\n");
	scanf("%s", orderBuffer);
	getchar();
	if (*orderBuffer == 'a')
	{
		// printf("Initially the free memory is as below:\n");
		// adjustFreeTable();
		// printFreeTable();
		printf("input memory size requested:");
		scanf("%d", &requestSize);
		getchar();   /* 吸收回车符 */

		startAddr = alloc(requestSize);
		if (startAddr == -1)
		{
			printf("memory allocation failed.Please wait!\n");
		}
		else
		{
			printf("allocation successs!\n");
			printf("job's memory start address is:%d, memory size is %d\n",
				startAddr, requestSize);
			// adjustFreeTable();
			// printf("after allocation, the free memory is as below:\n");
			// printFreeTable();
			printf("job is running.\n");
			printf("job is terminated.\n");
			//延时
			for (i = 0; i < 1000; i++);
		}
	}
	else if(*orderBuffer == 'f') {
		printf("Please input the start address of the area to be released:\n");
		scanf("%d", &releaseStartAddr);
		getchar();
		printf("Please input the size of the area to be released:\n");
		scanf("%d", &releaseSize);
		getchar();
		setFree(releaseStartAddr, releaseSize);
		// adjustFreeTable();
		// printf("after free the memory, the free memory is as below:\n");
		// printFreeTable();
	}
	else if (*orderBuffer == 'e') {
		result = 0;
	}
	else {
		clear();
	}
	return result;
}

// ---- 从键盘读入进程号 然后查找并返回进程的下标
int shellForReadPid() {
	printf("输入进程标识号:");
	int pid;
	scanf("%d", &pid);
	// 吸收回车符
	getchar();
	int pSub = findProgressBlockSub(pid);
	if (pSub < 0 || pSub >= N) {
		if (pid >= pidOrigin) {
			pidOrigin = pid;
			pid = addProgressBlock();
		}
		else if (pid < 0) {
			pid = addProgressBlock();
		}
		else {
			printf("进程号只能大于现存所有进程的最大值 或者输入负数自动分配!\n");
		}
		pSub = findProgressBlockSub(pid);
	}
	else {
		// do nothing
	}
	return pSub;
}

// --- 第二题的shell: 带有进程表
int shellFor2() {
	int result = 1;
	int requestSize, startAddr;
	int releaseStartAddr, releaseSize;
	int i;
	char orderBuffer[1024] = {};
	adjustFreeTable();
	printProgressTable();
	printFreeTable();
	printf("\n分配请求: a; 释放请求: f; 退出程序: e ; 清屏: else\n");
	scanf("%s", orderBuffer);
	getchar();

	if (*orderBuffer == 'a'){
		int pSub = shellForReadPid();
		if (pSub < 0 || pSub >= N) {
			printf("进程编号错误!\n");
		}
		else {
			printf("input memory size requested:");
			scanf("%d", &requestSize);
			// 吸收回车符
			getchar();

			startAddr = alloc(requestSize);
			if (startAddr == -1) {
				printf("memory allocation failed.Please wait!\n");
			}
			else {
				printf("allocation successs!\n");
				printf("job's memory start address is:%d, memory size is %d\n",
					startAddr, requestSize);
				progressBlock[pSub].startAddress = startAddr;
				progressBlock[pSub].size = requestSize;
				printf("job is running.\n");
				printf("job is terminated.\n");
				//延时
				for (i = 0; i < 1000; i++);
			}
		}
	} else if (*orderBuffer == 'f') {
		int pSub = shellForReadPid();
		releaseStartAddr = progressBlock[pSub].startAddress;
		releaseSize = progressBlock[pSub].size;
		progressBlock[pSub].valid = 0;
		--progressCnt;
		setFree(releaseStartAddr, releaseSize);
	}
	else if (*orderBuffer == 'e') {
		result = 0;
	}
	else {
		clear();
	}
	return result;
}

int main() {
	char requestOrNot, ch;
	int i;
	int requestSize;
	int startAddr;
	int ret = 0;
	int releaseStartAddr, releaseSize;

	// while (shellFor1());
	while (shellFor2());
	return ret;
}
/*
--------- 此算法来源于z老师的doc 本人对其进行大量修改完成下述功能 ---------

四、上机思考题
1. 完善示例程序，增加错误处理的代码。
	比如程序中空闲分区登记表长度设为5，如果实际运行过程中空闲分区表长度超过5时的错误处理，
	{目前会直接放弃新增的表项 -> 阻止}
	另外如果用户手工输入回收区的起始地址和分区大小时，输入的分区与空闲分区登记表中的分区有矛盾时的错误处理等。
	{
		请求内存过大->不满足请求;
		释放时: 
		let 内存是表中已有的块 <==> start地址相同
		if 相邻{
			合并
		}else{
			let 目的: 将一个合法的state==0的块变为请求释放的空闲块
			let 行为1 : 将该合法块变为释放请求的块
			if 与任意空闲区相离{
				行为1
			}else{
				阻止
			}
		}
	}
2. 实现难度2的编程。数据结构中需要增加一张内存占用说明表，主程序可由用户选择进行主存分配或主存回收操作，
	所需参数是：若是主存分配，输入作业（进程）名和所需主存空间大小；若是回收，输入回收作业（进程）的作业号（进程号）。
	能循环进行主存分配和回收。
	要求画出主存分配和回收的算法流程图，并编程实现。
3. 假设数据结构由分区表改为分区双向链表，试画出主存分配和回收算法流程图，并编程实现。
4. 假设主存分配时采用最佳适应法而非首次适应法，试编程实现。
5. 结合进程控制及进程互斥同步内容模拟实现多个进程同时申请主存的编程，也需要考虑当多个进程同时释放占用主存的情形。

*/

