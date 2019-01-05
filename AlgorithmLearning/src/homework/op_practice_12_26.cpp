/*
origin: teacher's example1.c
�����г�����������

��ҳ�����������������

*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include <list>

// һҳ�Ĵ�С PAGE_SIZE byte; 1KB
#define PAGE_SIZE 1024
// ���̵�������Ŀ (�ٶ�ϵͳ��������5������)
#define MAX_PROC 5
// ÿ����������������ҳ���� (�ٶ�ÿ�������������8KB���ڴ�)
#define PROC_MAX_PAGE 8
// �����ڴ�ģ���(λ��ͼ)��ÿά��С
#define BIT_MAP_N 8
// ����
#define BLOCK_NUM BIT_MAP_N*BIT_MAP_N

/*
λʾͼ�ṹ
λʾͼ�����ö����Ƶ�һλ����ʾ�����е�һ���̿��ʹ�������
������ֽں� �� λ�Ų���ָ�����Byte��bit, ������ʾ��ʹ��һ��һλ�����е�ÿһ��1bit����ʾһ��������ʹ�����
��ÿһ������Ԫ��Ϊ1Byte��С�Ļ�, ��ô���൱��һ����ά������, �˴�ʵ��Ӧ��ֻ��Ϊ�˷���
*/
struct BitMap{
	// (�б�ʾ�ֽں�, �б�ʾλ��) ֵΪ1��ʾռ�� 0��ʾδ��ռ�� ���=row*BIT_MAP_N_COL
	int map[BIT_MAP_N][BIT_MAP_N];
	// ���п���
	int freeBlockNum;
} bitMap;

/* ���̿��ƿ���Ϣ�ṹ */
struct PCBInfo{
	/* ������ */
	char ProcName[30];
	/* ������� */
	int ProcIndex;
	/* ���̴�С ��λ: Byte */
	int ProcSize;
	/* ����ҳ����ʼ��ַ (ÿ�����̶���һ���Լ���ҳ������ ҳ������֮���ӳ�� index��ʾҳ��, value��ʾ���)*/
	int *ProcPageTable;
};

/* �����б� 
struct PCBInfo ProcessesArrList[MAX_PROC] = {
	{"null", -1, -1, NULL},
	{"null", -1, -1, NULL},
	{"null", -1, -1, NULL},
	{"null", -1, -1, NULL},
	{"null", -1, -1, NULL}
};*/

/* �������� */
std::list<PCBInfo> ProcessLinkList;

//typedef PCBInfo * IteratorPro;
typedef std::list<PCBInfo>::iterator IteratorPro;

/* ϵͳ���Ѵ��ڵĽ����� */
int procNum = 0;
/* �˱�����ڳ�ʼ�����е��������̵�ҳ�� */
int InitProcPageTable[MAX_PROC][PROC_MAX_PAGE] = {
	{0, 3, 5, 7, -1, -1, -1,-1},
	{1, 2, 9, 13, 16, -1, -1, -1}
};

// ---- ����̱������һ���½��� ��ʧ�ܷ���0
int addAndFillNewPro(PCBInfo bufferPCB) {
	//IteratorPro left = ProcessesArrList, right = ProcessesArrList + procNum;
	//IteratorPro left = ProcessLinkList.begin(), right = ProcessLinkList.end();
	//ProcessesArrList[procNum] = bufferPCB;
	ProcessLinkList.push_back(bufferPCB);
	// ϵͳ���н�������1
	++procNum;
	return 1;
}

// ---- ���ݽ���������ѯ����
// ���ؽ��̵ĵ����� �������ڷ��� end������
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

// ɾ��һ������ ����ֵ�Ǹ��º�������ı�ɾ��Ԫ�صĺ�һ��Ԫ�صĵ�����
IteratorPro deleteProc(IteratorPro left) {
	/* ���ս���ҳ��ռ� 
	free(left->ProcPageTable);
	left->ProcPageTable = NULL;
	// ��ʶ�ý��̵Ľ��̺�Ϊ-1����������������������
	left->ProcIndex = -1;
	left = NULL;
	*/
	left = ProcessLinkList.erase(left);
	return left;
}

