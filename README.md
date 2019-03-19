# archives-algorithm

算法归档, 主要是C/CPP  含有VS2017 的配置文件
---
该解决方案下有多个子项目，其中:
1. AlgorithmLearning 用于平时学习，包括oj的算法练习，以及一些未完成的算法或其它程序
2. 主项目archives-algorithm 用于测试 DSAUtilityExtension 生成的Dll库和算法的归档
3. DSAUtilityExtension 是脱胎于项目1和2的相对稳定, 具有一定复用性的程序('*'号表示只实现了局部或是实现得不完善)
    - 数据结构
        - 线性结构
             - 堆栈
             - 队列
             - 优先队列
             - 待列出...
        - 树
            - 二叉树
                - 二叉搜索树
                    - AVL树
                - Huffman树
                - 静态二叉搜索
                    - 堆
                        - 最大堆
                        - 最小堆
         - 图
             - 邻接矩阵图
             - 邻接表图
    - 算法
        - 实用算法(标准库拓展空间)
            - 待列出...
        - 数学
            - 待列出...
        - 图论
            - 待列出...
        - 其它
            - 背包算法
            - 回溯
            - 待列出...
    - 大数类*
        - 无限制加法
        - 无限制乘法
        - int32范围减法
        - int32范围除法
4. InterfaceForDotNet 是基于DSAUtilityExtension的C#接口


## PS: 
1. 若在X64下改动了DSAUtilityExtension中的内容, 但未见效果, 可能是此时引用的仍是X86下内容的原因
这通常发生在一个既有x86又有x64的dll被一个只有x86的程序引用的时候, 此时即使配置显示的是x64, 但实际当前项目不一定
(参考AlgorithmLearning引用DSAUtilityExtension)
2. 对于 模板; 结构体; 类 : #include"../../archives-algorithm/src/ExtendSpace.h" 是可行的
3. 配置后 #include"./ExtendSpace.h"
