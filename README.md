# archives-algorithm

算法归档, 主要是C/CPP  含有VS2017 的配置文件
---
该解决方案下有多个子项目，其中:
1. AlgorithmLearning 用于平时学习，包括oj的算法练习，以及一些未完成的算法或其它程序
2. 主项目archives-algorithm 用于测试 DSAUtilityExtension 生成的Dll库和算法的归档
3. DSAUtilityExtension 是脱胎于项目1和2的相对稳定, 具有一定复用性的程序('*'号表示只实现了局部或是实现得不完善)
    - 数据结构
        - 线性结构
            - 链接线性表
                - 虚拟(VirtualList 使用数组作为虚拟内存构造链表)*
                - 单向链接表
                - 双向链接表(未实现)
            - 顺序线性表(动态增长数组 PS 实现的比较简单)*
            - 堆栈
                - 链接堆栈*
            - 队列
                - 普通队列(Queue)
                - 双端队列(Deque)
                - 优先队列(使用堆实现)
            - 多项式
        - 查找结构(不包含搜索树)
            - 跳表(未实现)*
            - 并查集
        - 树(一般情况下 子级表示继承关系)
            - 二叉树
                - 二叉搜索树
                    - AVL树
                - Huffman树
                    - 数组式(没有继承关系)
                    - 树式(继承于二叉树)
                - 表达式树(ExpressionTree)*
                    - 前缀
                    - 中缀
                    - 后缀
                - 静态二叉树
                    - 静态二叉搜索树(虚继承了静态二叉树, 二叉搜索树; PS 这个实现有点欠妥)
                    - 完全静态二叉搜索树
                        - 堆
                            - 最大堆
                            - 最小堆
         - 图(接口类; 无向 || 有向; 计权存边; 此外还有一个模板图 实现之后感觉没啥必要 完全可使用这个基本的替代)
             - 邻接矩阵图(std::vector+std::vector)
             - 邻接表图(std::vector+std::list)
         - 坐标地图类(等权存点 4方向 || 8 方向 || 拓展方向)
    - 大数相关(基本都支持string直接或间接参与运算)
        - 大数类*
            - 无限制加法
            - 无限制乘法
            - int32范围减法
            - int32范围除法
            - 乘数int32范围阶乘
            - 补码减法*
        - 进制转换器类(Transition 无限制大小)
            - 基于二进制的转换器
            - 基于十进制的转换器
    - 其它
        - Point相关
        - Sub(下标)相关
        - 三角形类
        - 分数类
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
4. InterfaceForDotNet 是基于DSAUtilityExtension的C#接口


## PS: 
1. 若在X64下改动了DSAUtilityExtension中的内容, 但未见效果, 可能是此时引用的仍是X86下内容的原因
这通常发生在一个既有x86又有x64的dll被一个只有x86的程序引用的时候, 此时即使配置显示的是x64, 但实际当前项目不一定
(参考AlgorithmLearning引用DSAUtilityExtension)
2. 对于 模板; 结构体; 类 : #include"../../archives-algorithm/src/ExtendSpace.h" 是可行的
3. 配置后 #include"./ExtendSpace.h"
