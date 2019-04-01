#pragma once
#include "TreeObject.h"

template class BinTree<JCE::String>;

//���ʽ�� @TODO ��δͨ��PTA����(��Ҫ�Ǻ�׺���ʽ����ʱ�ĸ������� �������ǰ׺���ʽ����)
class DSAUTILITYEXTENSION_API ExpressionTree : private BinTree<JCE::String> {
public:
	enum ExpressionTypeEnum {
		PREFIX_EXPRESSION,//ǰ׺���ʽ
		INFIX_EXPRESSION,//�б��ʽ
		POST_EXPRESSION,//��׺���ʽ
		ERROR_EXPRESSION//����ı��ʽ
	};
	//��׺���ʽ�����ı��ʽ����Ψһ
	ExpressionTree(JCE::String express, ExpressionTypeEnum type_);
	//���ͼ�� �����ɺ�������ΪwillBe?
	bool check(ExpressionTypeEnum should, ExpressionTypeEnum willBe);
	//������һ���������
	JCE::String nextOper();
	/*
	1. ��������ɨ�贮
	��2. ������������ݹ鹹�����ڵ㣬��ǰ�������Ǹ��ڵ㣬���ݹ鹹�������ӽڵ�
	 ��3. ���������ǰ�����������
	  */
	  //���ʽת��  ���������ֱ�ӵ���
	JCE::String prefixToPost();
	//transform
	JCE::String toPost();
	double eval();
	/*��׺���ʽ����*/

	/*��������ո�֮���ǲ������򷵻ظò��� �������ִ���num�в�����'\0'*/
	char getNum(int *i, char const*ori, char *num);
	//���غ�׺���ʽans��ֵ ���Ϸ�����MAX_INT32
	double evalPostfix();
private:
	//BinTree base;
	JCE::String expression;
	//���ʽ��ǰ����
	ExpressionTypeEnum type;
};
