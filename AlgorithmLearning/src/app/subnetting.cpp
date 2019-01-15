#include "ExtendSpace.h"
#include "./BigInteger.h"

class SubnetIPV4 {
public:
	enum IpType{ A_IP, B_IP, C_IP , UNDEFINED_IP_TYPE};
	// 传入每个子网的用户(主机)数要求
	// 返回子网划分时[子网最小的二进制位数]; 主机位数
	template<class Iterator, class Integer>
	static void calcSubnetBitsAndHostBits(Iterator left, Iterator right, Integer &subnetBits, Integer &hostBits) {
		subnetBits = right - left;
		int maxSize = StandardExtend::maxValueStatistics(left, right, MIN_INT32);
		maxSize += invalidHostAddressNum;
		// sqrt(maxSize)
		hostBits = MathExtend::logRadix(maxSize, IP_RADIX);
	}
	
	// 判断IP地址类型 (点分10进制格式)
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

	// C类网络子网划分
	// @see: 计算机网络课件 > 网络层PPT > P120 > C类地址的子网划分举例
	void subnetting() {
		// 每个值表示源进制ip地址中的 DOTTED_QUAD_BITS bit
		int ip[GROUP_IP] = { 0 };
		// 子网位数 新主机位数 原主机位数
		int subnetBitsNumNew, hostBitsNumNew, hostBitsNumOld;
		std::cout << "子网二进制位数 新主机二进制位数 原主机二进制位数(ABC类分别为24 16 8)" << std::endl;
		std::cin >> subnetBitsNumNew >> hostBitsNumNew >> hostBitsNumOld;
		std::cout << "输入点分十进制IP地址" << std::endl;
		for (int i = 0; i < GROUP_IP; ++i) {
			std::cin >> ip[i];
			getchar();
		}

		_ASSERT_EXPR(subnetBitsNumNew + hostBitsNumNew <= hostBitsNumOld, "无法满足分配需求!");
		// 子网掩码左侧1的个数
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
		// (约定: 谈到 [大小Size], [长度Len], [个数], [数目Num], [计数Count]等时, 如果没有特殊说明进制, 就是10进制)
		// 其中, 由于[长度]容易和[二进制的位数]搞混 故只在字符串的处理中使用长度一词, 可考虑使用[大小]或[数目]替换

		// 子网个数 最大子网个数
		int subnetNum = (int)pow(IP_RADIX, subnetBitsNumNew);
		// 每个子网的大小(每个子网的主机地址总数)
		int subnetSize = (int)pow(IP_RADIX, hostBitsNumNew);
		int validHostAddressNum = subnetSize - invalidHostAddressNum;
		// 子网主机个数(约定: 其等价于可分配地址数目)
		int validSubnetHostNum = validHostAddressNum;
		printf("子网个数(subnetNum): %d\n", subnetNum);
		printf("每个子网可分配主机个数(HostAddress个数): %d\n", validSubnetHostNum);
		// 计算和输出每个子网号
		for (int i = 0; i < subnetNum; ++i) {
			for (int i1 = 0; i1 < GROUP_IP; ++i1) {
				std::cout << ip[i1];
				putchar((i1 == 0 ? '\0' : '.'));
			}
			puts("");
			// BUG: 此处决定了只能计算C类地址
			ip[GROUP_IP - 1] += subnetSize;
		}
	}
private:
	// IP地址的源进制
	static const int IP_RADIX = 2;
	// IP地址总位数
	static const int IP_BITS = 32;
	// 点分二进制位数
	static const int DOTTED_QUAD_BITS = 8;
	// 点分后的IP组数
	static const int GROUP_IP = IP_BITS / DOTTED_QUAD_BITS;
	// 无效的主机(每个网络中的特殊地址)个数
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