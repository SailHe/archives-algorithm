#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// ������˵�����еļ�¼������
#define N 5
struct freePartition            /* ������˵���� */
{
	int startAddress;           /* ��������ʼ��ַ*/
	int size;                   /* ��������С*/
	int state;                  /*������״̬��0Ϊ�ձ�Ŀ��1Ϊ���ÿ��п�*/
};
// ---- ���п����Ŀ(��ʹ�õ���Ŀ)
int allocedCnt = 4;
// ---- ������˵����
struct freePartition freeBlock[N] = { {20, 20, 1}, {80, 50, 1}, {150, 100, 1}, {300, 30, 0}, {600, 100, 1} };

// ---- ����
void clear() {
	system("cls");
}

// ---- Ѱ��һ��δʹ�õı�����������Ŀ�������� ����ɹ����ر����±� ʧ��(������)����һ������
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

// ---- �Ƚ���������������λ��{����(����), ����(0), �ཻ(����)} ���ؾ��� (PS: �������뱣֤lhs����, rhs����)
int calcDistenceBetweenLeftRight(int lhsLeftAddr, int lhsSize, int rhsLeftAddr, int rhsSize) {
	int lhsRightAddr = lhsLeftAddr + lhsSize;
	int rhsRightAddr = rhsLeftAddr + rhsSize;
	int dis = lhsRightAddr - rhsLeftAddr;
	return dis;
}

// ---- �Ƚ���������������λ��{����(����), ����(0), �ཻ(����)} ���ؾ��� (�Զ����ǲ���λ��)
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
 *                alloc()����
 *	�������ܣ�Ϊ��ҵ��������ռ�
 *	����������param1��sizeRequest������ҵ������Ŀ�������С
 *	��������ֵ������ҵ�����Ҫ��Ŀ��������򷵻�Ϊ��ҵ����Ŀ���������ʼ��ַ
 *              ���򣬷���-1
