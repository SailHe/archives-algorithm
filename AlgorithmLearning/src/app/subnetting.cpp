#include "ExtendSpace.h"
#include "./BigInteger.h"

class SubnetIPV4 {
public:
	enum IpType{ A_IP, B_IP, C_IP , UNDEFINED_IP_TYPE};
	// ����ÿ���������û�(����)��Ҫ��
	// ������������ʱ[������С�Ķ�����λ��]; ����λ��
	template<class Iterator, class Integer>
	static void calcSubnetBitsAndHostBits(Iterator left, Iterator right, Integer &subnetBits, Integer &hostBits) {
		subnetBits = (int)ceil(MathExtend::logRadix(right - left, IP_RADIX));
		int maxSize = StandardExtend::maxValueStatistics(left, right, MIN_INT32);
		maxSize += invalidHostAddressNum;
		// sqrt(maxSize)
		hostBits = (int)ceil(MathExtend::logRadix(maxSize, IP_RADIX));
	}
	
	static void dotPrint(int *left, int *right) {
		for (; left != right; ++left) {
			std::cout << *left;
			putchar((left == right - 1 ? '\0' : '.'));
		}
	}

	// �ж�IP��ַ���� (���10���Ƹ�ʽ)
	static IpType judgeIpAddress(int ip[4]) {
		int rangeArray[] = { 0, 128, 192, MAXINT32};
		IpType ipType = UNDEFINED_IP_TYPE;
		switch (StandardExtend::judgeRange(ip[0], rangeArray, rangeArray + 4))
		{
		case 0: {ipType = A_IP; break; }
		case 1: {ipType = B_IP; break; }
		case 2: {ipType = C_IP; break; }
		default:
			break;
		}
		return ipType;
	}

	// ����������������1����
	static int calcSubnetMask(int hostBitsNumNew) {
		// �����������1�ĸ���
		int count1L = IP_BITS - hostBitsNumNew;
		int subnetMaskBinTopLow[IP_BITS] = {};
		for (int i = 0; i < count1L; ++i) {
			subnetMaskBinTopLow[i] = 1;
		}
		int subnetMaskDeTopLow[GROUP_IP] = {};
		for (int i = 0; i < GROUP_IP; ++i) {
			subnetMaskDeTopLow[i] = BinaryTransition::radixTopLowToDecimal(
				subnetMaskBinTopLow + i * DOTTED_QUAD_BITS, IP_RADIX, DOTTED_QUAD_BITS
			);
		}
		printf("����������1����: %d; ���ʮ���Ʊ�ʾ: ", count1L);
		dotPrint(subnetMaskDeTopLow, subnetMaskDeTopLow + GROUP_IP);
		puts("");
		return count1L;
	}

	// C��������������
	// @see: ���������μ� > �����PPT > P120 > C���ַ���������־���
	// (���10����ip, ����������λ��, ������������λ��)
	static void subnetting(int subnetBitsNumNew, int hostBitsNumNew) {
		// ÿ��ֵ��ʾԴ����ip��ַ�е� DOTTED_QUAD_BITS bit
		int ip[GROUP_IP] = { 0 };
		std::cout << "������ʮ����IP��ַ" << std::endl;
		for (int i = 0; i < GROUP_IP; ++i) {
			std::cin >> ip[i];
			getchar();
		}

		// ԭ����������λ��
		int hostBitsNumOld = -1;
		// (ABC��ֱ�Ϊ24 16 8)
		switch (judgeIpAddress(ip))
		{
		case A_IP: {hostBitsNumOld = 24; break; }
		case B_IP: {hostBitsNumOld = 16; break; }
		case C_IP: {hostBitsNumOld = 8; break; }
		default:
			break;
		}

		_ASSERT_EXPR(subnetBitsNumNew + hostBitsNumNew <= hostBitsNumOld, "�޷������������!");
		int count1L = calcSubnetMask(hostBitsNumNew);
		// (Լ��: ̸�� [��СSize], [����Len], [����], [��ĿNum], [����Count]��ʱ, ���û������˵������, ����10����)
		// ����, ����[����]���׺�[�����Ƶ�λ��]��� ��ֻ���ַ����Ĵ�����ʹ�ó���һ��, �ɿ���ʹ��[��С]��[��Ŀ]�滻

		// �������� �����������
		int subnetNum = (int)pow(IP_RADIX, subnetBitsNumNew);
		// ÿ�������Ĵ�С(ÿ��������������ַ����)
		int subnetSize = (int)pow(IP_RADIX, hostBitsNumNew);
		int validHostAddressNum = subnetSize - invalidHostAddressNum;
		// ������������(Լ��: ��ȼ��ڿɷ����ַ��Ŀ)
		int validSubnetHostNum = validHostAddressNum;
		printf("��������(subnetNum): %d\n", subnetNum);
		printf("ÿ�������ɷ�����������(HostAddress����): %d\n", validSubnetHostNum);
		// ��������ÿ��������
		for (int i = 0; i < subnetNum; ++i) {
			dotPrint(ip, ip + GROUP_IP);
			printf("/%d\n", count1L);
			// BUG: �˴�������ֻ�ܼ���C���ַ
			ip[GROUP_IP - 1] += subnetSize;
		}
	}
private:
	// IP��ַ��Դ����
	static const int IP_RADIX = 2;
	// IP��ַ��λ��
	static const int IP_BITS = 32;
	// ��ֶ�����λ��
	static const int DOTTED_QUAD_BITS = 8;
	// ��ֺ��IP����
	static const int GROUP_IP = IP_BITS / DOTTED_QUAD_BITS;
	// ��Ч������(ÿ�������е������ַ)����
	static const int invalidHostAddressNum = 2;
};

int main() {
	// ��ϣ������ǿ��doubleתint�Ļ���ʹ��
	// �������� ����ȡ����ceil()�� ����ȡ����floor��
	int subnetBits, hostBits;

	// 192.168.10.0
	/*
	192.168.10.0 /27
	192.168.10.32 /27
	192.168.10.64 /27
	192.168.10.96 /27
	192.168.10.128 /27
	*/
	/*
	int hostUser[] = { 7, 7, 13, 15, 16 };
	SubnetIPV4::calcSubnetBitsAndHostBits(hostUser, hostUser + 5, subnetBits, hostBits);
	// (3, 5)
	SubnetIPV4::subnetting(subnetBits, hostBits);
	*/
	// 10.80.103.0
	// 10.80.103.0/26, 10.80.103.64/26, 10.80.103.128/26, 10.80.103.192/26
	int hostUser[] = { 7, 15, 27, 50 };
	SubnetIPV4::calcSubnetBitsAndHostBits(hostUser, hostUser + 4, subnetBits, hostBits);
	SubnetIPV4::subnetting(subnetBits, hostBits); // char const *dotDeIpStr
	return 0;
}