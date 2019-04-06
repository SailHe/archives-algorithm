#include "CompleteBinTree.h"

namespace BinTreeUtil{
	void calcPostOrder(Tree::string const &sPre, Tree::string const &sMed, Tree::string &sPostBuffer) {
		//中序中的根所在位置
		Tree::string::size_type medRootSub = sMed.find(sPre[0]);
		//左子树的长度
		Tree::string::size_type leftSubLen = medRootSub;
		/*
		A B CD	前序(Root Left Right)
		B A DC	中序(Left Root Right)

		BDC A	后序(Left Right Root)
		*/

		//若存在左子树则向左子树递归
		if (leftSubLen > 0) {
			//先序第一个值(root_)右边取左子树的长度即是左子树
			Tree::string sPreLeftSub = sPre.substr(1, leftSubLen);
			//root_的左边
			Tree::string sMedLeftSub = sMed.substr(0, leftSubLen);
			calcPostOrder(sPreLeftSub, sMedLeftSub, sPostBuffer);
		}
		else {
			//do nothing
		}

		//若存在右子树则向右子树递归
		if (medRootSub + 1 < sMed.size()) {
			//先序第一个值(root_)的右边跳过左子树长度取所有即是右子树
			Tree::string sPreRightSub = sPre.substr(1 + leftSubLen);
			//root_的右边
			Tree::string sMedRightSub = sMed.substr(medRootSub + 1);
			calcPostOrder(sPreRightSub, sMedRightSub, sPostBuffer);
		}
		else {
			//do nothing
		}

		if (medRootSub != Tree::string::npos) {
			//加上root_
			sPostBuffer += sMed.at(medRootSub);
		}
		else {
			//do nothing
		}
	}

}


