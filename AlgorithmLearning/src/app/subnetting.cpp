#include "ExtendSpace.h"
#include "./BigInteger.h"

class SubnetIPV4 {
public:
	enum IpType{ A_IP, B_IP, C_IP , UNDEFINED_IP_TYPE};
	// 传入每个子网的用户(主机)数要求
	// 返回子网划分时[子网最小的二进制位数]; 主机位数
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

	// 返回子网掩码左侧的1个数
	static int calcSubnetMask(int hostBitsNumNew) {
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
		printf("子网掩码左1个数: %d; 点分十进制表示: ", count1L);
		dotPrint(subnetMaskDeTopLow, subnetMaskDeTopLow + GROUP_IP);
		puts("");
		return count1L;
	}

	// C类网络子网划分
	// @see: 计算机网络课件 > 网络层PPT > P120 > C类地址的子网划分举例
	// (点分10进制ip, 子网二进制位数, 新主机二进制位数)
	static void subnetting(int subnetBitsNumNew, int hostBitsNumNew) {
		// 每个值表示源进制ip地址中的 DOTTED_QUAD_BITS bit
		int ip[GROUP_IP] = { 0 };
		std::cout << "输入点分十进制IP地址" << std::endl;
		for (int i = 0; i < GROUP_IP; ++i) {
			std::cin >> ip[i];
			getchar();
		}

		// 原主机二进制位数
		int hostBitsNumOld = -1;
		// (ABC类分别为24 16 8)
		switch (judgeIpAddress(ip))
		{
		case A_IP: {hostBitsNumOld = 24; break; }
		case B_IP: {hostBitsNumOld = 16; break; }
		case C_IP: {hostBitsNumOld = 8; break; }
		default:
			break;
		}

		_ASSERT_EXPR(subnetBitsNumNew + hostBitsNumNew <= hostBitsNumOld, "无法满足分配需求!");
		int count1L = calcSubnetMask(hostBitsNumNew);
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
			dotPrint(ip, ip + GROUP_IP);
			printf("/%d\n", count1L);
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

int main() {
	// 不希望出现强制double转int的话就使用
	// 四舍五入 向上取整（ceil()） 向下取整（floor）
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