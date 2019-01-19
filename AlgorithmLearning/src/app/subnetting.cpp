#include "ExtendSpace.h"
#include "./else/BigInteger.h"

class SubnetIPV4 {
public:
	enum IpType{ A_IP_ENUM, B_IP_ENUM, C_IP_ENUM , UNDEFINED_IP_TYPE_ENUM};
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
		int rangeArray[] = { 0, 128, 192, MAX_INT32};
		IpType ipType = UNDEFINED_IP_TYPE_ENUM;
		switch (StandardExtend::judgeRange(ip[0], rangeArray, rangeArray + 4))
		{
		case 0: {ipType = A_IP_ENUM; break; }
		case 1: {ipType = B_IP_ENUM; break; }
		case 2: {ipType = C_IP_ENUM; break; }
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
			subnetMaskDeTopLow[i] = TransitionUtility::radixTopLowToDecimal(
				subnetMaskBinTopLow + i * DOTTED_QUAD_BITS, IP_RADIX, DOTTED_QUAD_BITS
			);
		}
		printf("����������1����: %d; ���ʮ���Ʊ�ʾ: ", count1L);
		dotPrint(subnetMaskDeTopLow, subnetMaskDeTopLow + GROUP_IP);
		puts("");
		return count1L;
	}

	// C��������������
	// (���10����ip, ����������λ��, ������������λ��)
	static void subnetting(int subnetBitsNumNew, int hostBitsNumNew) {
		// ÿ��ֵ��ʾԴ����ip��ַ�е� DOTTED_QUAD_BITS bit
		int ip[GROUP_IP] = { 0 };
		std::cout << "������ʮ����IP��ַ" << std::endl;
		for (int i = 0; i < GROUP_IP; ++i) {
			std::cin >> ip[i];
			getchar();
		}
		int subnetMask[GROUP_IP] = { 0 };
		std::cout << "������ʮ������������" << std::endl;
		for (int i = 0; i < GROUP_IP; ++i) {
			std::cin >> subnetMask[i];
			getchar();
		}
		std::vector<int> subnetMaskBinTopLow(IP_BITS);
		// ���10������������תΪ������
		auto pastIt = subnetMaskBinTopLow.begin();
		for (int i = 0, bitsCnt = 0; bitsCnt < IP_BITS; ++i) {
			int bits = TransitionUtility::decimalToRadixLowTopBase(
				subnetMask[i]
				, subnetMaskBinTopLow.begin() + bitsCnt
				, IP_RADIX
			);
			// ���Է���ֵ ÿ��ǰ��8bit
			bitsCnt += 8;
			std::reverse(pastIt, subnetMaskBinTopLow.begin() + bitsCnt);
			pastIt = subnetMaskBinTopLow.begin() + bitsCnt;
		}
		// ԭ����������λ��
		int hostBitsNumOld = IP_BITS - std::count_if(subnetMaskBinTopLow.begin(), subnetMaskBinTopLow.end(), [](int value) {
			return value == 1;
		});

		// (ABC��ֱ�Ϊ24 16 8)
		/*switch (judgeIpAddress(ip))
		{
		case A_IP_ENUM: {hostBitsNumOld = 24; break; }
		case B_IP_ENUM: {hostBitsNumOld = 16; break; }
		case C_IP_ENUM: {hostBitsNumOld = 8; break; }
		default:
			break;
		}*/

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
		printf("ÿ������[�ɷ���]��������(HostAddress����): %d\n", validSubnetHostNum);
		// ��������ÿ��������
		for (int i = 0; i < subnetNum; ++i) {
			dotPrint(ip, ip + GROUP_IP);
			printf("/%d\n", count1L);
			// BUG: �˴�������ֻ�ܼ��� ������ĩ8bit����������
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


int main__() {
	// ��ϣ������ǿ��doubleתint�Ļ���ʹ��
	// �������� ����ȡ��ceil() ����ȡ��floor() Ȼ��ǿ��ת��...
	int subnetBits, hostBits;

	/*
	192.168.10.0
	255.255.255.0
	@see: ���������μ� > �����PPT > P120 > C���ַ���������־���
	192.168.10.0 /27
	192.168.10.32 /27
	192.168.10.64 /27
	192.168.10.96 /27
	192.168.10.128 /27
	*/
	
	int hostUserC[] = { 7, 7, 13, 15, 16 };
	SubnetIPV4::calcSubnetBitsAndHostBits(hostUserC, hostUserC + 5, subnetBits, hostBits);
	// (3, 5)
	SubnetIPV4::subnetting(subnetBits, hostBits);
	/**/

	/*
	10.80.103.0
	255.0.0.0

	10.80.103.0/26
	10.80.103.64/26
	10.80.103.128/26
	10.80.103.192/26
	*/
	int hostUserA[] = { 7, 15, 27, 50 };
	SubnetIPV4::calcSubnetBitsAndHostBits(hostUserA, hostUserA + 4, subnetBits, hostBits);
	SubnetIPV4::subnetting(subnetBits, hostBits); // char const *dotDeIpStr
	/**/

	/*
	���������ÿ������ĵ�ַ��Ϊ64=62+2 ƽ�ֵĻ����ü�����Ч��ַ
	��һ��Ӧ��Ϊ32����ַ ��ȥÿ�������������ַΪ30��
	10.80.103.128
	255.255.255.192
	*/
	int halfHost[] = { 30, 30 };
	SubnetIPV4::calcSubnetBitsAndHostBits(halfHost, halfHost + 2, subnetBits, hostBits);
	SubnetIPV4::subnetting(subnetBits, hostBits);
	return 0;
}