void printMenu() {
	printf("****************ʹ��λʾͼ����������ռ�****************\n");
	printf("\t\t1 ��ʾλʾͼ\n");
	printf("\t\t2 ��ʾ����ҳ��\n");
	printf("\t\t3 Ϊ���̷�������\n");
	printf("\t\t4 ���ս���ռ�õ�����\n");
	printf("\t\t5 �˳�ϵͳ\n");
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
	// ��ʼ�����н��̵�ҳ��
	for (; left != right; ++left, ++i){
		// ���������ռҳ���� ����ȡ��
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
			printf("--------%d�Ž���ҳ��--------\n", left->ProcIndex);
			printf("����: %4s\n", left->ProcName);
			printf("%4s\t", "ҳ��");
			printf("%4s\n", "���");
			pageNum = (int)ceil(left->ProcSize * 1.0 / PAGE_SIZE);
			for (j = 0; j < pageNum; j++){
				printf("%4d\t", j);
				printf("%4d\n", left->ProcPageTable[j]);
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
 * --------------------------------------------------------------------------------------------------------------------- */
int  alloc(int procIdx, char procName[], int procSize){
	int pageNum;
	int i, j, subProcPageTable = 0;
	int blockNum, ret;
	/* �����������Ŀռ乲��Ҫ�����ڴ��  */
	pageNum = (int)ceil(procSize * 1.0 / PAGE_SIZE);
	/* ����λʾͼ�����п������Ƿ�����������̵�Ҫ�� */
	if (pageNum <= PROC_MAX_PAGE && pageNum <= bitMap.freeBlockNum){
		PCBInfo tmpPCB;
		/* ��̬����ҳ��ռ䣬���ѽ��̵�ҳ�����ʼ��Ϊ-1 */
		tmpPCB.ProcPageTable = (int *)malloc(pageNum * sizeof(int));
		memset(tmpPCB.ProcPageTable, -1, pageNum * sizeof(int));
		ret = pageNum;
		/* �����������Ϣ*/
		tmpPCB.ProcIndex = procIdx;
		strcpy(tmpPCB.ProcName, procName);
		tmpPCB.ProcSize = procSize;
		for (i = 0; i < BIT_MAP_N; i++) {
			for (j = 0; j < BIT_MAP_N; j++){
				if (bitMap.map[i][j] == 0){
					blockNum = i * BIT_MAP_N + j;   /* �����к��к������Ӧ�Ŀ��*/
					printf("��: %d, ��: %d, ���: %d\n", i, j, blockNum);
					/* ������ҳ�� */
					tmpPCB.ProcPageTable[subProcPageTable++] = blockNum;
					/* �޸�λʾͼ��λ�ı�ǣ�Ϊ1��ʾ�ѱ�ռ�� */
					bitMap.map[i][j] = 1;
					bitMap.freeBlockNum--;  /* ���п�������1 */
					if (subProcPageTable == pageNum) {/* ��������ҳ���ѷ�����ռ䣬����ѭ�� */
						addAndFillNewPro(tmpPCB);
						return ret;
					}
				}
			}
		}
	}
	/* ��������ҳ�����ڹ涨�Ľ������ҳ�� */
	else if (pageNum > PROC_MAX_PAGE){
		printf("You can request 8KB at most!\n");
		return 0;
	}
	else {/* ��������㹻�Ŀ��п������ý��� */
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
 *                setFree()����
 *	�������ܣ��ͷ�ĳ������ռ�õ�����ռ䣬������λʾͼ��
 *	����������param1��procIdx �����ͷŽ��̵Ľ��̺�
 *	��������ֵ���޷���ֵ��
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
		/* ���ݿ�ŵõ���Ӧ��λʾͼ���кź��кţ�������λʾͼ*/
		for (j = 0; j < blockCount; j++) {
			row = left->ProcPageTable[j] / BIT_MAP_N;
			colum = left->ProcPageTable[j] % BIT_MAP_N;
			bitMap.map[row][colum] = 0;
			bitMap.freeBlockNum++;
		}
		deleteProc(left);
		/* �����б���������*/
		sortInvalidProc();
		/* ϵͳ�����н�������1 */
		procNum--;
		printf("memory occupied by process %d has been released.\n", procIdx);
	}

}
int main(){

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