* -------------------------------------------------------------------------------------------------------------------*/
int alloc(int sizeRequest)
{
	int i;
	/* flagFindΪ�Ƿ��ҵ�����Ҫ��Ŀ��п�ı�ǣ��ҵ�Ϊ1��δ�ҵ�Ϊ0  */
	int flagFind = 0;
	int returnAddress;
	for (i = 0; i < N; i++)
	{
		if (freeBlock[i].state == 1 && freeBlock[i].size >= sizeRequest)
		{
			/* ����Ϊ��ҵ����������ַ  */
			returnAddress = freeBlock[i].startAddress;
			flagFind = 1;
			if (freeBlock[i].size > sizeRequest)
			{
				int newAddres = freeBlock[i].startAddress + sizeRequest;
				int newSize = freeBlock[i].size - sizeRequest;
				if (findAndSetBlock(newAddres, newSize) > 0) {
					// ----������������ڴ�齫�з����б��� ��Ҫ���зֺ����ƬҲ������� ���������Ĵ�С ����ʧ��
					// freeBlock[i].startAddress = newAddres;
					// freeBlock[i].size = newSize;
					freeBlock[i].size = sizeRequest;
					freeBlock[i].state = 0;
				}
				else {
					clear();
					printf("�����[������˵����]�޷����ɱ���, ����ʧ��! (���Է�����ʵķ�������ʹĳһ�������Ϊδʹ��״̬)\n");
				}
				goto END;
			}
			if (freeBlock[i].size == sizeRequest)
			{
				--allocedCnt;
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
 *                setFree()����
 *	  �������ܣ�������պ���
 *	  ����������param1:startAddress ����������ʼ��ַ
 *              param2:size         �������Ĵ�С
 *	  ��������ֵ���޷���ֵ
 * -------------------------------------------------------------------------- */
void setFree(int startAddress, int size)
{
	int i, indexLinkNext, indexLinkPre;
	/* tagLinkNext������ǻ������ĸߵ�ַ�Ƿ��ڽ�һ��������, �ڽ�Ϊ1������Ϊ0; */
	/* tagLinkPre������ǻ������ĵص�ַ�Ƿ��ڽ�һ�����������ڽ�Ϊ1������Ϊ0; */
	int tagLinkPre = 0, tagLinkNext = 0;
	for (i = 0; i < N; i++)
	{
		/* ��鵱ǰ������˵�����еı�Ŀ�Ƿ��ǻ�������F2�ڽ��� */
		if (freeBlock[i].startAddress == startAddress + size && freeBlock[i].state == 1)
		{
			tagLinkNext = 1;
			indexLinkNext = i;
		}
		/* ��鵱ǰ������˵�����еı�Ŀ�Ƿ��ǻ�������F1�ڽ��� */
		if (freeBlock[i].startAddress + freeBlock[i].size == startAddress
			&& freeBlock[i].state == 1)
		{
			tagLinkPre = 1;
			indexLinkPre = i;
		}
		/* �����������F1�ڽ�����F2�ڽ��������ҵ���������ѭ��  */
		if (tagLinkPre && tagLinkNext)
		{
			break;
		}
	}
	if (tagLinkPre && tagLinkNext)   /* �����������F1��F2���ڽ� */
	{
		freeBlock[indexLinkPre].size += freeBlock[indexLinkNext].size + size;
		freeBlock[indexLinkNext].state = 0;
	}
	else if (tagLinkNext)            /* �����������F2���ڽ�  */
	{
		freeBlock[indexLinkNext].startAddress = startAddress;
		freeBlock[indexLinkNext].size += size;
	}
	else if (tagLinkPre)              /* �����������F1���ڽ�   */
	{
		freeBlock[indexLinkPre].size += size;
	}
	else                            /* �����������F1��F2�����ڽ� */
	{
		// ---- �����Ƕ��ͷ��ڴ����ڽӵı������ƴ��
		// ---- ������Ѱ���Ƿ����δʹ�õı����������ͷŵĿ��п�
		if (allocedCnt < N) {
			for (i = 0; i < N; i++)
			{
				if (freeBlock[i].state == 1){
					// ---- ���׼���ͷŵı�����Ƿ������п��ͻ (�������Ź����1 ��Ȼ������0ǰ��)
					if (calcDistenceBetween(freeBlock[i].startAddress, freeBlock[i].size, startAddress, size) < 0) {
						continue;
					}
					else {
						// ������
						clear();
						printf("�����ͷŵĿ��� ��ǰ������˵�������б����ͻ! �ͷ��������\n");
						printf("��ͻ�� ��%d��: ", i + 1);
						printf("\t| %3d\t\t%3d\t\t%3d\t\t|\n",
							freeBlock[i].startAddress, freeBlock[i].size, freeBlock[i].state);
						break;
					}
				}
				else {
					freeBlock[i].startAddress = startAddress;
					freeBlock[i].size = size;
					freeBlock[i].state = 1;
					++allocedCnt;
					break;
				}
			}
		}
		else {
			clear();
			printf("��ǰ������˵���� �޷����ɸ���ı��� �ͷ��������\n");
		}
	}
}

/* --------------------------------------------------------------------------
 *                adjustFreeTable()����
 *	  �������ܣ��Կ�����˵�����еĿ��������е���
 *              ʹ����������ʼ��ַ��С�������У��ձ�Ŀ���ں���
 *	  �����������޲���
 *	  ��������ֵ���޷���ֵ
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
	/*ͳ�ƿ��з���˵�����еĿձ�Ŀ��������emptyRecord�м�¼�ձ�Ŀ�ı��*/
	for (i = 0, j = 0; i < N; i++)
	{
		if (freeBlock[i].state == 0)
		{
			numEmptyRecord++;
			emptyRecord[j++] = i;
		}
	}

	/* �����������з���˵�����еĿ��п�Ǽ����������հױ���� */
	for (i = N - 1, j = 0; i >= N - numEmptyRecord; i--)
	{
		if (freeBlock[i].state == 1)
		{
			// ��ԭ��ɢ���ڿ��б��еĿհ����Ȼ������б�β��
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

	/* �ٰ�ǰN-numEmptyRecord����¼��ð�����򷨰���ַ��С�����˳������ */
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
 *                printFreeTable()����
 *	  �������ܣ���ӡ������˵����
 *	  �����������޲���
 *	  ��������ֵ���޷���ֵ
 * -------------------------------------------------------------------------- */
void printFreeTable()
{
	int i;
	printf("\t\t| --------------------------------------------\t|\n");
	printf("\t\t| ���п�(��ʹ�ñ���): %d\t\t\t\t|\n", allocedCnt);
	printf("\t\t| start\t\tsize\t\tstate\t\t|\n");
	printf("\t\t| --------------------------------------------\t|\n");

	for (i = 0; i < N; i++)
	{
		printf("\t\t| %3d\t\t%3d\t\t%3d\t\t|\n",
			freeBlock[i].startAddress, freeBlock[i].size, freeBlock[i].state);
		printf("\t\t| --------------------------------------------\t|\n");
	}
}

// --- ����û������뵼�³�����ֹ ����0
int myShell() {
	int result = 1;
	int requestSize, startAddr;
	int releaseStartAddr, releaseSize;
	int i;
	char orderBuffer[1024] = {};
	adjustFreeTable();
	printFreeTable();
	printf("\n��������: a; �ͷ�����: f; �˳�����: e ; ����: else\n");
	scanf("%s", orderBuffer);
	getchar();
	if (*orderBuffer == 'a')
	{
		// printf("Initially the free memory is as below:\n");
		// adjustFreeTable();
		// printFreeTable();
		printf("input memory size requested:");
		scanf("%d", &requestSize);
		getchar();   /* ���ջس��� */

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
			//��ʱ
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

int main()
{
	char requestOrNot, ch;
	int i;
	int requestSize;
	int startAddr;
	int ret = 0;
	int releaseStartAddr, releaseSize;

	while (myShell());
	return ret;
}
/*
--------- ���㷨��Դ��z��ʦ��doc ���˶�����д����޸������������ ---------

�ġ��ϻ�˼����
1. ����ʾ���������Ӵ�����Ĵ��롣
	��������п��з����ǼǱ�����Ϊ5�����ʵ�����й����п��з������ȳ���5ʱ�Ĵ�����
	{Ŀǰ��ֱ�ӷ��������ı��� -> ��ֹ}
	��������û��ֹ��������������ʼ��ַ�ͷ�����Сʱ������ķ�������з����ǼǱ��еķ�����ì��ʱ�Ĵ�����ȡ�
	{
		�����ڴ����->����������;
		�ͷ�ʱ: 
		let �ڴ��Ǳ������еĿ� <==> start��ַ��ͬ
		if ����{
			�ϲ�
		}else{
			let Ŀ��: ��һ���Ϸ���state==0�Ŀ��Ϊ�����ͷŵĿ��п�
			let ��Ϊ1 : ���úϷ����Ϊ�ͷ�����Ŀ�
			if ���������������{
				��Ϊ1
			}else{
				��ֹ
			}
		}
	}
2. ʵ���Ѷ�2�ı�̡����ݽṹ����Ҫ����һ���ڴ�ռ��˵��������������û�ѡ�������������������ղ�����
	��������ǣ�����������䣬������ҵ�����̣�������������ռ��С�����ǻ��գ����������ҵ�����̣�����ҵ�ţ����̺ţ���
	��ѭ�������������ͻ��ա�
	Ҫ�󻭳��������ͻ��յ��㷨����ͼ�������ʵ�֡�
3. �������ݽṹ�ɷ������Ϊ����˫�������Ի����������ͻ����㷨����ͼ�������ʵ�֡�
4. �����������ʱ���������Ӧ�������״���Ӧ�����Ա��ʵ�֡�
5. ��Ͻ��̿��Ƽ����̻���ͬ������ģ��ʵ�ֶ������ͬʱ��������ı�̣�Ҳ��Ҫ���ǵ��������ͬʱ�ͷ�ռ����������Ρ�

*/

