#include "ExtendSpace.h"
#include "./else/BigInteger.h"

class SubnetIPV4 {
public:
	enum IpType{ A_IP_ENUM, B_IP_ENUM, C_IP_ENUM , UNDEFINED_IP_TYPE_ENUM};
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
			subnetMaskDeTopLow[i] = TransitionUtility::radixTopLowToDecimal(
				subnetMaskBinTopLow + i * DOTTED_QUAD_BITS, IP_RADIX, DOTTED_QUAD_BITS
			);
		}
		printf("子网掩码左1个数: %d; 点分十进制表示: ", count1L);
		dotPrint(subnetMaskDeTopLow, subnetMaskDeTopLow + GROUP_IP);
		puts("");
		return count1L;
	}

	// C类网络子网划分
	// (点分10进制ip, 子网二进制位数, 新主机二进制位数)
	static void subnetting(int subnetBitsNumNew, int hostBitsNumNew) {
		// 每个值表示源进制ip地址中的 DOTTED_QUAD_BITS bit
		int ip[GROUP_IP] = { 0 };
		std::cout << "输入点分十进制IP地址" << std::endl;
		for (int i = 0; i < GROUP_IP; ++i) {
			std::cin >> ip[i];
			getchar();
		}
		int subnetMask[GROUP_IP] = { 0 };
		std::cout << "输入点分十进制子网掩码" << std::endl;
		for (int i = 0; i < GROUP_IP; ++i) {
			std::cin >> subnetMask[i];
			getchar();
		}
		std::vector<int> subnetMaskBinTopLow(IP_BITS);
		// 点分10进制子网掩码转为二进制
		auto pastIt = subnetMaskBinTopLow.begin();
		for (int i = 0, bitsCnt = 0; bitsCnt < IP_BITS; ++i) {
			int bits = TransitionUtility::decimalToRadixLowTopBase(
				subnetMask[i]
				, subnetMaskBinTopLow.begin() + bitsCnt
				, IP_RADIX
			);
			// 忽略返回值 每次前进8bit
			bitsCnt += 8;
			std::reverse(pastIt, subnetMaskBinTopLow.begin() + bitsCnt);
			pastIt = subnetMaskBinTopLow.begin() + bitsCnt;
		}
		// 原主机二进制位数
		int hostBitsNumOld = IP_BITS - std::count_if(subnetMaskBinTopLow.begin(), subnetMaskBinTopLow.end(), [](int value) {
			return value == 1;
		});

		// (ABC类分别为24 16 8)
		/*switch (judgeIpAddress(ip))
		{
		case A_IP_ENUM: {hostBitsNumOld = 24; break; }
		case B_IP_ENUM: {hostBitsNumOld = 16; break; }
		case C_IP_ENUM: {hostBitsNumOld = 8; break; }
		default:
			break;
		}*/

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
		printf("每个子网[可分配]主机个数(HostAddress个数): %d\n", validSubnetHostNum);
		// 计算和输出每个子网号
		for (int i = 0; i < subnetNum; ++i) {
			dotPrint(ip, ip + GROUP_IP);
			printf("/%d\n", count1L);
			// BUG: 此处决定了只能计算 限于最末8bit的子网划分
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


int main__() {
	// 不希望出现强制double转int的话就使用
	// 四舍五入 向上取整ceil() 向下取整floor() 然后强制转换...
	int subnetBits, hostBits;

	/*
	192.168.10.0
	255.255.255.0
	@see: 计算机网络课件 > 网络层PPT > P120 > C类地址的子网划分举例
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
	上面算出的每个网络的地址数为64=62+2 平分的话不用计算无效地址
	故一半应有为32个地址 除去每个子网的特殊地址为30个
	10.80.103.128
	255.255.255.192
	*/
	int halfHost[] = { 30, 30 };
	SubnetIPV4::calcSubnetBitsAndHostBits(halfHost, halfHost + 2, subnetBits, hostBits);
	SubnetIPV4::subnetting(subnetBits, hostBits);
	return 0;
}
