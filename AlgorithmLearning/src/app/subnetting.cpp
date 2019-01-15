#include "ExtendSpace.h"
#include "./BigInteger.h"

class SubnetIPV4 {
public:
	enum IpType{ A_IP, B_IP, C_IP , UNDEFINED_IP_TYPE};
	// ����ÿ���������û�(����)��Ҫ��
	// ������������ʱ[������С�Ķ�����λ��]; ����λ��
	template<class Iterator, class Integer>
	static void calcSubnetBitsAndHostBits(Iterator left, Iterator right, Integer &subnetBits, Integer &hostBits) {
		subnetBits = right - left;
		int maxSize = StandardExtend::maxValueStatistics(left, right, MIN_INT32);
		maxSize += invalidHostAddressNum;
		// sqrt(maxSize)
		hostBits = MathExtend::logRadix(maxSize, IP_RADIX);
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

	// C��������������
	// @see: ���������μ� > �����PPT > P120 > C���ַ���������־���
	void subnetting() {
		// ÿ��ֵ��ʾԴ����ip��ַ�е� DOTTED_QUAD_BITS bit
		int ip[GROUP_IP] = { 0 };
		// ����λ�� ������λ�� ԭ����λ��
		int subnetBitsNumNew, hostBitsNumNew, hostBitsNumOld;
		std::cout << "����������λ�� ������������λ�� ԭ����������λ��(ABC��ֱ�Ϊ24 16 8)" << std::endl;
		std::cin >> subnetBitsNumNew >> hostBitsNumNew >> hostBitsNumOld;
		std::cout << "������ʮ����IP��ַ" << std::endl;
		for (int i = 0; i < GROUP_IP; ++i) {
			std::cin >> ip[i];
			getchar();
		}

		_ASSERT_EXPR(subnetBitsNumNew + hostBitsNumNew <= hostBitsNumOld, "�޷������������!");
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
			for (int i1 = 0; i1 < GROUP_IP; ++i1) {
				std::cout << ip[i1];
				putchar((i1 == 0 ? '\0' : '.'));
			}
			puts("");
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

/*
3 5 8
192.168.10.0
*/
int main() {
	int ip[] = { 192,168,10,0 };
	printf("%d", SubnetIPV4::judgeIpAddress(ip));
	return 0;
}