#pragma once
#include "TreeObject.h"

template class BinTree<JCE::String>;

//表达式树 @TODO 并未通过PTA测试(主要是后缀表达式计算时的负号问题 考虑添加前缀表达式计算)
class DSAUTILITYEXTENSION_API ExpressionTree : private BinTree<JCE::String> {
public:
	enum ExpressionTypeEnum {
		PREFIX_EXPRESSION,//前缀表达式
		INFIX_EXPRESSION,//中表达式
		POST_EXPRESSION,//后缀表达式
		ERROR_EXPRESSION//错误的表达式
	};
	//中缀表达式构建的表达式树不唯一
	ExpressionTree(JCE::String express, ExpressionTypeEnum type_);
	//类型检查 检查完成后将类型置为willBe?
	bool check(ExpressionTypeEnum should, ExpressionTypeEnum willBe);
	//返回下一个运算符号
	JCE::String nextOper();
	/*
	1. 从左往右扫描串
	　2. 遇到操作符则递归构造树节点，当前操作符是根节点，并递归构造左右子节点
	 　3. 后序遍历当前结果，并返回
	  */
	  //表达式转换  不允许不检查直接调用
	JCE::String prefixToPost();
	//transform
	JCE::String toPost();
	double eval();
	/*后缀表达式计算*/

	/*如果两个空格之间是操作数则返回该操作 否则将数字存于num中并返回'\0'*/
	char getNum(int *i, char const*ori, char *num);
	//返回后缀表达式ans的值 不合法返回MAX_INT32
	double evalPostfix();
private:
	//BinTree base;
	JCE::String expression;
	//表达式当前类型
	ExpressionTypeEnum type;
};
