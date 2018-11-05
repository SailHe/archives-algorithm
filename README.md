# archives-algorithm

算法归档, 主要是C/CPP  含有VS2017 的配置文件
=====
该解决方案下有多个子项目，其中:
1. AlgorithmLearning 用于平时学习
2. 主项目archives-algorithm 用于测试 DSAUtilityExtension 生成的Dll库和算法的归档
3. DSAUtilityExtension 是脱胎于项目1和2的相对稳定, 具有一定复用性的程序
4. InterfaceForDotNet 是基于DSAUtilityExtension的C#接口

=
PS: 如果在X64下改动了DSAUtilityExtension中的内容, 但未见效果, 可能是此时引用的仍是X86下内容的原因
这通常发生在一个既有x86又有x64的dll被一个只有x86的程序引用的时候, 此时即使配置显示的是x64, 但实际当前项目不一定
(参考AlgorithmLearning引用DSAUtilityExtension)
