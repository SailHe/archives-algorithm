#include "../../nonlinear/ExpressionTree.h"

ExpressionTree::ExpressionTree(JCE::String express, ExpressionTree::ExpressionTypeEnum type_) {
	expression = express;
	type = type_;
}
bool ExpressionTree::check(ExpressionTree::ExpressionTypeEnum should, ExpressionTree::ExpressionTypeEnum willBe) {
	if (type != should) {
		return false;
	}
	else {
		if (should == ORDER_PREFIX_ROOT) {
			int i = 0, countNum = 0, countOp = 0;
			char num[10] = { 0 };
			do {
				if (getNum(&i, expression.c_str(), num) != '\0')
					++countOp;
				else
					++countNum;
			} while (expression[i++] != '\0');
			//操作符个数检查:正确的二元前缀表达式中，用空格间隔的操作数必然比操作符多一个
			if (countNum - countOp != 1) {
				type = ERROR_EXPRESSION;
				return false;
			}
			else {
				type = willBe;
				return true;
			}
		}
		else {
			_DEBUG_ERROR("后缀向前缀转换还没写");
			return false;
		}
	}
}

JCE::String ExpressionTree::nextOper() {
	static size_t ind = 0;
	string next;
	for (; ind < expression.size(); ind++) {
		if (expression[ind] != ' ')
			next += expression[ind];
		else
			break;
	}
	ind++;
	return next;
}

JCE::String ExpressionTree::prefixToPost() {
	string next = nextOper(), ans;
	if (!isdigit(next[0])) {
		Position node = nodeCreater(next);//递归构造树
		node->Left = nodeCreater(prefixToPost());
		node->Right = nodeCreater(prefixToPost());
		postTraversal(node, [&](BT const bT) {
			if (ans != "")
				ans += " " + bT->Data;
			else
				ans = bT->Data;
		});//后序遍历转换
		delete node->Left;
		delete node->Right;
		delete node;
		return ans;
	}
	else
		return next;
}
JCE::String ExpressionTree::toPost() {
	if (type == PREFIX_EXPRESSION) {
		if (check(PREFIX_EXPRESSION, POST_EXPRESSION))
			return expression = prefixToPost();
		else
			return "0";
	}
	else {
		_DEBUG_ERROR("方法调用错误: 只能是前缀向后缀转换");
		return "0";
	}
}
double ExpressionTree::eval() {
	if (type == POST_EXPRESSION) {
		if (check(POST_EXPRESSION, POST_EXPRESSION))
			return evalPostfix();
		else
			return MAX_INT32;
	}
	else if (type == PREFIX_EXPRESSION) {
		toPost();
		return eval();
	}
	else if (type == ERROR_EXPRESSION)
		return MAX_INT32;
	else {
		_DEBUG_ERROR("无法直接计算中缀表达式");
		return MAX_INT32;
	}
}

char ExpressionTree::getNum(int *i, char const*ori, char *num) {
	int sub = 0;
	while (1) {
		if (ori[*i] == ' ' || ori[*i] == '\0') {
			num[sub] = '\0';
			if (sub == 1 && !isdigit(num[0]))
				return num[0];
			else
				return '\0';
		}
		else {
			num[sub++] = ori[(*i)++];
			continue;
		}
	}
}
double ExpressionTree::evalPostfix() {
	int i = 0;
	char c = 0;
	static char num[50] = { 0 };
	stack<double> ps;
	double po1, po2;
	do {
		if ((c = getNum(&i, expression.c_str(), num)) == '\0') {
			ps.push(atof(num));
		}
		else {
			if (ps.empty()) {//运算符号缺少 或者多余
				return MAX_INT32;
			}
			po1 = ps.top();
			ps.pop();
			if (ps.empty()) {
				return MAX_INT32;
			}
			po2 = ps.top(); ps.pop();
			switch (c) {
			case '+':ps.push(po2 + po1); break;
			case '-':ps.push(po2 - po1); break;
			case '*':ps.push(po2 * po1); break;
			case '/': {
				if (po1 - 0.0 == 0) {//除以0错误
					return MAX_INT32;
				}
				else
					ps.push(po2 / po1); break;
			}
			default:break;
			}
		}
	} while (expression[i++] != 0);
	double re = ps.top(); ps.pop();
	if (ps.empty()) {
		return re;
	}
	else {//运算数多余
		return MAX_INT32;
	}
}
